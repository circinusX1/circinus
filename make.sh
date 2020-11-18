#!/bin/bash

[[ -z $1 ]] && echo "$0 parameter as: beaglebone (on ARM), imx6 (on ARM) or raspberrypi (on ARM) thishost (on x86)" && exit 1
BOARD=$1
arch=$(uname -m)
oss=$(uname)

pushd src/sq/squirrel
# rm -rf *.o
rm -rf *.a
g++ -I../include -c -fPIC  *.cpp 
ar rcs libsquirrel-${arch}-${oss}.a *.o
popd 

pushd src/sq/sqstdlib
rm -rf *.o
rm -rf *.a
g++ -I../include -c -fPIC  *.cpp 
ar rcs libsqstdlib-${arch}-${oss}.a *.o
popd 

rm ./lib/*
mv src/sq/sqstdlib/libsqstdlib-${arch}-${oss}.a ./lib
mv src/sq/squirrel/libsquirrel-${arch}-${oss}.a ./lib


pushd src
includes="-I./sq/sqrat/include/sqrat -I./sq/include -I./sq/squirrel -I./sq/sqrat -I./sq/sqrat/include -I./builtin -I./drivers/platform/linux/${BOARD} -I./drivers -I./drivers/common -I./comm  -I./rapidjson/include -I./"
libs="-L../lib -lsquirrel-${arch}-${oss} -lsqstdlib-${arch}-${oss} -lpthread -ldl"
defines="-DWITH_MAIN_MUTEX -D${BOARD}"
files="*.cpp ./builtin/*.cpp ./drivers/*.cpp ./drivers/platform/linux/${BOARD}/*.cpp"
g++ ${files} ${defines} ${includes} -std=c++14 ${libs} -o circinus-${oss}-${arch}
popd

cp ./src/circinus-${oss}-${arch} ./bin

echo "./src/circinus-${oss}-${arch} generated in bin folder"




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

echo "MAKING SOLIBS"
pushd solibs
    for mod in $(ls -d */);do
        pushd $mod
            echo "BUILDING $mod"
            [[ -f ./make.sh ]] && ./make.sh
            cp -rf ./*.so ../../bin/solibs/
        popd
    done
popd


