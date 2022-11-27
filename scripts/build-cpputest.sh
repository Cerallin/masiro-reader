#!/bin/bash

cd cpputest
git checkout v4.0
cd -

mkdir -p cpputest-build
cd cpputest-build
cmake -DCMAKE_INSTALL_PREFIX=$(pwd)/../cpputest-install ../cpputest
make -j32
make install
cd -
