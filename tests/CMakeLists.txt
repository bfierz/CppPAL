include(../cmake/StaticCodeAnalysis.cmake)

string(REGEX REPLACE "([0-9]+)\\.[0-9]+(\\.[0-9]+)?(\\.[0-9]+)?" "\\1" COMPILER_VERSION_MAJOR ${CMAKE_CXX_COMPILER_VERSION})
string(REGEX REPLACE "[0-9]+\\.([0-9]+)(\\.[0-9]+)?(\\.[0-9]+)?" "\\1" COMPILER_VERSION_MINOR ${CMAKE_CXX_COMPILER_VERSION})
if (${CMAKE_CXX_COMPILER_VERSION} MATCHES "[0-9]+\\.[0-9]+\\.[0-9]+(\\.[0-9]+)?")
	string(REGEX REPLACE "[0-9]+\\.[0-9]+\\.([0-9]+)(\\.[0-9]+)?" "\\1" COMPILER_VERSION_PATCHLEVEL ${CMAKE_CXX_COMPILER_VERSION})
else()
	set(COMPILER_VERSION_PATCHLEVEL 0)
endif()
message(STATUS "${CMAKE_CXX_COMPILER_ID}: ${COMPILER_VERSION_MAJOR}.${COMPILER_VERSION_MINOR}.${COMPILER_VERSION_PATCHLEVEL}")

add_library(platform_macros STATIC
	platform_macros.cpp
)

math(EXPR COMPILER_VERSION "100*${COMPILER_VERSION_MAJOR}+${COMPILER_VERSION_MINOR}")
target_compile_definitions(platform_macros PUBLIC CPPPAL_TEST_COMP_VERSION=${COMPILER_VERSION})

target_compile_definitions(platform_macros PUBLIC CPPPAL_TEST_CXX=${CPPPAL_CXX_STANDARD})

target_link_libraries(platform_macros CppPAL)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	cpppal_enable_clang_tidy(platform_macros)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" AND ${CMAKE_GENERATOR} MATCHES "Visual Studio")
	cpppal_enable_core_guideline_checker(platform_macros "")
endif()
