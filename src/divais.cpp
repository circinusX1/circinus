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

#include <iostream>
#include "sqwrap.h"
#include "apis.h"
#include "divais.h"

int IDX = 0;

extern sq_api*     SQ_PTRS;

const char* __stypes[]={
    "void",
    "int",
    "int64",
    "uint",
    "uint64",
    "real",
    "string",
    "octets",
    nullptr,
};

const char* __scats[]={
    "na",
    "eGPIO",
    "ePWMM",
    "eI2C",
    "eSPI",
    "eADC",
    "eTIMER",
    "eUART",
    "eHTTP",
    "eSSH",
    "eBASE",
    "eSRV",
    "eDB",
    "eFILE",
    "eSOLIB",
    "eUSB",
    "eSOCKET",
    "eDEVMODULE",
    nullptr,
};

Divais::~Divais()
{
    if(_o.IsNull()) return;
    _o.AddRef();
}

bool Divais::_check_dirt()
{
    if(_old_data == _curdata){;}else
    {
        _old_data = _curdata;
        return true;
    }
    return false;
}

void Divais::ctx_it(SqObj& o, const char* dev_key)
{
    _ukey = dev_key;
    _o = o;
    SqMemb::getFoo(App->psqvm() , o, "set_value", _oset_value);
    SqMemb::getFoo(App->psqvm() , o, "get_value", _oget_value);
}

void   Divais::reset()
{
    _curdata.clear();
}

bool Divais::set_value(const char* key, const char* value)
{
    if(!_oset_value.IsNull())
    {
        try{
            bool rv =  *(_oset_value.Fcall<bool>(key, value).Get());
            return rv;
        }catch(Sqrat::Exception& ex)
        {
            LOGEX(ex.Message());
            LOGEX(SqErrStr);
        }
    }
    return _set_values(key, value);
}

const char* Divais::get_value(const char* key)
{
    _forjson.clear();
    if(key[0]==ALLDATA)
    {
        IoOps* pops = dynamic_cast<IoOps*>(this);
        if(pops)
        {
            _forjson += "name,";
            _forjson += _name;

            _forjson += ",cat,";
            _forjson += __scats[pops->peer_of()];

            _forjson += ",type,";
            _forjson += __stypes[pops->data_of()];

            _forjson += ",mon,";
            _forjson += std::to_string(_monitor);

            if(*pops->err_desc()){
                _forjson += ",err,";
                _forjson += pops->err_desc();
            }
            _forjson += ",";
        }
        if(!_oget_value.IsNull())
        {
            _forjson += *(_oget_value.Fcall<const char*>(key).Get());
            return _forjson.c_str();
        }
        return _get_values(key);
    }

    if(key[0]=='u')      // user key
        return _ukey.c_str();
    if(key[0]=='n')      // name
        return _name.c_str();
    if(key[0]=='m')
        return _monitor ? "1" : "0";
    if(key[0]=='t')      //type
       return __stypes[_etype];
    if(key[0]=='k')      //type
    {
        IoOps* pops = dynamic_cast<IoOps*>(this);
        if(pops)
            return __scats[pops->data_of()];
    }
    return _get_values(key);
}

bool	Divais::_set_values(const char* key, const char* value)
{
    return false;
}

const char*	Divais::_get_values(const char* key)
{
    return "";
}

const any_t& Divais::get_data()const
{
    return _curdata;
}

void  Divais::sync(const char* filter)
{
    any_t loco;
    _read_now(loco, filter);
}

EPERIPH Divais::Get_cat(const char* cat)
{
    const char** pc = __scats;
    int index = 0;
    while(*pc){
        if(!::strcmp(cat,*pc))
            return EPERIPH(index);
        ++pc;
        ++index;
    }
    return eNOCAT;
}

Sqrat::Object Divais::object()
{
    if(_o.IsNull())
    {
        Sqrat::Object o;
        return o;
    }
    return _o;
}
