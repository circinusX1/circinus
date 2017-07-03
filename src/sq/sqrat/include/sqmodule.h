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
        HSKVM           (*open)(int initialstacksize);
        HSKVM           (*newthread)(HSKVM friendvm, int initialstacksize);
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
        int             (*getvmstate)(HSKVM v);

        /*compiler*/
        SQRESULT        (*compile)(HSKVM v,SQLEXREADFUNC read,PVOID p,const SQChar *sourcename,bool raiseerror);
        SQRESULT        (*compilebuffer)(HSKVM v,const SQChar *s,int size,const SQChar *sourcename,bool raiseerror);
        void            (*enabledebuginfo)(HSKVM v, bool enable);
        void            (*notifyallexceptions)(HSKVM v, bool enable);
        void            (*setcompilererrorhandler)(HSKVM v,SQCOMPILERERROR f);

        /*stack operations*/
        void            (*push)(HSKVM v,int idx);
        void            (*pop)(HSKVM v,int nelemstopop);
        void            (*poptop)(HSKVM v);
        void            (*remove)(HSKVM v,int idx);
        int             (*gettop)(HSKVM v);
        void            (*settop)(HSKVM v,int newtop);
#if SQUIRREL_VERSION_NUMBER >= 300
        SQRESULT            (*reservestack)(HSKVM v,int nsize);
#else
        void            (*reservestack)(HSKVM v,int nsize);
#endif
        int             (*cmp)(HSKVM v);
        void            (*move)(HSKVM dest,HSKVM src,int idx);

        /*object creation handling*/
        PVOID           (*newuserdata)(HSKVM v,size_t size);
        void            (*newtable)(HSKVM v);
        void            (*newarray)(HSKVM v,int size);
        void            (*newclosure)(HSKVM v,SQFUNCTION func,size_t nfreevars, int nargs);
        void            (*newclosure_rt)(HSKVM v, SQFUNCTION_RT at,size_t nfreevars,int nargs);
        SQRESULT        (*setparamscheck)(HSKVM v,int nparamscheck,const SQChar *typemask);
        SQRESULT        (*bindenv)(HSKVM v,int idx);
        void            (*pushstring)(HSKVM v,const SQChar *s,int len);
        void            (*pushfloat)(HSKVM v,SQFloat f);
        void            (*pushinteger)(HSKVM v,int n);
        void            (*pushbool)(HSKVM v,bool b);
        void            (*pushuserpointer)(HSKVM v,PVOID p);
        void            (*pushnull)(HSKVM v);
        SQObjectType    (*gettype)(HSKVM v,int idx);
        int       (*getsize)(HSKVM v,int idx);
        SQRESULT        (*getbase)(HSKVM v,int idx);
        bool          (*instanceof)(HSKVM v);
#if SQUIRREL_VERSION_NUMBER >= 300
        SQRESULT            (*tostring)(HSKVM v,int idx);
#else
        void            (*tostring)(HSKVM v,int idx);
#endif
        void            (*tobool)(HSKVM v, int idx, bool *b);
        SQRESULT        (*getstring)(HSKVM v,int idx,const SQChar **c);
        SQRESULT        (*getinteger)(HSKVM v,int idx,int *i);
        SQRESULT        (*getfloat)(HSKVM v,int idx,SQFloat *f);
        SQRESULT        (*getbool)(HSKVM v,int idx,bool *b);
        SQRESULT        (*getthread)(HSKVM v,int idx,HSKVM *thread);
        SQRESULT        (*getuserpointer)(HSKVM v,int idx,PVOID *p);
        SQRESULT        (*getuserdata)(HSKVM v,int idx,PVOID *p,PVOID *typetag);
        SQRESULT        (*settypetag)(HSKVM v,int idx,PVOID typetag);
        SQRESULT        (*gettypetag)(HSKVM v,int idx,PVOID *typetag);
        void            (*setreleasehook)(HSKVM v,int idx,SQRELEASEHOOK hook);
        SQChar*         (*getscratchpad)(HSKVM v,int minsize);
        SQRESULT        (*getclosureinfo)(HSKVM v,int idx,size_t *nparams,size_t *nfreevars);
        SQRESULT        (*setnativeclosurename)(HSKVM v,int idx,const SQChar *name);
        SQRESULT        (*setinstanceup)(HSKVM v, int idx, PVOID p);
        SQRESULT        (*getinstanceup)(HSKVM v, int idx, PVOID *p,PVOID typetag);
        SQRESULT        (*setclassudsize)(HSKVM v, int idx, int udsize);
        SQRESULT        (*newclass)(HSKVM v,bool hasbase);
        SQRESULT        (*createinstance)(HSKVM v,int idx);
        SQRESULT        (*setattributes)(HSKVM v,int idx);
        SQRESULT        (*getattributes)(HSKVM v,int idx);
        SQRESULT        (*getclass)(HSKVM v,int idx);
        void            (*weakref)(HSKVM v,int idx);
        SQRESULT        (*getdefaultdelegate)(HSKVM v,SQObjectType t);

        /*object manipulation*/
        void            (*pushroottable)(HSKVM v);
        void            (*pushregistrytable)(HSKVM v);
        void            (*pushconsttable)(HSKVM v);
        SQRESULT        (*setroottable)(HSKVM v);
        SQRESULT        (*setconsttable)(HSKVM v);
        SQRESULT        (*newslot)(HSKVM v, int idx, bool bstatic);
        SQRESULT        (*deleteslot)(HSKVM v,int idx,bool pushval);
        SQRESULT        (*set)(HSKVM v,int idx);
        SQRESULT        (*get)(HSKVM v,int idx);
        SQRESULT        (*rawget)(HSKVM v,int idx);
        SQRESULT        (*rawset)(HSKVM v,int idx);
        SQRESULT        (*rawdeleteslot)(HSKVM v,int idx,bool pushval);
        SQRESULT        (*arrayappend)(HSKVM v,int idx);
        SQRESULT        (*arraypop)(HSKVM v,int idx,bool pushval);
        SQRESULT        (*arrayresize)(HSKVM v,int idx,int newsize);
        SQRESULT        (*arrayreverse)(HSKVM v,int idx);
        SQRESULT        (*arrayremove)(HSKVM v,int idx,int itemidx);
        SQRESULT        (*arrayinsert)(HSKVM v,int idx,int destpos);
        SQRESULT        (*setdelegate)(HSKVM v,int idx);
        SQRESULT        (*getdelegate)(HSKVM v,int idx);
        SQRESULT        (*clone)(HSKVM v,int idx);
        SQRESULT        (*setfreevariable)(HSKVM v,int idx,size_t nval);
        SQRESULT        (*next)(HSKVM v,int idx);
        SQRESULT        (*getweakrefval)(HSKVM v,int idx);
        SQRESULT        (*clear)(HSKVM v,int idx);

        /*calls*/
        SQRESULT        (*call)(HSKVM v,int params,bool retval,bool raiseerror);
        SQRESULT        (*resume)(HSKVM v,bool retval,bool raiseerror);
        const SQChar*   (*getlocal)(HSKVM v,size_t level,size_t idx);
        const SQChar*   (*getfreevariable)(HSKVM v,int idx,size_t nval);
        SQRESULT        (*throwerror)(HSKVM v,const SQChar *err);
        void            (*reseterror)(HSKVM v);
        void            (*getlasterror)(HSKVM v);

        /*raw object handling*/
        SQRESULT        (*getstackobj)(HSKVM v,int idx,HSQOBJECT *po);
        void            (*pushobject)(HSKVM v,HSQOBJECT obj);
        void            (*addref)(HSKVM v,HSQOBJECT *po);
        bool            (*release)(HSKVM v,HSQOBJECT *po);
        void            (*resetobject)(HSQOBJECT *po);
        const SQChar*   (*objtostring)(const HSQOBJECT *o);
        bool            (*objtobool)(const HSQOBJECT *o);
        int             (*objtointeger)(const HSQOBJECT *o);
        SQFloat         (*objtofloat)(const HSQOBJECT *o);
        SQRESULT        (*getobjtypetag)(const HSQOBJECT *o,PVOID * typetag);

        /*GC*/
        int             (*collectgarbage)(HSKVM v);

        /*serialization*/
        SQRESULT        (*writeclosure)(HSKVM vm,SQWRITEFUNC writef,PVOID up);
        SQRESULT        (*readclosure)(HSKVM vm,SQREADFUNC readf,PVOID up);

        /*mem allocation*/
        void*           (*malloc)(size_t size);
        void*           (*realloc)(void* p,size_t oldsize,size_t newsize);
        void            (*free)(void *p,size_t size);

        /*debug*/
        SQRESULT        (*stackinfos)(HSKVM v,int level,SQStackInfos *si);
        void            (*setdebughook)(HSKVM v);


        SQRESULT        (*ptr_typeof)(HSKVM v,int idx);
        SQRESULT        (*throwobject)(HSKVM v);

    } sq_api;

    typedef sq_api* HSQAPI;
    /// @endcond

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQ_MODULE_H_*/
