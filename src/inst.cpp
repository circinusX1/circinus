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

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <sys/wait.h>
#include "inst.h"
#include "apis.h"
#include "logs.h"
#include "filedev.h"
#include "gpiodev.h"
#include "i2cdev.h"
#include "pwmdev.h"
#include "spidev.h"
#include "uartdev.h"
#include "usbdev.h"
#include "sshcomm.h"
#include "rtxbus.h"
#include "solib.h"
#include "scrbase.h"
#include "rawsock.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/error.h"

Inst*               App;

Inst::Inst(SqEnvi& sq, char* p[]):Sqrat::Class<Inst>(sq.theVM(),"App")
{
    App = this;
    _thisbin = p[0];
    if(p[1]){ _param = p[1]; }
    this->Functor(_SC("notify"), &Inst::notify);
    this->Functor(_SC("get_dev"), &Inst::get_sqo);
    this->Functor(_SC("set_timer"), &Inst::set_timer);
    this->Functor(_SC("set_priority"), &Inst::set_timer);
    this->Functor(_SC("start_task"), &Inst::start_task);
    this->Functor(_SC("_sync"), &Inst::sync_all);
    Sqrat::RootTable().Bind(_SC("App"), *this);
}

Inst::~Inst()
{
    if(_childrens.size())
    {
        int st;
        LOGI("waiting for " << _childrens.size() << " children to end");
        for(auto& pid : _childrens)
        {
             ::waitpid(pid, &st, 0);
             if(st==0){
                LOGI("child process" << pid << "exited normally ");
             }
             else {
                LOGI("child process" << pid << "exited with error ");
             }
        }
    }
    App=nullptr;
}

int    Inst::set_priority(int p)
{
    int zerotoforty = p + 20;
    if(zerotoforty>=0 && zerotoforty<=40) // 40 lower 0 max, 20-normal
    {
        _sleep_loop = (p * 3);
    }
    return p;
}

void    Inst::get_alldevs(devsmap_t& refrdevs, EPERIPH et, bool update)
{
    for (const auto& a :_devs)
    {
        IoOps* po = dynamic_cast<IoOps*>(a.second);
        if(et == eNOCAT)
        {
            refrdevs[a.second] = READDEV;
        }
        else if(et == po->peer_of())
        {
            refrdevs[a.second] = READDEV;
        }
    }
}

void Inst::remove(const Divais* p)
{
    _devs.erase(p->dev_key());
}

const char* Inst::notify(const char* appname)
{
    return param();
}

SQVM* Inst::psqvm()
{
    return VM();
}

SqEnvi* Inst::scr_env()
{
    return __sq_env;
}

void Inst::add_this(I_IDev* o, const char* name)
{
    LOGD1("adding dev: " << name);
    _devs[name] = o;
}

void    Inst::remove_obj(const char* name)
{
    std::map<std::string, I_IDev*>::iterator it = _devs.find(name);
    if(it != _devs.end())
    {
        _devs.erase(it);
    }
}

void    Inst::sync_all()
{
    for(auto& u : _devs)
    {
        u.second->sync();
    }
}

void    Inst::comit_devs()
{
    for(auto& u : _pending)
    {
        size_t eq = u.second.find('=');
        if(eq != NOPOS)
        {
            u.first->set_value(u.second.substr(0,eq).c_str(),
                               u.second.substr(eq+1).c_str());
        }
        else if(u.second==READDEV)
        {
            u.first->sync(u.second.c_str()); //
        }
    }
    _pending.clear();
}

void Inst::add(Divais* p)
{
    const char* pk = p->dev_key();
    LOGD1("adding class " << pk);
    _devs[pk]=p;
}

void   Inst::call_backs(time_t curtick)
{
    for(auto& e: _cbs)
    {
        TimCb& t = e.second;
        if(curtick - t.remember >= t.delay)
        {
            t.remember = curtick;// - deltat; //reposition if deviates
            try{
                e.second.f.Fcall<bool>(this);
            }
            catch(Sqrat::Exception ex)
            {
                ApStat=RESTART;
                LOGEX( __PRETTY_FUNCTION__ << ex.Message()  << " ");
                LOGEX(SqErrStr);
            }
        }
    }

    ::usleep(_sleep_loop);
}

void    Inst::thread_main()
{
    while(ApStat==RUNNING && !this->is_stopped())
    {
        sleep(1);
    }
}

IoOps* Inst::get_devi(const char* name)
{
    if(_devs.find(name)!=_devs.end())
    {
        IoOps* pio = dynamic_cast<IoOps*>(_devs[name]);
        return pio;
    }
    return nullptr;
}

I_IDev* Inst::get_module(const char* name)
{
    if(_devs.find(name)!=_devs.end())
        return _devs[name];
    return nullptr;
}

void   Inst::_dequeue_events(std::vector<I_IDev*>& arr)
{
    ;
}

void   Inst::check_devs(std::vector<I_IDev*>& arr, size_t t)
{
    for(auto& d : _devs)
    {
        if(d.second->is_dirty(t))
        {
            const Sqrat::Object& o = d.second->object();
            if(o.IsNull())
            {
                LOGW("If the object is monitorred call: _o.BindCppObject(this); in c-tor")
            }
            else
            {
                arr.push_back(d.second);
            }
        }
    }
    //
    //  check the rising falling hooks queue
    //
    _dequeue_events(arr);
}

I_IDev* Inst::get_per(const char* name)
{
    const auto& a = _devs.find(name);
    if(a!=_devs.end())
    {
        return a->second;
    }
    for(const auto& a:_devs)
    {
        if(!::strcmp(a.second->dev_key(),name))
        {
            return a.second;
        }
    }
    return nullptr;
}

Sqrat::Object Inst::get_sqo(const char* name)
{
    const auto& a = _devs.find(name);
    if(a!=_devs.end())
    {
        return a->second->object();
    }
    for(const auto& a:_devs)
    {
        if(!::strcmp(a.second->dev_key(),name))
        {
            return a.second->object();
        }
    }
    Sqrat::Object o;
    return o;
}

int Inst::set_timer(SqMemb f, int milis, size_t uniq)
{
    const auto& a = _cbs.find(uniq);
    if(a != _cbs.end())
    {
        if(milis==0)
        {
            _cbs.erase(uniq);
            return 0;
        }
        return -1; /* exist*/
    }
    TimCb t;
    t.remember = tick_count(); /* now */
    t.delay    = milis;
    t.f        = f;
    _cbs[uniq] = t;
    return uniq;
}

void Inst::web_set_data(const devsmap_t& devs, int apply)
{
    std::unique_lock<std::mutex> lck(__bsqenv->mutex_);
    _pending = devs;
    LOGD2(__FUNCTION__);
    if(apply > 0)
    {
        __bsqenv->let_thread_go();
        __bsqenv->condvar_.wait(lck,[]{return __bsqenv->ready_.load();} );
        assert(__bsqenv->ready_.load()==true);
    }
}

/**
 * @brief Inst::start_task
 * @param script_file
 * @return (check the instance singleton thing)
 */
int Inst::start_task(const char* script_file)
{
    const char* progname = _thisbin.c_str();
    const char *argument1 = script_file;
    const char * argument2 = "@bypass_singleton";

    pid_t   pricid = ::fork();
    if (pricid == 0)
    {
        /**
          in child
          */
        RestDisabled = true;
        LOGI("spawning: " << progname <<" " << argument1);
        ::execl(progname, progname, argument1, argument2, (char *)NULL);
    }
    _childrens.push_back(pricid);
    return pricid;
}

