/*  see copyright notice in squirrel.h */
#ifndef _SQARRAY_H_
#define _SQARRAY_H_

struct SQArray : public CHAINABLE_OBJ
{
private:
    SQArray(SQSharedState *ss,int nsize){_values.resize(nsize); INIT_CHAIN();ADD_TO_CHAIN(&_ss(this)->_gc_chain,this);}
    ~SQArray()
    {
        REMOVE_FROM_CHAIN(&_ss(this)->_gc_chain,this);
    }
public:
    static SQArray* Create(SQSharedState *ss,int nInitialSize){
        SQArray *newarray=(SQArray*)SQ_MALLOC(sizeof(SQArray));
        new (newarray) SQArray(ss,nInitialSize);
        return newarray;
    }
#ifndef NO_GARBAGE_COLLECTOR
    void Mark(SQCollectable **chain);
    SQObjectType GetType() {return OT_ARRAY;}
#endif
    void Finalize(){
        _values.resize(0);
    }
    bool Get(const int nidx,SQObjectPtr &val)
    {
        if(nidx>=0 && nidx<(int)_values.size()){
            SQObjectPtr &o = _values[nidx];
            val = _realval(o);
            return true;
        }
        else return false;
    }
    bool Set(const int nidx,const SQObjectPtr &val)
    {
        if(nidx>=0 && nidx<(int)_values.size()){
            _values[nidx]=val;
            return true;
        }
        else return false;
    }
    int Next(const SQObjectPtr &refpos,SQObjectPtr &outkey,SQObjectPtr &outval)
    {
        size_t idx=TranslateIndex(refpos);
        while(idx<_values.size()){
            //first found
            outkey=(int)idx;
            SQObjectPtr &o = _values[idx];
            outval = _realval(o);
            //return idx for the next iteration
            return ++idx;
        }
        //nothing to iterate anymore
        return -1;
    }
    SQArray *Clone(){SQArray *anew=Create(_opt_ss(this),0); anew->_values.copy(_values); return anew; }
    int Size() const {return _values.size();}
    void Resize(int size)
    {
        SQObjectPtr _null;
        Resize(size,_null);
    }
    void Resize(int size,SQObjectPtr &fill) { _values.resize(size,fill); ShrinkIfNeeded(); }
    void Reserve(int size) { _values.reserve(size); }
    void Append(const SQObject &o){_values.push_back(o);}
    void Extend(const SQArray *a);
    SQObjectPtr &Top(){return _values.top();}
    void Pop(){_values.pop_back(); ShrinkIfNeeded(); }
    bool Insert(int idx,const SQObject &val){
        if(idx < 0 || idx > (int)_values.size())
            return false;
        _values.insert(idx,val);
        return true;
    }
    void ShrinkIfNeeded() {
        if(_values.size() <= _values.capacity()>>2) //shrink the array
            _values.shrinktofit();
    }
    bool Remove(int idx){
        if(idx < 0 || idx >= (int)_values.size())
            return false;
        _values.remove(idx);
        ShrinkIfNeeded();
        return true;
    }
    void Release()
    {
        sq_delete(this,SQArray);
    }

    SQObjectPtrVec _values;
};
#endif //_SQARRAY_H_
