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

#ifndef ADCDEV_H
#define ADCDEV_H

#include "dadc.h"
#include "divais.h"
#include "sqwrap.h"
#include "inst.h"

using namespace GenericHw;

class AdcDev: public DvAdc, public Divais, private Reg<AdcDev>// , public Divais
{
public:
    explicit AdcDev(const char* pn,  const char* name=nullptr); // tone
    explicit AdcDev(SqObj&, const char* fname, const char* name=nullptr);
    virtual ~AdcDev();
    int      get_value();
    bool     call_back(SqMemb& ch);
    OVERW(AdcDev,Divais)
    static void squit(SqEnvi& e){
        Sqrat::Class<AdcDev> cls(e.theVM(), _SC("ADC"));
        cls.Ctor<const char*, const char*>();
        cls.Ctor<SqObj&, const char*, const char*>();

        cls.Functor(_SC("plug_it"), &AdcDev::plug_it);
        cls.Functor(_SC("get_value"), &AdcDev::get_value);
        cls.Functor(_SC("call_back"), &AdcDev::call_back);
        cls.Overload<void (Divais::*)(const char*)>(_SC("set_name"), &Divais::set_name);
        Sqrat::RootTable().Bind(_SC("ADC"), cls);
    }
private:
    bool        _write_now(const any_t& vl);
    size_t      _fecth(any_t& vl, const char* filter);
    const char*	_get_values(const char* key);
    bool        _set_values(const char* key, const char* value);

};

#endif // GPIODEV_H
