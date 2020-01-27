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




#include "main.h"
#include "dgpio.h"
#include "dlconfig.h"
/*
 * cat /sys/kernel/debug/pwm|gpio|i2c
 */

namespace GenericHw
{
//std::string     DvGpio::_sys_gpio = "/sys/class/gpio";

DvGpio::DvGpio(EGPIO_PIN pin, EPIN_DIR dir, int on):_pin(pin),_dir(dir)
{
    char sf[256];

    _config("gpio",_sys);
    _config("gpio_fmt",_fmt);
    ::sprintf(sf, _fmt.c_str(), _sys.c_str(), pin);
	_dev_node  = sf;
    _exPin();
    _dirIt();
    _wrt(_dev_node+"/value",std::to_string(on).c_str());
}

DvGpio::~DvGpio()
{
    if(_pfile)
        ::close(_pfile);
    iclose();
}

bool    DvGpio::iopen(int mode){return false;}

void    DvGpio::iclose(){}

void    DvGpio::flush(){}

bool DvGpio::_exPin()
{
	::msleep(128);
	std::string gpiofl = _dev_node+"/value";
    if(::access(gpiofl.c_str(),0)!=0)
    {
		gpiofl = _sys+ "/export";
		return _wrt(gpiofl,std::to_string(this->_pin).c_str());
    }
    return true;
}

bool DvGpio::_dirIt()
{
	::msleep(128);
    if(this->_dir & eOut)
    {
		return _wrt(_dev_node+"/direction", "out");
    }
	return _wrt(_dev_node+"/direction", "in");
}

size_t     DvGpio::bread(uint8_t* buff, int len, int)
{
	size_t rv = _red(_dev_node+"/value", buff, len);
	if(rv>0) on_event(eREAD, buff, len);
	return rv;
}

int     DvGpio::bwrite(const uint8_t* buff, int len, int)
{
	int rv =  _wrt(_dev_node+"/value", (const char*)buff, len);
	if(rv>0) on_event(eREAD, buff, len);
	return rv;
}

bool    DvGpio::_watch_edge(int updpwn)
{
    if(updpwn==0)
    {
        if(_pfile>0)::close(_pfile);_pfile=0;
        if(_wrt(_dev_node+"/edge", "none", 4))
            return true;
    }
    else if(updpwn==1)
    {
        if(_pfile>0)::close(_pfile);_pfile=0;
        if(_wrt(_dev_node+"/edge", "rising", 6))
        {
            _pfile = open(this->_dev_node.c_str(),O_RDONLY | O_NONBLOCK);
            if(_pfile <=0)
            {
                LOGE("cannot open "<<this->_dev_node);
                return false;
            }
            return true;
        }
    }
    if(updpwn==-1)
    {
        if(_pfile>0)::close(_pfile);_pfile=0;
        if(_wrt(_dev_node+"/edge", "falling", 6))
        {
            _pfile = open(this->_dev_node.c_str(),O_RDONLY | O_NONBLOCK);
            if(_pfile <=0)
            {
                LOGE("cannot open "<<this->_dev_node);
                return false;
            }
            return true;
        }
    }
    return false;
}

}


