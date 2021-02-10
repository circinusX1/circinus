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
    RtxBus(T* d, bool autoo, bool alloca=false):_pd(d),_auto(autoo),_iobuff(nullptr)
    {
        if(alloca)
            IoType_t::construct(&_iobuff);
    }

    virtual ~RtxBus()
    {
        IoType_t::destroy(&_iobuff);
    }

    int _putsln(const char* str)
    {
        AutoOC oc(_pd, _auto);
        _devflush();
        _iobuff->set(str);
        _iobuff->store(_cr);
        return _pd->bwrite(_iobuff->buf(), _iobuff->len());
    }

    int _puts(const char* str)
    {
        AutoOC oc(_pd, _auto);
        _devflush();
        return _pd->bwrite((const uint8_t*)str, ::strlen(str));
    }

    bool _puts_cb(const char* str, SqMemb& f)
    {
        _iobuff->set(str);
        _iobuff->store(_cr);
        if(_puts(_iobuff->c_str()))
        {
            size_t  bytes;
            Sqrat::SharedPtr<bool> srv;
            AutoOC oc(_pd, _auto);
            time_t fut = tick_count() + _tout;

            _iobuff->clear();
            while(fut - tick_count() >0 ||
                  (bytes = _pd->bread(_iobuff->buf(), _iobuff->cap()))>0)
            {
                if(bytes>0)
                {
                    srv = f.Fcall<bool>(_iobuff->c_str());
                    fut = 0;
                    if(*srv.Get()==false)
                    {
                        break;
                    }
                }
            }
            f.Fcall<bool>(_iobuff->c_str()); /* no more data */
            return true;
        }
        return false;
    }

    bool _write_cb(SqArr& a, SqMemb& f)
    {
        if(_write(a))
        {
            size_t  bytes;
            time_t fut = tick_count()+_tout;
            Sqrat::SharedPtr<bool> srv;
            AutoOC oc(_pd, _auto);

            _iobuff->clear();
            while(fut>tick_count() ||
                  (bytes = _pd->bread(_iobuff->buf(), _iobuff->cap()))>0)
            {
                if(bytes)
                {
                    SqArr ra(App->psqvm(), bytes);
                    for(size_t i = 0 ; i < bytes; i++)
                    {
                        ra.SetValue(i, _iobuff->at(i));
                    }
                    srv = f.Fcall<bool>(ra);
                    fut = 0;
                    if(*srv.Get()==false)
                    {
                        break;
                    }
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
        fastbuf_t   loco(_bufsz);

        _pd->reset();
        int bytes = _pd->bread((uint8_t*)loco, _bufsz);
        if(bytes)
        {
            if(_pd->is_picking())  _pd->dirtyit();
            _iobuff->store(loco,bytes);
        }
        if(_iobuff->len())
            _tmpstr.assign(_iobuff->buf(), _iobuff->len());
        else
            _tmpstr.clear();
        if(!_pd->is_picking()){
            _iobuff->clear();
        }
        return (const char*)_tmpstr.c_str();
    }

    SqArr _read()
    {
        SqArr           rar(App->psqvm(), 0);
        AutoOC          oc(_pd,  _auto);
        fastbuf_t       loco(_bufsz);

        _pd->reset();
        int bytes = _pd->bread((uint8_t*)loco, _bufsz);
        if(bytes)
        {
            if(_pd->is_picking())  _pd->dirtyit();
            _iobuff->store(loco,bytes);
        }
        if(_iobuff->len())
        {
            rar.Resize(_iobuff->len());
            for(size_t i = 0 ; i < _iobuff->len(); i++)
            {
                rar.SetValue(i, _iobuff->at(i));
            }
        }
        if(!_pd->is_picking()){
            _iobuff->clear();
        }
        return rar;
    }

    void _autoopen(bool b){
        _auto=b;
    }

    int _fwrite(SqArr& a )
    {
        int        sz = a.GetSize();
        fastbuf_t  ptr(sz);

        a.GetArray((uint8_t*)ptr, sz);
        AutoOC oc(_pd,  _auto);
        int ret = _pd->fwrite(ptr, sz);
        return ret;
    }

    SqArr _fread(int bytes)
    {
        fastbuf_t  ptr(bytes+1);
        SqArr      rar(App->psqvm(), 0);

        AutoOC oc(_pd,  _auto);
        int rv = _pd->fread(ptr, bytes);
        if(rv>0)
        {
            rar.Resize(bytes);
            for(int i = 0 ; i < bytes; i++)
            {
                rar.SetValue(i, ptr[i]);
            }
        }

        return rar;
    }

    SqArr _readreg(uint8_t reg, size_t chars)
    {
        SqArr       rar(App->psqvm(), 0);
        fastbuf_t   loco(chars+1);
        size_t      bytes = 0;
        AutoOC      oc(_pd,  _auto);

        bytes = _pd->bread(loco, chars, reg);
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
        int      sz = a.GetSize();
        if(sz)
        {
            fastbuf_t  ptr(sz);

            _devflush();
            a.GetArray((uint8_t*)ptr, sz);
            AutoOC oc(_pd, _auto);
            sz = _pd->bwrite(ptr,sz);
        }
        return sz;
    }

    int _writereg(uint8_t reg, SqArr& a)
    {
        int     sz = a.GetSize();
        if(sz)
        {
            fastbuf_t  ptr(sz);

            _devflush();
            a.GetArray((uint8_t*)ptr, sz);
            AutoOC oc(_pd,  _auto);
            sz = _pd->bwrite(ptr,sz,reg);
        }
        else {
            sz = _pd->bwrite(nullptr,0,reg);
        }
        return sz;
    }

    SqArr& _ioctl(int ctl, SqArr& a, int expect)
    {
        size_t          sz = a.GetSize();
        fastbuf_t  ptr(sz);

        a.GetArray((uint8_t*)ptr, sz);
        AutoOC oc(_pd,  _auto);
        if(_pd->do_ioctl(ctl, ptr, expect)==0)
        {
            a.Resize(expect);
            for(int i=0;i<expect;++i)
                a.SetValue(i,ptr[i]);
        }
        return a;
    }

    int _setcr(SqArr& a)
    {
        size_t  sz = a.GetSize();
        uint8_t ptr[32]={0};

        a.GetArray((uint8_t*)ptr, sz);
        _cr.assign(ptr,sz);
        return 0;
    }

    void _devflush()
    {
        _pd->flush();
        _pd->reset();
        _tmpstr.clear();
        _tmp2.clear();
    }

    SqArr _expect_arr(SqArr& a)
    {
        bool     rv=false;
        uint8_t  loco[_bufsz]={0};
        size_t   now = tick_count();
        time_t   tot  = now + _tout;
        size_t   sz = 0;

        _tmpstr.clear();
        _tmp2.clear();
        AutoOC oc(_pd,  _auto);
        while(tick_count() < tot && rv==false )
        {
            size_t bytes = _pd->bread(loco, sizeof(loco)-1);
            if(bytes>0){
                _tmpstr.append(loco, bytes);
                int ns  = a.GetSize();

                for(int i=0;i<ns;i++)
                {
                    const char* token = *(a.GetValue<const char*>(i).Get());
                    sz += ::strlen(token)+1;
                    if(_tmpstr.find((const uint8_t*)token)!=std::string::npos)
                    {
                        _tmp2.assign((const uint8_t*)token,::strlen(token));
                        tot=::tick_count();
                        break;
                    }
                }
                _truncate(_tmpstr);
            }
        }
        if(_tmp2.length())
        {
            SqArr ra(App->psqvm(), _tmp2.length());
            for(size_t i=0; i< _tmpstr.length(); ++i )
            {
                ra.SetValue(i, _tmpstr.at(i));
            }
            return ra;
        }
        return _nula;
    }


    bool _expect_str(const char* ex)
    {
        bool     rv=false;
        uint8_t  loco[_bufsz]={0};
        size_t   now = tick_count();
        time_t   tot  = now + _tout;
        size_t   sz = ::strlen(ex);

        _tmpstr.clear();

        AutoOC oc(_pd,  _auto);
        while(tick_count() < tot && rv==false )
        {
            size_t bytes = _pd->bread(loco, sizeof(loco)-1);
            if(bytes>0){
                _tmpstr.append(loco,bytes);
                if(_tmpstr.length()>sz*4)
                {
                    _tmpstr.erase(0,sz);
                }
            }
            size_t f = _tmpstr.find((const uint8_t*)ex, 0, ::strlen(ex));
            rv = (f!=std::string::npos);
        }
        size_t bytes = _pd->bread(loco, sizeof(loco)-1);
        if(bytes>0){
            _tmpstr.append(loco,bytes);
            if(_tmpstr.length()>sz*4)
            {
                _tmpstr.erase(0,sz);
            }
        }
        return rv;
    }

    bool _expect_bin(SqArr& a)
    {
        bool     rv=false;
        uint8_t  loco[_bufsz]={0};
        time_t   now = tick_count();
        time_t   tot  = now + _tout;
        size_t      sz = a.GetSize();
        uint8_t     ptr[1024]={0};

        a.GetArray((uint8_t*)ptr, sz);
        _devflush();
        _tmpstr.clear();

        AutoOC oc(_pd,  _auto);
        while(tick_count() < tot && rv==false)
        {
            size_t bytes = _pd->bread(loco, sizeof(loco)-1);
            if(bytes>0)
            {
                std::cout << (const char*)loco << "\n";
                _tmpstr.append(loco,bytes);
                if(_tmpstr.length()>sz*4)
                {
                    _tmpstr.erase(0,sz);
                }
            }
            rv = _tmpstr.find(ptr,sz) != std::string::npos;
        }
        return rv;
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
    void set_buffers(size_t r, size_t maxb){
        _bufsz = r;
        _maxbufsz = maxb;
        IoType_t::destroy(&_iobuff);
        IoType_t::construct(&_iobuff, _bufsz);
    }

protected:
    void _truncate(std::basic_string<uint8_t>& _tmpstr)
    {
        if(!_tmpstr.empty() && _tmpstr.length() > _maxbufsz)
        {
            _tmpstr.erase(0,(_bufsz<<4));
        }
    }

protected:
    SqArr                        _nula;
    T*                           _pd;
    std::basic_string<uint8_t>   _cr;
    time_t                       _tout=DEF_TOUT;
    size_t                       _bufsz=DEF_BUFF_SZ;
    size_t                       _maxbufsz=MAX_BUFF_SZ;
    std::basic_string<uint8_t>   _tmpstr;
    std::basic_string<uint8_t>   _tmp2;
    bool                         _auto;
    const char                   _emptystr[1]="";
    SqArr                        _emptyarr;
    IoType_t*                      _iobuff = nullptr;
};

#endif // RTXBUS_H
