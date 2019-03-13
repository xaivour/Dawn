#include "event_handler.h"
#include "key_events.h"
#include "mouse_events.h"

namespace Dawn
{
    void EventHandler::onKeyDown(uint32 keyCode, bool isRepeat)
    {
    	static KeyDownEvent keyDownEvent;
    	keyDownEvent.keyCode = keyCode;
    	keyDownEvent.isRepeat = isRepeat;

    	EventDispatcher::getEventDispatcher().dispatchEvent(keyDownEvent);
    }

    void EventHandler::onKeyUp(uint32 keyCode, bool isRepeat)
    {
    	static KeyUpEvent keyUpEvent;
    	keyUpEvent.keyCode = keyCode;
    	keyUpEvent.isRepeat = isRepeat;

    	EventDispatcher::getEventDispatcher().dispatchEvent(keyUpEvent);
    }

    void EventHandler::onMouseButtonDown(uint32 mouseButton, uint8 numClicks)
    {
    	static MouseButtonDownEvent mouseButtonDownEvent;
    	mouseButtonDownEvent.mouseButtonCode = mouseButton;
    	mouseButtonDownEvent.numClicks = numClicks;

    	EventDispatcher::getEventDispatcher().dispatchEvent(mouseButtonDownEvent);
    }

    void EventHandler::onMouseButtonUp(uint32 mouseButton, uint8 numClicks)
    {
    	static MouseButtonUpEvent mouseButtonUpEvent;
    	mouseButtonUpEvent.mouseButtonCode = mouseButton;
    	mouseButtonUpEvent.numClicks = numClicks;

    	EventDispatcher::getEventDispatcher().dispatchEvent(mouseButtonUpEvent);
    }

    void EventHandler::onMouseMove(int32 mousePosX, int32 mousePosY, 
	    	                     int32 deltaX, int32 deltaY)
    {
    	static MouseMoveEvent mouseMoveEvent;
    	mouseMoveEvent.posX = mousePosX;
    	mouseMoveEvent.posY = mousePosY;

    	mouseMoveEvent.deltaX = deltaX;
    	mouseMoveEvent.deltaY = deltaY;

    	EventDispatcher::getEventDispatcher().dispatchEvent(mouseMoveEvent);
    }
}