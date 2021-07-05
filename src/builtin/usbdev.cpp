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
#include "usbdev.h"
#include "inst.h"
#include "rtxbus.h"
#include "divais.h"

bool UsbDev::_squed;


UsbDev::UsbDev(E_TYPE  e,
               const char* dev,
               const char *name):DvUsb(dev),
                                  Divais(e,eUSB,name),
                                  Reg<UsbDev>(this),
                                  RtxBus<UsbDev>(this,false,true)
{
    UsbDev::_squed ? _o.BindCppObject(this) : (void)(0);
}

UsbDev::UsbDev( SqObj& o,
                E_TYPE  e,
                const char* dev,
                const char *name):DvUsb(dev),
                                  Divais(e,eUSB,name),
                                  Reg<UsbDev>(this),
                                  RtxBus<UsbDev>(this,false,true)
{
    plug_it(o, name);

}

UsbDev::~UsbDev()
{
}

bool  UsbDev::_write_now(const devdata_t& vl)
{
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  UsbDev::_fecth(devdata_t& vl, const char* filter)
{
    return 0;
}

SqArr UsbDev::enumerate()
{
    char    id[128];
    size_t  usbs = 0;

    _enumerate(usbs);
    SqArr rar(App->psqvm(),0);

    for(size_t j=0;j<usbs;j++)
    {
        rar.Resize(usbs);
        libusb_device_descriptor pd;
        memset(&pd,0,sizeof(pd));
        libusb_device *device = _devs[j];
        libusb_get_device_descriptor(device, &pd);

        sprintf(id,"0x%X:0x%X %X/%X/%X",pd.idVendor,pd.idProduct,
                                    pd.bDeviceClass,
                                    pd.bDeviceSubClass,
                                    pd.bDeviceProtocol);
        rar.SetValue(j, id);
    }
    return rar;
}

bool UsbDev::_mon_callback(time_t tnow)
{
    if(_etype==eSTRING){
        const char* rv = RtxBus<UsbDev>::_gets();
        if(rv && *rv)
            return this->Divais::_call_cb(rv);
    }
    else{
        const SqArr& rv = RtxBus<UsbDev>::_read();
        if(rv.Length())
            return this->Divais::_call_cb(rv);
    }
    return false;
}

const char* UsbDev::_gets()
{
    return RtxBus<UsbDev>::_gets();
}

SqArr UsbDev::_read(int chars)
{
    return RtxBus<UsbDev>::_read();
}

bool UsbDev::set_cb(SqMemb& mem)
{
    return this->Divais::set_cb(mem);
}

void UsbDev::on_event(E_VENT e, const uint8_t* buff, int len, int options)
{
    _mon_dirt = true;
}

bool	UsbDev::_set_value(const char* key, const char* value)
{
    return bwrite((const uint8_t*)value, ::strlen(value));
}

const char*	UsbDev::_get_values(const char* key)
{
    if(key[0]==ALLDATA)
    {
        _retparams += "&value=";
        _retparams += _cur_value.c_chars();
        return _retparams.c_str();
    }
    GETER_SYSCAT(); return Divais::_get_values(key);
}


#endif //#ifdef WITH_USB
