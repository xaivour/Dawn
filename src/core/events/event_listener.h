#pragma once

#include <functional>

namespace Dawn
{
	class EventDispatcher;
	class Event;

	class EventListener
	{
	public:
		int track{2};
	public:
		virtual void shipEvent(Event& e);
		
		virtual void onEvent(Event& e) {}
		virtual void onKeyDown(KeyDownEvent& keyDownEvent) {}
		virtual void onKeyUp(KeyUpEvent& keyUpEvent) {}
		virtual void onMouseButtonDown(MouseButtonDownEvent& mouseDownEvent) {}
		virtual void onMouseButtonUp(MouseButtonUpEvent& mouseButtonUp) {}
		virtual void onMouseMove(MouseMoveEvent& mouseMoveEvent) {}
	};
}
