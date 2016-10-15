/*
Copyright (c) 2014-2016 PANGEEA.DPT All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/pangeea. The name of the
https://github.com/pangeea may not be used to endorse or promote
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
#define MAX_BUFF_SZ 128000

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

typedef enum EPIN_DIR
{   eIn = 0x1,
    eOut = 0x2,
    eInOut = 0x3
}EPIN_DIR;


extern  int      IDX;

extern const char* __stypes[];
extern const char* __scats[];


class Divais : public I_IDev
{
public:
	Divais(E_TYPE etype):_mon_dirt(false),_monitor(false),_etype(etype)
	{
		char t[16];
		::sprintf(t,"D_%d",++IDX);
		_name = t;
		_ukey = t;
	}
	Divais(E_TYPE etype, const char* name):_name(name?name:""),
		_mon_dirt(false),
		_monitor(false),
		_etype(etype)
	{
		char t[32];
		if(_name.empty())
		{
			::sprintf(t,"D_%d",++IDX);
			_name = t;
			_ukey = t;
		}
		else {
			_ukey = _name;
		}
	}
	virtual ~Divais();
	void  ctx_it(Sqrat::Object& o, const char* dev_key);
	const char* name()const{return _name.c_str();};
	const char* dev_key()const{return _ukey.c_str();};
	void  reset();
	bool is_monitorred(size_t t){
		if(_monitor){
			bool dirt = _touch_it(t);
			return _monitor && dirt;
		}
		return false;
	}
	void    set_name(const char* name){_name=name;}
	virtual bool iopen(int rm=0)=0;
	virtual void iclose()=0;
	virtual bool set_value(const char* key, const char* value);
	virtual const char* get_value(const char* key);
	virtual const any_t& get_data()const;
	static EPERIPH Get_cat(const char* cat);
	virtual void   sync(const char* filter);
	virtual Sqrat::Object object();
protected:
	virtual bool	_write_now(const any_t& a)=0;
	virtual size_t  _read_now(any_t& _curdata, const char* filter)=0;
	virtual bool	_set_values(const char* key, const char* value);
	virtual const char*	_get_values(const char* key);
	Sqrat::Object&	_so(){return _o;}
	virtual bool	_touch_it(size_t t)=0;
	bool			_check_dirt();

protected:
    std::string     _name;
    std::string     _ukey;
	any_t			_curdata;
	any_t			_old_data;
	std::string     _forjson;
    bool            _mon_dirt;
    bool            _monitor;
    Sqrat::Object   _o;
    E_TYPE          _etype;
private:
    Sqrat::Function  _oset_value;
    Sqrat::Function  _oget_value;
};

#define OVERW(B1,B2)															\
	void ctx_it(Sqrat::Object& o,const char* dev_key){B2::ctx_it(o, dev_key);}	\
	bool iopen(int em=O_RDWR){return B1::iopen(em);}							\
	void iclose(){B1::iclose();}												\
	void on_event(E_VENT e, const uint8_t* buff, int len, int options=0);			\
	bool _touch_it(size_t t);

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

