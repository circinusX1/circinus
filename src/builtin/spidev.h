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

#ifndef SPIDEV_H
#define SPIDEV_H

#include "dvspi.h"
#include "divais.h"
#include "sqwrap.h"
#include "inst.h"
#include "rtxbus.h"

using namespace GenericHw;

class SpiDev:   public DvSpi,
                public Divais,
                private Reg<SpiDev>,
                public RtxBus<SpiDev>// , public Divais
{
public:

    SpiDev(ESPIBUS spi, uint8_t addr, uint8_t mode, uint8_t wc, uint32_t freq, const char* name=nullptr);
    SpiDev(SqObj&, ESPIBUS spi, uint8_t addr, uint8_t mode, uint8_t wc, uint32_t freq, const char* name=nullptr);
    virtual ~SpiDev();
    void set_monitor(int bytes);
    SqArr  _readreg(int bytes);
    OVERW(DvSpi,Divais)
    static void squit(SqEnvi& e){
        Sqrat::Class<SpiDev> cls(e.theVM(), _SC("SPI"));
        cls.Ctor<ESPIBUS, uint8_t, uint8_t, uint8_t, uint32_t, const char*>();
        cls.Ctor<SqObj&,ESPIBUS, uint8_t, uint8_t , uint8_t, uint32_t, const char*>();

        cls.Functor(_SC("open"), &SpiDev::iopen);
        cls.Functor(_SC("close"), &SpiDev::iclose);

        cls.Functor(_SC("ctx_it"), &SpiDev::ctx_it);
        cls.Functor(_SC("set_monitor"), &SpiDev::set_monitor);
        cls.Overload<int (SpiDev::*)(SqArr&)>(_SC("write"), &RtxBus<SpiDev>::_fwrite);
        cls.Overload<SqArr (SpiDev::*)(int)>(_SC("read"), &RtxBus<SpiDev>::_fread);
        cls.Overload<SqArr& (SpiDev::*)(int,SqArr& a,int)>(_SC("ioctl"), &RtxBus<SpiDev>::_ioctl);
        cls.Functor(_SC("ioread"), &SpiDev::_readreg);
        cls.Overload<int (SpiDev::*)(SqArr&)>(_SC("iowrite"), &RtxBus<SpiDev>::_write);
        cls.Overload<void (Divais::*)(const char*)>(_SC("set_name"), &Divais::set_name);
        Sqrat::RootTable().Bind(_SC("SPI"), cls);
    }

protected:
    bool  _write_now(const any_t& vl);
    size_t  _fecth(any_t& vl, const char* filter);
    bool                _set_values(const char* key, const char* value);
    const char*         _get_values(const char* key);

private:
    int                 _regaddr;
    uint8_t*            _bytes; /*max i2c bytes*/
    size_t              _nbytes;
    bool                _cach;
};

#endif // SPIDEV_H
