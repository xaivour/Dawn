/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/json/types.h"
#include "core/strings/types.h"

namespace dawn
{
/// Functions to parse JSON-encoded data.
///
/// @ingroup JSON
namespace json
{
	/// Returns the data type of the JSON string @a json.
	JsonValueType::Enum type(const char* json);

	/// Returns the JSON number @a json as int.
	s32 parse_int(const char* json);

	/// Returns the JSON number @a json as f32.
	f32 parse_float(const char* json);

	/// Returns the JSON boolean @a json as bool.
	bool parse_bool(const char* json);

	/// Parses the JSON string @a json ad puts it into @a string.
	void parse_string(const char* json, DynamicString& string);

	/// Parses the JSON array @a json and puts it into @a array as pointers to
	/// the corresponding items into the original @a json string.
	void parse_array(const char* json, JsonArray& array);

	/// Parses the JSON object @a json and puts it into @a object as map from
	/// key to pointer to the corresponding value into the original string @a json.
	void parse_object(const char* json, JsonObject& object);

	/// Parses the JSON-encoded @a json.
	void parse(const char* json, JsonObject& object);

	/// Parses the JSON-encoded @a json.
	void parse(Buffer& json, JsonObject& object);

} // namespace json

} // namespace dawn
