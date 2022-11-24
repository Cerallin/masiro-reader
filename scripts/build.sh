#!/bin/bash

enable_debug=1

mkdir -p build

cd build

[ -f Makefile ] && make distclean

INSTALL_DIR=$(pwd)/.libmasiro

CXXFLAGS="  -g -O2 -Wall -Wno-unused-function"
CXXFLAGS+=" -I$(pwd)/libbcm2835/include"
CXXFLAGS+=" -I$(pwd)/libcpputest/include"

LDFLAGS="-L$(pwd)/libbcm2835/lib -L$(pwd)/libcpputest/lib"

CXXFLAGS=$CXXFLAGS LDFLAGS=$LDFLAGS \
    ../configure \
    --enable-debug \
    --prefix=$INSTALL_DIR

make -j32 && make install

cd -
