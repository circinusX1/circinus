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

#include "jsadaptor.h"
#include "../modules/iper.h"
#include "divais.h"
#if 0
bool  set_data_from_rest(const std::string& key, const std::string& value)
{
    any_t   a;
    switch(1)
    {
    case eINT:
        _writeata.let(::atoi(value.c_str()),0);
        break;
    case eFLOAT:
        _writeata.let(::atof(value.c_str()),0);
        break;
    case eBINARY: //3,4,5,6,7,8
        do{
            strarray_t p;
            CFL::explode(value,',',p);       // param=&param=
            _writeata.clear();
            for(const auto& c : p)
            {
                if(c[0]=='0')
                {
                    int lv = strtoul(c.c_str(),nullptr,16);
                    _writeata.pusht(lv);
                }
                else {
                    _writeata.pusht(::atoi(c.c_str()));
                }
            }
        }while(0);
        break;
    case eSTRING:
    default:
        _writeata.let(value.c_str(), value.length());
        break;
    }
    return true;
}


bool  Divais::set_value(const char* json)
{
    rapidjson::Document d;
    d.Parse(json);
    if(d.IsObject())
    {
        return set_value(d);
    }
    return false;
}

bool  Divais::set_value(rapidjson::Document& d)
{
    std::string member;
    if(d.HasMember(_name.c_str()))
        member = _name;
    else if(d.HasMember(_ukey.c_str()))
        member = _ukey;
    if(member.empty())
    {
        rapidjson::StringBuffer strbuf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
        d.Accept(writer);
        LOGW(__FUNCTION__ << "json has no member for device " << _name);
        LOGW(strbuf.GetString());
        return false;
    }
    rapidjson::Value& s = d[member.c_str()];

    if(!s.IsObject())
        return false;

    if(!_setjson.IsNull())
    {
        Sqrat::Table    t;
        rapidjson::Document::AllocatorType& alc = d.GetAllocator();
        TblJsTbl                            embunk(s, t, alc);

        if(!_setjson.IsNull())
        {
            try{
                return *_setjson.Fcall<bool>(embunk._t).Get();
            }catch(Sqrat::Exception& ex)
            {
                LOGEX(ex.Message());
                LOGEX(SqErrStr);
            }
            return false;
        }
    }
    else {
        std::string jss;
        if(s.HasMember("V"))
        {
            rapidjson::Value& dp = d["V"];
            if(dp.IsArray()) // only bytes
            {
                rapidjson::Value::ConstValueIterator itr = dp.Begin();
                _writeata.clear();
                for (;itr != dp.End(); ++itr)
                {
                    _writeata.pusht(dp.GetInt());
                }
            }
            else if(dp.IsString())
            {
                const char* p = dp.GetString();
                _writeata.let(p,::strlen(p));
            }
            else if(dp.IsInt())
            {
                _writeata.let(dp.GetInt(),0);
            }
            return true;
        }
    }
    return false;
}


bool Divais::get_value(rapidjson::Document* pd, const char* what)
{
    /**
      custom script comples csensors
      */
    if(!_getjson.IsNull())
    {
        Sqrat::SharedPtr<Sqrat::Table> srv;
        try{
            srv = _getjson.Fcall<Sqrat::Table>(what);
        }catch(Sqrat::Exception& ex)
        {
            LOGEX(ex.Message());
            LOGEX(SqErrStr);
            return false;
        }
        Sqrat::Table& t = *srv.Get();
        if(!t.IsNull())
        {
            rapidjson::Document::AllocatorType& allc = pd->GetAllocator();
            rapidjson::Value fragment(rapidjson::kObjectType);
            TblJsTbl emb(t, fragment, allc);
            rapidjson::Value kname(_ukey.c_str(),allc);
            rapidjson::Value kvalue(emb._pd,allc);
            pd->AddMember(kname, kvalue, allc);
        }
        return _forjson.c_str();
    }//in script
    /***
    name:{
        {0:asdfadsf}
        {1:[3,3,3,3}
    }
    */
    char seln[8];
    rapidjson::Document::AllocatorType& allc = pd->GetAllocator();
    rapidjson::Value kisland(rapidjson::kObjectType);
    rapidjson::Value kname(_ukey.c_str(),allc);
    for(size_t el = 0 ; el < _curdata.size(); el++)
    {
        sprintf(seln,"%d",el);
        if(_curdata.length(el)==0)continue;

        rapidjson::Value elname(seln, allc);

        if(_etype==eSTRING)
        {
            rapidjson::Value kvalue((const char*)_curdata.c_chars(),allc);
            kisland.AddMember(elname, kvalue, allc);
        }
        else if(_etype==eFLOAT)
        {
            double f = ::atof((const char*)_curdata.c_chars());
            kisland.AddMember(elname, f, allc);
        }
        else if(_etype==eINT)
        {
            int f = ::atoi((const char*)_curdata.c_chars());
            kisland.AddMember(elname, f, allc);
        }
        else
        {
            const uint8_t* pb = _curdata.c_bytes();
            rapidjson::Value dpts(rapidjson::kArrayType);
            for(size_t i=0; i< _writeata.length();++i)
            {
                dpts.PushBack(pb[i], allc);
            }
            kisland.AddMember(elname, dpts, allc);
        }
    }
    kname.SetString((const char*)_name.c_str(),allc);
    pd->AddMember(kname, kisland, allc);
    return true;
}


#endif
