#!/bin/bash -e

download_and_install_bcm2835() {
    [ -f bcm2835-1.71.tar.gz ] ||
        wget -q http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
    tar xf bcm2835-1.71.tar.gz

    cd bcm2835-1.71

    CFLAGS="-fPIC -DPIC" ./configure --prefix=$(pwd)/../libbcm2835 $@
    make -j8
    make install

    cd -
}

checkout_and_install_cpputest() {
    cd ../cpputest
    git checkout v4.0
    cd -

    mkdir -p cpputest-build
    cd cpputest-build
    cmake -DCMAKE_INSTALL_PREFIX=$(pwd)/../libcpputest ../../cpputest
    make -j8
    make install
    cd -
}

download_and_install_submodules() {
    checkout_and_install_cpputest
    download_and_install_bcm2835
}

show_usage() {
    local script="${0}"
    cat <<EOF >&2
Usage:
    $script [configure options]
----------------
Example:
    $script --host=arm-linux-gnueabihf

EOF
}

case $1 in
-h | --help)
    show_usage
    exit 0
    ;;
esac

git submodule init
git submodule update

mkdir -p build
cd build

download_and_install_submodules $@

cd -

