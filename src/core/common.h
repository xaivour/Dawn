#pragma once

#include <cstdint>

#include "log.h"

namespace Dawn
{
	typedef int8_t int8;
	typedef int16_t int16;
	typedef int32_t int32;
	typedef uint8_t uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
}

#define DAWN_NULL_COPY_AND_ASSIGN(T) \
	T(const T& other) {(void)other;} \
	void operator=(const T& other) { (void)other; }

#define DAWN_UNUSED(x) (void)x