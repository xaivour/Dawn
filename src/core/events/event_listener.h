#pragma once

#include <functional>

namespace Dawn
{
	class EventDispatcher;
	class Event;

	class EventListener
	{
		friend class EventDispatcher;

		void shipEvent(Event& e)
		{
			onEvent(e);
			switch(e.getEventType())
			{
			    case Event::KeyDownEvent:
					onKeyDown((KeyDownEvent&)e);
					break;
				case Event::KeyUpEvent:
					onKeyUp((KeyUpEvent&)e);
				    break;
				case Event::MouseButtonDownEvent:
					onMouseButtonDown((MouseButtonDownEvent&)e);
					break;
				case Event::MouseButtonUpEvent:
					onMouseButtonUp((MouseButtonUpEvent&)e);
					break;
				case Event::MouseMoveEvent:
					onMouseMove((MouseMoveEvent&)e);
					break;
			}
		}
	public:
		virtual void onEvent(Event& e) {}
		virtual void onKeyDown(KeyDownEvent& keyDownEvent) {}
		virtual void onKeyUp(KeyUpEvent& keyUpEvent) {}
		virtual void onMouseButtonDown(MouseButtonDownEvent& mouseDownEvent) {}
		virtual void onMouseButtonUp(MouseButtonUpEvent& mouseButtonUp) {}
		virtual void onMouseMove(MouseMoveEvent& mouseMoveEvent) {}
	};
}
