/*  see copyright notice in squirrel.h */
#ifndef _SQSTDIO_H_
#define _SQSTDIO_H_

#ifdef __cplusplus

#define SQSTD_STREAM_TYPE_TAG 0x80000000

struct SQStream {
    virtual ~SQStream() {}
    virtual int Read(void *buffer, int size) = 0;
    virtual int Write(void *buffer, int size) = 0;
    virtual int Flush() = 0;
    virtual int Tell() = 0;
    virtual int Len() = 0;
    virtual int Seek(int offset, int origin) = 0;
    virtual bool IsValid() = 0;
    virtual bool EOS() = 0;
};

extern "C" {
#endif

#define SQ_SEEK_CUR 0
#define SQ_SEEK_END 1
#define SQ_SEEK_SET 2

typedef void* SQFILE;

SQUIRREL_API SQFILE sqstd_fopen(const SQChar *,const SQChar *);
SQUIRREL_API int sqstd_fread(PVOID, int, int, SQFILE);
SQUIRREL_API int sqstd_fwrite(const PVOID, int, int, SQFILE);
SQUIRREL_API int sqstd_fseek(SQFILE , int , int);
SQUIRREL_API int sqstd_ftell(SQFILE);
SQUIRREL_API int sqstd_fflush(SQFILE);
SQUIRREL_API int sqstd_fclose(SQFILE);
SQUIRREL_API int sqstd_feof(SQFILE);

SQUIRREL_API SQRESULT sqstd_createfile(HSKVM v, SQFILE file,bool own);
SQUIRREL_API SQRESULT sqstd_getfile(HSKVM v, int idx, SQFILE *file);

//compiler helpers
SQUIRREL_API SQRESULT sqstd_loadfile(HSKVM v,const SQChar *filename,bool printerror);
SQUIRREL_API SQRESULT sqstd_dofile(HSKVM v,const SQChar *filename,bool retval,bool printerror);
SQUIRREL_API SQRESULT sqstd_writeclosuretofile(HSKVM v,const SQChar *filename);

SQUIRREL_API SQRESULT sqstd_register_iolib(HSKVM v);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQSTDIO_H_*/

