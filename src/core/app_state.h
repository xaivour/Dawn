#pragma once

#include <string>
#include "common.h"
#include "event_handler.h"

namespace Dawn
{
    class AppState
    {
    public:
        static AppState* create();
        
        AppState() {}
        virtual ~AppState() {};

        virtual void initWindow(const std::string& title, uint32 width, uint32 height) = 0;
        virtual inline bool isRunning() const = 0;
        virtual uint32 getFps() const = 0;
        virtual void execute() = 0;
    protected:
        virtual void processEvents(EventHandler& eventHandler) = 0;
    private:
        DAWN_NULL_COPY_AND_ASSIGN(AppState)
    };
};
