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

#ifndef PWMDEV_H
#define PWMDEV_H

#include "dpwm.h"
#include "divais.h"
#include "sqwrap.h"
#include "inst.h"

using namespace GenericHw;

class PwmDev: public DvPwm, public Divais, private Reg<PwmDev>// , public Divais
{
public:
    PwmDev(EPWM_PIN pin, int period, int val, bool inv, const char* name=nullptr);
    PwmDev(SqObj&, EPWM_PIN pin, int period, int val, bool inv, const char* name=nullptr);
    virtual ~PwmDev();
    int         set_value(EPWM_VAL val);
    EPWM_VAL    get_value();
    bool        call_back(SqMemb& mem);
    bool        set_invert(bool set_invert){return _reversed=set_invert;}
    OVERW(DvPwm,Divais)
    static void squit(SqEnvi& e){
        Sqrat::Class<PwmDev> cls(e.theVM(), _SC("PWM"));
        cls.Ctor<EPWM_PIN, int, int, bool,const char*>();
        cls.Ctor<SqObj&, EPWM_PIN, int, int, bool,const char*>();

        cls.Functor(_SC("regiter_it"), &PwmDev::regiter_it);
        cls.Functor(_SC("set_value"), &PwmDev::set_value);
        cls.Functor(_SC("get_value"), &PwmDev::get_value);
        cls.Functor(_SC("call_back"), &PwmDev::call_back);
        cls.Functor(_SC("set_invert"), &PwmDev::set_invert);
        cls.Overload<void (Divais::*)(const char*)>(_SC("set_name"), &Divais::set_name);
        Sqrat::RootTable().Bind(_SC("PWM"), cls);
    }
   
protected:
    bool  _write_now(const any_t& vl);
    size_t _fecth(any_t& vl, const char* filter);
    bool                _set_values(const char* key, const char* value);
    const char*         _get_values(const char* key);

private:
    bool  _reversed = false;
};

#endif // PWMDEV_H
