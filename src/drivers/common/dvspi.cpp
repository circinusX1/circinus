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



#include "main.h"
#include "dvspi.h"
#include "dlconfig.h"
#include "logs.h"

namespace GenericHw
{

DvSpi::DvSpi(ESPIBUS i2c, uint8_t addr):_ifile(0)
{
}

DvSpi::~DvSpi()
{
}

bool    DvSpi::iopen(int)
{
    return false;
}

void    DvSpi::iclose()
{
    ;
}

size_t     DvSpi::bread(uint8_t* buff, int len, int)
{
	on_event(eREAD, buff, len);
    return 0;
}

int     DvSpi::bwrite(const uint8_t* buff, int len, int)
{
	on_event(eWRITE, buff, len);

    return 0;
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
    return ::read(_ifile, buff, len);
}

int  DvSpi::fwrite(const uint8_t* buff, int len)
{
    return ::write(_ifile, buff, len);
}


}
