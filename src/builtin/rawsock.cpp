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

#include "rawsock.h"
#include "inst.h"


RawSock::RawSock(E_TYPE e,
                 const char* ip,
                 int port,
                 const char* name):DvSocket(ip,port),
                                    Divais (e, eSOCKET, name),
                                    Reg<RawSock>(this),
                                    RtxBus<RawSock>(this,false,true)
{
    _o.BindCppObject(this);

}

RawSock::RawSock(SqObj& o,
                 E_TYPE e,
                 const char* ip,
                 int  port,
                 const char* name):DvSocket(ip,port),
    Divais (e, eSOCKET, name),
    Reg<RawSock>(this),
    RtxBus<RawSock>(this,false)
{
    plug_it(o, name);
}

RawSock::~RawSock()
{
    this->iclose();
}

bool  RawSock::_write_now(const devdata_t& vl)
{
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  RawSock::_fecth(devdata_t& vl, const char* filter)
{
    return 0;
}

bool RawSock::_mon_pick(time_t tnow)
{

    return false;
}

void RawSock::on_event_(SqMemb& m)
{
    _cach = false;
    if(m.IsNull())
    {
        _monitor = false;
        if(!_on_event.IsNull())
            _on_event.Release();
    }
    else {
        if(!_on_event.IsNull())
            _on_event.Release();
        _monitor = true;
        _on_event=m;
    }
}

int RawSock::puts(const char* b, int sz)
{
    return this->sendall((const unsigned char*)b,sz);
}

const char* RawSock::gets(int chars, int to)
{
    this->select_receive(_iobuff->buf(), _iobuff->cap(), to);
    return (const char*)_iobuff->buf();
}

int RawSock::write(Sqrat::Array& a)
{
    int sz = a.GetSize();
    if(sz<512)
    {
        fastbuf_t  ptr(sz);

        a.GetArray((uint8_t*)ptr, sz);
        return this->sendall((const unsigned char*)ptr,sz);
    }
    return 0;
}

Sqrat::Array RawSock::read(int bytes, int to)
{
    int by = this->select_receive(_iobuff->buf(), _iobuff->cap(), to);
    if(by>0)
    {
        SqArr  rar(App->psqvm(),by );
        for(int i = 0 ; i < bytes; i++)
        {
            rar.SetValue(i, _iobuff->at(i));
        }
        return rar;
    }
    SqArr nulla;
    return nulla;
}

void RawSock::on_event(E_VENT e, const uint8_t* buff, int len, int options)
{
}

bool	RawSock::_set_values(const char* key, const char* value)
{
    return false;
}

const char*	RawSock::_get_values(const char* key)
{
    return "";
}




