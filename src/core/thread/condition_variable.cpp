/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#include "core/error/error.h"
#include "core/platform.h"
#include "core/thread/condition_variable.h"
#include "core/thread/mutex.h"

#if DAWN_PLATFORM_POSIX
	#include <pthread.h>
#elif DAWN_PLATFORM_WINDOWS
	#include <windows.h>
#endif

namespace dawn
{
struct Private
{
#if DAWN_PLATFORM_POSIX
	pthread_cond_t cond;
#elif DAWN_PLATFORM_WINDOWS
	CONDITION_VARIABLE cv;
#endif
};

ConditionVariable::ConditionVariable()
{
	DE_STATIC_ASSERT(sizeof(_data) >= sizeof(Private));
	Private* priv = (Private*)_data;

#if DAWN_PLATFORM_POSIX
	int err = pthread_cond_init(&priv->cond, NULL);
	DE_ASSERT(err == 0, "pthread_cond_init: errno = %d", err);
	DE_UNUSED(err);
#elif DAWN_PLATFORM_WINDOWS
	InitializeConditionVariable(&priv->cv);
#endif
}

ConditionVariable::~ConditionVariable()
{
	Private* priv = (Private*)_data;

#if DAWN_PLATFORM_POSIX
	int err = pthread_cond_destroy(&priv->cond);
	DE_ASSERT(err == 0, "pthread_cond_destroy: errno = %d", err);
	DE_UNUSED(err);
#elif DAWN_PLATFORM_WINDOWS
	// Do nothing
#endif
}

void ConditionVariable::wait(Mutex& mutex)
{
	Private* priv = (Private*)_data;

#if DAWN_PLATFORM_POSIX
	int err = pthread_cond_wait(&priv->cond, (pthread_mutex_t*)mutex.native_handle());
	DE_ASSERT(err == 0, "pthread_cond_wait: errno = %d", err);
	DE_UNUSED(err);
#elif DAWN_PLATFORM_WINDOWS
	SleepConditionVariableCS(&priv->cv, (CRITICAL_SECTION*)mutex.native_handle(), INFINITE);
#endif
}

void ConditionVariable::signal()
{
	Private* priv = (Private*)_data;

#if DAWN_PLATFORM_POSIX
	int err = pthread_cond_signal(&priv->cond);
	DE_ASSERT(err == 0, "pthread_cond_signal: errno = %d", err);
	DE_UNUSED(err);
#elif DAWN_PLATFORM_WINDOWS
	WakeConditionVariable(&priv->cv);
#endif
}

} // namespace dawn
