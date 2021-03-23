/*  see copyright notice in squirrel.h */
#ifndef _SQSTD_STRING_H_
#define _SQSTD_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int SQRexBool;
typedef struct SQRex SQRex;

typedef struct {
    const SQChar *begin;
    int len;
} SQRexMatch;

SQUIRREL_API SQRex *sqstd_rex_compile(const SQChar *pattern,const SQChar **error);
SQUIRREL_API void sqstd_rex_free(SQRex *exp);
SQUIRREL_API bool sqstd_rex_match(SQRex* exp,const SQChar* text);
SQUIRREL_API bool sqstd_rex_search(SQRex* exp,const SQChar* text, const SQChar** out_begin, const SQChar** out_end);
SQUIRREL_API bool sqstd_rex_searchrange(SQRex* exp,const SQChar* text_begin,const SQChar* text_end,const SQChar** out_begin, const SQChar** out_end);
SQUIRREL_API isize_t sqstd_rex_getsubexpcount(SQRex* exp);
SQUIRREL_API bool sqstd_rex_getsubexp(SQRex* exp, isize_t n, SQRexMatch *subexp);

SQUIRREL_API SQRESULT sqstd_format(HSKVM v,isize_t nformatstringidx,isize_t *outlen,SQChar **output);

SQUIRREL_API void sqstd_pushstringf(HSKVM v,const SQChar *s,...);

SQUIRREL_API SQRESULT sqstd_register_stringlib(HSKVM v);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQSTD_STRING_H_*/
