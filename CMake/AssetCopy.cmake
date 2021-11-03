cmake_minimum_required(VERSION 3.20)

# creates a function that allows asset copying to the working directory
function(AddAsset PathToAsset CopyPath)
	# Add the asset to the build system or else it won't be tracked
	target_sources(${PROJ_NAME} PRIVATE ${PathToAsset})

	# Add a custom target to copy the asset
	add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		"${PathToAsset}"
		"$<TARGET_FILE_DIR:${PROJ_NAME}>/${CopyPath}"
		DEPENDS "${PathToAsset}"
		COMMENT "Copying Asset ${PathToAsset}"
		)
endfunction()