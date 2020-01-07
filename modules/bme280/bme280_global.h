#ifndef BMP180_GLOBAL_H
#define BMP180_GLOBAL_H

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



#endif // BMP180_GLOBAL_H
