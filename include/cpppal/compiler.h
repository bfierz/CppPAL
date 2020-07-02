#pragma once

// Identify compiler
#if defined __ICC || defined __ICL
#    define CPPPAL_COMPILER_ICC __INTEL_COMPILER
#    if (__INTEL_COMPILER < 1700)
#        warning "Minimum supported version is ICC 17. Good luck."
#    endif
#elif defined _MSC_VER && !defined __clang__
#    define CPPPAL_COMPILER_MSVC _MSC_VER
#    if (_MSC_VER < 1900)
#        warning "Minimum supported version is MSVC 2015. Good luck."
#    endif
#elif defined __clang__
#    define CPPPAL_COMPILER_CLANG (__clang_major__ * 100 + __clang_minor__)
#    if (__clang_major__ < 3) || (__clang_major__ == 3 && __clang_minor__ < 5)
#        warning "Minimum supported version is Clang 3.5. Good luck."
#    endif
#elif defined __GNUC__
#    define CPPPAL_COMPILER_GCC (__GNUC__ * 100 + __GNUC_MINOR__)
#    if (__GNUC__ < 5)
#        warning "Minimum supported version is GCC 5. Good luck."
#    endif
#endif

// Identify C++ standard
#if defined _MSVC_LANG // Requires "Visual Studio 2015 Update 3"
#    if _MSVC_LANG > 201703L
#        define CPPPAL_STDCXX 20
#    elif _MSVC_LANG > 201402L
#        define CPPPAL_STDCXX 17
#    elif _MSVC_LANG > 201103L
#        define CPPPAL_STDCXX 14
#    endif
#elif defined __cplusplus
#    if __cplusplus > 201703L
#        define CPPPAL_STDCXX 20
#    elif __cplusplus > 201402L
#        define CPPPAL_STDCXX 17
#    elif __cplusplus > 201103L
#        define CPPPAL_STDCXX 14
#    elif __cplusplus == 201103L || _MSC_VER >= 1800
#        define CPPPAL_STDCXX 11
#    endif
#endif
#ifndef CPPPAL_STDCXX
#    define CPPPAL_STDCXX 03
#endif

// Identify platform
#if defined CPPPAL_COMPILER_ICC || defined CPPPAL_COMPILER_MSVC ||             \
  defined CPPPAL_COMPILER_CLANG || defined CPPPAL_COMPILER_GCC
#    if defined(_WIN32)
#        define CPPPAL_PLATFORM_WIN
#        if defined(_WIN64)
#            define CPPPAL_PLATFORM_WIN64
#        else
#            define CPPPAL_PLATFORM_WIN32
#        endif
#    endif

#    if defined(__linux__) && !defined(__ANDROID__)
#        define CPPPAL_PLATFORM_LINUX
#    endif
#endif

// Identify CPU base instruction set
#if defined CPPPAL_COMPILER_ICC

// The Intel compiler only supports Intel64 and x86 instruction sets
#    if defined _M_X64 || defined __x86_64 || defined __x86_64__
#        define CPPPAL_ARCH_X64
#    else
#        define CPPPAL_ARCH_X86
#    endif

#elif defined CPPPAL_COMPILER_MSVC
#    if defined _M_IX86
#        define CPPPAL_ARCH_X86
#    endif

#    if defined _M_X64
#        define CPPPAL_ARCH_X64
#    endif

#    if defined _M_ARM
#        define CPPPAL_ARCH_ARM
#    endif

#    if defined _M_ARM64
#        define CPPPAL_ARCH_ARM64
#    endif

#elif defined CPPPAL_COMPILER_GCC || defined CPPPAL_COMPILER_CLANG
#    if defined __i386__ || defined __i686__
#        define CPPPAL_ARCH_X86
#    endif

#    if defined __x86_64
#        define CPPPAL_ARCH_X64
#    endif

#    if defined __arm__
#        define CPPPAL_ARCH_ARM
#    endif

#    if defined __aarch64__
#        define CPPPAL_ARCH_ARM64
#    endif

#    if defined EMSCRIPTEN
#        define CPPPAL_ARCH_WEBASM
#    endif

#endif

// Support compiler specific extensions
#if defined CPPPAL_COMPILER_MSVC

#    define CPPPAL_STRONG_INLINE __forceinline
#    define CPPPAL_DEBUG_BREAK __debugbreak()

#elif defined(CPPPAL_COMPILER_GCC) || defined(CPPPAL_COMPILER_CLANG)

#    define CPPPAL_STRONG_INLINE inline
#    define CPPPAL_DEBUG_BREAK __builtin_trap()

#elif defined(CPPPAL_COMPILER_ICC)

#    define CPPPAL_STRONG_INLINE __forceinline
#    define CPPPAL_DEBUG_BREAK

#else // No compiler found
#    define CPPPAL_STRONG_INLINE inline
#    define CPPPAL_DEBUG_BREAK
#endif

////////////////////////////////////////////////////////////////////////////////
// Support for older compiler versions
////////////////////////////////////////////////////////////////////////////////

// Add missing definition for max_align_t for compatibility with older clang
// version (3.4, 3.5)
#if defined(CPPPAL_COMPILER_CLANG)
#    if defined(CPPPAL_COMPILER_CLANG) && !defined(_MSC_VER)
#        if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) ||      \
          __cplusplus >= 201103L
#            if !defined(__CLANG_MAX_ALIGN_T_DEFINED) &&                       \
              !defined(_GCC_MAX_ALIGN_T) && !defined(__DEFINED_max_align_t)
typedef struct
{
    long long __clang_max_align_nonce1
      __attribute__((__aligned__(__alignof__(long long))));
    long double __clang_max_align_nonce2
      __attribute__((__aligned__(__alignof__(long double))));
} max_align_t;
#                define __DEFINED_max_align_t
#            endif
#        endif
#    endif

#endif

// Add support for alignas/alignof for older MSVC versions
#if defined(CPPPAL_COMPILER_MSVC)
#    if (_MSC_VER <= 1800)
#        include <xkeycheck.h>
#        if defined alignof
#            undef alignof
#        endif
#        define alignof(x) __alignof(x)
#        define alignas(x) __declspec(align(x))
#    endif
#endif

////////////////////////////////////////////////////////////////////////////////
// Evaluate compiler feature support
////////////////////////////////////////////////////////////////////////////////

// constexpr
#if defined(CPPPAL_COMPILER_MSVC)
#    if (_MSC_VER < 1900)
#        define VCL_HAS_CPP_CONSTEXPR_11 0
#        define VCL_HAS_CPP_CONSTEXPR_14 0
#        define VCL_CPP_CONSTEXPR_11
#        define VCL_CPP_CONSTEXPR_14
#    elif (_MSC_VER <= 1900)
#        define VCL_HAS_CPP_CONSTEXPR_11 1
#        define VCL_HAS_CPP_CONSTEXPR_14 0
#        define VCL_CPP_CONSTEXPR_11 constexpr
#        define VCL_CPP_CONSTEXPR_14
#    elif (_MSC_VER > 1900)
#        define VCL_HAS_CPP_CONSTEXPR_11 1
#        define VCL_HAS_CPP_CONSTEXPR_14 1
#        define VCL_CPP_CONSTEXPR_11 constexpr
#        define VCL_CPP_CONSTEXPR_14 constexpr
#    endif
#elif defined(CPPPAL_COMPILER_GCC) || defined(CPPPAL_COMPILER_CLANG) ||        \
  defined(CPPPAL_COMPILER_ICC)
#    define VCL_HAS_CPP_CONSTEXPR_11 1
#    define VCL_HAS_CPP_CONSTEXPR_14 1
#    define VCL_CPP_CONSTEXPR_11 constexpr
#    define VCL_CPP_CONSTEXPR_14 constexpr
#else
#    define VCL_HAS_CPP_CONSTEXPR_11 0
#    define VCL_HAS_CPP_CONSTEXPR_14 0
#    define VCL_CPP_CONSTEXPR_11
#    define VCL_CPP_CONSTEXPR_14
#endif

// if constexpr
#if defined(CPPPAL_COMPILER_MSVC)
#    if (_MSC_VER >= 1912 && CPPPAL_STDCXX >= 17)
#        define CPPPAL_IF_CONSTEXPR if constexpr
#    else
#        define CPPPAL_IF_CONSTEXPR if
#    endif
#elif defined(CPPPAL_COMPILER_GCC)
#    if defined(__cpp_if_constexpr) && __cpp_if_constexpr >= 201606
#        define CPPPAL_IF_CONSTEXPR if constexpr
#    else
#        define CPPPAL_IF_CONSTEXPR if
#    endif
#elif defined(CPPPAL_COMPILER_CLANG)
#    if __clang_major__ < 3 ||                                                 \
      (__clang_major__ == 3 && __clang_minor__ < 9) || __cplusplus < 201703l
#        define CPPPAL_IF_CONSTEXPR if
#    else
#        define CPPPAL_IF_CONSTEXPR if constexpr
#    endif
#endif

// noexcept
#if defined(CPPPAL_COMPILER_MSVC)
#    if (_MSC_VER <= 1800)
#        define noexcept _NOEXCEPT
#        define VCL_NOEXCEPT_PARAM(param)
#    else
#        define VCL_NOEXCEPT_PARAM(param) noexcept(param)
#    endif
#elif defined(CPPPAL_COMPILER_GCC) || defined(CPPPAL_COMPILER_CLANG) ||        \
  defined(CPPPAL_COMPILER_ICC)
#    define VCL_NOEXCEPT_PARAM(param) noexcept(param)
#else
#    define VCL_NOEXCEPT_PARAM(param)
#endif

// thread_local
#if defined(CPPPAL_COMPILER_MSVC)
#    if (_MSC_VER <= 1900)
#        define thread_local __declspec(thread)
#    endif
#elif defined(CPPPAL_COMPILER_GCC)
#    if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 8)
#        define thread_local __thread
#    endif
#elif defined(CPPPAL_COMPILER_CLANG)
#    if __clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ < 3)
#        define thread_local __thread
#    endif
#endif

////////////////////////////////////////////////////////////////////////////////
// Evaluate standard library support
////////////////////////////////////////////////////////////////////////////////

// chrono
#if defined(CPPPAL_COMPILER_MSVC)
#    if _MSC_VER >= 1700
#        define CPPPAL_STL_CHRONO
#    endif
#elif defined(CPPPAL_COMPILER_GCC) || defined(CPPPAL_COMPILER_CLANG)
#    if __cplusplus >= 201103L
#        define CPPPAL_STL_CHRONO
#    endif
#endif
