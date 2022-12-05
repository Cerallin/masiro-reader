#!/bin/bash

enable_debug=1

mkdir -p build

dir=$(pwd)

cd build

[ -f Makefile ] && make distclean

INSTALL_DIR=$dir/build/.libmasiro

CXXFLAGS="  -g -flto -O2 -Wall -Wno-unused-function"
CXXFLAGS+=" -I$dir/bcm2835-install/include"
CXXFLAGS+=" -I$dir/cpputest-install/include"

LDFLAGS="-L$dir/bcm2835-install/lib -L$dir/cpputest-install/lib"

CXXFLAGS=$CXXFLAGS LDFLAGS=$LDFLAGS \
    ../configure \
    --enable-debug \
    --prefix=$INSTALL_DIR

make -j32 && make install

cd -
