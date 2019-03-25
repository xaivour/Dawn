/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/containers/types.h"

/// @defgroup String String
/// @ingroup Core
namespace dawn
{
struct DynamicString;
struct FixedString;
struct StringId32;
struct StringId64;

typedef StringId64 ResourceId;

/// Stream of characters.
///
/// @ingroup String
typedef Array<char> StringStream;

} // namespace dawn
