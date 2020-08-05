# Setup the Visual Studio environment for CMake
macro(setup_vs_env ARCHITECTURE TOOLSET)
  if("${TOOLSET}" STREQUAL "")
    set(TOOLSET_PARAM "")
  else()
    set(TOOLSET_PARAM "-toolset ${TOOLSET}")
  endif()

  execute_process(
    # Set up Visual Studio build environment and parse as key-value pairs
    COMMAND powershell -Command "${CMAKE_CURRENT_LIST_DIR}/../Setup-VSEnv.ps1 -architecture ${ARCHITECTURE} ${TOOLSET_PARAM};Get-ChildItem env:* | % { \"$($_.Name)=$($_.Value)\" }"
    OUTPUT_VARIABLE ENV_VARS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # Convert multi-line string to list.
  # ';' is used as list item separator.
  string(REPLACE ";" "," ENV_VARS "${ENV_VARS}")
  string(REPLACE "\n" ";" ENV_VARS ${ENV_VARS})

  foreach(KEY_VALUE ${ENV_VARS})
    # Strip leading spaces
    string(REGEX REPLACE "^[ ]+" "" KEY_VALUE ${KEY_VALUE})
    # Find variable name
    string(REGEX MATCH "^[^=]+" NAME ${KEY_VALUE})
    # Find the value
    string(REPLACE "${NAME}=" "" VALUE ${KEY_VALUE})
    # Restore item separator ',' with ';'
    string(REPLACE "," ";" VALUE ${VALUE})
    # Set the variable
    set(ENV{${NAME}} "${VALUE}")
  endforeach()
endmacro()
