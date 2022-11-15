#include "config.h"

#include <stdio.h>

extern "C" void version() {
    fprintf(stderr, "Masiro-Reader Library " PACKAGE_VERSION "\n"
                    "\n"
                    "Copy 2022 Cerallin <cerallin@cerallin.top>");
}
