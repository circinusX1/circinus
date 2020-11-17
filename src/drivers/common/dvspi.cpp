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


#include <cstdint>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "main.h"
#include "dvspi.h"
#include "dlconfig.h"
#include "logs.h"

namespace GenericHw
{

DvSpi::DvSpi(const char* spisyss,
             uint8_t addr,
             uint8_t mode,
             uint8_t wordlen,
             uint32_t speed):_ifile(0),_mode(mode),_word(wordlen),_freq(speed)
{

    _dev_node = spisyss;
}

DvSpi::~DvSpi()
{
}

bool    DvSpi::iopen(int)
{
    if(0==_ifile){
        _ifile = ::open(_dev_node.c_str(), O_RDWR);
        if(_ifile)
        {
            if(::ioctl(_ifile, SPI_IOC_WR_MODE, &_mode) == -1 )
            {
                iclose();
                return false;
            }
            if(::ioctl(_ifile, SPI_IOC_WR_MAX_SPEED_HZ, &_freq) == -1 )
            {
                iclose();
                return false;
            }
            if(::ioctl(_ifile, SPI_IOC_WR_BITS_PER_WORD, &_word) == -1 )
            {
                return false;
            }
        }
    }
    return _ifile>0;
}

void    DvSpi::iclose()
{
    if(_ifile)
        ::close(_ifile);
    _ifile=0;
}

size_t     DvSpi::bread(uint8_t* buff, int len, int waitus)
{
    spi_ioc_transfer package;

    package.tx_buf          = (unsigned long)0;
    package.rx_buf          = (unsigned long)buff;
    package.len             = len;
    package.delay_usecs     = waitus;
    package.speed_hz        = _freq;
    package.bits_per_word   = _word;
    if( ::ioctl(_ifile, SPI_IOC_MESSAGE(1), &package)< 0)
    {
        return -1;
    }
    on_event(eREAD, buff, len);
    return 0;
}

int     DvSpi::bwrite(const uint8_t* buff, int len, int waitus)
{
    spi_ioc_transfer package;

    package.tx_buf          = (unsigned long)buff;
    package.rx_buf          = (unsigned long)nullptr;
    package.len             = len;
    package.delay_usecs     = waitus;
    package.speed_hz        = _freq;
    package.bits_per_word   = _word;
    if( ::ioctl(_ifile, SPI_IOC_MESSAGE(1), &package)< 0)
    {
        return -1;
    }
    on_event(eWRITE, buff, len);
    return len;
}

void    DvSpi::flush()
{
    ;
}

int     DvSpi::do_ioctl(int ctl, int val)
{
    if (ioctl(_ifile, ctl, val) != 0)
    {
        LOGW("read: failed to I2C_RDWR i2c:" <<  errno );
        return 0;
    }
    return 1;
}

int     DvSpi::do_ioctl(int ctl, uint8_t* buf, int expect)
{
    if (ioctl(_ifile, ctl, buf) != 0)
    {
        LOGW("read: failed to I2C_RDWR i2c:" <<  errno );
        return 0;
    }
    return expect;
}


int  DvSpi::fread(uint8_t* buff, int len)const
{
    return -1;
}

int  DvSpi::fwrite(const uint8_t* buff, int len)
{
    return -1;
}


}
