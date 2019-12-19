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

#ifndef JSADAPTOR_H
#define JSADAPTOR_H

#include <squirrel.h>
#include <sqrat.h>
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


struct TblJsTbl final
{
    rapidjson::Value&                   _pd;
    Sqrat::Table&                       _t;
    rapidjson::Document::AllocatorType& _at;


    TblJsTbl(Sqrat::Table& t, rapidjson::Value& d,
             rapidjson::Document::AllocatorType& at):_pd(d),_t(t),_at(at)
    {
        rapidize(t,d);
    }

    TblJsTbl(rapidjson::Value& d, Sqrat::Table& t,
             rapidjson::Document::AllocatorType& at):_pd(d),_t(t),_at(at)
    {
        tableize(d,t,nullptr);
    }

    void tableize(const rapidjson::Value &node, Sqrat::Table& t, const char* name)
    {
        for (rapidjson::Value::ConstMemberIterator childNode =
             node.MemberBegin(); childNode != node.MemberEnd();
             ++childNode)
        {
            is_node(childNode->value, t, childNode->name.GetString());
        }
    }

    void is_node(const rapidjson::Value &node,
                 Sqrat::Table& t,
                 const char* nodename,
                 Sqrat::Array* pa=nullptr, int idx=0)
    {
        if (node.IsBool() || node.IsInt() || node.IsUint())
        {
            int i = node.GetInt();
            if(pa)
                pa->SetValue(idx, i);
            else
                t.SetValue(nodename, i);
        }
        else if (node.IsDouble())
        {
            double i = node.GetDouble();
            if(pa)
                pa->SetValue(idx, i);
            else
                t.SetValue(nodename, i);
        }
        else if (node.IsFloat())
        {
            double i = node.GetFloat();
            if(pa)
                pa->SetValue(idx, i);
            else
                t.SetValue(nodename, i);
        }
        else if (node.IsString())
        {
            if(pa)
                pa->SetValue(idx, node.GetString());
            else
                t.SetValue(nodename, node.GetString());
        }
        else if (node.IsObject())   //no object in arrays
        {
            Sqrat::Table t2(t.GetVM());
            tableize(node, t2, nodename);
            t.SetValue(nodename, t2);
        }
        else if (node.IsArray())
        {
            is_array(node, t, nodename); // no arrays in arrays
        }
    }

    void is_array(const rapidjson::Value& node,   Sqrat::Table& t, const char* nodename)
    {
        Sqrat::Array ar(t.GetVM(), node.Size());

        for (rapidjson::SizeType i = 0; i < node.Size(); ++i)
        {
            is_node(node[i], t, nodename, &ar, i);
        }
        _t.SetValue(nodename, ar);
    }

    void rapidize(Sqrat::Table& t, rapidjson::Value& d)
    {
        Sqrat::Object::iterator it;

        while(t.Next(it)==true)
        {
            HSQOBJECT v = it.getValue();
            const char* s = it.getName();
            switch(v._type)
            {
            case OT_INTEGER:{
                rapidjson::Value kname(s,_at);
                rapidjson::Value kvalue((int)v._unVal.nInteger);
                _pd.AddMember(kname,kvalue,_at);
            }
                break;
            case OT_FLOAT:{
                rapidjson::Value kname(s,_at);
                rapidjson::Value kvalue(v._unVal.fFloat);
                _pd.AddMember(kname,kvalue,_at);
            }
                break;
            case OT_BOOL:
            {
                rapidjson::Value kname(s,_at);
                rapidjson::Value kvalue(v._unVal.nInteger);
                _pd.AddMember(kname,kvalue,_at);
            }
                break;
            case OT_STRING:
            {
                rapidjson::Value kname(s,_at);
                const char* ps = SQ_PTRS->objtostring(&v);
                rapidjson::Value kvalue(ps,::strlen(ps));
                _pd.AddMember(kname,kvalue,_at);
            }
                break;
            case OT_TABLE:{
                Sqrat::Table t2(v);
                if(!_t.IsNull())
                {
                    rapidjson::Value fragment2(rapidjson::kObjectType);
                    rapidize(t2, fragment2);
                    rapidjson::Value kname(s,_at);
                    rapidjson::Value kvalue(fragment2,_at);
                    _pd.AddMember(kname, kvalue, _at);
                }
            }
                break;
            case OT_ARRAY:
            {
                Sqrat::Array a(v);
                if(!a.IsNull())
                {
                    //rapidjson::Value fragment2(rapidjson::kObjectType);
                    rapidjson::Value dpts(rapidjson::kArrayType);
                    rapidjson::Value kname(s,_at);

                    for(int i=0; i < a.Length(); ++i)
                    {
                        Sqrat::SharedPtr<uint8_t> pv = a.GetValue<uint8_t>(i);
                        dpts.PushBack((uint8_t)*pv.Get(), _at);
                    }
                    _pd.AddMember(kname,dpts,_at);
                }
            }
                break;
            default:{
                char err[32];
                ::sprintf(err,"typeof_%s",s);
                rapidjson::Value kname(s,_at);
                rapidjson::Value kvalue((size_t)v._type);
                _pd.AddMember(kname, kvalue, _at);
            }
                break;
            }
        }
    }
};

#endif // JSADAPTOR_H
