/*
Copyright (c) 2014-2016 PANGEEA.DPT All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/pangeea. The name of the
https://github.com/pangeea may not be used to endorse or promote
products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "filedev.h"


FileDev::FileDev(E_TYPE e,
                 const char* file,
                 const char* name):DvFile(file),
                                   Divais(e, name),
                                   Reg<FileDev>(this),
                                   RtxBus<FileDev>(this)
{
    _o.BindCppObject(this);
}

FileDev::FileDev(SqObj& o,
                 E_TYPE e,
                 const char* file,
                 const char* name):DvFile(file),
                                   Divais(e, name),
                                   Reg<FileDev>(this),
                                   RtxBus<FileDev>(this)
{
    ctx_it(o, name);
}


FileDev::~FileDev()
{

}

bool  FileDev::_write_now(const any_t& vl)
{
    return this->bwrite(vl.c_bytes(), vl.length());
}

size_t  FileDev::_read_now(any_t& vl, const char* filter)
{
    return 0;
}


bool FileDev::_touch_it(size_t t)
{
    return _mon_dirt;
}

int FileDev::set_monitor(bool mon)
{
    _monitor = mon;
    return 0;
}

void FileDev::on_event(E_VENT e, const uint8_t* buff, int len, int options)
{
}

const char*	FileDev::_get_values(const char* key)
{
    GETER_SYSCAT();
}

bool	FileDev::_set_values(const char* key, const char* value)
{
    return false;
}
