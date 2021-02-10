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
#include "../modules/iper.h"

class   Divais;
#define MAX_BUFF_SZ (32768-1)
#define DEF_BUFF_SZ	(512-1)
#define ONE_SHOT_BUFF_SZ	(512-1)
#define DEF_TOUT    10000

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

class IoType_t
{
	size_t _cap;
	bool _truncated=false;
	IoType_t(size_t SZ=DEF_BUFF_SZ):_cap(SZ){_storage.reserve(_cap);}
	std::basic_string<uint8_t> _storage;

public:
	void  set(const uint8_t* p, size_t sz){
		_storage.assign(p,sz);
	}
	void set(const char* p){
		_storage.clear();
		_storage.assign((const uint8_t*)p,::strlen(p));
	}
	void store(const uint8_t* p, size_t sz){
		if(_storage.length()+sz > _cap){
			_storage.erase(0,_cap+1);
			_truncated=true;
		}else{
			_truncated=false;
		}
		_storage.append(p,sz);
	}
	void store(const std::basic_string<uint8_t>& bff){
		store(bff.data(), bff.length());
	}
	void clear(){_storage.clear();}
	bool is_trunc()const{return _truncated;}
	static void destroy(IoType_t** ppd){
		delete *ppd;
		*ppd=nullptr;
	}
	static void construct(IoType_t** ppd, size_t sz=DEF_BUFF_SZ){
		assert(*ppd==nullptr);
		*ppd=new IoType_t(sz+1);
	}
	const IoType_t& operator=(const char* s){
		_storage.assign((const uint8_t*)s,::strlen(s));
		return *this;}
	size_t   len()const{return _storage.length();}
	size_t   cap()const{return _cap;}
	uint8_t* buf(){return _storage.data();}
	const char* c_str()const{return (const char*)_storage.data();}
	uint8_t& at(size_t i){return _storage.data()[i];}
};



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
	bool is_dirty(time_t tnow){
		if(_monitor){
			_picking = true;
			bool rv = _mon_pick( tnow);
			_picking=false;
			return rv;
		}
		return false;
	}
	void    set_name(const char* name){_name=name;}
	virtual bool iopen(int rm=0)=0;
	virtual void iclose()=0;
	virtual bool set_value(const char* key, const char* value);
	virtual const char* get_value(const char* key);
	virtual const devdata_t& get_data()const;
	virtual bool  on_event();
	static EPERIPH get_category(const char* cat);
	virtual void   sync(const char* filter=nullptr);
	virtual Sqrat::Object object()const;
	const std::string& eol(){return __empty;}
	bool is_picking()const{return _picking;};
	void dirtyit(){_mon_dirt=true;}
protected:
	virtual bool	_write_now(const devdata_t& a)=0;
	virtual bool	_mon_pick(time_t tnow)=0;

	virtual size_t  _fecth(devdata_t& _curdata, const char* filter)=0;
	virtual bool	_set_values(const char* key, const char* value);
	virtual const char*	_get_values(const char* key);
	Sqrat::Object&	_so(){return _o;}
	bool			_check_dirt();

private:
	void _tbl2string(Sqrat::Table& t, std::string& s);

protected:
    std::string      _name;
    std::string      _ukey;
    devdata_t		 _curdata;
    devdata_t		 _old_data;
    std::string      _forjson;
    bool             _mon_dirt;
    bool             _monitor;
    Sqrat::Object    _o;
    E_TYPE           _etype;
    Sqrat::Function  _on_event;
    bool             _picking = false;
private:

    Sqrat::Function  _oset_value;
    Sqrat::Function  _oget_value;
};

#define OVERW(B1,B2)																\
	const char* get_label_name()const{return B2::name();}							\
	void plug_it(Sqrat::Object& o,const char* dev_key){B2::plug_it(o, dev_key);}	\
	bool iopen(int em=O_RDWR){return B1::iopen(em);}								\
	void iclose(){B1::iclose();}													\
	void on_event(E_VENT e, const uint8_t* buff, int len, int options=0);			\
	bool _mon_pick(time_t tnow);

#define IS_SNULL(per) per==0 || (per[0]=='(' && per[1]=='n')

#define VIRT_NOT_USED()	\
int     fread(uint8_t* buff, int len){return 0;}; \
int     fwrite(const uint8_t* buff, int len){return 0;}; \
int     do_ioctl(int ctl, int val){return 0;}; \
int     do_ioctl(int ctl, uint8_t* buf, int expect){return 0;};


#define GETER_SYSCAT()  if(key[0]=='s')  return sf().c_str();   \
						if(key[0]=='k')  return __scats[peer_of()]; \
						return EMPTYS;

#define NOPOS std::string::npos
#define EMPTYS ""
#define READDEV "*"
#define IS_STR(s)  (s && *s)


#endif // PLUG_EMBIX_

