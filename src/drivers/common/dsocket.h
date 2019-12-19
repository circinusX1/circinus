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

#ifndef _DVSOCK_H_
#define _DVSOCK_H_

#include "sock.h"
#include "dcore.h"


namespace GenericHw
{

class DvSocket : public DvCore, public tcp_clis, public IoOps
{
public:
    DvSocket(const char* ip, int port);
    virtual ~DvSocket();

    virtual bool    iopen(int  mode=O_RDWR);
    virtual void    iclose();
    virtual size_t  bread(uint8_t* buff, int len, int options=0);
    virtual int     bwrite(const uint8_t* buff, int len, int options=0);
    virtual void    flush();
    virtual const   std::string& sf()const{return _dev_node;}
    virtual EPERIPH peer_of()const{return eSOCKET;}
    virtual E_TYPE  data_of()const{return eBINARY;}
    virtual const char* err_desc()const{return _err.c_str();}
private:
    int           _port;
};

};


#endif /*  */
