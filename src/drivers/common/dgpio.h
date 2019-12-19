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

#ifndef __GPIO_H_
#define __GPIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dcore.h"

namespace GenericHw
{

typedef enum EGPIO{}EGPIO;
typedef int EGPIO_PIN;
typedef enum E_ONOFF{eLERR=-1, eLOW=0,eHIGH=1}E_ONOFF;

class DvGpio : public DvCore, public IoOps
{
public:
    DvGpio(EGPIO_PIN pn, EPIN_DIR pd, int on=eLOW);
    virtual   ~DvGpio();
    virtual bool    iopen(int  mode=O_RDWR);
    virtual void    iclose();
    virtual void    flush();
    virtual size_t  bread(uint8_t* buff, int len, int options=0);
    virtual int     bwrite(const uint8_t* buff, int len, int options=0);
    virtual const   std::string& sf()const{return _sys;}
    virtual EPERIPH peer_of()const{return eGPIO;};
    virtual E_TYPE  data_of()const{return eINT;}
    virtual const char* err_desc()const{return _err.c_str();}
    static void     config(char* s, char* f){
        if(s && *s)  DvGpio::_sys=s ;
        if(f && *f)  DvGpio::_fmt=f ;
    }
private:
    bool _exPin();
    bool _dirIt();

private:
    static std::string        _sys;
    static std::string        _fmt;
    EGPIO_PIN                _pin;
    EPIN_DIR                 _dir;
};

inline const uint8_t* tos(int t)
{
    static uint8_t s[16];
    sprintf((char*)s,"%d",t);
    return s;
}

};

#endif /* __GPIO_H_ */
