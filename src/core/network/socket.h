/*
 * Copyright (c) 2012-2018 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/network/types.h"
#include "core/types.h"

namespace dawn
{
struct ConnectResult
{
	enum
	{
		SUCCESS,
		REFUSED,
		TIMEOUT,
		UNKNOWN
	} error;
};

struct BindResult
{
	enum
	{
		SUCCESS,
		ADDRESS_IN_USE,
		UNKNOWN
	} error;
};

struct AcceptResult
{
	enum
	{
		SUCCESS,
		NO_CONNECTION,
		UNKNOWN
	} error;
};

struct ReadResult
{
	enum
	{
		SUCCESS,
		WOULDBLOCK,
		REMOTE_CLOSED,
		TIMEOUT,
		UNKNOWN
	} error;
	u32 bytes_read;
};

struct WriteResult
{
	enum
	{
		SUCCESS,
		WOULDBLOCK,
		REMOTE_CLOSED,
		TIMEOUT,
		UNKNOWN
	} error;
	u32 bytes_wrote;
};

/// TCP socket
///
/// @ingroup Network
struct TCPSocket
{
	DE_ALIGN_DECL(16, u8 _data[8]);

	///
	TCPSocket();

	/// Closes the socket.
	void close();

	/// Connects to the @a ip address and @a port and returns the result.
	ConnectResult connect(const IPAddress& ip, u16 port);

	/// Binds the socket to @a port and returns the result.
	BindResult bind(u16 port);

	/// Listens for @a max socket connections.
	void listen(u32 max);

	/// Accepts a new connection @a c.
	AcceptResult accept(TCPSocket& c);

	/// Accepts a new connection @a c.
	AcceptResult accept_nonblock(TCPSocket& c);

	/// Reads @a size bytes and returns the result.
	ReadResult read(void* data, u32 size);

	/// Reads @a size bytes and returns the result.
	ReadResult read_nonblock(void* data, u32 size);

	/// Writes @a size bytes and returns the result.
	WriteResult write(const void* data, u32 size);

	/// Writes @a size bytes and returns the result.
	WriteResult write_nonblock(const void* data, u32 size);
};

} // namespace dawn
