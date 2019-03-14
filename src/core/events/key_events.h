#pragma once

#include "event_interface.h"
#include "core/common.h"

namespace Dawn
{
    struct KeyDownEvent : public Event
    {
    	KeyDownEvent() { eventType = Event::KEY_DOWN; }
    	~KeyDownEvent() {}

    	inline const uint32& getKeyCode() const { return keyCode; }
    	inline bool isKeyRepeat() const { return isRepeat; }

        uint32 keyCode{};
        bool isRepeat{};
    };
    
    struct KeyUpEvent : public Event
    {
    	KeyUpEvent() { eventType = Event::KEY_UP; }
    	~KeyUpEvent() {}

    	inline const uint32& getKeyCode() const { return keyCode; }
        inline bool isKeyRepeat() const { return isRepeat; }

        uint32 keyCode{};
        bool isRepeat{};
    };
    
}
