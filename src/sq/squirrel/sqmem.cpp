/*
    see copyright notice in squirrel.h
*/
#include "sqpcheader.h"
#ifndef SQ_EXCLUDE_DEFAULT_MEMFUNCTIONS

#ifdef MEM_POOL
#include <stdint.h>
#include "sqmem.h"


minipool<128>       _pool128(32);
minipool<256>       _pool256(32);
minipool<512>       _pool512(32);
minipool<1024>      _pool1k(16);
minipool<4096>      _pool4k(16);
minipool<16384>     _pool16k(16);
minipool<32768>     _pool32k(8);
static size_t       _maxbytes = 0;
static size_t       _maxblock = 0;
extern bool         _mempool;

void *sq_vm_malloc(size_t size)
{
    if(!_mempool)
        return ::malloc(size);
    _maxbytes+=size;
    _maxblock = std::max(_maxblock,size);

   // printf("alloc sz=%d ammnt=%d maxblk=%d \n", size, _maxbytes, _maxblock);

    if(size<=128)
        return _pool128.alloc();
    if(size<=256)
        return _pool256.alloc();
    if(size<=512)
        return _pool512.alloc();
    if(size<=1024)
        return _pool1k.alloc();
    if(size<=4096)
        return _pool4k.alloc();
    if(size<=16384)
        return _pool16k.alloc();
    if(size<=32728)
        return _pool32k.alloc();
    return malloc(size);
}

void *sq_vm_realloc(void *p, size_t oldsiz, size_t size)
{
    if(!_mempool)
        return ::realloc(p, size);

    _maxbytes -= oldsiz;
    _maxbytes += size;
    _maxblock = std::max(_maxblock,size);

   // printf("realloc sz=%d ammnt=%d maxblk=%d \n", size, _maxbytes, _maxblock);

    void *pn = nullptr;

    if(size<=128)
    {
        pn=_pool128.alloc();
        goto DONE;
    }
    if(size<=256)
    {
        pn=_pool256.alloc();
        goto DONE;
    }
    if(size<=512)
    {
        pn=_pool512.alloc();
        goto DONE;
    }
    if(size<=1024)
    {
        pn=_pool1k.alloc();
        goto DONE;
    }
    if(size<=4096)
    {
        pn=_pool4k.alloc();
        goto DONE;
    }
    if(size<=16384)
    {
        pn=_pool16k.alloc();
        goto DONE;
    }
    if(size<=32768)
        pn=_pool32k.alloc();
DONE:

    if(pn)
    {
        if(p)
        {
            ::memcpy(pn, p, oldsiz);
            if(p)
                ::sq_vm_free(p, oldsiz);
        }
        return pn;
    }
    if(p)
    {
        if(::sq_vm_free(p, oldsiz))
        {
            return ::malloc(size);
        }
    }
    if(p==0)
        return ::malloc(size);
    return ::realloc(p, size);
}

bool sq_vm_free(void *p, size_t size)
{
    if(!_mempool)
        ::free(p);

    _maxbytes-=size;
  //  printf("free sz=%d ammnt=%d maxblk=%d \n", size, _maxbytes, _maxblock);

    if(size<=128)
    {    _pool128.free(p); return true;}
    if(size<=256)
    {   _pool256.free(p); return true;}
    if(size<=512)
    {   _pool512.free(p); return true;}
    if(size<=1024)
    {   _pool1k.free(p); return true;}
    if(size<=4096)
    {   _pool4k.free(p); return true;}
    if(size<=16384)
    {   _pool16k.free(p); return true;}
    if(size<=32768)
    {   _pool32k.free(p); return true;}
    ::free(p);
    return false;
}



#else

void *sq_vm_malloc(size_t size){ return malloc(size); }
void *sq_vm_realloc(void *p, size_t SQ_UNUSED_ARG(oldsize), size_t size){ return realloc(p, size); }
bool sq_vm_free(void *p, size_t SQ_UNUSED_ARG(size)){ free(p); return true;}

#endif

#endif
