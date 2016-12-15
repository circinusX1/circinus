/*
Copyright (c) 2014-2016 PANGEEA.DPT All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/pangeea. The name of the
https://github.com/pangeea may not be used to endorse or promote
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
                                    Divais (e, name),
                                    Reg<RawSock>(this),
                                    RtxBus<RawSock>(this),_bytes(nullptr)
{
    _o.BindCppObject(this);

}

RawSock::RawSock(SqObj& o,
                 E_TYPE e,
                 const char* ip,
                 int  port,
                 const char* name):DvSocket(ip,port),
    Divais (e, name),
    Reg<RawSock>(this),
    RtxBus<RawSock>(this),_bytes(nullptr)
{
    ctx_it(o, name);
}

RawSock::~RawSock()
{
    this->iclose();
    delete[] _bytes;
}

bool  RawSock::_write_now(const any_t& vl)
{
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  RawSock::_read_now(any_t& vl, const char* filter)
{
    return 0;
}


bool RawSock::_touch_it(size_t t)
{
    _cach = this->bread(_bytes, _nbytes, 0);
    return _mon_dirt;
}

void RawSock::set_monitor(int bytes)
{
    _cach = false;
    if(bytes==0)
    {
        delete[] _bytes;
        _bytes = nullptr;
        _monitor = false;
    }
    else {
        _monitor = true;
        _bytes = new uint8_t[bytes];
        _nbytes = bytes;
    }
}

int RawSock::puts(const char* b, int sz)
{
    return this->sendall((const unsigned char*)b,sz);
}

const char* RawSock::gets(int chars, int to)
{
    if(_bytes==nullptr)
    {
        _bytes = new uint8_t[chars+1];
        _nbytes = chars + 1;
    }
    if(chars > (int)_nbytes)
    {
        delete[] _bytes;
        _bytes = new uint8_t[chars+1];
        _nbytes = chars + 1;
    }
    this->select_receive(_bytes, _nbytes, to);
    return (const char*)_bytes;
}

int RawSock::write(Sqrat::Array& a)
{
    int             sz = a.GetSize();
    if(sz<512)
    {
        fastbuf_t<512>  ptr(sz);

        a.GetArray((uint8_t*)ptr, sz);
        return this->sendall((const unsigned char*)ptr,sz);
    }
    return 0;
}

Sqrat::Array RawSock::read(int bytes, int to)
{
    if(_bytes==nullptr)
    {
        _bytes = new uint8_t[bytes+1];
        _nbytes = bytes + 1;
    }
    if(bytes > (int)_nbytes)
    {
        delete[] _bytes;
        _bytes = new uint8_t[bytes+1];
        _nbytes = bytes + 1;
    }
    int by = this->select_receive(_bytes, _nbytes, to);
    if(by>0)
    {
        SqArr           rar(App->psqvm(),by );
        for(int i = 0 ; i < bytes; i++)
        {
            rar.SetValue(i, _bytes[i]);
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




