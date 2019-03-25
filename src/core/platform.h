/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

// Adapted from Branimir Karadžić's platform.h (https://github.com/bkaradzic/bx)

#pragma once

#define DAWN_COMPILER_CLANG 0
#define DAWN_COMPILER_GCC 0
#define DAWN_COMPILER_MSVC 0

#define DAWN_PLATFORM_ANDROID 0
#define DAWN_PLATFORM_IOS 0
#define DAWN_PLATFORM_LINUX 0
#define DAWN_PLATFORM_OSX 0
#define DAWN_PLATFORM_WINDOWS 0

#define DAWN_CPU_ARM  0
#define DAWN_CPU_JIT  0
#define DAWN_CPU_MIPS 0
#define DAWN_CPU_PPC  0
#define DAWN_CPU_X86  0

#define DAWN_ARCH_32BIT 0
#define DAWN_ARCH_64BIT 0

#define DAWN_CPU_ENDIAN_BIG 0
#define DAWN_CPU_ENDIAN_LITTLE 0

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Compilers
#if defined(_MSC_VER)
	#undef DAWN_COMPILER_MSVC
	#define DAWN_COMPILER_MSVC 1
#elif defined(__clang__)
	// clang defines __GNUC__
	#undef DAWN_COMPILER_CLANG
	#define DAWN_COMPILER_CLANG 1
#elif defined(__GNUC__)
	#undef DAWN_COMPILER_GCC
	#define DAWN_COMPILER_GCC 1
#else
	#error "DAWN_COMPILER_* is not defined!"
#endif

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Operating_Systems
#if defined(_WIN32) || defined(_WIN64)
	#undef DAWN_PLATFORM_WINDOWS
// http://msdn.microsoft.com/en-us/library/6sehtctf.aspx
	#if !defined(WINVER) && !defined(_WIN32_WINNT)
// Windows 7 and above
		#define WINVER 0x0601
		#define _WIN32_WINNT 0x0601
	#endif // !defined(WINVER) && !defined(_WIN32_WINNT)
	#define DAWN_PLATFORM_WINDOWS 1
#elif defined(__ANDROID__)
// Android compiler defines __linux__
	#undef DAWN_PLATFORM_ANDROID
	#define DAWN_PLATFORM_ANDROID 1
#elif defined(__linux__)
	#undef DAWN_PLATFORM_LINUX
	#define DAWN_PLATFORM_LINUX 1
#elif defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
	#undef DAWN_PLATFORM_IOS
	#define DAWN_PLATFORM_IOS 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
	#undef DAWN_PLATFORM_OSX
	#define DAWN_PLATFORM_OSX 1
#else
#	error "DAWN_PLATFORM_* is not defined!"
#endif

#define DAWN_PLATFORM_POSIX (DAWN_PLATFORM_ANDROID \
						|| DAWN_PLATFORM_IOS \
						|| DAWN_PLATFORM_LINUX \
						|| DAWN_PLATFORM_OSX)

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Architectures
#if defined(__arm__)
	#undef DAWN_CPU_ARM
	#define DAWN_CPU_ARM 1
	#define DAWN_CACHE_LINE_SIZE 64
#elif defined(__MIPSEL__) || defined(__mips_isa_rev) // defined(mips)
	#undef DAWN_CPU_MIPS
	#define DAWN_CPU_MIPS 1
	#define DAWN_CACHE_LINE_SIZE 64
#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
	#undef DAWN_CPU_PPC
	#define DAWN_CPU_PPC 1
	#define DAWN_CACHE_LINE_SIZE 128
#elif defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
	#undef DAWN_CPU_X86
	#define DAWN_CPU_X86 1
	#define DAWN_CACHE_LINE_SIZE 64
#else // PNaCl doesn't have CPU defined.
	#undef DAWN_CPU_JIT
	#define DAWN_CPU_JIT 1
	#define DAWN_CACHE_LINE_SIZE 64
#endif //

#if defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
	#undef DAWN_ARCH_64BIT
	#define DAWN_ARCH_64BIT 64
#else
	#undef DAWN_ARCH_32BIT
	#define DAWN_ARCH_32BIT 32
#endif //

#if DAWN_CPU_PPC
	#undef DAWN_CPU_ENDIAN_BIG
	#define DAWN_CPU_ENDIAN_BIG 1
#else
	#undef DAWN_CPU_ENDIAN_LITTLE
	#define DAWN_CPU_ENDIAN_LITTLE 1
#endif

#if DAWN_COMPILER_GCC
	#define DAWN_COMPILER_NAME "GCC"
#elif DAWN_COMPILER_MSVC
	#define DAWN_COMPILER_NAME "MSVC"
#endif

#if DAWN_PLATFORM_ANDROID
	#define DAWN_PLATFORM_NAME "android"
#elif DAWN_PLATFORM_IOS
	#define DAWN_PLATFORM_NAME "ios"
#elif DAWN_PLATFORM_LINUX
	#define DAWN_PLATFORM_NAME "linux"
#elif DAWN_PLATFORM_OSX
	#define DAWN_PLATFORM_NAME "osx"
#elif DAWN_PLATFORM_WINDOWS
	#define DAWN_PLATFORM_NAME "windows"
#endif // DAWN_PLATFORM_

#if DAWN_CPU_ARM
	#define DAWN_CPU_NAME "ARM"
#elif DAWN_CPU_MIPS
	#define DAWN_CPU_NAME "MIPS"
#elif DAWN_CPU_PPC
	#define DAWN_CPU_NAME "PowerPC"
#elif DAWN_CPU_JIT
	#define DAWN_CPU_NAME "JIT-VM"
#elif DAWN_CPU_X86
	#define DAWN_CPU_NAME "x86"
#endif // DAWN_CPU_

#if DAWN_ARCH_32BIT
	#define DAWN_ARCH_NAME "32-bit"
#elif DAWN_ARCH_64BIT
	#define DAWN_ARCH_NAME "64-bit"
#endif // DAWN_ARCH_
