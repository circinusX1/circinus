//
// SqratGlobalMethods: Global Methods
//

//
// Copyright (c) 2009 Brandon Jones
// Copyirght 2011 Li-Cheng (Andy) Tai
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

#if !defined(_SCRAT_GLOBAL_METHODS_H_)
#define _SCRAT_GLOBAL_METHODS_H_

#include <squirrel.h>
#include "sqr_imp_exp.h"
#include "sqratTypes.h"

extern tagSQObjectType param_types[16];

namespace Sqrat {

/// @cond DEV

//
// Squirrel Global Functions
//
template <class R>
class SqGlobal {
public:

    // Arg Count 0
    template <bool overloaded /*= false*/>
    static int Memb0(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)();
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                R ret = (*method)();
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
    template <class A1, int startIdx, bool overloaded /*= false*/>
    static int Memb1(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 1) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
    template <class A1, class A2, int startIdx, bool overloaded /*= false*/>
    static int Memb2(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2);
        M* method;
        R ret;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);
        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        ret = (*method)(
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
    template <class A1, class A2, class A3, int startIdx, bool overloaded /*= false*/>
    static int Memb3(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 3) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2, A3);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, int startIdx, bool overloaded /*= false*/>
    static int Memb4(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 4) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, int startIdx, bool overloaded /*= false*/>
    static int Memb5(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 5) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
#ifdef RAT_MANY_ARGS
    // Arg Count 6
    template <class A1, class A2, class A3, class A4, class A5, class A6, int startIdx, bool overloaded /*= false*/>
    static int Memb6(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 6) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, A6);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, int startIdx, bool overloaded /*= false*/>
    static int Memb7(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 7) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, int startIdx, bool overloaded /*= false*/>
    static int Memb8(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 8) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, int startIdx, bool overloaded /*= false*/>
    static int Memb9(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 9) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, int startIdx, bool overloaded /*= false*/>
    static int Memb10(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 10) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, int startIdx, bool overloaded /*= false*/>
    static int Memb11(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 11) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        Var<A11> a11(vm, startIdx + 10);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, int startIdx, bool overloaded /*= false*/>
    static int Memb12(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 12) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        Var<A11> a11(vm, startIdx + 10);
        Var<A12> a12(vm, startIdx + 11);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, int startIdx, bool overloaded /*= false*/>
    static int Memb13(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 13) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        Var<A11> a11(vm, startIdx + 10);
        Var<A12> a12(vm, startIdx + 11);
        Var<A13> a13(vm, startIdx + 12);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, int startIdx, bool overloaded /*= false*/>
    static int Memb14(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 14) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        Var<A11> a11(vm, startIdx + 10);
        Var<A12> a12(vm, startIdx + 11);
        Var<A13> a13(vm, startIdx + 12);
        Var<A14> a14(vm, startIdx + 13);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R ret = (*method)(
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
#endif
};


//
// reference return specialization
//

template <class R>
class SqGlobal<R&> {
public:

    // Arg Count 0
    template <bool overloaded /*= false*/>
    static int Memb0(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)();
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                R& ret = (*method)();
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
    template <class A1, int startIdx, bool overloaded /*= false*/>
    static int Memb1(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 1) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
    template <class A1, class A2, int startIdx, bool overloaded /*= false*/>
    static int Memb2(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
    template <class A1, class A2, class A3, int startIdx, bool overloaded /*= false*/>
    static int Memb3(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 3) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2, A3);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, int startIdx, bool overloaded /*= false*/>
    static int Memb4(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 4) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, int startIdx, bool overloaded /*= false*/>
    static int Memb5(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 5) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
#ifdef RAT_MANY_ARGS
    // Arg Count 6
    template <class A1, class A2, class A3, class A4, class A5, class A6, int startIdx, bool overloaded /*= false*/>
    static int Memb6(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 6) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, A6);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, int startIdx, bool overloaded /*= false*/>
    static int Memb7(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 7) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, A6, A7);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, int startIdx, bool overloaded /*= false*/>
    static int Memb8(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 8) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, A6, A7, A8);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, int startIdx, bool overloaded /*= false*/>
    static int Memb9(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 9) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, int startIdx, bool overloaded /*= false*/>
    static int Memb10(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 10) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, int startIdx, bool overloaded /*= false*/>
    static int Memb11(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 11) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        Var<A11> a11(vm, startIdx + 10);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, int startIdx, bool overloaded /*= false*/>
    static int Memb12(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 12) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        Var<A11> a11(vm, startIdx + 10);
        Var<A12> a12(vm, startIdx + 11);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, int startIdx, bool overloaded /*= false*/>
    static int Memb13(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 13) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        Var<A11> a11(vm, startIdx + 10);
        Var<A12> a12(vm, startIdx + 11);
        Var<A13> a13(vm, startIdx + 12);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, int startIdx, bool overloaded /*= false*/>
    static int Memb14(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 14) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        Var<A11> a11(vm, startIdx + 10);
        Var<A12> a12(vm, startIdx + 11);
        Var<A13> a13(vm, startIdx + 12);
        Var<A14> a14(vm, startIdx + 13);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        R& ret = (*method)(
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
#endif// #ifdef RAT_MANY_ARGS
};


//
// void return specialization
//

template <>
class SqGlobal<void> {
public:
    template <bool overloaded /*= false*/>
    static int MembRt(HSKVM vm, bool rv, int nargs)
    {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)();
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                (*method)();
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        //PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }


    // Arg Count 0
    template <bool overloaded /*= false*/>
    static int Memb0(HSKVM vm) {
/** mco-out
#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif
*/

        typedef void (*M)();
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);
        printf("PTR = %p %p\n",&method, method);

        SQTRY()
                (*method)();
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        //PushVar(vm, ret);
        SQCATCH(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 1
    template <class A1, int startIdx, bool overloaded /*= false*/>
    static int Memb1(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 1) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
    template <class A1, class A2, int startIdx, bool overloaded /*= false*/>
    static int Memb2(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 2) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
    template <class A1, class A2, class A3, int startIdx, bool overloaded /*= false*/>
    static int Memb3(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 3) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2, A3);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
    template <class A1, class A2, class A3, class A4, int startIdx, bool overloaded /*= false*/>
    static int Memb4(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 4) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, int startIdx, bool overloaded /*= false*/>
    static int Memb5(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 5) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
#ifdef RAT_MANY_ARGS
    // Arg Count 6
    template <class A1, class A2, class A3, class A4, class A5, class A6, int startIdx, bool overloaded /*= false*/>
    static int Memb6(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 6) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, A6);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, int startIdx, bool overloaded /*= false*/>
    static int Memb7(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 7) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, int startIdx, bool overloaded /*= false*/>
    static int Memb8(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 8) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, int startIdx, bool overloaded /*= false*/>
    static int Memb9(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 9) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, int startIdx, bool overloaded /*= false*/>
    static int Memb10(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 10) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, int startIdx, bool overloaded /*= false*/>
    static int Memb11(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 11) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        Var<A11> a11(vm, startIdx + 10);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, int startIdx, bool overloaded /*= false*/>
    static int Memb12(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 12) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        Var<A11> a11(vm, startIdx + 10);
        Var<A12> a12(vm, startIdx + 11);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, int startIdx, bool overloaded /*= false*/>
    static int Memb13(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 13) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        Var<A11> a11(vm, startIdx + 10);
        Var<A12> a12(vm, startIdx + 11);
        Var<A13> a13(vm, startIdx + 12);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, int startIdx, bool overloaded /*= false*/>
    static int Memb14(HSKVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && SQ_PTRS->gettop(vm) != startIdx + 14) {
            return SQ_PTRS->throwerror(vm, _SC("parameters count or return type"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14);
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        SQTRY()
                Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        Var<A9> a9(vm, startIdx + 8);
        Var<A10> a10(vm, startIdx + 9);
        Var<A11> a11(vm, startIdx + 10);
        Var<A12> a12(vm, startIdx + 11);
        Var<A13> a13(vm, startIdx + 12);
        Var<A14> a14(vm, startIdx + 13);
        SQCATCH_NOEXCEPT(vm) {
            return SQ_PTRS->throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        (*method)(
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
#endif
    // Arg C mco-mco RUNTIME
    typedef any_tsize (*type_r0)(void);
    typedef any_tsize (*type_r1)(any_tsize);
    typedef any_tsize (*type_r2)(any_tsize,any_tsize);
    typedef any_tsize (*type_r3)(any_tsize,any_tsize,any_tsize);
    typedef any_tsize (*type_r4)(any_tsize,any_tsize,any_tsize,any_tsize);
    typedef any_tsize (*type_r5)(any_tsize,any_tsize,any_tsize,any_tsize,any_tsize);
    typedef any_tsize (*type_r6)(any_tsize,any_tsize,any_tsize,any_tsize,any_tsize,any_tsize);
    typedef any_tsize (*type_r7)(any_tsize,any_tsize,any_tsize,any_tsize,any_tsize,any_tsize,any_tsize);

    typedef void (*type_v0)(void);
    typedef void (*type_v1)(any_tsize);
    typedef void (*type_v2)(any_tsize,any_tsize);
    typedef void (*type_v3)(any_tsize,any_tsize,any_tsize);
    typedef void (*type_v4)(any_tsize,any_tsize,any_tsize,any_tsize);
    typedef void (*type_v5)(any_tsize,any_tsize,any_tsize,any_tsize,any_tsize);
    typedef void (*type_v6)(any_tsize,any_tsize,any_tsize,any_tsize,any_tsize,any_tsize);
    typedef void (*type_v7)(any_tsize,any_tsize,any_tsize,any_tsize,any_tsize,any_tsize,any_tsize);

    typedef int (*right_t)(const char* , int);

    template <int startIdx, bool overloaded /*= false*/>
    static int MembX(HSKVM vm, int nargs)
    {
        if(nargs>8)
        {
            return SQ_PTRS->throwerror(vm, "too many parameters");
        }
        bool rv=false;
        union{
            const char* s;
            int         i;
            float       f;
            bool        b;
            size_t      v;
            uint8_t*    p;
        } u [8];
        if(nargs < 0){rv=true; nargs = -nargs;}

        typedef int (*M)();
        M* method;
        SQ_PTRS->getuserdata(vm, -1, (PVOID*)&method, NULL);

        for(int i = 0; i < nargs; i++)
        {
            switch(param_types[i+1])
            {
                case OT_NULL:
                {
                    u[i].p=nullptr;
                }break;
                case OT_USERDATA:
                {
                    Var<uint8_t*> a(vm, startIdx+i+1);
                    u[i].p=a.value;
                }break;

                case OT_INTEGER:
                {
                    Var<int> a(vm, startIdx+i+1);
                    u[i].i=a.value;
                }break;
                case OT_FLOAT:
                {
                    Var<float> a(vm, startIdx+i+1);
                    u[i].f=a.value;
                }break;
                case OT_BOOL:
                {
                    Var<bool> a(vm, startIdx+i+1);
                    u[i].b=a.value;
                }break;
                case OT_STRING:
                {
                    Var<const char*> a(vm, startIdx+i+1);
                    u[i].s=a.value;
                }break;
                default:
                    return SQ_PTRS->throwerror(vm, "Unlnown type for shared library function parameter");
                break;
            }
        }
        if(rv)
        {
            any_tsize r;
            switch(nargs)
            {
            case 0:{ r=((type_r0)(*method))();} break;
            case 1:{ r=((type_r1)(*method))(u[0].v);} break;
            case 2:{ r=((type_r2)(*method))(u[0].v,u[1].v);} break;
            case 3:{ r=((type_r3)(*method))(u[0].v,u[1].v,u[2].v);} break;
            case 4:{ r=((type_r4)(*method))(u[0].v,u[1].v,u[2].v,u[3].v);} break;
            case 5:{ r=((type_r5)(*method))(u[0].v,u[1].v,u[2].v,u[3].v,u[4].v);} break;
            case 6:{ r=((type_r6)(*method))(u[0].v,u[1].v,u[2].v,u[3].v,u[4].v,u[5].v);} break;
            case 7:{ r=((type_r7)(*method))(u[0].v,u[1].v,u[2].v,u[3].v,u[4].v,u[5].v,u[6].v);} break;
            }
            PushVarR(vm, r);
        }
        else
        {
            switch(nargs)
            {
            case 0:{ ((type_v0)(*method))();} break;
            case 1:{ ((type_v1)(*method))(u[0].v);} break;
            case 2:{ ((type_v2)(*method))(u[0].v,u[1].v);} break;
            case 3:{ ((type_v3)(*method))(u[0].v,u[1].v,u[2].v);} break;
            case 4:{ ((type_v4)(*method))(u[0].v,u[1].v,u[2].v,u[3].v);} break;
            case 5:{ ((type_v5)(*method))(u[0].v,u[1].v,u[2].v,u[3].v,u[4].v);} break;
            case 6:{ ((type_v6)(*method))(u[0].v,u[1].v,u[2].v,u[3].v,u[4].v,u[5].v);} break;
            case 7:{ ((type_v7)(*method))(u[0].v,u[1].v,u[2].v,u[3].v,u[4].v,u[5].v,u[6].v);} break;
            }
        }
        return rv;
    }

};



// Arg Count 0
template <class R>
SQFUNCTION SqGlobalMemb(R (* /*method*/)()) {
    return &SqGlobal<R>::template Memb0<false>;
}

// Arg Count 0
template <class R>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)()) {
    return &SqGlobal<R&>::template Memb0<false>;
}

// Arg Count 1
template <class R, class A1>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1)) {
    return &SqGlobal<R>::template Memb1<A1, 2, false>;
}

// Arg Count 1
template <class R, class A1>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1)) {
    return &SqGlobal<R&>::template Memb1<A1, 2, false>;
}

// Arg Count 2
template <class R, class A1, class A2>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2)) {
    return &SqGlobal<R>::template Memb2<A1, A2, 2, false>;
}

// Arg Count 2
template <class R, class A1, class A2>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2)) {
    return &SqGlobal<R&>::template Memb2<A1, A2, 2, false>;
}

// Arg Count 3
template <class R, class A1, class A2, class A3>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2, A3)) {
    return &SqGlobal<R>::template Memb3<A1, A2, A3, 2, false>;
}

// Arg Count 3
template <class R, class A1, class A2, class A3>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2, A3)) {
    return &SqGlobal<R&>::template Memb3<A1, A2, A3, 2, false>;
}

// Arg Count 4
template <class R, class A1, class A2, class A3, class A4>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2, A3, A4)) {
    return &SqGlobal<R>::template Memb4<A1, A2, A3, A4, 2, false>;
}

// Arg Count 4
template <class R, class A1, class A2, class A3, class A4>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4)) {
    return &SqGlobal<R&>::template Memb4<A1, A2, A3, A4, 2, false>;
}

// Arg Count 5
template <class R, class A1, class A2, class A3, class A4, class A5>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5)) {
    return &SqGlobal<R>::template Memb5<A1, A2, A3, A4, A5, 2, false>;
}

// Arg Count 5
template <class R, class A1, class A2, class A3, class A4, class A5>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5)) {
    return &SqGlobal<R&>::template Memb5<A1, A2, A3, A4, A5, 2, false>;
}

// Arg Count 6

template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6)) {
    return &SqGlobal<R>::template Memb6<A1, A2, A3, A4, A5, A6, 2, false>;
}

// Arg Count 6
template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6)) {
    return &SqGlobal<R&>::template Memb6<A1, A2, A3, A4, A5, A6, 2, false>;
}

// Arg Count 7
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7)) {
    return &SqGlobal<R>::template Memb7<A1, A2, A3, A4, A5, A6, A7, 2, false>;
}

// Arg Count 7
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7)) {
    return &SqGlobal<R&>::template Memb7<A1, A2, A3, A4, A5, A6, A7, 2, false>;
}

// Arg Count 8
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8)) {
    return &SqGlobal<R>::template Memb8<A1, A2, A3, A4, A5, A6, A7, A8, 2, false>;
}

// Arg Count 8
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8)) {
    return &SqGlobal<R&>::template Memb8<A1, A2, A3, A4, A5, A6, A7, A8, 2, false>;
}
#ifdef RAT_MANY_ARGS
// Arg Count 9
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9)) {
    return &SqGlobal<R>::template Memb9<A1, A2, A3, A4, A5, A6, A7, A8, A9, 2, false>;
}

// Arg Count 9
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9)) {
    return &SqGlobal<R&>::template Memb9<A1, A2, A3, A4, A5, A6, A7, A8, A9, 2, false>;
}

// Arg Count 10
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)) {
    return &SqGlobal<R>::template Memb10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, 2, false>;
}

// Arg Count 10
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)) {
    return &SqGlobal<R&>::template Memb10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, 2, false>;
}

// Arg Count 11
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11)) {
    return &SqGlobal<R>::template Memb11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, 2, false>;
}

// Arg Count 11
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11)) {
    return &SqGlobal<R&>::template Memb11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, 2, false>;
}

// Arg Count 12
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12)) {
    return &SqGlobal<R>::template Memb12<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, 2, false>;
}

// Arg Count 12
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12)) {
    return &SqGlobal<R&>::template Memb12<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, 2, false>;
}

// Arg Count 13
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13)) {
    return &SqGlobal<R>::template Memb13<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, 2, false>;
}

// Arg Count 13
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13)) {
    return &SqGlobal<R&>::template Memb13<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, 2, false>;
}

// Arg Count 14
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
SQFUNCTION SqGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14)) {
    return &SqGlobal<R>::template Memb14<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, 2, false>;
}

// Arg Count 14
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
SQFUNCTION SqGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14)) {
    return &SqGlobal<R&>::template Memb14<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, 2, false>;
}
#endif //#ifdef RAT_MANY_ARGS

//
// Member Global Function Resolvers
//

// Arg Count 1
template <class R, class A1>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1)) {
    return &SqGlobal<R>::template Memb1<A1, 1, false>;
}

// Arg Count 1
template <class R, class A1>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1)) {
    return &SqGlobal<R&>::template Memb1<A1, 1, false>;
}

// Arg Count 2
template <class R, class A1, class A2>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2)) {
    return &SqGlobal<R>::template Memb2<A1, A2, 1, false>;
}

// Arg Count 2
template <class R, class A1, class A2>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1, A2)) {
    return &SqGlobal<R&>::template Memb2<A1, A2, 1, false>;
}

// Arg Count 3
template <class R, class A1, class A2, class A3>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2, A3)) {
    return &SqGlobal<R>::template Memb3<A1, A2, A3, 1, false>;
}

// Arg Count 3
template <class R, class A1, class A2, class A3>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1, A2, A3)) {
    return &SqGlobal<R&>::template Memb3<A1, A2, A3, 1, false>;
}

// Arg Count 4
template <class R, class A1, class A2, class A3, class A4>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2, A3, A4)) {
    return &SqGlobal<R>::template Memb4<A1, A2, A3, A4, 1, false>;
}

// Arg Count 4
template <class R, class A1, class A2, class A3, class A4>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4)) {
    return &SqGlobal<R&>::template Memb4<A1, A2, A3, A4, 1, false>;
}

// Arg Count 5
template <class R, class A1, class A2, class A3, class A4, class A5>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5)) {
    return &SqGlobal<R>::template Memb5<A1, A2, A3, A4, A5, 1, false>;
}

// Arg Count 5
template <class R, class A1, class A2, class A3, class A4, class A5>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5)) {
    return &SqGlobal<R&>::template Memb5<A1, A2, A3, A4, A5, 1, false>;
}


// Arg Count 6
template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6)) {
    return &SqGlobal<R>::template Memb6<A1, A2, A3, A4, A5, A6, 1, false>;
}

// Arg Count 6
template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6)) {
    return &SqGlobal<R&>::template Memb6<A1, A2, A3, A4, A5, A6, 1, false>;
}

// Arg Count 7
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7)) {
    return &SqGlobal<R>::template Memb7<A1, A2, A3, A4, A5, A6, A7, 1, false>;
}

// Arg Count 7
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7)) {
    return &SqGlobal<R&>::template Memb7<A1, A2, A3, A4, A5, A6, A7, 1, false>;
}

// Arg Count 8
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8)) {
    return &SqGlobal<R>::template Memb8<A1, A2, A3, A4, A5, A6, A7, A8, 1, false>;
}

// Arg Count 8
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8)) {
    return &SqGlobal<R&>::template Memb8<A1, A2, A3, A4, A5, A6, A7, A8, 1, false>;
}
#ifdef RAT_MANY_ARGS
// Arg Count 9
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9)) {
    return &SqGlobal<R>::template Memb9<A1, A2, A3, A4, A5, A6, A7, A8, A9, 1, false>;
}

// Arg Count 9
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9)) {
    return &SqGlobal<R&>::template Memb9<A1, A2, A3, A4, A5, A6, A7, A8, A9, 1, false>;
}

// Arg Count 10
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)) {
    return &SqGlobal<R>::template Memb10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, 1, false>;
}

// Arg Count 10
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)) {
    return &SqGlobal<R&>::template Memb10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, 1, false>;
}

// Arg Count 11
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11)) {
    return &SqGlobal<R>::template Memb11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, 1, false>;
}

// Arg Count 11
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11)) {
    return &SqGlobal<R&>::template Memb11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, 1, false>;
}

// Arg Count 12
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12)) {
    return &SqGlobal<R>::template Memb12<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, 1, false>;
}

// Arg Count 12
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12)) {
    return &SqGlobal<R&>::template Memb12<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, 1, false>;
}

// Arg Count 13
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13)) {
    return &SqGlobal<R>::template Memb13<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, 1, false>;
}

// Arg Count 13
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
SQFUNCTION SqMemberGlobalMemb(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13)) {
    return &SqGlobal<R&>::template Memb13<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, 1, false>;
}

// Arg Count 14
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
SQFUNCTION SqMemberGlobalMemb(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14)) {
    return &SqGlobal<R>::template Memb14<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, 1, false>;
}
#endif // RAT_MANY_ARGS


// Arg Count X mco-mco get for FUNC the SQFUNC
template <class R>
SQFUNCTION_RT SqGlobalMembRt(R& (* /*method*/)()) {
    return &SqGlobal<R&>::template MembX<1,false>;
}

// Arg Count X
template <class R>
SQFUNCTION_RT SqGlobalMembRt(R (* /*method*/)()) {
    return &SqGlobal<R>::template MembX<1,false>;
}


/// @endcond

}

#endif
