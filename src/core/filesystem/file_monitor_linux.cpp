/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#include "core/platform.h"

#if DAWN_PLATFORM_LINUX

#include "core/containers/hash_map.h"
#include "core/filesystem/file_monitor.h"
#include "core/filesystem/path.h"
#include "core/memory/temp_allocator.h"
#include "core/strings/dynamic_string.h"
#include "core/thread/thread.h"
#include <dirent.h> // opendir, readdir
#include <errno.h>
#include <limits.h> // NAME_MAX
#include <sys/inotify.h>
#include <sys/select.h>
#include <unistd.h> // read

namespace dawn
{
template <>
struct hash<DynamicString>
{
	u32 operator()(const DynamicString& str) const
	{
		return str.to_string_id()._id;
	}
};

struct FileMonitorImpl
{
	Allocator* _allocator;
	int _fd;
	HashMap<int, DynamicString> _watches;
	HashMap<DynamicString, int> _watches_reverse;
	Thread _thread;
	bool _exit;
	bool _recursive;
	FileMonitorFunction _function;
	void* _user_data;

	FileMonitorImpl(Allocator& a)
		: _allocator(&a)
		, _fd(0)
		, _watches(a)
		, _watches_reverse(a)
		, _exit(false)
		, _recursive(false)
		, _function(NULL)
		, _user_data(NULL)
	{
	}

	void add_watch(const char* path, bool recursive)
	{
		DE_ENSURE(path != NULL);
		DE_ASSERT(!path::has_trailing_separator(path), "Malformed path");

		int wd = inotify_add_watch(_fd
			, path
			, IN_CREATE
			| IN_DELETE      // File/directory deleted from watched directory.
			| IN_MODIFY
			| IN_ATTRIB
			| IN_MOVE
			| IN_DELETE_SELF // Watched file/directory was itself deleted.
			| IN_DONT_FOLLOW // Don't dereference pathname if it is a symbolic link.
			| IN_ONLYDIR     // Only watch pathname if it is a directory.
			);
		DE_ASSERT(wd != -1, "inotify_add_watch: errno: %d", errno);

		TempAllocator512 ta;
		DynamicString str(ta);
		str = path;
		hash_map::set(_watches, wd, str);
		hash_map::set(_watches_reverse, str, wd);

		// Add all sub-dirs recursively
		DIR *dir;
		struct dirent *entry;

		if (!(dir = opendir(path)))
			return;

		while ((entry = readdir(dir)))
		{
			const char* dname = entry->d_name;

			if (!strcmp(dname, ".") || !strcmp(dname, ".."))
				continue;

			// FIXME: some filesystems do not support DT_DIR.
			if (entry->d_type != DT_DIR)
				continue;

			TempAllocator512 ta;
			DynamicString str(ta);
			path::join(str, path, dname);

			add_watch(str.c_str(), recursive);
		}

		closedir(dir);
	}

	void start(const char* path, bool recursive, FileMonitorFunction fmf, void* user_data)
	{
		DE_ENSURE(NULL != path);
		DE_ENSURE(NULL != fmf);

		_recursive = recursive;
		_function = fmf;
		_user_data = user_data;

		_fd = inotify_init();
		DE_ASSERT(_fd != -1, "inotify_init: errno: %d", errno);

		add_watch(path, recursive);

		_thread.start(run, this);
	}

	void stop()
	{
		_exit = true;
		_thread.stop();

		close(_fd);
	}

	int watch()
	{
		while (!_exit)
		{
			TempAllocator512 ta;

			fd_set set;
			struct timeval timeout;
			u32 cookie = 0;
			u32 cookie_mask = 0;
			DynamicString cookie_path(ta);

			FD_ZERO(&set);
			FD_SET(_fd, &set);

			timeout.tv_sec = 0;
			timeout.tv_usec = 32*1000;

			// select returns 0 if timeout, 1 if input available, -1 if error.
			if (select(FD_SETSIZE, &set, NULL, NULL, &timeout) == 0)
				continue;

#define BUF_LEN sizeof(struct inotify_event) + NAME_MAX + 1
			char buf[BUF_LEN*16];
#undef BUF_LEN
			ssize_t len = read(_fd, buf, sizeof(buf));
			if (len == 0)
				return 0;
			if (len == -1)
				return -1;

			for (char* p = buf; p < buf + len;)
			{
				inotify_event* ev = (inotify_event*)p;

				if (ev->mask & IN_IGNORED)
				{
					// Watch was removed explicitly (inotify_rm_watch(2)) or
					// automatically (file was deleted, or filesystem was
					// unmounted).
					hash_map::remove(_watches, ev->wd);
				}
				if (ev->mask & IN_CREATE)
				{
					DynamicString path(ta);
					full_path(path, ev->wd, ev->name);

					_function(_user_data
						, FileMonitorEvent::CREATED
						, ev->mask & IN_ISDIR
						, path.c_str()
						, NULL
						);

					if (ev->mask & IN_ISDIR)
						add_watch(path.c_str(), _recursive);
				}
				if (ev->mask & IN_DELETE)
				{
					DynamicString path(ta);
					full_path(path, ev->wd, ev->name);

					_function(_user_data
						, FileMonitorEvent::DELETED
						, ev->mask & IN_ISDIR
						, path.c_str()
						, NULL
						);
				}
				if (ev->mask & IN_MODIFY || ev->mask & IN_ATTRIB)
				{
					DynamicString path(ta);
					full_path(path, ev->wd, ev->name);

					_function(_user_data
						, FileMonitorEvent::CHANGED
						, ev->mask & IN_ISDIR
						, path.c_str()
						, NULL
						);
				}
				if (ev->mask & IN_MOVED_FROM)
				{
					// Two consecutive IN_MOVED_FROM
					if (cookie != 0)
					{
						_function(_user_data
							, FileMonitorEvent::DELETED
							, cookie_mask & IN_ISDIR
							, cookie_path.c_str()
							, NULL
							);

						u32 wd = hash_map::get(_watches_reverse, cookie_path, INT32_MAX);
						hash_map::remove(_watches_reverse, cookie_path);
						inotify_rm_watch(_fd, wd);

						cookie = 0;
						cookie_mask = 0;
					}
					else
					{
						DynamicString path(ta);
						full_path(path, ev->wd, ev->name);

						cookie = ev->cookie;
						cookie_mask = ev->mask;
						cookie_path = path;
					}
				}
				if (ev->mask & IN_MOVED_TO)
				{
					if (cookie == ev->cookie)
					{
						// File or directory has been renamed
						DynamicString path(ta);
						full_path(path, ev->wd, ev->name);

						_function(_user_data
							, FileMonitorEvent::RENAMED
							, ev->mask & IN_ISDIR
							, cookie_path.c_str()
							, path.c_str()
							);

						cookie = 0;
						cookie_mask = 0;

						if (ev->mask & IN_ISDIR)
						{
							u32 wd = hash_map::get(_watches_reverse, cookie_path, INT32_MAX);
							hash_map::remove(_watches_reverse, cookie_path);
							inotify_rm_watch(_fd, wd);

							add_watch(path.c_str(), _recursive);
						}
					}
					else
					{
						// File or directory was moved to this folder
						DynamicString path(ta);
						full_path(path, ev->wd, ev->name);

						_function(_user_data
							, FileMonitorEvent::CREATED
							, ev->mask & IN_ISDIR
							, path.c_str()
							, NULL
							);

						cookie = 0;
						cookie_mask = 0;

						if (ev->mask & IN_ISDIR)
							add_watch(path.c_str(), _recursive);
					}
				}

				p += sizeof(inotify_event) + ev->len;
			}

			// Unpaired IN_MOVE_TO
			if (cookie != 0)
			{
				_function(_user_data
					, FileMonitorEvent::DELETED
					, cookie_mask & IN_ISDIR
					, cookie_path.c_str()
					, NULL
					);

				u32 wd = hash_map::get(_watches_reverse, cookie_path, INT32_MAX);
				hash_map::remove(_watches_reverse, cookie_path);
				inotify_rm_watch(_fd, wd);

				cookie = 0;
				cookie_mask = 0;
			}
		}

		return 0;
	}

	void full_path(DynamicString& path, int wd, const char* name)
	{
		TempAllocator512 ta;
		DynamicString path_base(ta);
		path_base = hash_map::get(_watches, wd, path_base);
		path::join(path, path_base.c_str(), name);
	}

	static int run(void* thiz)
	{
		return ((FileMonitorImpl*)thiz)->watch();
	}
};

FileMonitor::FileMonitor(Allocator& a)
{
	_impl = DE_NEW(a, FileMonitorImpl)(a);
}

FileMonitor::~FileMonitor()
{
	DE_DELETE(*_impl->_allocator, _impl);
}

void FileMonitor::start(const char* path, bool recursive, FileMonitorFunction fmf, void* user_data)
{
	_impl->start(path, recursive, fmf, user_data);
}

void FileMonitor::stop()
{
	_impl->stop();
}

} // namespace dawn

#endif // DAWN_PLATFORM_LINUX
