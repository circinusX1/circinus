/*
Copyright (c) 2014-2016 PANGEEA.DPT All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/pangeea. The name of the
https://github.com/pangeea may not be used to endorse or promote
products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef LOGS_H
#define LOGS_H

#include <string>
#include <fstream>
#include "osthread.h"

void f2l(std::stringstream& ss, bool fatal=false);
extern int  Loggran;          // debug flags
namespace Tc
{
    enum Code {
        FG_RED     =36,
        FG_GREEN   =32,
        FG_YELLOW  =33,
        FG_BLUE    =34,
        FG_CYAN    =36,
        FG_DEFAULT =39,

        BG_RED     =41,
        BG_GREEN   =42,
        BG_YELLOW  =43,
        BG_BLUE    =44,
        BG_CYAN    =46,
        BG_DEFAULT =49
    };
    class Md {
        Code code;
    public:
        Md(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os,const Md& mod) {
            return os << "\033[" << mod.code << "m";
        }
        operator const char*(){return "";}
    };
};


#ifdef DEBUG

#define LOGI(x) if(Loggran & 0x1) \
do{\
    std::cout <<  Tc::Md(Tc::FG_BLUE) << "I: "<< x << Tc::Md(Tc::FG_DEFAULT) <<"\r\n";\
    std::stringstream ss; ss << "I: "<< str_time() <<" "<< x << "\r\n";\
    f2l(ss); \
}while(0);


//-----------------------------------------------------------------------------
// WARNING LOGING
#define LOGW(x) if(Loggran & 0x2)\
do{\
    std::cout <<  Tc::Md(Tc::FG_YELLOW) << "W: " << x << Tc::Md(Tc::FG_DEFAULT) <<"\r\n";\
}while(0);

#define SGLOGW(x) if(Loggran & 0x2)\
do{\
    std::cout <<  Tc::Md(Tc::FG_YELLOW) << "WS " << x << Tc::Md(Tc::FG_DEFAULT) <<"\r\n";\
}while(0);

//-----------------------------------------------------------------------------
// ERROR LOGING
#define LOGE(x) \
do{\
    std::cerr <<  Tc::Md(Tc::FG_RED) << "E: " << x << Tc::Md(Tc::FG_DEFAULT) <<"\r\n";\
}while(0);

//-----------------------------------------------------------------------------
// FATAL LOGING
#define LOGEX(x) \
do{\
    std::cerr <<  Tc::Md(Tc::FG_RED) << "F: " << x << Tc::Md(Tc::FG_DEFAULT) <<"\r\n";\
    std::stringstream ss; ss << "F: "<< str_time() <<" "<< x << "\r\n";\
    f2l(ss,true);  \
}while(0);


//-----------------------------------------------------------------------------
// // TRACE LOGING
#define LOGD1(x) if(Loggran & 0x08)\
do{\
    std::cout << Tc::Md(Tc::FG_GREEN) << "D: "<< x << Tc::Md(Tc::FG_DEFAULT) <<"\r\n";\
}while(0);


//-----------------------------------------------------------------------------
// OUTPUT LOGING
#define LOGD2(x) if(Loggran & 0x08)\
    do{\
        std::cout << Tc::Md(Tc::FG_CYAN) << "D: "<< x << Tc::Md(Tc::FG_DEFAULT) <<"\r\n";\
    }while(0);
//-----------------------------------------------------------------------------
// OUTPUT LOGING
#define LOGD3(x) if(Loggran & 0x10)\
do{\
    std::cout << Tc::Md(Tc::FG_BLUE) << "O: " << x << Tc::Md(Tc::FG_DEFAULT) <<"\r\n";\
}while(0);


#else

#define LOGI(x) if(Loggran & 0x1) \
do{\
    std::stringstream ss; ss << "I: "<< str_time() <<" "<< x << "\r\n";\
    f2l(ss); \
}while(0);

//-----------------------------------------------------------------------------
// WARNING LOGING
#define LOGW(x) if(Loggran & 0x2)\
do{\
    std::stringstream ss; ss << "W: "<< str_time()<<" " << x << "\r\n";\
    f2l(ss); \
}while(0);

//-----------------------------------------------------------------------------
// SCRIPT WARNING LOGING
#define SGLOGW(x) if(Loggran & 0x2)\
do{\
    std::stringstream ss; ss << "WS "<< str_time()<<" " << x << "\r\n";\
    f2l(ss); \
}while(0);



//-----------------------------------------------------------------------------
// ERROR LOGING
#define LOGE(x) \
do{\
    std::stringstream ss; ss  <<  "E: "<< str_time()<<" " << x << "\r\n";\
    f2l(ss); \
}while(0);

//-----------------------------------------------------------------------------
// ERROR LOGING
#define LOG(x) \
do{\
    std::stringstream ss; ss  <<  "ES "<< str_time()<<" " << x << "\r\n";\
    f2l(ss); \
}while(0);

//-----------------------------------------------------------------------------
// ERROR LOGING
#define LOGEX(x) \
do{\
    std::stringstream ss; ss  <<  "F: "<< str_time()<<" " << x << "\r\n";\
    f2l(ss,true); \
}while(0);

//-----------------------------------------------------------------------------
// // TRACE LOGING
#define LOGD1(x) if(Loggran & 0x8)\
do{\
    std::stringstream ss; ss  <<  "D: "<< str_time()<<" " << x << "\r\n";\
    f2l(ss); \
}while(0);

//-----------------------------------------------------------------------------
// OUTPUT LOGING
#define LOGD2(x) if(Loggran & 0x10)\
do{\
    std::stringstream ss; ss  <<  "O: " << str_time()<<" "<< x << "\r\n";\
    f2l(ss); \
}while(0);

//-----------------------------------------------------------------------------
// OUTPUT LOGING
#define LOGD3(x) if(Loggran & 0x20)\
do{\
    std::stringstream ss; ss  << "X: " << x << "\r\n";\
    f2l(ss); \
}while(0);

#endif //

class Inst;
bool is_unlocked();
void save_passw(const char* admpsw);

struct LogScop
{
    std::string op;
    LogScop(const char* ch);
    ~LogScop();
};


#endif // LOGS_H
