#pragma once

#include <string>
#include "common.h"
#include "events/events.h"

namespace Dawn
{
    class AppState : public EventListener
    {
    public:
        static AppState* create();
        
        AppState() { EventDispatcher::getEventDispatcher().addEventListener(this); }
        virtual ~AppState() { EventDispatcher::getEventDispatcher().removeEventListener(this); };

        virtual void initWindow(const std::string& title, uint32 width, uint32 height) = 0;
        inline bool isRunning() const { return isAppRunning; }
        virtual uint32 getFps() const = 0;
        virtual void execute() = 0;

        void onKeyDown(KeyDownEvent& e)
        {
            if(e.keyCode == Input::KEY_A)
            {
                DAWN_INFO("Key pressed 0x{:x}", e.keyCode);
            }
        }
    protected:
        virtual void processEvents() = 0;

        bool isAppRunning{};
    private:
        DAWN_NULL_COPY_AND_ASSIGN(AppState)
    };
};
