#!/bin/bash

[[ -z $1 ]] && echo "$0 parameter as: beaglebone (on ARM), imx6 (on ARM) or raspberrypi (on ARM) thishost (on x86)" && exit 1
BOARD=$1
arch=$(uname -m)
oss=$(uname)


echo "MAKING MODULES AND SO LIBS"
[[ ! -d ./bin/modules ]] && mkdir ./bin/modules
[[ ! -d ./bin/solibs ]] && mkdir ./bin/solibs

echo "MAKING MODULES"
pushd modules
    for mod in $(ls -d */);do
        pushd $mod
            echo "BUILDING $mod"
            [[ -f ./make.sh ]] && ./make.sh
            cp -rf  ./*.so ../../bin/modules/
        popd
    done
popd

# no need we can load so direct
#echo "MAKING SOLIBS"
#pushd solibs
#    for mod in $(ls -d */);do
#        pushd $mod
#            echo "BUILDING $mod"
#            [[ -f ./make.sh ]] && ./make.sh
#            cp -rf ./*.so ../../bin/solibs/
#        popd
#    done
#popd


