/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/thread/types.h"

namespace dawn
{
/// Condition variable.
///
/// @ingroup Thread
struct ConditionVariable
{
	DE_ALIGN_DECL(16, u8 _data[64]);

	///
	ConditionVariable();

	///
	~ConditionVariable();

	///
	ConditionVariable(const ConditionVariable&) = delete;

	///
	ConditionVariable& operator=(const ConditionVariable&) = delete;

	///
	void wait(Mutex& mutex);

	///
	void signal();
};

} // namespace dawn
