/*  see copyright notice in squirrel.h */
#ifndef _SQFUNCTION_H_
#define _SQFUNCTION_H_

#include "sqopcodes.h"

enum SQOuterType {
    otLOCAL = 0,
    otOUTER = 1
};

struct SQOuterVar
{

    SQOuterVar(){}
    SQOuterVar(const SQObjectPtr &name,const SQObjectPtr &src,SQOuterType t)
    {
        _name = name;
        _src=src;
        _type=t;
    }
    SQOuterVar(const SQOuterVar &ov)
    {
        _type=ov._type;
        _src=ov._src;
        _name=ov._name;
    }
    SQOuterType _type;
    SQObjectPtr _name;
    SQObjectPtr _src;
};

struct SQLocalVarInfo
{
    SQLocalVarInfo():_start_op(0),_end_op(0),_pos(0){}
    SQLocalVarInfo(const SQLocalVarInfo &lvi)
    {
        _name=lvi._name;
        _start_op=lvi._start_op;
        _end_op=lvi._end_op;
        _pos=lvi._pos;
    }
    SQObjectPtr _name;
    size_t _start_op;
    size_t _end_op;
    size_t _pos;
};

struct SQLineInfo { int _line;int _op; };

typedef sqvector<SQOuterVar> SQOuterVarVec;
typedef sqvector<SQLocalVarInfo> SQLocalVarInfoVec;
typedef sqvector<SQLineInfo> SQLineInfoVec;

#define _FUNC_SIZE(ni,nl,nparams,nfuncs,nouters,nlineinf,localinf,defparams) (sizeof(SQFunctionProto) \
        +((ni-1)*sizeof(SQInstruction))+(nl*sizeof(SQObjectPtr)) \
        +(nparams*sizeof(SQObjectPtr))+(nfuncs*sizeof(SQObjectPtr)) \
        +(nouters*sizeof(SQOuterVar))+(nlineinf*sizeof(SQLineInfo)) \
        +(localinf*sizeof(SQLocalVarInfo))+(defparams*sizeof(int)))


struct SQFunctionProto : public CHAINABLE_OBJ
{
private:
    SQFunctionProto(SQSharedState *ss);
    ~SQFunctionProto();

public:
    static SQFunctionProto *Create(SQSharedState *ss,int ninstructions,
        int nliterals,int nparameters,
        int nfunctions,int noutervalues,
        int nlineinfos,int nlocalvarinfos,int ndefaultparams)
    {
        SQFunctionProto *f;
        //I compact the whole class and members in a single memory allocation
        f = (SQFunctionProto *)sq_vm_malloc(_FUNC_SIZE(ninstructions,nliterals,nparameters,nfunctions,noutervalues,nlineinfos,nlocalvarinfos,ndefaultparams));
        new (f) SQFunctionProto(ss);
        f->_ninstructions = ninstructions;
        f->_literals = (SQObjectPtr*)&f->_instructions[ninstructions];
        f->_nliterals = nliterals;
        f->_parameters = (SQObjectPtr*)&f->_literals[nliterals];
        f->_nparameters = nparameters;
        f->_functions = (SQObjectPtr*)&f->_parameters[nparameters];
        f->_nfunctions = nfunctions;
        f->_outervalues = (SQOuterVar*)&f->_functions[nfunctions];
        f->_noutervalues = noutervalues;
        f->_lineinfos = (SQLineInfo *)&f->_outervalues[noutervalues];
        f->_nlineinfos = nlineinfos;
        f->_localvarinfos = (SQLocalVarInfo *)&f->_lineinfos[nlineinfos];
        f->_nlocalvarinfos = nlocalvarinfos;
        f->_defaultparams = (int *)&f->_localvarinfos[nlocalvarinfos];
        f->_ndefaultparams = ndefaultparams;

        _CONSTRUCT_VECTOR(SQObjectPtr,f->_nliterals,f->_literals);
        _CONSTRUCT_VECTOR(SQObjectPtr,f->_nparameters,f->_parameters);
        _CONSTRUCT_VECTOR(SQObjectPtr,f->_nfunctions,f->_functions);
        _CONSTRUCT_VECTOR(SQOuterVar,f->_noutervalues,f->_outervalues);
        //_CONSTRUCT_VECTOR(SQLineInfo,f->_nlineinfos,f->_lineinfos); //not required are 2 integers
        _CONSTRUCT_VECTOR(SQLocalVarInfo,f->_nlocalvarinfos,f->_localvarinfos);
        return f;
    }
    void Release(){
        _DESTRUCT_VECTOR(SQObjectPtr,_nliterals,_literals);
        _DESTRUCT_VECTOR(SQObjectPtr,_nparameters,_parameters);
        _DESTRUCT_VECTOR(SQObjectPtr,_nfunctions,_functions);
        _DESTRUCT_VECTOR(SQOuterVar,_noutervalues,_outervalues);
        //_DESTRUCT_VECTOR(SQLineInfo,_nlineinfos,_lineinfos); //not required are 2 integers
        _DESTRUCT_VECTOR(SQLocalVarInfo,_nlocalvarinfos,_localvarinfos);
        int size = _FUNC_SIZE(_ninstructions,_nliterals,_nparameters,_nfunctions,_noutervalues,_nlineinfos,_nlocalvarinfos,_ndefaultparams);
        this->~SQFunctionProto();
        sq_vm_free(this,size);
    }

    const SQChar* GetLocal(SQVM *v,size_t stackbase,size_t nseq,size_t nop);
    int GetLine(SQInstruction *curr);
    bool Save(SQVM *v,PVOID up,SQWRITEFUNC write);
    static bool Load(SQVM *v,PVOID up,SQREADFUNC read,SQObjectPtr &ret);
#ifndef NO_GARBAGE_COLLECTOR
    void Mark(SQCollectable **chain);
    void Finalize(){ _NULL_SQOBJECT_VECTOR(_literals,_nliterals); }
    SQObjectType GetType() {return OT_FUNCPROTO;}
#endif
    SQObjectPtr _sourcename;
    SQObjectPtr _name;
    int _stacksize;
    bool _bgenerator;
    int _varparams;

    int _nlocalvarinfos;
    SQLocalVarInfo *_localvarinfos;

    int _nlineinfos;
    SQLineInfo *_lineinfos;

    int _nliterals;
    SQObjectPtr *_literals;

    int _nparameters;
    SQObjectPtr *_parameters;

    int _nfunctions;
    SQObjectPtr *_functions;

    int _noutervalues;
    SQOuterVar *_outervalues;

    int _ndefaultparams;
    int *_defaultparams;

    int _ninstructions;
    SQInstruction _instructions[1];
};

#endif //_SQFUNCTION_H_
