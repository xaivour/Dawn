/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#include "core/containers/map.h"
#include "core/json/json.h"
#include "core/memory/temp_allocator.h"
#include "core/strings/dynamic_string.h"
#include "core/strings/string.h"

namespace dawn
{
namespace json
{
	static const char* next(const char* json, const char c = 0)
	{
		DE_ENSURE(NULL != json);

		if (c && c != *json)
		{
			DE_ASSERT(false, "Expected '%c' got '%c'", c, *json);
		}

		return ++json;
	}

	static const char* skip_string(const char* json)
	{
		DE_ENSURE(NULL != json);

		while (*++json)
		{
			if (*json == '"')
			{
				++json;
				return json;
			}
			else if (*json == '\\')
			{
				++json;
			}
		}

		return json;
	}

	static const char* skip_value(const char* json)
	{
		DE_ENSURE(NULL != json);

		switch (*json)
		{
		case '"': json = skip_string(json); break;
		case '[': json = skip_block(json, '[', ']'); break;
		case '{': json = skip_block(json, '{', '}'); break;
		default: for (; *json != ',' && *json != '}' && *json != ']'; ++json) ; break;
		}

		return json;
	}

	JsonValueType::Enum type(const char* json)
	{
		DE_ENSURE(NULL != json);

		switch (*json)
		{
		case '"': return JsonValueType::STRING;
		case '{': return JsonValueType::OBJECT;
		case '[': return JsonValueType::ARRAY;
		case '-': return JsonValueType::NUMBER;
		default: return (isdigit(*json)) ? JsonValueType::NUMBER : (*json == 'n' ? JsonValueType::NIL : JsonValueType::BOOL);
		}
	}

	static f64 parse_number(const char* json)
	{
		DE_ENSURE(NULL != json);

		TempAllocator512 alloc;
	 	Array<char> number(alloc);

		if (*json == '-')
		{
			array::push_back(number, '-');
			++json;
		}
		while (isdigit(*json))
		{
			array::push_back(number, *json);
			++json;
		}

		if (*json == '.')
		{
			array::push_back(number, '.');
			while (*++json && isdigit(*json))
			{
				array::push_back(number, *json);
			}
		}

		if (*json == 'e' || *json == 'E')
		{
			array::push_back(number, *json);
			++json;

			if (*json == '-' || *json == '+')
			{
				array::push_back(number, *json);
				++json;
			}
			while (isdigit(*json))
			{
				array::push_back(number, *json);
				++json;
			}
		}

		array::push_back(number, '\0');

		f64 val;
		int ok = sscanf(array::begin(number), "%lf", &val);
		DE_ASSERT(ok == 1, "Failed to parse f64: %s", array::begin(number));
		DE_UNUSED(ok);
		return val;
	}

	s32 parse_int(const char* json)
	{
		return (s32)parse_number(json);
	}

	f32 parse_float(const char* json)
	{
		return (f32)parse_number(json);
	}

	bool parse_bool(const char* json)
	{
		DE_ENSURE(NULL != json);

		switch (*json)
		{
		case 't':
			json = next(json, 't');
			json = next(json, 'r');
			json = next(json, 'u');
			next(json, 'e');
			return true;

		case 'f':
			json = next(json, 'f');
			json = next(json, 'a');
			json = next(json, 'l');
			json = next(json, 's');
			next(json, 'e');
			return false;

		default:
			DE_FATAL("Bad boolean");
			return false;
		}
	}

	void parse_string(const char* json, DynamicString& string)
	{
		DE_ENSURE(NULL != json);

		if (*json == '"')
		{
			while (*++json)
			{
				// Empty string
				if (*json == '"')
				{
					++json;
					return;
				}
				else if (*json == '\\')
				{
					++json;

					switch (*json)
					{
					case '"': string += '"'; break;
					case '\\': string += '\\'; break;
					case '/': string += '/'; break;
					case 'b': string += '\b'; break;
					case 'f': string += '\f'; break;
					case 'n': string += '\n'; break;
					case 'r': string += '\r'; break;
					case 't': string += '\t'; break;
					default:
					{
						DE_FATAL("Bad escape character");
						break;
					}
				}
				}
				else
				{
					string += *json;
				}
			}
		}

		DE_FATAL("Bad string");
	}

	void parse_array(const char* json, JsonArray& array)
	{
		DE_ENSURE(NULL != json);

		if (*json == '[')
		{
			json = skip_spaces(++json);

			if (*json == ']')
				return;

			while (*json)
			{
				array::push_back(array, json);

				json = skip_value(json);
				json = skip_spaces(json);

				if (*json == ']')
					return;

				json = next(json, ',');
				json = skip_spaces(json);
			}
		}

		DE_FATAL("Bad array");
	}

	void parse_object(const char* json, JsonObject& object)
	{
		DE_ENSURE(NULL != json);

		if (*json == '{')
		{
			json = skip_spaces(++json);

			if (*json == '}')
				return;

			while (*json)
			{
				const char* key_begin = *json == '"' ? (json + 1) : json;

				TempAllocator256 ta;
				DynamicString key(ta);
				parse_string(json, key);

				FixedString fs_key(key_begin, key.length());

				json = skip_string(json);
				json = skip_spaces(json);
				json = next(json, ':');
				json = skip_spaces(json);

				map::set(object._map, fs_key, json);

				json = skip_value(json);
				json = skip_spaces(json);

				if (*json == '}')
					return;

				json = next(json, ',');
				json = skip_spaces(json);
			}
		}

		DE_FATAL("Bad object");
	}

	void parse(const char* json, JsonObject& object)
	{
		DE_ENSURE(NULL != json);
		parse_object(json, object);
	}

	void parse(Buffer& json, JsonObject& object)
	{
		array::push_back(json, '\0');
		array::pop_back(json);
		parse(array::begin(json), object);
	}

} // namespace json

} // namespace dawn
