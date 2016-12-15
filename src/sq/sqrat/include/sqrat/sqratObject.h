//
// SqratObject: Referenced Squirrel Object Wrapper
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

#if !defined(_SCRAT_OBJECT_H_)
#define _SCRAT_OBJECT_H_

#include <squirrel.h>
#include <string.h>
#include "sqr_imp_exp.h"
#include "sqratAllocator.h"
#include "sqratTypes.h"
#include "sqratOverloadMethods.h"
#include "sqratUtil.h"

namespace Sqrat {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// The base class for classes that represent Squirrel objects
///
/// \remarks
/// All Object and derived classes MUST be destroyed before calling SQ_PTRS->close or your application will crash when exiting.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Object {
protected:

/// @cond DEV
    HSKVM vm;
    HSQOBJECT obj;
    bool release;

    Object(HSKVM v, bool releaseOnDestroy = true) : vm(v), release(releaseOnDestroy) {
        SQ_PTRS->resetobject(&obj);
    }
/// @endcond

public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor (null)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object() : vm(0), release(true) {
        SQ_PTRS->resetobject(&obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param so Object to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object(const Object& so) : vm(so.vm), obj(so.obj), release(so.release) {
        SQ_PTRS->addref(vm, &obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an Object from a Squirrel object
    ///
    /// \param o Squirrel object
    /// \param v VM that the object will exist in
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object(HSQOBJECT o, HSKVM v = DefaultVM::Get()) : vm(v), obj(o), release(true) {
        SQ_PTRS->addref(vm, &obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an Object from a C++ instance
    ///
    /// \param instance Pointer to a C++ class instance that has been bound already
    /// \param v        VM that the object will exist in
    ///
    /// \tparam T Type of instance
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    Object(T* instance, HSKVM v = DefaultVM::Get()) : vm(v), release(true) {
        ClassType<T>::PushInstance(vm, instance);
        SQ_PTRS->getstackobj(vm, -1, &obj);
        SQ_PTRS->addref(vm, &obj);
    }


    template<class T>
    Object(T* instance, bool b, HSKVM v = DefaultVM::Get()) : vm(v), release(b) {
        ClassType<T>::PushInstance(vm, instance);
        SQ_PTRS->getstackobj(vm, -1, &obj);
        SQ_PTRS->addref(vm, &obj);
    }


    template <class T>
    void BindCppObject(T* instance)
    {
        Release();
        vm  = DefaultVM::Get();
        ClassType<T>::PushInstance(vm, instance);
        SQ_PTRS->getstackobj(vm, -1, &obj);
        SQ_PTRS->addref(vm, &obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Object() {
        if(release) {
            Release();
            release = false;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param so Object to copy
    ///
    /// \return The Object itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object& operator=(const Object& so) {
        if(release) {
            Release();
        }
        vm = so.vm;
        obj = so.obj;
        release = so.release;
        SQ_PTRS->addref(vm, &GetObject());
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel VM for this Object (reference)
    ///
    /// \return Squirrel VM associated with the Object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSKVM& GetVM() {
        return vm;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel VM for this Object (copy)
    ///
    /// \return Squirrel VM associated with the Object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSKVM GetVM() const {
        return vm;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the type of the Object as defined by the Squirrel API
    ///
    /// \return SQObjectType for the Object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SQObjectType GetType() const {
        return GetObject()._type;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks whether the Object is null
    ///
    /// \return True if the Object currently has a null value, otherwise false
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool IsNull() const {
        return sq_isnull(GetObject());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel object for this Object (copy)
    ///
    /// \return Squirrel object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual HSQOBJECT GetObject() const {
        return obj;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel object for this Object (reference)
    ///
    /// \return Squirrel object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual HSQOBJECT& GetObject() {
        return obj;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Allows the Object to be inputted directly into places that expect a HSQOBJECT
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    operator HSQOBJECT&() {
        return GetObject();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets the Object to null (removing its references to underlying Squirrel objects)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Release() {
        if(vm){
            SQ_PTRS->release(vm, &obj);
            SQ_PTRS->resetobject(&obj);
            vm=0;
        }
    }


    /// mco-mco

    void    AddRef()
    {
        SQ_PTRS->addref(vm, &obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value of a slot from the object
    ///
    /// \param slot Name of the slot
    ///
    /// \return An Object representing the value of the slot (can be a null object if nothing was found)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object GetSlot(const SQChar* slot) const {
        HSQOBJECT slotObj;
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushstring(vm, slot, -1);

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(SQ_PTRS->get(vm, -2))) {
            SQ_PTRS->pop(vm, 1);
            return Object(vm); // Return a NULL object
        } else {
            SQ_PTRS->getstackobj(vm, -1, &slotObj);
            Object ret(slotObj, vm); // must addref before the pop!
            SQ_PTRS->pop(vm, 2);
            return ret;
        }
#else
        SQ_PTRS->get(vm, -2);
        SQ_PTRS->getstackobj(vm, -1, &slotObj);
        Object ret(slotObj, vm); // must addref before the pop!
        SQ_PTRS->pop(vm, 2);
        return ret;
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value of an index from the object
    ///
    /// \param index Index of the slot
    ///
    /// \return An Object representing the value of the slot (can be a null object if nothing was found)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object GetSlot(int index) const {
        HSQOBJECT slotObj;
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushinteger(vm, index);

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(SQ_PTRS->get(vm, -2))) {
            SQ_PTRS->pop(vm, 1);
            return Object(vm); // Return a NULL object
        } else {
            SQ_PTRS->getstackobj(vm, -1, &slotObj);
            Object ret(slotObj, vm); // must addref before the pop!
            SQ_PTRS->pop(vm, 2);
            return ret;
        }
#else
        SQ_PTRS->get(vm, -2);
        SQ_PTRS->getstackobj(vm, -1, &slotObj);
        Object ret(slotObj, vm); // must addref before the pop!
        SQ_PTRS->pop(vm, 2);
        return ret;
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Casts the object to a certain C++ type
    ///
    /// \tparam T Type to cast to
    ///
    /// \return A copy of the value of the Object with the given type
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class T>
    T Cast(int index=-1) const {
        SQ_PTRS->pushobject(vm, GetObject());
        T ret = Var<T>(vm, index).value;
        SQ_PTRS->pop(vm, 1);
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Allows Object to be used like C++ arrays with the [] operator
    ///
    /// \param slot The slot key
    ///
    /// \tparam T Type of the slot key (usually doesnt need to be defined explicitly)
    ///
    /// \return An Object representing the value of the slot (can be a null object if nothing was found)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class T>
    inline Object operator[](T slot)
    {
        return GetSlot(slot);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns the size of the Object
    ///
    /// \return Size of Object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int GetSize() const {
        SQ_PTRS->pushobject(vm, GetObject());
        int ret = SQ_PTRS->getsize(vm, -1);
        SQ_PTRS->pop(vm, 1);
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Iterator for going over the slots in the object using Object::Next
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct iterator
    {
        /// @cond DEV
        friend class Object;
        /// @endcond

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Default constructor (null)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        iterator()
        {
            Index = 0;
            SQ_PTRS->resetobject(&Key);
            SQ_PTRS->resetobject(&Value);
            Key._type = OT_NULL;
            Value._type = OT_NULL;
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Returns the string value of the key the iterator is on if possible
        ///
        /// \return String or NULL
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const SQChar* getName() { return SQ_PTRS->objtostring(&Key); }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Gets the Squirrel object for the key the iterator is on
        ///
        /// \return HSQOBJECT representing a key
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        HSQOBJECT getKey() { return Key; }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Gets the Squirrel object for the value the iterator is on
        ///
        /// \return HSQOBJECT representing a value
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        HSQOBJECT getValue() { return Value; }
    private:

        HSQOBJECT Key;
        HSQOBJECT Value;
        int Index;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Used to go through all the slots in an Object (same limitations as SQ_PTRS->next)
    ///
    /// \param iter An iterator being used for going through the slots
    ///
    /// \return Whether there is a next slot
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool Next(iterator& iter) const
    {
        SQ_PTRS->pushobject(vm,obj);
        SQ_PTRS->pushinteger(vm,iter.Index);
        if(SQ_SUCCEEDED(SQ_PTRS->next(vm,-2)))
        {
            SQ_PTRS->getstackobj(vm,-1,&iter.Value);
            SQ_PTRS->getstackobj(vm,-2,&iter.Key);
            SQ_PTRS->getinteger(vm,-3,&iter.Index);
            SQ_PTRS->pop(vm,4);
            return true;
        }
        else
        {
            SQ_PTRS->pop(vm,2);
            return false;
        }
    }

protected:
/// @cond DEV

    // Bind a function and it's associated Squirrel closure to the object
    inline void BindMemb(const SQChar* name, void* method, size_t methodSize,
                         SQFUNCTION func,  bool staticVar = false, int nargs=0)
    {
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushstring(vm, name, -1);

        PVOID methodPtr = SQ_PTRS->newuserdata(vm, static_cast<size_t>(methodSize));
        memcpy(methodPtr, method, methodSize);

        SQ_PTRS->newclosure(vm, func, 1, nargs);
        SQ_PTRS->newslot(vm, -3, staticVar);
        SQ_PTRS->pop(vm,1); // pop table
    }

    inline void BindMemb(const int index, void* method, size_t methodSize,
                         SQFUNCTION func, bool staticVar = false) {
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushinteger(vm, index);

        PVOID methodPtr = SQ_PTRS->newuserdata(vm, static_cast<size_t>(methodSize));
        memcpy(methodPtr, method, methodSize);

        SQ_PTRS->newclosure(vm, func, 1, 0);
        SQ_PTRS->newslot(vm, -3, staticVar);
        SQ_PTRS->pop(vm,1); // pop table
    }

    inline void BindMembRt(const SQChar* name,
                           void* method,
                           size_t methodSize,
                           SQFUNCTION_RT func,
                           int nargs,
                           bool staticVar = false)
    {
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushstring(vm, name, -1);

        PVOID methodPtr = SQ_PTRS->newuserdata(vm, static_cast<size_t>(methodSize));
        memcpy(methodPtr, method, methodSize);

        SQ_PTRS->newclosure_rt(vm, func, 1, nargs);
        SQ_PTRS->newslot(vm, -3, staticVar);
        SQ_PTRS->pop(vm,1); // pop table
    }



    // Bind a function and it's associated Squirrel closure to the object
    inline void BindOverload(const SQChar* name, void* method, size_t methodSize, SQFUNCTION func, SQFUNCTION overload, int argCount, bool staticVar = false) {
        string overloadName = SqOverloadName::Get(name, argCount);

        SQ_PTRS->pushobject(vm, GetObject());

        // Bind overload handler
        SQ_PTRS->pushstring(vm, name, -1);
        SQ_PTRS->pushstring(vm, name, -1); // function name is passed as a free variable
        SQ_PTRS->newclosure(vm, overload, 1, 0);
        SQ_PTRS->newslot(vm, -3, staticVar);

        // Bind overloaded function
        SQ_PTRS->pushstring(vm, overloadName.c_str(), -1);
        PVOID methodPtr = SQ_PTRS->newuserdata(vm, static_cast<size_t>(methodSize));
        memcpy(methodPtr, method, methodSize);
        SQ_PTRS->newclosure(vm, func, 1, 0);
        SQ_PTRS->newslot(vm, -3, staticVar);

        SQ_PTRS->pop(vm,1); // pop table
    }

    // Set the value of a variable on the object. Changes to values set this way are not reciprocated
    template<class V>
    inline void BindValue(const SQChar* name, const V& val, bool staticVar = false) {
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushstring(vm, name, -1);
        PushVar(vm, val);
        SQ_PTRS->newslot(vm, -3, staticVar);
        SQ_PTRS->pop(vm,1); // pop table
    }
    template<class V>
    inline void BindValue(const int index, const V& val, bool staticVar = false) {
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushinteger(vm, index);
        PushVar(vm, val);
        SQ_PTRS->newslot(vm, -3, staticVar);
        SQ_PTRS->pop(vm,1); // pop table
    }

    // Set the value of an instance on the object. Changes to values set this way are reciprocated back to the source instance
    template<class V>
    inline void BindInstance(const SQChar* name, V* val, bool staticVar = false) {
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushstring(vm, name, -1);
        PushVar(vm, val);
        SQ_PTRS->newslot(vm, -3, staticVar);
        SQ_PTRS->pop(vm,1); // pop table
    }
    template<class V>
    inline void BindInstance(const int index, V* val, bool staticVar = false) {
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushinteger(vm, index);
        PushVar(vm, val);
        SQ_PTRS->newslot(vm, -3, staticVar);
        SQ_PTRS->pop(vm,1); // pop table
    }

/// @endcond
};

/// @cond DEV
template<>
inline void Object::BindValue<int>(const SQChar* name, const int & val, bool staticVar /* = false */) {
    SQ_PTRS->pushobject(vm, GetObject());
    SQ_PTRS->pushstring(vm, name, -1);
    PushVar<int>(vm, val);
    SQ_PTRS->newslot(vm, -3, staticVar);
    SQ_PTRS->pop(vm,1); // pop table
}
/// @endcond

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Object instances to and from the stack as references (Object is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<Object> {

    Object value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as an Object
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSKVM vm, int idx) {
        HSQOBJECT sqValue;
        SQ_PTRS->getstackobj(vm, idx, &sqValue);
        value = Object(sqValue, vm);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put an Object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSKVM vm, const Object& value) {
        SQ_PTRS->pushobject(vm, value.GetObject());
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Object instances to and from the stack as references (Object is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<Object&> : Var<Object> {Var(HSKVM vm, int idx) : Var<Object>(vm, idx) {}};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Object instances to and from the stack as references (Object is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const Object&> : Var<Object> {Var(HSKVM vm, int idx) : Var<Object>(vm, idx) {}};

}

#endif
