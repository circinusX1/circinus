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

#ifndef SOLIB_H
#define SOLIB_H

#include "sqwrap.h"
#include "divais.h"
#include "dcore.h"
#include "inst.h"

using namespace GenericHw;

class SoLib: public Divais, private Reg<SoLib>
{
public:
    SoLib(const char* plugin);
    SoLib(SqObj&, const char* plugin);
    virtual ~SoLib();
    bool _mon_pick(size_t t);
    bool iopen(int);
    void  iclose();

    bool load(const char* foo, bool ret, int nargs);
    static void squit(SqEnvi& sq)
    {
        Sqrat::Class<SoLib> cls(sq.theVM(), _SC("LIB"));
        cls.Ctor<const char*>();
        cls.Ctor<SqObj&, const char*>();
        cls.Functor("load",&SoLib::load);
        Sqrat::RootTable().Bind(_SC("LIB"), cls);
    }
protected:
    bool  _write_now(const any_t& vl);
    size_t  _fecth(any_t& vl, const char* filter);

private:
    void*    _soh;
};

#endif // SQDEV_H
