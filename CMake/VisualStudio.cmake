cmake_minimum_required(VERSION 3.20)
# Visual studio needs some extra commands to make everything all nice

# Set the default target to be the game
set_property(DIRECTORY ${PROJ_ROOT} PROPERTY VS_STARTUP_PROJECT ${PROJ_NAME})

# Get all of the source files added to the project
get_target_property(PROJ_SOURCES ${PROJ_NAME} SOURCES)

# Add a little function that sorts all of the files to be
# Yoinked from https://stackoverflow.com/a/31423421
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

assign_source_group(${PROJ_SOURCES})
