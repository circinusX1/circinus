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

#ifndef MSGQUEUE_H_
#define MSGQUEUE_H_

#include <stdio.h>
#include <ctype.h>
#include <deque>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <sqrat.h>
#include "osthread.h"
#include "sqwrap.h"
#include "logs.h"
#include "divais.h"


class Divais;
struct DevMsg{
    Divais* _pc;
};

#define MAX_QSZ 1024
class EvQue
{
public:
    EvQue(){}
    ~EvQue()
    {
        _c.lock();
        while(_q.size()) {
            _q.pop_front();
        }
        _c.unlock();
    }
    int push(CntPtr<DevMsg>& c)
    {
        _c.lock();
        if(_q.size()<MAX_QSZ)
        {
            _q.push_back(c);
            _c.signal();
        }
        else{
            LOGE("In Queue overflow");
        }
        _c.unlock();
         return 1;
    }

    int rpush(CntPtr<DevMsg>& c)
    {
        _c.lock();
        if(_q.size()<MAX_QSZ)
        {
            _q.push_front(c);
        }
        else{
            LOGE("In Queue overflow");
        }
        _c.unlock();
         return 1;
    }

    CntPtr<DevMsg> pop(bool& ok)
    {
        CntPtr<DevMsg> r;
        ok = false;
        _c.lock();
        if(_q.size()) {
            r = _q.front();
            ok = true;
            _q.pop_front();
        }
        _c.unlock();
        if(_q.size())
            _c.signal();
        return r;
    }

    void remove(Divais* pm)
    {
        _c.lock();
AGAIN:
        std::deque<CntPtr<DevMsg> >::iterator it = _q.begin();
        for(; it != _q.end(); it++)
        {
            if(it->ptr()->_pc == pm)
            {
                _q.erase(it);
                goto AGAIN;
            }
        }
        _c.unlock();
    }

    void signal() {
        _c.signal();
    }
    void broadcast() {
        _c.broadcast();
    }
    size_t size()const {
        return _q.size();
    }
private:
    std::deque<CntPtr<DevMsg> > _q;
    conditio _c;
};


#endif //MSGQUEUE_H_

