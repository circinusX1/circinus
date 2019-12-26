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

#ifdef WITH_USB
#include "usbdev.h"
#include "ctx.h"
#include "rtxbus.h"

UsbDev::UsbDev(E_TYPE  e,
               const char* dev,
               const char *name):DvUsb(dev),
                                  Divais(e,eUSB,name),
                                  Reg<UsbDev>(this),
                                  RtxBus<UsbDev>(this),_bytes(nullptr),_nbytes(0)
{
    _o.BindCppObject(this);

}

UsbDev::UsbDev( SqObj& o,
                E_TYPE  e,
                const char* dev,
                const char *name):DvUsb(dev),
                                  Divais(e,eUSB,name),
                                  Reg<UsbDev>(this),
                                  RtxBus<UsbDev>(this),_bytes(nullptr),_nbytes(0)
{
    ctx_it(o, name);

}


UsbDev::~UsbDev()
{
    delete[] _bytes;
}

bool  UsbDev::_write_now(const any_t& vl)
{
    _mon_dirt = true;
    return this->bwrite(vl[0].data(), vl[0].length());
}

size_t  AdcDev::_read_now(any_t& vl, const char* filter)
{
    return this->bread(vl.c_bytes(), vl.length());
}



SqArr UsbDev::enumerate()
{
    char    id[32];
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
        sprintf(id,"0x%X:0x%X",pd.idVendor,pd.idProduct);
        rar.SetValue(j, id);
    }
    return rar;
}


bool UsbDev::_touch_it(size_t t)
{
    if(this->bread(_bytes, _nbytes))
    {
        return _mon_dirt=true;
    }
    return false;
}


const char* UsbDev::_gets(int chars)
{
    _mon_dirt = false;
    if(_curdata)
    {
        _curdata=false;
        return (const char*)_bytes;
    }
    return RtxBus<UsbDev>::_gets(chars);
}

SqArr UsbDev::_read(int chars)
{
    _mon_dirt = false;
    if(_curdata)
    {
        _curdata=false;
        SqArr  rar(App->psqvm(), _nbytes);
        for(size_t i = 0 ; i < _nbytes; i++)
        {
            rar.SetValue(i, _bytes[i]);
        }
        return rar;
    }
    return RtxBus<UsbDev>::_read(chars);
}

void UsbDev::set_monitor(size_t bytes)
{
    if(bytes==0)
    {
        delete[] _bytes;
        _bytes = nullptr;
        _monitor = false;
    }
    else {
        _monitor = true;
        _bytes = new uint8_t[bytes];
        _nbytes = bytes;
    }
    _curdata = false;
}

void UsbDev::on_event(E_VENT e, const uint8_t* buff, int len, int options)
{
}

bool	UsbDev::_set_values(const char* key, const char* value)
{
    return bwrite((const uint8_t*)value, ::strlen(value));
}

const char*	UsbDev::_get_values(const char* key)
{
    return _curdata.to_string();
}


#endif //#ifdef WITH_USB