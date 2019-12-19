/*
Copyright (c) 2014-2016 Marius.C All rights reserved.

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

#include "spidev.h"
#include "inst.h"
#include "dlconfig.h"

SpiDev::SpiDev(ESPIBUS spi, uint8_t addr,
               const char* name):DvSpi(spi,addr),
                                 Divais (eBINARY, name),
                                 Reg<SpiDev>(this),
                                 RtxBus<SpiDev>(this)
{
    _o.BindCppObject(this);

}

SpiDev::SpiDev(SqObj& o,
               ESPIBUS spi,
               uint8_t addr,
               const char* name):DvSpi(spi,addr),
                                 Divais (eBINARY, name),
                                 Reg<SpiDev>(this),
                                 RtxBus<SpiDev>(this)
{
    ctx_it(o, name);
    
}


SpiDev::~SpiDev()
{
    delete[] _bytes;
}

bool  SpiDev::_write_now(const any_t& vl)
{
    _mon_dirt = true;
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  SpiDev::_read_now(any_t& vl, const char* filter)
{
    return 0;
}

SqArr  SpiDev::_readreg(int bytes)
{
    _mon_dirt = false;
    if(_cach)
    {
        SqArr  rar(App->psqvm(), _nbytes);
        for(size_t i = 0 ; i < _nbytes; i++)
        {
            rar.SetValue(i, _bytes[i]);
        }
        _cach = false;
        return rar;
    }
    return RtxBus<SpiDev>::_readreg(0,bytes);
}

bool SpiDev::_touch_it(size_t t)
{
    _cach = this->bread(_bytes, _nbytes, _regaddr);
    return _mon_dirt;
}

void SpiDev::set_monitor(int bytes)
{
    _cach=false;
    if(bytes==0)
    {
        delete[] _bytes;
        _bytes = nullptr;
        _monitor = false;
    }
    else {
        _monitor = true;
        _bytes = new uint8_t[bytes];
        _nbytes = bytes;
    }
}

void SpiDev::on_event(E_VENT e, const uint8_t* buff, int len, int options)
{
}


// 3,0x55,0c55,0x56,0x55
bool	SpiDev::_set_values(const char* key, const char* value)
{
    any_t      loco;
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
