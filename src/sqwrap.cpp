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

#include <assert.h>
#include <stdarg.h>
#include <iostream>
#include <dlfcn.h>
#include <string>
#include "sqwrap.h"
#include "osthread.h"
#include "logs.h"
#include "dlconfig.h"
#include "psingle.h"


//static __thread HookPrint _hook_print;
//static __thread HSKVM __vm;

#define BGAP    32

typedef const char  khar;
static __thread    HookPrint    _hook_print;
HSKVM  __thread     __vm;
extern std::string  SqErrStr;

class               SqEnvi;
SqEnvi*             __sq_env;
BaseSqEnvi*         __bsqenv;

/*static*/
void SqEnvi::set_print_foo(HookPrint  hp)
{
    _hook_print = hp;
}

SqEnvi::SqEnvi(size_t sz):_vm(0)
{
    _init(sz);
    acquire();
}

SqEnvi::~SqEnvi()
{
    __sq_env = nullptr;
    __bsqenv = nullptr;
    if(*_vm){
        Sqrat::DefaultVM::Set(*_vm);
        sq_close(*_vm);
        *_vm = 0;
    }
    _close_sos();
}

void SqEnvi::_close_sos()
{

    for(auto& a : _dlls)
        ::dlclose(a);
}

void SqEnvi::_init(size_t sz)
{
    //AutoLock a(&_m);
    __vm = sq_open(sz);
    _vm= &__vm;

    assert( _vm );
    Sqrat::DefaultVM::Set(*_vm);
    sq_setprintfunc(*_vm, SqEnvi::print_func, SqEnvi::print_func);
    sq_newclosure(*_vm, SqEnvi::error_handler,0);
    sq_seterrorhandler(*_vm);
    //sq
    sq_pushroottable(*_vm);
    sqstd_register_iolib(*_vm);
    sqstd_register_bloblib(*_vm);
    sqstd_register_mathlib(*_vm);
    sqstd_register_stringlib(*_vm);
    sqstd_register_systemlib(*_vm);

    sqstd_seterrorhandlers(*_vm);
    //sqstd_printcallstack(*_vm);
    //    setnativedebughook(_vmsys,debug_hook);
    sq_notifyallexceptions(*_vm, true);
    __sq_env      = this;
    __bsqenv      = this;

}

void SqEnvi::debug_hook(HSKVM /*v*/,
                        int tip/*type*/,
                        const SQChar * s/*sourcename*/,
                        int line/*line*/,
                        const SQChar * func/*funcname*/)
{
    //AutoLock a(&_m);
    char buff[8912];
    snprintf (buff,8911, "%d: %d  %s, %s\n",(int)tip, (int)line, func,s);
    if(_hook_print){
        _hook_print(buff);
    }else{
        std::cout << buff;
        std::cout.flush();
    }
    SqErrStr += buff;
}

void SqEnvi::print_func(HSKVM, const SQChar * s,...)
{
    char szBuffer[2048];

    va_list args;
    va_start(args, s);
    ::vsnprintf(szBuffer,sizeof(szBuffer)-1, s, args);
    va_end(args);
    SLOG(szBuffer);
    std::cout.flush();
}

isize_t SqEnvi::error_handler(HSKVM v)
{
    //AutoLock a(&_m);
    const SQChar *sErr = 0;
    if(sq_gettop(v)>=1)
    {
        if(SQ_SUCCEEDED(sq_getstring(v,2,&sErr)))
        {
            LOGEX("A Script Error Occured: " << sErr);
        }
        else
        {
            LOGEX("Unknown Error Occured");
        }
    }
    return 0;
}

EngScript* SqEnvi::compile_script_new(const SQChar* s,  const SQChar * debugInfo)const
{
    //AutoLock a(&_m);
    Sqrat::DefaultVM::Set(*_vm);

    (void)debugInfo;
    SQObject obj;
    if(SQ_FAILED(sqstd_loadfile(*_vm, s, true)))
    {
        //return false;
        throw Sqrat::Exception(Sqrat::LastErrorString(*_vm));
    }
    sq_getstackobj(*_vm,-1,&obj);
    //sq_pop(_vm,1); //mco
    return new EngScript(*_vm, obj);
}

int SqEnvi::push_main(bool call)
{
    if(call==false)
    {
        const SQChar* main_foo = _SC(const_cast<const SQChar*>("context()._sync();main(context());"));
        if(SQ_FAILED(SQ_PTRS->compilebuffer(theVM(),
                                            main_foo,
                                            ::strlen(main_foo), _SC(""), true)))
        {
            return SQ_ERROR;
        }
        return 0;
    }
    SQ_PTRS->pushroottable(theVM());
    SQ_PTRS->call(theVM(), 1, 0, 1);
    SQ_PTRS->remove(theVM(), -1);
    return 0;
}


EngScript SqEnvi::compile_script(const std::string& s,  const SQChar * debugInfo)
{
    char cwd[PATH_MAX+BGAP];
    char cwdf[PATH_MAX+BGAP];
    Sqrat::DefaultVM::Set(*_vm);

    _script = s;
    (void)debugInfo;
    SQObject obj;

#if 1
    ::getcwd(cwd, PATH_MAX-1);
    ::sprintf(cwdf,"%s/%s",cwd,s.c_str());
    if(::access(cwdf,0)!=0)
    {
        LOGE("Cannot find: " << cwdf);
    }
    sprintf(cwd,"cp %s /tmp/os.embix && echo 'context()._sync();main(context());' >> /tmp/os.embix ", cwdf);
    ::system(cwd);
    if(SQ_FAILED(sqstd_loadfile(*_vm, "/tmp/os.embix", true)))
    {
        throw Sqrat::Exception(Sqrat::LastErrorString(*_vm));
    }
#else
    if(::access(_script.c_str(),0)!=0)
    {
        LOGE("Cannot find: " << _script);
    }
    push_main(false);
    if(SQ_FAILED(sqstd_loadfile(*_vm, _script.c_str(), true)))
    {
        throw Sqrat::Exception(Sqrat::LastErrorString(*_vm));
    }
    push_main(true);

#endif
    sq_getstackobj(*_vm,-1,&obj);
    //sq_pop(_vm,1); //mco
    return EngScript(*_vm, obj);
}

EngScript SqEnvi::compile_buffer(const SQChar *s,  size_t length, const SQChar * debugInfo)const
{
    //AutoLock a(&_m);
    Sqrat::DefaultVM::Set(*_vm);

    SQObject obj;
    if(SQ_FAILED(sq_compilebuffer(*_vm, s,
                                  static_cast<int>(length),
                                  debugInfo, true)))
    {
        throw Sqrat::Exception(Sqrat::LastErrorString(*_vm));
    }
    sq_getstackobj(*_vm,-1,&obj);
    return EngScript(*_vm, obj);
}

EngScript SqEnvi::compile_buffer(const std::string& s, const SQChar * debugInfo)const
{
    return SqEnvi::compile_buffer(_SC(s.c_str()), s.length(), debugInfo);
}

void SqEnvi::reset()
{
    //AutoLock a(&_m);
    sq_pop(*_vm, 1);
    sq_close(*this->_vm);
    this->_init();
}

void SqEnvi::debunk_error(const std::string& err)
{
}

