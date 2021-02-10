/*
Copyright (c) 2014-2016 Marius C. All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/circinusX1. The name of the
https://github.com/circinusX1/amutrion may not be used to endorse or promote
products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef OSGEN_H
#define OSGEN_H


#include <iostream>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <atomic>
#define     IDLE_SLEEP  0

#ifdef __linux
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#define UNUS(x_)   (void)(x_)

typedef unsigned long ulong;
typedef unsigned long THANDLE;
typedef const char kchar;


/**
** * @brief The umutex class
** */
class conditio;
class umutex
{
    mutable pthread_mutex_t _mut;
public:
    friend class conditio;
    umutex()
    {
        pthread_mutexattr_t     attr;

        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&_mut, &attr);
        pthread_mutexattr_destroy(&attr);
    }

    virtual ~umutex()
    {
        pthread_mutex_unlock(&_mut);
        pthread_mutex_destroy(&_mut);
    }

    int mlock() const
    {
        int err = pthread_mutex_lock(&_mut);
        return err;
    }

    bool try_lock() const
    {
        int err =pthread_mutex_trylock(&_mut);
        return err==0;
    }

    bool munlock() const
    {
        int err =pthread_mutex_unlock(&_mut);
        return err==0;
    }
};


/**
** * @brief The Mpthrd class
** */
class Mpthrd
{
public:
    Mpthrd()
    {
        _bstop   = 1;
        _hthread = 0;
        _joined  = false;
        _init = -1;
        _stopped=false;
    }

    virtual ~Mpthrd()
    {
        if(!_stopped)
        {
            stop_it();
        }

        if(_joined && _hthread)
        {
            pthread_detach(_hthread);
        }
        if(_init==0)
            pthread_attr_destroy(&_attr);
        _hthread = 0;

    }

    virtual int  start_thread()
    {
        _bstop   = 0;

        _init=pthread_attr_init(&_attr);
        pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_JOINABLE);
        if (pthread_create(&_hthread, &_attr, SFoo, this) != 0)
        {
            pthread_attr_destroy(&_attr);
            return errno;
        }
        pthread_attr_destroy(&_attr);
        usleep(1000);
        return 0;
    }

    virtual void sig_it()
    {
        _bstop = 1;
        ::usleep(10000);
    }
    virtual void    stop_it()
    {
        _bstop = 1;
        ::usleep(512000);
        if(_hthread)
        {
            t_join();
        }
    }

    bool  is_stopped()
    {
        return _bstop;
    }

    int t_join()
    {
        if(!_joined && _hthread)
        {
            _joined = true;
            return pthread_join(_hthread,0);
        }
        return 0;
    }

protected:
    virtual void thread_main()=0;
    int         _bstop;
protected:
    int         _init;
    pthread_attr_t  _attr;
    THANDLE     _hthread;
    int         _stopped;
    bool        _joined;
    static void* SFoo(void* pData)
    {
        Mpthrd* pT = reinterpret_cast<Mpthrd*>(pData);
        pT->_stopped = 0;
        pT->thread_main();
        pT->_stopped = 1;
        return 0;
    }
};

class umutex;
/**
 * @brief The conditio class
 */
class conditio
{
public:
    conditio(umutex& m)
    {
        _pmutex = &m._mut;
        pthread_cond_init(&_cond, NULL);
        pthread_mutex_init(_pmutex  ,NULL);
    }
    conditio()
    {
        _pmutex = &_private;
        pthread_cond_init(&_cond, NULL);
        pthread_mutex_init(_pmutex  ,NULL);
    }
    ~conditio()
    {
        pthread_cond_signal(&_cond);
        pthread_mutex_unlock(_pmutex);

        pthread_cond_destroy(&_cond);
        pthread_mutex_destroy(_pmutex);
    }
    void lock()
    {
        pthread_mutex_lock(_pmutex);
    }
    void signal()
    {
        pthread_cond_signal(&_cond);
    }
    void broadcast()
    {
        pthread_cond_broadcast(&_cond);
    };

    void wait()
    {
        pthread_cond_wait(&_cond, _pmutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(_pmutex);
    }
private:
    pthread_cond_t _cond;
    pthread_mutex_t* _pmutex;
    pthread_mutex_t _private;
};


template <typename U>
class CntPtr
{
public:
    explicit CntPtr(U* p = 0) : _c(0) {
        if (p) _c = new cnt(p);
    }
    ~CntPtr() {
        dec();
    }
    CntPtr(const CntPtr& r) throw() {
        add(r._c);
    }
    CntPtr& operator=(const CntPtr& r) {
        if (this != &r) {
            dec();
            add(r._c);
        }
        return *this;
    }
    CntPtr& operator=(const U* r) {
        dec();
        if(r)
        {
            _c = new cnt(r);
        }
        return *this;
    }
    U* ptr()const{return _c ? _c->p : 0;}
    U& operator*()   throw()   {
        return *_c->p;
    }
    U* operator->() const throw()   {
        return _c->p;
    }

    const U& obj(){
        return *_c->p;
    }
    U* detach()
    {
        U* p = _c->p;
        delete _c;
        _c = 0;
        return p;
    }
private:
    struct cnt {
        cnt(U* p = 0, int32_t c = 1) : p(p), c(c) {}
        U*            p;
        int32_t     c;
    }* _c;
    void add(cnt* c) throw() {
        _c = c;
        if (c) ++c->c;
    }
    void dec() {
        if (_c) {
            if (--_c->c == 0) {
                delete _c->p;
                delete _c;
            }
            _c = 0;
        }
    }
};



#elif _WIN32

#include <windows.h>
#include <process.h>

#define RST  ""
#define KRED  ""

struct umutex
{
    CRITICAL_SECTION _cs;
    umutex(){
        InitializeCriticalSection(&_cs);
    }
    ~umutex(){
        //LeaveCriticalSection(&_cs);
        DeleteCriticalSection(&_cs);
    }
    int mlock() {

        EnterCriticalSection(&_cs);

        return 0;
    }
    int try_lock()
    {

        #if(_WIN32_WINNT >= 0x0400)
            if(TryEnterCriticalSection(&_cs))
            {
                return 1;
            }
            return 0;
        #else
            EnterCriticalSection(&_cs);
            return 1;
        #endif
    }
    int munlock() {

        LeaveCriticalSection(&_cs);
        return 1;
    }

};

#define SAFE_CLOSEHANDLE(_handle)  if(VALID_HANDLE(_handle))\
                                      {\
                                          BOOL b = ::CloseHandle(_handle);\
                                          assert(b);\
                                          if(b)\
                                              _handle = INVALID_HANDLE_VALUE;\
                                      }
#define BEGINTHREADEX(psa, cbStack, pfnStartAddr, pvParam, fdwCreate, pdwThreadID)  \
                      ((HANDLE) _beginthreadex((void*)psa, (UINT)cbStack,             \
                                      (PTHREAD_START)(pfnStartAddr),                  \
                                      (void*)pvParam, (UINT)(fdwCreate),              \
                                     (UINT*)(pdwThreadID)))

class Mpthrd
{
public:
    Mpthrd()
    {
        _bstop   = 1;
        _hthread = 0;
        _joined  = false;
        _init = -1;
    }

    virtual ~Mpthrd()
    {
        if(!_stopped)
        {
            stop_it();
        }

        _hthread = 0;

    }

    virtual int  start_thread()
    {
        DWORD nDummy = 0;
        _hthread = ::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&Mpthrd::SFoo,
                                                static_cast<void*>(this),
                                                CREATE_SUSPENDED, &nDummy);
        if(_hthread){
            _bstop=0;
            ::ResumeThread(_hthread);
            return 0;
        }
        return errno;
    }

    virtual void sig_it()
    {
        _bstop = 1;
    }
    virtual void    stop_it()
    {
        DWORD dwRetVal=WaitForSingleObject(_hthread, 1000);
        if(WAIT_OBJECT_0 != dwRetVal)
        {
            if(!is_stopped())
            {
                ::TerminateThread(_hthread, 0);
            }
            ::CloseHandle(_hthread);
            _hthread = 0;
        }
    }

    bool  is_stopped()
    {
        return _bstop;
    }

    int t_join()
    {
       WaitForSingleObject(_hthread, 30000);
        return 0;
    }

protected:
    virtual void thread_main()=0;
    int         _bstop;
private:
    int         _init;
    HANDLE      _hthread;
    int         _stopped;
    bool        _joined;
    static void* SFoo(void* pData)
    {
        Mpthrd* pT = reinterpret_cast<Mpthrd*>(pData);
        pT->_stopped = 0;
        pT->thread_main();
        pT->_stopped = 1;
        return 0;
    }
};

class conditio
{
public:
    conditio()
    {
        _event = ::CreateEvent(NULL,TRUE,FALSE,0);
    }
    ~conditio()
    {
        ::CloseHandle(_event);
    }
    void lock()
    {
        _mut.mlock();
    }
    void signal()
    {
         SetEvent(_event);
    }
    void broadcast()
    {
         SetEvent(_event);
    };

    bool wait(int to=-1)
    {
        DWORD drr;
        if(to==-1)
            drr = WaitForSingleObject(_event,INFINITE);
        else
            drr = WaitForSingleObject(_event,to);
        return drr== WAIT_OBJECT_0;

    }
    void unlock()
    {
        _mut.munlock();
    }
private:
    HANDLE  _event;
    umutex  _mut;
};

#endif

class AutoLock
{
public:
    AutoLock(umutex* m):_mutex(m)
    {
         _mutex->mlock();
    }
    AutoLock(const umutex* m):_mutex((umutex*)m)
    {
         _mutex->mlock();
    }
    ~AutoLock()
    {
        _mutex->munlock();
    }
private:
    umutex* _mutex;
};


class AutoReset
{
    std::atomic<bool>& _ref;
public:
    AutoReset(std::atomic<bool>& ab):_ref(ab)
    {
        _ref=true;
    }
    ~AutoReset()
    {
        _ref=false;
    }
    void reset(){_ref=false;}
};
#ifdef __linux

inline bool msleep(int t)
{
    ::usleep(t<<10);
    return true;
}

inline time_t tick_count()
{
    struct timeval tv;
    if(gettimeofday(&tv, NULL) != 0)
        return 0;
    time_t retv = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return retv;
}

#endif

struct Autoreset
{
public:
    bool* _reset;
    Autoreset(bool* r):_reset(r){*r=true;}
    ~Autoreset(){*_reset=false;}
};


#ifdef __linux

#define PATH_SEP "/"

inline bool IDLE_THREAD()
{
    pthread_yield();
    ::msleep(IDLE_SLEEP);
    return true;
}


inline kchar*  str_time()
{
/*
    static char buffer[64];
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 63, "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
 */
    static char timestamp[64];
    time_t  curtime = time(0);

    strcpy(timestamp, ctime(&curtime));
    char *pe = strchr(timestamp, '\r');
    if(pe)*pe=0;
    pe = strchr(timestamp, '\n');
    if(pe)*pe=0;
    return timestamp;
}



#define LASTR_ERR() errno

#else
inline bool msleep(int t)
{
    ::usleep(t*1000);
    return true;
}

inline size_t tick_count()
{
    return ::uptime();
}

inline bool IDLE_THREAD()
{
    SwitchToThread();
    ::msleep(IDLE_SLEEP);
    return true;
}

#define PATH_SEP "\\"

#define LASTR_ERR() GetLastError()

inline const char*  str_time()
{
    static char timestamp[64];
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf(timestamp, "%d-%d-%d %d:%d:%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    return timestamp;
}


#endif


#endif // OSGEN_H
