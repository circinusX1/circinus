/*  see copyright notice in squirrel.h */
#ifndef _SQSTDBLOB_H_
#define _SQSTDBLOB_H_

#ifdef __cplusplus
extern "C" {
#endif

SQUIRREL_API PVOID sqstd_createblob(HSKVM v, isize_t size);
SQUIRREL_API SQRESULT sqstd_getblob(HSKVM v,isize_t idx,PVOID *ptr);
SQUIRREL_API isize_t sqstd_getblobsize(HSKVM v,isize_t idx);

SQUIRREL_API SQRESULT sqstd_register_bloblib(HSKVM v);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQSTDBLOB_H_*/

