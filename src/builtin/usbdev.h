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

#ifdef WITH_USB
#ifndef USBDEV_H
#define USBDEV_H

#include "dusb.h"
#include "divais.h"
#include "sq_engine.h"
#include "rtxbus.h"


using namespace GenericHw;

#define USB_MAX_BYTES  4096

class UsbDev :  public DvUsb,
                public Divais,
                private Reg<UsbDev>,
                public RtxBus<UsbDev>
{
public:
    UsbDev(E_TYPE  e, const char* dev, const char* name);
    UsbDev(SqObj&, E_TYPE  e, const char* dev,const char* name);
    virtual ~UsbDev();
    SqArr enumerate();
    bool  set_cb(SqMemb& mem);
    const char* _gets();
    SqArr _read(int chars);
    OVERW(DvUsb,Divais)
    static void squit(SqEnvi& e){
        Sqrat::Class<UsbDev> cls(e.theVM(), _SC("USB"));
        cls.Ctor<E_TYPE,const char*, const char*>();
        cls.Ctor<SqObj&, E_TYPE, const char*, const char*>();

        cls.Functor(_SC("plug_it"), &UsbDev::plug_it);
        cls.Functor(_SC("open"), &DvUsb::iopen);
        cls.Functor(_SC("close"), &DvUsb::iclose);
        cls.Functor(_SC("enumerate"), &UsbDev::enumerate);
        cls.Functor(_SC("set_cb"), &UsbDev::set_cb);
        cls.Overload<int (UsbDev::*)(SqArr&)>(_SC("setcr"), &RtxBus<UsbDev>::_setcr);
        cls.Overload<int (UsbDev::*)(const char*)>(_SC("puts"), &RtxBus<UsbDev>::_puts);
        cls.Overload<bool (UsbDev::*)(const char*, SqMemb&)>(_SC("puts_cb"), &RtxBus<UsbDev>::_puts_cb);
        cls.Functor(_SC("gets"), &RtxBus<UsbDev>::_gets);
        cls.Functor(_SC("read"), &RtxBus<UsbDev>::_read);
        cls.Overload<int (UsbDev::*)(SqArr&)>(_SC("write"), &RtxBus<UsbDev>::_write);
        cls.Overload<bool (UsbDev::*)(SqArr&, SqMemb&)>(_SC("write_cb"), &RtxBus<UsbDev>::_write_cb);
        cls.Overload<bool (UsbDev::*)(const char*)>(_SC("expect_str"), &RtxBus<UsbDev>::_expect_str);
        cls.Overload<bool (UsbDev::*)(SqArr&)>(_SC("expect_bin"), &RtxBus<UsbDev>::_expect_bin);
        cls.Overload<const char* (UsbDev::*)()>(_SC("pick_str"), &RtxBus<UsbDev>::_pick_str);
        cls.Overload<SqArr (UsbDev::*)()>(_SC("pick_bin"), &RtxBus<UsbDev>::_pick_bin);
        cls.Overload<void (UsbDev::*)()>(_SC("flush"), &RtxBus<UsbDev>::_devflush);
        cls.Overload<void (Divais::*)(const char*)>(_SC("set_name"), &Divais::set_name);
        cls.Functor(_SC("get_name"), &UsbDev::get_label_name);
        cls.Overload<void (UsbDev::*)(size_t)>(_SC("set_tout"), &RtxBus<UsbDev>::_set_touts);
        cls.Overload<void (UsbDev::*)(size_t,size_t)>(_SC("set_buff_size"), &RtxBus<UsbDev>::set_buff_size);
        Sqrat::RootTable().Bind(_SC("USB"), cls);
        UsbDev::_squed = true;
    }
protected:
    bool  _write_now(const devdata_t& vl);
    size_t  _fecth(devdata_t& vl, const char* filter);
    bool                _set_value(const char* key, const char* value);
    const char*         _get_values(const char* key);

private:

};

#endif // UARTDEV_H
#endif //#ifdef WITH_USB
