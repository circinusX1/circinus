/*------------------------------------------------------------------------------------------------
    Copyright FLEX COMPUTING / FLEXTRONCS 
    Author Marius Chincisan March-April 2009
    oslinux.h    - 
-------------------------------------------------------------------------------------------------*/
#ifndef __OSLINUX_H__
#define __OSLINUX_H__

#define   SOCKET_ERROR   -1
#define   INVALID_SOCKET -1
#define    INFINITE      NILLA

#include <stdarg.h>
#include <errno.h>
#include <resolv.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <asm/ioctls.h>
#include <assert.h>
#include <string.h>
#include <netdb.h>
//#include <curses.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <stdlib.h> // atoi
#include <ctype.h>  // isdigit
#include <unistd.h>

#define INLN        inline


//-----------------------------------------------------------------------------
typedef unsigned short   WORD;
typedef size_t		     DWORD;
typedef unsigned int     UINT;
typedef char       TCHAR;
typedef int        BOOL;
typedef unsigned char    BYTE;

#define TRUE       1
#define FALSE      0

#define  _MAX_PATH       512
#define  NILLA           0x80000000
#define  LOWORD(_x)      (_x&0xFFFF)
#define  HIWORD(_x)      ((_x&0xFFFF0000)>>16)
#define  MAKELONG(a,b)   (a|(b<<16))
#define  ASSERT          assert
#define OUT_
#define OVERWRITE
#define PSEP  "/"


#define str_len     (int)strlen
#define f_open      fopen
#define f_read      fread
#define f_create    fopen
#define f_close     fclose
#define f_write     fwrite
#define f_puts      fputs
#define f_printf    fprintf
#define f_gets      fgets
#define f_stat      fstat
#define t_sprintf   sprintf
#define s_tat       stat
#define f_stat      fstat

#define un_link     unlink
#define mk_dir(a_)  mkdir(a_, 0777)
#define sn_printf   vsnprintf
#define t_printf    printf
#define t_atoi      atoi
#define t_strncmp   strncmp
#define t_strtok    strtok


namespace os
{
//public:
    typedef FILE     H_FILE;
//file

//char
    INLN void    ge_getch(){;};
	INLN void    ge_sleep(DWORD milisecs){usleep(milisecs * 1000);};
	INLN void    ge_idle(){usleep(8000);};
	INLN long    ge_error(){return errno;};
	INLN void    ge_error(long err){errno=err;};
	INLN DWORD   ge_gettick()
	{
        struct timeval     t;
        ::gettimeofday(&t, NULL);
        return (t.tv_sec * 1000) + (t.tv_usec / 1000);
    }
    INLN void  str_time(char* dTime){//++++

	    time_t  curtime = time(0);
	    strcpy(dTime, ctime(&curtime));
	    char *pe = strchr(dTime,'\r'); if(pe)*pe=0;
      pe = strchr(dTime,'\n'); if(pe)*pe=0;
    }
};

//#define max(a_,b_)	((a_ >= b_) ? a_ : b_)
//-----------------------------------------------------------------------------
class MutexCr
{
    mutable pthread_mutex_t _mut;
public:
    MutexCr()
    {
        pthread_mutexattr_t  attr;
		::memset(&attr,0,sizeof(attr));
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&_mut,&attr);
        pthread_mutexattr_destroy(&attr);
    }

    virtual ~MutexCr(){
        pthread_mutex_unlock(&_mut);
        pthread_mutex_destroy(&_mut); }

    int Lock() const{
        return pthread_mutex_lock(&_mut);
    }

    int TryLock() const {
        return pthread_mutex_trylock(&_mut);
    }

    int Unlock() const{
        return pthread_mutex_unlock(&_mut);
    }
};

//-----------------------------------------------------------------------------
class csemaphor
{
    sem_t _sem;

public:
    csemaphor( int init = 0 )
    {
        ::sem_init(&_sem,0,init);
	}

    ~csemaphor()
	{
        ::sem_destroy(&_sem);
    }

    INLN int Wait(int to=-1) const
	{
        ::sem_wait((sem_t *)&_sem);
        return 1;
    }

    INLN int TryWait(int to=-1) const
	{
        return (sem_trywait((sem_t *)&_sem)?errno:0);
    }

    int Notify() const
	{
        return (sem_post((sem_t *)&_sem) ? errno : 0);
    }

    int Value() const {
        int val = -1;
        sem_getvalue((sem_t *)&_sem,&val);
        return val;
    }
    void Reset( int init = 0 ){
        ::sem_destroy(&_sem);
        sem_init(&_sem,0,init);
    }
};

class CHEvent
{
public:
    CHEvent(BOOL mr=0, const char* name=0)
    {
        pthread_cond_init(&_he,0);
    }
    ~CHEvent(){
        if(_waiting)
        {
      pthread_cond_broadcast(&_he);
      os::ge_sleep(1000);
        }
        pthread_cond_destroy(&_he);
    }
    void signal(){
        pthread_cond_broadcast(&_he);
    }
    void reset(){
        ;
    }
    void pulse(){
        pthread_cond_broadcast(&_he);
    }
    int wait(int miis=NILLA)
	{
        _waiting=1;
        pthread_mutex_t t;

		pthread_cond_wait(&_he, &t );

        _waiting=0;
        return 0;
    }

private:
    pthread_cond_t _he;
    long           _waiting;
};


//-----------------------------------------------------------------------------
class AutoLock
{
public:
    AutoLock(MutexCr* mutex):o_mutex(mutex){if(o_mutex)o_mutex->Lock();}
    ~AutoLock(){if(o_mutex)o_mutex->Unlock();}
private:
    MutexCr* o_mutex;
};

//-----------------------------------------------------------------------------
class osthread
{
public:
    typedef pthread_t THANDLE;
    osthread(){
        _bstop   = 1;
        _hthread = 0;
		_stopped = 1;
    }

    virtual ~osthread(){
        if(!_stopped)
      Stop();
		if(_hthread)
			pthread_detach(_hthread);
    }

    int     start()
    {
		_stopped = 0;
        _bstop   = 0;

        pthread_attr_t  attr;
        ::pthread_attr_init(&attr);
        ::pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        if (::pthread_create(&_hthread, &attr, SFoo, this) != 0)
        {
      ::pthread_attr_destroy(&attr);
      return os::ge_error();
        }
        ::pthread_attr_destroy(&attr);
        os::ge_sleep(100);
        _start.Wait();
        return 0;
    }

    void issuestop()
    {
        _bstop = 1;
		os::ge_sleep(100);
    }
    void    stop()
    {
		if(!_stopped)
		{
			issuestop();
		}
		if(!_stopped)
		{
			join();
		}
    }

    BOOL  running()
    {
        return !_bstop;
    }

    long  isstopped()
    {
        return _stopped;
    }
    INLN int join()
	{
		if(_hthread)
        	return ::pthread_join(_hthread, 0);
		return 0;
    }

	void leave()
	{
		_stopped = 1;
	}

    int kill(){
		return 0;
    }
    int detach(){return 0;}
    virtual void run()=0;

protected:
    csemaphor   _start;
    long        _bstop;
    THANDLE     _hthread;
    long        _stopped;

    static void* SFoo(void* pData)
    {
		osthread* pT = reinterpret_cast<osthread*>(pData);
        pT->_stopped = 0;
        pT->_start.Notify();
        pT->run();
        pT->_stopped = 1;
		return 0;
    }
};

#define _PLATFORM_ "LINUX"

typedef  char TCHAR;
#define _T(c_) c_
#define _tcscpy strcpy
#define _tcscat strcat
#define _tcsicmp strcasecmp
#define _tcsncmp strncasecmp
#define _stprintf t_sprintf

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

inline int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}



#endif // !__OS_H__
