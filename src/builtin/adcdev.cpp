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

#include "adcdev.h"
#include "inst.h"

bool AdcDev::_squed;

AdcDev::AdcDev(const char* fname,
                 const char* name):DvAdc(fname),
                 Divais(eINT, eADC, name),Reg<AdcDev>(this)
{
    AdcDev::_squed ? _o.BindCppObject(this) : (void)(0);
}

AdcDev::AdcDev(SqObj& o,
                 const char* fname, const char* name):DvAdc(fname),
                 Divais(eINT, eADC, name),Reg<AdcDev>(this)
{
    plug_it(o, name);
}

AdcDev::~AdcDev()
{
    ;
}

int  AdcDev::get_value()
{
    char val[16] = {0};

    _cur_value.clear();
    if(this->bread((uint8_t*)val, sizeof(val))>0) //comes as string ->on_event
    {
        return _cur_value.atoi();
    }
    return -INT_MAX;
}

bool  AdcDev::_write_now(const devdata_t& vl)
{
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  AdcDev::_fecth(devdata_t& vl, const char* filter)
{
    return get_value();
}

bool AdcDev::_mon_callback(time_t tnow)
{
    return _call_cb(get_value());
}

bool AdcDev::set_cb(SqMemb& m)
{
    return this->Divais::set_cb(m);
}

void AdcDev::on_event(E_VENT e, const uint8_t* buff, int len, int options)
{
    _mon_dirt = true;
    int ival = ::atoi((const char*)buff);
    _cur_value.let(ival);
}

const char*	AdcDev::_get_values(const char* key)
{
    if(key[0]=='V') { return _cur_value.c_chars();}
    GETER_SYSCAT(); return Divais::_get_values(key);
}

bool	AdcDev::_set_value(const char* key, const char* value)
{
    return Divais::_set_value(key,value);
}
