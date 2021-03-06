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

#define MAX_I2C_BYTES   4096

class I2CDev: public DvI2c,
              public Divais,
              private Reg<I2CDev>,
              public RtxBus<I2CDev>// , public Divais
{
public:
    explicit I2CDev(EI2CBUS i2c, uint8_t addr, const char* name=nullptr);
    explicit I2CDev(SqObj&, EI2CBUS i2c, uint8_t addr,const char* name=nullptr);
    // selector uses file
    explicit I2CDev(const char* i2cfile, uint8_t addr,const char* name=nullptr);
    explicit I2CDev(SqObj&, const char* i2cfile, uint8_t addr,const char* name=nullptr);
    virtual ~I2CDev();
    int     setreg(uint8_t cmd);
    SqArr  _readreg(uint8_t reg, int bytes);
    bool    set_cb(SqMemb& ch, int reg); /*-1/+1*/
    OVERW(DvI2c,Divais);
    static void squit(SqEnvi& e){
        Sqrat::Class<I2CDev> cls(e.theVM(), _SC("I2C"));
        cls.Ctor<EI2CBUS, uint8_t, const char*>();
        cls.Ctor<SqObj&, EI2CBUS, uint8_t, const char*>();

        cls.Ctor<const char*, uint8_t, const char*>();
        cls.Ctor<SqObj&, const char*, uint8_t, const char*>();


        cls.Functor(_SC("plug_it"), &I2CDev::plug_it);
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
        cls.Functor(_SC("get_name"), &I2CDev::get_label_name);
        cls.Functor(_SC("set_cb"), &I2CDev::set_cb);

        Sqrat::RootTable().Bind(_SC("I2C"), cls);
        I2CDev::_squed = true;
    }

protected:
    bool                _write_now(const devdata_t& vl);
    size_t              _fecth(devdata_t& vl, const char* filter);
    bool                _set_value(const char* key, const char* value);
    const char*         _get_values(const char* key);

private:
    int                 _monreg_addr;

};

#endif // I2CDEV_H
