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

#ifdef WITH_SSH
#include "sshcomm.h"


SshComm::SshComm(const char* cred,
                 const char* name):ComSsh(cred),
    Divais(name),
    Reg<SshComm>(this),
    RtxBus<SshComm>(this)
{
    _o.BindCppObject(this);

}

SshComm::SshComm(SqObj& o,
                 const char* cred,
                 const char* name):ComSsh(cred),
                                    Divais(eSSH, name),
                                    Reg<SshComm>(this),
                                    RtxBus<SshComm>(this,false,true)
{
    plug_it(o, name);
    IoType_t::construct(&_uchars);
}


SshComm::~SshComm()
{
    IoType_t::destroy(&_uchars);
}

bool  SshComm::_write_now(const devdata_t& vl)
{
    _mon_dirt = true;
    return this->bwrite(vl[0].data(), vl[0].length());
}

size_t  SshComm::_fecth(devdata_t& vl, const char* filter)
{
    return this->bread(vl.c_bytes(), vl.length());
}


bool SshComm::_mon_callback(time_t tnow)
{
    _cach = this->bread(_uchars->buf(), _uchars->cap());
    return _mon_dirt;
}

const char* SshComm::_gets(int chars)
{
    _mon_dirt = false;
    return RtxBus<SshComm>::_gets(chars);
}

void SshComm::set_cb(SqMemb& mem)
{
    return this->Divais::set_cb(m);
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
