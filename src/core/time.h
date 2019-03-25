/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/types.h"

namespace dawn
{
namespace time
{
	/// Returns the current time in ticks.
	s64 now();

	/// Returns the ticks in seconds.
	f64 seconds(s64 ticks);

} // namespace time

} // namespace dawn
