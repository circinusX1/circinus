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
#include "inst.h"
#include "apis.h"
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
    if(p[1]){ _param = p[1]; }
    this->Functor(_SC("notify"), &Inst::notify);
    this->Functor(_SC("get_dev"), &Inst::get_sqo);
    this->Functor(_SC("set_timer"), &Inst::set_timer);
    Sqrat::RootTable().Bind(_SC("App"), *this);
}

Inst::~Inst()
{
    for(auto& a : _dlls)
        ::dlclose(a);
    App=nullptr;
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

void Inst::add_obj(I_IDev* o, const char* name)
{
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
}

void    Inst::thread_main()
{
    while(ApStat==RUNNING && !this->is_stopped())
    {
        sleep(1);
    }
}

IoOps* Inst::get_proxy(const char* name)
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

void   Inst::check_devs(std::vector<I_IDev*>& arr, size_t t)
{
    for(auto& d : _devs)
    {
        if(d.second->is_monitorred(t))
        {
            arr.push_back(d.second);
        }
    }
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

void Inst::web_set_data(const devsmap_t& devs, bool apply)
{
    std::unique_lock<std::mutex> lck(__bsqenv->mutex_);
    _pending = devs;

    if(apply)
    {
        __bsqenv->let_thread_go();
        __bsqenv->condvar_.wait(lck,[]{return __bsqenv->ready_.load();} );
        assert(__bsqenv->ready_.load()==true);
    }
}


