//
// SqModule: API used to communicate with and register squirrel modules
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

#if !defined(_SQ_MODULE_H_)
#define _SQ_MODULE_H_

#include "squirrel.h"

#if defined(_MSC_VER)
    //  Microsoft
#ifdef APPLICATION
    #define EXPORT
    #define IMPORT
#else
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#endif
#elif defined(__GNUC__)
    //  GCC
#ifdef APPLICATION
    #define EXPORT
    #define IMPORT
#else
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#endif
#else
    //  do nothing and hope for the best?
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif



#ifdef __cplusplus
extern "C" {
#endif

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @cond DEV
    /// Allows modules to interface with Squirrel's C api without linking to the squirrel library
    /// If new functions are added to the Squirrel API, they should be added here too
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    typedef struct sq_api_typedef{
        /*vm*/
        HSKVM           (*open)(isize_t initialstacksize);
        HSKVM           (*newthread)(HSKVM friendvm, isize_t initialstacksize);
        void            (*seterrorhandler)(HSKVM v);
        void            (*close)(HSKVM v);
        void            (*setforeignptr)(HSKVM v,PVOID p);
        PVOID           (*getforeignptr)(HSKVM v);
#if SQUIRREL_VERSION_NUMBER >= 300
        void            (*setprintfunc)(HSKVM v, SQPRINTFUNCTION printfunc, SQPRINTFUNCTION);
#else
        void            (*setprintfunc)(HSKVM v, SQPRINTFUNCTION printfunc);
#endif
        SQPRINTFUNCTION (*getprintfunc)(HSKVM v);
        SQRESULT        (*suspendvm)(HSKVM v);
        SQRESULT        (*wakeupvm)(HSKVM v,bool resumedret,bool retval,bool raiseerror,bool throwerror);
        isize_t             (*getvmstate)(HSKVM v);

        /*compiler*/
        SQRESULT        (*compile)(HSKVM v,SQLEXREADFUNC read,PVOID p,const SQChar *sourcename,bool raiseerror);
        SQRESULT        (*compilebuffer)(HSKVM v,const SQChar *s,isize_t size,const SQChar *sourcename,bool raiseerror);
        void            (*enabledebuginfo)(HSKVM v, bool enable);
        void            (*notifyallexceptions)(HSKVM v, bool enable);
        void            (*setcompilererrorhandler)(HSKVM v,SQCOMPILERERROR f);

        /*stack operations*/
        void            (*push)(HSKVM v,isize_t idx);
        void            (*pop)(HSKVM v,isize_t nelemstopop);
        void            (*poptop)(HSKVM v);
        void            (*remove)(HSKVM v,isize_t idx);
        isize_t             (*gettop)(HSKVM v);
        void            (*settop)(HSKVM v,isize_t newtop);
#if SQUIRREL_VERSION_NUMBER >= 300
        SQRESULT            (*reservestack)(HSKVM v,isize_t nsize);
#else
        void            (*reservestack)(HSKVM v,isize_t nsize);
#endif
        isize_t             (*cmp)(HSKVM v);
        void            (*move)(HSKVM dest,HSKVM src,isize_t idx);

        /*object creation handling*/
        PVOID           (*newuserdata)(HSKVM v,size_t size);
        void            (*newtable)(HSKVM v);
        void            (*newarray)(HSKVM v,isize_t size);
        void            (*newclosure)(HSKVM v,SQFUNCTION func,size_t nfreevars, isize_t nargs);
        void            (*newclosure_rt)(HSKVM v, SQFUNCTION_RT at,size_t nfreevars,isize_t nargs);
        SQRESULT        (*setparamscheck)(HSKVM v,isize_t nparamscheck,const SQChar *typemask);
        SQRESULT        (*bindenv)(HSKVM v,isize_t idx);
        void            (*pushstring)(HSKVM v,const SQChar *s,isize_t len);
        void            (*pushfloat)(HSKVM v,SQFloat f);
        void            (*pushinteger)(HSKVM v,isize_t n);
        void            (*pushbool)(HSKVM v,bool b);
        void            (*pushuserpointer)(HSKVM v,PVOID p);
        void            (*pushnull)(HSKVM v);
        SQObjectType    (*gettype)(HSKVM v,isize_t idx);
        isize_t       (*getsize)(HSKVM v,isize_t idx);
        SQRESULT        (*getbase)(HSKVM v,isize_t idx);
        bool          (*instanceof)(HSKVM v);
#if SQUIRREL_VERSION_NUMBER >= 300
        SQRESULT            (*tostring)(HSKVM v,isize_t idx);
#else
        void            (*tostring)(HSKVM v,isize_t idx);
#endif
        void            (*tobool)(HSKVM v, isize_t idx, bool *b);
        SQRESULT        (*getstring)(HSKVM v,isize_t idx,const SQChar **c);
        SQRESULT        (*getinteger)(HSKVM v,isize_t idx,isize_t *i);
        SQRESULT        (*getfloat)(HSKVM v,isize_t idx,SQFloat *f);
        SQRESULT        (*getbool)(HSKVM v,isize_t idx,bool *b);
        SQRESULT        (*getthread)(HSKVM v,isize_t idx,HSKVM *thread);
        SQRESULT        (*getuserpointer)(HSKVM v,isize_t idx,PVOID *p);
        SQRESULT        (*getuserdata)(HSKVM v,isize_t idx,PVOID *p,PVOID *typetag);
        SQRESULT        (*settypetag)(HSKVM v,isize_t idx,PVOID typetag);
        SQRESULT        (*gettypetag)(HSKVM v,isize_t idx,PVOID *typetag);
        SQObjectType    (*sgettype)(HSKVM v,isize_t idx);
        void            (*setreleasehook)(HSKVM v,isize_t idx,SQRELEASEHOOK hook);
        SQChar*         (*getscratchpad)(HSKVM v,isize_t minsize);
        SQRESULT        (*getclosureinfo)(HSKVM v,isize_t idx,size_t *nparams,size_t *nfreevars);
        SQRESULT        (*setnativeclosurename)(HSKVM v,isize_t idx,const SQChar *name);
        SQRESULT        (*setinstanceup)(HSKVM v, isize_t idx, PVOID p);
        SQRESULT        (*getinstanceup)(HSKVM v, isize_t idx, PVOID *p,PVOID typetag);
        SQRESULT        (*setclassudsize)(HSKVM v, isize_t idx, isize_t udsize);
        SQRESULT        (*newclass)(HSKVM v,bool hasbase);
        SQRESULT        (*createinstance)(HSKVM v,isize_t idx);
        SQRESULT        (*setattributes)(HSKVM v,isize_t idx);
        SQRESULT        (*getattributes)(HSKVM v,isize_t idx);
        SQRESULT        (*getclass)(HSKVM v,isize_t idx);
        void            (*weakref)(HSKVM v,isize_t idx);
        SQRESULT        (*getdefaultdelegate)(HSKVM v,SQObjectType t);

        /*object manipulation*/
        void            (*pushroottable)(HSKVM v);
        void            (*pushregistrytable)(HSKVM v);
        void            (*pushconsttable)(HSKVM v);
        SQRESULT        (*setroottable)(HSKVM v);
        SQRESULT        (*setconsttable)(HSKVM v);
        SQRESULT        (*newslot)(HSKVM v, isize_t idx, bool bstatic);
        SQRESULT        (*deleteslot)(HSKVM v,isize_t idx,bool pushval);
        SQRESULT        (*set)(HSKVM v,isize_t idx);
        SQRESULT        (*get)(HSKVM v,isize_t idx);
        SQRESULT        (*rawget)(HSKVM v,isize_t idx);
        SQRESULT        (*rawset)(HSKVM v,isize_t idx);
        SQRESULT        (*rawdeleteslot)(HSKVM v,isize_t idx,bool pushval);
        SQRESULT        (*arrayappend)(HSKVM v,isize_t idx);
        SQRESULT        (*arraypop)(HSKVM v,isize_t idx,bool pushval);
        SQRESULT        (*arrayresize)(HSKVM v,isize_t idx,isize_t newsize);
        SQRESULT        (*arrayreverse)(HSKVM v,isize_t idx);
        SQRESULT        (*arrayremove)(HSKVM v,isize_t idx,isize_t itemidx);
        SQRESULT        (*arrayinsert)(HSKVM v,isize_t idx,isize_t destpos);
        SQRESULT        (*setdelegate)(HSKVM v,isize_t idx);
        SQRESULT        (*getdelegate)(HSKVM v,isize_t idx);
        SQRESULT        (*clone)(HSKVM v,isize_t idx);
        SQRESULT        (*setfreevariable)(HSKVM v,isize_t idx,size_t nval);
        SQRESULT        (*next)(HSKVM v,isize_t idx);
        SQRESULT        (*getweakrefval)(HSKVM v,isize_t idx);
        SQRESULT        (*clear)(HSKVM v,isize_t idx);

        /*calls*/
        SQRESULT        (*call)(HSKVM v,isize_t params,bool retval,bool raiseerror);
        SQRESULT        (*resume)(HSKVM v,bool retval,bool raiseerror);
        const SQChar*   (*getlocal)(HSKVM v,size_t level,size_t idx);
        const SQChar*   (*getfreevariable)(HSKVM v,isize_t idx,size_t nval);
        SQRESULT        (*throwerror)(HSKVM v,const SQChar *err);
        void            (*reseterror)(HSKVM v);
        void            (*getlasterror)(HSKVM v);

        /*raw object handling*/
        SQRESULT        (*getstackobj)(HSKVM v,isize_t idx,HSQOBJECT *po);
        void            (*pushobject)(HSKVM v,HSQOBJECT obj);
        void            (*addref)(HSKVM v,HSQOBJECT *po);
        bool            (*release)(HSKVM v,HSQOBJECT *po);
        void            (*resetobject)(HSQOBJECT *po);
        const SQChar*   (*objtostring)(const HSQOBJECT *o);
        bool            (*objtobool)(const HSQOBJECT *o);
        isize_t             (*objtointeger)(const HSQOBJECT *o);
        SQFloat         (*objtofloat)(const HSQOBJECT *o);
        SQRESULT        (*getobjtypetag)(const HSQOBJECT *o,PVOID * typetag);

        /*GC*/
        isize_t             (*collectgarbage)(HSKVM v);

        /*serialization*/
        SQRESULT        (*writeclosure)(HSKVM vm,SQWRITEFUNC writef,PVOID up);
        SQRESULT        (*readclosure)(HSKVM vm,SQREADFUNC readf,PVOID up);

        /*mem allocation*/
        void*           (*malloc)(size_t size);
        void*           (*realloc)(void* p,size_t oldsize,size_t newsize);
        void            (*free)(void *p,size_t size);

        /*debug*/
        SQRESULT        (*stackinfos)(HSKVM v,isize_t level,SQStackInfos *si);
        void            (*setdebughook)(HSKVM v);


        SQRESULT        (*ptr_typeof)(HSKVM v,isize_t idx);
        SQRESULT        (*throwobject)(HSKVM v);

    } sq_api;

    typedef sq_api* HSQAPI;
    /// @endcond

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQ_MODULE_H_*/
