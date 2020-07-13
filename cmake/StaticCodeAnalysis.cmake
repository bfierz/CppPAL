# Travers all targets and collect the include paths
function(cpppal_get_include_paths OUTPUT_LIST TARGET)
	list(APPEND VISITED_TARGETS "${TARGET}")

	# Determine target type. IMPORTED and INTERFACE_LIBRARY only support
	# a restricted interface.
	get_target_property(IMPORTED "${TARGET}" IMPORTED)
	get_target_property(TYPE "${TARGET}" TYPE)

	# Query the possible links for the recursive search
	if (IMPORTED OR TYPE STREQUAL "INTERFACE_LIBRARY")
		get_target_property(TARGETS "${TARGET}" INTERFACE_LINK_LIBRARIES)
	else()
		get_target_property(TARGETS "${TARGET}" LINK_LIBRARIES)
	endif()

	set(INCLUDE_PATHS "")
	foreach(TGT ${TARGETS})
		if (TARGET ${TGT})
			list(FIND VISITED_TARGETS ${TGT} VISITED)
			if (${VISITED} EQUAL -1)
				get_target_property(TGT_INC_PATHS ${TGT} INTERFACE_INCLUDE_DIRECTORIES)
				foreach(PATH ${TGT_INC_PATHS})
					string(FIND ${PATH} ${PROJECT_SOURCE_DIR} FOUND)
					if (EXISTS ${PATH} AND ${FOUND} EQUAL -1)
						list(APPEND INCLUDE_PATHS ${PATH} ${DEPENDENCIES_INC_PATHS})
					endif()
				endforeach()
				cpppal_get_include_paths(DEPENDENCIES_INC_PATHS ${TGT})
			endif()
		endif()
	endforeach()
	set(VISITED_TARGETS ${VISITED_TARGETS} PARENT_SCOPE)
	list(REMOVE_DUPLICATES INCLUDE_PATHS)
	set(${OUTPUT_LIST} ${INCLUDE_PATHS} PARENT_SCOPE)
endfunction()

# Enable clang-tidy checking for given target
function(cpppal_enable_clang_tidy TARGET)
	if (${CMAKE_VERSION} VERSION_LESS "3.6.0") 
		message(ERROR "Clang-tidy integration requires at least CMake 3.6.0")
	endif()
	
	# Find clang-tidy
	if(NOT CLANG_TIDY_EXE)
		get_filename_component(COMP_DIRECTORY "${CMAKE_CXX_COMPILER}" DIRECTORY)
		find_program(
			CLANG_TIDY_EXE
			NAMES "clang-tidy"
			DOC "Path to clang-tidy executable"
			PATHS ${COMP_DIRECTORY}
		)
		if(NOT CLANG_TIDY_EXE)
			message(STATUS "clang-tidy not found.")
		else()
			message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
		endif()
	endif()
	
	if(CLANG_TIDY_EXE)
		message(STATUS "Enable clang-tidy on ${TARGET}")
		set_target_properties(
			${TARGET} PROPERTIES
			CXX_CLANG_TIDY "${CLANG_TIDY_EXE}"
		)
	endif()
endfunction()

# Enable Visual Studio core guideline checking for given target
function(cpppal_enable_core_guideline_checker TARGET RULESET_FILE)
	message(STATUS "Enable core guideline checker on ${TARGET}")
	cpppal_get_include_paths(TARGET_INCLUDE_PATHS ${TARGET})

	if (MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.13)
		# Exclude external headers from static code analysis. According to a remark in
		# https://blogs.msdn.microsoft.com/vcblog/2017/12/13/broken-warnings-theory/
		# only the environment variabl 'CAExcludePath' seems to work.
		target_compile_options(${TARGET} PRIVATE "/experimental:external" "/external:env:CAExcludePath")
	endif()

	set_target_properties(${TARGET} PROPERTIES
		VS_GLOBAL_EnableCppCoreCheck true
		VS_GLOBAL_CodeAnalysisRuleSet "${RULESET_FILE}"
		VS_GLOBAL_CAExcludePath "${TARGET_INCLUDE_PATHS}"
		VS_GLOBAL_RunCodeAnalysis true)
endfunction()
