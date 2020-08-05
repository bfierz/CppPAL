include(${CMAKE_CURRENT_LIST_DIR}/win-vs-toolchain.cmake)
setup_vs_env(amd64 "")

# Search for clang-cl
execute_process(
  # Query registry for LLVM installation
  COMMAND powershell -Command "(Get-ItemProperty -path HKLM:\\SOFTWARE\\WOW6432Node\\LLVM\\LLVM).\"(default)\""
  OUTPUT_VARIABLE LLVM_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REPLACE "\\" "/" LLVM_PATH "${LLVM_PATH}")
message(STATUS "Reported LLVM installation at ${LLVM_PATH}")

find_program(CLANGCL_PATH
  clang-cl.exe
  HINTS
    ${LLVM_PATH}/bin
)

if(NOT EXISTS "${CLANGCL_PATH}")
  message(FATAL_ERROR "Did not find clang-cl")
endif()

set(ENV{CFLAGS} "-m64")
set(ENV{CXXFLAGS} "-m64")
set(CMAKE_C_COMPILER ${CLANGCL_PATH})
set(CMAKE_CXX_COMPILER ${CLANGCL_PATH})
