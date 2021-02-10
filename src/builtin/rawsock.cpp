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

bool RawSock::_mon_callback(time_t tnow)
{
    if(_etype==eSTRING){
        const char* rv = gets(_bufsz);
        return this->Divais::_call_cb(rv);
    }

    const SqArr& rv = read(_bufsz);
    return this->Divais::_call_cb(rv);
}

bool RawSock::set_cb(SqMemb& m)
{
    return this->Divais::set_cb(m);
}

int RawSock::puts(const char* b, int sz)
{
    return this->sendall((const unsigned char*)b,sz);
}

const char* RawSock::gets(int chars)
{
    bytes_t loco(chars);

    _t1.clear();
    const int bytes = this->select_receive(loco.data(), loco.cap(), _tout);
    if(bytes){
        loco.resize(bytes);
        _t1.assign(loco.data(),loco.length());
    }
    return (const char*)_t1.data();
}

int RawSock::write(Sqrat::Array& a)
{
    const int sz = a.GetSize();
    bytes_t loco(sz);
    a.GetArray(loco.data(), sz);
    return this->sendall(loco.data(),sz);
}

Sqrat::Array RawSock::read(int maxb)
{
    bytes_t loco(maxb);
    const int bytes = this->select_receive(loco.data(), loco.cap(), _tout);
    if(bytes>0)
    {
        SqArr  rar(App->psqvm(), bytes);
        loco.resize(bytes);
        for(int i = 0 ; i < bytes; i++)
        {
            rar.SetValue(i, loco[i]);
        }
        return rar;
    }
    return _emptyarr;
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

void RawSock::_set_touts(time_t tout)
{
    this->_tout = tout;
}

void RawSock::_set_buffers(size_t bytes)
{
    this->_bufsz = bytes;
}



