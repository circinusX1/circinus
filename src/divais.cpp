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

#include <iostream>
#include "sq_engine.h"
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

bool Divais::_mon_callback(time_t tnow)
{
    _mon_dirt = false;
    return false;
}

/**
 * @brief Divais::plug_it
 * @param o
 * @param dev_key
 * get funcitons form script
 */
void Divais::plug_it(SqObj& o, const char* dev_key)
{
    _ukey = dev_key;
    _o = o;
    SqMemb::getFoo(App->psqvm() , o, "set_value", _oset_value);
    SqMemb::getFoo(App->psqvm() , o, "get_value", _oget_value);
    SqMemb::getFoo(App->psqvm() , o, "on_event", _on_event);
}

void   Divais::reset()
{
    _cur_value.clear();
}

bool Divais::set_value(const char* key, const char* value)
{
    bool rv = _set_value(key, value);
    _mon_dirt = true;
    if(!_oset_value.IsNull())
    {
        try{
            rv |=  *(_oset_value.Fcall<bool>(key, value).Get());
        }catch(Sqrat::Exception& ex)
        {
            LOGEX(ex.Message());
            LOGEX(SqErrStr);
        }
    }
    return rv;
}

static std::string operator "" _AS(const char* a,std::size_t) {
  return std::string(a);
}


const char* Divais::get_value(const char* key)
{
    _retparams.clear();
    if(key[0]==ALLDATA)
    {
        _retparams = "N=" +_name + "&U="_AS + _ukey;
        IoOps* pops = dynamic_cast<IoOps*>(this);
        if(pops)
        {
            _retparams += "&K="_AS + __scats[pops->peer_of()];
            _retparams += "&T="_AS + __stypes[pops->data_of()];
            _retparams += "&M="_AS + std::to_string(_monitor);
            if(*pops->err_desc()){ _retparams += "&_E="_AS + pops->err_desc(); }
        }
        if(!_oget_value.IsNull())
        {
            Sqrat::Table t = *(_oget_value.Fcall<Sqrat::Table>(key).Get());
            if(!t.IsNull())
            {
                _tbl2string(t,_retparams);
                return _retparams.c_str();
            }
        }
        return _get_values(key);
    }

    if(key[0]=='U') return _ukey.c_str();
    if(key[0]=='N') return _name.c_str();
    if(key[0]=='M') return _monitor ? "1" : "0";
    if(key[0]=='T') return __stypes[_etype];
    if(key[0]=='K'){ IoOps* pops = dynamic_cast<IoOps*>(this);
        if(pops) return __scats[pops->data_of()]; }
    if(!_oget_value.IsNull())
    {
        Sqrat::Table t = *(_oget_value.Fcall<Sqrat::Table>(key).Get());
        if(!t.IsNull())
        {
            _tbl2string(t,_retparams);
            return _retparams.c_str();
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
        case _RT_NULL:         ::sprintf(out, "%d",0); break;
        case _RT_INTEGER:      ::sprintf(out, "%lld", SQ_PTRS->objtointeger(&o)); break;
        case _RT_FLOAT:        ::sprintf(out, "%d", SQ_PTRS->objtobool(&o));   break;
        case _RT_BOOL:         ::sprintf(out, "%f", SQ_PTRS->objtofloat(&o)); break;
        case _RT_STRING:
            ::sprintf(out, "%s", SQ_PTRS->objtostring(&o));
            if(strchr(out,':')){
                ::sprintf(out, "\"%s\"", SQ_PTRS->objtostring(&o));
            }
            break;
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
                else ::sprintf(out,"%d",*a.GetValue<int>(i).Get());
                s+=out;
                s+=",";
                out[0]=0;
            }
        }
            break;
        case _RT_USERDATA:      ::sprintf(out, "%p", (void*)o._unVal.pUserData); break;
        case _RT_USERPOINTER:   ::sprintf(out, "%p", (void*)o._unVal.pUserPointer); break;
        default: ::sprintf(out, "%p", (void*)o._unVal.raw); break;
        }
        if(out[0])
            s+= out;
    }
    LOGI(s);
}

bool	Divais::_set_value(const char* key, const char* value)
{
    _mon_dirt = true;
    if(key[0]=='_' && key[1]=='M'){_monitor = value[0]=='1' ? true : false; return true;}
    return false;
}

const char*	Divais::_get_values(const char* key)
{

    return _retparams.c_str();
}

const devdata_t& Divais::get_data()const
{
    return _cur_value;
}

void  Divais::sync(const char* filter)
{
    devdata_t loco;
    _fecth(loco, filter == nullptr  ? "*" : filter);
}

EPERIPH Divais::get_category(const char* cat)
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

Sqrat::Object Divais::object()const
{
    return _o;
}

bool Divais::set_cb(SqMemb& m)
{
	_monitor = false;
	if(!_on_event.IsNull())
		_on_event.Release();
	if(!m.IsNull()){
		_monitor = true;
		_on_event = m;
		_mon_dirt = true;
	}
	return _monitor;
}

void Divais::on_event(E_VENT e,
					  const uint8_t* buff,
					  int len,
					  int options)
{
	_mon_dirt = true;
}
