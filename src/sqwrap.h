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

#ifndef SQWRAP_H
#define SQWRAP_H

#include <string>
#include <stdio.h>
#include <squirrel.h>
#include <sqstdio.h>
#include <assert.h>
#include <sqvm.h>
#include <sqstdaux.h>
#include <sqstdblob.h>
#include <sqstdmath.h>
#include <sqstdsystem.h>
#include <sqstdstring.h>
#include <sqrat.h>
#include "sqratThread.h"
#include "osthread.h"

class SqEnvi;

extern SqEnvi*              __sq_env;
typedef Sqrat::Object       SqObj;
typedef Sqrat::Array        SqArr;
typedef Sqrat::Table        SqTbl;
typedef Sqrat::Function     SqMemb;
struct BaseSqEnvi;
extern BaseSqEnvi*         __bsqenv;

/**
 * @brief The EngScript class
 * a work around sq script
 */
class EngScript
{
public:
    friend class SqEnvi;
    EngScript(const EngScript& r)
    {
        _vm = r._vm;
        _obj = r._obj;
        sq_addref(_vm, &_obj);
    }
    ~EngScript()
    {
        reset();
    }

    void reset()
    {
        if(_vm && _obj._type!=OT_NULL)
            sq_release(_vm, &_obj);
        else
            if( _obj._type!=OT_NULL && _obj._unVal.pRefCounted )
                printf( "SquirrelObject::~SquirrelObject - Cannot release\n" );
        sq_resetobject(&_obj);
        int pb =  _vm->_top;
        if(pb>0)//mco-mco
            sq_pop(_vm, 1);
    }

    bool run_cmdline() // <---- FIX (from SqPLus)
    {
        Sqrat::DefaultVM::Set(_vm);
        //sq_pushobject(_vm,_obj);
        sq_pushroottable(_vm);
        if(SQ_SUCCEEDED(sq_call(_vm,1,false,false)))
        {
            //SQObject obj;
            //sq_getstackobj(_vm,-1,&obj);
            //sq_pop(_vm,2);
            return true;
        }
        sq_pop(_vm,1);
        throw Sqrat::Exception(Sqrat::LastErrorString(_vm));
        return false;//never reached
    }

    bool run_script() // <---- FIX (from SqPLus)
    {
        Sqrat::DefaultVM::Set(_vm);
        sq_pushobject(_vm,_obj);
        sq_pushroottable(_vm);
        if(SQ_SUCCEEDED(sq_call(_vm,1,true,true/*we need a return getJson*/)))
        {
            SQObject obj;
            sq_getstackobj(_vm,-1,&obj);
            sq_pop(_vm,2);
            return true;
        }
        sq_pop(_vm,1);
        throw Sqrat::Exception(Sqrat::LastErrorString(_vm));
        return false;//never reached
    }

    bool RunOld()const
    {
        Sqrat::DefaultVM::Set(_vm);

        if(!sq_isnull(_obj))
        {
            sq_pushobject(_vm, _obj);
            sq_pushroottable(_vm);
            if(SQ_FAILED(sq_call(_vm, 1, false, true)))
            {
                //return false;
                throw Sqrat::Exception(Sqrat::LastErrorString(_vm));
            }
        }
        return true;
    }
    void WriteCompiledFile(const std::string& path)
    {
        Sqrat::DefaultVM::Set(_vm);

        if(!sq_isnull(_obj))
        {
            sq_pushobject(_vm, _obj);
            sqstd_writeclosuretofile(_vm, path.c_str());
        }
    }
private:

    EngScript(HSKVM vm, const SQObject& o )
    {
        sq_resetobject(&_obj);
        _vm = vm;
        _obj = o;
        sq_addref(_vm, &_obj);
    }
    HSQOBJECT      _obj; //HSQOBJECT
    HSKVM           _vm;
};

typedef void (*HookPrint)(const SQChar*);

class SqEnvi : public BaseSqEnvi
{

public:
    SqEnvi(size_t stack=16384);
    ~SqEnvi();

    void acquire(){Sqrat::DefaultVM::Set(*_vm);}
    static void set_print_foo(HookPrint  hp);//{_hook_print = hp;};
    EngScript compile_script(const std::string& s,  const SQChar * debugInfo=_SC("console_buffer"));
    EngScript compile_buffer(const SQChar *s , size_t length, const SQChar * debugInfo=_SC("console_buffer"))const;
    EngScript compile_buffer(const std::string& s, const SQChar * debugInfo=_SC("console_buffer"))const;

    EngScript* compile_script_new(const SQChar* s,  const SQChar * debugInfo=_SC("console_buffer"))const;
    void reset();
    HSKVM theVM(){return *_vm;}
    int push_main(bool );
private:
    static void debunk_error(const std::string& err);
    static void debug_hook(HSKVM /*v*/,
                    int tip/*type*/,
                    const SQChar * s/*sourcename*/,
                    int line/*line*/,
                    const SQChar * func/*funcname*/);
    static void print_func(HSKVM, const SQChar * s,...);
    static int error_handler(HSKVM v);
    void  _init(size_t sz=16386);
private:
    HSKVM*          _vm;
    std::string     _script;
};

extern HSKVM    __vm;
#define VM()    __vm


#endif // SQWRAP_H
