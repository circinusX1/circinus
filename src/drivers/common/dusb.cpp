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

#include "dusb.h"
#include "ctx.h"

namespace GenericHw
{
libusb_context          *_ctx;

DvUsb::DvUsb(const char* usbdev)
{
	_dev_node = usbdev;
    _devs=nullptr;
    _dev_handle=nullptr;
    _large = new uint8_t[USBBUFF];
    //name  P:V
    char loconame[64];
    ::strcpy(loconame, usbdev);
    _p= std::stoi((const char*)strtok(loconame,":"), 0, 16);
    _v = std::stoi((const char*)strtok(nullptr,":"), 0, 16);
}

DvUsb::~DvUsb()
{
    iclose();
    delete[] _large;
}

libusb_device** DvUsb::_enumerate(size_t& elems)
{
    elems = libusb_get_device_list(_ctx, &_devs);
    return _devs;
}

bool DvUsb::iopen(EMode)
{
    size_t t;

    if(_enumerate(t))
    {
        _dev_handle = libusb_open_device_with_vid_pid(_ctx, _p, _v);
        if(_dev_handle == NULL)
        {
            std::cout<<"Cannot open device"<<std::endl;
            libusb_free_device_list(_devs, 1);
            _devs=0;
            this->iclose();

            return false;
        }
        if(libusb_kernel_driver_active(_dev_handle, 0) == 1)
        { //find out if kernel driver is attached
            std::cout<<"Kernel Driver Active"<<std::endl;
            if(libusb_detach_kernel_driver(_dev_handle, 0) == 0) //detach it
                std::cout<<"Kernel Driver Detached!"<<std::endl;
        }
        int r = libusb_claim_interface(_dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
        if(r < 0) {
            std::cout<<"Cannot Claim Interface"<<std::endl;
            this->iclose();
            return false;
        }
    }
    return true;
}

void DvUsb::iclose()
{
    if(_devs)
        libusb_free_device_list(_devs, 1);
    _devs = 0;
    if(_dev_handle)
    {
        int r = libusb_release_interface(_dev_handle, 0); //release the claimed interface
        if(r!=0) {
            std::cout<<"Cannot Release Interface"<<std::endl;
        }else
            std::cout<<"Released Interface"<<std::endl;
        libusb_close(_dev_handle); //close the device we opened
    }

    _dev_handle=0;
}

int DvUsb::bwrite(const uint8_t* t, int l, int tout)
{
    if(_dev_handle==0)
        return 0;

    int actual = 0;
    if(tout==0)
        tout=128;
    libusb_bulk_transfer(_dev_handle,
                         (1|LIBUSB_ENDPOINT_OUT),
                         (unsigned char*)t, l, &actual, tout);
	if(actual>0) on_event(eWRITE, t, actual);
    return actual;
}

// int dev, uint8_t* pb, size_t nlen, size_t ms
int DvUsb::bread(uint8_t* pb, int room, int)const
{
    int ret = const_cast<DvUsb*>( this )->_fetch(1);
    if(ret==-1)
        return -1;
	int rv = const_cast<DvUsb*>( this )->_discard(pb, room);
	if(rv>0) on_event(eREAD, pb, rv);

}

int DvUsb::_discard(uint8_t * dest , size_t len)
{
    size_t tocopy = 0;
    if(_bytes>0)
    {
        tocopy = std::min(len,(size_t)_bytes);
        ::memcpy(dest, _large, tocopy);
        _bytes -= tocopy;
        if(_bytes)
            ::memmove(_large, _large+tocopy, _bytes);
    }
    return tocopy;
}

int DvUsb::_fetch(int dev)
{
    int chunk = 0;
    if(_bytes > USBBUFF - 1024)
    {
        std::cout << "====================buffer overflow =================\n";
        return -1;
    }
    int r = libusb_bulk_transfer(_dev_handle, (dev|LIBUSB_ENDPOINT_IN),
                                 (uint8_t*)_large+_bytes, USBBUFF-_bytes,
                                 &chunk, 256);
    if(r==0)
        _bytes += chunk;
    else{
        _bytes = 0;
        switch(r)
        {
            case LIBUSB_ERROR_TIMEOUT:
                return 0;
                break;
            case LIBUSB_ERROR_PIPE:
            case LIBUSB_ERROR_NO_DEVICE:
            default:
                this->iclose();
                std::cout << "errno: " << errno << std::endl;
                break;
        }
        return -1;
    }
    return r;
}

void DvUsb::flush()
{
    if(_dev_handle==0)
        return;
    int chunk;
    time_t tflush = ::time(0) + 4;

     do{
        chunk=0;
        libusb_bulk_transfer(_dev_handle, (1|LIBUSB_ENDPOINT_IN),
                                     (uint8_t*)_large, USBBUFF,
                                    &chunk, 1);
    }while (chunk || ::time(0) < tflush);
    _bytes=0;
}


}
#endif //#ifdef WITH_USB
