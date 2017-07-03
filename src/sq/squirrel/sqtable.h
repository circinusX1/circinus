/*  see copyright notice in squirrel.h */
#ifndef _SQTABLE_H_
#define _SQTABLE_H_

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
        case OT_FLOAT:      return (size_t)((int)_float(key));
        case OT_BOOL:
        case OT_INTEGER:    return (size_t)((int)_integer(key));
        default:            return hashptr(key._unVal.pRefCounted);
    }
}

struct SQTable : public SQDelegable
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
    int _numofnodes;
    int _usednodes;

///////////////////////////
    void AllocNodes(int nSize);
    void Rehash(bool force);
    SQTable(SQSharedState *ss, int nInitialSize);
    void _ClearNodes();
public:
    static SQTable* Create(SQSharedState *ss,int nInitialSize)
    {
        SQTable *newtable = (SQTable*)SQ_MALLOC(sizeof(SQTable));
        new (newtable) SQTable(ss, nInitialSize);
        newtable->_delegate = NULL;
        return newtable;
    }
    void Finalize();
    SQTable *Clone();
    ~SQTable()
    {
        SetDelegate(NULL);
        REMOVE_FROM_CHAIN(&_sharedstate->_gc_chain, this);
        for (int i = 0; i < _numofnodes; i++) _nodes[i].~_HashNode();
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
    inline bool GetStr(const SQChar* key,int keylen,SQObjectPtr &val)
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
    int Next(bool getweakrefs,const SQObjectPtr &refpos, SQObjectPtr &outkey, SQObjectPtr &outval);

    int CountUsed(){ return _usednodes;}
    void Clear();
    void Release()
    {
        sq_delete(this, SQTable);
    }

};

#endif //_SQTABLE_H_
