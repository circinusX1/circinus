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
                                    Divais(eSSH, name),
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

size_t  SshComm::_fecth(any_t& vl, const char* filter)
{
    return this->bread(vl.c_bytes(), vl.length());
}


bool SshComm::_mon_pick(size_t t)
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

void SshComm::set_monitor(SqMemb& mem, size_t bytes)
{
    _cach=false;
    if(bytes==0 || m.IsNull())
    {
        delete[] _bytes;
        _bytes = nullptr;
        _monitor = false;
        if(!_on_event.IsNull())
            _on_event.Release();

    }
    else {
        _monitor = true;
        _bytes = new uint8_t[bytes];
        _nbytes = bytes;
        _on_event=m;
    }
    return _monitor;
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
