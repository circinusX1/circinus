/*
Copyright (c) 2014-2016 PANGEEA.DPT All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/pangeea. The name of the
https://github.com/pangeea may not be used to endorse or promote
products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifdef WITH_SSH

#ifndef COMSSH_H
#define COMSSH_H

#include <libssh/libssh.h> //sudo apt-get install libssh-dev
#include <libssh/server.h>
#include "dcore.h"

using namespace GenericHw;
class ComSsh: public DvCore
{
public:
    // "user/pass@host:port"
    ComSsh(const char* );
    virtual ~ComSsh();
    virtual bool    iopen(int  mode=O_RDWR);
    virtual void    iclose();
    virtual void    flush();
    virtual size_t  bread(uint8_t* buff, int len, int options=0);
    virtual int     bwrite(const uint8_t* buff, int len, int options=0);

protected:
    int         _verify_knownhost();
    bool        _opentty();

protected:
    ssh_session      _ssh_session=nullptr;
    ssh_channel      _channel = nullptr;
    std::string      _prompt;
    std::string      _user;
    std::string      _pass;
    std::string      _host;
    int              _port;
};

#endif // COMSSH_H
#endif // WITH_CURL
