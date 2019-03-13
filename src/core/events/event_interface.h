#pragma once

namespace Dawn
{
	struct Event
	{
		Event() :eventType(None) {}
		~Event() {}

		enum EventType
		{
			None,
			KeyDownEvent,
			KeyUpEvent,
			MouseButtonDownEvent,
			MouseButtonUpEvent,
			MouseMoveEvent
		};

		inline const EventType& getEventType() const { return eventType; }

		EventType eventType{};
	};
}
