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

#ifndef __ADC_H_
#define __ADC_H_

#include <stdint.h>
#include "dcore.h"

#define __noop (void)0

namespace GenericHw
{

typedef enum EADC{}EADC;
enum EADC_PIN {
    AIN0 = 0,
    AIN1 = 1,
    AIN2 = 2,
    AIN3 = 3,
    AIN4 = 4,
    AIN5 = 5,
    AIN6 = 6
};

typedef enum E_ADCERR{eADLERR=-999999999}E_ADCERR;

class DvAdc : public DvCore, public IoOps
{
public:
    DvAdc(EADC_PIN adc);
    virtual ~DvAdc();
    virtual bool    iopen(int  mode=O_RDWR);
    virtual void    iclose();
    virtual size_t  bread(uint8_t* buff, int len, int options=0);
    virtual int     bwrite(const uint8_t* buff, int len, int options=0);
    virtual void    flush();
    virtual EPERIPH peer_of()const{return eADC;};
    virtual E_TYPE  data_of()const{return eINT;}
    virtual const char* err_desc()const{return _err.c_str();}
    virtual const  std::string& sf()const{return _sys;}
    static void config(char* s, char* f){
		if(s && *s)  DvAdc::_sys=s ;
		if(f && *f)  DvAdc::_fmt=f ;
	}
private:
    static std::string        _fmt;
    static std::string        _sys;
};
}

#endif /* __ADC_H_ */
