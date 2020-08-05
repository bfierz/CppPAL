include(${CMAKE_CURRENT_LIST_DIR}/win-vs-toolchain.cmake)
setup_vs_env(arm64 14.1)

set(CMAKE_C_COMPILER cl.exe)
set(CMAKE_CXX_COMPILER cl.exe)
