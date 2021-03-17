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

#include "i2cdev.h"
#include "inst.h"
#include "dlconfig.h"

bool I2CDev::_squed;

// sudo i2cdetect -y -r I2CNO
I2CDev::I2CDev(EI2CBUS i2c, uint8_t addr,
               const char* name):DvI2c(i2c,addr),
    Divais (eBINARY, eI2C, name),
    Reg<I2CDev>(this),
    RtxBus<I2CDev>(this,true),_monreg_addr(0)
{
    I2CDev::_squed ? _o.BindCppObject(this) : (void)(0);
}

I2CDev::I2CDev(SqObj& o,
               EI2CBUS i2c,
               uint8_t addr,
               const char* name):DvI2c(i2c,addr),
    Divais (eBINARY, eI2C, name),
    Reg<I2CDev>(this),
    RtxBus<I2CDev>(this,true),_monreg_addr(0)
{
    plug_it(o, name);
}

I2CDev::I2CDev(const char* i2cfile, uint8_t addr,
               const char* name):DvI2c(i2cfile,addr),
    Divais (eBINARY, eI2C, name),
    Reg<I2CDev>(this),
    RtxBus<I2CDev>(this,true),_monreg_addr(0)
{
    I2CDev::_squed ? _o.BindCppObject(this): (void)(0);
}

I2CDev::I2CDev(SqObj& o, const char* i2cfile,
               uint8_t addr,const char* name):DvI2c(i2cfile,addr),
    Divais (eBINARY, eI2C, name),
    Reg<I2CDev>(this),
    RtxBus<I2CDev>(this,true),_monreg_addr(0)
{
    plug_it(o, name);
}

I2CDev::~I2CDev()
{
    this->iclose();
}

bool  I2CDev::_write_now(const devdata_t& vl)
{
    AutoOC  oc(this,  _auto);
    int     reg = 0;
    if(vl.length(1))
        reg = vl.to_t<int>(1);
    return this->bwrite(vl.c_bytes(), vl.length(), reg);
}

size_t  I2CDev::_fecth(devdata_t& vl, const char* filter)
{
    return 0;
}

int I2CDev::setreg(uint8_t cmd)
{
    return this->bwrite(nullptr, 0, cmd);
}

bool I2CDev::set_cb(SqMemb& ch, int regaddr)
{
    _monreg_addr=regaddr;
    return this->Divais::set_cb(ch);
}

bool I2CDev::_mon_callback(time_t tnow)
{
    if(_monreg_addr != 0xFF)
    {
        const SqArr& rv =  I2CDev::_readreg(_monreg_addr,_bufsz);
        if(rv.Length())
            return this->_call_cb(rv);
    }
    const char* pvalues = this->_get_values(SALLDATA);
    return this->_call_cb(pvalues);
}

SqArr  I2CDev::_readreg(uint8_t reg, int bytes)
{
    return RtxBus<I2CDev>::_readreg(reg, bytes);
}


void I2CDev::on_event(E_VENT e, const uint8_t* buff, int len, int options)
{
    _mon_dirt = true;
    if(e==eREAD)
    {
        _cur_value.let(buff, (size_t)len, 0);
        _cur_value.let(options,1); //register
    }
}

// 3,0x55,0c55,0x56,0x55
/**
 * @brief I2CDev::_set_value
 * @param key   = REGISTER string rep aka '0x20',
 *                VALUE string as pair ox 2 HEX  aka "0x0005C2B7"
 * @param value
 * @return
 */
bool	I2CDev::_set_value(const char* key, const char* value)
{
    if(!Divais::_set_value(key, value))
    {
        int ival;
        char two[4]={0};
        _cur_value.clear();
        _cur_value.let(::atoi(key),1);
        for(int i=0;value[i];i+=2)
        {
            two[0] = value[i];
            two[1] = value[i+1];
            ::sscanf(two, "%0X2", &ival);
            this->_cur_value.pusht((uint8_t)ival);
        }
        return true;
    }
    return false;
}

const char*	I2CDev::_get_values(const char* key)
{
    return Divais::get_value(key);
}

