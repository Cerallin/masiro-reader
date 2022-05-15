#include <CppUTest/CommandLineTestRunner.h>

#include "tests/test_paint.h"

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
