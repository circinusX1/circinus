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

#ifndef RTXBUS_H
#define RTXBUS_H

#include "divais.h"
#include "sqwrap.h"
#include "inst.h"
#include "dserial.h"

using namespace GenericHw;



struct AutoOC{
    Divais* _p; bool _a=false;
    AutoOC(Divais* p,  bool a):_p(p), _a(a){ if(a) p->iopen(O_RDWR); }
    ~AutoOC(){ if(_a)_p->iclose();}
};

template<typename T> class RtxBus
{
public:
    RtxBus(T* d, bool autoo, bool alloca=false):_pd(d),_auto(autoo)
    {
    }

    virtual ~RtxBus()
    {
    }

    int _putsln(const char* str)
    {
        AutoOC      oc(_pd, _auto);
        _devflush();
        _pd->bwrite((const uint8_t*)str,::strlen(str));
        return _pd->bwrite(_cr.data(),_cr.length());
    }

    int _puts(const char* str)
    {
        AutoOC oc(_pd, _auto);
        _devflush();
        return _pd->bwrite((const uint8_t*)str, ::strlen(str));
    }

    bool _puts_cb(const char* str, SqMemb& f)
    {
        if(_puts(str))
        {

            AutoOC      oc(_pd, _auto);
            time_t      fut = tick_count() + _tout;
            Sqrat::SharedPtr<bool> srv;

            while(fut < tick_count())
            {
                bytes_t       loco(_bufsz);
                const size_t  bytes = _pd->bread(loco.data(), loco.cap());
                if(bytes ==0 ) continue;
                loco.resize(bytes);
                srv = f.Fcall<bool>(loco.data());
                fut = 0;
                if(*srv.Get()==false)
                {
                    break;
                }
            }
            f.Fcall<bool>(__empty.c_str());
            return true;
        }
        return false;
    }

    bool _write_cb(SqArr& a, SqMemb& f)
    {
        if(_write(a))
        {
            time_t fut = tick_count()+_tout;
            Sqrat::SharedPtr<bool> srv;
            AutoOC oc(_pd, _auto);

            while(fut>tick_count())
            {
                bytes_t       loco(_bufsz);
                const size_t  bytes = _pd->bread(loco.data(), loco.cap());
                if(bytes ==0 ) continue;

                loco.resize(bytes);
                SqArr ra(App->psqvm(), bytes);
                for(size_t i = 0 ; i < bytes; i++)
                {
                    ra.SetValue(i, loco[i]);
                }
                srv = f.Fcall<bool>(ra);
                fut = 0;
                if(*srv.Get()==false)
                {
                    break;
                }
            }
            f.Fcall<bool>(_emptyarr);
            return true;
        }
        return false;
    }

    const char* _gets()
    {
        AutoOC      oc(_pd, _auto);
        bytes_t     loco(_bufsz);

        _pd->reset();
        const size_t  bytes = _pd->bread(loco.data(), loco.cap());
        if(bytes){
            loco.resize(bytes);
            _t1.assign(loco.data(), bytes);
            return (const char*)_t1.data();
        }
        return __empty.c_str();
    }

    SqArr _read()
    {
        SqArr           rar(App->psqvm(), 0);
        AutoOC          oc(_pd,  _auto);
        bytes_t         loco(_bufsz);

        _pd->reset();
        const size_t  bytes = _pd->bread(loco.data(), loco.cap());
        if(bytes){
            rar.Resize(bytes);
            for(size_t i = 0 ; i < bytes; i++)
            {
                rar.SetValue(i, loco[i]);
            }
        }
        return rar;
    }

    void _autoopen(bool b){
        _auto=b;
    }

    int _fwrite(SqArr& a )
    {
        AutoOC      oc(_pd,  _auto);
        const int   sz = a.GetSize();
        bytes_t     loco(sz);

        a.GetArray(loco.data(), sz);
        loco.resize(sz);
        return _pd->fwrite(loco.data(), sz);
    }

    SqArr _fread(int bytes)
    {
        AutoOC oc(_pd,  _auto);
        bytes_t    loco(bytes);
        SqArr      rar(App->psqvm(), 0);

        const int rv = _pd->fread(loco.data(), bytes);
        loco.resize(rv);
        if(rv>0)
        {
            rar.Resize(bytes);
            for(int i = 0 ; i < bytes; i++)
            {
                rar.SetValue(i, loco[i]);
            }
        }
        return rar;
    }

    SqArr _readreg(uint8_t reg, size_t chars)
    {
        AutoOC      oc(_pd,  _auto);
        SqArr       rar(App->psqvm(), 0);
        bytes_t     loco(chars);

        const size_t bytes = _pd->bread(loco.data(), chars, reg);
        if(bytes>0)
        {
            rar.Resize(bytes);
            for(size_t i = 0 ; i < bytes; i++)
            {
                rar.SetValue(i, loco[i]);
            }
        }
        return rar;
    }

    int _write(SqArr& a)
    {
        int  sz = a.GetSize();
        if(sz)
        {
            AutoOC   oc(_pd, _auto);
            bytes_t  loco(sz);

            _devflush();
            a.GetArray(loco.data(), sz);
            sz = _pd->bwrite(loco.data(),sz);
        }
        return sz;
    }

    int _writereg(uint8_t reg, SqArr& a)
    {
        int     sz = a.GetSize();
        if(sz)
        {
            AutoOC  oc(_pd,  _auto);
            bytes_t loco(sz);

            _devflush();
            a.GetArray(loco.data(), sz);
            sz = _pd->bwrite(loco.data(), sz, reg);
        }
        else
        {
            sz = _pd->bwrite(nullptr, 0, reg);
        }
        return sz;
    }

    SqArr& _ioctl(int ctl, SqArr& a, int toread)
    {
        AutoOC      oc(_pd,  _auto);
        size_t      sz = a.GetSize();
        bytes_t     loco(std::max((size_t)toread,sz));

        a.GetArray(loco.data(), sz);
        if(_pd->do_ioctl(ctl, loco.data(), toread)==toread)
        {
            for(int i=0;i<toread;++i){
                a.SetValue(i,loco[i]);
            }
        }
        return a;
    }

    int _setcr(SqArr& a)
    {
        size_t  sz = a.GetSize();
        uint8_t ptr[8]={0};
        a.GetArray((uint8_t*)ptr, sz);
        _cr.assign(ptr,sz);
        return 0;
    }

    void _devflush()
    {
        _pd->flush();
        _pd->reset();
        _t1.clear();
        _t2.clear();
    }

    // any from array of strings, there is a expect bin
    const char* _expect_any(SqArr& a)
    {
        AutoOC   oc(_pd,  _auto);
        bytes_t  loco(_bufsz);
        time_t   fut  = tick_count() + _tout;

        _t1.clear();
        _t2.clear();

        while(tick_count() < fut)
        {
            const size_t bytes = _pd->bread(loco.data(), loco.cap());
            if(bytes == 0) continue;
            loco.resize(bytes);
            _t1.append(loco.data(), bytes);
            const int ns  = a.GetSize();
            for(int i=0;i<ns;i++)
            {
                const char* token = *(a.GetValue<const char*>(i).Get());
                if(_t1.find((const uint8_t*)token)!=std::string::npos)
                {
                    _t2.assign((const uint8_t*)token,::strlen(token));
                    fut = 0;
                    break;
                }
            }
           _truncate(_t1);
        }
        return (const char*)_t2.c_str();
    }

    bool _expect_str(const char* ex)
    {
        AutoOC   oc(_pd, _auto);
        bytes_t  loco(_bufsz);
        time_t   fut = tick_count() + _tout;
        const size_t   sz  = ::strlen(ex);

        _t1.clear();
        while(tick_count() < fut)
        {
            const size_t bytes = _pd->bread(loco.data(), loco.cap());
            if(bytes==0) continue;
            _t1.append(loco,bytes);
            const size_t f = _t1.find((const uint8_t*)ex, 0, sz);
            if(f!=std::string::npos){
                fut = 0;
                break;
            }
            _truncate(_t1);
        }
        return fut==0;
    }

    bool _expect_bin(SqArr& a)
    {
        AutoOC   oc(_pd,  _auto);
        time_t   fut  = tick_count() + _tout;
        const    size_t sz = a.GetSize();
        bytes_t  loco(sz);
        bytes_t  income(_bufsz);
        int      from = 0;

        a.GetArray(loco.data(), sz);
        _t1.clear();
        while(tick_count() < fut)
        {
            const size_t bytes = _pd->bread(income.data(), loco.cap());
            if(bytes==0) continue;
            income.resize(bytes);
            _t1.append(income,bytes);
            if(_t1.find(loco,from) != std::string::npos)
            {
                fut = 0;
            }
            from += (bytes-sz);
            if(from<0) {from=0;}
            _truncate(_t1);
        }
        return fut==0;
    }

    SqArr _pick_bin()
    {
        int     k = 0;
        const   devdata_t& raw =  _pd->get_data();
        SqArr   ra(App->psqvm(), raw.length());
        for(size_t i=0; i< raw.length(); ++i )
        {
            ra.SetValue(k++, raw.c_bytes()[i]);
        }
        return ra;
    }

    const char* _pick_str()
    {
        return _pd->get_data().c_chars();
    }

    void _set_touts(size_t r){_tout=r;}
    void set_buff_size(size_t r, size_t maxb){
        _bufsz = r;
        _maxbufsz = maxb;
    }

protected:
    template<class TX>
    void _truncate(TX& _t1)
    {
        if(!_t1.empty() && _t1.length() > _maxbufsz)
        {
            _t1.erase(0,(_bufsz>>2));
        }
    }

protected:
    SqArr                        _nula;
    T*                           _pd;
    std::basic_string<uint8_t>   _cr;
    time_t                       _tout=DEF_TOUT;
    size_t                       _bufsz=DEF_BUFF_SZ;
    size_t                       _maxbufsz=MAX_BUFF_SZ;
    bytes_t                      _t1;
    bytes_t                      _t2;
    bool                         _auto;
    SqArr                        _emptyarr;
};

#endif // RTXBUS_H
