/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#include "core/error/error.h"
#include "core/platform.h"
#include "core/thread/thread.h"

#if DAWN_PLATFORM_POSIX
	#include <pthread.h>
#elif DAWN_PLATFORM_WINDOWS
	#include <windows.h>
	#include <process.h>
#endif

namespace dawn
{
struct Private
{
#if DAWN_PLATFORM_POSIX
	pthread_t handle;
#elif DAWN_PLATFORM_WINDOWS
	HANDLE handle;
#endif
};

#if DAWN_PLATFORM_POSIX
static void* thread_proc(void* arg)
{
	Thread* thread = (Thread*)arg;
	thread->_sem.post();
	return (void*)(uintptr_t)thread->_function(thread->_user_data);
}
#elif DAWN_PLATFORM_WINDOWS
static DWORD WINAPI thread_proc(void* arg)
{
	Thread* thread = (Thread*)arg;
	thread->_sem.post();
	return thread->_function(thread->_user_data);
}
#endif

Thread::Thread()
	: _function(NULL)
	, _user_data(NULL)
	, _is_running(false)
	, _exit_code(0)
{
	Private* priv = (Private*)_data;
	DE_STATIC_ASSERT(sizeof(_data) >= sizeof(Private));
#if DAWN_PLATFORM_POSIX
	priv->handle = 0;
#elif DAWN_PLATFORM_WINDOWS
	priv->handle = INVALID_HANDLE_VALUE;
#endif
}

Thread::~Thread()
{
	if (_is_running)
		stop();
}

void Thread::start(ThreadFunction func, void* user_data, u32 stack_size)
{
	Private* priv = (Private*)_data;

	DE_ASSERT(!_is_running, "Thread is already running");
	DE_ASSERT(func != NULL, "Function must be != NULL");
	_function = func;
	_user_data = user_data;

#if DAWN_PLATFORM_POSIX
	pthread_attr_t attr;
	int err = pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	DE_ASSERT(err == 0, "pthread_attr_init: errno = %d", err);

	if (stack_size != 0)
	{
		err = pthread_attr_setstacksize(&attr, stack_size);
		DE_ASSERT(err == 0, "pthread_attr_setstacksize: errno = %d", err);
	}

	err = pthread_create(&priv->handle, &attr, thread_proc, this);
	DE_ASSERT(err == 0, "pthread_create: errno = %d", err);

	err = pthread_attr_destroy(&attr);
	DE_ASSERT(err == 0, "pthread_attr_destroy: errno = %d", err);
	DE_UNUSED(err);
#elif DAWN_PLATFORM_WINDOWS
	priv->handle = CreateThread(NULL, stack_size, thread_proc, this, 0, NULL);
	DE_ASSERT(priv->handle != NULL, "CreateThread: GetLastError = %d", GetLastError());
#endif

	_is_running = true;
	_sem.wait();
}

void Thread::stop()
{
	Private* priv = (Private*)_data;

	DE_ASSERT(_is_running, "Thread is not running");

#if DAWN_PLATFORM_POSIX
	void* retval;
	int err = pthread_join(priv->handle, &retval);
	DE_ASSERT(err == 0, "pthread_join: errno = %d", err);
	DE_UNUSED(err);
	_exit_code = (s32)(uintptr_t)retval;
	priv->handle = 0;
#elif DAWN_PLATFORM_WINDOWS
	WaitForSingleObject(priv->handle, INFINITE);
	GetExitCodeThread(priv->handle, (DWORD*)&_exit_code);
	CloseHandle(priv->handle);
	priv->handle = INVALID_HANDLE_VALUE;
#endif

	_is_running = false;
}

bool Thread::is_running()
{
	return _is_running;
}

s32 Thread::exit_code()
{
	return _exit_code;
}

} // namespace dawn
