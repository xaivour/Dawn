/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/error/error.h"
#include "core/math/matrix4x4.h"
#include "core/math/types.h"
#include "core/math/vector3.h"

namespace dawn
{
/// Functions to manipulate AABB.
///
/// @ingroup Math
namespace aabb
{
	/// Resets the AABB @a b.
	void reset(AABB& b);

	/// Creates the AABB @a a from @a num @a points.
	void from_points(AABB& a, u32 num, u32 stride, const void* points);

	/// Creates the AABB @a a from @a num @a points.
	void from_points(AABB& b, u32 num, const Vector3* points);

	/// Creates the AABB @a a from @a num @a boxes.
	void from_boxes(AABB& b, u32 num, const AABB* boxes);

	/// Returns the center of the box @a b.
	Vector3 center(const AABB& b);

	/// Returns the radius of the box @a b.
	f32 radius(const AABB& b);

	/// Returns the volume of the box @a b.
	f32 volume(const AABB& b);

	/// Returns whether point @a p is contained in the box @a b.
	bool contains_point(const AABB& b, const Vector3& p);

	/// Returns the @a index -th vertex of the box.
	Vector3 vertex(const AABB& b, u32 index);

	/// Returns the box enclosing @a b transformed by @a m.
	AABB transformed(const AABB& b, const Matrix4x4& m);

	/// Returns the eight vertices of the box @a b.
	void to_vertices(const AABB& b, Vector3 v[8]);

	/// Returns the sphere enclosing the box @a b.
	Sphere to_sphere(const AABB& b);

} // namespace aabb

namespace aabb
{
	inline void reset(AABB& b)
	{
		b.min = VECTOR3_ZERO;
		b.max = VECTOR3_ZERO;
	}

	inline void from_points(AABB& b, u32 num, const Vector3* points)
	{
		aabb::from_points(b, num, sizeof(Vector3), points);
	}

	inline Vector3 center(const AABB& b)
	{
		return (b.min + b.max) * 0.5f;
	}

	inline f32 radius(const AABB& b)
	{
		return length(b.max - (b.min + b.max) * 0.5f);
	}

	inline f32 volume(const AABB& b)
	{
		return (b.max.x - b.min.x) * (b.max.y - b.min.y) * (b.max.z - b.min.z);
	}

	inline bool contains_point(const AABB& b, const Vector3& p)
	{
		return p.x > b.min.x
			&& p.y > b.min.y
			&& p.z > b.min.z
			&& p.x < b.max.x
			&& p.y < b.max.y
			&& p.z < b.max.z
			;
	}

	inline Vector3 vertex(const AABB& b, u32 index)
	{
		DE_ASSERT(index < 8, "Index out of bounds");

		switch (index)
		{
		case 0: return vector3(b.min.x, b.min.y, b.min.z);
		case 1: return vector3(b.max.x, b.min.y, b.min.z);
		case 2: return vector3(b.max.x, b.min.y, b.max.z);
		case 3: return vector3(b.min.x, b.min.y, b.max.z);
		case 4: return vector3(b.min.x, b.max.y, b.min.z);
		case 5: return vector3(b.max.x, b.max.y, b.min.z);
		case 6: return vector3(b.max.x, b.max.y, b.max.z);
		case 7: return vector3(b.min.x, b.max.y, b.max.z);
		default: return VECTOR3_ZERO;
		}
	}

	inline AABB transformed(const AABB& b, const Matrix4x4& m)
	{
		Vector3 vertices[8];
		to_vertices(b, vertices);

		vertices[0] = vertices[0] * m;
		vertices[1] = vertices[1] * m;
		vertices[2] = vertices[2] * m;
		vertices[3] = vertices[3] * m;
		vertices[4] = vertices[4] * m;
		vertices[5] = vertices[5] * m;
		vertices[6] = vertices[6] * m;
		vertices[7] = vertices[7] * m;

		AABB r;
		aabb::from_points(r, countof(vertices), vertices);
		return r;
	}

	inline void to_vertices(const AABB& b, Vector3 v[8])
	{
		// 7 ---- 6
		// |      |
		// |      |  <--- Top face
		// 4 ---- 5
		//
		// 3 ---- 2
		// |      |
		// |      |  <--- Bottom face
		// 0 ---- 1
		v[0].x = b.min.x;
		v[0].y = b.min.y;
		v[0].z = b.max.z;

		v[1].x = b.max.x;
		v[1].y = b.min.y;
		v[1].z = b.max.z;

		v[2].x = b.max.x;
		v[2].y = b.min.y;
		v[2].z = b.min.z;

		v[3].x = b.min.x;
		v[3].y = b.min.y;
		v[3].z = b.min.z;

		v[4].x = b.min.x;
		v[4].y = b.max.y;
		v[4].z = b.max.z;

		v[5].x = b.max.x;
		v[5].y = b.max.y;
		v[5].z = b.max.z;

		v[6].x = b.max.x;
		v[6].y = b.max.y;
		v[6].z = b.min.z;

		v[7].x = b.min.x;
		v[7].y = b.max.y;
		v[7].z = b.min.z;
	}

	inline Sphere to_sphere(const AABB& b)
	{
		Sphere s;
		s.c = aabb::center(b);
		s.r = aabb::radius(b);
		return s;
	}

} // namespace aabb

} // namespace dawn
