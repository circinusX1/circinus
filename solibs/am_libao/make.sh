#!/bin/bash
rm -rf *.o
rm -rf *.a
rm -rf *.so*

arch=$(uname -m)
oss=$(uname)
libname=$(pwd | awk -F"/" '{print $NF}')
includes="../include"

for k in $(ls *.cpp);do
    g++ -c -Wall -Werror -fpic -I${includes} $k
done

for k in $(ls *.cpp);do
    filename=$(basename -- "$k")
    filename="${filename%.*}"
    echo "linking ${filename}"
    gcc -shared -o lib${libname}-${oss}-${arch}.so ${filename}.o
done

