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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include "buff.h"
#include "inst.h"

Buff::Buff(Sqrat::Array & a)
{
    int sz = a.GetSize();
    dlayout_t dt = {0,0,0,nullptr};
    
    for(int i=0;i<sz;i++)
    {
        int cl = *a.GetValue<int>(i);
        dt.type = cl;
        dt.puser = nullptr;
        dt.len = 0;
        switch(cl)
        {
                case 'B': dt.len=sizeof(char);      break;
                case 'W': dt.len=sizeof(short);     break;
                case 'I': dt.len=sizeof(int);       break;
                case 'Z': dt.len=sizeof(size_t);    break;
                case 'P': dt.len=(sizeof(uint8_t*));break;
                default:  dt.len=cl; break;
        }
        _layout.push_back(dt);
        dt.off += dt.len;
    }
    _layout.push_back(dt);
    _pb = new uint8_t[dt.len];
    ::memset(_pb,0,dt.len);
    _isz = dt.len;
    _o.BindCppObject(this);
}

void Buff::set(Sqrat::Array& a)
{
    Uc* pucd;
    int sz = a.GetSize();
    //bool isptr = false;
   
    for(int i=0;i<sz;i++)
    {
        dlayout_t& p = _layout[i];
        if(p.puser)
            delete[] p.puser;
        p.puser=nullptr;
        pucd = (Uc*)((char*)(_pb + p.off));
        switch(p.len)
        {
        case sizeof(char):{ 
                pucd->cv = *a.GetValue<char>(i);   
            }
            break;
        case  sizeof(short):{
                pucd->sv = *a.GetValue<short>(i);   
            }
            break;   
#if defined(__LP64__) && __LP64__
        case  sizeof(size_t):
#endif 
        case  sizeof(int):{
            if(p.type=='I')
            {
                pucd->iv = *a.GetValue<int>(i);   
            }
            else if(p.type=='Z')
            {
                pucd->sz = *a.GetValue<size_t>(i);   
            }
            else if(p.type=='P')
            {
                SQObjectType t = a.GetType(i);
                if(t == OT_STRING)
                {
                    const char* c = *a.GetValue<const char*>(i);
                    p.puser = new uint8_t[::strlen(c)+1];
                    ::memcpy(p.puser,c,::strlen(c)+1);
                    pucd->pu = p.puser;
                }
                else {
                    p.puser      = new uint8_t[sizeof(size_t)];
                    memset(p.puser,0,sizeof (sizeof(size_t)));
                    Uc* pusr     = (Uc*)p.puser;
                    pusr->sz     = *a.GetValue<size_t>(i);
                    pucd->pu     = p.puser;
                }
            }
        }
            break;   
        default:
            LOGE("invalid buff slot length");
            assert(0);
            break;
            
        }
    }
    return;
}

pointer_t Buff::at(int slot)
{
    Uc* pb = (Uc*)((uint8_t*)(_pb + slot));
    return pointer_t(pb->sz);
}

pointer_t Buff::get()
{
    if(_layout.size()>0)
        return pointer_t(_pb);
    dlayout_t& p = _layout[0];
    if(p.type=='P' && p.puser==nullptr)
    {
        p.puser      = new uint8_t[sizeof(size_t)];
        memset(p.puser,0,sizeof (sizeof(size_t)));
        Uc* pusr     = (Uc*)p.puser;
        pusr->sz     = 0;
        Uc* pucd     = (Uc*)_pb;
        pucd->pu     = p.puser;
    }
    return pointer_t(_pb);
}

pointer_t Buff::deref()
{
    Uc* pb = (Uc*)((uint8_t*)(_pb));
    return pb->sz;
}


const char* Buff::str()
{
    return (const char*)(_pb);
}

void Buff::test()
{
    //Uc* pb = (Uc*)((uint8_t*)(_pb));
    //printf("\n");
}

void Buff::_clear()
{
    for(const auto& a : _layout)
        delete[] a.puser;
    _layout.clear();
}

Buff::~Buff()
{
    delete[] _pb;
    _clear();
}
