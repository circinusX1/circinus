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

#ifndef I2CDEV_H
#define I2CDEV_H

#include "di2c.h"
#include "divais.h"
#include "sqwrap.h"
#include "inst.h"
#include "rtxbus.h"

using namespace GenericHw;

class I2CDev: public DvI2c,
              public Divais,
              private Reg<I2CDev>,
              public RtxBus<I2CDev>// , public Divais
{
public:
    I2CDev(EI2CBUS i2c, uint8_t addr,const char* name=nullptr);
    I2CDev(SqObj&, EI2CBUS i2c, uint8_t addr,const char* name=nullptr);
    virtual ~I2CDev();
    int     setreg(uint8_t cmd);
    void    call_back(SqMemb& m, int regsddr, int bytes);
    SqArr  _readreg(uint8_t reg, int bytes);
    OVERW(DvI2c,Divais);
    static void squit(SqEnvi& e){
        Sqrat::Class<I2CDev> cls(e.theVM(), _SC("I2C"));
        cls.Ctor<EI2CBUS, uint8_t, const char*>();
        cls.Ctor<SqObj&, EI2CBUS, uint8_t, const char*>();
        cls.Functor(_SC("ctx_it"), &I2CDev::ctx_it);
        cls.Functor(_SC("call_back"), &I2CDev::call_back);
        cls.Functor(_SC("open"), &I2CDev::iopen);
        cls.Functor(_SC("close"), &I2CDev::iclose);
        cls.Overload<void (I2CDev::*)(bool)>(_SC("autoopen"), &RtxBus<I2CDev>::_autoopen);
        cls.Overload<int (I2CDev::*)(SqArr&)>(_SC("write"), &RtxBus<I2CDev>::_fwrite);
        cls.Overload<SqArr (I2CDev::*)(int)>(_SC("read"), &RtxBus<I2CDev>::_fread);
        cls.Overload<SqArr& (I2CDev::*)(int,SqArr& a,int)>(_SC("ioctl"), &RtxBus<I2CDev>::_ioctl);
        cls.Functor(_SC("setreg"), &I2CDev::setreg);
        cls.Functor(_SC("writecmd"), &I2CDev::setreg);
        cls.Functor(_SC("ioread"), &I2CDev::_readreg);
        cls.Overload<int (I2CDev::*)(uint8_t, SqArr&)>(_SC("iowrite"), &RtxBus<I2CDev>::_writereg);
        cls.Overload<void (Divais::*)(const char*)>(_SC("set_name"), &Divais::set_name);
        Sqrat::RootTable().Bind(_SC("I2C"), cls);
    }

protected:
    bool                _write_now(const any_t& vl);
    size_t              _fecth(any_t& vl, const char* filter);
    bool                _set_values(const char* key, const char* value);
    const char*         _get_values(const char* key);

private:
    int                 _regaddr;
    uint8_t*            _bytes; /*max i2c bytes*/
    size_t              _nbytes;
    bool                _cach;
};

#endif // I2CDEV_H
