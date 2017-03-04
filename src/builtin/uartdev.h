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

#ifndef UARXDEV_H
#define UARXDEV_H

#include "dserial.h"
#include "divais.h"
#include "sqwrap.h"
#include "rtxbus.h"

using namespace GenericHw;

class UartDev : public DvSerial,
                public Divais,
                private Reg<UartDev>,
                public RtxBus<UartDev>// , public Divais
{
public:
    UartDev(E_TYPE  e, const char* dev, int bps, const char* mode, const char* name=nullptr);
    UartDev(SqObj&, E_TYPE  e, const char* dev, int bps, const char* mode, const char* name=nullptr);
    virtual ~UartDev();
    OVERW(DvSerial,Divais)
    void set_monitor(size_t bytes);
    const char* _gets(int chars);
    SqArr _read(int chars);

    static void squit(SqEnvi& e)
    {
        Sqrat::Class<UartDev> cls(e.theVM(), _SC("UART"));
        cls.Ctor<E_TYPE, const char*,int, const char*,const char*>();
        cls.Ctor<SqObj&, E_TYPE, const char*,int, const char*, const char*>();
        cls.Functor(_SC("ctx_it"), &UartDev::ctx_it);

        cls.Functor(_SC("open"), &UartDev::iopen);
        cls.Functor(_SC("close"), &UartDev::iclose);
        cls.Functor(_SC("set_monitor"), &UartDev::set_monitor);
        cls.Overload<int (UartDev::*)(SqArr&)>(_SC("setcr"), &RtxBus<UartDev>::_setcr);
        cls.Overload<int (UartDev::*)(const char*)>(_SC("puts"), &RtxBus<UartDev>::_puts);
        cls.Overload<bool (UartDev::*)(const char*, SqMemb&)>(_SC("puts_cb"), &RtxBus<UartDev>::_puts_cb);
        cls.Functor(_SC("gets"), &UartDev::_gets);
        cls.Functor(_SC("read"), &UartDev::_read);
        cls.Overload<int (UartDev::*)(SqArr&)>(_SC("write"), &RtxBus<UartDev>::_write);
        cls.Overload<bool (UartDev::*)(SqArr&, SqMemb&)>(_SC("write_cb"), &RtxBus<UartDev>::_write_cb);
        cls.Overload<bool (UartDev::*)(const char*, int)>(_SC("expect_str"), &RtxBus<UartDev>::_expect_str);
        cls.Overload<bool (UartDev::*)(SqArr&, int)>(_SC("expect_bin"), &RtxBus<UartDev>::_expect_bin);
        cls.Overload<const char* (UartDev::*)()>(_SC("pick_str"), &RtxBus<UartDev>::_pick_str);
        cls.Overload<SqArr (UartDev::*)()>(_SC("pick_bin"), &RtxBus<UartDev>::_pick_bin);
        cls.Overload<void (UartDev::*)()>(_SC("flush"), &RtxBus<UartDev>::_devflush);
        cls.Overload<void (Divais::*)(const char*)>(_SC("set_name"), &Divais::set_name);
        Sqrat::RootTable().Bind(_SC("UART"), cls);
    }
   
protected:
    bool  _write_now(const any_t& vl);
    size_t  _read_now(any_t& vl, const char* filter);
    bool                _set_values(const char* key, const char* value);
    const char*         _get_values(const char* key);

private:
    uint8_t* _bytes;
    size_t   _nbytes;
    bool     _bcurdata;
};

#endif // UARUartDevDEV_H
