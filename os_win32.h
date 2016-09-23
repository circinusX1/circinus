/*------------------------------------------------------------------------------------------------
    Copyright FLEX COMPUTING / FLEXTRONCS 
    Author Marius Chincisan March-April 2009
    oslinux.h    - 
-------------------------------------------------------------------------------------------------*/
#ifndef __WIN32_OS_H__
#define __WIN32_OS_H__

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.             
    #define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif		
#pragma warning (disable: 4996)
//#define _WINSOCKAPI_
//#define __WINCRYPT_H__

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <winsock.h>
#include <process.h>
#include <direct.h>
#include <stdio.h>
#include <tchar.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include "sys/stat.h"

#pragma comment(lib,"Wsock32.lib")

//-----------------------------------------------------------------------------
#define NILLA   0x80000000
#define INLN    __forceinline
#define REG
#define NO_VT   __declspec(novtable)
#define VIRT
#define NAKED_  __declspec( naked )
#define ASM_    __asm
#define nvtclass    class __declspec(novtable)
#define OUT_
#define OVERWRITE
#define PSEP  "\\"
#define OPBS  '/'
#define BS    '\\'
#define BSS   "\\"
//-----------------------------------------------------------------------------
#define     str_len     (int)strlen
#define     t_strtok    strtok
#define     t_atoi      atoi
#define     t_printf    printf   
#define     t_sprintf   sprintf  
#define     t_strncmp   strncmp

#define f_open      fopen
#define f_read      fread
#define f_create    fopen 
#define f_close     fclose
#define f_write     fwrite 
#define f_printf    fprintf
#define f_puts      fputs 
#define f_gets      fgets 
#define f_eof       feof
#define f_error     ferror
#define s_tat      _stat
#define f_stat     _fstat
#define un_link     _unlink
#define mk_dir      _mkdir
#define sn_printf   _vsnprintf



namespace os
{
//file    
    typedef FILE     H_FILE;
 //XCHAR
    INLN void    ge_getch(){getch();};
    INLN void    ge_sleep(DWORD milisecs){Sleep(milisecs);}
    INLN void    ge_idle(){Sleep(8);}
    INLN DWORD   ge_gettick(){return GetTickCount();}
    INLN long    ge_error(){return GetLastError();}
    INLN void    ge_error(long err){SetLastError(err);}
    INLN void   str_time(char* dTime){
      SYSTEMTIME st;
      GetSystemTime(&st);
      sprintf(dTime, "%u/%u/%u %u:%u:%u:%u",st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,st.wMilliseconds);
    }
};

//-----------------------------------------------------------------------------
struct MutexCr
{
    mutable CRITICAL_SECTION _cs;
    MutexCr(){
        InitializeCriticalSection(&_cs);
    }
    ~MutexCr(){
        DeleteCriticalSection(&_cs);
    }
    int Lock() {
        EnterCriticalSection(&_cs);
        return 0;
    }
    int TryLock() 
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
    int Unlock() { 
        LeaveCriticalSection(&_cs); 
        return 1; 
    }
};

//-----------------------------------------------------------------------------
struct csemaphor
{
    HANDLE _sem;
    csemaphor( int init = 0 ){
        _sem = CreateSemaphore(0,init,0x7fffffff,0);
    }
    ~csemaphor(){ CloseHandle(_sem); }
    int Wait(int to = INFINITE) {
        return WaitForSingleObject((HANDLE)_sem,to)==WAIT_OBJECT_0;
    }
    int TryWait(int to = INFINITE) {
        return ((WaitForSingleObject((HANDLE)_sem,to)==WAIT_OBJECT_0) ? 1 : -1);
    }
    int  Notify() const{
        return (ReleaseSemaphore((HANDLE)_sem, 1, 0) ? 0 : -1);
    }

    int  Value() { long val = -1;
        ReleaseSemaphore((HANDLE)_sem,0,&val); return val;
    }
    void Reset( int init = 0 ){
        CloseHandle(_sem);
        _sem = CreateSemaphore(0,init,0x7fffffff,0);
    }
};

class CHEvent
{
public:
    CHEvent(int imr=0)
    {
        _he=CreateEvent(0,imr,0,0);
        _waiting=0;
    }
    ~CHEvent(){   
        if(_waiting)
        {
      SetEvent(_he); 
      wait(1000);
        }
        ::CloseHandle(_he);
    }
    void signal(){
        SetEvent(_he); 
    }
    void reset(){
        ResetEvent(_he); 
    }
    void pulse(){
        PulseEvent(_he); 
    }
    int wait(int miis = INFINITE){
        _waiting=1;
        int rv = WaitForSingleObject( _he,miis ); 
        _waiting=0;
        return rv;
    }

private:
    HANDLE _he;
    long   _waiting;
};

//-----------------------------------------------------------------------------
struct AutoLock
{
    AutoLock(MutexCr* MutexCr):o_mutex(MutexCr){if(o_mutex)o_mutex->Lock();}
    ~AutoLock(){if(o_mutex)o_mutex->Unlock();}

    MutexCr* o_mutex;
};

//-----------------------------------------------------------------------------
#pragma warning (disable:4312)
class osthread
{
public:
    typedef DWORD THANDLE;
    osthread(){
        _bstop   = 1;
        _hthread = 0;
        _stopped = 1;
    }

    virtual ~osthread(){
        if(!_stopped)
      kill();
		//if(_hthread) 
		//	CloseHandle((HANDLE)_hthread);
    }

    virtual int     start()
    {
        _bstop   = 0;
        _stopped = 0;
        _hthread = (THANDLE)_beginthread(SFoo, 0, (void*)this);
        os::ge_sleep(32);
        _start.Wait();
        return 0;
    }
    virtual void issuestop()
    {
        _bstop = 1;
		os::ge_sleep(100);
    }

    virtual void    stop()
    {
		if(!isstopped())
			issuestop();
		if(!isstopped())
        {
			if(join()==WAIT_TIMEOUT)
                kill();
        }
    }

    long  isstopped()
    {
        return _stopped;
    }

    int join(){
		if(_hthread)
			return WaitForSingleObject((HANDLE)_hthread,10000);
		return WAIT_OBJECT_0;
    }

    int kill(){
        if(_hthread) TerminateThread((HANDLE)_hthread, 0);
	   _hthread = 0;
       _stopped = 1;
       return 0;
    }
    void leave()
    {
		_bstop = 0;
		_stopped = 1;
    }
    int detach(){}
    virtual void run()=0;
	BOOL  running()
    {
        return !_bstop;
    }

protected:
    csemaphor   _start;
    long        _bstop;
    long        _stopped;
    THANDLE     _hthread;
    static void SFoo(void* pData){
        osthread* pT = reinterpret_cast<osthread*>(pData);
        pT->_start.Notify();
        pT->run();
        pT->_stopped = 1;
        _endthread();
    }
};





#define _PLATFORM_ "WINDOWS MS"





#endif // !__OS_H__
