/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/thread/semaphore.h"
#include "core/types.h"

/// @defgroup Thread Thread
/// @ingroup Core
namespace dawn
{
/// Thread.
///
/// @ingroup Thread.
struct Thread
{
	typedef s32 (*ThreadFunction)(void* data);

	ThreadFunction _function;
	void* _user_data;
	Semaphore _sem;
	bool _is_running;
	s32 _exit_code;
	DE_ALIGN_DECL(16, u8 _data[32]);

	///
	Thread();

	///
	~Thread();

	///
	Thread(const Thread&) = delete;

	///
	Thread& operator=(const Thread&) = delete;

	///
	void start(ThreadFunction func, void* user_data = NULL, u32 stack_size = 0);

	///
	void stop();

	///
	bool is_running();

	///
	s32 exit_code();
};

} // namespace dawn
