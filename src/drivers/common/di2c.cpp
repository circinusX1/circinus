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
#include "di2c.h"
#include "dlconfig.h"
#include "logs.h"

namespace GenericHw
{
//const char* DvI2c::_i2c_fmt;
//std::string DvI2c::_sys_i2c;

DvI2c::DvI2c(EI2CBUS i2c, uint8_t addr):_addr(addr)
{
    _config("i2c",_sys);
    _config("i2c_fmt",_fmt);
    char sy[256];
	::sprintf(sy, _fmt.c_str(), _sys.c_str(), i2c);
	_dev_node = sy;
}

DvI2c::~DvI2c()
{
    iclose();
}

bool    DvI2c::iopen(int  mode)
{
	if(_ifile<=0)
		_ifile = ::open(_dev_node.c_str(), O_RDWR);
    return _ifile>0;
}

void    DvI2c::iclose()
{
	if(_ifile)
		::close(_ifile);
    _ifile=0;
}

void    DvI2c::flush()
{
}

int  DvI2c::fread(uint8_t* buff, int len)
{

	int rv = ::read(_ifile, buff, len);
	if(rv>0) on_event(eREAD, buff, len);
	return rv;
}

int  DvI2c::fwrite(const uint8_t* buff, int len)
{
	int rv = ::write(_ifile, buff, len);
	if(rv>0) on_event(eWRITE, buff, len);
	return rv;
}

#if defined (__linux__)
int     DvI2c::bwrite(const uint8_t* pb, int length, int reg_addr)
{
    if(reg_addr==-1) return this->fwrite(pb,length);
	int err = ::ioctl(_ifile, I2C_SLAVE, _addr);
    if(err)
    {
		LOGW("Error I2C_SLAVE i2c: "<<  _dev_node << ": " << int(_addr) << "[" << int(reg_addr) << "]"  << ", "<<  strerror(errno) );
        return 0;
    }

    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg  messages[1];
    uint8_t         buff[length + 1];

    buff[0] = reg_addr;
    for (uint16_t i = 0; i < length; i++)
        buff[i + 1] = pb[i];

    messages[0].addr = _addr;
    messages[0].flags = 0;
    messages[0].len = length + 1;
    messages[0].buf = buff;
    packets.msgs = messages;
    packets.nmsgs = 1;
	if (::ioctl(_ifile, I2C_RDWR, &packets) < 0)
    {
		LOGW("write: failed to I2C_RDWR i2c:" << _dev_node <<", errno"<<  errno );
        return 0;
    }
	on_event(eWRITE, pb, length, reg_addr);
    return length;
}

size_t     DvI2c::bread(uint8_t* pb, int length, int reg_addr)
{
    if(reg_addr==-1) return this->fread(pb,length);
	int err = ::ioctl(_ifile, I2C_SLAVE, _addr);
    if(err)
    {
        LOGW("Failed to I2C_SLAVE i2c:" <<  errno );
        return 0;
    }

    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
    messages[0].addr = _addr;
    messages[0].flags = 0;
    messages[0].len = 1;
    messages[0].buf = (uint8_t*)&reg_addr;

    //where to return data
    messages[1].addr = _addr;
    messages[1].flags = I2C_M_RD;///* | I2C_M_NOSTART*/;
    messages[1].len = length;
    messages[1].buf = pb;

    packets.msgs = messages;
    packets.nmsgs = 2;
	if (::ioctl(_ifile, I2C_RDWR, &packets) < 0)
    {
        LOGW("read: failed to I2C_RDWR i2c:" <<  errno );
        return 0;
    }
	on_event(eREAD, pb, length, reg_addr);
    return length;
}

int     DvI2c::do_ioctl(int ctl, int val)
{
	if (::ioctl(_ifile, ctl, val) != 0)
    {
        LOGW("read: failed to I2C_RDWR i2c:" <<  errno );
        return 0;
    }
	on_event(eIOCTL, 0, ctl, val);
    return 1;
}

int     DvI2c::do_ioctl(int ctl, uint8_t* buf, int expect)
{
	if (::ioctl(_ifile, ctl, buf) != 0)
    {
        LOGW("read: failed to I2C_RDWR i2c:" <<  errno );
        return 0;
    }
	on_event(eIOCTL, buf, ctl, expect);
    return expect;
}


#endif // __linux__

#if defined __FreeBSD__

bool DvI2c::bwrite(const uint8_t* pb, int length, int reg_addr)
{
    uint8_t local[length+1] = {0};

    if(length < sizeof(local)-1)
    {
        uint8_t addr = _addr << 1;
        uint8_t result[2] = {0,0};
        ::memcpy(int  DvI2c::dev_bread(uint8_t* buff, int len)const
        {
            return ::read(_ifile, buff, len);
        }

        int  DvI2c::dev_bwrite(const uint8_t* buff, int len)
        {
            return ::write(_ifile, buf, len);
        }local+1, pb, length);
        local[0]=(uint8_t)reg_addr;
        struct iic_msg msgs[3] = {{addr|IIC_M_WR, IIC_M_WR, length+1, local},
                                  {addr|IIC_M_RD, IIC_M_RD, 2, result}};
        struct iic_rdwr_data data = {msgs, 1};

        if (ioctl(file_i2c, I2CRDWR, &data) < 0)
        {
            perror("/dev/iic2 I2CRDWR writing");
            LOGW("write: failed to I2CRDWR i2c:" <<  errno );
			return false;
        }
		on_event(eWRITE, pe, length, reg_addr);
		return true;
    }
    return 0;
}

int  DvI2c::bread(uint8_t* pb, int length, int reg_addr)const
{
    uint8_t addr   = _addr << 1;
    uint8_t reg[1] = {reg_addr};

    struct iic_msg msgs[3] = {{addr|IIC_M_WR, IIC_M_WR, 1,     reg},
                             {addr|IIC_M_RD, IIC_M_RD, bytes, pb}};
    struct iic_rdwr_data regsel = {&msgs[0], 1};
    struct iic_rdwr_data data   = {&msgs[1], 1};

    if (ioctl(file, I2CRDWR, &regsel) < 0)
    {
        ::perror("/dev/iic2 I2CRDWR selecting register");
        LOGW("read: failed to I2C_RDWR i2c:" <<  errno );
        return 0;
    }
    if (ioctl(file, I2CRDWR, &data) < 0)
    {
        perror("/dev/iic2 I2CRDWR reading");
        LOGW("read: failed to I2C_RDWR i2c:" <<  errno );
        return 0;
    }
	on_event(eREAD, pe, length, reg_addr);
    return length;
}

#endif // __FeeeBSD__
}


