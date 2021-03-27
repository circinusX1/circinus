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
#include "sq_engine.h"
#include "inst.h"

using namespace GenericHw;

union Uc{
    uint8_t* pu;
    void*    pv;
    char*    pc;
    size_t   sz;
    int      iv;
    short    sv;
    char     cv;
    unsigned int      uv;
    unsigned short    us;
    unsigned char     uc;
};


class Buff
{
public:
    Buff(Sqrat::Array& a);
    virtual ~Buff();
    void set(Sqrat::Array& a);
    pointer_t get();
    pointer_t deref();
    const char* str();
    pointer_t at(int offset);
    uint8_t* buffer(){return _pb;};
    int length(){return _isz;}
    void test();
     static void squit(SqEnvi& e){
        Sqrat::Class<Buff> cls(e.theVM(), _SC("BUFF"));
        cls.Ctor<Sqrat::Array>();
        cls.Functor(_SC("set"), &Buff::set);
        cls.Functor(_SC("get"), &Buff::get);
        cls.Functor(_SC("deref"), &Buff::deref);
        cls.Functor(_SC("str"), &Buff::str);
        cls.Functor(_SC("length"), &Buff::length);
        cls.Functor(_SC("test"), &Buff::test);
        Sqrat::RootTable().Bind(_SC("BUFF"), cls);
    }

private:
     void _clear();
private:     
     
    struct dlayout_t{
        int     type; 
        int     len; 
        int     off;  
        uint8_t* puser;
    }; 
    uint8_t*                    _pb = nullptr;
    Sqrat::Object               _o;
    std::vector<dlayout_t>      _layout;
    int _isz = 0;
};

#endif // GPIODEV_H
