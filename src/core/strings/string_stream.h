/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/containers/array.h"
#include "core/strings/string.h"
#include "core/strings/types.h"

namespace dawn
{
/// Functions to manipulate StringStream.
///
/// @ingroup String
namespace string_stream
{
	/// Returns the stream as a NUL-terminated string.
	const char* c_str(StringStream& s);

	template <typename T> StringStream& stream_printf(StringStream& s, const char* format, T& val);

} // namespace string_stream

/// @addtogroup String
/// @{

/// Appends @a val to the stream @a s using appropriate formatting.
inline StringStream& operator<<(StringStream& s, char val)
{
	array::push_back(s, val);
	return s;
}

/// Appends @a val to the stream @a s using appropriate formatting.
inline StringStream& operator<<(StringStream& s, s16 val)
{
	return string_stream::stream_printf(s, "%hd", val);
}

/// Appends @a val to the stream @a s using appropriate formatting.
inline StringStream& operator<<(StringStream& s, u16 val)
{
	return string_stream::stream_printf(s, "%hu", val);
}

/// Appends @a val to the stream @a s using appropriate formatting.
inline StringStream& operator<<(StringStream& s, s32 val)
{
	return string_stream::stream_printf(s, "%d", val);
}

/// Appends @a val to the stream @a s using appropriate formatting.
inline StringStream& operator<<(StringStream& s, u32 val)
{
	return string_stream::stream_printf(s, "%u", val);
}

/// Appends @a val to the stream @a s using appropriate formatting.
inline StringStream& operator<<(StringStream& s, s64 val)
{
	return string_stream::stream_printf(s, "%lld", val);
}

/// Appends @a val to the stream @a s using appropriate formatting.
inline StringStream& operator<<(StringStream& s, u64 val)
{
	return string_stream::stream_printf(s, "%llu", val);
}

/// Appends @a val to the stream @a s using appropriate formatting.
inline StringStream& operator<<(StringStream& s, f32 val)
{
	return string_stream::stream_printf(s, "%.9g", val);
}

/// Appends @a val to the stream @a s using appropriate formatting.
inline StringStream& operator<<(StringStream& s, f64 val)
{
	return string_stream::stream_printf(s, "%.17g", val);
}

/// Appends the string @a str to the stream @a s.
inline StringStream& operator<<(StringStream& s, const char* str)
{
	array::push(s, str, strlen32(str));
	return s;
}
/// @}

namespace string_stream
{
	inline const char* c_str(StringStream& s)
	{
		array::push_back(s, '\0');
		array::pop_back(s);
		return array::begin(s);
	}

	template <typename T>
	inline StringStream& stream_printf(StringStream& s, const char* format, T& val)
	{
		char buf[32];
		snprintf(buf, sizeof(buf), format, val);
		return s << buf;
	}

} // namespace string_stream

} // namespace dawn
