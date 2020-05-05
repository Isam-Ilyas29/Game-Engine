function(unzip ARCHIVE SUBDIR)
	if(NOT EXISTS "${SUBDIR}/${ARCHIVE}")
		message(FATAL_ERROR "ERROR::CMAKE::NO_ARCHIVE_SPECIFIED\n${SUBDIR}/${ARCHIVE}")
	endif()
	
	message(STATUS "Extracting ${ARCHIVE}...")
	
	execute_process(COMMAND 
		${CMAKE_COMMAND} -E tar -xf "${ARCHIVE}"
		WORKING_DIRECTORY "${SUBDIR}"
	)
endfunction()