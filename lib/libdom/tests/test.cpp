#include <CppUTest/CommandLineTestRunner.h>


#include "tests/styles/test_margin.h"
#include "tests/styles/test_padding.h"
#include "tests/styles/test_position.h"
#include "tests/styles/test_textalign.h"
#include "tests/styles/test_textindent.h"

#include "tests/test_treenode.h"
#include "tests/test_domnode.h"

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
