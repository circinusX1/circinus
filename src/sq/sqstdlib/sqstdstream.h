/*  see copyright notice in squirrel.h */
#ifndef _SQSTD_STREAM_H_
#define _SQSTD_STREAM_H_

int _stream_readblob(HSKVM v);
int _stream_readline(HSKVM v);
int _stream_readn(HSKVM v);
int _stream_writeblob(HSKVM v);
int _stream_writen(HSKVM v);
int _stream_seek(HSKVM v);
int _stream_tell(HSKVM v);
int _stream_len(HSKVM v);
int _stream_eos(HSKVM v);
int _stream_flush(HSKVM v);

#define _DECL_STREAM_FUNC(name,nparams,typecheck) {_SC(#name),_stream_##name,nparams,typecheck}
SQRESULT declare_stream(HSKVM v,const SQChar* name,PVOID typetag,const SQChar* reg_name,const SQRegFunction *methods,const SQRegFunction *globals);
#endif /*_SQSTD_STREAM_H_*/
