include(${CMAKE_CURRENT_LIST_DIR}/win-vs-toolchain.cmake)
setup_vs_env(arm 14.3)

set(CMAKE_C_COMPILER cl.exe)
set(CMAKE_CXX_COMPILER cl.exe)
