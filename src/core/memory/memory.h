/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/error/error.h"
#include "core/memory/allocator.h"
#include "core/types.h"
#include <new>

namespace dawn
{
Allocator& default_allocator();
Allocator& default_scratch_allocator();

namespace memory
{
	/// Returns the pointer @a p aligned to the desired @a align byte
	inline void* align_top(void* p, u32 align)
	{
		DE_ASSERT(align >= 1, "Alignment must be > 1");
		DE_ASSERT(align % 2 == 0 || align == 1, "Alignment must be a power of two");

		uintptr_t ptr = (uintptr_t)p;
		const u32 mod = ptr % align;

		if (mod)
			ptr += align - mod;

		return (void*)ptr;
	}

	/// Respects standard behaviour when calling on NULL @a ptr
	template <typename T>
	inline void call_destructor_and_deallocate(Allocator& a, T* ptr)
	{
		if (!ptr)
			return;

		ptr->~T();
		a.deallocate(ptr);
	}

} // namespace memory

#define ALLOCATOR_AWARE typedef int allocator_aware

/// Convert integer to type.
template <int v>
struct Int2Type { enum {value=v}; };

/// Determines if a type is allocator aware.
template <typename T>
struct is_allocator_aware {
	template <typename C>
	static char test_fun(typename C::allocator_aware *);

	template <typename C>
	static int test_fun(...);

	enum {
		value = (sizeof(test_fun<T>(0)) == sizeof(char))
	};
};

#define IS_ALLOCATOR_AWARE(T) is_allocator_aware<T>::value
#define IS_ALLOCATOR_AWARE_TYPE(T) Int2Type< IS_ALLOCATOR_AWARE(T) >

/// Allocator aware constuction
template <typename T> inline T &construct(void *p, Allocator& a, Int2Type<true>) {new (p) T(a); return *(T *)p;}
template <typename T> inline T &construct(void *p, Allocator& /*a*/, Int2Type<false>) {new (p) T; return *(T *)p;}
template <typename T> inline T &construct(void *p, Allocator& a) {return construct<T>(p, a, IS_ALLOCATOR_AWARE_TYPE(T)());}

namespace memory_globals
{
	/// Constructs the initial default allocators.
	/// @note
	/// Has to be called before anything else during the engine startup.
	void init();

	/// Destroys the allocators created with memory_globals::init().
	/// @note
	/// Should be the last call of the program.
	void shutdown();

} // namespace memory_globals

} // namespace dawn

/// Allocates memory with @a allocator for the given @a T type
/// and calls constructor on it.
/// @note
/// @a allocator must be a reference to an existing allocator.
#define DE_NEW(allocator, T) new ((allocator).allocate(sizeof(T), alignof(T))) T

/// Calls destructor on @a ptr and deallocates memory using the
/// given @a allocator.
/// @note
/// @a allocator must be a reference to an existing allocator.
#define DE_DELETE(allocator, ptr) dawn::memory::call_destructor_and_deallocate(allocator, ptr)
