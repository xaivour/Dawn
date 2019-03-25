/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/strings/types.h"

namespace dawn
{
namespace error
{
	/// Fills @a ss with the current call stack.
	void callstack(StringStream& ss);

} // namespace error

} // namespace dawn
