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

#ifndef DIVAIS_EMBIX_
#define DIVAIS_EMBIX_

#include <string>
#include <vector>
#include <string>
#include <cstdio>
#include <stdint.h>
#include <stddef.h>
#include <iostream>
#include <inttypes.h>
#include "squirrel.h"
#include "sqrat.h"
#include "jsadaptor.h"
#include "sq_engine.h"
#include "logs.h"
#include "../modules/iper.h"
#include "../modules/isqcls.h"

class   Divais;
#define MAX_BUFF_SZ         (32768)
#define DEF_BUFF_SZ         (256)
#define ONE_SHOT_BUFF_SZ	(256)
#define DEF_TOUT            256

extern  SQVM*       ModVM;
extern  sq_api*     ModPtrs;
void    set_sq_ctx(HSKVM vm, sq_api* ptrs);

typedef    std::map<I_IDev*, std::string>  devsmap_t;

typedef enum EPIN_CHANGE
{
    eNONE = 0,
    e0,
    e1,
    eCHANGE
}EPIN_CHANGE;

typedef enum EPIN_EDGE
{
    eRISING=0x1,
    eFALLING=0x2,
}EPIN_EDGE;

typedef enum EPIN_DIR
{   eIn = 0x1,
    eOut = 0x2,
    eInOut = 0x3
}EPIN_DIR;


extern  int      IDX;
extern const char* __stypes[];
extern const char* __scats[];
static std::string __empty;

class Divais : public I_IDev
{
public:
	Divais(E_TYPE etype, EPERIPH per):_mon_dirt(false),_monitor(false),_etype(etype)
	{
		char t[16];
		::sprintf(t,"%s_%d",__scats[per],++IDX);
		_name = t;
		_ukey = t;
	}
	Divais(E_TYPE etype, EPERIPH per, const char* name):_name(name?name:""),
		_mon_dirt(false),
		_monitor(false),
		_etype(etype)
	{
		char t[32];
		if(_name.empty())
		{
			::sprintf(t,"%s_%d",__scats[per],++IDX);
			_name = t;
			_ukey = t;
		}
		else {
			_ukey = _name;
		}
	}
	virtual ~Divais();
	void  plug_it(Sqrat::Object& o, const char* dev_key);
	const char* name()const{
		return _name.c_str();
	};
	const char* dev_key()const{return _ukey.c_str();};
	void  reset();
	bool _mon_cbacks_call(time_t tnow){
		if(_monitor){
			_mon_dirt = _mon_callback(tnow);
		}
		return _mon_dirt;
	}
	void    set_name(const char* name){_name=name;}
	virtual bool iopen(int rm=0)=0;
	virtual void iclose()=0;
	virtual bool set_value(const char* key, const char* value);
	virtual const char* get_value(const char* key);
	virtual const devdata_t& get_data()const;
	static EPERIPH get_category(const char* cat);
	virtual void   sync(const char* filter=nullptr);
	virtual Sqrat::Object object()const;
	const std::string& eol(){return __empty;}
	void dirtyit(){_mon_dirt=true;}
	virtual bool set_cb(SqMemb& m);
	virtual bool _mon_callback(time_t tnow);
	virtual void on_event(E_VENT e, const uint8_t* buff, int len, int options);
protected:
	virtual bool	_write_now(const devdata_t& a)=0;
	virtual size_t  _fecth(devdata_t& _cur_value, const char* filter)=0;
	virtual bool	_set_value(const char* key, const char* value);
	virtual const char*	_get_values(const char* key);
	Sqrat::Object&	_so(){return _o;}
	template <class T> bool _call_cb(const T& d)
	{
		_mon_dirt = false;
		if(!_on_event.IsNull())
		{
			try{
				_on_event.Fcall<bool>(_o, d);
			}
			catch(Sqrat::Exception& ex){
				LOGEX(ex.Message());
				return false;
			}
			return true;
		}
		return false;
	}
private:
	void _tbl2string(Sqrat::Table& t, std::string& s);

protected:
    std::string      _name;
    std::string      _ukey;
    devdata_t		 _cur_value;
    devdata_t		 _prev_value;
    std::string      _retparams;
    bool             _mon_dirt;
    bool             _monitor;
    Sqrat::Object    _o;
    E_TYPE           _etype;
    Sqrat::Function  _on_event;
private:

    Sqrat::Function  _oset_value;
    Sqrat::Function  _oget_value;
};

template <typename T>
class fast_allocator
{
    T     _local[DEF_BUFF_SZ];
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    fast_allocator(){}
    ~fast_allocator(){}

    template <class U> struct rebind { typedef fast_allocator<U> other; };
    template <class U> fast_allocator(const fast_allocator<U>&){}

    pointer address(reference x) const {return &x;}
    const_pointer address(const_reference x) const {return &x;}
    size_type max_size() const throw() {return size_t(-1) / sizeof(value_type);}
    pointer allocate(size_type n, fast_allocator<T>::const_pointer hint = 0)
    {
        if(n<=DEF_BUFF_SZ){
            return _local;
        }
        return static_cast<pointer>(::malloc(n*sizeof(T)));
    }

    void deallocate(pointer p, size_type n)
    {
        if(p!=_local)::free(p);
    }

    void construct(pointer p, const T& val)
    {
        new(static_cast<void*>(p)) T(val);
    }

    void construct(pointer p)
    {
        new(static_cast<void*>(p)) T();
    }

    void destroy(pointer p)
    {
        p->~T();
    }
};

class bytes_t : public std::basic_string<uint8_t,
                          std::char_traits<uint8_t>,
                          fast_allocator<uint8_t> >
{
    size_t _cap;
public:
    bytes_t(size_t rez=DEF_BUFF_SZ){
        this->reserve(rez-1);
        this->resize(rez-1);
        _cap = rez-1;
    }
    void append(const uint8_t* b, const size_t& l)
    {
        this->std::basic_string<uint8_t,
                          std::char_traits<uint8_t>,
                          fast_allocator<uint8_t> >::append(b,l);
    }
    void append(const bytes_t& b, const size_t& l)
    {
        this->std::basic_string<uint8_t,
                          std::char_traits<uint8_t>,
                          fast_allocator<uint8_t> >::append(b.data(),l);
    }
    size_t cap()const{return _cap-1;}
};


#define OVERW(B1,B2)																\
	static bool _squed; \
	const char* get_label_name()const{return B2::name();}							\
	void plug_it(Sqrat::Object& o,const char* dev_key){B2::plug_it(o, dev_key);}	\
	bool iopen(int em=O_RDWR){return B1::iopen(em);}								\
	void iclose(){B1::iclose();}													\
	void on_event(E_VENT e, const uint8_t* buff, int len, int options=0);			\
	bool _mon_callback(time_t tnow);



#define IS_SNULL(per) per==0 || (per[0]=='(' && per[1]=='n')

#define VIRT_NOT_USED()	\
int     fread(uint8_t* buff, int len){return 0;}; \
int     fwrite(const uint8_t* buff, int len){return 0;}; \
int     do_ioctl(int ctl, int val){return 0;}; \
int     do_ioctl(int ctl, uint8_t* buf, int expect){return 0;};


#define GETER_SYSCAT()  if(key[0]=='Y')  return sf().c_str();   \
						if(key[0]=='K')  return __scats[peer_of()]; \
						return EMPTYS;

#define NOPOS std::string::npos
#define EMPTYS ""
#define READDEV "*"
#define IS_STR(s)  (s && *s)


#endif // PLUG_EMBIX_

