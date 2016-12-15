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
    void AddInstruction(SQOpcode _op,int arg0=0,int arg1=0,int arg2=0,int arg3=0){SQInstruction i(_op,arg0,arg1,arg2,arg3);AddInstruction(i);}
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
    void SetInstructionParams(int pos,int arg0,int arg1,int arg2=0,int arg3=0);
    void SetInstructionParam(int pos,int arg,int val);
    SQInstruction &GetInstruction(int pos){return _instructions[pos];}
    void PopInstructions(int size){for(int i=0;i<size;i++)_instructions.pop_back();}
    void SetStackSize(int n);
    int CountOuters(int stacksize);
    void SnoozeOpt(){_optimization=false;}
    void AddDefaultParam(int trg) { _defaultparams.push_back(trg); }
    int GetDefaultParamCount() { return _defaultparams.size(); }
    int GetCurrentPos(){return _instructions.size()-1;}
    int GetNumericConstant(const int cons);
    int GetNumericConstant(const SQFloat cons);
    int PushLocalVariable(const SQObject &name);
    void AddParameter(const SQObject &name);
    //void AddOuterValue(const SQObject &name);
    int GetLocalVariable(const SQObject &name);
    void MarkLocalAsOuter(int pos);
    int GetOuterVariable(const SQObject &name);
    int GenerateCode();
    int GetStackSize();
    int CalcStackFrameSize();
    void AddLineInfos(int line,bool lineop,bool force=false);
    SQFunctionProto *BuildProto();
    int AllocStackPos();
    int PushTarget(int n=-1);
    int PopTarget();
    int TopTarget();
    int GetUpTarget(int n);
    void DiscardTarget();
    bool IsLocal(size_t stkpos);
    SQObject CreateString(const SQChar *s,int len = -1);
    SQObject CreateTable();
    bool IsConstant(const SQObject &name,SQObject &e);
    int _returnexp;
    SQLocalVarInfoVec _vlocals;
    SQIntVec _targetstack;
    int _stacksize;
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
    int _nliterals;
    SQLineInfoVec _lineinfos;
    SQMembState *_parent;
    SQIntVec _scope_blocks;
    SQIntVec _breaktargets;
    SQIntVec _continuetargets;
    SQIntVec _defaultparams;
    int _lastline;
    int _traps; //contains number of nested exception traps
    int _outers;
    bool _optimization;
    SQSharedState *_sharedstate;
    sqvector<SQMembState*> _childstates;
    int GetConstant(const SQObject &cons);
private:
    CompilerErrorMemb _errfunc;
    void *_errtarget;
    SQSharedState *_ss;
};


#endif //_SQFUNCSTATE_H_

