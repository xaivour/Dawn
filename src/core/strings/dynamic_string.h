/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/containers/array.h"
#include "core/error/error.h"
#include "core/memory/memory.h"
#include "core/strings/fixed_string.h"
#include "core/strings/string.h"
#include "core/strings/string_id.h"
#include <string.h> // memmove

namespace dawn
{
/// Dynamic array of characters.
///
/// @ingroup String
struct DynamicString
{
	ALLOCATOR_AWARE;

	Array<char> _data;

	DynamicString(Allocator& a);

	DynamicString& operator=(const DynamicString& ds);
	DynamicString& operator=(const char* str);
	DynamicString& operator=(const char c);
	DynamicString& operator=(const FixedString& fs);

	/// Sets the string to @a str.
	void set(const char* str, u32 len);

	/// Reserves space for at least @a n characters.
	void reserve(u32 n);

	/// Returns the length of the string.
	u32 length() const;

	/// Returns whether the string is empty.
	bool empty() const;

	/// Removes leading white-space characters from the string.
	void ltrim();

	/// Removes trailing white-space characters from the string.
	void rtrim();

	/// Removes leading and trailing white-space characters from the string.
	void trim();

	/// Returns whether the string starts with @a str.
	bool has_prefix(const char* str) const;

	/// Returns wheterh the string ends with @a str.
	bool has_suffix(const char* str) const;

	/// Returns the StringId32 of the string.
	StringId32 to_string_id() const;

	/// Returns the string as a NULL-terminated string.
	const char* c_str() const;
};

inline DynamicString::DynamicString(Allocator& a)
	: _data(a)
{
}

inline void DynamicString::set(const char* str, u32 len)
{
	array::resize(_data, len);
	memcpy(array::begin(_data), str, len);
}

/// Appends the string @a b to @a a.
inline DynamicString& operator+=(DynamicString& a, const DynamicString& b)
{
	array::push(a._data, array::begin(b._data), array::size(b._data));
	return a;
}

/// Appends the string @a s to @a a.
inline DynamicString& operator+=(DynamicString& a, const char* str)
{
	DE_ENSURE(NULL != str);
	array::push(a._data, str, strlen32(str));
	return a;
}

/// Appends the char @a c to @a a.
inline DynamicString& operator+=(DynamicString& a, const char c)
{
	array::push_back(a._data, c);
	return a;
}

/// Appends the string @a s to @a a.
inline DynamicString& operator+=(DynamicString& a, const FixedString& fs)
{
	array::push(a._data, fs.data(), fs.length());
	return a;
}

inline DynamicString& DynamicString::operator=(const DynamicString& ds)
{
	_data = ds._data;
	return *this;
}

inline DynamicString& DynamicString::operator=(const char* str)
{
	DE_ENSURE(NULL != str);
	array::clear(_data);
	array::push(_data, str, strlen32(str));
	return *this;
}

inline DynamicString& DynamicString::operator=(const char c)
{
	array::clear(_data);
	array::push_back(_data, c);
	return *this;
}

inline DynamicString& DynamicString::operator=(const FixedString& fs)
{
	array::clear(_data);
	array::push(_data, fs.data(), fs.length());
	return *this;
}

inline bool operator<(const DynamicString& a, const DynamicString& b)
{
	return strcmp(a.c_str(), b.c_str()) < 0;
}

inline bool operator==(const DynamicString& a, const DynamicString& b)
{
	return strcmp(a.c_str(), b.c_str()) == 0;
}

inline bool operator==(const DynamicString& a, const char* str)
{
	DE_ENSURE(NULL != str);
	return strcmp(a.c_str(), str) == 0;
}

inline void DynamicString::reserve(u32 n)
{
	array::reserve(_data, n);
}

inline u32 DynamicString::length() const
{
	return strlen32(c_str());
}

inline bool DynamicString::empty() const
{
	return length() == 0;
}

inline void DynamicString::ltrim()
{
	const char* str = c_str();
	const char* end = skip_spaces(str);

	const u32 len = strlen32(end);

	memmove(array::begin(_data), end, len);
	array::resize(_data, len);
}

inline void DynamicString::rtrim()
{
	char* str = (char*)c_str();
	char* end = str + strlen32(str) - 1;

	while (end > str && isspace(*end)) --end;

	*(end + 1) = '\0';
}

inline void DynamicString::trim()
{
	ltrim();
	rtrim();
}

inline bool DynamicString::has_prefix(const char* str) const
{
	DE_ENSURE(NULL != str);
	const u32 ml = strlen32(c_str());
	const u32 sl = strlen32(str);
	return sl <= ml && strncmp(&_data[0], str, sl) == 0;
}

inline bool DynamicString::has_suffix(const char* str) const
{
	DE_ENSURE(NULL != str);
	const u32 ml = strlen32(c_str());
	const u32 sl = strlen32(str);
	return sl <= ml && strncmp(&_data[ml-sl], str, sl) == 0;
}

inline StringId32 DynamicString::to_string_id() const
{
	return StringId32(c_str());
}

inline const char* DynamicString::c_str() const
{
	Array<char>& data = const_cast<Array<char>& >(_data);
	array::push_back(data, '\0');
	array::pop_back(data);
	return array::begin(_data);
}

} // namespace dawn
