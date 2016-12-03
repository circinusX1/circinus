
#ifdef __linux
#   include <unistd.h>
#   include <errno.h>
#else
#   include <windows.h>
#endif

#include "usb.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include "squirrel.h"
#include "sqrat.h"

HSQUIRRELVM VM;
extern libusb_context          *_ctx;
#define USBBUFF   32768

PUsb::PUsb(IDevice* pdev,  const char* constr, const char* name):_debug(0),
                                                                _fname(constr),
                                                                _open(false),
                                                                _name(name)

{
    _pdev = pdev;
    pdev->addDev(this);

    _devs=nullptr;
    _dev_handle=nullptr;
    _large = new uint8_t[USBBUFF];
    _bytes = 0;
    //name  P:V
    char loconame[64];
    ::strcpy(loconame, constr);
    _p= std::stoi((const char*)strtok(loconame,":"), 0, 16);
    _v = std::stoi((const char*)strtok(nullptr,":"), 0, 16);


}

PUsb::~PUsb()
{
    close();
    _pdev->remDev(this);
    delete[] _large;
    _large = 0;
}

const char* PUsb::name()const
{
    return _name.c_str();
}

int PUsb::reopen(const char* cs)
{
    return 0;
}

size_t PUsb::_enumerate(size_t& elems)
{
    elems = libusb_get_device_list(_ctx, &_devs);
    return elems;
}


int PUsb::open()
{
    size_t t;
    _bytes = 0;

    if(_enumerate(t))
    {
        _dev_handle = libusb_open_device_with_vid_pid(_ctx, _p, _v);
        if(_dev_handle == NULL)
        {
            std::cout<<"Cannot open device"<<std::endl;
            libusb_free_device_list(_devs, 1);
            _devs=0;
            this->close();

            return errno;
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
            this->close();
            return errno;
        }
    }

    _open=true;
    return 0;
}

int PUsb::close()
{
    _open=false;
    _bytes = 0;
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
    return 0;
}

void PUsb::setspeed(int ms)
{
}


int PUsb::sendTo(const uint8_t* t, int l)
{
    if(_dev_handle==0)
        return 0;

    int actual = 0;
    libusb_bulk_transfer(_dev_handle,
                         (1|LIBUSB_ENDPOINT_OUT),
                         (unsigned char*)t, l, &actual, _tout);
    return actual;

}

// int dev, uint8_t* pb, size_t nlen, size_t ms
int PUsb::recFrom(unsigned char* pb, int room)
{
    int ret = 0;
    if(_bytes < room){
        ret = _fetch(1);
    }
    if(ret==-1)
        return -1;
    return _discard(pb, room);
}

int PUsb::_discard(uint8_t * dest , size_t len)
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

int PUsb::_fetch(int dev)
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
                this->close();
                std::cout << "errno: " << errno << std::endl;
                break;
        }
        return -1;
    }
    return r;
}


void PUsb::debug(int d)
{
    _debug = d;
}

void PUsb::setTout(int to, int selto)
{
    _tout = to;
}

bool PUsb::isOpen()
{
    return _open;
}

void PUsb::flush()
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



int PUsb::setOptions(const char*)
{
    return 0;
}

const char* PUsb::getOptions(const char*)
{
    return "";
}

IDevice::D_MSG PUsb::getMode()const
{
    return IDevice::eTEXT;
}

int PUsb::esc(const char*,int)
{
    return -1;
}

int PUsb::removeFile()
{
    return 0;
}

int PUsb::createFile()
{
    return 0;
}

void PUsb::testin(const char* param)
{
    std::cout << param << "\n";
}

void PUsb::publish()
{
    std::cout << "publishing \n";

    Sqrat::Class<PUsb> cls(VM, _SC("file"));
    cls.Func(_SC("textin"), &PUsb::testin);

    Sqrat::RootTable().Bind(_SC("file"), cls);
}

/**
 *
 */
extern "C"
{
    EXPORT_CLS  IComm* getInstance(IDevice* pd, const char* cs,
                                   const char* name, HSQUIRRELVM vm)
    {
        VM = vm;
        int r = libusb_init(&_ctx);
        if(r < 0)
        {
            std::cout<<"Init Error "<<r<<std::endl;
            return 0;
        }
        libusb_set_debug(_ctx, 3);        return new PUsb(pd, cs, name);
    }

    EXPORT_CLS  void destroyInstance(IComm* pc)
    {
        if(_ctx)
            libusb_exit(_ctx); //needs to be called to end the
        delete pc;
    }
}

