/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/platform.h"
#include <stdint.h>
#include <stddef.h>

#ifndef DAWN_DEBUG
	#define DAWN_DEBUG 0
#endif // DAWN_DEBUG

#ifndef DAWN_DEVELOPMENT
	#define DAWN_DEVELOPMENT 0
#endif // DAWN_DEVELOPMENT

/// @defgroup Core Core

namespace dawn
{
/// @addtogroup Core
/// @{
typedef int8_t   s8;
typedef uint8_t  u8;
typedef int16_t  s16;
typedef uint16_t u16;
typedef int32_t  s32;
typedef uint32_t u32;
typedef int64_t  s64;
typedef uint64_t u64;
typedef float    f32;
typedef double   f64;
/// @}

template <typename T>
inline void exchange(T& a, T& b)
{
	T c = a;
	a = b;
	b = c;
}

template <typename T>
inline T min(const T& a, const T& b)
{
	return a < b ? a : b;
}

template <typename T>
inline T max(const T& a, const T& b)
{
	return a > b ? a : b;
}

template <typename T>
inline T clamp(T val, T mmin, T mmax)
{
	return min(max(mmin, val), mmax);
}

} // namespace dawn

#if defined(_MSC_VER)
	#define _ALLOW_KEYWORD_MACROS
#endif

#if !defined(alignof)
	#define alignof(x) __alignof(x)
#endif

#if !defined(__va_copy)
	#define __va_copy(dest, src) (dest = src)
#endif

#define countof(arr) (sizeof(arr)/sizeof(arr[0]))
#define container_of(ptr, type, member) ((char*)ptr - offsetof(type, member))

#define DE_NOOP(...) do { (void)0; } while (0)
#define DE_UNUSED(x) do { (void)(x); } while (0)
#define DE_STATIC_ASSERT(condition, ...) static_assert(condition, "" # __VA_ARGS__)

#if defined(__GNUC__)
	#define DE_LIKELY(x) __builtin_expect((x), 1)
	#define DE_UNLIKELY(x) __builtin_expect((x), 0)
	#define DE_UNREACHABLE() __builtin_unreachable()
#else
	#define DE_LIKELY(x) (x)
	#define DE_UNLIKELY(x) (x)
	#define DE_UNREACHABLE()
#endif
#if defined(__GNUC__)
	#define DE_THREAD __thread
#elif defined(_MSC_VER)
	#define DE_THREAD __declspec(thread)
#else
	#error "Compiler not supported"
#endif

#if DAWN_PLATFORM_LINUX
	#define DE_ALIGN_DECL(align, decl) decl __attribute__ ((aligned (align)))
#elif DAWN_PLATFORM_WINDOWS
	#define DE_ALIGN_DECL(align_, decl) __declspec (align(align_)) decl
#endif
