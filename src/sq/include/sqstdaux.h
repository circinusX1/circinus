/*  see copyright notice in squirrel.h */
#ifndef _SQSTD_AUXLIB_H_
#define _SQSTD_AUXLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

SQUIRREL_API void sqstd_seterrorhandlers(HSKVM v);
SQUIRREL_API void sqstd_printcallstack(HSKVM v);

SQUIRREL_API SQRESULT sqstd_throwerrorf(HSKVM v,const SQChar *err,...);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* _SQSTD_AUXLIB_H_ */
