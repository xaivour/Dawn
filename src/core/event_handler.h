#pragma once 

#include "common.h"
#include "input.h"
 
namespace Dawn
{
	class EventHandler
	{
	public:
		EventHandler() {}
		virtual ~EventHandler() {};
		virtual void onKeyDown(uint32 keyCode, bool isRepeat) { if(keyCode == Input::KEY_A) {DAWN_INFO("Key pressed down 0x{0:x}", keyCode);} }
	    virtual void onKeyUp(uint32 keyCode, bool isRepeat) {}
	    virtual void onMouseDown(uint32 mouseButton, uint8 numClicks) {}
	    virtual void onMouseUp(uint32 mouseButton, uint8 numClicks) {}
	    virtual void onMouseMove(int32 mousePosX, int32 mousePosY, 
	    	                     int32 deltaX, int32 deltaY) { DAWN_INFO("Mouse Moved pos ({0}, {1})", mousePosX, mousePosY); }
	private:
		DAWN_NULL_COPY_AND_ASSIGN(EventHandler)
	};
}
