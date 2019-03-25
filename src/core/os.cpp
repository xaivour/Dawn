/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#include "core/containers/vector.h"
#include "core/error/error.h"
#include "core/memory/temp_allocator.h"
#include "core/os.h"
#include "core/platform.h"
#include "core/strings/dynamic_string.h"
#include "core/strings/string_stream.h"
#include <string.h>   // strcmp
#include <sys/stat.h> // stat, mkdir

#if DAWN_PLATFORM_POSIX
	#include <dirent.h>   // opendir, readdir
	#include <dlfcn.h>    // dlopen, dlclose, dlsym
	#include <errno.h>
	#include <stdio.h>    // fputs
	#include <stdlib.h>   // getenv
	#include <string.h>   // memset
	#include <sys/wait.h> // wait
	#include <time.h>     // clock_gettime
	#include <unistd.h>   // unlink, rmdir, getcwd, fork, execv
#elif DAWN_PLATFORM_WINDOWS
	#include <io.h>
	#include <stdio.h>
	#include <windows.h>
#endif
#if DAWN_PLATFORM_ANDROID
	#include <android/log.h>
#endif

namespace dawn
{
namespace os
{
	void sleep(u32 ms)
	{
#if DAWN_PLATFORM_POSIX
		usleep(ms * 1000);
#elif DAWN_PLATFORM_WINDOWS
		Sleep(ms);
#endif
	}

	void* library_open(const char* path)
	{
#if DAWN_PLATFORM_POSIX
		return ::dlopen(path, RTLD_LAZY);
#elif DAWN_PLATFORM_WINDOWS
		return (void*)LoadLibraryA(path);
#endif
	}

	void library_close(void* library)
	{
#if DAWN_PLATFORM_POSIX
		dlclose(library);
#elif DAWN_PLATFORM_WINDOWS
		FreeLibrary((HMODULE)library);
#endif
	}

	void* library_symbol(void* library, const char* name)
	{
#if DAWN_PLATFORM_POSIX
		return ::dlsym(library, name);
#elif DAWN_PLATFORM_WINDOWS
		return (void*)GetProcAddress((HMODULE)library, name);
#endif
	}

	void log(const char* msg)
	{
#if DAWN_PLATFORM_ANDROID
		__android_log_write(ANDROID_LOG_DEBUG, "dawn", msg);
#elif DAWN_PLATFORM_WINDOWS
		OutputDebugStringA(msg);
#else
		fputs(msg, stdout);
		fflush(stdout);
#endif
	}

	void stat(Stat& info, const char* path)
	{
		info.file_type = Stat::NO_ENTRY;
		info.size = 0;
		info.mtime = 0;

#if DAWN_PLATFORM_POSIX
		struct stat buf;
		memset(&buf, 0, sizeof(buf));
		int err = ::stat(path, &buf);
		if (err != 0)
			return;

		if (S_ISREG(buf.st_mode) == 1)
			info.file_type = Stat::REGULAR;
		else if (S_ISDIR(buf.st_mode) == 1)
			info.file_type = Stat::DIRECTORY;
#elif DAWN_PLATFORM_WINDOWS
		struct _stat64 buf;
		int err = ::_stat64(path, &buf);
		if (err != 0)
			return;

		if ((buf.st_mode & _S_IFREG) != 0)
			info.file_type = Stat::REGULAR;
		else if ((buf.st_mode & _S_IFDIR) != 0)
			info.file_type = Stat::DIRECTORY;
#endif

		info.size = buf.st_size;
		info.mtime = buf.st_mtime;
	}

	void delete_file(const char* path)
	{
#if DAWN_PLATFORM_POSIX
		int err = ::unlink(path);
		DE_ASSERT(err == 0, "unlink: errno = %d", errno);
		DE_UNUSED(err);
#elif DAWN_PLATFORM_WINDOWS
		BOOL err = DeleteFile(path);
		DE_ASSERT(err != 0, "DeleteFile: GetLastError = %d", GetLastError());
		DE_UNUSED(err);
#endif
	}

	void create_directory(const char* path)
	{
#if DAWN_PLATFORM_POSIX
		int err = ::mkdir(path, 0755);
		DE_ASSERT(err == 0, "mkdir: errno = %d", errno);
		DE_UNUSED(err);
#elif DAWN_PLATFORM_WINDOWS
		BOOL err = CreateDirectory(path, NULL);
		DE_ASSERT(err != 0, "CreateDirectory: GetLastError = %d", GetLastError());
		DE_UNUSED(err);
#endif
	}

	void delete_directory(const char* path)
	{
#if DAWN_PLATFORM_POSIX
		int err = ::rmdir(path);
		DE_ASSERT(err == 0, "rmdir: errno = %d", errno);
		DE_UNUSED(err);
#elif DAWN_PLATFORM_WINDOWS
		BOOL err = RemoveDirectory(path);
		DE_ASSERT(err != 0, "RemoveDirectory: GetLastError = %d", GetLastError());
		DE_UNUSED(err);
#endif
	}

	const char* getcwd(char* buf, u32 size)
	{
#if DAWN_PLATFORM_POSIX
		return ::getcwd(buf, size);
#elif DAWN_PLATFORM_WINDOWS
		GetCurrentDirectory(size, buf);
		return buf;
#endif
	}

	const char* getenv(const char* name)
	{
#if DAWN_PLATFORM_POSIX
		return ::getenv(name);
#elif DAWN_PLATFORM_WINDOWS
		// GetEnvironmentVariable(name, buf, size);
		return NULL;
#endif
	}

	void list_files(const char* path, Vector<DynamicString>& files)
	{
#if DAWN_PLATFORM_POSIX
		DIR *dir;
		struct dirent *entry;

		if (!(dir = opendir(path)))
			return;

		while ((entry = readdir(dir)))
		{
			const char* dname = entry->d_name;

			if (!strcmp(dname, ".") || !strcmp(dname, ".."))
				continue;

			TempAllocator512 ta;
			DynamicString fname(ta);
			fname.set(dname, strlen32(dname));
			vector::push_back(files, fname);
		}

		closedir(dir);
#elif DAWN_PLATFORM_WINDOWS
		TempAllocator1024 ta;
		DynamicString cur_path(ta);
		cur_path += path;
		cur_path += "\\*";

		WIN32_FIND_DATA ffd;
		HANDLE file = FindFirstFile(cur_path.c_str(), &ffd);
		if (file == INVALID_HANDLE_VALUE)
			return;

		do
		{
			const char* dname = ffd.cFileName;

			if (!strcmp(dname, ".") || !strcmp(dname, ".."))
				continue;

			TempAllocator512 ta;
			DynamicString fname(ta);
			fname.set(dname, strlen32(dname));
			vector::push_back(files, fname);
		}
		while (FindNextFile(file, &ffd) != 0);

		FindClose(file);
#endif
	}

	int execute_process(const char* const* argv, StringStream& output)
	{
		TempAllocator512 ta;
		StringStream path(ta);

		path << argv[0];
		path << ' ';
#if DAWN_PLATFORM_POSIX
		path << "2>&1 ";
#endif
		for (s32 i = 1; argv[i] != NULL; ++i)
		{
			const char* arg = argv[i];
			for (; *arg; ++arg)
			{
				if (*arg == ' ')
					path << '\\';
				path << *arg;
			}
			path << ' ';
		}
#if DAWN_PLATFORM_POSIX
		FILE* file = popen(string_stream::c_str(path), "r");

		char buf[1024];
		while (fgets(buf, sizeof(buf), file) != NULL)
			output << buf;

		return pclose(file);
#elif DAWN_PLATFORM_WINDOWS
		STARTUPINFO info;
		memset(&info, 0, sizeof(info));
		info.cb = sizeof(info);

		PROCESS_INFORMATION process;
		memset(&process, 0, sizeof(process));

		int err = CreateProcess(argv[0]
			, (LPSTR)string_stream::c_str(path)
			, NULL
			, NULL
			, FALSE
			, CREATE_NO_WINDOW
			, NULL
			, NULL
			, &info
			, &process
			);
		DE_ASSERT(err != 0, "CreateProcess: GetLastError = %d", GetLastError());
		DE_UNUSED(err);

		DWORD exitcode = 1;
		::WaitForSingleObject(process.hProcess, INFINITE);
		GetExitCodeProcess(process.hProcess, &exitcode);
		CloseHandle(process.hProcess);
		CloseHandle(process.hThread);
		return (int)exitcode;
#endif
	}

} // namespace os

} // namespace dawn
