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

#include "pwmdev.h"
#include "inst.h"

PwmDev::PwmDev(const char* syspath, int period, int val, bool inv,
                const char* name):
                DvPwm(syspath,period,val,inv),Divais(eINT, ePWMM, name),Reg<PwmDev>(this)
{
    _o.BindCppObject(this);
}


PwmDev::PwmDev(SqObj& o, const char* syspath, int period, int val, bool inv,
               const char* name):DvPwm(syspath,period,val,inv),
                    Divais(eINT, ePWMM, name),
                    Reg<PwmDev>(this)
{
    plug_it(o, name);
}



PwmDev::~PwmDev()
{
}

int      PwmDev::set_duty(EPWM_VAL val)
{
    if(_reversed)
        val=100-val;
    int ival = SET_DUTY(_period(),val);
    std::string s = std::to_string(ival);
    return this->bwrite((const uint8_t*)s.c_str(), s.length());
}

int      PwmDev::set_abs_duty(EPWM_VAL val)
{
    if(_reversed)
        val=_period()-val;
    int ival = val;
    std::string s = std::to_string(ival);
    return this->bwrite((const uint8_t*)s.c_str(), s.length());
}


EPWM_VAL  PwmDev::get_duty()
{
    if(_mon_dirt)
    {
        _mon_dirt = false;
        int val = _curdata.to_t<int>();
        return _reversed ? 100-val : val;
    }
    char val[8]={0};
    if(this->bread((uint8_t*)val, sizeof(val)))
    {
        if(_monitor)
            _check_dirt();
        return GET_DUTY(_period(), _curdata.to_t<int>());
    }
    return -1;
}

bool  PwmDev::_write_now(const any_t& vl)
{
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  PwmDev::_fecth(any_t& vl, const char* filter)
{
    return get_duty();
}

bool PwmDev::_mon_pick(size_t t)
{
    get_duty();
    return _mon_dirt;
}

bool PwmDev::call_back(SqMemb& m)
{
    if(m.IsNull()){
        _monitor = false;
        if(!_on_event.IsNull())
            _on_event.Release();
    }else{
        _on_event=m;
        _monitor = true;
    }
    return _monitor;
}

void PwmDev::on_event(E_VENT e, const uint8_t* val, int len, int options)
{
    int ival = ::atoi((const char*)val);
    if(_reversed)
    {
        ival = 100-ival;
    }
    _curdata.let(ival); //stored as "1" string
}

bool	PwmDev::_set_values(const char* key, const char* value)
{
    if(key[0]=='d' || key[0]=='v') // duty / value
        return set_duty(::atoi(value));
    if(key[0]=='r') { _reversed = value[0]==1 ? true : false; return true;}
    return false;
}

const char*	PwmDev::_get_values(const char* key)
{
    if(key[0]==ALLDATA)
    {
        _forjson += "&duty=";
        _forjson += _curdata.to_string<int>();
        _forjson += "&reverse=";
        _forjson += std::to_string(_reversed);
        return _forjson.c_str();
    }
    if(key[0]=='v') // duty
        return _curdata.to_string<int>().c_str();
    if(key[0]=='i') { return _reversed ? "1" : "0";}
    GETER_SYSCAT();
}

