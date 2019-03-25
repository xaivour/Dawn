/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/types.h"

namespace dawn
{
u32 murmur32(const void* key, u32 len, u32 seed);
u64 murmur64(const void* key, u32 len, u64 seed);

} // namespace dawn
