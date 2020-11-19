#!/bin/bash
rm -rf *.o
rm -rf *.a
rm -rf *.so*

arch=$(uname -m)
oss=$(uname)
libname=$(pwd | awk -F"/" '{print $NF}')
includes="../include"

for k in $(ls *.cpp);do
    g++ -c -Wall -Werror -fpic -I${includes} -DPLUGIN_LIB $k
done

files=""
for k in $(ls *.cpp);do
    filename=$(basename -- "$k")
    filename="${filename%.*}"
    files="${files} ${filename}.o"
done
echo "gcc -shared -o lib${libname}-${oss}-${arch}.so ${files}"
gcc -shared -o lib${libname}-${oss}-${arch}.so ${files}



