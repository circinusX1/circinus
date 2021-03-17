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

bool PwmDev::_squed;

PwmDev::PwmDev(const char* syspath, int period, int val, bool inv,
                const char* name):
                DvPwm(syspath,period,val,inv),Divais(eINT, ePWMM, name),Reg<PwmDev>(this)
{
    PwmDev::_squed ? _o.BindCppObject(this) : (void)(0);
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
    char val[8]={0};
    if(this->bread((uint8_t*)val, sizeof(val)))
    {
        return GET_DUTY(_period(), _cur_value.to_t<int>());
    }
    return -1;
}

bool  PwmDev::_write_now(const devdata_t& vl)
{
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  PwmDev::_fecth(devdata_t& vl, const char* filter)
{
    return get_duty();
}

bool PwmDev::_mon_callback(time_t tnow)
{
    return false;
}

void PwmDev::on_event(E_VENT e, const uint8_t* val, int len, int options)
{
    int ival = ::atoi((const char*)val);
    if(_reversed)
    {
        ival = 100-ival;
    }
    _cur_value.let(ival); //stored as "1" string
}

bool	PwmDev::_set_value(const char* key, const char* value)
{
    if(key[0]=='D' || key[0]=='V') return set_duty(::atoi(value));
    if(key[0]=='R') { _reversed = value[0]==1 ? true : false; return true;}
    return Divais::_set_value(key, value);
}

const char*	PwmDev::_get_values(const char* key)
{
    if(key[0]==ALLDATA)
    {
        _retparams += "&V=";
        _retparams += _cur_value.to_string<int>();
        _retparams += "&R=";
        _retparams += std::to_string(_reversed);
        return _retparams.c_str();
    }
    if(key[0]=='V' || key[0]=='D') // duty
        return _cur_value.to_string<int>().c_str();
    if(key[0]=='R') { return _reversed ? "1" : "0";}
    GETER_SYSCAT(); return Divais::_get_values(key);
}

