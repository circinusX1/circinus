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

#include <sys/types.h>
#include <sys/stat.h>
#include "logs.h"
#include "main.h"
#include "apis.h"
#include "inst.h"

static int    Logscount=16;
static int    Logcheck=0;
static size_t Last_sz;
static size_t Maxsz=4000000;
static std::string LgFl = "/var/log/rembix/rembix.log";

static int l_recycle()
{
    if(!LgFl.empty())
    {
        char drloc[256] = {0};
        char fname[256] = {0};
        char siletmp1[256];
        char siletmp2[256];


        ::strcpy(siletmp1,  LgFl.c_str());
        ::strcpy(fname, LgFl.substr(LgFl.find_last_of('/')).c_str());
        ::strcpy(drloc, ::dirname(siletmp1));

        if(::strchr(fname,'-')!=0)
            *(::strchr(fname,'-'))=0;
        if(Last_sz > Maxsz)
        {
            Last_sz = 0;
            for(int k=Logscount; k>0; k-- )
            {
                ::sprintf(siletmp2,"%s%s-%d",drloc,fname,k);
                ::sprintf(siletmp1,"%s%s-%d",drloc,fname,k-1);
                if(k == Logscount)
                {
                    if(::access(siletmp2,0)==0)
                        ::unlink(siletmp2);
                }
                else
                {
                    if(::access(siletmp1,0)==0)
                    {
                        ::rename(siletmp1, siletmp2);
                    }
                }
            }
            ::rename(LgFl.c_str(), siletmp1);
            ::unlink("/var/log/rembix/rembix-rest.log");

            FILE* pf = ::fopen(LgFl.c_str(),"wb");
            if(pf)
            {
                ::fclose(pf);
                chmod(LgFl.c_str(), S_IRWXO|S_IRWXG|S_IRWXU);
                system("chown www-data:$USER /var/log/rembix/rembix.log");
            }
        }
    }
    return 0;
}

static int is_big_endian(void) {
    static const int n = 1;
    return ((char *) &n)[0] == 0;
}

LogScop::LogScop(const char* ch):op(ch)
{
    LOGI("K: " << ch << "{");
}

LogScop::~LogScop()
{
    LOGI("K: " << op << "}");
}

void f2l(std::stringstream& ss, bool fatal)
{
    static bool once =false;
    if(once)
    {
        std::cout << ss.str();
        return;
    }
    std::lock_guard<std::mutex> guard(MuxLog);

    bool chown=false;
    FILE *log = ::fopen(LgFl.c_str(), "ab");
    if (!log){
        log = fopen(LgFl.c_str(), "wb");
        chown=true;
    }
    if (!log && false==once)
    {
        once=true;
        std::cout << "Can not open" << LgFl << "\r\n";
        return;
    }
    ::fwrite(ss.str().c_str(),sizeof(char),ss.str().length(),log);
    if(Logcheck%100==0)
    {
        Last_sz = ::ftell(log);
    }
    ::fclose(log);

    if(chown){
        chmod(LgFl.c_str(), S_IRWXO|S_IRWXG|S_IRWXU);
        system("chown www-data:$USER /var/log/rembix/rembix.log");
    }
    if(fatal)
    {
        static bool bf=false;
        if(bf==false)
        {
            bf=true;
            unlink("/var/log/rembix/errors.log");
        }
        FILE *log = ::fopen(LgFl.c_str(), "ab");
        if (!log){
            log = fopen(LgFl.c_str(), "wb");
            chown=true;
        }
        if(log)
        {
            ::fwrite(ss.str().c_str(),sizeof(char),ss.str().length(),log);
            ::fclose(log);
        }
        if(chown){
            chmod(LgFl.c_str(), S_IRWXO|S_IRWXG|S_IRWXU);
            system("chown www-data:$USER /var/log/rembix/errors.log");
        }
    }

    if(Logcheck%100==0)
    {
        if(Last_sz > Maxsz)
        {
            l_recycle();
        }
    }
    ++Logcheck;
}
