/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#include "core/time.h"

#if DAWN_PLATFORM_POSIX
	#include <time.h> // clock_gettime
#elif DAWN_PLATFORM_WINDOWS
	#include <windows.h>
#endif

namespace dawn
{
namespace time
{
	s64 now()
	{
#if DAWN_PLATFORM_LINUX || DAWN_PLATFORM_ANDROID
		timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		return now.tv_sec * s64(1000000000) + now.tv_nsec;
#elif DAWN_PLATFORM_OSX
		struct timeval now;
		gettimeofday(&now, NULL);
		return now.tv_sec * s64(1000000) + now.tv_usec;
#elif DAWN_PLATFORM_WINDOWS
		LARGE_INTEGER ttime;
		QueryPerformanceCounter(&ttime);
		return (s64)ttime.QuadPart;
#endif
	}

	inline s64 frequency()
	{
#if DAWN_PLATFORM_LINUX || DAWN_PLATFORM_ANDROID
		return s64(1000000000);
#elif DAWN_PLATFORM_OSX
		return s64(1000000);
#elif DAWN_PLATFORM_WINDOWS
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		return (s64)freq.QuadPart;
#endif
	}

	f64 seconds(s64 ticks)
	{
		return f64(ticks) / f64(frequency());
	}

} // namespace time

} // namespace dawn
