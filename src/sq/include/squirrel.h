/*
Copyright (c) 2003-2017 Alberto Demichelis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef _SQUIRREL_H_
#define _SQUIRREL_H_

#include <stdint.h>
#include <stddef.h>


#ifdef _SQ_CONFIG_INCLUDE
#include _SQ_CONFIG_INCLUDE
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SQUIRREL_API
#define SQUIRREL_API extern
#endif

#if (defined(_WIN64) || defined(_LP64))
#ifndef _SQ64
#define _SQ64
#endif
#endif


#define SQTrue  (1)
#define SQFalse (0)

struct SQVM;
struct SQTable;
struct SQArray;
struct SQString;
struct SQClosure;
struct SQGenerator;
struct SQNativeClosure;
struct SQUserData;
struct SQFunctionProto;
struct SQRefCounted;
struct SQClass;
struct SQInstance;
struct SQDelegable;
struct SQOuter;

#ifdef _UNICODE
#define SQUNICODE
#endif

#include "sqconfig.h"

#define SQUIRREL_VERSION    _SC("Squirrel 3.1 stable")
#define SQUIRREL_COPYRIGHT  _SC("Copyright (C) 2003-2017 Alberto Demichelis")
#define SQUIRREL_AUTHOR     _SC("Alberto Demichelis")
#define SQUIRREL_VERSION_NUMBER 310

#define SQ_VMSTATE_IDLE         0
#define SQ_VMSTATE_RUNNING      1
#define SQ_VMSTATE_SUSPENDED    2

#define SQUIRREL_EOB 0
#define SQ_BYTECODE_STREAM_TAG  0xFAFA

#define SQOBJECT_REF_COUNTED    0x08000000
#define SQOBJECT_NUMERIC        0x04000000
#define SQOBJECT_DELEGABLE      0x02000000
#define SQOBJECT_CANBEFALSE     0x01000000

#define SQ_MATCHTYPEMASKSTRING (-99999)

#define _RT_MASK 0x00FFFFFF
#define _RAW_TYPE(type) (type&_RT_MASK)

#define _RT_NULL            0x00000001
#define _RT_INTEGER         0x00000002
#define _RT_FLOAT           0x00000004
#define _RT_BOOL            0x00000008
#define _RT_STRING          0x00000010
#define _RT_TABLE           0x00000020
#define _RT_ARRAY           0x00000040
#define _RT_USERDATA        0x00000080
#define _RT_CLOSURE         0x00000100
#define _RT_NATIVECLOSURE   0x00000200
#define _RT_GENERATOR       0x00000400
#define _RT_USERPOINTER     0x00000800
#define _RT_THREAD          0x00001000
#define _RT_FUNCPROTO       0x00002000
#define _RT_CLASS           0x00004000
#define _RT_INSTANCE        0x00008000
#define _RT_WEAKREF         0x00010000
#define _RT_OUTER           0x00020000

//mco-mco rtuntime function call



typedef enum tagSQObjectType{
    OT_NULL =           (_RT_NULL|SQOBJECT_CANBEFALSE),
    OT_INTEGER =        (_RT_INTEGER|SQOBJECT_NUMERIC|SQOBJECT_CANBEFALSE),
    OT_FLOAT =          (_RT_FLOAT|SQOBJECT_NUMERIC|SQOBJECT_CANBEFALSE),
    OT_BOOL =           (_RT_BOOL|SQOBJECT_CANBEFALSE),
    OT_STRING =         (_RT_STRING|SQOBJECT_REF_COUNTED),
    OT_TABLE =          (_RT_TABLE|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
    OT_ARRAY =          (_RT_ARRAY|SQOBJECT_REF_COUNTED),
    OT_USERDATA =       (_RT_USERDATA|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
    OT_CLOSURE =        (_RT_CLOSURE|SQOBJECT_REF_COUNTED),
    OT_NATIVECLOSURE =  (_RT_NATIVECLOSURE|SQOBJECT_REF_COUNTED),
    OT_GENERATOR =      (_RT_GENERATOR|SQOBJECT_REF_COUNTED),
    OT_USERPOINTER =    _RT_USERPOINTER,
    OT_THREAD =         (_RT_THREAD|SQOBJECT_REF_COUNTED) ,
    OT_FUNCPROTO =      (_RT_FUNCPROTO|SQOBJECT_REF_COUNTED), //internal usage only
    OT_CLASS =          (_RT_CLASS|SQOBJECT_REF_COUNTED),
    OT_INSTANCE =       (_RT_INSTANCE|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
    OT_WEAKREF =        (_RT_WEAKREF|SQOBJECT_REF_COUNTED),
    OT_OUTER =          (_RT_OUTER|SQOBJECT_REF_COUNTED) //internal usage only
}SQObjectType;

#define ISREFCOUNTED(t) (t&SQOBJECT_REF_COUNTED)


typedef union tagSQObjectValue
{
    struct SQTable *pTable;
    struct SQArray *pArray;
    struct SQClosure *pClosure;
    struct SQOuter *pOuter;
    struct SQGenerator *pGenerator;
    struct SQNativeClosure *pNativeClosure;
    struct SQString *pString;
    struct SQUserData *pUserData;
    int nInteger;
    SQFloat fFloat;
    PVOID pUserPointer;
    struct SQFunctionProto *pFunctionProto;
    struct SQRefCounted *pRefCounted;
    struct SQDelegable *pDelegable;
    struct SQVM *pThread;
    struct SQClass *pClass;
    struct SQInstance *pInstance;
    struct SQWeakRef *pWeakRef;
    SQRawObjectVal raw;
}SQObjectValue;


typedef struct tagSQObject
{
    SQObjectType _type;
    SQObjectValue _unVal;
}SQObject;

typedef struct  tagSQMemberHandle{
    bool _static;
    int _index;
}SQMemberHandle;

typedef struct tagSQStackInfos{
    const SQChar* funcname;
    const SQChar* source;
    int line;
}SQStackInfos;

typedef size_t    any_tsize;

typedef struct SQVM* HSKVM;
typedef SQObject HSQOBJECT;
typedef SQMemberHandle HSQMEMBERHANDLE;
typedef void (*ANY_FOO_TYPE)(void); //mco-mco
typedef int  (*SQFUNCTION_RT)(HSKVM, int);//mco-mco
typedef int  (*SQFUNCTION)(HSKVM);
typedef int  (*SQRELEASEHOOK)(PVOID,int size);
typedef void (*SQCOMPILERERROR)(HSKVM,const SQChar * /*desc*/,const SQChar * /*source*/,int /*line*/,int /*column*/);
typedef void (*SQPRINTFUNCTION)(HSKVM,const SQChar * ,...);
typedef void (*SQDEBUGHOOK)(HSKVM /*v*/, int /*type*/, const SQChar * /*sourcename*/, int /*line*/, const SQChar * /*funcname*/);
typedef int (*SQWRITEFUNC)(PVOID,PVOID,int);
typedef int (*SQREADFUNC)(PVOID,PVOID,int);

typedef int (*SQLEXREADFUNC)(PVOID);

typedef struct tagSQRegFunction{
    const SQChar *name;
    SQFUNCTION f;
    int nparamscheck;
    const SQChar *typemask;
}SQRegFunction;

typedef struct tagSQFunctionInfo {
    PVOID funcid;
    const SQChar *name;
    const SQChar *source;
    int line;
}SQFunctionInfo;

/*vm*/
SQUIRREL_API HSKVM sq_open(int initialstacksize);
SQUIRREL_API HSKVM sq_newthread(HSKVM friendvm, int initialstacksize);
SQUIRREL_API void sq_seterrorhandler(HSKVM v);
SQUIRREL_API void sq_close(HSKVM v);
SQUIRREL_API void sq_setforeignptr(HSKVM v,PVOID p);
SQUIRREL_API PVOID sq_getforeignptr(HSKVM v);
SQUIRREL_API void sq_setsharedforeignptr(HSKVM v,PVOID p);
SQUIRREL_API PVOID sq_getsharedforeignptr(HSKVM v);
SQUIRREL_API void sq_setvmreleasehook(HSKVM v,SQRELEASEHOOK hook);
SQUIRREL_API SQRELEASEHOOK sq_getvmreleasehook(HSKVM v);
SQUIRREL_API void sq_setsharedreleasehook(HSKVM v,SQRELEASEHOOK hook);
SQUIRREL_API SQRELEASEHOOK sq_getsharedreleasehook(HSKVM v);
SQUIRREL_API void sq_setprintfunc(HSKVM v, SQPRINTFUNCTION printfunc,SQPRINTFUNCTION errfunc);
SQUIRREL_API SQPRINTFUNCTION sq_getprintfunc(HSKVM v);
SQUIRREL_API SQPRINTFUNCTION sq_geterrorfunc(HSKVM v);
SQUIRREL_API SQRESULT sq_suspendvm(HSKVM v);
SQUIRREL_API SQRESULT sq_wakeupvm(HSKVM v,bool resumedret,bool retval,bool raiseerror,bool throwerror);
SQUIRREL_API int sq_getvmstate(HSKVM v);
SQUIRREL_API int sq_getversion();

/*compiler*/
SQUIRREL_API SQRESULT sq_compile(HSKVM v,SQLEXREADFUNC read,PVOID p,const SQChar *sourcename,bool raiseerror);
SQUIRREL_API SQRESULT sq_compilebuffer(HSKVM v,const SQChar *s,int size,const SQChar *sourcename,bool raiseerror);
SQUIRREL_API void sq_enabledebuginfo(HSKVM v, bool enable);
SQUIRREL_API void sq_notifyallexceptions(HSKVM v, bool enable);
SQUIRREL_API void sq_setcompilererrorhandler(HSKVM v,SQCOMPILERERROR f);

/*stack operations*/
SQUIRREL_API void sq_push(HSKVM v,int idx);
SQUIRREL_API void sq_pop(HSKVM v,int nelemstopop);
SQUIRREL_API void sq_poptop(HSKVM v);
SQUIRREL_API void sq_remove(HSKVM v,int idx);
SQUIRREL_API int sq_gettop(HSKVM v);
SQUIRREL_API void sq_settop(HSKVM v,int newtop);
SQUIRREL_API SQRESULT sq_reservestack(HSKVM v,int nsize);
SQUIRREL_API int sq_cmp(HSKVM v);
SQUIRREL_API void sq_move(HSKVM dest,HSKVM src,int idx);

/*object creation handling*/
SQUIRREL_API PVOID sq_newuserdata(HSKVM v,size_t size);
SQUIRREL_API void sq_newtable(HSKVM v);
SQUIRREL_API void sq_newtableex(HSKVM v,int initialcapacity);
SQUIRREL_API void sq_newarray(HSKVM v,int size);
SQUIRREL_API void sq_newclosure(HSKVM v,SQFUNCTION func,size_t nfreevars, int nargs=0);
SQUIRREL_API void sq_newclosure_rt(HSKVM v, SQFUNCTION_RT at,size_t nfreevars, int nargs); //mco-mco rt func
SQUIRREL_API SQRESULT sq_setparamscheck(HSKVM v,int nparamscheck,const SQChar *typemask);
SQUIRREL_API SQRESULT sq_bindenv(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_setclosureroot(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_getclosureroot(HSKVM v,int idx);
SQUIRREL_API void sq_pushstring(HSKVM v,const SQChar *s,int len);
SQUIRREL_API void sq_pushfloat(HSKVM v,SQFloat f);
SQUIRREL_API void sq_pushinteger(HSKVM v,int n);
SQUIRREL_API void sq_pushbool(HSKVM v,bool b);
SQUIRREL_API void sq_pushuserpointer(HSKVM v,PVOID p);
SQUIRREL_API void sq_pushnull(HSKVM v);
SQUIRREL_API void sq_pushthread(HSKVM v, HSKVM thread);
SQUIRREL_API SQObjectType sq_gettype(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_typeof(HSKVM v,int idx);
SQUIRREL_API int sq_getsize(HSKVM v,int idx);
SQUIRREL_API size_t sq_gethash(HSKVM v, int idx);
SQUIRREL_API SQRESULT sq_getbase(HSKVM v,int idx);
SQUIRREL_API bool sq_instanceof(HSKVM v);
SQUIRREL_API SQRESULT sq_tostring(HSKVM v,int idx);
SQUIRREL_API void sq_tobool(HSKVM v, int idx, bool *b);
SQUIRREL_API SQRESULT sq_getstringandsize(HSKVM v,int idx,const SQChar **c,int *size);
SQUIRREL_API SQRESULT sq_getstring(HSKVM v,int idx,const SQChar **c);
SQUIRREL_API SQRESULT sq_getinteger(HSKVM v,int idx,int *i);
SQUIRREL_API SQRESULT sq_getfloat(HSKVM v,int idx,SQFloat *f);
SQUIRREL_API SQRESULT sq_getbool(HSKVM v,int idx,bool *b);
SQUIRREL_API SQRESULT sq_getthread(HSKVM v,int idx,HSKVM *thread);
SQUIRREL_API SQRESULT sq_getuserpointer(HSKVM v,int idx,PVOID *p);
SQUIRREL_API SQRESULT sq_getuserdata(HSKVM v,int idx,PVOID *p,PVOID *typetag);
SQUIRREL_API SQRESULT sq_settypetag(HSKVM v,int idx,PVOID typetag);
SQUIRREL_API SQRESULT sq_gettypetag(HSKVM v,int idx,PVOID *typetag);
SQUIRREL_API void sq_setreleasehook(HSKVM v,int idx,SQRELEASEHOOK hook);
SQUIRREL_API SQRELEASEHOOK sq_getreleasehook(HSKVM v,int idx);
SQUIRREL_API SQChar *sq_getscratchpad(HSKVM v,int minsize);
SQUIRREL_API SQRESULT sq_getfunctioninfo(HSKVM v,int level,SQFunctionInfo *fi);
SQUIRREL_API SQRESULT sq_getclosureinfo(HSKVM v,int idx,size_t *nparams,size_t *nfreevars);
SQUIRREL_API SQRESULT sq_getclosurename(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_setnativeclosurename(HSKVM v,int idx,const SQChar *name);
SQUIRREL_API SQRESULT sq_setinstanceup(HSKVM v, int idx, PVOID p);
SQUIRREL_API SQRESULT sq_getinstanceup(HSKVM v, int idx, PVOID *p,PVOID typetag);
SQUIRREL_API SQRESULT sq_setclassudsize(HSKVM v, int idx, int udsize);
SQUIRREL_API SQRESULT sq_newclass(HSKVM v,bool hasbase);
SQUIRREL_API SQRESULT sq_createinstance(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_setattributes(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_getattributes(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_getclass(HSKVM v,int idx);
SQUIRREL_API void sq_weakref(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_getdefaultdelegate(HSKVM v,SQObjectType t);
SQUIRREL_API SQRESULT sq_getmemberhandle(HSKVM v,int idx,HSQMEMBERHANDLE *handle);
SQUIRREL_API SQRESULT sq_getbyhandle(HSKVM v,int idx,const HSQMEMBERHANDLE *handle);
SQUIRREL_API SQRESULT sq_setbyhandle(HSKVM v,int idx,const HSQMEMBERHANDLE *handle);

/*object manipulation*/
SQUIRREL_API void sq_pushroottable(HSKVM v);
SQUIRREL_API void sq_pushregistrytable(HSKVM v);
SQUIRREL_API void sq_pushconsttable(HSKVM v);
SQUIRREL_API SQRESULT sq_setroottable(HSKVM v);
SQUIRREL_API SQRESULT sq_setconsttable(HSKVM v);
SQUIRREL_API SQRESULT sq_newslot(HSKVM v, int idx, bool bstatic);
SQUIRREL_API SQRESULT sq_deleteslot(HSKVM v,int idx,bool pushval);
SQUIRREL_API SQRESULT sq_set(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_get(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_rawget(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_rawset(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_rawdeleteslot(HSKVM v,int idx,bool pushval);
SQUIRREL_API SQRESULT sq_newmember(HSKVM v,int idx,bool bstatic);
SQUIRREL_API SQRESULT sq_rawnewmember(HSKVM v,int idx,bool bstatic);
SQUIRREL_API SQRESULT sq_arrayappend(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_arraypop(HSKVM v,int idx,bool pushval);
SQUIRREL_API SQRESULT sq_arrayresize(HSKVM v,int idx,int newsize);
SQUIRREL_API SQRESULT sq_arrayreverse(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_arrayremove(HSKVM v,int idx,int itemidx);
SQUIRREL_API SQRESULT sq_arrayinsert(HSKVM v,int idx,int destpos);
SQUIRREL_API SQRESULT sq_setdelegate(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_getdelegate(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_clone(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_setfreevariable(HSKVM v,int idx,size_t nval);
SQUIRREL_API SQRESULT sq_next(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_getweakrefval(HSKVM v,int idx);
SQUIRREL_API SQRESULT sq_clear(HSKVM v,int idx);

/*calls*/
SQUIRREL_API SQRESULT sq_call(HSKVM v,int params,bool retval,bool raiseerror);
SQUIRREL_API SQRESULT sq_resume(HSKVM v,bool retval,bool raiseerror);
SQUIRREL_API const SQChar *sq_getlocal(HSKVM v,size_t level,size_t idx);
SQUIRREL_API SQRESULT sq_getcallee(HSKVM v);
SQUIRREL_API const SQChar *sq_getfreevariable(HSKVM v,int idx,size_t nval);
SQUIRREL_API SQRESULT sq_throwerror(HSKVM v,const SQChar *err);
SQUIRREL_API SQRESULT sq_throwobject(HSKVM v);
SQUIRREL_API void sq_reseterror(HSKVM v);
SQUIRREL_API void sq_getlasterror(HSKVM v);
SQUIRREL_API SQRESULT sq_tailcall(HSKVM v, int nparams);

/*raw object handling*/
SQUIRREL_API SQRESULT sq_getstackobj(HSKVM v,int idx,HSQOBJECT *po);
SQUIRREL_API void sq_pushobject(HSKVM v,HSQOBJECT obj);
SQUIRREL_API void sq_addref(HSKVM v,HSQOBJECT *po);
SQUIRREL_API bool sq_release(HSKVM v,HSQOBJECT *po);
SQUIRREL_API size_t sq_getrefcount(HSKVM v,HSQOBJECT *po);
SQUIRREL_API void sq_resetobject(HSQOBJECT *po);
SQUIRREL_API const SQChar *sq_objtostring(const HSQOBJECT *o);
SQUIRREL_API bool sq_objtobool(const HSQOBJECT *o);
SQUIRREL_API int sq_objtointeger(const HSQOBJECT *o);
SQUIRREL_API SQFloat sq_objtofloat(const HSQOBJECT *o);
SQUIRREL_API PVOID sq_objtouserpointer(const HSQOBJECT *o);
SQUIRREL_API SQRESULT sq_getobjtypetag(const HSQOBJECT *o,PVOID * typetag);
SQUIRREL_API size_t sq_getvmrefcount(HSKVM v, const HSQOBJECT *po);


/*GC*/
SQUIRREL_API int sq_collectgarbage(HSKVM v);
SQUIRREL_API SQRESULT sq_resurrectunreachable(HSKVM v);

/*serialization*/
SQUIRREL_API SQRESULT sq_writeclosure(HSKVM vm,SQWRITEFUNC writef,PVOID up);
SQUIRREL_API SQRESULT sq_readclosure(HSKVM vm,SQREADFUNC readf,PVOID up);

/*mem allocation*/
SQUIRREL_API void *sq_malloc(size_t size);
SQUIRREL_API void *sq_realloc(void* p,size_t oldsize,size_t newsize);
SQUIRREL_API void sq_free(void *p,size_t size);

/*debug*/
SQUIRREL_API SQRESULT sq_stackinfos(HSKVM v,int level,SQStackInfos *si);
SQUIRREL_API void sq_setdebughook(HSKVM v);
SQUIRREL_API void sq_setnativedebughook(HSKVM v,SQDEBUGHOOK hook);

/*UTILITY MACRO*/
#define sq_isnumeric(o) ((o)._type&SQOBJECT_NUMERIC)
#define sq_istable(o) ((o)._type==OT_TABLE)
#define sq_isarray(o) ((o)._type==OT_ARRAY)
#define sq_isfunction(o) ((o)._type==OT_FUNCPROTO)
#define sq_isclosure(o) ((o)._type==OT_CLOSURE)
#define sq_isgenerator(o) ((o)._type==OT_GENERATOR)
#define sq_isnativeclosure(o) ((o)._type==OT_NATIVECLOSURE)
#define sq_isstring(o) ((o)._type==OT_STRING)
#define sq_isinteger(o) ((o)._type==OT_INTEGER)
#define sq_isfloat(o) ((o)._type==OT_FLOAT)
#define sq_isuserpointer(o) ((o)._type==OT_USERPOINTER)
#define sq_isuserdata(o) ((o)._type==OT_USERDATA)
#define sq_isthread(o) ((o)._type==OT_THREAD)
#define sq_isnull(o) ((o)._type==OT_NULL)
#define sq_isclass(o) ((o)._type==OT_CLASS)
#define sq_isinstance(o) ((o)._type==OT_INSTANCE)
#define sq_isbool(o) ((o)._type==OT_BOOL)
#define sq_isweakref(o) ((o)._type==OT_WEAKREF)
#define sq_type(o) ((o)._type)

/* deprecated */
#define sq_createslot(v,n) sq_newslot(v,n,SQFalse)

#define SQ_OK (0)
#define SQ_ERROR (-1)

#define SQ_FAILED(res) (res<0)
#define SQ_SUCCEEDED(res) (res>=0)

#ifdef __GNUC__
# define SQ_UNUSED_ARG(x) x __attribute__((__unused__))
#else
# define SQ_UNUSED_ARG(x) x
#endif


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQUIRREL_H_*/
