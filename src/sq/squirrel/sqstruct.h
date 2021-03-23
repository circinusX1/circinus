/*  see copyright notice in squirrel.h */
#ifndef _SQSTRUCT_H_
#define _SQSTRUCT_H_

#include <stdint.h>
/*  
* The following code is based on Lua 4.0 (Copyright 1994-2002 Tecgraf, PUC-Rio.)
* http://www.lua.org/copyright.html#4
* http://www.lua.org/source/4.0.1/src_ltable.c.html
*/
#include "sqobject.h"
#include "sqstring.h"


#define hashptr(p)  ((size_t)(((size_t)p) >> 3))

inline size_t HashObj(const SQObjectPtr &key)
{
    switch(sq_type(key)) {
        case OT_STRING:     return _string(key)->_hash;
        case OT_FLOAT:      return (size_t)((isize_t)_float(key));
        case OT_BOOL:
        case OT_INTEGER:    return (size_t)((isize_t)_integer(key));
        default:            return hashptr(key._unVal.pRefCounted);
    }
}

struct SQStruct : public SQDelegable
{
private:
    struct _HashNode
    {
        _HashNode() { next = NULL; }
        SQObjectPtr val;
        SQObjectPtr key;
        _HashNode *next;
    };
    _HashNode *_firstfree;
    _HashNode *_nodes;
    isize_t _numofnodes;
    isize_t _usednodes;

///////////////////////////
    void AllocNodes(isize_t nSize);
    void Rehash(bool force);
    SQStruct(SQSharedState *ss, isize_t nInitialSize);
    void _ClearNodes();
public:
    static SQStruct* Create(SQSharedState *ss,isize_t nInitialSize)
    {
        SQStruct *newtable = (SQStruct*)SQ_MALLOC(sizeof(SQStruct));
        new (newtable) SQStruct(ss, nInitialSize);
        newtable->_delegate = NULL;
        return newtable;
    }
    void Finalize();
    SQStruct *Clone();
    ~SQStruct()
    {
        SetDelegate(NULL);
        REMOVE_FROM_CHAIN(&_sharedstate->_gc_chain, this);
        for (isize_t i = 0; i < _numofnodes; i++) _nodes[i].~_HashNode();
        SQ_FREE(_nodes, _numofnodes * sizeof(_HashNode));
    }
#ifndef NO_GARBAGE_COLLECTOR
    void Mark(SQCollectable **chain);
    SQObjectType GetType() {return OT_TABLE;}
#endif
    inline _HashNode *_Get(const SQObjectPtr &key,size_t hash)
    {
        _HashNode *n = &_nodes[hash];
        do{
            if(_rawval(n->key) == _rawval(key) && sq_type(n->key) == sq_type(key)){
                return n;
            }
        }while((n = n->next));
        return NULL;
    }
    //for compiler use
    inline bool GetStr(const SQChar* key,isize_t keylen,SQObjectPtr &val)
    {
        size_t hash = _hashstr(key,keylen);
        _HashNode *n = &_nodes[hash & (_numofnodes - 1)];
        _HashNode *res = NULL;
        do{
            if(sq_type(n->key) == OT_STRING && (scstrcmp(_stringval(n->key),key) == 0)){
                res = n;
                break;
            }
        }while((n = n->next));
        if (res) {
            val = _realval(res->val);
            return true;
        }
        return false;
    }
    bool Get(const SQObjectPtr &key,SQObjectPtr &val);
    void Remove(const SQObjectPtr &key);
    bool Set(const SQObjectPtr &key, const SQObjectPtr &val);
    //returns true if a new slot has been created false if it was already present
    bool NewSlot(const SQObjectPtr &key,const SQObjectPtr &val);
    isize_t Next(bool getweakrefs,const SQObjectPtr &refpos, SQObjectPtr &outkey, SQObjectPtr &outval);

    isize_t CountUsed(){ return _usednodes;}
    void Clear();
    void Release()
    {
        sq_delete(this, SQStruct);
    }

};

#endif //_SQTABLE_H_
