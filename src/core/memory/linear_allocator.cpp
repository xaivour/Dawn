/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#include "core/memory/linear_allocator.h"
#include "core/memory/memory.h"

namespace dawn
{
LinearAllocator::LinearAllocator(Allocator& backing, u32 size)
	: _backing(&backing)
	, _physical_start(NULL)
	, _total_size(size)
	, _offset(0)
{
	_physical_start = backing.allocate(size);
}

LinearAllocator::LinearAllocator(void* start, u32 size)
	: _backing(NULL)
	, _physical_start(start)
	, _total_size(size)
	, _offset(0)
{
}

LinearAllocator::~LinearAllocator()
{
	if (_backing)
		_backing->deallocate(_physical_start);

	DE_ASSERT(_offset == 0
		, "Memory leak of %d bytes, maybe you forgot to call clear()?"
		, _offset
		);
}

void* LinearAllocator::allocate(u32 size, u32 align)
{
	const u32 actual_size = size + align;

	// Out of memory
	if (_offset + actual_size > _total_size)
		return NULL;

	void* user_ptr = memory::align_top((char*)_physical_start + _offset, align);

	_offset += actual_size;

	return user_ptr;
}

void LinearAllocator::deallocate(void* /*data*/)
{
	// Single deallocations not supported. Use clear().
}

void LinearAllocator::clear()
{
	_offset = 0;
}

} // namespace dawn
