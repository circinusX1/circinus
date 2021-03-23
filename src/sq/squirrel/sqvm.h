/*  see copyright notice in squirrel.h */
#ifndef _SQVM_H_
#define _SQVM_H_

#include "sqopcodes.h"
#include "sqobject.h"
#define MAX_NATIVE_CALLS 100
#define MIN_STACK_OVERHEAD 15

#define SQ_SUSPEND_FLAG -666
#define SQ_TAILCALL_FLAG -777
#define DONT_FALL_BACK 666
//#define EXISTS_FALL_BACK -1

#define GET_FLAG_RAW                0x00000001
#define GET_FLAG_DO_NOT_RAISE_ERROR 0x00000002
//base lib
void sq_base_register(HSKVM v);


struct SQExceptionTrap{
    SQExceptionTrap() {}
    SQExceptionTrap(isize_t ss, isize_t stackbase,SQInstruction *ip, isize_t ex_target){ _stacksize = ss; _stackbase = stackbase; _ip = ip; _extarget = ex_target;}
    SQExceptionTrap(const SQExceptionTrap &et) { (*this) = et;  }
    isize_t _stackbase;
    isize_t _stacksize;
    SQInstruction *_ip;
    isize_t _extarget;
};

#define _INLINE

typedef sqvector<SQExceptionTrap> ExceptionsTraps;

struct SQVM : public CHAINABLE_OBJ
{
    struct CallInfo{
        //CallInfo() { _generator = NULL;}
        SQInstruction *_ip;
        SQObjectPtr *_literals;
        SQObjectPtr _closure;
        SQGenerator *_generator;
        int32_t _etraps;
        int32_t _prevstkbase;
        int32_t _prevtop;
        int32_t _target;
        int32_t _ncalls;
        bool _root;
    };

typedef sqvector<CallInfo> CallInfoVec;
public:
    void DebugHookProxy(isize_t type, const SQChar * sourcename, isize_t line, const SQChar * funcname);
    static void _DebugHookProxy(HSKVM v, isize_t type, const SQChar * sourcename, isize_t line, const SQChar * funcname);
    enum ExecutionType { ET_CALL, ET_RESUME_GENERATOR, ET_RESUME_VM,ET_RESUME_THROW_VM };
    SQVM(SQSharedState *ss);
    ~SQVM();
    bool Init(SQVM *friendvm, isize_t stacksize);
    bool Execute(SQObjectPtr &func, isize_t nargs, isize_t stackbase, SQObjectPtr &outres, bool raiseerror, ExecutionType et = ET_CALL);
    //starts a native call return when the NATIVE closure returns
    bool CallNative(SQNativeClosure *nclosure, isize_t nargs, isize_t newbase, SQObjectPtr &retval, int32_t target, bool &suspend,bool &tailcall);
	bool TailCall(SQClosure *closure, isize_t firstparam, isize_t nparams);
    //starts a SQUIRREL call in the same "Execution loop"
    bool StartCall(SQClosure *closure, isize_t target, isize_t nargs, isize_t stackbase, bool tailcall);
    bool CreateClassInstance(SQClass *theclass, SQObjectPtr &inst, SQObjectPtr &constructor);
    //call a generic closure pure SQUIRREL or NATIVE
    bool Call(SQObjectPtr &closure, isize_t nparams, isize_t stackbase, SQObjectPtr &outres,bool raiseerror);
    SQRESULT Suspend();

    void CallDebugHook(isize_t type,isize_t forcedline=0);
    void CallErrorHandler(SQObjectPtr &e);
    bool Get(const SQObjectPtr &self, const SQObjectPtr &key, SQObjectPtr &dest, size_t getflags, isize_t selfidx);
    isize_t FallBackGet(const SQObjectPtr &self,const SQObjectPtr &key,SQObjectPtr &dest);
    bool InvokeDefaultDelegate(const SQObjectPtr &self,const SQObjectPtr &key,SQObjectPtr &dest);
    bool Set(const SQObjectPtr &self, const SQObjectPtr &key, const SQObjectPtr &val, isize_t selfidx);
    isize_t FallBackSet(const SQObjectPtr &self,const SQObjectPtr &key,const SQObjectPtr &val);
    bool NewSlot(const SQObjectPtr &self, const SQObjectPtr &key, const SQObjectPtr &val,bool bstatic);
    bool NewSlotA(const SQObjectPtr &self,const SQObjectPtr &key,const SQObjectPtr &val,const SQObjectPtr &attrs,bool bstatic,bool raw);
    bool DeleteSlot(const SQObjectPtr &self, const SQObjectPtr &key, SQObjectPtr &res);
    bool Clone(const SQObjectPtr &self, SQObjectPtr &target);
    bool ObjCmp(const SQObjectPtr &o1, const SQObjectPtr &o2,isize_t &res);
    bool StringCat(const SQObjectPtr &str, const SQObjectPtr &obj, SQObjectPtr &dest);
    static bool IsEqual(const SQObjectPtr &o1,const SQObjectPtr &o2,bool &res);
    bool ToString(const SQObjectPtr &o,SQObjectPtr &res);
    SQString *PrintObjVal(const SQObjectPtr &o);


    void Raise_Error(const SQChar *s, ...);
    void Raise_Error(const SQObjectPtr &desc);
    void Raise_IdxError(const SQObjectPtr &o);
    void Raise_CompareError(const SQObject &o1, const SQObject &o2);
    void Raise_ParamTypeError(isize_t nparam,isize_t typemask,isize_t type);

    void FindOuter(SQObjectPtr &target, SQObjectPtr *stackindex);
    void RelocateOuters();
    void CloseOuters(SQObjectPtr *stackindex);

    bool TypeOf(const SQObjectPtr &obj1, SQObjectPtr &dest);
    bool CallMetaMethod(SQObjectPtr &closure, SQMetaMethod mm, isize_t nparams, SQObjectPtr &outres);
    bool ArithMetaMethod(isize_t op, const SQObjectPtr &o1, const SQObjectPtr &o2, SQObjectPtr &dest);
    bool Return(isize_t _arg0, isize_t _arg1, SQObjectPtr &retval);
    //new stuff
    _INLINE bool ARITH_OP(size_t op,SQObjectPtr &trg,const SQObjectPtr &o1,const SQObjectPtr &o2);
    _INLINE bool BW_OP(size_t op,SQObjectPtr &trg,const SQObjectPtr &o1,const SQObjectPtr &o2);
    _INLINE bool NEG_OP(SQObjectPtr &trg,const SQObjectPtr &o1);
    _INLINE bool CMP_OP(CmpOP op, const SQObjectPtr &o1,const SQObjectPtr &o2,SQObjectPtr &res);
    bool CLOSURE_OP(SQObjectPtr &target, SQFunctionProto *func);
    bool CLASS_OP(SQObjectPtr &target,isize_t base,isize_t attrs);
    //return true if the loop is finished
    bool FOREACH_OP(SQObjectPtr &o1,SQObjectPtr &o2,SQObjectPtr &o3,SQObjectPtr &o4,isize_t arg_2,isize_t exitpos,isize_t &jump);
    //_INLINE bool LOCAL_INC(isize_t op,SQObjectPtr &target, SQObjectPtr &a, SQObjectPtr &incr);
    _INLINE bool PLOCAL_INC(isize_t op,SQObjectPtr &target, SQObjectPtr &a, SQObjectPtr &incr);
    _INLINE bool DerefInc(isize_t op,SQObjectPtr &target, SQObjectPtr &self, SQObjectPtr &key, SQObjectPtr &incr, bool postfix,isize_t arg0);
#ifdef _DEBUG_DUMP
    void dumpstack(isize_t stackbase=-1, bool dumpall = false);
#endif

#ifndef NO_GARBAGE_COLLECTOR
    void Mark(SQCollectable **chain);
    SQObjectType GetType() {return OT_THREAD;}
#endif
    void Finalize();
    void GrowCallStack() {
        isize_t newsize = _alloccallsstacksize*2;
        _callstackdata.resize(newsize);
        _callsstack = &_callstackdata[0];
        _alloccallsstacksize = newsize;
    }
    bool EnterFrame(isize_t newbase, isize_t newtop, bool tailcall);
    void LeaveFrame();
    void Release(){ sq_delete(this,SQVM); }
////////////////////////////////////////////////////////////////////////////
    //stack functions for the api
    void Remove(isize_t n);

    static bool IsFalse(SQObjectPtr &o);

    void Pop();
    void Pop(isize_t n);
    void Push(const SQObjectPtr &o);
    void PushNull();
    SQObjectPtr &Top();
    SQObjectPtr &PopGet();
    SQObjectPtr &GetUp(isize_t n);
    SQObjectPtr &GetAt(isize_t n);

    SQObjectPtrVec _stack;

    isize_t _top;
    isize_t _stackbase;
    SQOuter *_openouters;
    SQObjectPtr _roottable;
    SQObjectPtr _lasterror;
    SQObjectPtr _errorhandler;

    bool _debughook;
    SQDEBUGHOOK _debughook_native;
    SQObjectPtr _debughook_closure;

    SQObjectPtr temp_reg;


    CallInfo* _callsstack;
    isize_t _callsstacksize;
    isize_t _alloccallsstacksize;
    sqvector<CallInfo>  _callstackdata;

    ExceptionsTraps _etraps;
    CallInfo *ci;
    PVOID _foreignptr;
    //VMs sharing the same state
    SQSharedState *_sharedstate;
    isize_t _nnativecalls;
    isize_t _nmetamethodscall;
    SQRELEASEHOOK _releasehook;
    //suspend infos
    bool _suspended;
    bool _suspended_root;
    isize_t _suspended_target;
    isize_t _suspended_traps;
};

struct AutoDec{
    AutoDec(isize_t *n) { _n = n; }
    ~AutoDec() { (*_n)--; }
    isize_t *_n;
};

inline SQObjectPtr &stack_get(HSKVM v,isize_t idx){return ((idx>=0)?(v->GetAt(idx+v->_stackbase-1)):(v->GetUp(idx)));}

#define _ss(_vm_) (_vm_)->_sharedstate

#ifndef NO_GARBAGE_COLLECTOR
#define _opt_ss(_vm_) (_vm_)->_sharedstate
#else
#define _opt_ss(_vm_) NULL
#endif

#define PUSH_CALLINFO(v,nci){ \
    isize_t css = v->_callsstacksize; \
    if(css == v->_alloccallsstacksize) { \
        v->GrowCallStack(); \
    } \
    v->ci = &v->_callsstack[css]; \
    *(v->ci) = nci; \
    v->_callsstacksize++; \
}

#define POP_CALLINFO(v){ \
    isize_t css = --v->_callsstacksize; \
    v->ci->_closure.Null(); \
    v->ci = css?&v->_callsstack[css-1]:NULL;    \
}

extern tagSQObjectType param_types[16];

#endif //_SQVM_H_
