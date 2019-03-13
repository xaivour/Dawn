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
		static EventDispatcher& getEventDispatcher()
		{
			static EventDispatcher eventDispathcer;
			return eventDispathcer;
		}

		template <typename T>
		void dispatchEvent(T& event)
		{
			for(auto e : eventListeners)
			{
				e->shipEvent(event);
			}
		}

		void addEventListener(EventListener* eventListener)
		{
			eventListeners.push_back(eventListener);
		}

		void removeEventListener(EventListener* eventListener)
		{
			std::remove_if(eventListeners.begin(), eventListeners.end(), [eventListener](EventListener* e){
				return (e == eventListener);
			});
		}
	};
}
