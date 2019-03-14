#pragma once 

#include <string>
#include <algorithm>
#include <functional>
#include "core/common.h"
#include "core/input.h"
#include "event_listener.h"

namespace Dawn
{
	class EventDispatcher
	{
		EventDispatcher() {}
		~EventDispatcher() {}

		std::vector<EventListener*> eventListeners;
	public:
		static EventDispatcher& getEventDispatcher();

		template <typename T>
		void dispatchEvent(T& event)
		{
			for(auto e : eventListeners)
			{
				e->shipEvent(event);
			}
		}

		void addEventListener(EventListener* eventListener);
		void removeEventListener(EventListener* eventListener);
	};
}
