#pragma once 

#include <vector>
#include "core/common.h"
#include "core/input.h"
#include "events.h"
 
namespace Dawn
{
	class EventHandler
	{
	private:
		EventHandler() {}
		~EventHandler() {}

		DAWN_NULL_COPY_AND_ASSIGN(EventHandler)
	public:
		void onKeyDown(uint32 keyCode, bool isRepeat);
	    void onKeyUp(uint32 keyCode, bool isRepeat);
	    void onMouseButtonDown(uint32 mouseButton, uint8 numClicks, int32 posX, int32 posY);
	    void onMouseButtonUp(uint32 mouseButton, uint8 numClicks, int32 posX, int32 posY);
	    void onMouseMove(int32 mousePosX, int32 mousePosY, 
	    	                     int32 deltaX, int32 deltaY);

		static EventHandler* getEventHandler();
	};
}
