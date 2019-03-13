#pragma once 

#include "event_interface.h"

namespace Dawn
{
	struct MouseButtonDownEvent : public Event
	{
		MouseButtonDownEvent() { eventType = Event::MouseButtonDownEvent; }

		inline const uint32& getMouseButtonCode() const { return mouseButtonCode; }
		inline const uint32& getNumClicks() const { return numClicks; }

		uint32 mouseButtonCode{};
		uint32 numClicks{};
	};

	struct MouseButtonUpEvent : public Event
	{
		MouseButtonUpEvent()  { eventType = Event::MouseButtonUpEvent; }

		inline const uint32& getMouseButtonCode() const { return mouseButtonCode; }
		inline const uint32& getNumClicks() const { return numClicks; }

		uint32 mouseButtonCode{};
		uint32 numClicks{};
	};

	struct MouseMoveEvent : public Event
	{
		MouseMoveEvent()  { eventType = Event::MouseMoveEvent; }

		inline const int32& getMousePosX() const { return posX; }
		inline const int32& getMousePosY() const { return posY; }

		inline const int32& getMouseDeltaX() const { return deltaX; }
		inline const int32& getMouseDeltaY() const { return deltaY; }

		int32 posX{}, posY{};
		int32 deltaX{}, deltaY{};
	};
}

