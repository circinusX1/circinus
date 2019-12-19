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

#ifndef TTY_SERIAL_H
#define TTY_SERIAL_H

#include <termios.h>

#include "dcore.h"

namespace GenericHw
{

typedef enum EUART{}EUART;

class DvSerial : public DvCore, public IoOps
{
public:
    DvSerial(const char*, int, const char *);
    ~DvSerial();
    virtual bool    iopen(int  mode=O_RDWR);
    virtual void    iclose();
    virtual size_t     bread(uint8_t* buff, int len, int to=1024);
    virtual int     bwrite(const uint8_t* buff, int len, int to=0);
    virtual void    flush();
    virtual const  std::string& sf()const{return _dev_node;}
    virtual EPERIPH peer_of()const{return eUART;}
    virtual E_TYPE  data_of()const{return eBINARY;}
    virtual const char* err_desc()const{return _err.c_str();}
protected:
    int _com_setting(const char* cmd,int opt=-1);
    int _async_read(unsigned char *, int, int to=100, int sto=2);
    int _is_dcd_enabled();
    int _is_cts_enabled();
    int _is_dsr_enabled();
    void _enable_dtr();
    void _disable_dtr();
    void _enable_rts();
    void _disable_rts();

#if defined (__linux__) || defined(__FreeBSD__)
    struct termios _curTCSett;
    struct termios _oldTCSett;
    int            _port;
#else
    HANDLE         _port;
#endif
    int         _error;
    std::string _mode;
    int         _bouts=0;
};
};

#endif // SERIAL_H
