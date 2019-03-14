#pragma once

namespace Dawn
{
	struct Event
	{
		Event() :eventType(NONE) {}
		~Event() {}

		enum EventType
		{
			NONE,
			KEY_DOWN,
			KEY_UP,
			MOUSE_BUTTON_DOWN,
			MOUSE_BUTTON_UP,
			MOUSE_MOVE
		};

		inline const EventType& getEventType() const { return eventType; }

		EventType eventType{};
	};
}
