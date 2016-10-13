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

//
// SqratImport: Supports importing of squirrel modules
//

//
// Copyright (c) 2009 Brandon Jones
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//  distribution.
//
//  COM 2019-Nov-22, Rat moved to use pointers to functions
#include <assert.h>
#include "sqratimport.h"
#include "sqmodule.h"

//#include "sqratlib/sqratBase.h"
#include <sqstdio.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "logs.h"
#include "inst.h"

#if defined(_WIN32)

#include <windows.h>

#elif defined(__unix)

#include <dlfcn.h>

#endif

typedef SQRESULT (*SQMODULELOAD)(HSKVM v, HSQAPI sq);
static bool   _gotptrs;
static sq_api _sqapi;

// Create and populate the HSQAPI structure with function pointers
// If new functions are added to the Squirrel API, they should be added here too
bool sqrat_newapi(sq_api** sq)
{
    *sq = &_sqapi;
    /* all vm functors */
    _sqapi.open = sq_open;
    _sqapi.newthread = sq_newthread;
    _sqapi.seterrorhandler = sq_seterrorhandler;
    _sqapi.close = sq_close;
    _sqapi.setforeignptr = sq_setforeignptr;
    _sqapi.getforeignptr = sq_getforeignptr;
    _sqapi.setprintfunc = sq_setprintfunc;
    _sqapi.getprintfunc = sq_getprintfunc;
    _sqapi.suspendvm = sq_suspendvm;
    _sqapi.wakeupvm = sq_wakeupvm;
    _sqapi.getvmstate = sq_getvmstate;

    /*compiler*/
    _sqapi.compile = sq_compile;
    _sqapi.compilebuffer = sq_compilebuffer;
    _sqapi.enabledebuginfo = sq_enabledebuginfo;
    _sqapi.notifyallexceptions = sq_notifyallexceptions;
    _sqapi.setcompilererrorhandler = sq_setcompilererrorhandler;

    /*stack operations*/
    _sqapi.push = sq_push;
    _sqapi.pop = sq_pop;
    _sqapi.poptop = sq_poptop;
    _sqapi.remove = sq_remove;
    _sqapi.gettop = sq_gettop;
    _sqapi.settop = sq_settop;
    _sqapi.reservestack = sq_reservestack;
    _sqapi.cmp = sq_cmp;
    _sqapi.move = sq_move;

    /*object creation handling*/
    _sqapi.newuserdata = sq_newuserdata;
    _sqapi.newtable = sq_newtable;
    _sqapi.newarray = sq_newarray;
    _sqapi.newclosure = sq_newclosure;
    _sqapi.newclosure_rt = sq_newclosure_rt;
    _sqapi.setparamscheck = sq_setparamscheck;
    _sqapi.bindenv = sq_bindenv;
    _sqapi.pushstring = sq_pushstring;
    _sqapi.pushfloat = sq_pushfloat;
    _sqapi.pushinteger = sq_pushinteger;
    _sqapi.pushbool = sq_pushbool;
    _sqapi.pushuserpointer = sq_pushuserpointer;
    _sqapi.pushnull = sq_pushnull;
    _sqapi.gettype = sq_gettype;
    _sqapi.getsize = sq_getsize;
    _sqapi.getbase = sq_getbase;
    _sqapi.instanceof = sq_instanceof;
    _sqapi.tostring = sq_tostring;
    _sqapi.tobool = sq_tobool;
    _sqapi.getstring = sq_getstring;
    _sqapi.getinteger = sq_getinteger;
    _sqapi.getthread = sq_getthread;
    _sqapi.getbool = sq_getbool;
    _sqapi.getuserpointer = sq_getuserpointer;
    _sqapi.getuserdata = sq_getuserdata;
    _sqapi.settypetag = sq_settypetag;
    _sqapi.gettypetag = sq_gettypetag;
    _sqapi.setreleasehook = sq_setreleasehook;
    _sqapi.getscratchpad = sq_getscratchpad;
    _sqapi.getclosureinfo = sq_getclosureinfo;
    _sqapi.setnativeclosurename = sq_setnativeclosurename;
    _sqapi.setinstanceup = sq_setinstanceup;
    _sqapi.getinstanceup = sq_getinstanceup;
    _sqapi.setclassudsize = sq_setclassudsize;
    _sqapi.newclass = sq_newclass;
    _sqapi.createinstance = sq_createinstance;
    _sqapi.setattributes = sq_setattributes;
    _sqapi.getattributes = sq_getattributes;
    _sqapi.getclass = sq_getclass;
    _sqapi.weakref = sq_weakref;
    _sqapi.getdefaultdelegate = sq_getdefaultdelegate;

    /*object manipulation*/
    _sqapi.pushroottable = sq_pushroottable;
    _sqapi.pushregistrytable = sq_pushregistrytable;
    _sqapi.pushconsttable = sq_pushconsttable;
    _sqapi.setroottable = sq_setroottable;
    _sqapi.setconsttable = sq_setconsttable;
    _sqapi.newslot = sq_newslot;
    _sqapi.deleteslot = sq_deleteslot;
    _sqapi.set = sq_set;
    _sqapi.get = sq_get;
    _sqapi.rawset = sq_rawset;
    _sqapi.rawget = sq_rawget;
    _sqapi.rawdeleteslot = sq_rawdeleteslot;
    _sqapi.arrayappend = sq_arrayappend;
    _sqapi.arraypop = sq_arraypop;
    _sqapi.arrayresize = sq_arrayresize;
    _sqapi.arrayreverse = sq_arrayreverse;
    _sqapi.arrayremove = sq_arrayremove;
    _sqapi.arrayinsert = sq_arrayinsert;
    _sqapi.setdelegate = sq_setdelegate;
    _sqapi.getdelegate = sq_getdelegate;
    _sqapi.clone = sq_clone;
    _sqapi.setfreevariable = sq_setfreevariable;
    _sqapi.next = sq_next;
    _sqapi.getweakrefval = sq_getweakrefval;
    _sqapi.clear = sq_clear;

    /*calls*/
    _sqapi.call = sq_call;
    _sqapi.resume = sq_resume;
    _sqapi.getlocal = sq_getlocal;
    _sqapi.getfreevariable = sq_getfreevariable;
    _sqapi.throwerror = sq_throwerror;
    _sqapi.reseterror = sq_reseterror;
    _sqapi.getlasterror = sq_getlasterror;

    /*raw object handling*/
    _sqapi.getstackobj = sq_getstackobj;
    _sqapi.pushobject = sq_pushobject;
    _sqapi.addref = sq_addref;
    _sqapi.release = sq_release;
    _sqapi.resetobject = sq_resetobject;
    _sqapi.objtostring = sq_objtostring;
    _sqapi.objtobool = sq_objtobool;
    _sqapi.objtointeger = sq_objtointeger;
    _sqapi.objtofloat = sq_objtofloat;
    _sqapi.getobjtypetag = sq_getobjtypetag;

    /*GC*/
    _sqapi.collectgarbage = sq_collectgarbage;

    /*serialization*/
    _sqapi.writeclosure = sq_writeclosure;
    _sqapi.readclosure = sq_readclosure;

    /*mem allocation*/
    _sqapi.malloc = sq_malloc;
    _sqapi.realloc = sq_realloc;
    _sqapi.free = sq_free;

    /*debug*/
    _sqapi.stackinfos = sq_stackinfos;
    _sqapi.setdebughook = sq_setdebughook;

    _sqapi.ptr_typeof = sq_typeof;
    _sqapi.throwobject = sq_throwobject;
    return _gotptrs = true;
}


static SQRESULT sqrat_importscript(HSKVM v, const SQChar* moduleName) {
    std::basic_string<SQChar> filename(moduleName);
    //filename += _SC(".nut");
    if(SQ_FAILED(sqstd_loadfile(v, moduleName, true))) {
        if(SQ_FAILED(sqstd_loadfile(v, filename.c_str(), true))) {
            return SQ_ERROR;
        }
    }
    sq_push(v, -2);
    sq_call(v, 1, false, true);
    return SQ_OK;
}

static SQRESULT sqrat_importbin(HSKVM v, const SQChar* moduleName) {
#ifdef SQUNICODE
#warning sqrat_importbin() Not Implemented
    return SQ_ERROR;
#else
    SQMODULELOAD modLoad = 0;

#if defined(_WIN32)
    HMODULE mod;
    mod = GetModuleHandle(moduleName);
    if(mod == NULL) {
        mod = LoadLibrary(moduleName);
        if(mod == NULL) {
            return SQ_ERROR;
        }
    }

    modLoad = (SQMODULELOAD)GetProcAddress(mod, "sqmodule_load");
    if(modLoad == NULL) {
        FreeLibrary(mod);
        return SQ_ERROR;
    }
#elif defined(__unix)
    /* adding .so to moduleName? */
    char l[256];
    getcwd(l,255);

    LOGI("Loading: " << moduleName);
    if(access(moduleName,0)!=0)
    {
        sprintf(l,"%s/%s",l,moduleName);
        if(access(l,0)!=0)
        {
            LOGD1("No file: " << l);
            return SQ_ERROR;
        }
    }
    else {
        sprintf(l,"./%s",moduleName);
    }
    void *mod = dlopen(l, RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD); //RTLD_NOLOAD flag is not specified in POSIX.1-2001..so not the best solution :(
    if (mod == NULL) {
        mod = dlopen(l, RTLD_NOW | RTLD_LOCAL);
        if (mod == NULL)
        {
            LOGE("cannot load DLL" << __FUNCTION__ << dlerror());
            return SQ_ERROR;
        }
    }
    modLoad = (SQMODULELOAD) dlsym(mod, "sqmodule_load");
    if (modLoad == NULL)
    {
        LOGE("cannot load sqmodule_load" << __FUNCTION__ << dlerror());
        dlclose(mod);
        return SQ_ERROR;
    }
#endif

    assert(_gotptrs);
    SQRESULT res = modLoad(v, &_sqapi);
    if(SQ_OK == res)
    {
        App->add_dll(mod);
    }
    else
    {
        LOGE("sqmodule_load failed...");
        dlclose(mod);
    }
    LOGI("Module: " << l << " loaded: return code: " << res);
    return res;
#endif
}

SQRESULT sqrat_import(HSKVM v) {
    const SQChar* moduleName;
    HSQOBJECT table;
    SQRESULT res = SQ_OK;


    sq_getstring(v, -2, &moduleName);
    sq_getstackobj(v, -1, &table);
    sq_addref(v, &table);

    sq_settop(v, 0); // Clear Stack
    sq_pushobject(v, table); // Push the target table onto the stack

    if(SQ_FAILED(sqrat_importscript(v, moduleName))) {
        res = sqrat_importbin(v, moduleName);
    }

    sq_settop(v, 0); // Clean up the stack (just in case the module load leaves it messy)
    sq_pushobject(v, table); // return the target table
    sq_release(v, &table);
    
    return res;
}

static int sqratbase_import(HSKVM v) {
    int args = sq_gettop(v);
    switch(args) {
    case 2:
        sq_pushroottable(v);
        break;
    case 3:
        // should already have the desired table pushed onto the stack
        break;
    default:
        // Error, unexpected number of arguments
        break;
    }
    sqrat_import(v);
    return 1;
}

SQRESULT sqrat_register_importlib(HSKVM v) {
    sq_pushroottable(v);

    sq_pushstring(v, _SC("import"), -1);
    sq_newclosure(v, &sqratbase_import, 0);
    sq_newslot(v, -3, 0);
    sq_pop(v, 1); // pop sqrat table
    return SQ_OK;
}
