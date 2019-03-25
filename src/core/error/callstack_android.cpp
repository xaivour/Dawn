/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#include "core/platform.h"

#if DAWN_PLATFORM_ANDROID

#include "core/strings/string_stream.h"

namespace dawn
{
namespace error
{
	void callstack(StringStream& ss)
	{
		ss << "Not supported";
	}

} // namespace error

} // namespace dawn

#endif // DAWN_PLATFORM_ANDROID
