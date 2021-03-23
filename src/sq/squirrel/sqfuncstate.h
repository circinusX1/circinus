/*  see copyright notice in squirrel.h */
#ifndef _SQFUNCSTATE_H_
#define _SQFUNCSTATE_H_
///////////////////////////////////
#include "squtils.h"

struct SQMembState
{
    SQMembState(SQSharedState *ss,SQMembState *parent,CompilerErrorMemb efunc,void *ed);
    ~SQMembState();
#ifdef _DEBUG_DUMP
    void Dump(SQFunctionProto *func);
#endif
    void Error(const SQChar *err);
    SQMembState *PushChildState(SQSharedState *ss);
    void PopChildState();
    void AddInstruction(SQOpcode _op,isize_t arg0=0,isize_t arg1=0,isize_t arg2=0,isize_t arg3=0){SQInstruction i(_op,arg0,arg1,arg2,arg3);AddInstruction(i);}
    template <typename T>  //mco
    void AddInstruction2(SQOpcode _op,
                        T arg0=0,
                        T arg1=0,
                        T arg2=0,
                        T arg3=0)
    {
        SQInstruction i(_op,arg0,arg1,arg2,arg3);
        AddInstruction(i);
    }

    void AddInstruction(SQInstruction &i);
    void SetInstructionParams(isize_t pos,isize_t arg0,isize_t arg1,isize_t arg2=0,isize_t arg3=0);
    void SetInstructionParam(isize_t pos,isize_t arg,isize_t val);
    SQInstruction &GetInstruction(isize_t pos){return _instructions[pos];}
    void PopInstructions(isize_t size){for(isize_t i=0;i<size;i++)_instructions.pop_back();}
    void SetStackSize(isize_t n);
    isize_t CountOuters(isize_t stacksize);
    void SnoozeOpt(){_optimization=false;}
    void AddDefaultParam(isize_t trg) { _defaultparams.push_back(trg); }
    isize_t GetDefaultParamCount() { return _defaultparams.size(); }
    isize_t GetCurrentPos(){return _instructions.size()-1;}
    isize_t GetNumericConstant(const isize_t cons);
    isize_t GetNumericConstant(const SQFloat cons);
    isize_t PushLocalVariable(const SQObject &name);
    void AddParameter(const SQObject &name);
    //void AddOuterValue(const SQObject &name);
    isize_t GetLocalVariable(const SQObject &name);
    void MarkLocalAsOuter(isize_t pos);
    isize_t GetOuterVariable(const SQObject &name);
    isize_t GenerateCode();
    isize_t GetStackSize();
    isize_t CalcStackFrameSize();
    void AddLineInfos(isize_t line,bool lineop,bool force=false);
    SQFunctionProto *BuildProto();
    isize_t AllocStackPos();
    isize_t PushTarget(isize_t n=-1);
    isize_t PopTarget();
    isize_t TopTarget();
    isize_t GetUpTarget(isize_t n);
    void DiscardTarget();
    bool IsLocal(size_t stkpos);
    SQObject CreateString(const SQChar *s,isize_t len = -1);
    SQObject CreateTable();
    bool IsConstant(const SQObject &name,SQObject &e);
    isize_t _returnexp;
    SQLocalVarInfoVec _vlocals;
    SQIntVec _targetstack;
    isize_t _stacksize;
    bool _varparams;
    bool _bgenerator;
    SQIntVec _unresolvedbreaks;
    SQIntVec _unresolvedcontinues;
    SQObjectPtrVec _functions;
    SQObjectPtrVec _parameters;
    SQOuterVarVec _outervalues;
    SQInstructionVec _instructions;
    SQLocalVarInfoVec _localvarinfos;
    SQObjectPtr _literals;
    SQObjectPtr _strings;
    SQObjectPtr _name;
    SQObjectPtr _sourcename;
    isize_t _nliterals;
    SQLineInfoVec _lineinfos;
    SQMembState *_parent;
    SQIntVec _scope_blocks;
    SQIntVec _breaktargets;
    SQIntVec _continuetargets;
    SQIntVec _defaultparams;
    isize_t _lastline;
    isize_t _traps; //contains number of nested exception traps
    isize_t _outers;
    bool _optimization;
    SQSharedState *_sharedstate;
    sqvector<SQMembState*> _childstates;
    isize_t GetConstant(const SQObject &cons);
private:
    CompilerErrorMemb _errfunc;
    void *_errtarget;
    SQSharedState *_ss;
};


#endif //_SQFUNCSTATE_H_

