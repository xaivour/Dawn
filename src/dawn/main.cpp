#include "core/log.h"

int main(int argc, char** argv)
{
    Dawn::Log::initLog();

    DAWN_WARN("Hello");

    return EXIT_SUCCESS;
}
