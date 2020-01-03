/* Copyright (C) Marius C 2015 2017 - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the ZIREXIX license, (author: Marius Chincisan)
*
* You should have received a copy of the ZIREXIX license with
* this file. If not, please visit www.meeiot.org, Author Marius Chincisan
*/

#include "dadc.h"

namespace GenericHw
{
    std::string  DvAdc::_aio_sysfs = "";
    const char* DvAdc::_voltage_fmt = "/tmp/dummy.volts";
}

#include "../../../common/dadc.cpp"
