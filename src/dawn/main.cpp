#include "core/log.h"
#include "core/app_state.h"

int main(int argc, char** argv)
{
    Dawn::Log::initLog();
    
    DAWN_WARN("Hello");
   
    auto app = Dawn::AppState::create();
    app->init("Dawn", 840, 640);
    app->execute();
    
    delete app;
    
    return EXIT_SUCCESS;
}
