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
    "eSCRIPT",
    "eCURL",
    "eSUNRS",
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
        _forjson += "name=";
        _forjson += _name;

   //     _forjson += "&id=";
   //     _forjson += _ukey;

        IoOps* pops = dynamic_cast<IoOps*>(this);
        if(pops)
        {
            _forjson += "&cat=";
            _forjson += __scats[pops->peer_of()];

            _forjson += "&type=";
            _forjson += __stypes[pops->data_of()];

            _forjson += "&mon=";
            _forjson += std::to_string(_monitor);

            if(*pops->err_desc()){
                _forjson += "&err=";
                _forjson += pops->err_desc();
            }
        }
        if(!_oget_value.IsNull())
        {
            Sqrat::Table t = *(_oget_value.Fcall<Sqrat::Table>(key).Get());
            if(!t.IsNull())
            {
                _tbl2string(t,_forjson);
                return _forjson.c_str();
            }
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
    if(!_oget_value.IsNull())
    {
        Sqrat::Table t = *(_oget_value.Fcall<Sqrat::Table>(key).Get());
        if(!t.IsNull())
        {
            _tbl2string(t,_forjson);
            return _forjson.c_str();
        }
    }
    return _get_values(key);
}

void Divais::_tbl2string(Sqrat::Table& t, std::string& s)
{
    Sqrat::Object::iterator it;
    char                    out[32];

    while(t.Next(it)==true)
    {
        const char* sn = it.getName();
        HSQOBJECT   o = it.getValue();

        s += "&";  s += sn;  s+="=";
        switch(_RAW_TYPE(o._type))
        {
            case _RT_NULL:         ::sprintf(out,"%d",0); break;
            case _RT_INTEGER:      ::sprintf(out, "%d", SQ_PTRS->objtointeger(&o)); break;
            case _RT_FLOAT:        ::sprintf(out, "%d", SQ_PTRS->objtobool(&o));   break;
            case _RT_BOOL:         ::sprintf(out, "%f", SQ_PTRS->objtofloat(&o)); break;
            case _RT_STRING:       ::sprintf(out, "%s", SQ_PTRS->objtostring(&o));  break;
            case _RT_TABLE:        assert(0); break;
            case _RT_ARRAY:
                {
                    Sqrat::Array a(o);
                    SQObjectType t =  a.GetType(0);

                    for(int i=0; i< a.GetSize();++i)
                    {
                        if(t==OT_INTEGER)       ::sprintf(out,"%d",*a.GetValue<int>(i).Get());
                        else if(t==OT_FLOAT)    ::sprintf(out,"%f",*a.GetValue<float>(i).Get());
                        else if(t==OT_BOOL)     ::sprintf(out,"%d",*a.GetValue<int>(i).Get());
                        else if(t==OT_STRING)   ::sprintf(out,"%s",*a.GetValue<const char*>(i).Get());
                        else ::sprintf(out,"%p",a.GetValue<int>(i));
                        s+=out;
                        s+=",";
                        out[0]=0;
                    }
                }
                break;
            case _RT_USERDATA:      ::sprintf(out, "%p", o._unVal.pUserData); break;
            case _RT_USERPOINTER:   ::sprintf(out, "%p", o._unVal.pUserPointer); break;
            default: ::sprintf(out, "%p", o._unVal.raw); break;
        }
        if(out[0])
            s+= out;
    }
    LOGI(s);
}

bool	Divais::_set_values(const char* key, const char* value)
{
    return false;
}

const char*	Divais::_get_values(const char* key)
{
    return _forjson.c_str();
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
    return _o;
}
