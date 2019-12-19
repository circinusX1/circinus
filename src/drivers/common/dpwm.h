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

#ifndef __PWM_H_
#define __PWM_H_

#include "dcore.h"

#include <iostream>
#include <fstream>
#include <cmath>

#define SET_DUTY(p,d)   (p * ( (double)d / 100.0) )
#define GET_DUTY(p,d)   (((double)d / (double)p) * 100.0)

namespace GenericHw
{
typedef enum EPWM{}EPWM;
typedef int EPWM_VAL;
typedef const char*   EPWM_PIN;
typedef int EPWM_PERIOD;


class DvPwm : public DvCore, public IoOps
{
public:
    DvPwm(){}
    DvPwm(EPWM_PIN pin, EPWM_PERIOD period, EPWM_VAL val, bool inv=false);
    virtual ~DvPwm();

    bool    iopen(int  mode=O_RDWR);
    void    iclose();
    size_t  bread(uint8_t* buff, int len, int options=0);
    int     bwrite(const uint8_t* buff, int len, int options=0);
    void    flush();
    virtual const  std::string& sf()const{return _sys;}
    virtual EPERIPH peer_of()const{return ePWMM;}
    virtual E_TYPE  data_of()const{return eINT;}
    virtual const char* err_desc()const{return _err.c_str();}
    static void config(char* s, char* f=nullptr, char* c=nullptr){
        if(s && *s)  DvPwm::_sys=s ;
        if(f && *f)  DvPwm::_fmt=f ;
        if(c)  DvPwm::_fmt_chip=f ;
    }
protected:
    int  _period()const{return _prd;};
private:
    int                       _prd;
    static std::string        _sys;
    static std::string        _fmt_chip;
    static std::string        _fmt;
};
}

#endif
