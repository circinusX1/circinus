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




#include "dadc.h"
#include "main.h"
#include "dlconfig.h"

namespace GenericHw
{
    DvAdc::DvAdc(EADC_PIN adc)
    {
		char sys[128];

		_config("adc",_sys);
		_config("adc_fmt",_fmt);
		::sprintf(sys, _fmt.c_str(), _fmt.c_str(), adc);
		_dev_node = sys;
    }

    DvAdc::~DvAdc()
    {
		iclose();
    }

	bool    DvAdc::iopen(int mode)
	{
		return false;
	}

	void    DvAdc::iclose()
	{
	}

	size_t     DvAdc::bread(uint8_t* buff, int len, int options)
    {
		size_t rv = _red(_dev_node.c_str(), buff, len);
		if(rv > 0) on_event(eREAD, buff,len,options);
		return rv;
    }

    int     DvAdc::bwrite(const uint8_t* buff, int len, int options)
    {
        return -1;
    }

    void    DvAdc::flush()
    {

    }

}
