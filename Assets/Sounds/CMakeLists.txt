cmake_minimum_required(VERSION 3.20)

# Set a list of sound files
set(SOUND_LIST
	chill.mp3
)

# For each sound file add a custom command that copies it to the binary directory
foreach(sound ${SOUND_LIST})

	add_custom_command(
		OUTPUT ${PROJ_ROOT}/bin/$<CONFIG>/Sounds/${sound}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJ_ROOT}/bin/$<CONFIG>/Sounds
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_CURRENT_LIST_DIR}/${sound}
			${PROJ_ROOT}/bin/$<CONFIG>/Sounds/${sound}
		DEPENDS ${sound}
		COMMENT "Copying sounds: ${sound}"
		)
	
endforeach()

# Create a custom target
add_custom_target(Sound_Target ALL DEPENDS ${SOUND_LIST})
add_dependencies(${PROJ_NAME} Sound_Target)