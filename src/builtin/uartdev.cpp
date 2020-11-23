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
                     RtxBus<UartDev>(this),_bytes(nullptr),_nbytes(0)
{
    _cr.assign(1, 0xD);
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
                     RtxBus<UartDev>(this),_bytes(nullptr),_nbytes(0)
{
    plug_it(o,name);
    _cr.assign(1, 0xD);
}

UartDev::~UartDev()
{
    delete[] _bytes;
}

bool  UartDev::_write_now(const any_t& vl)
{
    _mon_dirt = true;
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  UartDev::_fecth(any_t& vl, const char* filter)
{
    return 0;
}


bool UartDev::_mon_pick(size_t t)
{
    this->bread(_bytes, _nbytes);
    return _mon_dirt;
}

const char* UartDev::_gets(int chars)
{
    _mon_dirt = false;
    if(_bcurdata)
    {
        _bcurdata=false;
        return (const char*)_bytes;
    }
    return RtxBus<UartDev>::_gets(chars);
}

SqArr UartDev::_read(int chars)
{
    _mon_dirt = false;
    if(_bcurdata)
    {
        _bcurdata = false;
        SqArr  rar(App->psqvm(), _nbytes);
        for(size_t i = 0 ; i < _nbytes; i++)
        {
            rar.SetValue(i, _bytes[i]);
        }
        return rar;
    }
    return RtxBus<UartDev>::_read(chars);
}

bool UartDev::call_back(SqMemb& m, size_t bytes)
{
    if(bytes==0 || m.IsNull())
    {
        delete[] _bytes;
        _bytes = nullptr;
        _monitor = false;
        if(!_on_event.IsNull())
            _on_event.Release();

    }
    else {
        _monitor = true;
        _bytes = new uint8_t[bytes];
        _nbytes = bytes;
        _on_event=m;
    }
    _bcurdata = false;
    return _monitor;

}

void UartDev::on_event(E_VENT e, const uint8_t* buff, int len, int options)
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
