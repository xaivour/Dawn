/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/containers/types.h"
#include "core/filesystem/types.h"
#include "core/strings/types.h"

namespace dawn
{
/// Provides a platform-independent way to access files and directories
/// on the host filesystem.
///
/// @ingroup Filesystem
struct Filesystem
{
	Filesystem() {};
	virtual ~Filesystem() {};
	Filesystem(const Filesystem&) = delete;
	Filesystem& operator=(const Filesystem&) = delete;

	/// Opens the file at the given @a path with the given @a mode.
	virtual File* open(const char* path, FileOpenMode::Enum mode) = 0;

	/// Closes the given @a file.
	virtual void close(File& file) = 0;

	/// Returns whether @a path exists.
	virtual bool exists(const char* path) = 0;

	/// Returns true if @a path is a directory.
	virtual bool is_directory(const char* path) = 0;

	/// Returns true if @a path is a regular file.
	virtual bool is_file(const char* path) = 0;

	/// Returns the time of last modify operaton to @a path.
	virtual u64 last_modified_time(const char* path) = 0;

	/// Creates the directory at the given @a path.
	virtual void create_directory(const char* path) = 0;

	/// Deletes the directory at the given @a path.
	virtual void delete_directory(const char* path) = 0;

	/// Deletes the file at the given @a path.
	virtual void delete_file(const char* path) = 0;

	/// Returns the relative file names in the given @a path.
	virtual void list_files(const char* path, Vector<DynamicString>& files) = 0;

	/// Returns the absolute path of the given @a path based on
	/// the root path of the file source. If @a path is absolute,
	/// the given path is returned.
	virtual void get_absolute_path(const char* path, DynamicString& os_path) = 0;
};

} // namespace dawn
