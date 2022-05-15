#include <CppUTest/CommandLineTestRunner.h>


#include "tests/dom/test_margin.h"
#include "tests/dom/test_padding.h"
#include "tests/dom/test_position.h"
#include "tests/dom/test_textalign.h"
#include "tests/dom/test_textindent.h"

#include "tests/test_treenode.h"
#include "tests/test_domnode.h"

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
