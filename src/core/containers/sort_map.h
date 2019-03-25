/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/containers/vector.h"
#include <algorithm>

namespace dawn
{
/// Functions to manipulate SortMap.
///
/// @ingroup Containers
namespace sort_map
{
	/// Returns the number of items in the map @a m.
	template <typename TKey, typename TValue, typename Compare> u32 size(const SortMap<TKey, TValue, Compare>& m);

	/// Returns whether the @a key exists in the map.
	template <typename TKey, typename TValue, typename Compare> bool has(const SortMap<TKey, TValue, Compare>& m, const TKey& key);

	/// Returns the value for the given @a key or @a deffault if
	/// the key does not exist in the map.
	template <typename TKey, typename TValue, typename Compare> const TValue& get(const SortMap<TKey, TValue, Compare>& m, const TKey& key, const TValue& deffault);

	/// Returns the value for the given @a key or @a deffault if
	/// the key does not exist in the map.
	template <typename TKey, typename TValue, typename Compare> TValue& get(SortMap<TKey, TValue, Compare>& m, const TKey& key, const TValue& deffault);

	/// Sorts the keys in the map.
	template <typename TKey, typename TValue, typename Compare> void sort(SortMap<TKey, TValue, Compare>& m);

	template <typename TKey, typename TValue, typename Compare> void set(SortMap<TKey, TValue, Compare>& m, const TKey& key, const TValue& val);

	/// Removes the @a key from the map if it exists.
	template <typename TKey, typename TValue, typename Compare> void remove(SortMap<TKey, TValue, Compare>& m, const TKey& key);

	/// Removes all the items in the map.
	template <typename TKey, typename TValue, typename Compare> void clear(SortMap<TKey, TValue, Compare>& m);

	/// Returns a pointer to the first item in the map, can be used to
	/// efficiently iterate over the elements.
	template <typename TKey, typename TValue, typename Compare> const typename SortMap<TKey, TValue, Compare>::Entry* begin(const SortMap<TKey, TValue, Compare>& m);
	template <typename TKey, typename TValue, typename Compare> const typename SortMap<TKey, TValue, Compare>::Entry* end(const SortMap<TKey, TValue, Compare>& m);

} // namespace sort_map

namespace sort_map_internal
{
	const u32 END_OF_LIST = 0xffffffffu;

	struct FindResult
	{
		u32 item_i;
	};

	template <typename TKey, typename TValue, typename Compare>
	struct CompareEntry
	{
		bool operator()(const typename SortMap<TKey, TValue, Compare>::Entry& a,
			const typename SortMap<TKey, TValue, Compare>::Entry& b) const
		{
			return comp(a.first, b.first);
		}

		bool operator()(const typename SortMap<TKey, TValue, Compare>::Entry& a,
			const TKey& key) const
		{
			return comp(a.first, key);
		}

		Compare comp;
	};

	template <typename TKey, typename TValue, typename Compare>
	inline FindResult find(const SortMap<TKey, TValue, Compare>& m, const TKey& key)
	{
		DE_ASSERT(m._is_sorted, "Map not sorted");

		FindResult result;
		result.item_i = END_OF_LIST;

		const typename SortMap<TKey, TValue, Compare>::Entry* first =
			std::lower_bound(vector::begin(m._data), vector::end(m._data), key,
			sort_map_internal::CompareEntry<TKey, TValue, Compare>());

		if (first != vector::end(m._data) && !(key < first->first))
			result.item_i = u32(first - vector::begin(m._data));

		return result;
	}

} // namespace sort_map_internal

namespace sort_map
{
	template <typename TKey, typename TValue, typename Compare>
	inline u32 size(const SortMap<TKey, TValue, Compare>& m)
	{
		return vector::size(m._data);
	}

	template <typename TKey, typename TValue, typename Compare>
	inline bool has(const SortMap<TKey, TValue, Compare>& m, const TKey& key)
	{
		return sort_map_internal::find(m, key).item_i != sort_map_internal::END_OF_LIST;
	}

	template <typename TKey, typename TValue, typename Compare>
	const TValue& get(const SortMap<TKey, TValue, Compare>& m, const TKey& key, const TValue& deffault)
	{
		sort_map_internal::FindResult result = sort_map_internal::find(m, key);

		if (result.item_i == sort_map_internal::END_OF_LIST)
			return deffault;

		return m._data[result.item_i].second;
	}

	template <typename TKey, typename TValue, typename Compare>
	TValue& get(SortMap<TKey, TValue, Compare>& m, const TKey& key, const TValue& deffault)
	{
		return const_cast<TValue&>(get(static_cast<const SortMap<TKey, TValue, Compare>&>(m), key, deffault));
	}

	template <typename TKey, typename TValue, typename Compare>
	inline void sort(SortMap<TKey, TValue, Compare>& m)
	{
		std::sort(vector::begin(m._data), vector::end(m._data),
			sort_map_internal::CompareEntry<TKey, TValue, Compare>());
#if DAWN_DEBUG
		m._is_sorted = true;
#endif // DAWN_DEBUG
	}

	template <typename TKey, typename TValue, typename Compare>
	inline void set(SortMap<TKey, TValue, Compare>& m, const TKey& key, const TValue& val)
	{
		sort_map_internal::FindResult result = sort_map_internal::find(m, key);

		if (result.item_i == sort_map_internal::END_OF_LIST)
		{
			typename SortMap<TKey, TValue, Compare>::Entry e(*m._data._allocator);
			e.first = key;
			e.second = val;
			vector::push_back(m._data, e);
		}
		else
		{
			m._data[result.item_i].second = val;
		}
#if DAWN_DEBUG
		m._is_sorted = false;
#endif // DAWN_DEBUG
	}

	template <typename TKey, typename TValue, typename Compare>
	inline void remove(SortMap<TKey, TValue, Compare>& m, const TKey& key)
	{
		sort_map_internal::FindResult result = sort_map_internal::find(m, key);

		if (result.item_i == sort_map_internal::END_OF_LIST)
			return;

		if (vector::size(m._data))
		{
			m._data[result.item_i] = m._data[vector::size(m._data) - 1];
			vector::pop_back(m._data);
		}
#if DAWN_DEBUG
		m._is_sorted = false;
#endif // DAWN_DEBUG
	}

	template <typename TKey, typename TValue, typename Compare>
	inline void clear(SortMap<TKey, TValue, Compare>& m)
	{
		vector::clear(m._data);
#if DAWN_DEBUG
		m._is_sorted = true;
#endif // DAWN_DEBUG
	}

	template <typename TKey, typename TValue, typename Compare>
	inline const typename SortMap<TKey, TValue, Compare>::Entry* begin(const SortMap<TKey, TValue, Compare>& m)
	{
		return vector::begin(m._data);
	}

	template <typename TKey, typename TValue, typename Compare>
	inline const typename SortMap<TKey, TValue, Compare>::Entry* end(const SortMap<TKey, TValue, Compare>& m)
	{
		return vector::end(m._data);
	}

} // namespace sort_map

template <typename TKey, typename TValue, typename Compare>
inline SortMap<TKey, TValue, Compare>::SortMap(Allocator& a)
	: _data(a)
#if DAWN_DEBUG
	, _is_sorted(true)
#endif // DAWN_DEBUG
{
}

} // namespace dawn
