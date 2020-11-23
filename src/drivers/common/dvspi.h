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

#ifndef _SPI_H_
#define _SPI_H_

#include "dcore.h"
#if defined (__linux__)
#   include <linux/spi/spidev.h>
#endif
#if defined (__FreeBSD__)
#   include <dev/iicbus/spidev.h>
#endif

namespace GenericHw
{

typedef enum ESPI{}ESPI;

enum ESPIBUS            {
    SPI_0  = 0,
    SPI_1  = 1,
    SPI_2  = 2
};

class DvSpi : public DvCore, public IoOps
{
protected:
    DvSpi(){}
public:
    DvSpi(const char* spisyss, uint8_t addr, uint8_t mode, uint8_t wordlen, uint32_t speed);
     ~DvSpi();
     bool    iopen(int  mode=O_RDWR);
     void    iclose();
     int     fread(uint8_t* buff, int len)const ;
     int     fwrite(const uint8_t* buff, int len);
     size_t     bread(uint8_t* buff, int len, int options=0);
     int     bwrite(const uint8_t* buff, int len, int options=0);
     int     do_ioctl(int ctl, int val);
     int     do_ioctl(int ctl, uint8_t* buf, int expect);
     void    flush();
     virtual EPERIPH peer_of()const{return eSPI;}
     virtual E_TYPE  data_of()const{return eBINARY;}
     const  std::string& sf()const{return _sys;}
     virtual const char* err_desc()const{return _err.c_str();}
     static void config(char* s, char* f=nullptr){
        if(s && *s)  DvSpi::_sys=s ;
        if(f && *f)  DvSpi::_fmt=f ;
    }
private:
    static std::string        _sys;
    static std::string        _fmt;
    int         _ifile=0;
    uint8_t     _mode=0;
    uint8_t     _word=0;
    uint32_t    _freq=0;
};
}

#endif /*  */
