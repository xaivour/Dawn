/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/strings/types.h"
#include "core/types.h"

namespace dawn
{
/// Hashed string.
///
/// @ingroup String
struct StringId32
{
	u32 _id;

	StringId32() : _id(0) {}
	explicit StringId32(u32 idx) : _id(idx) {}
	explicit StringId32(const char* str);
	explicit StringId32(const char* str, u32 len);

	void hash(const char* str, u32 len);

	/// Fills @a s with the string representation of the id.
	void to_string(DynamicString& s);
};

/// Hashed string.
///
/// @ingroup String
struct StringId64
{
	u64 _id;

	StringId64() : _id(0) {}
	explicit StringId64(u64 idx) : _id(idx) {}
	explicit StringId64(const char* str);
	explicit StringId64(const char* str, u32 len);

	void hash(const char* str, u32 len);

	/// Fills @a s with the string representation of the id.
	void to_string(DynamicString& s);
};

/// @addtogroup String
/// @{
inline bool operator==(const StringId32& a, const StringId32& b)
{
	return a._id == b._id;
}

inline bool operator!=(const StringId32& a, const StringId32& b)
{
	return a._id != b._id;
}

inline bool operator<(const StringId32& a, const StringId32& b)
{
	return a._id < b._id;
}

inline bool operator==(const StringId64& a, const StringId64& b)
{
	return a._id == b._id;
}

inline bool operator!=(const StringId64& a, const StringId64& b)
{
	return a._id != b._id;
}

inline bool operator<(const StringId64& a, const StringId64& b)
{
	return a._id < b._id;
}

template <>
struct hash<StringId32>
{
	u32 operator()(const StringId32& id) const
	{
		return id._id;
	}
};

template <>
struct hash<StringId64>
{
	u32 operator()(const StringId64& id) const
	{
		return (u32)id._id;
	}
};
/// @}

} // namespace dawn
