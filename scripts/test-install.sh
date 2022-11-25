#!/bin/bash

MASIRO_INSTALL_DIR=$(pwd)/build/.libmasiro

if ! [ -d "$MASIRO_INSTALL_DIR" ]; then
    # always lower case
    declare -l flag
    read -p "Library not install, compile?[yes/No] " flag
    if [ "${flag:-n}" != "yes" ]; then
        echo "Abort." >&2
        exit 1
    else
        ${0%\/*}/build.sh
    fi
fi

cat <<EOF >build/test.cpp
#include "text.h"
#include "display.h"
#include "graphics.h"

int main() {
    print_version();
    return 0;
}
EOF

g++ \
    -g -O2 -flto \
    -I$(pwd)/build/libbcm2835/include \
    -I$(pwd)/build \
    -I$MASIRO_INSTALL_DIR/include \
    -o $(pwd)/build/test.out \
    build/test.cpp \
    -Wl,$MASIRO_INSTALL_DIR/lib/libmasiro.a

echo "Test result:"
$(pwd)/build/test.out
