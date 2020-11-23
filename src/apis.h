/*
Copyright (c) 2014-2016 Marius C. All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/circinusX1. The name of the
https://github.com/circinusX1/amutrion may not be used to endorse or promote
products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef APPX_H_
#define APPX_H_

#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <libgen.h>
#include <time.h>
#include <string>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdexcept>
#include <set>
#include <sys/statvfs.h>
#include <sys/file.h>
#include <map>
#include <atomic>
#include <errno.h>
#include <mutex>
#include "sqwrap.h"
#include "sqratConst.h"
#include "restsrv.h"
#include "logs.h"
#include "database.h"
#include "sunset.h"
#include "database.h"
#include "psingle.h"
#include "dlconfig.h"
#include "swdog.h"

class RestSrv;
class Gpio;


extern int          Loggran;          // debug flags
extern uint64_t     HddRoom;       // disk free Mb
extern size_t       OTime;      // app start time in seconds
extern pthread_t    MainThrId;        // main thread ID
extern std::string  Arhic;         // system platform name
extern std::string  Program;     // script file name
extern std::string  EmbVersion;        // vurrent version
extern std::mutex   MuxLog;         // log file mutex
extern Database*    Pdb;         // pointer to database
extern RestSrv*     PSockSrv;       // pointer to io
extern int          Wdto;         // watch dog minutes
extern CurrTime     Clk;      // current time
extern CurrTime     CurrTick;
extern int          Secret;
extern int          ThrLock;
#ifdef DEBUG
extern int          DbgMuxLock;
#endif

void globals_expose(SqEnvi &);
std::string execute(const char*);
void make_dir(const std::string& path);
int  wd_pull(unsigned long int ctl, int flags);

int exitapp(const char* message);
int reload_scr();

#define ARR_MAX             4096
#define MAIN_THR            (MainThrId==pthread_self())
#define REST_THREAD           (MainThrId!=pthread_self())
#define THREAD_NAME()       ((MainThrId==pthread_self() ) ? (const char*)"main" : (const char*)"io")
//#define MAIN_AUTOLOCK_TRACE()     lock_guard_mco<std::mutex> guard(Mutx);

#define EMPTY_STR           "\0"
#define IS_STRPTR(p)        (p && *p)
#define MIN_DRIVE_MEGS      800



#ifdef DEBUG
template<typename _Mutex>
class lock_guard_mco
{
public:
    typedef _Mutex mutex_type;
    pthread_t   _owner;
    explicit lock_guard_mco(mutex_type& __m) : _M_device(__m)
    {
        _M_device.lock();
        ++ThrLock;
        //LOGE("LOCK1 " << THREAD_NAME() << ThrLock);
    }

    lock_guard_mco(mutex_type& __m, adopt_lock_t) noexcept : _M_device(__m)
    { } // calling thread owns mutex

    ~lock_guard_mco()
    {
        _M_device.unlock();
        --ThrLock;
        //LOGE("UN-LOCK " << THREAD_NAME() << ThrLock);
    }

    lock_guard_mco(const lock_guard_mco&) = delete;
    lock_guard_mco& operator=(const lock_guard_mco&) = delete;

private:
    mutex_type&  _M_device;
};

#endif //DEBUG

#ifdef DEBUG
#define AUTO_LOCK_CTX(m)     lock_guard_mco<std::mutex> guard(m);
#define ASSERT_LOCKED()     assert(ThrLock)
#else
#define AUTO_LOCK_CTX(m)     std::lock_guard<std::mutex> guard(m);
#define ASSERT_LOCKED()
#endif


#define LOCK_CTX(m,x)   do{ AUTO_LOCK_CTX(m); x;} while(0);


#endif // APP_H_


