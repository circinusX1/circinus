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
#ifndef SSHCOMM_H
#define SSHCOMM_H

#include "comssh.h"
#include "iplugbase.h"
#include "sqwrap.h"
#include "ctx.h"
#include "rtxbus.h"

using namespace GenericHw;

class SshComm:
        public ComSsh,
        public Divais,
        private Reg<SshComm>,
        public RtxBus<SshComm>// , public Divais
{
public:
    SshComm(const char* credentials, const char* name=nullptr);
    SshComm(SqObj&, const char* credentials, const char* name=nullptr);

    virtual ~SshComm();
    void set_monitor(size_t bytes);
    const char* _gets(int chars);
    OVERW(ComSsh,Divais);
    static void squit(SqEnvi& e){
        Sqrat::Class<SshComm> cls(e.theVM(), _SC("SSH"));
        cls.Ctor<const char*, const char* >();
        cls.Ctor<SqObj&, const char*, const char* >();

        cls.Functor(_SC("ctx_it"), &SshComm::ctx_it);
        cls.Functor(_SC("open"), &SshComm::iopen);
        cls.Functor(_SC("close"), &SshComm::iclose);
        cls.Functor(_SC("set_monitor"), &SshComm::set_monitor);
        cls.Functor(_SC("gets"), &SshComm::_gets);
        cls.Overload<int (SshComm::*)(const char*)>(_SC("puts"), &RtxBus<SshComm>::_puts);
        cls.Overload<void (SshComm::*)()>(_SC("flush"), &RtxBus<SshComm>::_devflush);
        cls.Overload<void (Divais::*)(const char*)>(_SC("set_name"), &Divais::set_name);
        Sqrat::RootTable().Bind(_SC("SSH"), cls);

    }
protected:
    bool  _write_now(const any_t& vl);
    size_t  _fecth(any_t& vl, const char* filter);
    bool                _set_values(const char* key, const char* value);
    const char*         _get_values(const char* key);

private:

    uint8_t* _bytes;
    size_t   _nbytes;
    bool     _curdata;
};

#endif // SSHCOMM_H
#endif // WITH_SSH
