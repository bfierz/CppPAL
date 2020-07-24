project(CppPAL)
cmake_minimum_required(VERSION 3.8)

# Define C++ standard, minimum requirement is C++14
# As MSVC is not able to define the minimum level, software needs
# to implement per feature detection
set(CPPPAL_CXX_STANDARD "14" CACHE STRING "C++ standard")
set_property(CACHE CPPPAL_CXX_STANDARD PROPERTY STRINGS "14" "17" "20")
message(STATUS "Using C++${CPPPAL_CXX_STANDARD}")

# Select C++ version
set(CMAKE_CXX_STANDARD ${CPPPAL_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Abseil for general type support
# any, optional, span, string_view
add_subdirectory(external/abseil EXCLUDE_FROM_ALL)

# C++20 formatting support
add_subdirectory(external/fmt EXCLUDE_FROM_ALL)

# C++23 expected proposal
add_subdirectory(external/expected-lite EXCLUDE_FROM_ALL)

add_library(CppPAL INTERFACE)
target_include_directories(CppPAL INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(CppPAL INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cpppal/any.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cpppal/compiler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cpppal/expected.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cpppal/optional.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cpppal/span.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cpppal/string_view.h"
)
target_link_libraries(CppPAL INTERFACE absl::any absl::bad_any_cast absl::container absl::optional nonstd::expected-lite)

# Only build tests when building as standalone project
if("^${CMAKE_SOURCE_DIR}$" STREQUAL "^${PROJECT_SOURCE_DIR}$")
	add_subdirectory(external/catch2 EXCLUDE_FROM_ALL)
	add_subdirectory(tests)
endif()
