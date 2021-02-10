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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include "gpiodev.h"
#include "inst.h"

GpioDev::GpioDev(EGPIO_PIN pn,
                 EPIN_DIR pd, int on,
                 const char* name):DvGpio(pn,pd,on),
    Divais(eINT, eGPIO, name),Reg<GpioDev>(this),
    _sec(0),
    _dir(pd),
    _counter(0),
    _counting(false)
{
    _o.BindCppObject(this);
    LOGD3(__FUNCTION__ <<" "<< name);
}

GpioDev::GpioDev(SqObj& o,
                 EGPIO_PIN pn,
                 EPIN_DIR pd, int on,
                 const char* name):DvGpio(pn,pd,on),
    Divais(eINT, eGPIO, name),Reg<GpioDev>(this),
    _sec(0),
    _dir(pd),
    _counter(0),
    _counting(false)
{
    plug_it(o, name);
    LOGD3(__FUNCTION__ <<" "<< name);
}


//tone up to 500 Hz
GpioDev::GpioDev(EGPIO_PIN pn, int freq,
                 const char* name):DvGpio(pn,freq >0 ? eOut : eIn,0),
    Divais(eINT,eGPIO,name),Reg<GpioDev>(this),
    _sec(0),
    _dir(freq >0 ? eOut : eIn),
    _freq(freq > 0 ? freq : 0 ),
    _counter(0),
    _counting(freq < 0)
{
    _o.BindCppObject(this);
    _monitor = true;
    if(freq>=0)
    {
        if(_freq==0)
            _freq=2;
        _freq*=2;
    }
    else {
        _interval=-freq;
    }
    LOGD3(__FUNCTION__ <<" "<< name);
}

GpioDev::~GpioDev()
{
    LOGD3(__FUNCTION__ <<" "<< _name);
}

int    GpioDev::set_toggle()
{
    int v = get_value();
    if(v>=0)
    {
        v = !v;
        return set_value(v);
    }
    return -1;
}

int  GpioDev::set_value(int val)
{
    std::string s = std::to_string(val);
    return this->bwrite((const uint8_t*)s.c_str(), s.length());
}

int  GpioDev::get_value()
{
    _curdata.clear();
    char val[8]={0};
    if(this->bread((uint8_t*)val,sizeof(val)))
    {
        _mon_dirt=false;
        return _curdata.to_t<int>();
    }
    return -1;
}

bool  GpioDev::_write_now(const devdata_t& vl)
{
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  GpioDev::_fecth(devdata_t& vl, const char* filter)
{
    return get_value();
}

int GpioDev::get_freq()
{
    return _freq/2;
}

int GpioDev::set_freq(int freq)
{
    if(_dir & eOut)
    {
        if(freq==0)freq = 2;
        freq *= 2;
        return _freq = freq;
    }
    return 0;
}

bool GpioDev::_mon_pick(time_t tnow)
{


    if(_dir & eIn)
    {
        int cv = get_value();
        if(_counting)
        {
            if(tnow - _sec>_interval)
            {
                _freq = _counter;
                _counter  = 0;
                _sec = tnow;
            }
            if(cv != _curval)
            {
                ++_counter;
                _curval = cv;
            }
            return false;
        }
        _mon_dirt = _check_dirt();
        return _mon_dirt;
    }
    return false;
}

bool GpioDev::on_event_(SqMemb& mon)
{
    if(_dir & eOut || _counting )
    {
        LOGW("cannot moitor out pin or a counter");
        return false;
    }
    if(!mon.IsNull())
    {
        _watch_edge(1);
        _monitor = true;
        _edging = 1;
        if(!_on_event.IsNull())
            _on_event.Release();
        _on_event = mon;
    }else{
        _watch_edge(0);
        _monitor = false;
        if(!_on_event.IsNull())
            _on_event.Release();
    }
    return true;
}

void GpioDev::on_event(E_VENT e, const uint8_t* val, int len, int options)
{
    if(e==eREAD)
    {
        int ival = ::atoi((const char*)val);
        if(_reversed)
            ival = !ival;
        _curdata.let(ival);
    }
}

bool	GpioDev::_set_values(const char* key, const char* value)
{
    if(key[0]=='v') return set_value(::atoi(value));
    if(key[0]=='f') return  set_freq(::atoi(value));
    if(key[0]=='r') { _reversed = value[0]==1 ? true : false; return true;}
    if(key[0]=='t') { return set_toggle();}
    return false;
}

const char*	GpioDev::_get_values(const char* key)
{
    if(key[0]==ALLDATA)
    {
        _forjson += "&value=";
        _forjson += _curdata.to_string<int>();
        _forjson += "&freq=";
        _forjson += std::to_string(_freq);
        _forjson += "&reverse=";
        _forjson += std::to_string(_reversed);
        return _forjson.c_str();
    }
    if(key[0]=='v') { return _curdata.c_chars();}
    if(key[0]=='f') { ::sprintf(_sret,"%d",_freq); return _sret;}
    if(key[0]=='r') { ::sprintf(_sret,"%d",_reversed); return _sret;}
    GETER_SYSCAT();
}


