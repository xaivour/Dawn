#include "core/log.h"
#include "core/error/error.h"

int main(int argc, char** argv)
{
    dawn::log_internal::init_log();

    DE_FATAL("hello");

    return EXIT_SUCCESS;
}
