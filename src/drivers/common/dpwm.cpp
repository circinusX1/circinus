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
#include "dpwm.h"
#include "dlconfig.h"

namespace GenericHw
{

// kernel beaglebone 4.14.108-ti-r108
// PIN NAMES: ecap0  ecap1  ecap2  ehrpwm0a  ehrpwm0b
// ehrpwm1a  ehrpwm1b  ehrpwm2a  ehrpwm2b
//
std::string DvPwm::_sys = "/dev/pwm/"; // /sys/class/pwm/

//
//R-PI  https://librpip.frasersdev.net/peripheral-config/pwm0and1/
//

DvPwm::DvPwm(const char* syspath,
             EPWM_PERIOD period,
             EPWM_VAL val, bool inv):_prd(period)
{
    _dev_node = syspath;

    _wrt(_dev_node+"/period", std::to_string(period).c_str());
    ::msleep(256);
    int d = SET_DUTY(period,val);
    ::msleep(256);
    _wrt(_dev_node+"/duty_cycle", std::to_string(d).c_str());
    if(inv){
        _wrt(_dev_node+"/polarity", "1");
    }else {
        _wrt(_dev_node+"/polarity", "0");
    }
    ::msleep(256);
    _wrt(_dev_node+"/enable", "1");
    _wrt(_dev_node+"/enable", "1");
    ::sync();
}


DvPwm::~DvPwm()
{
    iclose();
}

bool    DvPwm::iopen(int mode)
{
	return _wrt(_dev_node+"/enable", "1");
}

void    DvPwm::iclose()
{
	_wrt(_dev_node+"/enable", "0");
}

size_t     DvPwm::bread(uint8_t* buff, int len, int)
{
	size_t rv =  _red(_dev_node+"/duty_cycle", buff, len);
	if(rv>0) on_event(eREAD, buff, len);
	return rv;
}

int     DvPwm::bwrite(const uint8_t* buff, int len, int)
{
	int rv = _wrt(_dev_node+"/duty_cycle", (const char*)buff, len);
	if(rv>0) on_event(eWRITE, buff, len);
	return rv;
}

void    DvPwm::flush()
{
}
}
