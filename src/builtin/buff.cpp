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
    int tl = 0;
    for(int i=0;i<sz;i++)
    {
        int cl = *a.GetValue<char>(i);
        switch(cl)
        {
            case 'c':cl=sizeof(char);break;
            case 's':cl=sizeof(short);break;
            case 'i':cl=sizeof(int);break;
            case 'l':cl=sizeof(size_t);break;
            case 'v':cl=sizeof(void*);break;
            default:
                cl = *a.GetValue<int>(i);
                break;
        }
        _offs.push_back(tl);
        tl += cl;
    }
     _offs.push_back(tl);
    _pb = new uint8_t[tl];
    ::memset(_pb,0,tl);
    _isz = tl;
    _o.BindCppObject(this);
}

void Buff::set(Sqrat::Array& a)
{
    int sz = a.GetSize();
    
    for(int i=0;i<sz;i++)
    {
        int off = _offs[i+1]-_offs[i];
        switch(off)
        {
        case sizeof(char):{ 
            char c = *a.GetValue<char>(i);   
            *((char*)(_pb + _offs[i])) = c;
        }
            break;
        case  sizeof(short):{
            short c = *a.GetValue<short>(i);   
            *((short*)(_pb + _offs[i])) = c;
        }
            break;   
        case  sizeof(int):{
            int c = *a.GetValue<int>(i);   
            *((int*)(_pb + _offs[i])) = c;
        }
            break;   
        case  sizeof(size_t):{
            size_t c = *a.GetValue<size_t>(i);   
            *((size_t*)(_pb + _offs[i])) = c;
        }
            break;   
        default:assert(0);
        }
    }
    return;
}

pointer_t Buff::get()
{
    return (isize_t)_pb;
}

void Buff::test(isize_t pv)
{
    printf("\n");
}

Buff::~Buff()
{
    delete[] _pb;
}
