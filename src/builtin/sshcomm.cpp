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

#ifdef WITH_SSH
#include "sshcomm.h"


SshComm::SshComm(const char* cred,
                 const char* name):ComSsh(cred),
    Divais(name),
    Reg<SshComm>(this),
    RtxBus<SshComm>(this),_bytes(nullptr),_nbytes(0)
{
    _o.BindCppObject(this);

}

SshComm::SshComm(SqObj& o,
                 const char* cred,
                 const char* name):ComSsh(cred),
                                    Divais(name),
                                    Reg<SshComm>(this),
                                    RtxBus<SshComm>(this),_bytes(nullptr),_nbytes(0)
{
    ctx_it(o, name);
}


SshComm::~SshComm()
{
    delete[] _bytes;
}

bool  SshComm::_write_now(const any_t& vl)
{
    _mon_dirt = true;
    return this->bwrite(vl[0].data(), vl[0].length());
}

size_t  SshComm::_read_now(any_t& vl, const char* filter)
{
    return this->bread(vl.c_bytes(), vl.length());
}


bool SshComm::_touch_it(size_t t)
{
    _cach = this->bread(_bytes, _nbytes);
    return _mon_dirt;
}

const char* SshComm::_gets(int chars)
{
    _mon_dirt = false;
    if(_cach)
    {
        _cach=false;
        return (const char*)_bytes;
    }
    return RtxBus<SshComm>::_gets(chars);
}


void SshComm::set_monitor(size_t bytes)
{
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
    _curdata=false;
}

void SshComm::on_event(E_VENT e, const uint8_t* buff, int len, int options)
{
}

bool	SshComm::_set_values(const char* key, const char* value)
{
    return false;
}

const char*	SshComm::_get_values(const char* key)
{
    return "";
}


#endif //ifdef WITH_SSH
