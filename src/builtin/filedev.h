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

#ifndef FILEDEV_H
#define FILEDEV_H

#include "dfile.h"
#include "divais.h"
#include "sqwrap.h"
#include "inst.h"
#include "rtxbus.h"

using namespace GenericHw;

class FileDev:
        public DvFile,
        public Divais,
        private Reg<FileDev>,
        public  RtxBus<FileDev>// , public Divais
{
public:
    FileDev(E_TYPE e, const char* , const char* name=nullptr);
    FileDev(SqObj&, E_TYPE e, const char* , const char* name=nullptr);
    virtual ~FileDev();
    int call_back(SqMemb& mon);
    OVERW(DvFile,Divais)
    static void squit(SqEnvi& e){
        Sqrat::Class<FileDev> cls(e.theVM(), _SC("FILE"));
        cls.Ctor<E_TYPE,const char*, const char* >();
        cls.Ctor<SqObj&, E_TYPE, const char*, const char* >();


        cls.Functor(_SC("ctx_it"), &FileDev::ctx_it);
        cls.Functor(_SC("open"), &FileDev::iopen);
        cls.Functor(_SC("close"), &FileDev::iclose);
        cls.Functor(_SC("call_back"), &FileDev::call_back);
        cls.Overload<const char* (FileDev::*)(int)>(_SC("gets"), &RtxBus<FileDev>::_gets);
        cls.Overload<int (FileDev::*)(const char*)>(_SC("puts"), &RtxBus<FileDev>::_puts);
        cls.Overload<SqArr (FileDev::*)(int)>(_SC("read"), &RtxBus<FileDev>::_read);
        cls.Overload<int (FileDev::*)(SqArr&)>(_SC("write"), &RtxBus<FileDev>::_write);
        cls.Overload<void (FileDev::*)()>(_SC("flush"), &RtxBus<FileDev>::_devflush);
        cls.Overload<void (Divais::*)(const char*)>(_SC("set_name"), &Divais::set_name);
        Sqrat::RootTable().Bind(_SC("FILE"), cls);
    }
protected:
    const char*	_get_values(const char* key);
    bool	_set_values(const char* key, const char* value);
private:
    bool  _write_now(const any_t& vl);
    size_t  _fecth(any_t& vl, const char* filter);

};

#endif // FILEDEV_H
