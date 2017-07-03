/*
    see copyright notice in squirrel.h
*/
#include "sqpcheader.h"
#include "sqvm.h"
#include "sqstring.h"
#include "sqtable.h"
#include "sqarray.h"
#include "sqfuncproto.h"
#include "sqclosure.h"
#include "sqclass.h"
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

static bool str2num(const SQChar *s,SQObjectPtr &res,int base)
{
    SQChar *end;
    const SQChar *e = s;
    bool iseintbase = base > 13; //to fix error converting hexadecimals with e like 56f0791e
    bool isfloat = false;
    SQChar c;
    while((c = *e) != _SC('\0'))
    {
        if (c == _SC('.') || (!iseintbase && (c == _SC('E') || c == _SC('e')))) { //e and E is for scientific notation
            isfloat = true;
            break;
        }
        e++;
    }
    if(isfloat){
        SQFloat r = SQFloat(scstrtod(s,&end));
        if(s == end) return false;
        res = r;
    }
    else{
        int r = int(scstrtol(s,&end,(int)base));
        if(s == end) return false;
        res = r;
    }
    return true;
}

static int base_dummy(HSKVM SQ_UNUSED_ARG(v))
{
    return 0;
}

#ifndef NO_GARBAGE_COLLECTOR
static int base_collectgarbage(HSKVM v)
{
    sq_pushinteger(v, sq_collectgarbage(v));
    return 1;
}
static int base_resurectureachable(HSKVM v)
{
    sq_resurrectunreachable(v);
    return 1;
}
#endif

static int base_getroottable(HSKVM v)
{
    v->Push(v->_roottable);
    return 1;
}

static int base_getconsttable(HSKVM v)
{
    v->Push(_ss(v)->_consts);
    return 1;
}


static int base_setroottable(HSKVM v)
{
    SQObjectPtr o = v->_roottable;
    if(SQ_FAILED(sq_setroottable(v))) return SQ_ERROR;
    v->Push(o);
    return 1;
}

static int base_setconsttable(HSKVM v)
{
    SQObjectPtr o = _ss(v)->_consts;
    if(SQ_FAILED(sq_setconsttable(v))) return SQ_ERROR;
    v->Push(o);
    return 1;
}

static int base_seterrorhandler(HSKVM v)
{
    sq_seterrorhandler(v);
    return 0;
}

static int base_setdebughook(HSKVM v)
{
    sq_setdebughook(v);
    return 0;
}

static int base_enabledebuginfo(HSKVM v)
{
    SQObjectPtr &o=stack_get(v,2);

    sq_enabledebuginfo(v,SQVM::IsFalse(o)?SQFalse:SQTrue);
    return 0;
}

static int __getcallstackinfos(HSKVM v,int level)
{
    SQStackInfos si;
    int seq = 0;
    const SQChar *name = NULL;

    if (SQ_SUCCEEDED(sq_stackinfos(v, level, &si)))
    {
        const SQChar *fn = _SC("unknown");
        const SQChar *src = _SC("unknown");
        if(si.funcname)fn = si.funcname;
        if(si.source)src = si.source;
        sq_newtable(v);
        sq_pushstring(v, _SC("func"), -1);
        sq_pushstring(v, fn, -1);
        sq_newslot(v, -3, SQFalse);
        sq_pushstring(v, _SC("src"), -1);
        sq_pushstring(v, src, -1);
        sq_newslot(v, -3, SQFalse);
        sq_pushstring(v, _SC("line"), -1);
        sq_pushinteger(v, si.line);
        sq_newslot(v, -3, SQFalse);
        sq_pushstring(v, _SC("locals"), -1);
        sq_newtable(v);
        seq=0;
        while ((name = sq_getlocal(v, level, seq))) {
            sq_pushstring(v, name, -1);
            sq_push(v, -2);
            sq_newslot(v, -4, SQFalse);
            sq_pop(v, 1);
            seq++;
        }
        sq_newslot(v, -3, SQFalse);
        return 1;
    }

    return 0;
}
static int base_getstackinfos(HSKVM v)
{
    int level;
    sq_getinteger(v, -1, &level);
    return __getcallstackinfos(v,level);
}

static int base_assert(HSKVM v)
{
    if(SQVM::IsFalse(stack_get(v,2))){
        int top = sq_gettop(v);
        if (top>2 && SQ_SUCCEEDED(sq_tostring(v,3))) {
            const SQChar *str = 0;
            if (SQ_SUCCEEDED(sq_getstring(v,-1,&str))) {
                return sq_throwerror(v, str);
            }
        }
        return sq_throwerror(v, _SC("assertion failed"));
    }
    return 0;
}

static int get_slice_params(HSKVM v,int &sidx,int &eidx,SQObjectPtr &o)
{
    int top = sq_gettop(v);
    sidx=0;
    eidx=0;
    o=stack_get(v,1);
    if(top>1){
        SQObjectPtr &start=stack_get(v,2);
        if(sq_type(start)!=OT_NULL && sq_isnumeric(start)){
            sidx=tointeger(start);
        }
    }
    if(top>2){
        SQObjectPtr &end=stack_get(v,3);
        if(sq_isnumeric(end)){
            eidx=tointeger(end);
        }
    }
    else {
        eidx = sq_getsize(v,1);
    }
    return 1;
}

static int base_print(HSKVM v)
{
    const SQChar *str;
    if(SQ_SUCCEEDED(sq_tostring(v,2)))
    {
        if(SQ_SUCCEEDED(sq_getstring(v,-1,&str))) {
            if(_ss(v)->_printfunc) _ss(v)->_printfunc(v,_SC("%s"),str);
            return 0;
        }
    }
    return SQ_ERROR;
}

static int base_error(HSKVM v)
{
    const SQChar *str;
    if(SQ_SUCCEEDED(sq_tostring(v,2)))
    {
        if(SQ_SUCCEEDED(sq_getstring(v,-1,&str))) {
            if(_ss(v)->_errorfunc) _ss(v)->_errorfunc(v,_SC("%s"),str);
            return 0;
        }
    }
    return SQ_ERROR;
}

static int base_compilestring(HSKVM v)
{
    int nargs=sq_gettop(v);
    const SQChar *src=NULL,*name=_SC("unnamedbuffer");
    int size;
    sq_getstring(v,2,&src);
    size=sq_getsize(v,2);
    if(nargs>2){
        sq_getstring(v,3,&name);
    }
    if(SQ_SUCCEEDED(sq_compilebuffer(v,src,size,name,SQFalse)))
        return 1;
    else
        return SQ_ERROR;
}

static int base_newthread(HSKVM v)
{
    SQObjectPtr &func = stack_get(v,2);
    int stksize = (_closure(func)->_function->_stacksize << 1) +2;
    HSKVM newv = sq_newthread(v, (stksize < MIN_STACK_OVERHEAD + 2)? MIN_STACK_OVERHEAD + 2 : stksize);
    sq_move(newv,v,-2);
    return 1;
}

static int base_suspend(HSKVM v)
{
    return sq_suspendvm(v);
}

static int base_array(HSKVM v)
{
    SQArray *a;
    SQObject &size = stack_get(v,2);
    if(sq_gettop(v) > 2) {
        a = SQArray::Create(_ss(v),0);
        a->Resize(tointeger(size),stack_get(v,3));
    }
    else {
        a = SQArray::Create(_ss(v),tointeger(size));
    }
    v->Push(a);
    return 1;
}

static int base_type(HSKVM v)
{
    SQObjectPtr &o = stack_get(v,2);
    v->Push(SQString::Create(_ss(v),GetTypeName(o),-1));
    return 1;
}

static int base_callee(HSKVM v)
{
    if(v->_callsstacksize > 1)
    {
        v->Push(v->_callsstack[v->_callsstacksize - 2]._closure);
        return 1;
    }
    return sq_throwerror(v,_SC("no closure in the calls stack"));
}

static const SQRegFunction base_funcs[]={
    //generic
    {_SC("seterrorhandler"),base_seterrorhandler,2, NULL},
    {_SC("setdebughook"),base_setdebughook,2, NULL},
    {_SC("enabledebuginfo"),base_enabledebuginfo,2, NULL},
    {_SC("getstackinfos"),base_getstackinfos,2, _SC(".n")},
    {_SC("getroottable"),base_getroottable,1, NULL},
    {_SC("setroottable"),base_setroottable,2, NULL},
    {_SC("getconsttable"),base_getconsttable,1, NULL},
    {_SC("setconsttable"),base_setconsttable,2, NULL},
    {_SC("assert"),base_assert,-2, NULL},
    {_SC("print"),base_print,2, NULL},
    {_SC("error"),base_error,2, NULL},
    {_SC("compilestring"),base_compilestring,-2, _SC(".ss")},
    {_SC("newthread"),base_newthread,2, _SC(".c")},
    {_SC("suspend"),base_suspend,-1, NULL},
    {_SC("array"),base_array,-2, _SC(".n")},
    {_SC("type"),base_type,2, NULL},
    {_SC("callee"),base_callee,0,NULL},
    {_SC("dummy"),base_dummy,0,NULL},
#ifndef NO_GARBAGE_COLLECTOR
    {_SC("collectgarbage"),base_collectgarbage,0, NULL},
    {_SC("resurrectunreachable"),base_resurectureachable,0, NULL},
#endif
    {NULL,(SQFUNCTION)0,0,NULL}
};

void sq_base_register(HSKVM v)
{
    int i=0;
    sq_pushroottable(v);
    while(base_funcs[i].name!=0) {
        sq_pushstring(v,base_funcs[i].name,-1);
        sq_newclosure(v,base_funcs[i].f,0);
        sq_setnativeclosurename(v,-1,base_funcs[i].name);
        sq_setparamscheck(v,base_funcs[i].nparamscheck,base_funcs[i].typemask);
        sq_newslot(v,-3, SQFalse);
        i++;
    }

    sq_pushstring(v,_SC("_versionnumber_"),-1);
    sq_pushinteger(v,SQUIRREL_VERSION_NUMBER);
    sq_newslot(v,-3, SQFalse);
    sq_pushstring(v,_SC("_version_"),-1);
    sq_pushstring(v,SQUIRREL_VERSION,-1);
    sq_newslot(v,-3, SQFalse);
    sq_pushstring(v,_SC("_charsize_"),-1);
    sq_pushinteger(v,sizeof(SQChar));
    sq_newslot(v,-3, SQFalse);
    sq_pushstring(v,_SC("_intsize_"),-1);
    sq_pushinteger(v,sizeof(int));
    sq_newslot(v,-3, SQFalse);
    sq_pushstring(v,_SC("_floatsize_"),-1);
    sq_pushinteger(v,sizeof(SQFloat));
    sq_newslot(v,-3, SQFalse);
    sq_pop(v,1);
}

static int default_delegate_len(HSKVM v)
{
    v->Push(int(sq_getsize(v,1)));
    return 1;
}

static int default_delegate_tofloat(HSKVM v)
{
    SQObjectPtr &o=stack_get(v,1);
    switch(sq_type(o)){
    case OT_STRING:{
        SQObjectPtr res;
        if(str2num(_stringval(o),res,10)){
            v->Push(SQObjectPtr(tofloat(res)));
            break;
        }}
        return sq_throwerror(v, _SC("cannot convert the string"));
        break;
    case OT_INTEGER:case OT_FLOAT:
        v->Push(SQObjectPtr(tofloat(o)));
        break;
    case OT_BOOL:
        v->Push(SQObjectPtr((SQFloat)(_integer(o)?1:0)));
        break;
    default:
        v->PushNull();
        break;
    }
    return 1;
}

static int default_delegate_tointeger(HSKVM v)
{
    SQObjectPtr &o=stack_get(v,1);
    int base = 10;
    if(sq_gettop(v) > 1) {
        sq_getinteger(v,2,&base);
    }
    switch(sq_type(o)){
    case OT_STRING:{
        SQObjectPtr res;
        if(str2num(_stringval(o),res,base)){
            v->Push(SQObjectPtr(tointeger(res)));
            break;
        }}
        return sq_throwerror(v, _SC("cannot convert the string"));
        break;
    case OT_INTEGER:case OT_FLOAT:
        v->Push(SQObjectPtr(tointeger(o)));
        break;
    case OT_BOOL:
        v->Push(SQObjectPtr(_integer(o)?(int)1:(int)0));
        break;
    default:
        v->PushNull();
        break;
    }
    return 1;
}

static int default_delegate_tostring(HSKVM v)
{
    if(SQ_FAILED(sq_tostring(v,1)))
        return SQ_ERROR;
    return 1;
}

static int obj_delegate_weakref(HSKVM v)
{
    sq_weakref(v,1);
    return 1;
}

static int obj_clear(HSKVM v)
{
    return SQ_SUCCEEDED(sq_clear(v,-1)) ? 1 : SQ_ERROR;
}


static int number_delegate_tochar(HSKVM v)
{
    SQObject &o=stack_get(v,1);
    SQChar c = (SQChar)tointeger(o);
    v->Push(SQString::Create(_ss(v),(const SQChar *)&c,1));
    return 1;
}



/////////////////////////////////////////////////////////////////
//TABLE DEFAULT DELEGATE

static int table_rawdelete(HSKVM v)
{
    if(SQ_FAILED(sq_rawdeleteslot(v,1,SQTrue)))
        return SQ_ERROR;
    return 1;
}


static int container_rawexists(HSKVM v)
{
    if(SQ_SUCCEEDED(sq_rawget(v,-2))) {
        sq_pushbool(v,SQTrue);
        return 1;
    }
    sq_pushbool(v,SQFalse);
    return 1;
}

static int container_rawset(HSKVM v)
{
    return SQ_SUCCEEDED(sq_rawset(v,-3)) ? 1 : SQ_ERROR;
}


static int container_rawget(HSKVM v)
{
    return SQ_SUCCEEDED(sq_rawget(v,-2))?1:SQ_ERROR;
}

static int table_setdelegate(HSKVM v)
{
    if(SQ_FAILED(sq_setdelegate(v,-2)))
        return SQ_ERROR;
    sq_push(v,-1); // -1 because sq_setdelegate pops 1
    return 1;
}

static int table_getdelegate(HSKVM v)
{
    return SQ_SUCCEEDED(sq_getdelegate(v,-1))?1:SQ_ERROR;
}

static int table_filter(HSKVM v)
{
    SQObject &o = stack_get(v,1);
    SQTable *tbl = _table(o);
    SQObjectPtr ret = SQTable::Create(_ss(v),0);

    SQObjectPtr itr, key, val;
    int nitr;
    while((nitr = tbl->Next(false, itr, key, val)) != -1) {
        itr = (int)nitr;

        v->Push(o);
        v->Push(key);
        v->Push(val);
        if(SQ_FAILED(sq_call(v,3,SQTrue,SQFalse))) {
            return SQ_ERROR;
        }
        if(!SQVM::IsFalse(v->GetUp(-1))) {
            _table(ret)->NewSlot(key, val);
        }
        v->Pop();
    }

    v->Push(ret);
    return 1;
}

#define TABLE_TO_ARRAY_FUNC(_funcname_,_valname_) static int _funcname_(HSKVM v) \
{ \
	SQObject &o = stack_get(v, 1); \
	SQTable *t = _table(o); \
	SQObjectPtr itr, key, val; \
	SQObjectPtr _null; \
	int nitr, n = 0; \
	int nitems = t->CountUsed(); \
	SQArray *a = SQArray::Create(_ss(v), nitems); \
	a->Resize(nitems, _null); \
	if (nitems) { \
		while ((nitr = t->Next(false, itr, key, val)) != -1) { \
			itr = (int)nitr; \
			a->Set(n, _valname_); \
			n++; \
		} \
	} \
	v->Push(a); \
	return 1; \
}

TABLE_TO_ARRAY_FUNC(table_keys, key)
TABLE_TO_ARRAY_FUNC(table_values, val)


const SQRegFunction SQSharedState::_table_default_delegate_funcz[]={
    {_SC("len"),default_delegate_len,1, _SC("t")},
    {_SC("size"),default_delegate_len,1, _SC("t")},
    {_SC("rawget"),container_rawget,2, _SC("t")},
    {_SC("rawset"),container_rawset,3, _SC("t")},
    {_SC("rawdelete"),table_rawdelete,2, _SC("t")},
    {_SC("rawin"),container_rawexists,2, _SC("t")},
    {_SC("weakref"),obj_delegate_weakref,1, NULL },
    {_SC("tostring"),default_delegate_tostring,1, _SC(".")},
    {_SC("clear"),obj_clear,1, _SC(".")},
    {_SC("setdelegate"),table_setdelegate,2, _SC(".t|o")},
    {_SC("getdelegate"),table_getdelegate,1, _SC(".")},
    {_SC("filter"),table_filter,2, _SC("tc")},
	{_SC("keys"),table_keys,1, _SC("t") },
	{_SC("values"),table_values,1, _SC("t") },
    {NULL,(SQFUNCTION)0,0,NULL}
};

//ARRAY DEFAULT DELEGATE///////////////////////////////////////

static int array_append(HSKVM v)
{
    return SQ_SUCCEEDED(sq_arrayappend(v,-2)) ? 1 : SQ_ERROR;
}

static int array_extend(HSKVM v)
{
    _array(stack_get(v,1))->Extend(_array(stack_get(v,2)));
    sq_pop(v,1);
    return 1;
}

static int array_reverse(HSKVM v)
{
    return SQ_SUCCEEDED(sq_arrayreverse(v,-1)) ? 1 : SQ_ERROR;
}

static int array_pop(HSKVM v)
{
    return SQ_SUCCEEDED(sq_arraypop(v,1,SQTrue))?1:SQ_ERROR;
}

static int array_top(HSKVM v)
{
    SQObject &o=stack_get(v,1);
    if(_array(o)->Size()>0){
        v->Push(_array(o)->Top());
        return 1;
    }
    else return sq_throwerror(v,_SC("top() on a empty array"));
}

static int array_insert(HSKVM v)
{
    SQObject &o=stack_get(v,1);
    SQObject &idx=stack_get(v,2);
    SQObject &val=stack_get(v,3);
    if(!_array(o)->Insert(tointeger(idx),val))
        return sq_throwerror(v,_SC("index out of range"));
    sq_pop(v,2);
    return 1;
}

static int array_remove(HSKVM v)
{
    SQObject &o = stack_get(v, 1);
    SQObject &idx = stack_get(v, 2);
    if(!sq_isnumeric(idx)) return sq_throwerror(v, _SC("wrong type"));
    SQObjectPtr val;
    if(_array(o)->Get(tointeger(idx), val)) {
        _array(o)->Remove(tointeger(idx));
        v->Push(val);
        return 1;
    }
    return sq_throwerror(v, _SC("idx out of range"));
}

static int array_resize(HSKVM v)
{
    SQObject &o = stack_get(v, 1);
    SQObject &nsize = stack_get(v, 2);
    SQObjectPtr fill;
    if(sq_isnumeric(nsize)) {
        int sz = tointeger(nsize);
        if (sz<0)
          return sq_throwerror(v, _SC("resizing to negative length"));

        if(sq_gettop(v) > 2)
            fill = stack_get(v, 3);
        _array(o)->Resize(sz,fill);
        sq_settop(v, 1);
        return 1;
    }
    return sq_throwerror(v, _SC("size must be a number"));
}

static int __map_array(SQArray *dest,SQArray *src,HSKVM v) {
    SQObjectPtr temp;
    int size = src->Size();
    SQObject &closure = stack_get(v, 2);
    v->Push(closure);

    int nArgs=0;
    if(sq_type(closure) == OT_CLOSURE) {
        nArgs = _closure(closure)->_function->_nparameters;
    }
    else if (sq_type(closure) == OT_NATIVECLOSURE) {
        int nParamsCheck = _nativeclosure(closure)->_nparamscheck;
        if (nParamsCheck > 0)
            nArgs = nParamsCheck;
        else // push all params when there is no check or only minimal count set
            nArgs = 4;
    }

    for(int n = 0; n < size; n++) {
        src->Get(n,temp);
        v->Push(src);
        v->Push(temp);
        if (nArgs >= 3)
            v->Push(SQObjectPtr(n));
        if (nArgs >= 4)
            v->Push(src);
        if(SQ_FAILED(sq_call(v,nArgs,SQTrue,SQFalse))) {
            return SQ_ERROR;
        }
        dest->Set(n,v->GetUp(-1));
        v->Pop();
    }
    v->Pop();
    return 0;
}

static int array_map(HSKVM v)
{
    SQObject &o = stack_get(v,1);
    int size = _array(o)->Size();
    SQObjectPtr ret = SQArray::Create(_ss(v),size);
    if(SQ_FAILED(__map_array(_array(ret),_array(o),v)))
        return SQ_ERROR;
    v->Push(ret);
    return 1;
}

static int array_apply(HSKVM v)
{
    SQObject &o = stack_get(v,1);
    if(SQ_FAILED(__map_array(_array(o),_array(o),v)))
        return SQ_ERROR;
    sq_pop(v,1);
    return 1;
}

static int array_reduce(HSKVM v)
{
    SQObject &o = stack_get(v,1);
    SQArray *a = _array(o);
    int size = a->Size();
    SQObjectPtr res;
    int iterStart;
    if (sq_gettop(v)>2) {
        res = stack_get(v,3);
        iterStart = 0;
    } else if (size==0) {
        return 0;
    } else {
        a->Get(0,res);
        iterStart = 1;
    }
    if (size > iterStart) {
        SQObjectPtr other;
        v->Push(stack_get(v,2));
        for (int n = iterStart; n < size; n++) {
            a->Get(n,other);
            v->Push(o);
            v->Push(res);
            v->Push(other);
            if(SQ_FAILED(sq_call(v,3,SQTrue,SQFalse))) {
                return SQ_ERROR;
            }
            res = v->GetUp(-1);
            v->Pop();
        }
        v->Pop();
    }
    v->Push(res);
    return 1;
}

static int array_filter(HSKVM v)
{
    SQObject &o = stack_get(v,1);
    SQArray *a = _array(o);
    SQObjectPtr ret = SQArray::Create(_ss(v),0);
    int size = a->Size();
    SQObjectPtr val;
    for(int n = 0; n < size; n++) {
        a->Get(n,val);
        v->Push(o);
        v->Push(n);
        v->Push(val);
        if(SQ_FAILED(sq_call(v,3,SQTrue,SQFalse))) {
            return SQ_ERROR;
        }
        if(!SQVM::IsFalse(v->GetUp(-1))) {
            _array(ret)->Append(val);
        }
        v->Pop();
    }
    v->Push(ret);
    return 1;
}

static int array_find(HSKVM v)
{
    SQObject &o = stack_get(v,1);
    SQObjectPtr &val = stack_get(v,2);
    SQArray *a = _array(o);
    int size = a->Size();
    SQObjectPtr temp;
    for(int n = 0; n < size; n++) {
        bool res = false;
        a->Get(n,temp);
        if(SQVM::IsEqual(temp,val,res) && res) {
            v->Push(n);
            return 1;
        }
    }
    return 0;
}


static bool _sort_compare(HSKVM v,SQObjectPtr &a,SQObjectPtr &b,int func,int &ret)
{
    if(func < 0) {
        if(!v->ObjCmp(a,b,ret)) return false;
    }
    else {
        int top = sq_gettop(v);
        sq_push(v, func);
        sq_pushroottable(v);
        v->Push(a);
        v->Push(b);
        if(SQ_FAILED(sq_call(v, 3, SQTrue, SQFalse))) {
            if(!sq_isstring( v->_lasterror))
                v->Raise_Error(_SC("compare func failed"));
            return false;
        }
        if(SQ_FAILED(sq_getinteger(v, -1, &ret))) {
            v->Raise_Error(_SC("numeric value expected as return value of the compare function"));
            return false;
        }
        sq_settop(v, top);
        return true;
    }
    return true;
}

static bool _hsort_sift_down(HSKVM v,SQArray *arr, int root, int bottom, int func)
{
    int maxChild;
    int done = 0;
    int ret;
    int root2;
    while (((root2 = root * 2) <= bottom) && (!done))
    {
        if (root2 == bottom) {
            maxChild = root2;
        }
        else {
            if(!_sort_compare(v,arr->_values[root2],arr->_values[root2 + 1],func,ret))
                return false;
            if (ret > 0) {
                maxChild = root2;
            }
            else {
                maxChild = root2 + 1;
            }
        }

        if(!_sort_compare(v,arr->_values[root],arr->_values[maxChild],func,ret))
            return false;
        if (ret < 0) {
            if (root == maxChild) {
                v->Raise_Error(_SC("inconsistent compare function"));
                return false; // We'd be swapping ourselve. The compare function is incorrect
            }

            _Swap(arr->_values[root],arr->_values[maxChild]);
            root = maxChild;
        }
        else {
            done = 1;
        }
    }
    return true;
}

static bool _hsort(HSKVM v,SQObjectPtr &arr, int SQ_UNUSED_ARG(l), int SQ_UNUSED_ARG(r),int func)
{
    SQArray *a = _array(arr);
    int i;
    int array_size = a->Size();
    for (i = (array_size / 2); i >= 0; i--) {
        if(!_hsort_sift_down(v,a, i, array_size - 1,func)) return false;
    }

    for (i = array_size-1; i >= 1; i--)
    {
        _Swap(a->_values[0],a->_values[i]);
        if(!_hsort_sift_down(v,a, 0, i-1,func)) return false;
    }
    return true;
}

static int array_sort(HSKVM v)
{
    int func = -1;
    SQObjectPtr &o = stack_get(v,1);
    if(_array(o)->Size() > 1) {
        if(sq_gettop(v) == 2) func = 2;
        if(!_hsort(v, o, 0, _array(o)->Size()-1, func))
            return SQ_ERROR;

    }
    sq_settop(v,1);
    return 1;
}

static int array_slice(HSKVM v)
{
    int sidx,eidx;
    SQObjectPtr o;
    if(get_slice_params(v,sidx,eidx,o)==-1)return -1;
    int alen = _array(o)->Size();
    if(sidx < 0)sidx = alen + sidx;
    if(eidx < 0)eidx = alen + eidx;
    if(eidx < sidx)return sq_throwerror(v,_SC("wrong indexes"));
    if(eidx > alen || sidx < 0)return sq_throwerror(v, _SC("slice out of range"));
    SQArray *arr=SQArray::Create(_ss(v),eidx-sidx);
    SQObjectPtr t;
    int count=0;
    for(int i=sidx;i<eidx;i++){
        _array(o)->Get(i,t);
        arr->Set(count++,t);
    }
    v->Push(arr);
    return 1;

}

const SQRegFunction SQSharedState::_array_default_delegate_funcz[]={
    {_SC("len"),default_delegate_len,1, _SC("a")},
    {_SC("size"),default_delegate_len,1, _SC("a")},
    {_SC("append"),array_append,2, _SC("a")},
    {_SC("extend"),array_extend,2, _SC("aa")},
    {_SC("push"),array_append,2, _SC("a")},
    {_SC("pop"),array_pop,1, _SC("a")},
    {_SC("top"),array_top,1, _SC("a")},
    {_SC("insert"),array_insert,3, _SC("an")},
    {_SC("remove"),array_remove,2, _SC("an")},
    {_SC("resize"),array_resize,-2, _SC("an")},
    {_SC("reverse"),array_reverse,1, _SC("a")},
    {_SC("sort"),array_sort,-1, _SC("ac")},
    {_SC("slice"),array_slice,-1, _SC("ann")},
    {_SC("split"),array_slice,-1, _SC("ann")},
    {_SC("weakref"),obj_delegate_weakref,1, NULL },
    {_SC("tostring"),default_delegate_tostring,1, _SC(".")},
    {_SC("clear"),obj_clear,1, _SC(".")},
    {_SC("map"),array_map,2, _SC("ac")},
    {_SC("apply"),array_apply,2, _SC("ac")},
    {_SC("reduce"),array_reduce,-2, _SC("ac.")},
    {_SC("filter"),array_filter,2, _SC("ac")},
    {_SC("find"),array_find,2, _SC("a.")},
    {NULL,(SQFUNCTION)0,0,NULL}
};

//STRING DEFAULT DELEGATE//////////////////////////
static int string_slice(HSKVM v)
{
    int sidx,eidx;
    SQObjectPtr o;
    if(SQ_FAILED(get_slice_params(v,sidx,eidx,o)))return -1;
    int slen = _string(o)->_len;
    if(sidx < 0)sidx = slen + sidx;
    if(eidx < 0)eidx = slen + eidx;
    if(eidx < sidx) return sq_throwerror(v,_SC("wrong indexes"));
    if(eidx > slen || sidx < 0) return sq_throwerror(v, _SC("slice out of range"));
    v->Push(SQString::Create(_ss(v),&_stringval(o)[sidx],eidx-sidx));
    return 1;
}

static int string_find(HSKVM v)
{
    int top,start_idx=0;
    const SQChar *str,*substr,*ret;
    if(((top=sq_gettop(v))>1) && SQ_SUCCEEDED(sq_getstring(v,1,&str)) && SQ_SUCCEEDED(sq_getstring(v,2,&substr))){
        if(top>2)sq_getinteger(v,3,&start_idx);
        if((sq_getsize(v,1)>start_idx) && (start_idx>=0)){
            ret=scstrstr(&str[start_idx],substr);
            if(ret){
                sq_pushinteger(v,(int)(ret-str));
                return 1;
            }
        }
        return 0;
    }
    return sq_throwerror(v,_SC("invalid param"));
}

#define STRING_TOFUNCZ(func) static int string_##func(HSKVM v) \
{\
    int sidx,eidx; \
    SQObjectPtr str; \
    if(SQ_FAILED(get_slice_params(v,sidx,eidx,str)))return -1; \
    int slen = _string(str)->_len; \
    if(sidx < 0)sidx = slen + sidx; \
    if(eidx < 0)eidx = slen + eidx; \
    if(eidx < sidx) return sq_throwerror(v,_SC("wrong indexes")); \
    if(eidx > slen || sidx < 0) return sq_throwerror(v,_SC("slice out of range")); \
    int len=_string(str)->_len; \
    const SQChar *sthis=_stringval(str); \
    SQChar *snew=(_ss(v)->GetScratchPad(sq_rsl(len))); \
    memcpy(snew,sthis,sq_rsl(len));\
    for(int i=sidx;i<eidx;i++) snew[i] = func(sthis[i]); \
    v->Push(SQString::Create(_ss(v),snew,len)); \
    return 1; \
}


STRING_TOFUNCZ(tolower)
STRING_TOFUNCZ(toupper)

const SQRegFunction SQSharedState::_string_default_delegate_funcz[]={
    {_SC("len"),default_delegate_len,1, _SC("s")},
    {_SC("size"),default_delegate_len,1, _SC("s")},
    {_SC("tointeger"),default_delegate_tointeger,-1, _SC("sn")},
    {_SC("tofloat"),default_delegate_tofloat,1, _SC("s")},
    {_SC("tostring"),default_delegate_tostring,1, _SC(".")},
    {_SC("slice"),string_slice,-1, _SC("s n  n")},
    {_SC("split"),string_slice,-1, _SC("s n  n")},
    {_SC("find"),string_find,-2, _SC("s s n")},
    {_SC("tolower"),string_tolower,-1, _SC("s n n")},
    {_SC("toupper"),string_toupper,-1, _SC("s n n")},
    {_SC("weakref"),obj_delegate_weakref,1, NULL },
    {NULL,(SQFUNCTION)0,0,NULL}
};

//INTEGER DEFAULT DELEGATE//////////////////////////
const SQRegFunction SQSharedState::_number_default_delegate_funcz[]={
    {_SC("tointeger"),default_delegate_tointeger,1, _SC("n|b")},
    {_SC("tofloat"),default_delegate_tofloat,1, _SC("n|b")},
    {_SC("tostring"),default_delegate_tostring,1, _SC(".")},
    {_SC("tochar"),number_delegate_tochar,1, _SC("n|b")},
    {_SC("weakref"),obj_delegate_weakref,1, NULL },
    {NULL,(SQFUNCTION)0,0,NULL}
};

//CLOSURE DEFAULT DELEGATE//////////////////////////
static int closure_pcall(HSKVM v)
{
    return SQ_SUCCEEDED(sq_call(v,sq_gettop(v)-1,SQTrue,SQFalse))?1:SQ_ERROR;
}

static int closure_call(HSKVM v)
{
	SQObjectPtr &c = stack_get(v, -1);
	if (sq_type(c) == OT_CLOSURE && (_closure(c)->_function->_bgenerator == false))
	{
		return sq_tailcall(v, sq_gettop(v) - 1);
	}
	return SQ_SUCCEEDED(sq_call(v, sq_gettop(v) - 1, SQTrue, SQTrue)) ? 1 : SQ_ERROR;
}

static int _closure_acall(HSKVM v,bool raiseerror)
{
    SQArray *aparams=_array(stack_get(v,2));
    int nparams=aparams->Size();
    v->Push(stack_get(v,1));
    for(int i=0;i<nparams;i++)v->Push(aparams->_values[i]);
    return SQ_SUCCEEDED(sq_call(v,nparams,SQTrue,raiseerror))?1:SQ_ERROR;
}

static int closure_acall(HSKVM v)
{
    return _closure_acall(v,SQTrue);
}

static int closure_pacall(HSKVM v)
{
    return _closure_acall(v,SQFalse);
}

static int closure_bindenv(HSKVM v)
{
    if(SQ_FAILED(sq_bindenv(v,1)))
        return SQ_ERROR;
    return 1;
}

static int closure_getroot(HSKVM v)
{
    if(SQ_FAILED(sq_getclosureroot(v,-1)))
        return SQ_ERROR;
    return 1;
}

static int closure_setroot(HSKVM v)
{
    if(SQ_FAILED(sq_setclosureroot(v,-2)))
        return SQ_ERROR;
    return 1;
}

static int closure_getinfos(HSKVM v) {
    SQObject o = stack_get(v,1);
    SQTable *res = SQTable::Create(_ss(v),4);
    if(sq_type(o) == OT_CLOSURE) {
        SQFunctionProto *f = _closure(o)->_function;
        int nparams = f->_nparameters + (f->_varparams?1:0);
        SQObjectPtr params = SQArray::Create(_ss(v),nparams);
    SQObjectPtr defparams = SQArray::Create(_ss(v),f->_ndefaultparams);
        for(int n = 0; n<f->_nparameters; n++) {
            _array(params)->Set((int)n,f->_parameters[n]);
        }
    for(int j = 0; j<f->_ndefaultparams; j++) {
            _array(defparams)->Set((int)j,_closure(o)->_defaultparams[j]);
        }
        if(f->_varparams) {
            _array(params)->Set(nparams-1,SQString::Create(_ss(v),_SC("..."),-1));
        }
        res->NewSlot(SQString::Create(_ss(v),_SC("native"),-1),false);
        res->NewSlot(SQString::Create(_ss(v),_SC("name"),-1),f->_name);
        res->NewSlot(SQString::Create(_ss(v),_SC("src"),-1),f->_sourcename);
        res->NewSlot(SQString::Create(_ss(v),_SC("parameters"),-1),params);
        res->NewSlot(SQString::Create(_ss(v),_SC("varargs"),-1),f->_varparams);
    res->NewSlot(SQString::Create(_ss(v),_SC("defparams"),-1),defparams);
    }
    else { //OT_NATIVECLOSURE
        SQNativeClosure *nc = _nativeclosure(o);
        res->NewSlot(SQString::Create(_ss(v),_SC("native"),-1),true);
        res->NewSlot(SQString::Create(_ss(v),_SC("name"),-1),nc->_name);
        res->NewSlot(SQString::Create(_ss(v),_SC("paramscheck"),-1),nc->_nparamscheck);
        SQObjectPtr typecheck;
        if(nc->_typecheck.size() > 0) {
            typecheck =
                SQArray::Create(_ss(v), nc->_typecheck.size());
            for(size_t n = 0; n<nc->_typecheck.size(); n++) {
                    _array(typecheck)->Set((int)n,nc->_typecheck[n]);
            }
        }
        res->NewSlot(SQString::Create(_ss(v),_SC("typecheck"),-1),typecheck);
    }
    v->Push(res);
    return 1;
}



const SQRegFunction SQSharedState::_closure_default_delegate_funcz[]={
    {_SC("call"),closure_call,-1, _SC("c")},
    {_SC("pcall"),closure_pcall,-1, _SC("c")},
    {_SC("acall"),closure_acall,2, _SC("ca")},
    {_SC("pacall"),closure_pacall,2, _SC("ca")},
    {_SC("weakref"),obj_delegate_weakref,1, NULL },
    {_SC("tostring"),default_delegate_tostring,1, _SC(".")},
    {_SC("bindenv"),closure_bindenv,2, _SC("c x|y|t")},
    {_SC("getinfos"),closure_getinfos,1, _SC("c")},
    {_SC("getroot"),closure_getroot,1, _SC("c")},
    {_SC("setroot"),closure_setroot,2, _SC("ct")},
    {NULL,(SQFUNCTION)0,0,NULL}
};

//GENERATOR DEFAULT DELEGATE
static int generator_getstatus(HSKVM v)
{
    SQObject &o=stack_get(v,1);
    switch(_generator(o)->_state){
        case SQGenerator::eSuspended:v->Push(SQString::Create(_ss(v),_SC("suspended")));break;
        case SQGenerator::eRunning:v->Push(SQString::Create(_ss(v),_SC("running")));break;
        case SQGenerator::eDead:v->Push(SQString::Create(_ss(v),_SC("dead")));break;
    }
    return 1;
}

const SQRegFunction SQSharedState::_generator_default_delegate_funcz[]={
    {_SC("getstatus"),generator_getstatus,1, _SC("g")},
    {_SC("weakref"),obj_delegate_weakref,1, NULL },
    {_SC("tostring"),default_delegate_tostring,1, _SC(".")},
    {NULL,(SQFUNCTION)0,0,NULL}
};

//THREAD DEFAULT DELEGATE
static int thread_call(HSKVM v)
{
    SQObjectPtr o = stack_get(v,1);
    if(sq_type(o) == OT_THREAD) {
        int nparams = sq_gettop(v);
        _thread(o)->Push(_thread(o)->_roottable);
        for(int i = 2; i<(nparams+1); i++)
            sq_move(_thread(o),v,i);
        if(SQ_SUCCEEDED(sq_call(_thread(o),nparams,SQTrue,SQTrue))) {
            sq_move(v,_thread(o),-1);
            sq_pop(_thread(o),1);
            return 1;
        }
        v->_lasterror = _thread(o)->_lasterror;
        return SQ_ERROR;
    }
    return sq_throwerror(v,_SC("wrong parameter"));
}

static int thread_wakeup(HSKVM v)
{
    SQObjectPtr o = stack_get(v,1);
    if(sq_type(o) == OT_THREAD) {
        SQVM *thread = _thread(o);
        int state = sq_getvmstate(thread);
        if(state != SQ_VMSTATE_SUSPENDED) {
            switch(state) {
                case SQ_VMSTATE_IDLE:
                    return sq_throwerror(v,_SC("cannot wakeup a idle thread"));
                break;
                case SQ_VMSTATE_RUNNING:
                    return sq_throwerror(v,_SC("cannot wakeup a running thread"));
                break;
            }
        }

        int wakeupret = sq_gettop(v)>1?SQTrue:SQFalse;
        if(wakeupret) {
            sq_move(thread,v,2);
        }
        if(SQ_SUCCEEDED(sq_wakeupvm(thread,wakeupret,SQTrue,SQTrue,SQFalse))) {
            sq_move(v,thread,-1);
            sq_pop(thread,1); //pop retval
            if(sq_getvmstate(thread) == SQ_VMSTATE_IDLE) {
                sq_settop(thread,1); //pop roottable
            }
            return 1;
        }
        sq_settop(thread,1);
        v->_lasterror = thread->_lasterror;
        return SQ_ERROR;
    }
    return sq_throwerror(v,_SC("wrong parameter"));
}

static int thread_wakeupthrow(HSKVM v)
{
    SQObjectPtr o = stack_get(v,1);
    if(sq_type(o) == OT_THREAD) {
        SQVM *thread = _thread(o);
        int state = sq_getvmstate(thread);
        if(state != SQ_VMSTATE_SUSPENDED) {
            switch(state) {
                case SQ_VMSTATE_IDLE:
                    return sq_throwerror(v,_SC("cannot wakeup a idle thread"));
                break;
                case SQ_VMSTATE_RUNNING:
                    return sq_throwerror(v,_SC("cannot wakeup a running thread"));
                break;
            }
        }

        sq_move(thread,v,2);
        sq_throwobject(thread);
        bool rethrow_error = SQTrue;
        if(sq_gettop(v) > 2) {
            sq_getbool(v,3,&rethrow_error);
        }
        if(SQ_SUCCEEDED(sq_wakeupvm(thread,SQFalse,SQTrue,SQTrue,SQTrue))) {
            sq_move(v,thread,-1);
            sq_pop(thread,1); //pop retval
            if(sq_getvmstate(thread) == SQ_VMSTATE_IDLE) {
                sq_settop(thread,1); //pop roottable
            }
            return 1;
        }
        sq_settop(thread,1);
        if(rethrow_error) {
            v->_lasterror = thread->_lasterror;
            return SQ_ERROR;
        }
        return SQ_OK;
    }
    return sq_throwerror(v,_SC("wrong parameter"));
}

static int thread_getstatus(HSKVM v)
{
    SQObjectPtr &o = stack_get(v,1);
    switch(sq_getvmstate(_thread(o))) {
        case SQ_VMSTATE_IDLE:
            sq_pushstring(v,_SC("idle"),-1);
        break;
        case SQ_VMSTATE_RUNNING:
            sq_pushstring(v,_SC("running"),-1);
        break;
        case SQ_VMSTATE_SUSPENDED:
            sq_pushstring(v,_SC("suspended"),-1);
        break;
        default:
            return sq_throwerror(v,_SC("internal VM error"));
    }
    return 1;
}

static int thread_getstackinfos(HSKVM v)
{
    SQObjectPtr o = stack_get(v,1);
    if(sq_type(o) == OT_THREAD) {
        SQVM *thread = _thread(o);
        int threadtop = sq_gettop(thread);
        int level;
        sq_getinteger(v,-1,&level);
        SQRESULT res = __getcallstackinfos(thread,level);
        if(SQ_FAILED(res))
        {
            sq_settop(thread,threadtop);
            if(sq_type(thread->_lasterror) == OT_STRING) {
                sq_throwerror(v,_stringval(thread->_lasterror));
            }
            else {
                sq_throwerror(v,_SC("unknown error"));
            }
        }
        if(res > 0) {
            //some result
            sq_move(v,thread,-1);
            sq_settop(thread,threadtop);
            return 1;
        }
        //no result
        sq_settop(thread,threadtop);
        return 0;

    }
    return sq_throwerror(v,_SC("wrong parameter"));
}

const SQRegFunction SQSharedState::_thread_default_delegate_funcz[] = {
    {_SC("call"), thread_call, -1, _SC("v")},
    {_SC("wakeup"), thread_wakeup, -1, _SC("v")},
    {_SC("wakeupthrow"), thread_wakeupthrow, -2, _SC("v.b")},
    {_SC("getstatus"), thread_getstatus, 1, _SC("v")},
    {_SC("weakref"),obj_delegate_weakref,1, NULL },
    {_SC("getstackinfos"),thread_getstackinfos,2, _SC("vn")},
    {_SC("tostring"),default_delegate_tostring,1, _SC(".")},
    {NULL,(SQFUNCTION)0,0,NULL}
};

static int class_getattributes(HSKVM v)
{
    return SQ_SUCCEEDED(sq_getattributes(v,-2))?1:SQ_ERROR;
}

static int class_setattributes(HSKVM v)
{
    return SQ_SUCCEEDED(sq_setattributes(v,-3))?1:SQ_ERROR;
}

static int class_instance(HSKVM v)
{
    return SQ_SUCCEEDED(sq_createinstance(v,-1))?1:SQ_ERROR;
}

static int class_getbase(HSKVM v)
{
    return SQ_SUCCEEDED(sq_getbase(v,-1))?1:SQ_ERROR;
}

static int class_newmember(HSKVM v)
{
    int top = sq_gettop(v);
    bool bstatic = SQFalse;
    if(top == 5)
    {
        sq_tobool(v,-1,&bstatic);
        sq_pop(v,1);
    }

    if(top < 4) {
        sq_pushnull(v);
    }
    return SQ_SUCCEEDED(sq_newmember(v,-4,bstatic))?1:SQ_ERROR;
}

static int class_rawnewmember(HSKVM v)
{
    int top = sq_gettop(v);
    bool bstatic = SQFalse;
    if(top == 5)
    {
        sq_tobool(v,-1,&bstatic);
        sq_pop(v,1);
    }

    if(top < 4) {
        sq_pushnull(v);
    }
    return SQ_SUCCEEDED(sq_rawnewmember(v,-4,bstatic))?1:SQ_ERROR;
}

const SQRegFunction SQSharedState::_class_default_delegate_funcz[] = {
    {_SC("getattributes"), class_getattributes, 2, _SC("y.")},
    {_SC("setattributes"), class_setattributes, 3, _SC("y..")},
    {_SC("rawget"),container_rawget,2, _SC("y")},
    {_SC("rawset"),container_rawset,3, _SC("y")},
    {_SC("rawin"),container_rawexists,2, _SC("y")},
    {_SC("weakref"),obj_delegate_weakref,1, NULL },
    {_SC("tostring"),default_delegate_tostring,1, _SC(".")},
    {_SC("instance"),class_instance,1, _SC("y")},
    {_SC("getbase"),class_getbase,1, _SC("y")},
    {_SC("newmember"),class_newmember,-3, _SC("y")},
    {_SC("rawnewmember"),class_rawnewmember,-3, _SC("y")},
    {NULL,(SQFUNCTION)0,0,NULL}
};


static int instance_getclass(HSKVM v)
{
    if(SQ_SUCCEEDED(sq_getclass(v,1)))
        return 1;
    return SQ_ERROR;
}

const SQRegFunction SQSharedState::_instance_default_delegate_funcz[] = {
    {_SC("getclass"), instance_getclass, 1, _SC("x")},
    {_SC("rawget"),container_rawget,2, _SC("x")},
    {_SC("rawset"),container_rawset,3, _SC("x")},
    {_SC("rawin"),container_rawexists,2, _SC("x")},
    {_SC("weakref"),obj_delegate_weakref,1, NULL },
    {_SC("tostring"),default_delegate_tostring,1, _SC(".")},
    {NULL,(SQFUNCTION)0,0,NULL}
};

static int weakref_ref(HSKVM v)
{
    if(SQ_FAILED(sq_getweakrefval(v,1)))
        return SQ_ERROR;
    return 1;
}

const SQRegFunction SQSharedState::_weakref_default_delegate_funcz[] = {
    {_SC("ref"),weakref_ref,1, _SC("r")},
    {_SC("weakref"),obj_delegate_weakref,1, NULL },
    {_SC("tostring"),default_delegate_tostring,1, _SC(".")},
    {NULL,(SQFUNCTION)0,0,NULL}
};
