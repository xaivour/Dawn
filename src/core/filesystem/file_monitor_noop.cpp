/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#include "core/platform.h"

#if !DAWN_PLATFORM_LINUX

#include "core/filesystem/file_monitor.h"

namespace dawn
{
FileMonitor::FileMonitor(Allocator& /*a*/)
	: _impl(NULL)
{
}

FileMonitor::~FileMonitor()
{
}

void FileMonitor::start(const char* /*path*/, bool /*recursive*/, FileMonitorFunction /*fmf*/, void* /*user_data*/)
{
}

void FileMonitor::stop()
{
}

} // namespace dawn

#endif // !DAWN_PLATFORM_LINUX

