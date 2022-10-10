#!/bin/bash -e

download_and_compile_bcm2835() {
    [ -f bcm2835-1.71.tar.gz ] ||
        wget -q http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
    tar xf bcm2835-1.71.tar.gz

    cd bcm2835-1.71

    ./configure $@
    make -j8

    cd -
}

checkout_and_compile_cpputest() {
    cd ../cpputest
    git checkout v4.0
    cd -

    mkdir -p cpputest-build
    cd cpputest-build
    cmake ../../cpputest
    make -j8
    cd -
}

download_and_compile_submodules() {
    git submodule update

    checkout_and_compile_cpputest
    download_and_compile_bcm2835
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

mkdir -p build
cd build

download_and_compile_submodules $@

cd -

