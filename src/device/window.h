/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

#include "core/memory/types.h"
#include "core/types.h"

namespace dawn
{
/// Window interface.
///
/// @ingroup Device
struct Window
{
	/// Opens the window.
	virtual void open(u16 x, u16 y, u16 width, u16 height, u32 parent) = 0;

	/// Closes the window.
	virtual void close() = 0;

	/// Shows the window.
	virtual void show() = 0;

	/// Hides the window.
	virtual void hide() = 0;

	/// Resizes the window to @a width and @a height.
	virtual void resize(u16 width, u16 height) = 0;

	/// Moves the window to @a x and @a y.
	virtual void move(u16 x, u16 y) = 0;

	/// Minimizes the window.
	virtual void minimize() = 0;

	/// Maximizes the window.
	virtual void maximize() = 0;

	/// Restores the window.
	virtual void restore() = 0;

	/// Returns the title of the window.
	virtual const char* title() = 0;

	/// Sets the title of the window.
	virtual void set_title (const char* title) = 0;

	/// Returns the native window handle.
	virtual void* handle() = 0;

	/// Sets whether to @a show the cursor.
	virtual void show_cursor(bool show) = 0;

	/// Sets whether the window is @a fullscreen.
	virtual void set_fullscreen(bool fullscreen) = 0;

	virtual void bgfx_setup() = 0;
};

/// Functions to manipulate Window
///
/// @ingroup Device
namespace window
{
	/// Creates a new window.
	Window* create(Allocator& a);

	/// Destroys the window @a w.
	void destroy(Allocator& a, Window& w);

} // namespace window

} // namespace dawn
