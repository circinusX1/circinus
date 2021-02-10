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

#ifndef X_CTXX_H_
#define X_CTXX_H_

#include <stdint.h>
#include <mutex>
#include <condition_variable>
#include "sqwrap.h"
#include "osthread.h"
#include "evqueue.h"
#include "divais.h"
#include "dlconfig.h"


typedef struct TimCb{
    size_t remember;
    size_t delay;
    SqMemb f;
}TimCb;


/**
 * @brief The App class
 * running context classIEmbix
 */
class RestSrv;
class Database;

class Inst : public Mpthrd, public Sqrat::Class<Inst>, public IInstance
{
public:
    friend class RestSrv;
    friend class Database;
    Inst(SqEnvi& sq, char* p[]);
    virtual ~Inst();

public:
    SQVM*   psqvm();
    SqEnvi* scr_env();
    void    add(Divais*);
    void    add_this(I_IDev* o, const char* name);
    void    remove_obj(const char* name);
    void    remove(const Divais*);
    void    call_backs(time_t t);
    void    thread_main();
    I_IDev* get_per(const char* name);
    Sqrat::Object get_sqo(const char* name);
    const   std::map<std::string, I_IDev*>& devs()const {return _devs;};
    int     set_timer(SqMemb f, int seconds, size_t id);
    void    web_set_data(const devsmap_t& devs, int apply);
    int     set_priority(int p);
    int     start_task(const char* script_file);
    bool    check_devs(time_t t);
    const   char* notify(const char* appname);
    const   char* param()const{return _param.c_str();}

    const   char* get_value(const char* per, const char* what);
    void    comit_devs();
    IoOps*   get_devi(const char*);
    I_IDev* get_module(const char*);
    void    get_alldevs(devsmap_t& refrdevs, EPERIPH et, bool update);
    void    sync_all();
private:
    void   _wait_more();
private:
    std::map<std::string, I_IDev*>  _devs;
    std::map<std::string, SqMemb>   _userflds;
    devsmap_t                       _pending;
    std::map<size_t, TimCb>         _cbs;
    std::vector<pid_t>              _childrens;
    std::string                     _webreq;
    std::string                     _webresp;
    std::string                     _param;
    std::string                     _thisbin;
    SqObj                           _o;
    bool                            _okey;
    int                             _priority = 0;  /* -19 to 20 */
    time_t                          _sleep_loop = 128;
};

typedef enum {eURGENTTASK=1}E_URGENTS;
typedef enum {EXIT_APP=-1, RUNNING=0, DO_EXIT=1, RELOAD_S=2}EALIV;

extern Inst*        App;

extern EALIV        ApStat;
extern std::string  SqErrStr;

#ifdef DEEP_DEBUG
#   define RESTART  RELOAD_S
#else
#   define RESTART  EXIT_APP
#endif

#include "logs.h"

#define CFGF    "/etc/rembix/rembix.conf"

template <typename T> struct Reg
{
    T* _pt;
    Reg(T* t):_pt(t){
        App->add(t);
    }
    ~Reg(){
        if(App)
            App->remove(_pt);
    }
};


#endif
