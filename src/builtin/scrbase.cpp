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

#include <dlfcn.h>
#include "scrbase.h"
#include "inst.h"


ScrBase::ScrBase(SqObj& o, E_TYPE e, const char* name):Divais(e, name),
                Reg<ScrBase>(this)
{
    ctx_it(o, name);
    _o.BindCppObject(this);

    std::cout << __FUNCTION__ << "\n";
}

ScrBase::~ScrBase()
{
    std::cout << __FUNCTION__ << "\n";
}

bool  ScrBase::_write_now(const any_t& vl)
{
    return false;
}

size_t  ScrBase::_read_now(any_t& vl, const char* filter)
{
    return 0;
}


bool ScrBase::_touch_it(size_t t)
{
    return false;
}


bool ScrBase::iopen(int)
{
    return false;
}

void  ScrBase::iclose()
{
}
