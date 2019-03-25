/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#include "config.h"

#if DAWN_BUILD_UNIT_TESTS

#include "core/command_line.h"
#include "core/containers/array.h"
#include "core/containers/hash_map.h"
#include "core/containers/hash_set.h"
#include "core/containers/vector.h"
#include "core/filesystem/path.h"
#include "core/guid.h"
#include "core/json/json.h"
#include "core/json/sjson.h"
#include "core/math/aabb.h"
#include "core/math/color4.h"
#include "core/math/math.h"
#include "core/math/matrix3x3.h"
#include "core/math/matrix4x4.h"
#include "core/math/quaternion.h"
#include "core/math/sphere.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/math/vector4.h"
#include "core/memory/memory.h"
#include "core/memory/temp_allocator.h"
#include "core/murmur.h"
#include "core/strings/dynamic_string.h"
#include "core/strings/string.h"
#include "core/strings/string_id.h"
#include "core/thread/thread.h"
#include "core/time.h"
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE

#define ENSURE(condition)                                \
	do                                                   \
	{                                                    \
		if (!(condition))                                \
		{                                                \
			printf("Assertion failed: '%s' in %s:%d\n\n" \
				, #condition                             \
				, __FILE__                               \
				, __LINE__                               \
				);                                       \
			exit(EXIT_FAILURE);                          \
		}                                                \
	}                                                    \
	while (0)

namespace dawn
{
static void test_memory()
{
	memory_globals::init();
	Allocator& a = default_allocator();

	void* p = a.allocate(32);
	ENSURE(a.allocated_size(p) >= 32);
	a.deallocate(p);

	memory_globals::shutdown();
}

static void test_array()
{
	memory_globals::init();
	Allocator& a = default_allocator();
	{
		Array<int> v(a);

		ENSURE(array::size(v) == 0);
		array::push_back(v, 1);
		ENSURE(array::size(v) == 1);
		ENSURE(v[0] == 1);
	}
	memory_globals::shutdown();
}

static void test_vector()
{
	memory_globals::init();
	Allocator& a = default_allocator();
	{
		Vector<int> v(a);

		ENSURE(vector::size(v) == 0);
		vector::push_back(v, 1);
		ENSURE(vector::size(v) == 1);
		ENSURE(v[0] == 1);
	}
	memory_globals::shutdown();
}

static void test_hash_map()
{
	memory_globals::init();
	Allocator& a = default_allocator();
	{
		HashMap<s32, s32> m(a);

		ENSURE(hash_map::size(m) == 0);
		ENSURE(hash_map::get(m, 0, 42) == 42);
		ENSURE(!hash_map::has(m, 10));

		for (s32 i = 0; i < 100; ++i)
			hash_map::set(m, i, i*i);
		for (s32 i = 0; i < 100; ++i)
			ENSURE(hash_map::get(m, i, 0) == i*i);

		hash_map::remove(m, 20);
		ENSURE(!hash_map::has(m, 20));

		hash_map::remove(m, 2000);
		ENSURE(!hash_map::has(m, 2000));

		hash_map::remove(m, 50);
		ENSURE(!hash_map::has(m, 50));

		hash_map::clear(m);

		for (s32 i = 0; i < 100; ++i)
			ENSURE(!hash_map::has(m, i));
	}
	{
		HashMap<s32, s32> m(a);
		hash_map_internal::grow(m);
		ENSURE(hash_map::capacity(m) == 16);

		hash_map::set(m, 0, 7);

		hash_map::set(m, 1, 1);

		for (s32 i = 2; i < 150; ++i)
		{
			hash_map::set(m, i, 2);
			ENSURE(hash_map::has(m, 0));
			ENSURE(hash_map::has(m, 1));
			ENSURE(hash_map::has(m, i));
			hash_map::remove(m, i);
		}
	}
	memory_globals::shutdown();
}

static void test_hash_set()
{
	memory_globals::init();
	Allocator& a = default_allocator();
	{
		HashSet<s32> m(a);

		ENSURE(hash_set::size(m) == 0);
		ENSURE(!hash_set::has(m, 10));

		for (s32 i = 0; i < 100; ++i)
			hash_set::insert(m, i*i);
		for (s32 i = 0; i < 100; ++i)
			ENSURE(hash_set::has(m, i*i));

		hash_set::remove(m, 5*5);
		ENSURE(!hash_set::has(m, 5*5));

		hash_set::remove(m, 80*80);
		ENSURE(!hash_set::has(m, 80*80));

		hash_set::remove(m, 40*40);
		ENSURE(!hash_set::has(m, 40*40));

		hash_set::clear(m);

		for (s32 i = 0; i < 100; ++i)
			ENSURE(!hash_set::has(m, i*i));
	}
	memory_globals::shutdown();
}

static void test_vector2()
{
	{
		const Vector2 a = vector2(1.2f,  4.2f);
		const Vector2 b = vector2(2.7f, -1.9f);
		const Vector2 c = a - b;
		ENSURE(fequal(c.x, -1.5f, 0.0001f));
		ENSURE(fequal(c.y,  6.1f, 0.0001f));
	}
	{
		const Vector2 a = vector2(1.2f,  4.2f);
		const Vector2 b = vector2(2.7f, -1.9f);
		const Vector2 c = a + b;
		ENSURE(fequal(c.x,  3.9f, 0.0001f));
		ENSURE(fequal(c.y,  2.3f, 0.0001f));
	}
	{
		const Vector2 a = vector2(1.2f,  4.2f);
		const Vector2 b = a * 2.0f;
		ENSURE(fequal(b.x,  2.4f, 0.0001f));
		ENSURE(fequal(b.y,  8.4f, 0.0001f));
	}
	{
		const Vector2 a = vector2(1.2f,  4.2f);
		const Vector2 b = vector2(2.7f, -1.9f);
		const f32 c = dot(a, b);
		ENSURE(fequal(c,  -4.74f, 0.0001f));
	}
	{
		const Vector2 a = vector2(1.2f,  4.2f);
		const f32 c = length_squared(a);
		ENSURE(fequal(c, 19.08f, 0.0001f));
	}
	{
		const Vector2 a = vector2(1.2f,  4.2f);
		const f32 c = length(a);
		ENSURE(fequal(c,  4.36806f, 0.0001f));
	}
	{
		Vector2 a = vector2(1.2f,  4.2f);
		normalize(a);
		ENSURE(fequal(length(a), 1.0f, 0.00001f));
	}
	{
		const Vector2 a = vector2(1.2f,  4.2f);
		const Vector2 b = vector2(2.7f, -1.9f);
		const float c = distance_squared(a, b);
		ENSURE(fequal(c, 39.46f, 0.00001f));
	}
	{
		const Vector2 a = vector2(1.2f,  4.2f);
		const Vector2 b = vector2(2.7f, -1.9f);
		const float c = distance(a, b);
		ENSURE(fequal(c, 6.28171f, 0.00001f));
	}
	{
		const Vector2 a = vector2(1.2f,  4.2f);
		const Vector2 b = vector2(2.7f, -1.9f);
		const Vector2 c = max(a, b);
		ENSURE(fequal(c.x,  2.7f, 0.00001f));
		ENSURE(fequal(c.y,  4.2f, 0.00001f));
	}
	{
		const Vector2 a = vector2(1.2f,  4.2f);
		const Vector2 b = vector2(2.7f, -1.9f);
		const Vector2 c = min(a, b);
		ENSURE(fequal(c.x,  1.2f, 0.00001f));
		ENSURE(fequal(c.y, -1.9f, 0.00001f));
	}
}

static void test_vector3()
{
	{
		const Vector3 a = vector3(1.2f,  4.2f, -2.3f);
		const Vector3 b = vector3(2.7f, -1.9f, -4.1f);
		const Vector3 c = a - b;
		ENSURE(fequal(c.x, -1.5f, 0.0001f));
		ENSURE(fequal(c.y,  6.1f, 0.0001f));
		ENSURE(fequal(c.z,  1.8f, 0.0001f));
	}
	{
		const Vector3 a = vector3(1.2f,  4.2f, -2.3f);
		const Vector3 b = vector3(2.7f, -1.9f, -4.1f);
		const Vector3 c = a + b;
		ENSURE(fequal(c.x,  3.9f, 0.0001f));
		ENSURE(fequal(c.y,  2.3f, 0.0001f));
		ENSURE(fequal(c.z, -6.4f, 0.0001f));
	}
	{
		const Vector3 a = vector3(1.2f,  4.2f, -2.3f);
		const Vector3 b = a * 2.0f;
		ENSURE(fequal(b.x,  2.4f, 0.0001f));
		ENSURE(fequal(b.y,  8.4f, 0.0001f));
		ENSURE(fequal(b.z, -4.6f, 0.0001f));
	}
	{
		const Vector3 a = vector3(1.2f,  4.2f, -2.3f);
		const Vector3 b = vector3(2.7f, -1.9f, -4.1f);
		const f32 c = dot(a, b);
		ENSURE(fequal(c,  4.69f, 0.0001f));
	}
	{
		const Vector3 a = vector3(1.2f,  4.2f, -2.3f);
		const Vector3 b = vector3(2.7f, -1.9f, -4.1f);
		const Vector3 c = cross(a, b);
		ENSURE(fequal(c.x, -21.59f, 0.0001f));
		ENSURE(fequal(c.y,  -1.29f, 0.0001f));
		ENSURE(fequal(c.z, -13.62f, 0.0001f));
	}
	{
		const Vector3 a = vector3(1.2f,  4.2f, -2.3f);
		const f32 c = length_squared(a);
		ENSURE(fequal(c, 24.37f, 0.0001f));
	}
	{
		const Vector3 a = vector3(1.2f,  4.2f, -2.3f);
		const f32 c = length(a);
		ENSURE(fequal(c,  4.93659f, 0.0001f));
	}
	{
		Vector3 a = vector3(1.2f,  4.2f, -2.3f);
		normalize(a);
		ENSURE(fequal(length(a), 1.0f, 0.00001f));
	}
	{
		const Vector3 a = vector3(1.2f,  4.2f, -2.3f);
		const Vector3 b = vector3(2.7f, -1.9f, -4.1f);
		const float c = distance_squared(a, b);
		ENSURE(fequal(c, 42.70f, 0.00001f));
	}
	{
		const Vector3 a = vector3(1.2f,  4.2f, -2.3f);
		const Vector3 b = vector3(2.7f, -1.9f, -4.1f);
		const float c = distance(a, b);
		ENSURE(fequal(c, 6.53452f, 0.00001f));
	}
	{
		const Vector3 a = vector3(1.2f,  4.2f, -2.3f);
		const Vector3 b = vector3(2.7f, -1.9f, -4.1f);
		const Vector3 c = max(a, b);
		ENSURE(fequal(c.x,  2.7f, 0.00001f));
		ENSURE(fequal(c.y,  4.2f, 0.00001f));
		ENSURE(fequal(c.z, -2.3f, 0.00001f));
	}
	{
		const Vector3 a = vector3(1.2f,  4.2f, -2.3f);
		const Vector3 b = vector3(2.7f, -1.9f, -4.1f);
		const Vector3 c = min(a, b);
		ENSURE(fequal(c.x,  1.2f, 0.00001f));
		ENSURE(fequal(c.y, -1.9f, 0.00001f));
		ENSURE(fequal(c.z, -4.1f, 0.00001f));
	}
}

static void test_vector4()
{
	{
		const Vector4 a = vector4(1.2f,  4.2f, -2.3f, 5.5f);
		const Vector4 b = vector4(2.7f, -1.9f, -4.1f, 1.0f);
		const Vector4 c = a - b;
		ENSURE(fequal(c.x, -1.5f, 0.0001f));
		ENSURE(fequal(c.y,  6.1f, 0.0001f));
		ENSURE(fequal(c.z,  1.8f, 0.0001f));
		ENSURE(fequal(c.w,  4.5f, 0.0001f));
	}
	{
		const Vector4 a = vector4(1.2f,  4.2f, -2.3f, 5.5f);
		const Vector4 b = vector4(2.7f, -1.9f, -4.1f, 1.0f);
		const Vector4 c = a + b;
		ENSURE(fequal(c.x,  3.9f, 0.0001f));
		ENSURE(fequal(c.y,  2.3f, 0.0001f));
		ENSURE(fequal(c.z, -6.4f, 0.0001f));
		ENSURE(fequal(c.w,  6.5f, 0.0001f));
	}
	{
		const Vector4 a = vector4(1.2f,  4.2f, -2.3f, 1.5f);
		const Vector4 b = a * 2.0f;
		ENSURE(fequal(b.x,  2.4f, 0.0001f));
		ENSURE(fequal(b.y,  8.4f, 0.0001f));
		ENSURE(fequal(b.z, -4.6f, 0.0001f));
		ENSURE(fequal(b.w,  3.0f, 0.0001f));
	}
	{
		const Vector4 a = vector4(1.2f,  4.2f, -2.3f, 5.5f);
		const Vector4 b = vector4(2.7f, -1.9f, -4.1f, 1.0f);
		const f32 c = dot(a, b);
		ENSURE(fequal(c,  10.19f, 0.0001f));
	}
	{
		const Vector4 a = vector4(1.2f,  4.2f, -2.3f, 5.5f);
		const f32 c = length_squared(a);
		ENSURE(fequal(c, 54.62f, 0.0001f));
	}
	{
		const Vector4 a = vector4(1.2f,  4.2f, -2.3f, 5.5f);
		const f32 c = length(a);
		ENSURE(fequal(c,  7.39053f, 0.0001f));
	}
	{
		Vector4 a = vector4(1.2f,  4.2f, -2.3f, 5.5f);
		normalize(a);
		ENSURE(fequal(length(a), 1.0f, 0.00001f));
	}
	{
		const Vector4 a = vector4(1.2f,  4.2f, -2.3f, 5.5f);
		const Vector4 b = vector4(2.7f, -1.9f, -4.1f, 1.0f);
		const float c = distance_squared(a, b);
		ENSURE(fequal(c, 62.95f, 0.00001f));
	}
	{
		const Vector4 a = vector4(1.2f,  4.2f, -2.3f, 5.5f);
		const Vector4 b = vector4(2.7f, -1.9f, -4.1f, 1.0f);
		const float c = distance(a, b);
		ENSURE(fequal(c, 7.93410f, 0.00001f));
	}
	{
		const Vector4 a = vector4(1.2f,  4.2f, -2.3f, 5.5f);
		const Vector4 b = vector4(2.7f, -1.9f, -4.1f, 1.0f);
		const Vector4 c = max(a, b);
		ENSURE(fequal(c.x,  2.7f, 0.00001f));
		ENSURE(fequal(c.y,  4.2f, 0.00001f));
		ENSURE(fequal(c.z, -2.3f, 0.00001f));
		ENSURE(fequal(c.w,  5.5f, 0.00001f));
	}
	{
		const Vector4 a = vector4(1.2f,  4.2f, -2.3f, 5.5f);
		const Vector4 b = vector4(2.7f, -1.9f, -4.1f, 1.0f);
		const Vector4 c = min(a, b);
		ENSURE(fequal(c.x,  1.2f, 0.00001f));
		ENSURE(fequal(c.y, -1.9f, 0.00001f));
		ENSURE(fequal(c.z, -4.1f, 0.00001f));
		ENSURE(fequal(c.w,  1.0f, 0.00001f));
	}
}

static void test_quaternion()
{
	{
		const Quaternion a = quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		ENSURE(fequal(a.x, 0.0f, 0.00001f));
		ENSURE(fequal(a.y, 0.0f, 0.00001f));
		ENSURE(fequal(a.z, 0.0f, 0.00001f));
		ENSURE(fequal(a.w, 1.0f, 0.00001f));
	}
}

static void test_color4()
{
	{
		const Color4 a = color4(1.3f, 2.6f, 0.2f, 0.6f);
		ENSURE(fequal(a.x, 1.3f, 0.00001f));
		ENSURE(fequal(a.y, 2.6f, 0.00001f));
		ENSURE(fequal(a.z, 0.2f, 0.00001f));
		ENSURE(fequal(a.w, 0.6f, 0.00001f));
	}
	{
		const Color4 a = from_rgba(63, 231, 12, 98);
		ENSURE(fequal(a.x, 0.24705f, 0.00001f));
		ENSURE(fequal(a.y, 0.90588f, 0.00001f));
		ENSURE(fequal(a.z, 0.04705f, 0.00001f));
		ENSURE(fequal(a.w, 0.38431f, 0.00001f));
	}
	{
		const Color4 a = from_rgb(63, 231, 12);
		ENSURE(fequal(a.x, 0.24705f, 0.00001f));
		ENSURE(fequal(a.y, 0.90588f, 0.00001f));
		ENSURE(fequal(a.z, 0.04705f, 0.00001f));
		ENSURE(fequal(a.w, 1.0f    , 0.00001f));
	}
	{
		const Color4 a = from_rgba(0x3fe70c62);
		ENSURE(fequal(a.x, 0.24705f, 0.00001f));
		ENSURE(fequal(a.y, 0.90588f, 0.00001f));
		ENSURE(fequal(a.z, 0.04705f, 0.00001f));
		ENSURE(fequal(a.w, 0.38431f, 0.00001f));
	}
	{
		const Color4 a = from_rgba(63, 231, 12, 98);

		const u32 rgba = to_rgba(a);
		ENSURE(rgba == 0x3fe70c62);

		const u32 rgb = to_rgb(a);
		ENSURE(rgb == 0x3fe70cff);

		const u32 bgr = to_bgr(a);
		ENSURE(bgr == 0xff0ce73f);

		const u32 abgr = to_abgr(a);
		ENSURE(abgr == 0x620ce73f);
	}
}

static void test_matrix3x3()
{
	{
		const Matrix3x3 a = matrix3x3(1.2f, -2.3f, 5.1f
			,  2.2f, -5.1f,  1.1f
			,  3.2f,  3.3f, -3.8f
			);
		const Matrix3x3 b = matrix3x3(3.2f, 4.8f, 6.0f
			, -1.6f, -7.1f, -2.4f
			, -3.1f, -2.2f,  8.9f
			);
		const Matrix3x3 c = a + b;
		ENSURE(fequal(c.x.x,   4.4f, 0.00001f));
		ENSURE(fequal(c.x.y,   2.5f, 0.00001f));
		ENSURE(fequal(c.x.z,  11.1f, 0.00001f));
		ENSURE(fequal(c.y.x,   0.6f, 0.00001f));
		ENSURE(fequal(c.y.y, -12.2f, 0.00001f));
		ENSURE(fequal(c.y.z,  -1.3f, 0.00001f));
		ENSURE(fequal(c.z.x,   0.1f, 0.00001f));
		ENSURE(fequal(c.z.y,   1.1f, 0.00001f));
		ENSURE(fequal(c.z.z,   5.1f, 0.00001f));
	}
	{
		const Matrix3x3 a = matrix3x3(1.2f, -2.3f, 5.1f
			,  2.2f, -5.1f,  1.1f
			,  3.2f,  3.3f, -3.8f
			);
		const Matrix3x3 b = matrix3x3(3.2f, 4.8f, 6.0f
			, -1.6f, -7.1f, -2.4f
			, -3.1f, -2.2f,  8.9f
			);
		const Matrix3x3 c = a - b;
		ENSURE(fequal(c.x.x,  -2.0f, 0.00001f));
		ENSURE(fequal(c.x.y,  -7.1f, 0.00001f));
		ENSURE(fequal(c.x.z,  -0.9f, 0.00001f));
		ENSURE(fequal(c.y.x,   3.8f, 0.00001f));
		ENSURE(fequal(c.y.y,   2.0f, 0.00001f));
		ENSURE(fequal(c.y.z,   3.5f, 0.00001f));
		ENSURE(fequal(c.z.x,   6.3f, 0.00001f));
		ENSURE(fequal(c.z.y,   5.5f, 0.00001f));
		ENSURE(fequal(c.z.z, -12.7f, 0.00001f));
	}
	{
		const Matrix3x3 a = matrix3x3(1.2f, -2.3f, 5.1f
			,  2.2f, -5.1f,  1.1f
			,  3.2f,  3.3f, -3.8f
			);
		const Matrix3x3 b = matrix3x3(3.2f, 4.8f, 6.0f
			, -1.6f, -7.1f, -2.4f
			, -3.1f, -2.2f,  8.9f
			);
		const Matrix3x3 c = a * b;
		ENSURE(fequal(c.x.x,  -8.29f, 0.00001f));
		ENSURE(fequal(c.x.y,  10.87f, 0.00001f));
		ENSURE(fequal(c.x.z,  58.11f, 0.00001f));
		ENSURE(fequal(c.y.x,  11.79f, 0.00001f));
		ENSURE(fequal(c.y.y,  44.35f, 0.00001f));
		ENSURE(fequal(c.y.z,  35.23f, 0.00001f));
		ENSURE(fequal(c.z.x,  16.74f, 0.00001f));
		ENSURE(fequal(c.z.y,   0.29f, 0.00001f));
		ENSURE(fequal(c.z.z, -22.54f, 0.00001f));
	}
	{
		const Matrix3x3 a = matrix3x3(1.2f, -2.3f, 5.1f
			,  2.2f, -5.1f,  1.1f
			,  3.2f,  3.3f, -3.8f
			);
		const Matrix3x3 b = get_inverted(a);
		ENSURE(fequal(b.x.x,  0.140833f, 0.00001f));
		ENSURE(fequal(b.x.y,  0.072339f, 0.00001f));
		ENSURE(fequal(b.x.z,  0.209954f, 0.00001f));
		ENSURE(fequal(b.y.x,  0.106228f, 0.00001f));
		ENSURE(fequal(b.y.y, -0.186705f, 0.00001f));
		ENSURE(fequal(b.y.z,  0.088524f, 0.00001f));
		ENSURE(fequal(b.z.x,  0.210848f, 0.00001f));
		ENSURE(fequal(b.z.y, -0.101221f, 0.00001f));
		ENSURE(fequal(b.z.z, -0.009478f, 0.00001f));
	}
	{
		const Matrix3x3 a = matrix3x3(1.2f, -2.3f, 5.1f
			,  2.2f, -5.1f,  1.1f
			,  3.2f,  3.3f, -3.8f
			);
		const Matrix3x3 b = get_transposed(a);
		ENSURE(fequal(b.x.x,  1.2f, 0.00001f));
		ENSURE(fequal(b.x.y,  2.2f, 0.00001f));
		ENSURE(fequal(b.x.z,  3.2f, 0.00001f));
		ENSURE(fequal(b.y.x, -2.3f, 0.00001f));
		ENSURE(fequal(b.y.y, -5.1f, 0.00001f));
		ENSURE(fequal(b.y.z,  3.3f, 0.00001f));
		ENSURE(fequal(b.z.x,  5.1f, 0.00001f));
		ENSURE(fequal(b.z.y,  1.1f, 0.00001f));
		ENSURE(fequal(b.z.z, -3.8f, 0.00001f));
	}
}

static void test_matrix4x4()
{
	{
		const Matrix4x4 a = matrix4x4(1.2f, -2.3f, 5.1f, -1.2f
			,  2.2f, -5.1f,  1.1f, -7.4f
			,  3.2f,  3.3f, -3.8f, -9.2f
			, -6.8f, -2.9f,  1.0f,  4.9f
			);
		const Matrix4x4 b = matrix4x4(3.2f, 4.8f, 6.0f, 5.3f
			, -1.6f, -7.1f, -2.4f, -6.2f
			, -3.1f, -2.2f,  8.9f,  8.3f
			,  3.8f,  9.1f, -3.1f, -7.1f
			);
		const Matrix4x4 c = a + b;
		ENSURE(fequal(c.x.x,   4.4f, 0.00001f));
		ENSURE(fequal(c.x.y,   2.5f, 0.00001f));
		ENSURE(fequal(c.x.z,  11.1f, 0.00001f));
		ENSURE(fequal(c.x.w,   4.1f, 0.00001f));
		ENSURE(fequal(c.y.x,   0.6f, 0.00001f));
		ENSURE(fequal(c.y.y, -12.2f, 0.00001f));
		ENSURE(fequal(c.y.z,  -1.3f, 0.00001f));
		ENSURE(fequal(c.y.w, -13.6f, 0.00001f));
		ENSURE(fequal(c.z.x,   0.1f, 0.00001f));
		ENSURE(fequal(c.z.y,   1.1f, 0.00001f));
		ENSURE(fequal(c.z.z,   5.1f, 0.00001f));
		ENSURE(fequal(c.z.w,  -0.9f, 0.00001f));
		ENSURE(fequal(c.t.x,  -3.0f, 0.00001f));
		ENSURE(fequal(c.t.y,   6.2f, 0.00001f));
		ENSURE(fequal(c.t.z,  -2.1f, 0.00001f));
		ENSURE(fequal(c.t.w,  -2.2f, 0.00001f));
	}
	{
		const Matrix4x4 a = matrix4x4(1.2f, -2.3f, 5.1f, -1.2f
			,  2.2f, -5.1f,  1.1f, -7.4f
			,  3.2f,  3.3f, -3.8f, -9.2f
			, -6.8f, -2.9f,  1.0f,  4.9f
			);
		const Matrix4x4 b = matrix4x4(3.2f, 4.8f, 6.0f, 5.3f
			, -1.6f, -7.1f, -2.4f, -6.2f
			, -3.1f, -2.2f,  8.9f,  8.3f
			,  3.8f,  9.1f, -3.1f, -7.1f
			);
		const Matrix4x4 c = a - b;
		ENSURE(fequal(c.x.x,  -2.0f, 0.00001f));
		ENSURE(fequal(c.x.y,  -7.1f, 0.00001f));
		ENSURE(fequal(c.x.z,  -0.9f, 0.00001f));
		ENSURE(fequal(c.x.w,  -6.5f, 0.00001f));
		ENSURE(fequal(c.y.x,   3.8f, 0.00001f));
		ENSURE(fequal(c.y.y,   2.0f, 0.00001f));
		ENSURE(fequal(c.y.z,   3.5f, 0.00001f));
		ENSURE(fequal(c.y.w,  -1.2f, 0.00001f));
		ENSURE(fequal(c.z.x,   6.3f, 0.00001f));
		ENSURE(fequal(c.z.y,   5.5f, 0.00001f));
		ENSURE(fequal(c.z.z, -12.7f, 0.00001f));
		ENSURE(fequal(c.z.w, -17.5f, 0.00001f));
		ENSURE(fequal(c.t.x, -10.6f, 0.00001f));
		ENSURE(fequal(c.t.y, -12.0f, 0.00001f));
		ENSURE(fequal(c.t.z,   4.1f, 0.00001f));
		ENSURE(fequal(c.t.w,  12.0f, 0.00001f));
	}
	{
		const Matrix4x4 a = matrix4x4(1.2f, -2.3f, 5.1f, -1.2f
			,  2.2f, -5.1f,  1.1f, -7.4f
			,  3.2f,  3.3f, -3.8f, -9.2f
			, -6.8f, -2.9f,  1.0f,  4.9f
			);
		const Matrix4x4 b = matrix4x4(3.2f, 4.8f, 6.0f, 5.3f
			, -1.6f, -7.1f, -2.4f, -6.2f
			, -3.1f, -2.2f,  8.9f,  8.3f
			,  3.8f,  9.1f, -3.1f, -7.1f
			);
		const Matrix4x4 c = a * b;
		ENSURE(fequal(c.x.x, -12.85f, 0.00001f));
		ENSURE(fequal(c.x.y,  -0.05f, 0.00001f));
		ENSURE(fequal(c.x.z,  61.83f, 0.00001f));
		ENSURE(fequal(c.x.w,  71.47f, 0.00001f));
		ENSURE(fequal(c.y.x, -16.33f, 0.00001f));
		ENSURE(fequal(c.y.y, -22.99f, 0.00001f));
		ENSURE(fequal(c.y.z,  58.17f, 0.00001f));
		ENSURE(fequal(c.y.w, 104.95f, 0.00001f));
		ENSURE(fequal(c.z.x, -18.22f, 0.00001f));
		ENSURE(fequal(c.z.y, -83.43f, 0.00001f));
		ENSURE(fequal(c.z.z,   5.98f, 0.00001f));
		ENSURE(fequal(c.z.w,  30.28f, 0.00001f));
		ENSURE(fequal(c.t.x,  -1.60f, 0.00001f));
		ENSURE(fequal(c.t.y,  30.34f, 0.00001f));
		ENSURE(fequal(c.t.z, -40.13f, 0.00001f));
		ENSURE(fequal(c.t.w, -44.55f, 0.00001f));
	}
	{
		const Matrix4x4 a = matrix4x4(1.2f, -2.3f, 5.1f, -1.2f
			,  2.2f, -5.1f,  1.1f, -7.4f
			,  3.2f,  3.3f, -3.8f, -9.2f
			, -6.8f, -2.9f,  1.0f,  4.9f
			);
		const Matrix4x4 b = get_inverted(a);
		ENSURE(fequal(b.x.x, -0.08464f, 0.00001f));
		ENSURE(fequal(b.x.y,  0.06129f, 0.00001f));
		ENSURE(fequal(b.x.z, -0.15210f, 0.00001f));
		ENSURE(fequal(b.x.w, -0.21374f, 0.00001f));
		ENSURE(fequal(b.y.x,  0.14384f, 0.00001f));
		ENSURE(fequal(b.y.y, -0.18486f, 0.00001f));
		ENSURE(fequal(b.y.z,  0.14892f, 0.00001f));
		ENSURE(fequal(b.y.w,  0.03565f, 0.00001f));
		ENSURE(fequal(b.z.x,  0.26073f, 0.00001f));
		ENSURE(fequal(b.z.y, -0.09877f, 0.00001f));
		ENSURE(fequal(b.z.z,  0.07063f, 0.00001f));
		ENSURE(fequal(b.z.w,  0.04729f, 0.00001f));
		ENSURE(fequal(b.t.x, -0.08553f, 0.00001f));
		ENSURE(fequal(b.t.y, -0.00419f, 0.00001f));
		ENSURE(fequal(b.t.z, -0.13735f, 0.00001f));
		ENSURE(fequal(b.t.w, -0.08108f, 0.00001f));
	}
	{
		const Matrix4x4 a = matrix4x4(1.2f, -2.3f, 5.1f, -1.2f
			,  2.2f, -5.1f,  1.1f, -7.4f
			,  3.2f,  3.3f, -3.8f, -9.2f
			, -6.8f, -2.9f,  1.0f,  4.9f
			);
		const Matrix4x4 b = get_transposed(a);
		ENSURE(fequal(b.x.x,  1.2f, 0.00001f));
		ENSURE(fequal(b.x.y,  2.2f, 0.00001f));
		ENSURE(fequal(b.x.z,  3.2f, 0.00001f));
		ENSURE(fequal(b.x.w, -6.8f, 0.00001f));
		ENSURE(fequal(b.y.x, -2.3f, 0.00001f));
		ENSURE(fequal(b.y.y, -5.1f, 0.00001f));
		ENSURE(fequal(b.y.z,  3.3f, 0.00001f));
		ENSURE(fequal(b.y.w, -2.9f, 0.00001f));
		ENSURE(fequal(b.z.x,  5.1f, 0.00001f));
		ENSURE(fequal(b.z.y,  1.1f, 0.00001f));
		ENSURE(fequal(b.z.z, -3.8f, 0.00001f));
		ENSURE(fequal(b.z.w,  1.0f, 0.00001f));
		ENSURE(fequal(b.t.x, -1.2f, 0.00001f));
		ENSURE(fequal(b.t.y, -7.4f, 0.00001f));
		ENSURE(fequal(b.t.z, -9.2f, 0.00001f));
		ENSURE(fequal(b.t.w,  4.9f, 0.00001f));
	}
}

static void test_aabb()
{
	{
		AABB a;
		aabb::reset(a);
		ENSURE(a.min == VECTOR3_ZERO);
		ENSURE(a.max == VECTOR3_ZERO);
	}
	{
		AABB a;
		a.min = vector3(-2.3f, 1.2f, -4.5f);
		a.max = vector3( 3.7f, 5.3f, -2.9f);
		const Vector3 c = aabb::center(a);
		ENSURE(fequal(c.x,  0.70f, 0.00001f));
		ENSURE(fequal(c.y,  3.25f, 0.00001f));
		ENSURE(fequal(c.z, -3.70f, 0.00001f));
	}
	{
		AABB a;
		a.min = vector3(-2.3f, 1.2f, -4.5f);
		a.max = vector3( 3.7f, 5.3f, -2.9f);
		const float c = aabb::volume(a);
		ENSURE(fequal(c, 39.36f, 0.00001f));
	}
	{
		const Vector3 points[] =
		{
			{ -1.2f,  3.4f,  5.5f },
			{  8.2f, -2.4f, -1.5f },
			{ -5.9f,  9.2f,  6.0f }
		};
		AABB a;
		aabb::from_points(a, countof(points), points);
		ENSURE(fequal(a.min.x, -5.9f, 0.00001f));
		ENSURE(fequal(a.min.y, -2.4f, 0.00001f));
		ENSURE(fequal(a.min.z, -1.5f, 0.00001f));
		ENSURE(fequal(a.max.x,  8.2f, 0.00001f));
		ENSURE(fequal(a.max.y,  9.2f, 0.00001f));
		ENSURE(fequal(a.max.z,  6.0f, 0.00001f));
	}
	{
		const Vector3 points[] =
		{
			{ -1.2f,  3.4f,  5.5f },
			{  8.2f, -2.4f, -1.5f },
			{ -5.9f,  9.2f,  6.0f },

			{ -2.8f, -3.5f,  1.9f },
			{ -8.3f, -3.1f,  1.9f },
			{  4.0f, -3.9f, -1.4f },

			{ -0.4f, -1.8f, -2.2f },
			{ -8.6f, -4.8f,  2.8f },
			{  4.1f,  4.7f, -0.4f }
		};
		AABB boxes[3];
		aabb::from_points(boxes[0], countof(points)/3, &points[0]);
		aabb::from_points(boxes[1], countof(points)/3, &points[3]);
		aabb::from_points(boxes[2], countof(points)/3, &points[6]);

		AABB d;
		aabb::from_boxes(d, countof(boxes), boxes);
		ENSURE(fequal(d.min.x, -8.6f, 0.00001f));
		ENSURE(fequal(d.min.y, -4.8f, 0.00001f));
		ENSURE(fequal(d.min.z, -2.2f, 0.00001f));
		ENSURE(fequal(d.max.x,  8.2f, 0.00001f));
		ENSURE(fequal(d.max.y,  9.2f, 0.00001f));
		ENSURE(fequal(d.max.z,  6.0f, 0.00001f));
	}
	{
		AABB a;
		a.min = vector3(-2.3f, 1.2f, -4.5f);
		a.max = vector3( 3.7f, 5.3f, -2.9f);
		ENSURE( aabb::contains_point(a, vector3(1.2f,  3.0f, -4.4f)));
		ENSURE(!aabb::contains_point(a, vector3(3.8f,  3.0f, -4.4f)));
		ENSURE(!aabb::contains_point(a, vector3(1.2f, -1.0f, -4.4f)));
		ENSURE(!aabb::contains_point(a, vector3(1.2f,  3.0f, -4.6f)));
	}
}

static void test_sphere()
{
	{
		Sphere a;
		sphere::reset(a);
		ENSURE(a.c == VECTOR3_ZERO);
		ENSURE(fequal(a.r, 0.0f, 0.00001f));
	}
	{
		Sphere a;
		a.c = VECTOR3_ZERO;
		a.r = 1.61f;
		const float b = sphere::volume(a);
		ENSURE(fequal(b, 17.48099f, 0.00001f));
	}
	{
		Sphere a;
		sphere::reset(a);

		const Vector3 points[] =
		{
			{ -1.2f,  3.4f,  5.5f },
			{  8.2f, -2.4f, -1.5f },
			{ -5.9f,  9.2f,  6.0f }
		};
		sphere::add_points(a, countof(points), points);
		ENSURE(fequal(a.c.x, 0.0f, 0.00001f));
		ENSURE(fequal(a.c.y, 0.0f, 0.00001f));
		ENSURE(fequal(a.c.z, 0.0f, 0.00001f));
		ENSURE(fequal(a.r, 12.46795f, 0.00001f));
	}
	{
		Sphere spheres[3];
		sphere::reset(spheres[0]);
		sphere::reset(spheres[1]);
		sphere::reset(spheres[2]);

		const Vector3 points[] =
		{
			{  6.6f,  3.5f, -5.7f },
			{ -5.3f, -9.1f, -7.9f },
			{ -1.5f,  4.4f, -5.8f },

			{  7.2f, -2.4f, -9.5f },
			{  4.0f, -8.1f,  6.6f },
			{ -8.2f,  2.2f,  4.6f },

			{  2.9f, -4.8f, -6.8f },
			{ -7.6f, -7.0f,  0.8f },
			{  8.2f,  2.8f, -4.8f }
		};
		sphere::add_points(spheres[0], countof(points)/3, &points[0]);
		sphere::add_points(spheres[1], countof(points)/3, &points[3]);
		sphere::add_points(spheres[2], countof(points)/3, &points[6]);

		Sphere d;
		sphere::reset(d);
		sphere::add_spheres(d, countof(spheres), spheres);
		ENSURE(fequal(d.r, 13.16472f, 0.00001f));
	}
	{
		Sphere a;
		a.c = vector3(-2.3f, 1.2f, -4.5f);
		a.r = 1.0f;
		ENSURE( sphere::contains_point(a, vector3(-2.9f, 1.6f, -4.0f)));
		ENSURE(!sphere::contains_point(a, vector3(-3.9f, 1.6f, -4.0f)));
		ENSURE(!sphere::contains_point(a, vector3(-2.9f, 2.6f, -4.0f)));
		ENSURE(!sphere::contains_point(a, vector3(-2.9f, 1.6f, -6.0f)));
	}
}

static void test_murmur()
{
	const u32 m = murmur32("murmur32", 8, 0);
	ENSURE(m == 0x7c2365dbu);
	const u64 n = murmur64("murmur64", 8, 0);
	ENSURE(n == 0x90631502d1a3432bu);
}

static void test_string_id()
{
	memory_globals::init();
	{
		StringId32 a("murmur32");
		ENSURE(a._id == 0x7c2365dbu);

		StringId32 b("murmur32", 8);
		ENSURE(a._id == 0x7c2365dbu);

		TempAllocator64 ta;
		DynamicString str(ta);
		a.to_string(str);
		ENSURE(strcmp(str.c_str(), "7c2365db") == 0);
	}
	{
		StringId64 a("murmur64");
		ENSURE(a._id == 0x90631502d1a3432bu);

		StringId64 b("murmur64", 8);
		ENSURE(a._id == 0x90631502d1a3432bu);

		TempAllocator64 ta;
		DynamicString str(ta);
		a.to_string(str);
		ENSURE(strcmp(str.c_str(), "90631502d1a3432b") == 0);
	}
	memory_globals::shutdown();
}

static void test_dynamic_string()
{
	memory_globals::init();
	{
		TempAllocator1024 ta;
		DynamicString str(ta);
		ENSURE(str.empty());

		str.set("murmur32", 8);
		ENSURE(str.length() == 8);

		const StringId32 id = str.to_string_id();
		ENSURE(id._id == 0x7c2365dbu);
	}
	{
		TempAllocator1024 ta;
		DynamicString str(ta);
		str += "Test ";
		str += "string.";
		ENSURE(strcmp(str.c_str(), "Test string.") == 0);
	}
	{
		TempAllocator1024 ta;
		DynamicString str(ta);
		str.set("   \tSushi\t   ", 13);
		str.ltrim();
		ENSURE(strcmp(str.c_str(), "Sushi\t   ") == 0);
	}
	{
		TempAllocator1024 ta;
		DynamicString str(ta);
		str.set("   \tSushi\t   ", 13);
		str.rtrim();
		ENSURE(strcmp(str.c_str(), "   \tSushi") == 0);
	}
	{
		TempAllocator1024 ta;
		DynamicString str(ta);
		str.set("   \tSushi\t   ", 13);
		str.trim();
		ENSURE(strcmp(str.c_str(), "Sushi") == 0);
	}
	{
		TempAllocator1024 ta;
		DynamicString str(ta);
		str.set("Hello everyone!", 15);
		ENSURE(str.has_prefix("Hello"));
		ENSURE(!str.has_prefix("hello"));
		ENSURE(str.has_suffix("one!"));
		ENSURE(!str.has_suffix("one"));
		ENSURE(!str.has_prefix("Hello everyone!!!"));
		ENSURE(!str.has_suffix("Hello everyone!!!"));
	}
	memory_globals::shutdown();
}

static void test_guid()
{
	memory_globals::init();
	{
		Guid guid = guid::new_guid();
		TempAllocator1024 ta;
		DynamicString str(ta);
		guid::to_string(guid, str);
		Guid parsed = guid::parse(str.c_str());
		ENSURE(guid == parsed);
	}
	{
		Guid guid;
		ENSURE(guid::try_parse(guid, "961f8005-6a7e-4371-9272-8454dd786884"));
		ENSURE(!guid::try_parse(guid, "961f80056a7e-4371-9272-8454dd786884"));
	}
	memory_globals::shutdown();
}

static void test_json()
{
	memory_globals::init();
	{
		JsonValueType::Enum t = json::type("null");
		ENSURE(t == JsonValueType::NIL);
	}
	{
		JsonValueType::Enum t = json::type("true");
		ENSURE(t == JsonValueType::BOOL);
	}
	{
		JsonValueType::Enum t = json::type("false");
		ENSURE(t == JsonValueType::BOOL);
	}
	{
		JsonValueType::Enum t = json::type("3.14");
		ENSURE(t == JsonValueType::NUMBER);
	}
	{
		JsonValueType::Enum t = json::type("\"foo\"");
		ENSURE(t == JsonValueType::STRING);
	}
	{
		JsonValueType::Enum t = json::type("[]");
		ENSURE(t == JsonValueType::ARRAY);
	}
	{
		JsonValueType::Enum t = json::type("{}");
		ENSURE(t == JsonValueType::OBJECT);
	}
	{
		const s32 a = json::parse_int("3.14");
		ENSURE(a == 3);
	}
	{
		const f32 a = json::parse_float("3.14");
		ENSURE(fequal(a, 3.14f));
	}
	{
		const bool a = json::parse_bool("true");
		ENSURE(a == true);
	}
	{
		const bool a = json::parse_bool("false");
		ENSURE(a == false);
	}
	{
		TempAllocator1024 ta;
		DynamicString str(ta);
		json::parse_string("\"This is JSON\"", str);
		ENSURE(strcmp(str.c_str(), "This is JSON") == 0);
	}
	memory_globals::shutdown();
}

static void test_sjson()
{
	memory_globals::init();
	{
		JsonValueType::Enum t = sjson::type("null");
		ENSURE(t == JsonValueType::NIL);
	}
	{
		JsonValueType::Enum t = sjson::type("true");
		ENSURE(t == JsonValueType::BOOL);
	}
	{
		JsonValueType::Enum t = sjson::type("false");
		ENSURE(t == JsonValueType::BOOL);
	}
	{
		JsonValueType::Enum t = sjson::type("3.14");
		ENSURE(t == JsonValueType::NUMBER);
	}
	{
		JsonValueType::Enum t = sjson::type("\"foo\"");
		ENSURE(t == JsonValueType::STRING);
	}
	{
		JsonValueType::Enum t = sjson::type("[]");
		ENSURE(t == JsonValueType::ARRAY);
	}
	{
		JsonValueType::Enum t = sjson::type("{}");
		ENSURE(t == JsonValueType::OBJECT);
	}
	{
		const s32 a = sjson::parse_int("3.14");
		ENSURE(a == 3);
	}
	{
		const f32 a = sjson::parse_float("3.14");
		ENSURE(fequal(a, 3.14f));
	}
	{
		const bool a = sjson::parse_bool("true");
		ENSURE(a == true);
	}
	{
		const bool a = sjson::parse_bool("false");
		ENSURE(a == false);
	}
	{
		TempAllocator1024 ta;
		DynamicString str(ta);
		sjson::parse_string("\"This is JSON\"", str);
		ENSURE(strcmp(str.c_str(), "This is JSON") == 0);
	}
	{
		const Vector2 a = sjson::parse_vector2("[ 1.2 -2.5 ]");
		ENSURE(fequal(a.x,  1.2f));
		ENSURE(fequal(a.y, -2.5f));
	}
	{
		const Vector3 a = sjson::parse_vector3("[ 3.1 0.5 -5.7]");
		ENSURE(fequal(a.x,  3.1f));
		ENSURE(fequal(a.y,  0.5f));
		ENSURE(fequal(a.z, -5.7f));
	}
	{
		const Vector4 a = sjson::parse_vector4("[ 6.7 -1.3 2.9 -0.4 ]");
		ENSURE(fequal(a.x,  6.7f));
		ENSURE(fequal(a.y, -1.3f));
		ENSURE(fequal(a.z,  2.9f));
		ENSURE(fequal(a.w, -0.4f));
	}
	{
		const Quaternion a = sjson::parse_quaternion("[ -1.5 -3.4 9.1 -3.5 ]");
		ENSURE(fequal(a.x, -1.5f));
		ENSURE(fequal(a.y, -3.4f));
		ENSURE(fequal(a.z,  9.1f));
		ENSURE(fequal(a.w, -3.5f));
	}
	{
		const Matrix4x4 a = sjson::parse_matrix4x4(
			"["
			"-3.2  5.3 -0.7  4.1 "
			" 5.6  7.0 -3.2 -1.2 "
			"-6.3  9.0  3.9  1.1 "
			" 0.4 -7.3  8.9 -0.1 "
			"]"
			);
		ENSURE(fequal(a.x.x, -3.2f));
		ENSURE(fequal(a.x.y,  5.3f));
		ENSURE(fequal(a.x.z, -0.7f));
		ENSURE(fequal(a.x.w,  4.1f));
		ENSURE(fequal(a.y.x,  5.6f));
		ENSURE(fequal(a.y.y,  7.0f));
		ENSURE(fequal(a.y.z, -3.2f));
		ENSURE(fequal(a.y.w, -1.2f));
		ENSURE(fequal(a.z.x, -6.3f));
		ENSURE(fequal(a.z.y,  9.0f));
		ENSURE(fequal(a.z.z,  3.9f));
		ENSURE(fequal(a.z.w,  1.1f));
		ENSURE(fequal(a.t.x,  0.4f));
		ENSURE(fequal(a.t.y, -7.3f));
		ENSURE(fequal(a.t.z,  8.9f));
		ENSURE(fequal(a.t.w, -0.1f));
	}
	{
		const StringId32 a = sjson::parse_string_id("\"murmur32\"");
		ENSURE(a._id == 0x7c2365dbu);
	}
	{
		const ResourceId a = sjson::parse_resource_id("\"murmur64\"");
		ENSURE(a._id == 0x90631502d1a3432bu);
	}
	{
		const Guid guid = guid::parse("0f6c3b1c-9cba-4282-9096-2a77ca047b1b");
		const Guid parsed = sjson::parse_guid("\"0f6c3b1c-9cba-4282-9096-2a77ca047b1b\"");
		ENSURE(guid == parsed);
	}
	{
		TempAllocator128 ta;
		DynamicString str(ta);
		sjson::parse_verbatim("\"\"\"verbatim\"\"\"", str);
		ENSURE(strcmp(str.c_str(), "verbatim") == 0);
	}
	memory_globals::shutdown();
}

static void test_path()
{
#if DAWN_PLATFORM_POSIX
	{
		const bool a = path::is_absolute("/home/foo");
		ENSURE(a == true);
		const bool b = path::is_absolute("home/foo");
		ENSURE(b == false);
	}
	{
		const bool a = path::is_relative("/home/foo");
		ENSURE(a == false);
		const bool b = path::is_relative("home/foo");
		ENSURE(b == true);
	}
	{
		const bool a = path::is_root("/");
		ENSURE(a == true);
		const bool b = path::is_root("/home");
		ENSURE(b == false);
	}
	{
		TempAllocator128 ta;
		DynamicString path(ta);
		path::join(path, "/home", "foo");
		ENSURE(path == "/home/foo");
		path::join(path, "/home", "bar");
		ENSURE(path == "/home/bar");
	}
	{
		ENSURE(path::has_trailing_separator("/home/foo/"));
		ENSURE(!path::has_trailing_separator("/home/foo"));
	}
	{
		TempAllocator128 ta;
		DynamicString clean(ta);
		path::reduce(clean, "/home//foo/");
		ENSURE(clean == "/home/foo");
	}
	{
		TempAllocator128 ta;
		DynamicString clean(ta);
		path::reduce(clean, "\\home\\\\foo\\");
		ENSURE(clean == "/home/foo");
	}
#else
	{
		const bool a = path::is_absolute("C:\\Users\\foo");
		ENSURE(a == true);
		const bool b = path::is_absolute("Users\\foo");
		ENSURE(b == false);
	}
	{
		const bool a = path::is_relative("D:\\Users\\foo");
		ENSURE(a == false);
		const bool b = path::is_relative("Users\\foo");
		ENSURE(b == true);
	}
	{
		const bool a = path::is_root("E:\\");
		ENSURE(a == true);
		const bool b = path::is_root("E:\\Users");
		ENSURE(b == false);
	}
	{
		TempAllocator128 ta;
		DynamicString path(ta);
		path::join(path, "C:\\Users", "foo");
		ENSURE(path == "C:\\Users\\foo");
		path::join(path, "C:\\Users", "bar");
		ENSURE(path == "C:\\Users\\bar");
	}
	{
		ENSURE(path::has_trailing_separator("C:\\Users\\foo\\"));
		ENSURE(!path::has_trailing_separator("C:\\Users\\foo"));
	}
	{
		TempAllocator128 ta;
		DynamicString clean(ta);
		path::reduce(clean, "C:\\Users\\\\foo\\");
		ENSURE(clean == "C:\\Users\\foo");
	}
	{
		TempAllocator128 ta;
		DynamicString clean(ta);
		path::reduce(clean, "C:/Users//foo/");
		ENSURE(clean == "C:\\Users\\foo");
	}
#endif // DAWN_PLATFORM_POSIX
	{
		const char* p = path::basename("");
		ENSURE(strcmp(p, "") == 0);
		const char* q = path::basename("/");
		ENSURE(strcmp(q, "") == 0);
		const char* r = path::basename("boot.config");
		ENSURE(strcmp(r, "boot.config") == 0);
		const char* s = path::basename("foo/boot.config");
		ENSURE(strcmp(s, "boot.config") == 0);
		const char* t = path::basename("/foo/boot.config");
		ENSURE(strcmp(t, "boot.config") == 0);
	}
	{
		const char* p = path::extension("");
		ENSURE(p == NULL);
		const char* q = path::extension("boot");
		ENSURE(q == NULL);
		const char* r = path::extension("boot.bar.config");
		ENSURE(strcmp(r, "config") == 0);
	}
}

static void test_command_line()
{
	const char* argv[] =
	{
		"args",
		"-s",
		"--switch",
		"--argument",
		"orange"
	};

	CommandLine cl(countof(argv), argv);
	ENSURE(cl.has_option("switch", 's'));
	const char* orange = cl.get_parameter(0, "argument");
	ENSURE(orange != NULL && strcmp(orange, "orange") == 0);
}

static void test_thread()
{
	Thread thread;
	ENSURE(!thread.is_running());

	thread.start([](void*) { return 0; }, NULL);
	thread.stop();
	ENSURE(thread.exit_code() == 0);

	thread.start([](void*) { return -1; }, NULL);
	thread.stop();
	ENSURE(thread.exit_code() == -1);
}

int main_unit_tests()
{
	test_memory();
	test_array();
	test_vector();
	test_hash_map();
	test_hash_set();
	test_vector2();
	test_vector3();
	test_vector4();
	test_quaternion();
	test_color4();
	test_matrix3x3();
	test_matrix4x4();
	test_aabb();
	test_sphere();
	test_murmur();
	test_string_id();
	test_dynamic_string();
	test_guid();
	test_json();
	test_sjson();
	test_path();
	test_command_line();
	test_thread();

	return EXIT_SUCCESS;
}

} // namespace dawn

#endif // DAWN_BUILD_UNIT_TESTS
