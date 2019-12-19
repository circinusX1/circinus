/*
Copyright (c) 2014-2016 Marius.C All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/comarius. The name of the
https://github.com/comarius may not be used to endorse or promote
products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef D_CORE_H
#define D_CORE_H

#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <cstring>
#include <string>
#include <map>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdint>

#include "../modules/iper.h"
#include "divais.h"

#define S_NAN   "-1"
#define I_NAN    -1
#define F_NAN    -1.0f

#if __cplusplus >= 201103L && defined(_GLIBCXX_USE_C99)

# else
#   ifndef STDTOSTRING_H
#   define STDTOSTRING_H

#include <string>
#include <sstream>
#include <stdlib.h>



using namespace std;
namespace std
{
template < typename T > std::string to_string( const T& n )
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

#if ! defined (__FreeBSD__)
inline long stoll(const std::string& str)
{
    return ::atol(str.c_str());
}
#endif


}
#   endif
#endif

namespace GenericHw
{

template <typename T> inline std::string tostr(const T& t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}

class DvCore
{
public:
    DvCore();
    virtual ~DvCore();
    const std::string& sf()const{return _dev_node;}
    static std::string  sh_exec(std::string command);
protected:

    bool _config(const char* key, std::string& val);
    int _readint(const std::string& file);
    size_t _red(const std::string& file, uint8_t* pb, size_t l)const;
    const std::string& _red(const std::string& file);
    bool _wrt(const std::string& file, const char* value, bool check=false);

protected:
    std::string         _dev_node;
    std::string         _err;
    std::string         _content;
};

};
#endif //D_CORE_H


