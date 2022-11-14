#include <CppUTest/CommandLineTestRunner.h>

#include "tests/test_codepoint.h"
#include "tests/test_fontface.h"
#include "tests/test_fontfamily.h"
#include "tests/test_font.h"

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
