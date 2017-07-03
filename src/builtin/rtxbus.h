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
    RtxBus(T* d):_pd(d),_auto(true)
    {
    }

    virtual ~RtxBus()
    {

    }

    const char* _gets(int chars)
    {
        fastbuf_t<512> loco(chars+1);

        _pd->reset();
        AutoOC oc(_pd, _auto);
        if(_pd->bread(loco, chars))
            return (const char*)loco;
        return "";
    }

    int _puts(const char* str)
    {
        _devflush();
        AutoOC oc(_pd, _auto);
        int wr = _pd->bwrite((const uint8_t*)str, ::strlen(str));

        return wr;
    }

    bool _puts_cb(const char* str, SqMemb& f)
    {
        if(_puts(str))
        {
            size_t  bytes;
            char loco[512];

            Sqrat::SharedPtr<bool> srv;
            AutoOC oc(_pd, _auto);
            while((bytes = _pd->bread((uint8_t*)loco, 512))>0)
            {
                srv = f.Fcall<bool>(loco);
                if(*srv.Get()==false)
                {
                    break;
                }
            }

            f.Fcall<bool>(nullptr);
            return true;
        }
        return false;
    }

    bool _write_cb(SqArr& a, SqMemb& f)
    {
        if(_write(a))
        {
            size_t  bytes;
            char loco[512];

            Sqrat::SharedPtr<bool> srv;
            AutoOC oc(_pd, _auto);
            while((bytes = _pd->bread((uint8_t*)loco, 512))>0)
            {
                do{
                    SqArr ra(App->psqvm(), bytes);
                    for(size_t i = 0 ; i < bytes; i++)
                    {
                        ra.SetValue(i, loco[i]);
                    }
                    srv = f.Fcall<bool>(ra);
                }while(0);
                if(*srv.Get()==false)
                {
                    break;
                }
            }

            f.Fcall<bool>(nullptr);
            return true;
        }
        return false;
    }

    SqArr _read(int chars)
    {
        SqArr           rar(App->psqvm(), 0);
        fastbuf_t<512>  loco(chars+1);
        AutoOC oc(_pd,  _auto);
        size_t          bytes = _pd->bread(loco, chars);

        if(bytes>0)
        {
            any_t a;

            rar.Resize(bytes);
            for(size_t i = 0 ; i < bytes; i++)
            {
                rar.SetValue(i, loco[i]);
            }
        }
        return rar;
    }

    void _autoopen(bool b){
        _auto=true;
    }

    int _fwrite(SqArr& a )
    {
        int             sz = a.GetSize();
        fastbuf_t<512>  ptr(sz);

        a.GetArray((uint8_t*)ptr, sz);
        AutoOC oc(_pd,  _auto);
        int ret = _pd->fwrite(ptr, sz);

        return ret;
    }

    SqArr _fread(int bytes)
    {
        fastbuf_t<512>  ptr(bytes+1);
        SqArr           rar(App->psqvm(), 0);

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
        SqArr           rar(App->psqvm(), 0);
        fastbuf_t<512>  loco(chars+1);
        size_t          bytes = 0;
        AutoOC oc(_pd,  _auto);
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
            fastbuf_t<512>  ptr(sz);

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
            fastbuf_t<512>  ptr(sz);

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
        fastbuf_t<512>  ptr(sz);

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
    }

    bool _expect_str(const char* ex, int to)
    {
        bool     rv=false;
        uint8_t  loco[512]={0};
        size_t   now = tick_count();
        size_t   tot  = now + to;
        size_t   sz = ::strlen(ex);

        _tmpstr.clear();

        AutoOC oc(_pd,  _auto);
        while(tick_count() < tot && rv==false )
        {
            size_t bytes = _pd->bread(loco, sizeof(loco)-1);
            if(bytes>0){
                _tmpstr.append(loco,bytes);
                if(_tmpstr.length()>sz*32)
                {
                    _tmpstr.erase(0,sz);
                }
            }
            size_t f = _tmpstr.find((const uint8_t*)ex, 0, ::strlen(ex));
            rv = (f!=std::string::npos);
        }

        return rv;
    }

    bool _expect_bin(SqArr& a, int to)
    {
        bool     rv=false;
        uint8_t  loco[512]={0};
        size_t   now = tick_count();
        size_t   tot  = now + to;
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
                if(_tmpstr.length()>sz*32)
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
        int k = 0;
        const any_t& raw =  _pd->get_data();
        SqArr ra(App->psqvm(), raw.length());
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

protected:
    T*          _pd;
    std::basic_string<uint8_t>   _cr;
    std::basic_string<uint8_t>   _tmpstr;
    bool                         _auto;
};

#endif // RTXBUS_H
