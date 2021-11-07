cmake_minimum_required(VERSION 3.20)
# Compile a list of shaders to be compiled and copied by the cmake system

set(SHADER_FILES
	${CMAKE_CURRENT_LIST_DIR}/TestShader.vert
	${CMAKE_CURRENT_LIST_DIR}/TestShader.frag
	CACHE STRING "List of GLSL shaders" FORCE
)
