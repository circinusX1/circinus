#ifndef MCP9600_GLOBAL_H
#define MCP9600_GLOBAL_H
/*
 *
 *
 * (gdb) set auto-solib-add off

In Qt Creator, under Options->Debugger->Gdb
 *
 *
*/
#undef BUILDAPP
#include "../iper.h"
#ifdef __cplusplus
extern "C" {
#endif

EXPORT bool start_module(HSKVM vm, sq_api* ptrs,  IInstance* pi, const char* name);


#ifdef __cplusplus
}
#endif


#endif // MCP9600_GLOBAL_H
