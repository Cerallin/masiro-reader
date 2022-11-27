#!/bin/bash

[ -f bcm2835-1.71.tar.gz ] ||
    wget -q http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
tar xf bcm2835-1.71.tar.gz

cd bcm2835-1.71

CFLAGS="-fPIC -DPIC" ./configure --prefix=$(pwd)/../bcm2835-install $@
make -j32
make install

cd -
