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

#ifndef _I2C_H_
#define _I2C_H_

#include "dcore.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#if defined (__linux__)
#   include <linux/i2c.h>
#   include <linux/i2c-dev.h>
#endif 
#if defined (__FreeBSD__)
#   include <dev/iicbus/iic.h>
#endif 

namespace GenericHw
{

typedef enum EI2C{}EI2C;

enum EI2CBUS            {
    I2C_0  = 0,
    I2C_1  = 1,
    I2C_2  = 2
};

class DvI2c : public DvCore, public IoOps
{

public:
    DvI2c(EI2CBUS i2c, uint8_t addr);
    DvI2c(const char* i2c, uint8_t addr);
    virtual ~DvI2c();
    virtual bool    iopen(int mode=O_RDWR);
    virtual void    iclose();
    virtual size_t  bread(uint8_t* buff, int len, int reg_addr=0);
    virtual int     bwrite(const uint8_t* buff, int len, int reg_addr=0);
    virtual int     fread(uint8_t* buff, int len) ;
    virtual int     fwrite(const uint8_t* buff, int len);
    virtual int     do_ioctl(int ctl, int val);
    virtual int     do_ioctl(int ctl, uint8_t* buf, int expect);
    virtual void    flush();
    virtual const  std::string& sf()const{return _sys;}
    virtual EPERIPH peer_of()const{return eI2C;}
    virtual E_TYPE  data_of()const{return eBINARY;}
    virtual const char* err_desc()const{return _err.c_str();}
    static void config(char* s, char* f){
        if(s && *s)  DvI2c::_sys=s ;
        if(f && *f)  DvI2c::_fmt=f ;
    }
private:
    static std::string        _sys;
    static std::string        _fmt;
    uint8_t                 _addr = 0;
    int                     _ifile = 0;
};
}

#endif /*  */
