/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/types.h"

namespace dawn
{
/// @addtogroup Math
/// @{

static const f32 PI            = 3.14159265358979323846f;
static const f32 PI_TWO        = 6.28318530717958647693f;
static const f32 PI_HALF       = 1.57079632679489661923f;
static const f32 FLOAT_EPSILON = 1.0e-7f;

/// Returns whether @a a and @a b are equal according to @a epsilon.
bool fequal(f32 a, f32 b, f32 epsilon = FLOAT_EPSILON);

/// Returns the fractional part of @a a.
f32 ffract(f32 a);

/// Returns the absolute value of @a a.
f32 fabs(f32 a);

/// Returns the sine of @a a.
f32 fsin(f32 a);

/// Returns the cosine of @a a.
f32 fcos(f32 a);

/// Returns the arc cosine of @a a.
f32 facos(f32 a);

/// Returns the tangent of @a a.
f32 ftan(f32 a);

/// Returns the nonnegative square root of @a a.
f32 fsqrt(f32 a);

/// Returns @a deg in radians.
f32 frad(f32 deg);

/// Returns @a rad in degrees.
f32 fdeg(f32 rad);

/// Returns the linear interpolated value between @a p0 and @a p1 at time @a t
f32 lerp(const f32 p0, const f32 p1, f32 t);

/// Returns the cosine interpolated value between @a p0 and @a p1 at time @a t
f32 cosine(const f32 p0, const f32 p1, f32 t);

/// Returns the cubic interpolated value between @a p0 and @a p1 at time @a t
f32 cubic(const f32 p0, const f32 p1, f32 t);

/// Bezier interpolation
f32 bezier(const f32 p0, const f32 p1, const f32 p2, const f32 p3, f32 t);

/// Catmull-Rom interpolation
f32 catmull_rom(const f32 p0, const f32 p1, const f32 p2, const f32 p3, f32 t);

/// @}

} // namespace dawn
