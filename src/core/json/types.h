/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/containers/types.h"
#include "core/strings/fixed_string.h"

/// @defgroup JSON
/// @ingroup Core
namespace dawn
{
/// Enumerates JSON value types.
///
/// @ingroup JSON
struct JsonValueType
{
	enum Enum
	{
		NIL,
		BOOL,
		NUMBER,
		STRING,
		ARRAY,
		OBJECT
	};
};

/// Array of pointers to json-encoded data.
///
/// @ingroup JSON
typedef Array<const char*> JsonArray;

/// Map from key to pointers to json-encoded data.
///
/// @ingroup JSON
struct JsonObject
{
	Map<FixedString, const char*> _map;

	JsonObject(Allocator& a);

	const char* operator[](const char* key) const;
	const char* operator[](const FixedString& key) const;
};

} // namespace dawn
