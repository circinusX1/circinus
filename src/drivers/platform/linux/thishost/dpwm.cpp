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



#ifdef thishost
#include "main.h"
#include "dpwm.h"
#include "dlconfig.h"


#define SET_DUTY(p,d)   (p * ( (double)d / 100.0) )
#define GET_DUTY(p,d)   (((double)d / (double)p) * 100.0)

namespace GenericHw
{

    //
    // enable  /sys/.class/pwm in "/boot/config.txt"
    // #dtoverlay=pwm,pin=18,func=2
    // dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=4
    //

}

#include "../../../common/dpwm.cpp"
#endif //BEAGLEBONE


