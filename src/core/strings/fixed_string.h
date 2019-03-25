/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/strings/string.h"

namespace dawn
{
/// Fixed string.
///
/// @ingroup String
struct FixedString
{
	u32 _length;
	const char* _data;

	FixedString()
		: _length(0)
		, _data(NULL)
	{
	}

	FixedString(const char* str)
		: _length(strlen32(str))
		, _data(str)
	{
	}

	FixedString(const char* str, u32 len)
		: _length(len)
		, _data(str)
	{
	}

	FixedString& operator=(const char* str)
	{
		_length = strlen32(str);
		_data = str;
		return *this;
	}

	u32 length() const
	{
		return _length;
	}

	const char* data() const
	{
		return _data;
	}
};

inline bool operator==(const FixedString& a, const char* str)
{
	const u32 len = strlen32(str);
	return a._length == len
		&& strncmp(a._data, str, len) == 0
		;
}

inline bool operator==(const FixedString& a, const FixedString& b)
{
	return a._length == b._length
		&& strncmp(a._data, b._data, a._length) == 0
		;
}

inline bool operator<(const FixedString& a, const FixedString& b)
{
	const u32 len = max(a._length, b._length);
	return strncmp(a._data, b._data, len) < 0;
}

} // namespace dawn
