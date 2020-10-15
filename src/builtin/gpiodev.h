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

#ifndef GPIODEV_H
#define GPIODEV_H

#include "dgpio.h"
#include "divais.h"
#include "sqwrap.h"
#include "inst.h"

using namespace GenericHw;



class GpioDev: public DvGpio, public Divais, private Reg<GpioDev>
{
public:
    GpioDev(EGPIO_PIN pn, EPIN_DIR pd, int on, const char* name);
    GpioDev(EGPIO_PIN pn, int freq, const char* name); // tone/ countet()<0
    GpioDev(SqObj&, EGPIO_PIN pn, EPIN_DIR pd, int on, const char* name);
    virtual ~GpioDev();
    int      set_value(int val);
    int      get_value();
    int      get_freq();
    int      set_freq(int);
    bool     call_back(SqMemb& ch, int risefall); /*-1/+1*/
    int      set_toggle();
    bool     set_invert(bool set_invert){return _reversed=set_invert;}

    OVERW(DvGpio,Divais)
    static void squit(SqEnvi& e){
        Sqrat::Class<GpioDev> cls(e.theVM(), _SC("PIO"));
        cls.Ctor<EGPIO_PIN, EPIN_DIR, int, const char*>();
        cls.Ctor<EGPIO_PIN, int, const char*>();        // counter or tone
        cls.Ctor<SqObj&, EGPIO_PIN, EPIN_DIR, int, const char*>();

        //cls.Ctor<EGPIO_PIN, EPIN_DIR, int>();
        cls.Functor(_SC("ctx_it"), &GpioDev::ctx_it);
        cls.Functor(_SC("set_value"), &GpioDev::set_value);
        cls.Functor(_SC("get_value"), &GpioDev::get_value);
        cls.Functor(_SC("get_freq"), &GpioDev::get_freq);
        cls.Functor(_SC("set_freq"), &GpioDev::set_freq);
        cls.Functor(_SC("call_back"), &GpioDev::call_back);
        cls.Functor(_SC("set_invert"), &GpioDev::set_invert);
        cls.Functor(_SC("set_toggle"), &GpioDev::set_toggle);
        cls.Overload<void (Divais::*)(const char*)>(_SC("set_name"), &Divais::set_name);
        Sqrat::RootTable().Bind(_SC("PIO"), cls);
    }

private:
    bool        _write_now(const any_t& vl);
    size_t      _fecth(any_t& vl, const char* filter);
    bool        _set_values(const char* key, const char* value);
    const char*	_get_values(const char* key);
private:
    char        _sret[8];
    size_t      _sec;
    EPIN_DIR    _dir;
    int         _freq;
    int         _counter;
    int         _curval;
    bool        _counting;
    size_t      _interval;
    bool        _reversed = false;
    int         _edging = 0;
};

#endif // GPIODEV_H
