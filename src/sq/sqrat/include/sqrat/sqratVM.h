//
// wrapper for the Squirrel VM under Sqrat
//

//
// Copyright (c) 2011 Alston Chen
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

#if !defined(_SCRAT_VM_H_)
#define _SCRAT_VM_H_

#include <squirrel.h>
#include <sqrat.h>
#include "sq_modapi.h"
#include <iostream>
#include <stdarg.h>
#include <stdio.h>

#include <sqstdio.h>
#include <sqstdblob.h>
#include <sqstdmath.h>
#include <sqstdsystem.h>
#include <sqstdstring.h>

namespace Sqrat
{

#ifdef SQUNICODE
#define scvprintf vwprintf
#else
#define scvprintf vprintf
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper class that wraps a Squirrel virtual machine in a C++ API
///
/// \remarks
/// This class is not currently thread-safe for the case of different VMs running in different threads (all others are)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SqratVM
{
private:

    HSKVM m_vm;
    Sqrat::RootTable* m_rootTable;
    Sqrat::Script* m_script;
    Sqrat::string m_lastErrorMsg;

    static void s_addVM(HSKVM vm, SqratVM* sqratvm)
    {
        // TODO for user: use mutex to lock ms_sqratVMs if necessary for your uses
        ms_sqratVMs().insert(std::make_pair(vm, sqratvm));
    }

    static void s_deleteVM(HSKVM vm)
    {
        // TODO for user: use mutex to lock ms_sqratVMs if necessary for your uses
        ms_sqratVMs().erase(vm);
    }

    static SqratVM* s_getVM(HSKVM vm)
    {
        // TODO for user: use mutex to lock ms_sqratVMs if necessary for your uses
        return ms_sqratVMs()[vm];
    }

private:

    static SQRAT_API unordered_map<HSKVM, SqratVM*>::type& ms_sqratVMs();

    static void printMemb(HSKVM /*v*/, const SQChar *s, ...)
    {
        va_list vl;
        va_start(vl, s);
        scvprintf(s, vl);
        va_end(vl);
    }

    static isize_t runtimeErrorHandler(HSKVM v)
    {
        const SQChar *sErr = 0;
        if(SQ_PTRS->gettop(v) >= 1)
        {
            Sqrat::string& errStr = s_getVM(v)->m_lastErrorMsg;
            if(SQ_SUCCEEDED(SQ_PTRS->getstring(v, 2, &sErr)))
            {
                errStr = sErr;
            }
            else
            {
                errStr = _SC("an unknown runtime error has occured");
            }
        }
        return 0;
    }

    static void compilerErrorHandler(HSKVM v,
                                     const SQChar* desc,
                                     const SQChar* source,
                                     isize_t line,
                                     isize_t column)
    {
        SQChar buf[512];
        scsprintf(buf, _SC("%s:%d:%d: %s"), source, (isize_t) line, (isize_t) column, desc);
        buf[sizeof(buf) - 1] = 0;
        s_getVM(v)->m_lastErrorMsg = buf;
    }

public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Enumeration representing the different types of errors that may occur within a SqratVM
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    enum ERROR_STATE
    {
        SQRAT_NO_ERROR,      ///< For when no error has occurred
        SQRAT_COMPILE_ERROR, ///< For when a script compiling error has occurred
        SQRAT_RUNTIME_ERROR  ///< For when a script running error has occurred
    };

    static const unsigned char LIB_IO   = 0x01;                                              ///< Input/Output library
    static const unsigned char LIB_BLOB = 0x02;                                              ///< Blob library
    static const unsigned char LIB_MATH = 0x04;                                              ///< Math library
    static const unsigned char LIB_SYST = 0x08;                                              ///< Arhic library
    static const unsigned char LIB_STR  = 0x10;                                              ///< String library
    static const unsigned char LIB_ALL  = LIB_IO | LIB_BLOB | LIB_MATH | LIB_SYST | LIB_STR; ///< All libraries

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    /// \param initialStackSize Initial size of the execution stack (if the stack is too small it will automatically grow)
    /// \param libsToLoad       Specifies what standard Squirrel libraries should be loaded
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SqratVM(isize_t initialStackSize = 1024, unsigned char libsToLoad = LIB_ALL): m_vm(SQ_PTRS->open(initialStackSize))
        , m_rootTable(new Sqrat::RootTable(m_vm))
        , m_script(new Sqrat::Script(m_vm))
        , m_lastErrorMsg()
    {
        s_addVM(m_vm, this);
        //register std libs
        SQ_PTRS->pushroottable(m_vm);
        if (libsToLoad & LIB_IO)
            sqstd_register_iolib(m_vm);
        if (libsToLoad & LIB_BLOB)
            sqstd_register_bloblib(m_vm);
        if (libsToLoad & LIB_MATH)
            sqstd_register_mathlib(m_vm);
        if (libsToLoad & LIB_SYST)
            sqstd_register_systemlib(m_vm);
        if (libsToLoad & LIB_STR)
            sqstd_register_stringlib(m_vm);
        SQ_PTRS->pop(m_vm, 1);
        SetPrintMemb(printMemb, printMemb);
        SetErrorHandler(runtimeErrorHandler, compilerErrorHandler);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~SqratVM()
    {
        s_deleteVM(m_vm);
        delete m_script;
        delete m_rootTable;
        SQ_PTRS->close(m_vm);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the underlying Squirrel VM
    ///
    /// \return Underlying Squirrel VM
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSKVM GetVM()
    {
        return m_vm;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the root table for this VM
    ///
    /// \return RootTable for the VM
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Sqrat::RootTable& GetRootTable()
    {
        return *m_rootTable;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the associated Script for this VM
    ///
    /// \return Script for the VM
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Sqrat::Script& GetScript()
    {
        return *m_script;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the error message for the most recent Squirrel error with the VM
    ///
    /// \return String containing a nice error message
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Sqrat::string GetLastErrorMsg()
    {
        return m_lastErrorMsg;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Overwrites the most recent Squirrel error for this VM
    ///
    /// \param str A nice error message
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void SetLastErrorMsg(const Sqrat::string& str)
    {
        m_lastErrorMsg = str;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets the print function of the virtual machine (a default one is set in the constructor)
    ///
    /// \param printMemb A pointer to the print func or NULL to disable the output
    /// \param errMemb   A pointer to the error func or NULL to disable the output
    ///
    /// \remarks
    /// The print function is used by the built-in Squirrel print function to output text.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void SetPrintMemb(SQPRINTFUNCTION printMemb, SQPRINTFUNCTION errMemb)
    {
        SQ_PTRS->setprintfunc(m_vm, printMemb, errMemb);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets the Squirrel error handlers (both are set to defaults in the constructor)
    ///
    /// \param runErr A pointer to the runtime error handler func
    /// \param comErr A pointer to the compile error handler func
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void SetErrorHandler(SQFUNCTION runErr, SQCOMPILERERROR comErr)
    {
        SQ_PTRS->newclosure(m_vm, runErr, 0);
        SQ_PTRS->seterrorhandler(m_vm);
        SQ_PTRS->setcompilererrorhandler(m_vm, comErr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Runs a string containing a Squirrel script
    ///
    /// \param str String containing a Squirrel script
    ///
    /// \return An ERROR_STATE representing what happened
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ERROR_STATE DoString(const Sqrat::string& str)
    {
        Sqrat::string msg;
        m_lastErrorMsg.clear();
        if(!m_script->CompileString(str, msg))
        {
            if(m_lastErrorMsg.empty())
            {
                m_lastErrorMsg = msg;
            }
            return SQRAT_COMPILE_ERROR;
        }
        if(!m_script->Run(msg))
        {
            if(m_lastErrorMsg.empty())
            {
                m_lastErrorMsg = msg;
            }
            return SQRAT_RUNTIME_ERROR;
        }
        return SQRAT_NO_ERROR;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Runs a file containing a Squirrel script
    ///
    /// \param file File path containing a Squirrel script
    ///
    /// \return An ERROR_STATE representing what happened
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ERROR_STATE DoFile(const Sqrat::string& file)
    {
        Sqrat::string msg;
        m_lastErrorMsg.clear();
        if(!m_script->CompileFile(file, msg))
        {
            if(m_lastErrorMsg.empty())
            {
                m_lastErrorMsg = msg;
            }
            return SQRAT_COMPILE_ERROR;
        }
        if(!m_script->Run(msg))
        {
            if(m_lastErrorMsg.empty())
            {
                m_lastErrorMsg = msg;
            }
            return SQRAT_RUNTIME_ERROR;
        }
        return SQRAT_NO_ERROR;
    }

};

#if !defined(SCRAT_IMPORT)
inline unordered_map<HSKVM, SqratVM*>::type& SqratVM::ms_sqratVMs() {
    static unordered_map<HSKVM, SqratVM*>::type ms;
    return ms;
}
#endif

}

#endif
