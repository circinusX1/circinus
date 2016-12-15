/*  see copyright notice in squirrel.h */
#ifndef _SQSTDBLOB_H_
#define _SQSTDBLOB_H_

#ifdef __cplusplus
extern "C" {
#endif

SQUIRREL_API PVOID sqstd_createblob(HSKVM v, int size);
SQUIRREL_API SQRESULT sqstd_getblob(HSKVM v,int idx,PVOID *ptr);
SQUIRREL_API int sqstd_getblobsize(HSKVM v,int idx);

SQUIRREL_API SQRESULT sqstd_register_bloblib(HSKVM v);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQSTDBLOB_H_*/

