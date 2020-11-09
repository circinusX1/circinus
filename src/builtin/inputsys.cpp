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


#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <linux/fb.h>
#include <sys/mman.h>

#include "inputsys.h"
#include "inst.h"
#include "dlconfig.h"

/**
 *evtest /dev/input/event8
 */
enum {
	VERBOSE    = true,
	VALUE_PRESSED  = 1,
	VALUE_RELEASED = 0,
	BITS_PER_BYTE  = 8,
	CURSOR_COLOR   = 0x00FF00,
};

InputSys::InputSys(E_INPUT e, const char* device, const char* name):
    Divais (eBINARY, eSPI, name),Reg<InputSys>(this),_sysfile(device),_edev(e)
{
    _o.BindCppObject(this);

}

InputSys::InputSys(SqObj& o,
                   E_INPUT e,
                   const char* device,
                   const char* name):
    Divais (eBINARY, eSPI, name),Reg<InputSys>(this),_sysfile(device),_edev(e)
{
    regiter_it(o, name);
}


InputSys::~InputSys()
{
	lclose();
}

bool   InputSys::lopen(int)
{
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;

	if ((_fb.fd = open("/dev/fb0", O_RDWR)) < 0) {
		LOGE(strerror(errno));
		return false;
		::close(_fb.fd);
	}

	if (ioctl(_fb.fd, FBIOGET_FSCREENINFO, &finfo)) {
		LOGE(strerror(errno));
		::close(_fb.fd);
		return false;
	}

	if (ioctl(_fb.fd, FBIOGET_VSCREENINFO, &vinfo)) {
		LOGE(strerror(errno));
		::close(_fb.fd);
		return false;
	}

	_fb.width  = vinfo.xres;
	_fb.height = vinfo.yres;
	_fb.screen_size = finfo.smem_len;
	_fb.line_length = finfo.line_length;
	_fb.bits_per_pixel  = vinfo.bits_per_pixel;
	_fb.bytes_per_pixel = _ceil(_fb.bits_per_pixel, BITS_PER_BYTE);

	_current.x  = _current.y  = 0;
	_current.l = 0;
	_current.m = 0;
	_current.r = 0;

	::close(_fb.fd);
	_fb.fd = 0;

	_fd = ::open(_sysfile.c_str(),O_RDONLY);
	if(_fd<=0)
		LOGE(__FUNCTION__ << strerror(errno));
	return _fd > 0;
}

void   InputSys::lclose()
{
	if(_fd)
		::close(_fd);
	_fd = 0;
}

bool  InputSys::_write_now(const any_t& vl)
{
    _mon_dirt = true;
    return false;
}

Sqrat::Array InputSys::get()
{
    SqArr rar(App->psqvm(),5); // timems, timeus, type, kode, value

    rar.SetValue(0, _ie.type);
    rar.SetValue(1, _ie.code);
    rar.SetValue(2, _ie.value);
    rar.SetValue(3, _ie.time.tv_sec);
    rar.SetValue(4, _ie.time.tv_usec);
    return rar;
}

size_t  InputSys::_fecth(any_t& vl, const char* filter)
{
    if(_fd>0)
    {
        timeval  tv    = {0, 1000};
        fd_set   rdSet;

        FD_ZERO(&rdSet);
        FD_SET(_fd, &rdSet);
        int nfds = (int)_fd+1;

        int sel = ::select(nfds, &rdSet, 0, 0, &tv);
        if(sel > 0 && FD_ISSET(_fd, &rdSet))
        {
            int bytes = read(_fd, &_ie, sizeof(struct input_event));
            if(bytes == sizeof(struct input_event))
            {
                if (_ie.code == REL_X)
                {
                    _current.x += _ie.value;

                    if (_current.x < 0)
                        _current.x = 0;
                    else if (_current.x >= _fb.width)
                        _current.x = _fb.width - 1;
                }
                if (_ie.code == REL_Y)
                    _current.y += _ie.value;
                {

                    if (_current.y < 0)
                        _current.y = 0;
                    else if (_current.y >= _fb.height)
                        _current.y = _fb.height - 1;
                }
                if(_ie.type == BTN_LEFT)
                    _current.l=_ie.value;
                if(_ie.type== BTN_RIGHT)
                    _current.r=_ie.value;
                if(_ie.type== BTN_MIDDLE)
                    _current.m=_ie.value;

                vl.typeit(eSTRUCT,0);
                vl.let((const uint8_t*)&_ie, sizeof(_ie));
                if(_monitor)
                    _mon_dirt=_check_dirt();
                return sizeof(struct input_event);
            }
        }
    }
    return 0;
}

//L M R X Y
Sqrat::Array InputSys::get_mouse()
{
    SqArr rar(App->psqvm(),5); // timems, timeus, type, kode, value

    rar.SetValue(0, _current.x);
    rar.SetValue(1, _current.y);
    rar.SetValue(2, _current.l);
    rar.SetValue(3, _current.m);
    rar.SetValue(4, _current.r);
    return rar;
}

Sqrat::Array InputSys::get_keys()
{
    SqArr rar(App->psqvm(),5); // timems, timeus, type, kode, value

    rar.SetValue(0, _ie.type);
    rar.SetValue(1, _ie.code);
    rar.SetValue(2, _ie.value);
    rar.SetValue(3, _ie.time.tv_sec);
    rar.SetValue(4, _ie.time.tv_usec);
    return rar;
}

Sqrat::Array InputSys::get_touch()
{
    SqArr rar(App->psqvm(),5); // timems, timeus, type, kode, value

    rar.SetValue(0, _ie.type);
    rar.SetValue(1, _ie.code);
    rar.SetValue(2, _ie.value);
    rar.SetValue(3, _ie.time.tv_sec);
    rar.SetValue(4, _ie.time.tv_usec);
    return rar;
}

bool InputSys::_mon_pick(size_t t)
{
    _fecth(_curdata,nullptr);
    bool rv = _mon_dirt;
    return rv;
}

bool InputSys::call_back(SqMemb& m)
{
    if(m.IsNull()){
        _monitor = false;
        if(!_on_event.IsNull())
            _on_event.Release();
    }else{
        _on_event=m;
        _monitor = true;
    }
    return _monitor;
}

void InputSys::on_event(E_VENT e, const uint8_t* buff, int len, int options)
{
}

// 3,0x55,0c55,0x56,0x55
bool	InputSys::_set_values(const char* key, const char* value)
{
    return false;
}

const char*	InputSys::_get_values(const char* key)
{
    char loco[64];

    _forjson += "time=";
    ::sprintf(loco,"%ld.%06ld", _ie.time.tv_sec, _ie.time.tv_usec);
    _forjson += loco;
    _forjson += "&type=";
    ::sprintf(loco,"%d", _ie.type);
    _forjson += loco;
    _forjson += "&code=";
    ::sprintf(loco,"%d", _ie.code);
    _forjson += loco;
    ::sprintf(loco,"%d", _ie.value);
    _forjson += "&value=";
    _forjson += loco;
    return _forjson.c_str();
}

int InputSys::_ceil(int val, int div)
{
    if (div == 0)
        return 0;
    else
        return (val + div - 1) / div;
}

void InputSys::sqconsts(SqEnvi& e)
{
	Sqrat::ConstTable(e.theVM())
			.Const("EV_SYN",EV_SYN)
			.Const("EV_KEY",EV_KEY)
			.Const("EV_REL",EV_REL)
			.Const("EV_ABS",EV_ABS)
			.Const("EV_MSC",EV_MSC)
			.Const("EV_SW",EV_SW)
			.Const("EV_LED",EV_LED)
			.Const("EV_SND",EV_SND)
			.Const("EV_REP",EV_REP)
			.Const("EV_FF",EV_FF)
			.Const("EV_PWR",EV_PWR)
			.Const("EV_FF_STATUS",EV_FF_STATUS)
			.Const("EV_MAX",EV_MAX)
			.Const("EV_CNT",EV_CNT);
}
