/*
    see copyright notice in squirrel.h
*/
#include "sqpcheader.h"
#ifndef NO_COMPILER
#include "sqcompiler.h"
#include "sqstring.h"
#include "sqfuncproto.h"
#include "sqtable.h"
#include "sqopcodes.h"
#include "sqfuncstate.h"

#ifdef _DEBUG_DUMP
SQInstructionDesc g_InstrDesc[]={
    {_SC("_OP_LINE")},
    {_SC("_OP_LOAD")},
    {_SC("_OP_LOADINT")},
    {_SC("_OP_LOADFLOAT")},
    {_SC("_OP_DLOAD")},
    {_SC("_OP_TAILCALL")},
    {_SC("_OP_CALL")},
    {_SC("_OP_PREPCALL")},
    {_SC("_OP_PREPCALLK")},
    {_SC("_OP_GETK")},
    {_SC("_OP_MOVE")},
    {_SC("_OP_NEWSLOT")},
    {_SC("_OP_DELETE")},
    {_SC("_OP_SET")},
    {_SC("_OP_GET")},
    {_SC("_OP_EQ")},
    {_SC("_OP_NE")},
    {_SC("_OP_ADD")},
    {_SC("_OP_SUB")},
    {_SC("_OP_MUL")},
    {_SC("_OP_DIV")},
    {_SC("_OP_MOD")},
    {_SC("_OP_BITW")},
    {_SC("_OP_RETURN")},
    {_SC("_OP_LOADNULLS")},
    {_SC("_OP_LOADROOT")},
    {_SC("_OP_LOADBOOL")},
    {_SC("_OP_DMOVE")},
    {_SC("_OP_JMP")},
    {_SC("_OP_JCMP")},
    {_SC("_OP_JZ")},
    {_SC("_OP_SETOUTER")},
    {_SC("_OP_GETOUTER")},
    {_SC("_OP_NEWOBJ")},
    {_SC("_OP_APPENDARRAY")},
    {_SC("_OP_COMPARITH")},
    {_SC("_OP_INC")},
    {_SC("_OP_INCL")},
    {_SC("_OP_PINC")},
    {_SC("_OP_PINCL")},
    {_SC("_OP_CMP")},
    {_SC("_OP_EXISTS")},
    {_SC("_OP_INSTANCEOF")},
    {_SC("_OP_AND")},
    {_SC("_OP_OR")},
    {_SC("_OP_NEG")},
    {_SC("_OP_NOT")},
    {_SC("_OP_BWNOT")},
    {_SC("_OP_CLOSURE")},
    {_SC("_OP_YIELD")},
    {_SC("_OP_RESUME")},
    {_SC("_OP_FOREACH")},
    {_SC("_OP_POSTFOREACH")},
    {_SC("_OP_CLONE")},
    {_SC("_OP_TYPEOF")},
    {_SC("_OP_PUSHTRAP")},
    {_SC("_OP_POPTRAP")},
    {_SC("_OP_THROW")},
    {_SC("_OP_NEWSLOTA")},
    {_SC("_OP_GETBASE")},
    {_SC("_OP_CLOSE")},
};
#endif
void DumpLiteral(SQObjectPtr &o)
{
    switch(sq_type(o)){
        case OT_STRING: scprintf(_SC("\"%s\""),_stringval(o));break;
        case OT_FLOAT: scprintf(_SC("{%f}"),_float(o));break;
        case OT_INTEGER: scprintf(_SC("{") _PRINT_INT_FMT _SC("}"),_integer(o));break;
        case OT_BOOL: scprintf(_SC("%s"),_integer(o)?_SC("true"):_SC("false"));break;
        default: scprintf(_SC("(%s %p)"),GetTypeName(o),(void*)_rawval(o));break; break; //shut up compiler
    }
}

SQMembState::SQMembState(SQSharedState *ss,SQMembState *parent,CompilerErrorMemb efunc,void *ed)
{
        _nliterals = 0;
        _literals = SQTable::Create(ss,0);
        _strings =  SQTable::Create(ss,0);
        _sharedstate = ss;
        _lastline = 0;
        _optimization = true;
        _parent = parent;
        _stacksize = 0;
        _traps = 0;
        _returnexp = 0;
        _varparams = false;
        _errfunc = efunc;
        _errtarget = ed;
        _bgenerator = false;
        _outers = 0;
        _ss = ss;

}

void SQMembState::Error(const SQChar *err)
{
    _errfunc(_errtarget,err);
}

#ifdef _DEBUG_DUMP
void SQMembState::Dump(SQFunctionProto *func)
{
    size_t n=0,i;
    int si;
    scprintf(_SC("SQInstruction sizeof %d\n"),(int32_t)sizeof(SQInstruction));
    scprintf(_SC("SQObject sizeof %d\n"), (int32_t)sizeof(SQObject));
    scprintf(_SC("--------------------------------------------------------------------\n"));
    scprintf(_SC("*****FUNCTION [%s]\n"),sq_type(func->_name)==OT_STRING?_stringval(func->_name):_SC("unknown"));
    scprintf(_SC("-----LITERALS\n"));
    SQObjectPtr refidx,key,val;
    int idx;
    SQObjectPtrVec templiterals;
    templiterals.resize(_nliterals);
    while((idx=_table(_literals)->Next(false,refidx,key,val))!=-1) {
        refidx=idx;
        templiterals[_integer(val)]=key;
    }
    for(i=0;i<templiterals.size();i++){
        scprintf(_SC("[%d] "), (int32_t)n);
        DumpLiteral(templiterals[i]);
        scprintf(_SC("\n"));
        n++;
    }
    scprintf(_SC("-----PARAMS\n"));
    if(_varparams)
        scprintf(_SC("<<VARPARAMS>>\n"));
    n=0;
    for(i=0;i<_parameters.size();i++){
        scprintf(_SC("[%d] "), (int32_t)n);
        DumpLiteral(_parameters[i]);
        scprintf(_SC("\n"));
        n++;
    }
    scprintf(_SC("-----LOCALS\n"));
    for(si=0;si<func->_nlocalvarinfos;si++){
        SQLocalVarInfo lvi=func->_localvarinfos[si];
        scprintf(_SC("[%d] %s \t%d %d\n"), (int32_t)lvi._pos,_stringval(lvi._name), (int32_t)lvi._start_op, (int32_t)lvi._end_op);
        n++;
    }
    scprintf(_SC("-----LINE INFO\n"));
    for(i=0;i<_lineinfos.size();i++){
        SQLineInfo li=_lineinfos[i];
        scprintf(_SC("op [%d] line [%d] \n"), (int32_t)li._op, (int32_t)li._line);
        n++;
    }
    scprintf(_SC("-----dump\n"));
    n=0;
    for(i=0;i<_instructions.size();i++){
        SQInstruction &inst=_instructions[i];
        if(inst.op==_OP_LOAD || inst.op==_OP_DLOAD || inst.op==_OP_PREPCALLK || inst.op==_OP_GETK ){

            int lidx = inst._arg1;
            scprintf(_SC("[%03d] %15s %d "), (int32_t)n,g_InstrDesc[inst.op].name,inst._arg0);
            if(lidx >= 0xFFFFFFFF)
                scprintf(_SC("null"));
            else {
                int refidx;
                SQObjectPtr val,key,refo;
                while(((refidx=_table(_literals)->Next(false,refo,key,val))!= -1) && (_integer(val) != lidx)) {
                    refo = refidx;
                }
                DumpLiteral(key);
            }
            if(inst.op != _OP_DLOAD) {
                scprintf(_SC(" %d %d \n"),inst._arg2,inst._arg3);
            }
            else {
                scprintf(_SC(" %d "),inst._arg2);
                lidx = inst._arg3;
                if(lidx >= 0xFFFFFFFF)
                    scprintf(_SC("null"));
                else {
                    int refidx;
                    SQObjectPtr val,key,refo;
                    while(((refidx=_table(_literals)->Next(false,refo,key,val))!= -1) && (_integer(val) != lidx)) {
                        refo = refidx;
                }
                DumpLiteral(key);
                scprintf(_SC("\n"));
            }
            }
        }
        else if(inst.op==_OP_LOADFLOAT) {
            scprintf(_SC("[%03d] %15s %d %f %d %d\n"), (int32_t)n,g_InstrDesc[inst.op].name,inst._arg0,*((SQFloat*)&inst._arg1),inst._arg2,inst._arg3);
        }
    /*  else if(inst.op==_OP_ARITH){
            scprintf(_SC("[%03d] %15s %d %d %d %c\n"),n,g_InstrDesc[inst.op].name,inst._arg0,inst._arg1,inst._arg2,inst._arg3);
        }*/
        else {
            scprintf(_SC("[%03d] %15s %d %d %d %d\n"), (int32_t)n,g_InstrDesc[inst.op].name,inst._arg0,inst._arg1,inst._arg2,inst._arg3);
        }
        n++;
    }
    scprintf(_SC("-----\n"));
    scprintf(_SC("stack size[%d]\n"), (int32_t)func->_stacksize);
    scprintf(_SC("--------------------------------------------------------------------\n\n"));
}
#endif

int SQMembState::GetNumericConstant(const int cons)
{
    return GetConstant(SQObjectPtr(cons));
}

int SQMembState::GetNumericConstant(const SQFloat cons)
{
    return GetConstant(SQObjectPtr(cons));
}

int SQMembState::GetConstant(const SQObject &cons)
{
    SQObjectPtr val;
    if(!_table(_literals)->Get(cons,val))
    {
        val = _nliterals;
        _table(_literals)->NewSlot(cons,val);
        _nliterals++;
        if(_nliterals > MAX_LITERALS) {
            val.Null();
            Error(_SC("internal compiler error: too many literals"));
        }
    }
    return _integer(val);
}

void SQMembState::SetInstructionParams(int pos,int arg0,int arg1,int arg2,int arg3)
{
    _instructions[pos]._arg0=(unsigned char)*((size_t *)&arg0);
    _instructions[pos]._arg1=(int32_t)*((size_t *)&arg1);
    _instructions[pos]._arg2=(unsigned char)*((size_t *)&arg2);
    _instructions[pos]._arg3=(unsigned char)*((size_t *)&arg3);
}

void SQMembState::SetInstructionParam(int pos,int arg,int val)
{
    switch(arg){
        case 0:_instructions[pos]._arg0=(unsigned char)*((size_t *)&val);break;
        case 1:case 4:_instructions[pos]._arg1=(int32_t)*((size_t *)&val);break;
        case 2:_instructions[pos]._arg2=(unsigned char)*((size_t *)&val);break;
        case 3:_instructions[pos]._arg3=(unsigned char)*((size_t *)&val);break;
    };
}

int SQMembState::AllocStackPos()
{
    int npos=_vlocals.size();
    _vlocals.push_back(SQLocalVarInfo());
    if(_vlocals.size()>((size_t)_stacksize)) {
        if(_stacksize>MAX_FUNC_STACKSIZE) Error(_SC("internal compiler error: too many locals"));
        _stacksize=_vlocals.size();
    }
    return npos;
}

int SQMembState::PushTarget(int n)
{
    if(n!=-1){
        _targetstack.push_back(n);
        return n;
    }
    n=AllocStackPos();
    _targetstack.push_back(n);
    return n;
}

int SQMembState::GetUpTarget(int n){
    return _targetstack[((_targetstack.size()-1)-n)];
}

int SQMembState::TopTarget(){
    return _targetstack.back();
}
int SQMembState::PopTarget()
{
    size_t npos=_targetstack.back();
    assert(npos < _vlocals.size());
    SQLocalVarInfo &t = _vlocals[npos];
    if(sq_type(t._name)==OT_NULL){
        _vlocals.pop_back();
    }
    _targetstack.pop_back();
    return npos;
}

int SQMembState::GetStackSize()
{
    return _vlocals.size();
}

int SQMembState::CountOuters(int stacksize)
{
    int outers = 0;
    int k = _vlocals.size() - 1;
    while(k >= stacksize) {
        SQLocalVarInfo &lvi = _vlocals[k];
        k--;
        if(lvi._end_op == UINT_MINUS_ONE) { //this means is an outer
            outers++;
        }
    }
    return outers;
}

void SQMembState::SetStackSize(int n)
{
    int size=_vlocals.size();
    while(size>n){
        size--;
        SQLocalVarInfo lvi = _vlocals.back();
        if(sq_type(lvi._name)!=OT_NULL){
            if(lvi._end_op == UINT_MINUS_ONE) { //this means is an outer
                _outers--;
            }
            lvi._end_op = GetCurrentPos();
            _localvarinfos.push_back(lvi);
        }
        _vlocals.pop_back();
    }
}

bool SQMembState::IsConstant(const SQObject &name,SQObject &e)
{
    SQObjectPtr val;
    if(_table(_sharedstate->_consts)->Get(name,val)) {
        e = val;
        return true;
    }
    return false;
}

bool SQMembState::IsLocal(size_t stkpos)
{
    if(stkpos>=_vlocals.size())return false;
    else if(sq_type(_vlocals[stkpos]._name)!=OT_NULL)return true;
    return false;
}

int SQMembState::PushLocalVariable(const SQObject &name)
{
    int pos=_vlocals.size();
    SQLocalVarInfo lvi;
    lvi._name=name;
    lvi._start_op=GetCurrentPos()+1;
    lvi._pos=_vlocals.size();
    _vlocals.push_back(lvi);
    if(_vlocals.size()>((size_t)_stacksize))_stacksize=_vlocals.size();
    return pos;
}



int SQMembState::GetLocalVariable(const SQObject &name)
{
    int locals=_vlocals.size();
    while(locals>=1){
        SQLocalVarInfo &lvi = _vlocals[locals-1];
        if(sq_type(lvi._name)==OT_STRING && _string(lvi._name)==_string(name)){
            return locals-1;
        }
        locals--;
    }
    return -1;
}

void SQMembState::MarkLocalAsOuter(int pos)
{
    SQLocalVarInfo &lvi = _vlocals[pos];
    lvi._end_op = UINT_MINUS_ONE;
    _outers++;
}

int SQMembState::GetOuterVariable(const SQObject &name)
{
    int outers = _outervalues.size();
    for(int i = 0; i<outers; i++) {
        if(_string(_outervalues[i]._name) == _string(name))
            return i;
    }
    int pos=-1;
    if(_parent) {
        pos = _parent->GetLocalVariable(name);
        if(pos == -1) {
            pos = _parent->GetOuterVariable(name);
            if(pos != -1) {
                _outervalues.push_back(SQOuterVar(name,SQObjectPtr(int(pos)),otOUTER)); //local
                return _outervalues.size() - 1;
            }
        }
        else {
            _parent->MarkLocalAsOuter(pos);
            _outervalues.push_back(SQOuterVar(name,SQObjectPtr(int(pos)),otLOCAL)); //local
            return _outervalues.size() - 1;


        }
    }
    return -1;
}

void SQMembState::AddParameter(const SQObject &name)
{
    PushLocalVariable(name);
    _parameters.push_back(name);
}

void SQMembState::AddLineInfos(int line,bool lineop,bool force)
{
    if(_lastline!=line || force){
        SQLineInfo li;
        li._line=line;li._op=(GetCurrentPos()+1);
        if(lineop)AddInstruction(_OP_LINE,0,line);
        if(_lastline!=line) {
            _lineinfos.push_back(li);
        }
        _lastline=line;
    }
}

void SQMembState::DiscardTarget()
{
    int discardedtarget = PopTarget();
    int size = _instructions.size();
    if(size > 0 && _optimization){
        SQInstruction &pi = _instructions[size-1];//previous instruction
        switch(pi.op) {
        case _OP_SET:case _OP_NEWSLOT:case _OP_SETOUTER:case _OP_CALL:
            if(pi._arg0 == discardedtarget) {
                pi._arg0 = 0xFF;
            }
        }
    }
}

void SQMembState::AddInstruction(SQInstruction &i)
{
    int size = _instructions.size();
    if(size > 0 && _optimization){ //simple optimizer
        SQInstruction &pi = _instructions[size-1];//previous instruction
        switch(i.op) {
        case _OP_JZ:
            if( pi.op == _OP_CMP && pi._arg1 < 0xFF) {
                pi.op = _OP_JCMP;
                pi._arg0 = (unsigned char)pi._arg1;
                pi._arg1 = i._arg1;
                return;
            }
            break;
        case _OP_SET:
        case _OP_NEWSLOT:
            if(i._arg0 == i._arg3) {
                i._arg0 = 0xFF;
            }
            break;
        case _OP_SETOUTER:
            if(i._arg0 == i._arg2) {
                i._arg0 = 0xFF;
            }
            break;
        case _OP_RETURN:
            if( _parent && i._arg0 != MAX_FUNC_STACKSIZE && pi.op == _OP_CALL && _returnexp < size-1) {
                pi.op = _OP_TAILCALL;
            } else if(pi.op == _OP_CLOSE){
                pi = i;
                return;
            }
        break;
        case _OP_GET:
            if( pi.op == _OP_LOAD && pi._arg0 == i._arg2 && (!IsLocal(pi._arg0))){
                pi._arg2 = (unsigned char)i._arg1;
                pi.op = _OP_GETK;
                pi._arg0 = i._arg0;

                return;
            }
        break;
        case _OP_PREPCALL:
            if( pi.op == _OP_LOAD  && pi._arg0 == i._arg1 && (!IsLocal(pi._arg0))){
                pi.op = _OP_PREPCALLK;
                pi._arg0 = i._arg0;
                pi._arg2 = i._arg2;
                pi._arg3 = i._arg3;
                return;
            }
            break;
        case _OP_APPENDARRAY: {
            int aat = -1;
            switch(pi.op) {
            case _OP_LOAD: aat = AAT_LITERAL; break;
            case _OP_LOADINT: aat = AAT_INT; break;
            case _OP_LOADBOOL: aat = AAT_BOOL; break;
            case _OP_LOADFLOAT: aat = AAT_FLOAT; break;
            default: break;
            }
            if(aat != -1 && pi._arg0 == i._arg1 && (!IsLocal(pi._arg0))){
                pi.op = _OP_APPENDARRAY;
                pi._arg0 = i._arg0;
                pi._arg2 = (unsigned char)aat;
                pi._arg3 = MAX_FUNC_STACKSIZE;
                return;
            }
                              }
            break;
        case _OP_MOVE:
            switch(pi.op) {
            case _OP_GET: case _OP_ADD: case _OP_SUB: case _OP_MUL: case _OP_DIV: case _OP_MOD: case _OP_BITW:
            case _OP_LOADINT: case _OP_LOADFLOAT: case _OP_LOADBOOL: case _OP_LOAD:

                if(pi._arg0 == i._arg1)
                {
                    pi._arg0 = i._arg0;
                    _optimization = false;
                    //_result_elimination = false;
                    return;
                }
            }

            if(pi.op == _OP_MOVE)
            {
                pi.op = _OP_DMOVE;
                pi._arg2 = i._arg0;
                pi._arg3 = (unsigned char)i._arg1;
                return;
            }
            break;
        case _OP_LOAD:
            if(pi.op == _OP_LOAD && i._arg1 < 256) {
                pi.op = _OP_DLOAD;
                pi._arg2 = i._arg0;
                pi._arg3 = (unsigned char)i._arg1;
                return;
            }
            break;
        case _OP_EQ:case _OP_NE:
            if(pi.op == _OP_LOAD && pi._arg0 == i._arg1 && (!IsLocal(pi._arg0) ))
            {
                pi.op = i.op;
                pi._arg0 = i._arg0;
                pi._arg2 = i._arg2;
                pi._arg3 = MAX_FUNC_STACKSIZE;
                return;
            }
            break;
        case _OP_LOADNULLS:
            if((pi.op == _OP_LOADNULLS && pi._arg0+pi._arg1 == i._arg0)) {

                pi._arg1 = pi._arg1 + 1;
                pi.op = _OP_LOADNULLS;
                return;
            }
            break;
        case _OP_LINE:
            if(pi.op == _OP_LINE) {
                _instructions.pop_back();
                _lineinfos.pop_back();
            }
            break;
        }
    }
    _optimization = true;
    _instructions.push_back(i);
}

SQObject SQMembState::CreateString(const SQChar *s,int len)
{
    SQObjectPtr ns(SQString::Create(_sharedstate,s,len));
    _table(_strings)->NewSlot(ns,(int)1);
    return ns;
}

SQObject SQMembState::CreateTable()
{
    SQObjectPtr nt(SQTable::Create(_sharedstate,0));
    _table(_strings)->NewSlot(nt,(int)1);
    return nt;
}

SQFunctionProto *SQMembState::BuildProto()
{

    SQFunctionProto *f=SQFunctionProto::Create(_ss,_instructions.size(),
        _nliterals,_parameters.size(),_functions.size(),_outervalues.size(),
        _lineinfos.size(),_localvarinfos.size(),_defaultparams.size());

    SQObjectPtr refidx,key,val;
    int idx;

    f->_stacksize = _stacksize;
    f->_sourcename = _sourcename;
    f->_bgenerator = _bgenerator;
    f->_name = _name;

    while((idx=_table(_literals)->Next(false,refidx,key,val))!=-1) {
        f->_literals[_integer(val)]=key;
        refidx=idx;
    }

    for(size_t nf = 0; nf < _functions.size(); nf++) f->_functions[nf] = _functions[nf];
    for(size_t np = 0; np < _parameters.size(); np++) f->_parameters[np] = _parameters[np];
    for(size_t no = 0; no < _outervalues.size(); no++) f->_outervalues[no] = _outervalues[no];
    for(size_t nl = 0; nl < _localvarinfos.size(); nl++) f->_localvarinfos[nl] = _localvarinfos[nl];
    for(size_t ni = 0; ni < _lineinfos.size(); ni++) f->_lineinfos[ni] = _lineinfos[ni];
    for(size_t nd = 0; nd < _defaultparams.size(); nd++) f->_defaultparams[nd] = _defaultparams[nd];

    memcpy(f->_instructions,&_instructions[0],_instructions.size()*sizeof(SQInstruction));

    f->_varparams = _varparams;

    return f;
}

SQMembState *SQMembState::PushChildState(SQSharedState *ss)
{
    SQMembState *child = (SQMembState *)sq_malloc(sizeof(SQMembState));
    new (child) SQMembState(ss,this,_errfunc,_errtarget);
    _childstates.push_back(child);
    return child;
}

void SQMembState::PopChildState()
{
    SQMembState *child = _childstates.back();
    sq_delete(child,SQMembState);
    _childstates.pop_back();
}

SQMembState::~SQMembState()
{
    while(_childstates.size() > 0)
    {
        PopChildState();
    }
}

#endif
