//
// SqratMemberMethods: Member Methods
//

//
// Copyright (c) 2009 Brandon Jones
// Copyright 2011 Li-Cheng (Andy) Tai
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

#if !defined(_SCRAT_MEMBER_METHODS_H_)
#define _SCRAT_MEMBER_METHODS_H_

#include <squirrel.h>
#include "sq_modapi.h"
#include "sqratTypes.h"
#include "sq_thrlock.h"

namespace Sqrat {

/// @cond DEV

//
// Squirrel Global Functions
//

template <class C, class R>
class SqMember {
public:

    // Arg Count 0
    template <bool overloaded /*= false*/>
    static isize_t Memb0(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (C::*M)();
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()

        R ret = (ptr->*method)();
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <bool overloaded /*= false*/>
    static isize_t Memb0C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)() const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        R ret = (ptr->*method)();
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 1
    template <class A1, bool overloaded /*= false*/>
    static isize_t Memb1(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 3) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            ///assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        APP_LOCK()
        R ret = (ptr->*method)(
                    a1.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, bool overloaded /*= false*/>
    static isize_t Memb1C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 3) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        APP_LOCK()
        R ret = (ptr->*method)(
                    a1.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 2
    template <class A1, class A2, bool overloaded /*= false*/>
    static isize_t Memb2(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 4) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        APP_LOCK()
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, bool overloaded /*= false*/>
    static isize_t Memb2C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 4) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        APP_LOCK()
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 3
    template <class A1, class A2, class A3, bool overloaded /*= false*/>
    static isize_t Memb3(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 5) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        APP_LOCK()
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, bool overloaded /*= false*/>
    static isize_t Memb3C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 5) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        APP_LOCK()
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 4
    template <class A1, class A2, class A3, class A4, bool overloaded /*= false*/>
    static isize_t Memb4(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 6) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        APP_LOCK()
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, bool overloaded /*= false*/>
    static isize_t Memb4C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 6) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        APP_LOCK()
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 5
    template <class A1, class A2, class A3, class A4, class A5, bool overloaded /*= false*/>
    static isize_t Memb5(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 7) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        APP_LOCK()
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }


    template <class A1, class A2, class A3, class A4, class A5, bool overloaded /*= false*/>
    static isize_t Memb5C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 7) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 6
    template <class A1, class A2, class A3, class A4, class A5, class A6, bool overloaded /*= false*/>
    static isize_t Memb6(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 8) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, bool overloaded /*= false*/>
    static isize_t Memb6C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 8) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 7
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, bool overloaded /*= false*/>
    static isize_t Memb7(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 9) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, bool overloaded /*= false*/>
    static isize_t Memb7C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 9) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 8
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, bool overloaded /*= false*/>
    static isize_t Memb8(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 10) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, bool overloaded /*= false*/>
    static isize_t Memb8C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 10) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 9
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, bool overloaded /*= false*/>
    static isize_t Memb9(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 11) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, bool overloaded /*= false*/>
    static isize_t Memb9C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 11) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }


    // Arg Count 10
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, bool overloaded /*= false*/>
    static isize_t Memb10(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 12) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, bool overloaded  /*= false*/ >
    static isize_t Memb10C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 12) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }


    // Arg Count 11
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, bool overloaded /*= false*/>
    static isize_t Memb11(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 13) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, bool overloaded /*= false*/>
    static isize_t Memb11C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 13) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }


    // Arg Count 12
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, bool overloaded /*= false*/  >
    static isize_t Memb12(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 14) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value,
                    a12.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, bool overloaded /*= false*/>
    static isize_t Memb12C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 14) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value,
                    a12.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 13
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, bool overloaded /*= false*/>
    static isize_t Memb13(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 15) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        Var<A13> a13(vm, 14);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value,
                    a12.value,
                    a13.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, bool overloaded /*= false*/>
    static isize_t Memb13C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 15) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        Var<A13> a13(vm, 14);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value,
                    a12.value,
                    a13.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 14
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, bool overloaded /*= false*/>
    static isize_t Memb14(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 16) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        Var<A13> a13(vm, 14);
        Var<A14> a14(vm, 15);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value,
                    a12.value,
                    a13.value,
                    a14.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, bool overloaded /*= false*/>
    static isize_t Memb14C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 16) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        Var<A13> a13(vm, 14);
        Var<A14> a14(vm, 15);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value,
                    a12.value,
                    a13.value,
                    a14.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }
};


//
// reference return specialization
//

template <class C, class R>
class SqMember<C, R&> {
public:

    // Arg Count 0
    template <bool overloaded /*= false*/>
    static isize_t Memb0(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)();
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        R& ret = (ptr->*method)();
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <bool overloaded /*= false*/>
    static isize_t Memb0C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)() const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        R& ret = (ptr->*method)();
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 1
    template <class A1, bool overloaded /*= false*/>
    static isize_t Memb1(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 3) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, bool overloaded /*= false*/>
    static isize_t Memb1C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 3) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 2
    template <class A1, class A2, bool overloaded /*= false*/>
    static isize_t Memb2(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 4) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, bool overloaded /*= false*/>
    static isize_t Memb2C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 4) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 3
    template <class A1, class A2, class A3, bool overloaded /*= false*/>
    static isize_t Memb3(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 5) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, bool overloaded /*= false*/>
    static isize_t Memb3C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 5) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 4
    template <class A1, class A2, class A3, class A4, bool overloaded /*= false*/>
    static isize_t Memb4(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 6) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, bool overloaded /*= false*/>
    static isize_t Memb4C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 6) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 5
    template <class A1, class A2, class A3, class A4, class A5, bool overloaded /*= false*/>
    static isize_t Memb5(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 7) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, bool overloaded /*= false*/>
    static isize_t Memb5C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 7) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 6
    template <class A1, class A2, class A3, class A4, class A5, class A6, bool overloaded /*= false*/>
    static isize_t Memb6(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 8) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, bool overloaded /*= false*/>
    static isize_t Memb6C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 8) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 7
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, bool overloaded /*= false*/>
    static isize_t Memb7(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 9) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, bool overloaded /*= false*/>
    static isize_t Memb7C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 9) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 8
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, bool overloaded /*= false*/>
    static isize_t Memb8(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 10) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, bool overloaded /*= false*/>
    static isize_t Memb8C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 10) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 9
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, bool overloaded /*= false*/>
    static isize_t Memb9(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 11) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, bool overloaded /*= false*/>
    static isize_t Memb9C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 11) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }


    // Arg Count 10
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, bool overloaded /*= false*/>
    static isize_t Memb10(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 12) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, bool overloaded  /*= false*/ >
    static isize_t Memb10C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 12) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }


    // Arg Count 11
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, bool overloaded /*= false*/>
    static isize_t Memb11(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 13) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, bool overloaded /*= false*/>
    static isize_t Memb11C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 13) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }


    // Arg Count 12
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, bool overloaded /*= false*/  >
    static isize_t Memb12(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 14) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value,
                    a12.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, bool overloaded /*= false*/>
    static isize_t Memb12C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 14) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value,
                    a12.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 13
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, bool overloaded /*= false*/>
    static isize_t Memb13(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 15) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R&(C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        Var<A13> a13(vm, 14);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value,
                    a12.value,
                    a13.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, bool overloaded /*= false*/>
    static isize_t Memb13C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 15) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        Var<A13> a13(vm, 14);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value,
                    a12.value,
                    a13.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 14
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, bool overloaded /*= false*/>
    static isize_t Memb14(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 16) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        Var<A13> a13(vm, 14);
        Var<A14> a14(vm, 15);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value,
                    a12.value,
                    a13.value,
                    a14.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, bool overloaded /*= false*/>
    static isize_t Memb14C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 16) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef R& (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        Var<A13> a13(vm, 14);
        Var<A14> a14(vm, 15);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (ptr->*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    a9.value,
                    a10.value,
                    a11.value,
                    a12.value,
                    a13.value,
                    a14.value
                );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

};


//
// void return specialization
//

template <class C>
class SqMember<C, void> {
public:

    // Arg Count 0
    template <bool overloaded /*= false*/>
    static isize_t Memb0(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)();
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        (ptr->*method)();
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <bool overloaded /*= false*/>
    static isize_t Memb0C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)() const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        (ptr->*method)();
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 1
    template <class A1, bool overloaded /*= false*/>
    static isize_t Memb1(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 3) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, bool overloaded /*= false*/>
    static isize_t Memb1C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 3) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 2
    template <class A1, class A2, bool overloaded /*= false*/>
    static isize_t Memb2(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 4) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, bool overloaded /*= false*/>
    static isize_t Memb2C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 4) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 3
    template <class A1, class A2, class A3, bool overloaded /*= false*/>
    static isize_t Memb3(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 5) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, class A3, bool overloaded /*= false*/>
    static isize_t Memb3C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 5) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 4
    template <class A1, class A2, class A3, class A4, bool overloaded /*= false*/>
    static isize_t Memb4(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 6) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, class A3, class A4, bool overloaded /*= false*/>
    static isize_t Memb4C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 6) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 5
    template <class A1, class A2, class A3, class A4, class A5, bool overloaded /*= false*/>
    static isize_t Memb5(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 7) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, class A3, class A4, class A5, bool overloaded /*= false*/>
    static isize_t Memb5C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 7) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 6
    template <class A1, class A2, class A3, class A4, class A5, class A6, bool overloaded /*= false*/>
    static isize_t Memb6(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 8) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, bool overloaded /*= false*/>
    static isize_t Memb6C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 8) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 7
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, bool overloaded /*= false*/>
    static isize_t Memb7(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 9) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, bool overloaded /*= false*/>
    static isize_t Memb7C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 9) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 8
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, bool overloaded /*= false*/>
    static isize_t Memb8(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 10) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, bool overloaded /*= false*/>
    static isize_t Memb8C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 10) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 9
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, bool overloaded /*= false*/>
    static isize_t Memb9(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 11) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, bool overloaded /*= false*/>
    static isize_t Memb9C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 11) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 10
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, bool overloaded /*= false*/>
    static isize_t Memb10(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 12) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value,
            a10.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, bool overloaded /*= false*/>
    static isize_t Memb10C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 12) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value,
            a10.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 11
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, bool overloaded /*= false*/>
    static isize_t Memb11(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 13) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value,
            a10.value,
            a11.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, bool overloaded /*= false*/>
    static isize_t Memb11C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 13) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value,
            a10.value,
            a11.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 12
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, bool overloaded /*= false*/>
    static isize_t Memb12(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 14) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value,
            a10.value,
            a11.value,
            a12.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, bool overloaded /*= false*/>
    static isize_t Memb12C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 14) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value,
            a10.value,
            a11.value,
            a12.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 13
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, bool overloaded /*= false*/>
    static isize_t Memb13(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 15) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        Var<A13> a13(vm, 14);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value,
            a10.value,
            a11.value,
            a12.value,
            a13.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, bool overloaded /*= false*/>
    static isize_t Memb13C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 15) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        Var<A13> a13(vm, 14);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value,
            a10.value,
            a11.value,
            a12.value,
            a13.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 14
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, bool overloaded /*= false*/>
    static isize_t Memb14(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 16) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14);
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        Var<A13> a13(vm, 14);
        Var<A14> a14(vm, 15);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value,
            a10.value,
            a11.value,
            a12.value,
            a13.value,
            a14.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, bool overloaded /*= false*/>
    static isize_t Memb14C(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 16) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
        typedef void (C::*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14) const;
        M* methodPtr;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&methodPtr, NULL);
        M method = *methodPtr;

        C* ptr;
        SQTRY()
        ptr = Var<C*>(vm, 1).value;
        SQCATCH_NOEXCEPT(vm) {
            SQCLEAR(vm); // clear the previous error
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            assert(0); // may fail because C is not a type bound in the VM
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }

        SQTRY()
        Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        Var<A10> a10(vm, 11);
        Var<A11> a11(vm, 12);
        Var<A12> a12(vm, 13);
        Var<A13> a13(vm, 14);
        Var<A14> a14(vm, 15);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (ptr->*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value,
            a10.value,
            a11.value,
            a12.value,
            a13.value,
            a14.value
        );
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

};


//
// Member Function Resolvers
//

// Arg Count 0
template <class C, class R>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)()) {
    return &SqMember<C, R>::template Memb0<false>;
}

template <class C, class R>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)() const) {
    return &SqMember<C, R>::template Memb0C<false>;
}

template <class C, class R>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)()) {
    return &SqMember<C, R&>::template Memb0<false>;
}

template <class C, class R>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)() const) {
    return &SqMember<C, R&>::template Memb0C<false>;
}

// Arg Count 1
template <class C, class R, class A1>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1)) {
    return &SqMember<C, R>::template Memb1<A1, false>;
}

template <class C, class R, class A1>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1) const) {
    return &SqMember<C, R>::template Memb1C<A1, false>;
}

template <class C, class R, class A1>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1)) {
    return &SqMember<C, R&>::template Memb1<A1, false>;
}

template <class C, class R, class A1>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1) const) {
    return &SqMember<C, R&>::template Memb1C<A1, false>;
}

// Arg Count 2
template <class C, class R, class A1, class A2>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2)) {
    return &SqMember<C, R>::template Memb2<A1, A2, false>;
}

template <class C, class R, class A1, class A2>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2) const) {
    return &SqMember<C, R>::template Memb2C<A1, A2, false>;
}

template <class C, class R, class A1, class A2>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2)) {
    return &SqMember<C, R&>::template Memb2<A1, A2, false>;
}

template <class C, class R, class A1, class A2>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2) const) {
    return &SqMember<C, R&>::template Memb2C<A1, A2, false>;
}

// Arg Count 3
template <class C, class R, class A1, class A2, class A3>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3)) {
    return &SqMember<C, R>::template Memb3<A1, A2, A3, false>;
}

template <class C, class R, class A1, class A2, class A3>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3) const) {
    return &SqMember<C, R>::template Memb3C<A1, A2, A3, false>;
}

template <class C, class R, class A1, class A2, class A3>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3)) {
    return &SqMember<C, R&>::template Memb3<A1, A2, A3, false>;
}

template <class C, class R, class A1, class A2, class A3>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3) const) {
    return &SqMember<C, R&>::template Memb3C<A1, A2, A3, false>;
}

// Arg Count 4
template <class C, class R, class A1, class A2, class A3, class A4>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4)) {
    return &SqMember<C, R>::template Memb4<A1, A2, A3, A4, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4) const) {
    return &SqMember<C, R>::template Memb4C<A1, A2, A3, A4, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4)) {
    return &SqMember<C, R&>::template Memb4<A1, A2, A3, A4, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4) const) {
    return &SqMember<C, R&>::template Memb4C<A1, A2, A3, A4, false>;
}

// Arg Count 5
template <class C, class R, class A1, class A2, class A3, class A4, class A5>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5)) {
    return &SqMember<C, R>::template Memb5<A1, A2, A3, A4, A5, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5) const) {
    return &SqMember<C, R>::template Memb5C<A1, A2, A3, A4, A5, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5)) {
    return &SqMember<C, R&>::template Memb5<A1, A2, A3, A4, A5, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5>
inline SQFUNCTION SqMemberMemb(R&(C::* /*method*/)(A1, A2, A3, A4, A5) const) {
    return &SqMember<C, R&>::template Memb5C<A1, A2, A3, A4, A5, false>;
}

// Arg Count 6
template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6)) {
    return &SqMember<C, R>::template Memb6<A1, A2, A3, A4, A5, A6, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6) const) {
    return &SqMember<C, R>::template Memb6C<A1, A2, A3, A4, A5, A6, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6)) {
    return &SqMember<C, R&>::template Memb6<A1, A2, A3, A4, A5, A6, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6) const) {
    return &SqMember<C, R&>::template Memb6C<A1, A2, A3, A4, A5, A6, false>;
}

// Arg Count 7
template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7)) {
    return &SqMember<C, R>::template Memb7<A1, A2, A3, A4, A5, A6, A7, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7) const) {
    return &SqMember<C, R>::template Memb7C<A1, A2, A3, A4, A5, A6, A7, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7)) {
    return &SqMember<C, R&>::template Memb7<A1, A2, A3, A4, A5, A6, A7, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7) const) {
    return &SqMember<C, R&>::template Memb7C<A1, A2, A3, A4, A5, A6, A7, false>;
}

// Arg Count 8
template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8)) {
    return &SqMember<C, R>::template Memb8<A1, A2, A3, A4, A5, A6, A7, A8, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8) const) {
    return &SqMember<C, R>::template Memb8C<A1, A2, A3, A4, A5, A6, A7, A8, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8)) {
    return &SqMember<C, R&>::template Memb8<A1, A2, A3, A4, A5, A6, A7, A8, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8) const) {
    return &SqMember<C, R&>::template Memb8C<A1, A2, A3, A4, A5, A6, A7, A8, false>;
}

// Arg Count 9
template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9)) {
    return &SqMember<C, R>::template Memb9<A1, A2, A3, A4, A5, A6, A7, A8, A9, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9) const) {
    return &SqMember<C, R>::template Memb9C<A1, A2, A3, A4, A5, A6, A7, A8, A9, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
inline SQFUNCTION SqMemberMemb(R&(C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9)) {
    return &SqMember<C, R&>::template Memb9<A1, A2, A3, A4, A5, A6, A7, A8, A9, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9) const) {
    return &SqMember<C, R&>::template Memb9C<A1, A2, A3, A4, A5, A6, A7, A8, A9, false>;
}

// Arg Count 10
template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)) {
    return &SqMember<C, R>::template Memb10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) const) {
    return &SqMember<C, R>::template Memb10C<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, false>;
}
template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)) {
    return &SqMember<C, R&>::template Memb10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) const) {
    return &SqMember<C, R&>::template Memb10C<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, false>;
}

// Arg Count 11
template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11)) {
    return &SqMember<C, R>::template Memb11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11) const) {
    return &SqMember<C, R>::template Memb11C<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, false>;
}
template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11)) {
    return &SqMember<C, R&>::template Memb11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11) const) {
    return &SqMember<C, R&>::template Memb11C<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, false>;
}

// Arg Count 12
template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12)) {
    return &SqMember<C, R>::template Memb12<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12) const) {
    return &SqMember<C, R>::template Memb12C<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12)) {
    return &SqMember<C, R&>::template Memb12<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12) const) {
    return &SqMember<C, R&>::template Memb12C<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, false>;
}

// Arg Count 13
template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13)) {
    return &SqMember<C, R>::template Memb13<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13) const) {
    return &SqMember<C, R>::template Memb13C<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13)) {
    return &SqMember<C, R&>::template Memb13<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13) const) {
    return &SqMember<C, R&>::template Memb13C<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, false>;
}

// Arg Count 14
template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14)) {
    return &SqMember<C, R>::template Memb14<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
inline SQFUNCTION SqMemberMemb(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14) const) {
    return &SqMember<C, R>::template Memb14C<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14)) {
    return &SqMember<C, R&>::template Memb14<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, false>;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
inline SQFUNCTION SqMemberMemb(R& (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14) const) {
    return &SqMember<C, R&>::template Memb14C<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, false>;
}


//
// Variable Get
//

template <class C, class V>
inline isize_t sqDefaultGet(HSKVM vm) {
    C* ptr;
    SQTRY()
    ptr = Var<C*>(vm, 1).value;
    SQCATCH_NOEXCEPT(vm) {
        SQCLEAR(vm); // clear the previous error
        return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
    }
    SQCATCH(vm) {
        return SQ_PTRS->throwerror(vm, SQWHAT(vm));
    }

    typedef V C::*M;
    M* memberPtr = NULL;
    SQ_PTRS->getuserdata(vm, -1, (PVOID*)&memberPtr, NULL); // Get Member...
    M member = *memberPtr;

    PushVarR(vm, ptr->*member);

    return 1;
}

template <class C, class V>
inline isize_t sqStaticGet(HSKVM vm) {
    typedef V *M;
    M* memberPtr = NULL;
    SQ_PTRS->getuserdata(vm, -1, (PVOID*)&memberPtr, NULL); // Get Member...
    M member = *memberPtr;

    PushVarR(vm, *member);

    return 1;
}

inline isize_t sqVarGet(HSKVM vm) {
    // Find the get method in the get table
    SQ_PTRS->push(vm, 2);
#if !defined (SCRAT_NO_ERROR_CHECKING)
    if (SQ_FAILED(SQ_PTRS->rawget(vm, -2))) {
#if (SQUIRREL_VERSION_NUMBER>= 200) && (SQUIRREL_VERSION_NUMBER < 300) // Squirrel 2.x
        return SQ_PTRS->throwerror(vm, _SC("member variable not found"));
#else // Squirrel 3.x
        SQ_PTRS->pushnull(vm);
        return SQ_PTRS->throwobject(vm);
#endif
    }
#else
    SQ_PTRS->rawget(vm, -2);
#endif

    // push 'this'
    SQ_PTRS->push(vm, 1);

    // Call the getter
#if !defined (SCRAT_NO_ERROR_CHECKING)
    SQRESULT result = SQ_PTRS->call(vm, 1, true, ErrorHandling::IsEnabled());
    if (SQ_FAILED(result)) {
        return SQ_PTRS->throwerror(vm, LastErrorString(vm).c_str());
    }
#else
    SQ_PTRS->call(vm, 1, true, ErrorHandling::IsEnabled());
#endif

    return 1;
}


//
// Variable Set
//

template <class C, class V>
inline isize_t sqDefaultSet(HSKVM vm) {
    C* ptr;
    SQTRY()
    ptr = Var<C*>(vm, 1).value;
    SQCATCH_NOEXCEPT(vm) {
        SQCLEAR(vm); // clear the previous error
        return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
    }
    SQCATCH(vm) {
        return SQ_PTRS->throwerror(vm, SQWHAT(vm));
    }

    typedef V C::*M;
    M* memberPtr = NULL;
    SQ_PTRS->getuserdata(vm, -1, (PVOID*)&memberPtr, NULL); // Get Member...
    M member = *memberPtr;

    SQTRY()
    if (is_pointer<V>::value || is_reference<V>::value) {
        ptr->*member = Var<V>(vm, 2).value;
    } else {
        ptr->*member = Var<const V&>(vm, 2).value;
    }
    SQCATCH_NOEXCEPT(vm) {
        return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
    }
    SQCATCH(vm) {
        return SQ_PTRS->throwerror(vm, SQWHAT(vm));
    }

    return 0;
}

template <class C, class V>
inline isize_t sqStaticSet(HSKVM vm) {
    typedef V *M;
    M* memberPtr = NULL;
    SQ_PTRS->getuserdata(vm, -1, (PVOID*)&memberPtr, NULL); // Get Member...
    M member = *memberPtr;

    SQTRY()
    if (is_pointer<V>::value || is_reference<V>::value) {
        *member = Var<V>(vm, 2).value;
    } else {
        *member = Var<const V&>(vm, 2).value;
    }
    SQCATCH_NOEXCEPT(vm) {
        return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
    }
    SQCATCH(vm) {
        return SQ_PTRS->throwerror(vm, SQWHAT(vm));
    }

    return 0;
}

inline isize_t sqVarSet(HSKVM vm) {
    // Find the set method in the set table
    SQ_PTRS->push(vm, 2);
#if !defined (SCRAT_NO_ERROR_CHECKING)
    if (SQ_FAILED(SQ_PTRS->rawget(vm, -2))) {
#if (SQUIRREL_VERSION_NUMBER>= 200) && (SQUIRREL_VERSION_NUMBER < 300) // Squirrel 2.x
        return SQ_PTRS->throwerror(vm, _SC("member variable not found"));
#else // Squirrel 3.x
        SQ_PTRS->pushnull(vm);
        return SQ_PTRS->throwobject(vm);
#endif
    }
#else
    SQ_PTRS->rawget(vm, -2);
#endif

    // push 'this'
    SQ_PTRS->push(vm, 1);
    SQ_PTRS->push(vm, 3);

    // Call the setter
#if !defined (SCRAT_NO_ERROR_CHECKING)
    SQRESULT result = SQ_PTRS->call(vm, 2, false, ErrorHandling::IsEnabled());
    if (SQ_FAILED(result)) {
        return SQ_PTRS->throwerror(vm, LastErrorString(vm).c_str());
    }
#else
    SQ_PTRS->call(vm, 2, false, ErrorHandling::IsEnabled());
#endif

    return 0;
}

/// @endcond

}

#endif
