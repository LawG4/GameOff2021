cmake_minimum_required(VERSION 3.20)

# Set a list of texture maps
set(TEXTURE_LIST
	TempHopper.png
	rest.png
	MenuStart.png
	MenuStartDark.png
	floor.png
	network_box.png
	server_control.png
	server_front.png
	server_rack_rear.png
)

# For each texture listed add a custom command that copies it to the binary directory
foreach(texture ${TEXTURE_LIST})

	add_custom_command(
		OUTPUT ${PROJ_ROOT}/bin/$<CONFIG>/Textures/${texture}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJ_ROOT}/bin/$<CONFIG>/Textures
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_CURRENT_LIST_DIR}/${texture}
			${PROJ_ROOT}/bin/$<CONFIG>/Textures/${texture}
		DEPENDS ${texture}
		COMMENT "Copying Texture : ${texture}"
		)
	
endforeach()

# Create a custom target
add_custom_target(Texture_Target ALL DEPENDS ${TEXTURE_LIST})
add_dependencies(${PROJ_NAME} Texture_Target)