/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

/// @defgroup Filesystem Filesystem
/// @ingroup Core
namespace dawn
{
struct File;
struct FileMonitor;
struct Filesystem;

/// Enumerates file open modes.
///
/// @ingroup Filesystem
struct FileOpenMode
{
	enum Enum
	{
		READ,
		WRITE
	};
};

} // namespace dawn
