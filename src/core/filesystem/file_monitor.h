/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/memory/types.h"
#include "core/types.h"

namespace dawn
{
/// Enumerates file monitor events.
///
/// @ingroup Filesystem
struct FileMonitorEvent
{
	enum Enum
	{
		CREATED,
		DELETED,
		RENAMED,
		CHANGED,

		COUNT
	};
};

typedef void (*FileMonitorFunction)(void* user_data, FileMonitorEvent::Enum fme, bool is_dir, const char* path, const char* path_modified);

struct FileMonitorImpl;

/// Watches a directory for changes.
///
/// @ingroup Filesystem
struct FileMonitor
{
	FileMonitorImpl* _impl;

	FileMonitor(Allocator& a);
	~FileMonitor();

	// Starts monitoring the @a path.
	void start(const char* path, bool recursive, FileMonitorFunction fmf, void* user_data);

	// Stops monitoring.
	void stop();
};

} // namespace dawn
