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

// /usr/bin/valgrind --tool=memcheck  -v --leak-check=full  --track-origins=yes --show-reachable=yes /path/to/Loggran

#define EMBIX_VERSION "1.0.0"
#define PULL_ALL

#include "logs.h"
#include "apis.h"
#include "usbdev.h"
#include "evqueue.h"
#include "_test.h"
#include "sq_import.h"
#include <linux/watchdog.h>

bool        Reincepe     = false;
bool        ExBoot      = false;
bool        _mempool    = false;
sq_api*     SQ_PTRS;
EALIV       ApStat    = RUNNING;
std::string SqErrStr;
static int _inner(int argc, char *argv[]);

void __sigInt (int i)
{
    ApStat = DO_EXIT;
}
void __sipPipe (int i)
{
    LOGEX("SIGPIPE:" << i << " IGNORED");
}

int main(int argc, char *argv[])
{
    OneProc p (1584);       // singleton, limit this instance to 1

    if(!p() && argc < 3)    // second instance
    {
        std::cout << "an instance of this is already running\n";
        LOGE("a program is running.");
        return 0;
    }
    RestDisabled = (argc==3 && argv[2][0]=='@');

    LOGEX("");
#ifndef DEBUG
    signal(SIGINT,  __sigInt);
    // signal(SIGABRT, __sigInt);
    signal(SIGTRAP, __sipPipe);
    signal(SIGTERM, __sigInt);
    signal(SIGPIPE, __sipPipe);
    signal(SIGBUS, __sipPipe);
    signal(SIGKILL, __sigInt);
#else //
    signal(SIGKILL, __sigInt);
#endif 
    do {
#ifdef DEBUG_AUTO_RELOAD_SCRIPT
        if(Reincepe)
        {
            std::string c;
            std::cout << "\r\n*** Script error \r\n then. 'c' <enter> to continue\r\n any key <enter> to quit\r\n";
            std::cin >> c;
            ApStat = c=="c" ? RUNNING : DO_EXIT;
        }
#endif
        ApStat = RUNNING;
        sqrat_newapi(&SQ_PTRS);

#ifdef WITH_USB
        int r = libusb_init(&_ctx);
        if(r < 0)
        {
            std::cout<<"libusb_init error "<<r<<std::endl;
            return 0;
        }
#endif
        _inner(argc, argv);
        Reincepe=true;
        LOGI("done ");
    }while(0);
    // ApStat==RUNNING || ApStat==RELOAD_S);
#ifdef WITH_USB
    if(_ctx)
        libusb_exit(_ctx);
#endif
    if(ExBoot)
    {
        LOGW("reboot");
        ::system("reboot");
    }
    return 0;
}

int _inner(int argc, char *argv[])
{
    SqEnvi              sq;
    Inst                ctx(sq, argv, argc);
    std::string         scrname;
    std::string         exscrstr;

    EmbVersion = EMBIX_VERSION;
    EmbVersion += "-";
    EmbVersion += __DATE__;
    MainThrId = pthread_self();
    if(Reincepe==false)
        std::cout << EMBIX_VERSION<<" " <<__DATE__<<" \n";

    if(argc < 2)
    {
        std::cout << "requires a script as a parameter\n";
        ApStat = EXIT_APP;
        return -1;
    }
    else
    {
        scrname = argv[1];
    }
    Arhic = execute("dpkg --print-architecture");
    Arhic = Arhic.substr(0, Arhic.find_first_of('\n'));
    do{
        OTime = time(0);
        try{
            __bsqenv->ready_ = true;
            __bsqenv->snap_ = false;
            globals_expose(sq);
            EngScript os = sq.compile_script(scrname.c_str());
            os.WriteCompiledFile(std::string("data/precompiled.txt"));
            os.run_script();
        }
        catch(Sqrat::Exception& ex)
        {
            ApStat = RESTART;
            LOGEX(ex.Message());
            LOGEX(SqErrStr);
        }
    }while(0);

    if(PSockSrv){ PSockSrv->close(); }
    if(Wdto)
    {
        if(ApStat==RELOAD_S || ApStat==DO_EXIT)
        {
            LOGD1("dog dies ");
            wd_pull(WDIOC_GETPRETIMEOUT, 0);
        }
    }
    App = nullptr;
    return 0;
}
