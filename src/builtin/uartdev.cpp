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

#include "uartdev.h"
#include "inst.h"
#include "rtxbus.h"
#include "divais.h"


UartDev::UartDev(E_TYPE  e,
                 const char* dev,
                 int bps,
                 const char* mode,
                 const char *name):
                     DvSerial(dev,bps,mode),
                     Divais(e,eUART, name),
                     Reg<UartDev>(this),
                     RtxBus<UartDev>(this, false,true)
{
    _cr.append(1, 0xD);
    _cr.append(1, 0xA);

    _o.BindCppObject(this);
}

UartDev::UartDev(SqObj& o,
                 E_TYPE  e,
                 const char* dev,
                 int bps,
                 const char* mode,
                 const char *name):
                     DvSerial(dev,bps,mode),
                     Divais(e,eUART,name),
                     Reg<UartDev>(this),
                     RtxBus<UartDev>(this,false,true)
{
    plug_it(o,name);
    _cr.assign(1, 0xD);
}

UartDev::~UartDev()
{
}

bool  UartDev::_write_now(const devdata_t& vl)
{
    _mon_dirt = true;
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  UartDev::_fecth(devdata_t& vl, const char* filter)
{
    return 0;
}

bool UartDev::_mon_pick(time_t tnow)
{
    if(_etype==eSTRING)
        RtxBus<UartDev>::_gets();
    else
        RtxBus<UartDev>::_read();
    return _mon_dirt;
}

const char* UartDev::_gets()
{
    _mon_dirt = false;
    const char* rv =  RtxBus<UartDev>::_gets();
    return rv;
}

const char* UartDev::_getsln(char ceol)
{
    _mon_dirt = false;
    time_t fut = tick_count()+_tout;
    while(tick_count()<fut)
    {
        const char* pb = RtxBus<UartDev>::_gets();
        if(*pb){
            _tmp2.append((const uint8_t*)pb, ::strlen(pb));
        }
        size_t xeol = _tmp2.find(ceol);
        if(xeol != std::string::npos)
        {
            _tmpstr = _tmp2.substr(0,xeol);
            _tmp2 = _tmp2.substr(xeol+1);
            if(!_tmpstr.empty()){
                _mon_dirt = false;
                return (const char*)_tmpstr.c_str();
            }
        }
        _truncate(_tmp2);
    }
    return "";
}

SqArr UartDev::_read()
{
    _mon_dirt = false;
    return RtxBus<UartDev>::_read();
}

bool UartDev::on_event_(SqMemb& m)
{
    if(m.IsNull())
    {
        _monitor = false;
        if(!_on_event.IsNull())
            _on_event.Release();
    }
    else
    {
        if(!_on_event.IsNull())
            _on_event.Release();
        _monitor = true;
        _on_event=m;
    }
    return _monitor;
}

void UartDev::on_event(E_VENT e,
                       const uint8_t* buff,
                       int len,
                       int options)
{
}

bool	UartDev::_set_values(const char* key, const char* value)
{
    return bwrite((const uint8_t*)value, ::strlen(value));
}

const char*	UartDev::_get_values(const char* key)
{
    if(key[0]==ALLDATA)
    {
        _forjson += "&value=";
        _forjson += _curdata.c_chars();
        return _forjson.c_str();
    }
    return _curdata.c_chars();
}
