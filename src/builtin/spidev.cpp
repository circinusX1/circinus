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

#include "spidev.h"
#include "inst.h"
#include "dlconfig.h"

SpiDev::SpiDev(const char* spi, uint8_t addr, uint8_t mode, uint8_t wc, uint32_t freq,
               const char* name):DvSpi(spi,addr,mode,wc,freq),
                                 Divais (eBINARY, eSPI, name),
                                 Reg<SpiDev>(this),
                                 RtxBus<SpiDev>(this,false,true)
{
    _o.BindCppObject(this);

}

SpiDev::SpiDev(SqObj& o,
               const char* spi,
               uint8_t addr, uint8_t mode, uint8_t wc,  uint32_t freq,
               const char* name):DvSpi(spi,addr,mode,wc,freq),
                                 Divais (eBINARY, eSPI, name),
                                 Reg<SpiDev>(this),
                                 RtxBus<SpiDev>(this,false,true)
{
    plug_it(o, name);
    
}


SpiDev::~SpiDev()
{

}

bool  SpiDev::_write_now(const devdata_t& vl)
{
    _mon_dirt = true;
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  SpiDev::_fecth(devdata_t& vl, const char* filter)
{
    return 0;
}

SqArr  SpiDev::_readreg(int bytes)
{
    _mon_dirt = false;
    return RtxBus<SpiDev>::_readreg(0,bytes);
}

bool SpiDev::set_cb(SqMemb& ch, int bytes)
{
    return this->Divais::set_cb(ch);
}

bool SpiDev::_mon_callback(time_t tnow)
{
    const SqArr& rv =  _readreg(_bufsz);
    return this->_call_cb(rv);
}

void SpiDev::on_event(E_VENT e, const uint8_t* buff, int len, int options)
{
}


// 3,0x55,0c55,0x56,0x55
bool	SpiDev::_set_values(const char* key, const char* value)
{
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
    return _curdata.length() > 0;
}

const char*	SpiDev::_get_values(const char* key)
{
    if(key[0]=='v') //address
        return _curdata.to_string<int>().c_str();
    GETER_SYSCAT();
}
