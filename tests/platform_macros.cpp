#include <cpppal/compiler.h>

#ifdef CPPPAL_COMPILER_ICC
#    define CPPPAL_COMPILER CPPPAL_COMPILER_ICC
#endif
#ifdef CPPPAL_COMPILER_MSVC
#    define CPPPAL_COMPILER CPPPAL_COMPILER_MSVC
#endif
#ifdef CPPPAL_COMPILER_CLANG
#    define CPPPAL_COMPILER CPPPAL_COMPILER_CLANG
#endif
#ifdef CPPPAL_COMPILER_GCC
#    define CPPPAL_COMPILER CPPPAL_COMPILER_GCC
#endif
static_assert(CPPPAL_COMPILER == CPPPAL_TEST_COMP_VERSION,
              "Compiler identification");

static_assert(CPPPAL_STDCXX == CPPPAL_TEST_CXX, "C++ feature level support");
