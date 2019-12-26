/*
Copyright (c) 2014-2016 comarius.DPT All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/comarius. The name of the
https://github.com/comarius may not be used to endorse or promote
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
    bool _touch_it(size_t t);
    bool  iopen(int rm=0);
    void  iclose();
    static void squit(SqEnvi& sq)
    {
        Sqrat::Class<ScrBase> cls(sq.theVM(), _SC("BASE"));
        cls.Ctor<SqObj&, E_TYPE, const char*>();
        cls.Ctor<SqObj&, E_TYPE>();
        cls.Overload<void (Divais::*)(const char*)>(_SC("set_name"), &Divais::set_name);
        Sqrat::RootTable().Bind(_SC("BASE"), cls);
    }
protected:
    bool  _write_now(const any_t& vl);
    size_t  _read_now(any_t& vl, const char* filter);

private:
    any_t   _dummy;
};

#endif // SQDEV_H
