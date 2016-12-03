#ifndef ICOMBASEX_H
#define ICOMBASEX_H

#include <stdint.h>
#include <stddef.h>
#include <iostream>
#include <inttypes.h>
#include "squirrel.h"

typedef const uint8_t  cbyte;


class IComm;
class IDevice
{
public:
    enum D_MSG{eCLOSED=-1,  eIDLE=0,
                            eOPENDED=1,
                            eBINARY=100,
                            eTEXT=101,
                            eNUMBER=102,
                            eDISCARD=200,
                            eWAITING=201,
                            eRECEIVING=202,
                            eSENDING=203,
                            eHANG=204,
                            eERROR=256};
    IDevice(){}
    virtual ~IDevice(){}
    virtual void addDev(IComm*)=0;
    virtual void remDev(IComm*)=0;
    virtual int  devEvent(IComm*, IDevice::D_MSG, cbyte*, int)=0;
    virtual const char* name()const=0;
};

class IComm
{
public:
    IComm(){}
    virtual ~IComm(){}
    virtual int open()=0;
    virtual int close()=0;
    virtual int reopen(const char* where)=0;
    virtual void setspeed(int ms)=0;
    virtual int sendTo(cbyte*, int)=0;
    virtual int recFrom(unsigned char* bytes, int room)=0;
    virtual void debug(int d) = 0;
    virtual void setTout(int to, int selto)=0;
    virtual bool isOpen()=0;
    virtual void flush()=0;
    virtual int esc(const char* dt, int how)=0;
    virtual int setOptions(const char*)=0;
    virtual const char* getOptions(const char*)=0;
    virtual IDevice::D_MSG getMode()const=0;
    virtual const char* name()const=0;
    virtual void  publish()=0;
};


#ifdef __linux
#   define EXPORT_CLS __attribute__((visibility("default")))
#else
#   define EXPORT_CLS __declspec(dllexport)
#endif


typedef IComm* (*pGI)(IDevice* pd, const char* cs, const char*, HSQUIRRELVM);
typedef void (*pRI)(IComm* pc);

#ifdef SO_LIBRARY
extern "C"
{
    EXPORT_CLS  IComm* getInstance(IDevice* pd, const char* cs, const char* name, HSQUIRRELVM);
    EXPORT_CLS  void destroyInstance(IComm*);
}
#endif

extern int _Debug;

#ifdef __linux

#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define LOGSEND(x)      if(_Debug) {std::cout << KGRN <<"[D]<--: "<< x << RST << std::endl;}
#define LOGREC(x)       if(_Debug) {std::cout << KYEL <<"[D]-->: " << x << RST << std::endl;}
#define LOGERR(x)       {std::cout << KRED << "Err: " << x << RST << std::endl;}
#define LOGWARN(x)      if(_Debug) {std::cout << "Wrn: " << KCYN << x << RST << std::endl;}
#define LOGDETAIL(x)    if(_Debug>=2) {std::cout << "Inf: " << KMAG << x << RST << std::endl;}
#define LOGDETAIL2(x)   if(_Debug>=3) {std::cout << "Log: "<< KGRN << x << RST << std::endl;}

#define TC_YELLOW    KYEL
#define TC_KCYN      KCYN
#define TC_WHITE     RST
#define TC_KRED      KRED


#else

#include <iostream>
#include <windows.h>

namespace tclr
{
    inline std::ostream& blue(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
                  |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& red(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
                    FOREGROUND_RED|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& green(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
                  FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& cyan(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
                  FOREGROUND_INTENSITY|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& yellow(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
             FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& white(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
           FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
        return s;
    }

    struct color {
        color(WORD attribute):m_color(attribute){};
        WORD m_color;
    };

    template <class _Elem, class _Traits>
    std::basic_ostream<_Elem,_Traits>&
          operator<<(std::basic_ostream<_Elem,_Traits>& i, color& c)
    {
        HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,c.m_color);
        return i;
    }

};// namespace // Copyleft Vincent Godin

#define LOGSEND(x)  if(_Debug) std::cout <<"<-: "<<tclr::green << x << tclr::white << std::endl;
#define LOGREC(x)   if(_Debug) std::cout <<"->: "<<tclr::yellow << x << tclr::white << std::endl;
#define LOGERR(x)   std::cout << tclr::red << "Err: " << x << tclr::white << std::endl;
#define LOGWARN(x)  if(_Debug) std::cout << "Wrn: " << tclr::blue << x << tclr::white << std::endl;
#define LOGDETAIL(x)  if(_Debug>=2) std::cout << "Inf: " << tclr::cyan << x << tclr::white << std::endl;
#define LOGDETAIL2(x)  if(_Debug>=3) std::cout << "Log: " << tclr::green << x << tclr::white << std::endl;

#define TC_YELLOW     tclr::yellow
#define TC_WHITE      tclr::white
#define TC_KCYN       clr::cyan
#define TC_KRED       tclr::red

#endif //WIN

#endif // COMBASE_H
