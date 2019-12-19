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

#ifndef SWDOG_H
#define SWDOG_H

#include <signal.h>
#include <limits.h>
#include <sys/inotify.h>
#include <string>
#include <mutex>
#include "osthread.h"

/**
 * @brief The SwDog class
    If main sleeps longer than a minute and program is up and running
    keep this to poke the kernel dog
 */
class SwDog : public Mpthrd
{
public:
    SwDog(int m);
    virtual ~SwDog();

    void thread_main();
    void reset(){
        std::lock_guard<std::mutex> guard(_m);
        _localcnt=0;
    }
private:
    int _localcnt = 0;
    std::mutex _m;
    int        _fd = 0;
    int        _wd = 0;
    int        _secs=3;
};

class CurrTime
{
public:
    CurrTime():_loccount(0){
        _startt = time (&_rawtime);
    }
    const char* datetime(){
        struct tm * timeinfo=::localtime(&_rawtime);
        ::strftime(_buffer, sizeof(_buffer)-1, "%Y-%m-%d %H:%M:%S",timeinfo);
        return _buffer;
    }

    time_t    update();

    time_t tick()const{
        return time(0) - _startt;
    }

private:
    time_t      _startt;
    char        _buffer[64];
    time_t      _rawtime;
    time_t      _prevtime = 0;
    size_t      _loccount;
};

#endif // SWDOG_H
