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

#ifndef BUFF_H
#define BUFF_H

#include "dgpio.h"
#include "divais.h"
#include "sqwrap.h"
#include "inst.h"

using namespace GenericHw;


class Buff
{
public:
    Buff(Sqrat::Array& a);
    virtual ~Buff();
    void set(Sqrat::Array& a);
    pointer_t get();
    uint8_t* buffer(){return _pb;};
    int length(){return _isz;}
    void test(isize_t pv);
     static void squit(SqEnvi& e){
        Sqrat::Class<Buff> cls(e.theVM(), _SC("BUFF"));
        cls.Ctor<Sqrat::Array>();
        cls.Functor(_SC("set"), &Buff::set);
        cls.Functor(_SC("get"), &Buff::get);
        cls.Functor(_SC("length"), &Buff::length);
        cls.Functor(_SC("test"), &Buff::test);
        Sqrat::RootTable().Bind(_SC("BUFF"), cls);
    }

private:
    uint8_t* _pb = nullptr;
    Sqrat::Object    _o;
    std::vector<int>  _offs;
    int _isz = 0;
};

#endif // GPIODEV_H
