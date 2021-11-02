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

# Vulkan 
# We want to include Vulkan as a dynamically loaded module
# This is because we want to use GLFW to init Vulkan headers 
find_package(Vulkan REQUIRED FATAL_ERROR)
target_include_directories(${PROJ_NAME} PRIVATE ${Vulkan_INCLUDE_DIR})