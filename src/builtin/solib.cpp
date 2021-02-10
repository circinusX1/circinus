/*
Copyright (c) 2014-2016 Marius C. All rights reserved.

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

#include <dlfcn.h>
#include "solib.h"
#include "inst.h"


SoLib::SoLib(const char* plugin):Divais(eVOID, eSOLIB, plugin),Reg<SoLib>(this)
{
    _soh = dlopen(plugin, RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD); //RTLD_NOLOAD flag is not specified in POSIX.1-2001..so not the best solution :(
    if (_soh == NULL) {
        _soh = dlopen(plugin, RTLD_NOW | RTLD_LOCAL);
        if (_soh == NULL){
            LOGE(__FUNCTION__ << dlerror());
            return;
        }
    }
    _o.BindCppObject(this);
}

SoLib::SoLib(SqObj& o,
             const char* plugin):Divais(eVOID, eSOLIB, plugin),Reg<SoLib>(this)
{
    plug_it(o, plugin);
    _soh = dlopen(plugin, RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD); //RTLD_NOLOAD flag is not specified in POSIX.1-2001..so not the best solution :(
    if (_soh == NULL) {
        _soh = dlopen(plugin, RTLD_NOW | RTLD_LOCAL);
        if (_soh == NULL){
            LOGE(__FUNCTION__ << dlerror());
            return;
        }
    }
}

bool SoLib::load(const char* foo, bool ret, int nargs)
{
    if(_soh)
    {
        ANY_FOO_TYPE pfoo = (ANY_FOO_TYPE) dlsym(_soh, foo);
        union {
            ANY_FOO_TYPE pfoo;
            size_t sz;
        } uf;
        uf.pfoo = pfoo;
        if (pfoo == nullptr)
        {
            LOGE(__FUNCTION__ << dlerror());
            return false;
        }
        LOGI("adding function " << foo << " " << std::hex << uf.sz <<std::dec<< " to root");
        Sqrat::RootTable(VM()).MembRt(foo, pfoo, ret ? -nargs : nargs);
        return true;
    }
    return false;
}

SoLib::~SoLib()
{
    if(_soh)::dlclose(_soh);
}


bool  SoLib::_write_now(const devdata_t& vl)
{
    return false;
}

size_t  SoLib::_fecth(devdata_t& vl, const char* filter)
{
    return 0;
}


bool SoLib::_mon_callback(time_t tnow)
{
    return false;
}

bool SoLib::iopen(int){return false;}
void  SoLib::iclose(){}

