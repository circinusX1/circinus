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

#ifndef SDRBASE_H
#define SDRBASE_H

#include "sqwrap.h"
#include "divais.h"
#include "dcore.h"
#include "inst.h"

using namespace GenericHw;

class ScrBase: public Divais, private Reg<ScrBase>
{
public:
    ScrBase(SqObj&, E_TYPE e, const char* name=nullptr);
    virtual ~ScrBase();
    bool _mon_callback(time_t tnow);
    bool  iopen(int rm=0);
    void  iclose();
    static void squit(SqEnvi& sq)
    {
        Sqrat::Class<ScrBase> cls(sq.theVM(), _SC("BASE"));
        cls.Ctor<SqObj&, E_TYPE, const char*>();
        cls.Overload<void (Divais::*)(const char*)>(_SC("set_name"), &Divais::set_name);
        Sqrat::RootTable().Bind(_SC("BASE"), cls);
    }
protected:
    bool  _write_now(const devdata_t& vl);
    size_t  _fecth(devdata_t& vl, const char* filter);

private:
    devdata_t   _dummy;
};

#endif // SQDEV_H
