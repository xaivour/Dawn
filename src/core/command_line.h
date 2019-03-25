/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

namespace dawn
{
/// Helper for parsing command line.
///
/// @ingroup Core
struct CommandLine
{
	int _argc;
	const char** _argv;

	/// Constructor.
	CommandLine(int argc, const char** argv);

	/// Returns the i-th parameter of the option identified by
	/// @a longopt or @a shortopt, or NULL if the parameter does not exist.
	const char* get_parameter(int i, const char* longopt, char shortopt = '\0');

	/// Returns whether the command line has the option identified
	/// by @a longopt or @a shortopt.
	bool has_option(const char* longopt, char shortopt = '\0');
};

} // namespace dawn
