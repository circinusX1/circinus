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

#ifndef INPUSYS_H
#define INPUSYS_H

#include <unistd.h>
#include <linux/input.h>
#include <fcntl.h>
#include "divais.h"
#include "sq_engine.h"
#include "inst.h"
#include "rtxbus.h"

using namespace GenericHw;

typedef enum E_INPUT{
    eKEYBOARD,
    eMOUSE,
    eTOUCH,
    eJOYSTIC,
}E_INPUT;

class InputSys:   public Divais,
                  private Reg<InputSys>
{
public:

    InputSys(E_INPUT e, const char* device, const char* name=nullptr);
    InputSys(SqObj&, E_INPUT e, const char* device, const char* name=nullptr);
    virtual ~InputSys();
    bool   set_cb(SqMemb& mem);
    bool   lopen(int);
    void   lclose();
    Sqrat::Array get();
    Sqrat::Array get_mouse();
    Sqrat::Array get_keys();
    Sqrat::Array get_touch();
    OVERW(InputSys,Divais)
    static void sqconsts(SqEnvi& e);
    static void squit(SqEnvi& e){
        InputSys::sqconsts(e);
        Sqrat::Class<InputSys> cls(e.theVM(), _SC("SYSIN"));
        cls.Ctor<E_INPUT, const char* , const char*>();
        cls.Ctor<SqObj&, E_INPUT, const char* , const char*>();

        cls.Functor(_SC("set_cb"), &InputSys::set_cb);
        cls.Functor(_SC("open"), &InputSys::lopen);
        cls.Functor(_SC("close"), &InputSys::lclose);
        cls.Functor("get",&InputSys::get);
        cls.Functor("get_mouse",&InputSys::get_mouse);
        cls.Functor("get_keys",&InputSys::get_keys);
        cls.Functor("get_touch",&InputSys::get_touch);

        cls.Functor(_SC("plug_it"), &InputSys::plug_it);
        cls.Functor(_SC("get_name"), &InputSys::get_label_name);

        Sqrat::RootTable().Bind(_SC("SYSIN"), cls);
    }

protected:
    bool    _write_now(const devdata_t& vl);
    size_t  _fecth(devdata_t& vl, const char* filter);
    bool                _set_value(const char* key, const char* value);
    const char*         _get_values(const char* key);

    int                 _ceil(int val, int div);
private:
	struct cursor_t {
		int x, y;
		bool l;
		bool m;
		bool r;
	};
	cursor_t    _current;
	struct  {
		int fd;
		unsigned char *fp;
		int width, height;   /* display resolution */
		long screen_size;    /* screen data size (byte) */
		int line_length;     /* line length (byte) */
		int bytes_per_pixel;
		int bits_per_pixel;
	}			_fb;
	int			_fd=0;
	std::string  _sysfile;
	struct		 input_event _ie;
	E_INPUT		 _edev;
	int			 _keys[256];
};

#endif // SPIDEV_H
