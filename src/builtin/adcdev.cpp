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

AdcDev::AdcDev(const char* fname,
                 const char* name):DvAdc(fname),
                 Divais(eINT, eADC, name),Reg<AdcDev>(this)
{
    _o.BindCppObject(this);
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
    if(_mon_dirt)
    {
        _mon_dirt = false;
        return _curdata.to_t<int>();
    }
    _curdata.clear();
    char val[8] = {0};
    size_t  by;
    if((by=this->bread((uint8_t*)val, sizeof(val)))) //comes as string
    {
        _mon_dirt=_check_dirt();
        return _curdata.atoi();
    }
    return -INT_MAX;
}

bool  AdcDev::_write_now(const any_t& vl)
{
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  AdcDev::_fecth(any_t& vl, const char* filter)
{
    return get_value();
}

bool AdcDev::_mon_pick(size_t t)
{
    get_value();
    return _mon_dirt;
}

bool AdcDev::call_back(SqMemb& m)
{
    if(m.IsNull()){
        _monitor = false;
        if(!_on_event.IsNull())
            _on_event.Release();
    }else{
        _on_event=m;
        _monitor = true;
        get_value(); // clear ditry
    }
    return _monitor;
}

void AdcDev::on_event(E_VENT e, const uint8_t* buff, int len, int options)
{
    int ival = ::atoi((const char*)buff);
    _curdata.let(ival);
}

const char*	AdcDev::_get_values(const char* key)
{
    if(key[0]==ALLDATA)
    {
        _forjson += "value=";
        _forjson += _curdata.to_string<int>();
        return _forjson.c_str();
    }
    else if(key[0]=='v'/*alue*/)
        return _curdata.c_chars();
    GETER_SYSCAT();
}

bool	AdcDev::_set_values(const char* key, const char* value)
{
    if(key[0]=='m'/*onitor*/) //monitor
    {
        _monitor = value[0]=='1' ? true : false;
        return true;
    }
    return false;
}
