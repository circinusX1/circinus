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

// sudo i2cdetect -y -r I2CNO
I2CDev::I2CDev(EI2CBUS i2c, uint8_t addr,
               const char* name):DvI2c(i2c,addr),
    Divais (eBINARY, eI2C, name),
    Reg<I2CDev>(this),
    RtxBus<I2CDev>(this,true),_monreg_addr(0)
{
    _o.BindCppObject(this);
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

I2CDev::I2CDev(bool, const char* i2cfile, uint8_t addr,
               const char* name):DvI2c(i2cfile,addr),
    Divais (eBINARY, eI2C, name),
    Reg<I2CDev>(this),
    RtxBus<I2CDev>(this,true),_monreg_addr(0)
{
    _o.BindCppObject(this);
}

I2CDev::I2CDev(bool, SqObj& o, const char* i2cfile,
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

bool I2CDev::set_cb(SqMemb& ch, uint8_t regaddr)
{
    _monreg_addr=regaddr;
    return this->Divais::set_cb(ch);
}

bool I2CDev::_mon_callback(time_t tnow)
{
    const SqArr& rv =  I2CDev::_readreg(_monreg_addr,_bufsz);
    if(rv.Length())
        return this->_call_cb(rv);
    return false;
}

SqArr  I2CDev::_readreg(uint8_t reg, int bytes)
{
    _mon_dirt = false;
    return RtxBus<I2CDev>::_readreg(reg, bytes);
}


void I2CDev::on_event(E_VENT e, const uint8_t* buff, int len, int options)
{
    if(e==eREAD)
    {
        _curdata.let(buff, (size_t)len, 0);
        _curdata.let(options,1); //register
    }
}

// 3,0x55,0c55,0x56,0x55
bool	I2CDev::_set_values(const char* key, const char* value)
{
    int regi = ::atoi(key);
    devdata_t      loco;
    strarray_t bytes;
    _curdata.clear();
    CFL::explode(value,',',bytes);
    for(const auto& a:bytes)
    {
        int by;
        sscanf(a.c_str(),"%02X",&by);
        _curdata.pusht((uint8_t)by);
    }
    _curdata.let(regi,1);
    return true;
}

const char*	I2CDev::_get_values(const char* key)
{
    if(key[0]==ALLDATA)
    {
        char entry[4];
        std::string h;
        for(size_t t=0; t<_curdata.size();t++)
        {
            if(_curdata.length(t)==0)break;
            sprintf(entry,"%d=",(int)t);
            _forjson += t;
            _curdata.fmt_hex(h,t);
            _forjson += h;
            _forjson += "&";
        }
        return _forjson.c_str();
    }

    if(key[0]=='v')
    {
        _curdata.fmt_hex(_forjson);
        return _forjson.c_str();
    }
    if(key[0]=='r')
    {
        return _curdata.to_string<int>(1).c_str();
    }
    GETER_SYSCAT();
}

