/*  see copyright notice in squirrel.h */
#ifndef _SQSTD_STREAM_H_
#define _SQSTD_STREAM_H_

isize_t _stream_readblob(HSKVM v);
isize_t _stream_readline(HSKVM v);
isize_t _stream_readn(HSKVM v);
isize_t _stream_writeblob(HSKVM v);
isize_t _stream_writen(HSKVM v);
isize_t _stream_seek(HSKVM v);
isize_t _stream_tell(HSKVM v);
isize_t _stream_len(HSKVM v);
isize_t _stream_eos(HSKVM v);
isize_t _stream_flush(HSKVM v);

#define _DECL_STREAM_FUNC(name,nparams,typecheck) {_SC(#name),_stream_##name,nparams,typecheck}
SQRESULT declare_stream(HSKVM v,const SQChar* name,PVOID typetag,const SQChar* reg_name,const SQRegFunction *methods,const SQRegFunction *globals);
#endif /*_SQSTD_STREAM_H_*/
