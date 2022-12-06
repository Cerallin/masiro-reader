#include <CppUTest/CommandLineTestRunner.h>

#include "config.h"

#include "layer.h"

#include "imagelayer.h"
#include "textlayer.h"

#include "tests/test_layer.h"

#include "tests/test_imagelayer.h"
#include "tests/test_textlayer.h"

int main(int ac, char **av) {
    return CommandLineTestRunner::RunAllTests(ac, av);
}
