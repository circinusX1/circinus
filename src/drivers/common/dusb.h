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

#ifndef _USB_H_
#define _USB_H_

#include "dcore.h"
#include <libusb-1.0/libusb.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#endif


namespace GenericHw
{
extern libusb_context  *_ctx;
#define USBBUFF 16384
typedef enum EUSB{}EUSB;

class DvUsb : public DvCore, public IoOps
{
public:
    DvUsb(const char* device);
    virtual ~DvUsb();

    virtual bool    iopen(int  mode=O_RDWR);
    virtual void    iclose();
    virtual size_t  bread(uint8_t* buff, int len, int options=0);
    virtual int     bwrite(const uint8_t* buff, int len, int options=0);
    virtual void    flush();
    virtual EPERIPH peer_of()const{return eUSB;}
    virtual E_TYPE  data_of()const{return eBINARY;}
    virtual const char* err_desc()const{return _err.c_str();}
protected:
     libusb_device** _enumerate(size_t& elems);
     int _discard(uint8_t * dest , size_t len);
     int _fetch(int dev);

protected:
     std::string            _fname;
     int                    _tout = 32;
     libusb_device           **_devs;
     libusb_device_handle    *_dev_handle;
     uint8_t*                _large;
     int                     _v = 0;
     int                     _p = 0;
     int                     _bytes=0;
};

}

#endif /*  */
#endif //#ifdef WITH_USB
