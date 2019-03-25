/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/types.h"

/// @defgroup Error Error
/// @ingroup Core
namespace dawn
{
/// Error management.
///
/// @ingroup Error
namespace error
{
	/// Aborts the program execution logging an error message and the stacktrace if
	/// the platform supports it.
	void abort(const char* format, ...);

} // namespace error

} // namespace dawn

#if DAWN_DEBUG
	#define DE_ASSERT(condition, msg, ...)                   \
		do                                                   \
		{                                                    \
			if (DE_UNLIKELY(!(condition)))                   \
			{                                                \
				dawn::error::abort("Assertion failed: %s\n" \
					"    In: %s:%d\n"                        \
					"    " msg "\n"                          \
					, # condition                            \
					, __FILE__                               \
					, __LINE__                               \
					, ## __VA_ARGS__                         \
					);                                       \
				DE_UNREACHABLE();                            \
			}                                                \
		} while (0)
#else
	#define DE_ASSERT(...) DE_NOOP()
#endif // DAWN_DEBUG

#define DE_FATAL(msg, ...) DE_ASSERT(false, msg, ## __VA_ARGS__)
#define DE_ENSURE(condition) DE_ASSERT(condition, "")
