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

#ifndef RAWSOCK_H
#define RAWSOCK_H

#include "dsocket.h"
#include "divais.h"
#include "sqwrap.h"
#include "inst.h"
#include "rtxbus.h"

using namespace GenericHw;

#define RAW_SOC_MAX_BYTES   16384

class RawSock: public  DvSocket,
               public  Divais,
               private Reg<RawSock>,
               public  RtxBus<RawSock>// , public Divais
{
public:
    RawSock(E_TYPE e,const char* ip, int port ,const char* name=nullptr);
    RawSock(SqObj&, E_TYPE e, const char* ip, int port ,const char* name=nullptr);
    virtual ~RawSock();
    bool    set_cb(SqMemb& mem);
    int puts(const char* b, int sz);
    const char*  gets(int chars);
    int write(Sqrat::Array& a);
    Sqrat::Array read(int bytes);
    void _set_touts(time_t tout);
    void _set_buffers(size_t bytes);

    OVERW(RawSock,Divais)

    static void squit(SqEnvi& e){
        Sqrat::Class<RawSock> cls(e.theVM(), _SC("SOCKET"));
        cls.Ctor<E_TYPE, const char*, int ,const char*>();
        cls.Ctor<SqObj&, E_TYPE, const char*, int,const char*>();

        cls.Functor(_SC("plug_it"), &RawSock::plug_it);
        cls.Functor(_SC("set_cb"), &RawSock::set_cb);
        cls.Functor(_SC("open"), &RawSock::iopen);
        cls.Functor(_SC("close"), &RawSock::iclose);
        cls.Functor(_SC("puts"), &RawSock::puts);
        cls.Functor(_SC("gets"), &RawSock::gets);
        cls.Functor(_SC("write"), &RawSock::write);
        cls.Functor(_SC("read"), &RawSock::read);
        cls.Overload<void (Divais::*)(const char*)>(_SC("set_name"), &Divais::set_name);
        cls.Functor(_SC("get_name"), &RawSock::get_label_name);
        cls.Functor(_SC("set_tout"), &RawSock::_set_touts);
        cls.Functor(_SC("set_buff_size"), &RawSock::_set_buffers);

        Sqrat::RootTable().Bind(_SC("SOCKET"), cls);
    }
   
protected:
    bool                _write_now(const devdata_t& vl);
    size_t              _fecth(devdata_t& vl, const char* filter);
    bool                _set_values(const char* key, const char* value);
    const char*         _get_values(const char* key);
};

#endif // RAWSOCK_H
