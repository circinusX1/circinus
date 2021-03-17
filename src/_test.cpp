#include "_test.h"
/*
Copyright (c) 2014-2020 Marius C. All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/circinusX1. The name of the
https://github.com/circinusX1/amutrion may not be used to endorse or promote
products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <dlfcn.h>
#include "inst.h"
#include "database.h"
#include "restsrv.h"
#include "apis.h"
#include "swdog.h"
#include "sqratTable.h"
#include "sqratObject.h"
#include "sqratimport.h"
#include "sqwrap.h"
#include "sunset.h"
#include "filedev.h"
#include "i2cdev.h"
#include "dadc.h"
#include "pwmdev.h"
#include "gpiodev.h"
#include "spidev.h"
#include "uartdev.h"
#include "usbdev.h"
#include "adcdev.h"
#include "solib.h"
#include "divais.h"
#include "sunset.h"
#include "scrbase.h"
#include "rawsock.h"
#include "comcurl.h"
#include "comssh.h"
#include "inputsys.h"
#include "fastaccess.h"
#include "inst.h"

class MyI2c : public I2CDev
{
public:
    MyI2c():I2CDev(I2C_0,0,"test"){}

    void call()
    {
        _set_value("5","55661256");
    }

};

void _testfoo()
{
    /*
    MyI2c d;
    d.call();
    */
}
