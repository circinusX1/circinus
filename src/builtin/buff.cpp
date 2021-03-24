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
#include <ao/ao.h>

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
    int sz = a.GetSize();
    bool isptr = false;
    for(int i=0;i<sz;i++)
    {
        dlayout_t& p = _layout[i];
        switch(p.len)
        {
        case sizeof(char):{ 
            char c = *a.GetValue<char>(i);   
            *((char*)(_pb + p.off)) = c;
        }
            break;
        case  sizeof(short):{
            short c = *a.GetValue<short>(i);   
            *((short*)(_pb + p.off)) = c;
        }
            break;   
#if defined(__LP64__) && __LP64__
        case  sizeof(size_t):
#endif 
        case  sizeof(int):{
            if(p.type=='I')
            {
                int c = *a.GetValue<int>(i);   
                *((int*)(_pb + p.off)) = c;
            }
            else if(p.type=='Z')
            {
                int c = *a.GetValue<size_t>(i);   
                *((size_t*)(_pb + p.off)) = c;
            }
            else if(p.type=='P')
            {
                const char* c = *a.GetValue<const char*>(i);
                p.puser = new uint8_t[::strlen(c)+1];
                char* ptrdst = (char*)(_pb + p.off);
                *((size_t*)(_pb + p.off)) = (size_t)p.puser;
                ::memcpy(p.puser,c,::strlen(c)+1);
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

pointer_t Buff::get()
{
    ao_option* p = (ao_option*)_pb;
    
    return (isize_t)_pb;
}

void Buff::test(isize_t pv)
{
    printf("\n");
}

Buff::~Buff()
{
    delete[] _pb;
    for(const auto& a : _layout)
        delete[] a.puser;
}
