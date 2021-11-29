cmake_minimum_required(VERSION 3.20)
# Ensure that all of the github submodules get loaded 

# GLFW
# Check if the directory exists
if(NOT EXISTS ${PROJ_ROOT}/External/glfw)
    message(FATAL_ERROR "GLFW not found, ensure you have downloaded all submodules")
endif()
# Add the subdirectory
add_subdirectory(${PROJ_ROOT}/External/glfw)
# Link to the project
target_link_libraries(${PROJ_NAME} glfw)

# sspdlog
# Check if the directory exists 
if(NOT EXISTS ${PROJ_ROOT}/External/spdlog)
    message(FATAL_ERROR "Spdlog not found, ensure you have downloaded all submodules")
endif()
# Add the subdirectory
add_subdirectory(${PROJ_ROOT}/External/spdlog)
# Link to the project 
target_link_libraries(${PROJ_NAME} spdlog)

# GLM
# Add the subdirectory
add_subdirectory(${PROJ_ROOT}/External/glm)
# Link to the project 
target_link_libraries(${PROJ_NAME} glm)

# Json parser
# Add the subdirectory
add_subdirectory(${PROJ_ROOT}/External/json)
# Link to the project 
target_link_libraries(${PROJ_NAME} nlohmann_json)

#stb image parser, we only want the image parser so it's not a full submodule
target_include_directories(${PROJ_NAME} PRIVATE ${PROJ_ROOT}/External/stb)

# Vulkan 
# We want to include Vulkan as a dynamically loaded module
# This is because we want to use GLFW to init Vulkan headers 
find_package(Vulkan REQUIRED FATAL_ERROR)
target_include_directories(${PROJ_NAME} PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${Vulkan_LIBRARIES})
set(GLSLangValidatior ${Vulkan_GLSLC_EXECUTABLE} CACHE STRING "Location og GLSLangValidator")

# =========================================================================================
# Add in External irrKlang project

# Test if irrKlang has been downloaded
if(EXISTS "${PROJ_ROOT}/External/irrKlang/include")
    message(STATUS "Found irrKlang include directory")
else()
    message(FATAL_ERROR "Could not find irrKlang include dir, ensure you downloaded it 1.6 : https://www.ambiera.com/irrklang/downloads.html")
endif()

# Add the irrKlang include directories 
target_include_directories(${PROJ_NAME} PRIVATE "${PROJ_ROOT}/External/irrKlang/include")

# Get the name of the library, although I don't know if a linux version exists
if(WIN32)
    set(lib_irrKLANG_PREFIX "${PROJ_ROOT}/External/irrKlang/bin/Winx64-visualStudio")
    set(lib_irrKLANG "${lib_irrKLANG_PREFIX}/irrKlang.dll"
        "${lib_irrKLANG_PREFIX}/ikpMP3.dll"
        "${lib_irrKLANG_PREFIX}/ikpFlac.dll")
endif()

if(LINUX)
    set(lib_irrKLANG_PREFIX "${PROJ_ROOT}/External/irrKlang/bin/linux-gcc-64")
    set(lib_irrKLANG "${lib_irrKLANG_PREFIX}/irrKlang.so"
        "${lib_irrKLANG_PREFIX}/ikpMP3.so"
        "${lib_irrKLANG_PREFIX}/ikpFlac.so")
endif()

# For each dynamic library, make a build rule that copies it to the bin directory
foreach(lib ${lib_irrKLANG})

    # Find the name of the lib
    get_filename_component(lib_OUT ${lib} NAME)

    # Track all the output libs
    set(LIBS_LIST_OUT ${LIBS_LIST_OUT} ${lib_OUT})

    # Add the custom command that copies this library
    add_custom_command(
        OUTPUT ${PROJ_ROOT}/bin/$<CONFIG>/${lib_OUT}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${lib}"
            "${PROJ_ROOT}/bin/$<CONFIG>/${lib_OUT}"
        DEPENDS ${lib}
        COMMENT "Copying Dynamic Library : ${lib_OUT}"
    )
endforeach()

# Add a custom target
add_custom_target(Library_Target ALL DEPENDS ${LIBS_LIST_OUT})
add_dependencies(${PROJ_NAME} Library_Target)

# Link to the shared librarys that got copied over
# Okay and I think for some reason windows needs the static lib too
if(WIN32)
    target_link_libraries(${PROJ_NAME} "${PROJ_ROOT}/External/irrKlang/lib/Winx64-visualStudio/irrKlang.lib")
else()
    target_link_libraries(${PROJ_NAME} ${LIBS_LIST_OUT})
endif()



