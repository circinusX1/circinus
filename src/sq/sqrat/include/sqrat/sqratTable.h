//
// SqratTable: Table Binding
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

#if !defined(_SCRAT_TABLE_H_)
#define _SCRAT_TABLE_H_

#include <squirrel.h>
#include <string.h>
#include "sqratObject.h"
#include "sqratFunction.h"
#include "sqratGlobalMethods.h"

namespace Sqrat {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// The base class for Table that implements almost all of its functionality
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TableBase : public Object {
public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor (null)
    ///
    /// \param v VM that the table will exist in
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TableBase(HSKVM v = DefaultVM::Get()) : Object(v, true) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the TableBase from an Object that already exists
    ///
    /// \param obj An Object that should already represent a Squirrel table
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TableBase(const Object& obj) : Object(obj) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the TableBase from a HSQOBJECT and HSKVM that already exist
    ///
    /// \param o Squirrel object that should already represent a Squirrel table
    /// \param v Squirrel VM that contains the Squirrel object given
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TableBase(HSQOBJECT o, HSKVM v = DefaultVM::Get()) : Object(o, v) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds a Table or Class to the Table (can be used to facilitate namespaces)
    ///
    /// \param name The key in the table being assigned a Table or Class
    /// \param obj  Table or Class that is being placed in the table
    ///
    /// \remarks
    /// Bind cannot be called "inline" like other functions because it introduces order-of-initialization bugs.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Bind(const SQChar* name, Object& obj) {
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushstring(vm, name, -1);
        SQ_PTRS->pushobject(vm, obj.GetObject());
        SQ_PTRS->newslot(vm, -3, false);
        SQ_PTRS->pop(vm,1); // pop table
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds a raw Squirrel closure to the Table
    ///
    /// \param name The key in the table being assigned a function
    /// \param func Squirrel function that is being placed in the Table
    ///
    /// \return The Table itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TableBase& SquirrelMemb(const SQChar* name, SQFUNCTION func) {
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushstring(vm, name, -1);
        SQ_PTRS->newclosure(vm, func, 0, 0);
        SQ_PTRS->newslot(vm, -3, false);
        SQ_PTRS->pop(vm,1); // pop table
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets a key in the Table to a specific value
    ///
    /// \param name The key in the table being assigned a value
    /// \param val  Value that is being placed in the Table
    ///
    /// \tparam V Type of value (usually doesnt need to be defined explicitly)
    ///
    /// \return The Table itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class V>
    TableBase& SetValue(const SQChar* name, const V& val) {
        BindValue<V>(name, val, false);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets an index in the Table to a specific value
    ///
    /// \param index The index in the table being assigned a value
    /// \param val   Value that is being placed in the Table
    ///
    /// \tparam V Type of value (usually doesnt need to be defined explicitly)
    ///
    /// \return The Table itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class V>
    TableBase& SetValue(const isize_t index, const V& val) {
        BindValue<V>(index, val, false);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets a key in the Table to a specific instance (like a reference)
    ///
    /// \param name The key in the table being assigned a value
    /// \param val  Pointer to the instance that is being placed in the Table
    ///
    /// \tparam V Type of instance (usually doesnt need to be defined explicitly)
    ///
    /// \return The Table itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class V>
    TableBase& SetInstance(const SQChar* name, V* val) {
        BindInstance<V>(name, val, false);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets an index in the Table to a specific instance (like a reference)
    ///
    /// \param index The index in the table being assigned a value
    /// \param val   Pointer to the instance that is being placed in the Table
    ///
    /// \tparam V Type of instance (usually doesnt need to be defined explicitly)
    ///
    /// \return The Table itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class V>
    TableBase& SetInstance(const isize_t index, V* val) {
        BindInstance<V>(index, val, false);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets a key in the Table to a specific function
    ///
    /// \param name   The key in the table being assigned a value
    /// \param method Function that is being placed in the Table
    ///
    /// \tparam F Type of function (only define this if you need to choose a certain template specialization or overload)
    ///
    /// \return The Table itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class F>
    TableBase& Functor(const SQChar* name, F method, isize_t nargs=0) {
        BindMemb(name, &method, sizeof(method), SqGlobalMemb(method), false, nargs);
        return *this;
    }

    template<class F>
    TableBase& MembRt(const SQChar* name, F method, isize_t nargs) {
        BindMembRt(name, &method, sizeof(method), SqGlobalMembRt(method), nargs);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets a key in the Table to a specific function and allows the key to be overloaded with functions of a different amount of arguments
    ///
    /// \param name   The key in the table being assigned a value
    /// \param method Function that is being placed in the Table
    ///
    /// \tparam F Type of function (only define this if you need to choose a certain template specialization or overload)
    ///
    /// \return The Table itself so the call can be chained
    ///
    /// \remarks
    /// Overloading in Sqrat does not work for functions with the same amount of arguments (just like in Squirrel).
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class F>
    TableBase& Overload(const SQChar* name, F method) {
        BindOverload(name, &method, sizeof(method), SqGlobalOverloadedMemb(method), SqOverloadMemb(method), SqGetArgCount(method));
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks if the given key exists in the table
    ///
    /// \param name Key to check
    ///
    /// \return True on success, otherwise false
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool HasKey(const SQChar* name)
    {
        SQ_PTRS->pushobject(vm, obj);
        SQ_PTRS->pushstring(vm, name, -1);
        if (SQ_FAILED(SQ_PTRS->get(vm, -2))) {
            SQ_PTRS->pop(vm, 1);
            return false;
        }
        SQ_PTRS->pop(vm, 2);
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns the value at a given key
    ///
    /// \param name Key of the element
    ///
    /// \tparam T Type of value (fails if value is not of this type)
    ///
    /// \return SharedPtr containing the value (or null if failed)
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    SharedPtr<T> GetValue(const SQChar* name)
    {
        SQ_PTRS->pushobject(vm, obj);
        SQ_PTRS->pushstring(vm, name, -1);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (SQ_FAILED(SQ_PTRS->get(vm, -2))) {
            SQ_PTRS->pop(vm, 1);
            SQTHROW(vm, _SC("illegal index"));
            return SharedPtr<T>();
        }
#else
        SQ_PTRS->get(vm, -2);
#endif
        SQTRY()
        Var<SharedPtr<T> > entry(vm, -1);
        SQCATCH_NOEXCEPT(vm) {
            SQ_PTRS->pop(vm, 2);
            return SharedPtr<T>();
        }
        SQ_PTRS->pop(vm, 2);
        return entry.value;
        SQCATCH(vm) {
#if defined (SCRAT_USE_EXCEPTIONS)
            SQUNUSED(e); // avoid "unreferenced local variable" warning
#endif
            SQ_PTRS->pop(vm, 2);
            SQRETHROW(vm);
        }
        return SharedPtr<T>(); // avoid "not all control paths return a value" warning
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns the value at a given index
    ///
    /// \param index Index of the element
    ///
    /// \tparam T Type of value (fails if value is not of this type)
    ///
    /// \return SharedPtr containing the value (or null if failed)
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    SharedPtr<T> GetValue(isize_t index)
    {
        SQ_PTRS->pushobject(vm, obj);
        SQ_PTRS->pushinteger(vm, index);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (SQ_FAILED(SQ_PTRS->get(vm, -2))) {
            SQ_PTRS->pop(vm, 1);
            SQTHROW(vm, _SC("illegal index"));
            return SharedPtr<T>();
        }
#else
        SQ_PTRS->get(vm, -2);
#endif
        SQTRY()
        Var<SharedPtr<T> > entry(vm, -1);
        SQCATCH_NOEXCEPT(vm) {
            SQ_PTRS->pop(vm, 2);
            return SharedPtr<T>();
        }
        SQ_PTRS->pop(vm, 2);
        return entry.value;
        SQCATCH(vm) {
#if defined (SCRAT_USE_EXCEPTIONS)
            SQUNUSED(e); // avoid "unreferenced local variable" warning
#endif
            SQ_PTRS->pop(vm, 2);
            SQRETHROW(vm);
        }
        return SharedPtr<T>(); // avoid "not all control paths return a value" warning
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets a Function from a key in the Table
    ///
    /// \param name The key in the table that contains the Function
    ///
    /// \return Function found in the Table (null if failed)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Function GetFunction(const SQChar* name) {
        HSQOBJECT funcObj;
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushstring(vm, name, -1);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(SQ_PTRS->get(vm, -2))) {
            SQ_PTRS->pop(vm, 1);
            return Function();
        }
        SQObjectType value_type = SQ_PTRS->gettype(vm, -1);
        if (value_type != OT_CLOSURE && value_type != OT_NATIVECLOSURE) {
            SQ_PTRS->pop(vm, 2);
            return Function();
        }
#else
        SQ_PTRS->get(vm, -2);
#endif
        SQ_PTRS->getstackobj(vm, -1, &funcObj);
        Function ret(vm, GetObject(), funcObj); // must addref before the pop!
        SQ_PTRS->pop(vm, 2);
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets a Function from an index in the Table
    ///
    /// \param index The index in the table that contains the Function
    ///
    /// \return Function found in the Table (null if failed)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Function GetFunction(const isize_t index) {
        HSQOBJECT funcObj;
        SQ_PTRS->pushobject(vm, GetObject());
        SQ_PTRS->pushinteger(vm, index);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(SQ_PTRS->get(vm, -2))) {
            SQ_PTRS->pop(vm, 1);
            return Function();
        }
        SQObjectType value_type = SQ_PTRS->gettype(vm, -1);
        if (value_type != OT_CLOSURE && value_type != OT_NATIVECLOSURE) {
            SQ_PTRS->pop(vm, 2);
            return Function();
        }
#else
        SQ_PTRS->get(vm, -2);
#endif
        SQ_PTRS->getstackobj(vm, -1, &funcObj);
        Function ret(vm, GetObject(), funcObj); // must addref before the pop!
        SQ_PTRS->pop(vm, 2);
        return ret;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a table in Squirrel
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Table : public TableBase {
public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor (null)
    ///
    /// \remarks
    /// The Table is invalid until it is given a VM to exist in.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Table() {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a Table
    ///
    /// \param v VM to create the Table in
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Table(HSKVM v) : TableBase(v) {
        SQ_PTRS->newtable(vm);
        SQ_PTRS->getstackobj(vm,-1,&obj);
        SQ_PTRS->addref(vm, &obj);
        SQ_PTRS->pop(vm,1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the Table from an Object that already exists
    ///
    /// \param obj An Object that should already represent a Squirrel table
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Table(const Object& obj) : TableBase(obj) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the Table from a HSQOBJECT and HSKVM that already exist
    ///
    /// \param o Squirrel object that should already represent a Squirrel table
    /// \param v Squirrel VM that contains the Squirrel object given
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Table(HSQOBJECT o, HSKVM v = DefaultVM::Get()) : TableBase(o, v) {
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Table that is a reference to the Squirrel root table for a given VM
/// The Squirrel root table is usually where all globals are stored by the Squirrel language.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RootTable : public TableBase {
public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a RootTable object to represent the given VM's root table
    ///
    /// \param v VM to get the RootTable for
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RootTable(HSKVM v = DefaultVM::Get()) : TableBase(v) {
        SQ_PTRS->pushroottable(vm);
        SQ_PTRS->getstackobj(vm,-1,&obj);
        SQ_PTRS->addref(vm, &obj);
        SQ_PTRS->pop(v,1); // pop root table
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Table that is a reference to the Squirrel registry table for a given VM
/// The Squirrel registry table is where non-Squirrel code can store Squirrel objects without worrying about Squirrel code messing with them.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RegistryTable : public TableBase {
public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a RegistryTable object to represent the given VM's registry table
    ///
    /// \param v VM to get the RegistryTable for
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RegistryTable(HSKVM v = DefaultVM::Get()) : TableBase(v) {
        SQ_PTRS->pushregistrytable(v);
        SQ_PTRS->getstackobj(vm,-1,&obj);
        SQ_PTRS->addref(vm, &obj);
        SQ_PTRS->pop(v,1); // pop the registry table
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Table instances to and from the stack as references (tables are always references in Squirrel)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<Table> {

    Table value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a Table
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSKVM vm, isize_t idx) {
        HSQOBJECT obj;
        SQ_PTRS->resetobject(&obj);
        SQ_PTRS->getstackobj(vm,idx,&obj);
        value = Table(obj, vm);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQObjectType value_type = SQ_PTRS->gettype(vm, idx);
        if (value_type != OT_TABLE) {
            SQTHROW(vm, FormatTypeError(vm, idx, _SC("table")));
        }
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put an Table reference on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSKVM vm, const Table& value) {
        HSQOBJECT obj;
        SQ_PTRS->resetobject(&obj);
        obj = value.GetObject();
        SQ_PTRS->pushobject(vm,obj);
    }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Table instances to and from the stack as references (tables are always references in Squirrel)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<Table&> : Var<Table> {Var(HSKVM vm, isize_t idx) : Var<Table>(vm, idx) {}};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Table instances to and from the stack as references (tables are always references in Squirrel)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const Table&> : Var<Table> {Var(HSKVM vm, isize_t idx) : Var<Table>(vm, idx) {}};

}

#endif
