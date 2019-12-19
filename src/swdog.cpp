/*
Copyright (c) 2014-2016 comarius.DPT All rights reserved.

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

#include <limits.h>
#include "main.h"
#include "swdog.h"

#define EVENT_SIZE          ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN       ( 8 * ( EVENT_SIZE + PATH_MAX + 1) )
#define WATCH_FLAGS         ( IN_CREATE | IN_DELETE|IN_MODIFY )

SwDog::SwDog(int m):_secs(m)
{
    _localcnt=0;
    _fd = inotify_init();
    if(_fd>0)
    {
        char cwd[256];
        ::getcwd(cwd, 255);
        _wd = ::inotify_add_watch(_fd, cwd, WATCH_FLAGS);
    }
}

SwDog::~SwDog()
{
    ::inotify_rm_watch( _fd, _wd );
    ::close(_fd);
    this->stop_it();
    ::msleep(1000);
}

void SwDog::thread_main()
{
    fd_set  watch_set;
    timeval tv = {0, 0x1FF};
    char    buffer[ EVENT_BUF_LEN ];

    while(ApStat == RUNNING && !this->is_stopped())
    {
        ::msleep(1000);
        do{
            std::lock_guard<std::mutex> guard(_m);
            ++_localcnt;
            if(_localcnt>(_secs-10))
            {
                LOGE("no kicks");
            }

            if(_localcnt>_secs)
            {
                ApStat = EXIT_APP;
                ::sleep(1);
                LOGE("WD reboot");
                break;
            }

            FD_ZERO( &watch_set );
            FD_SET( _fd, &watch_set );
            tv.tv_sec = 0; tv.tv_usec=1000;
            if(::select( _fd+1, &watch_set, NULL, NULL, &tv)>0)
            {
                ::read( _fd, buffer, EVENT_BUF_LEN );
            }
        }while(0);
    }
}

time_t    CurrTime::update()
{
    time_t  rawtime;
    ::time (&rawtime);
    if(_prevtime != 0)
    {
        double diff = std::difftime(_rawtime, _prevtime );
        if(diff < 0)
        {
            struct tm * timep    = ::localtime(&_prevtime);
            struct tm * timeinfo =::localtime(&rawtime);
            char   out[128];

            ::strftime(out, sizeof(out)-1, "%Y-%m-%d %H:%M:%S", timep);
            ::strftime(_buffer, sizeof(_buffer)-1, "%Y-%m-%d %H:%M:%S",timeinfo);

            LOGW("Timeline reversed: " <<
                  " Before Time:" << out <<
                  " Now Time:" << _buffer<<", Ticks" << _loccount);
        }
    }
    _prevtime = _rawtime;
    _rawtime = rawtime;
    ++_loccount;
    return _rawtime;
}
