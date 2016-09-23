/*------------------------------------------------------------------------------------------------
Copyright FLEX COMPUTING / FLEXTRONCS 
Author Marius Chincisan march-April 2009
-------------------------------------------------------------------------------------------------*/
#pragma once

#include<string>
#include <deque>
#include "ustrings.h" 
#include "utsscript/plus/sqplus.h"
#include "telnetvt.h"
/*------------------------------------------------------------------------------------------------
 server type
------------------------------------------------------------------------------------------------*/
typedef enum _SRV_TYPE{
    eSRV_UTS,                   // implementted
    eSRV_FELX_TAP,              // pending
    eSRV_DB,                    // pending
}SRV_TYPE;

/*------------------------------------------------------------------------------------------------
 server exception codes
------------------------------------------------------------------------------------------------*/
typedef enum _SRV_EX_CODES
{
    eSRV_NOT_INITIALIZED    = -5,
    eSRV_NOT_CONFIGURED     = -4,
    eSRV_NOT_IMPLEMENTED    = -3,
    eCON_BROKEN             = -2,
    eCANNOT_CON             = -6,
    eINCOMPLETTE_RESPONSE   = -7,
    eCANNOT_RW              = -8,
}SRV_EX_CODES;

/*------------------------------------------------------------------------------------------------
    start states
------------------------------------------------------------------------------------------------*/
typedef enum _START_STATE
{   
    eOFFLINE  = -2,                  // initial state 
    eSTARTING = -1,                  // initial state 
    eSUCCESS  =  0,                  // Start/Stop succedeed
    eALREADY_MODE,                   // start find the test already running
    eTEST_IN_USE,                    // uts log to that slot is opened by another terminal
    eUTSFLOWERROR,                   // navigating to screens failed. see log
    eTIMEOUT,                        // waiting operation has timeout
    eLOGIN_AUTHENTICATION_FAILURE,   // user password failed
    eINVALID_SERIAL,                 // serial does not match
    eCANNOT_CONNECT,                 // cannot connect
}START_STATE;

/*------------------------------------------------------------------------------------------------
    test states
------------------------------------------------------------------------------------------------*/
typedef enum _TEST_STATE
{
    eNOT_CONNECTED = -2,            // initial state
    eINPROGRESS = -1,
    eRUNNING = 0,                   // test is runing
    eNOT_RUNNING,                   // not running
    ePASSED,                        // finsihed and passed    
    ePASSED_REMOVE_UNIT,            // SHOULD FOLLOW EPASSED !!!!!!!
    eFAILED,                        // test finished but is in error
    eFAILED_REMOVE_UNIT,            // SHOULD FOLLOW EFAILED !!!!!!!!!!
    eWAITING_FOR_USER_ACTION,       // file user_action_required_flag found - 
    eWAITING_FOR_USER_PROMPT,       // durring monitoring found the Continue [ Abort Cancel screen
    eEMPTY,                         // empy state. For historical info
    eERR_INTERACTIVE_MODE,          // unexcpected interacive mode. durring monitoring
    eNOT_CONFIGURED,                // internal state
    e_STATE_LAST
}TEST_STATE;



/*------------------------------------------------------------------------------------------------
 debug bit oriented modes
------------------------------------------------------------------------------------------------*/
#define TRACE_INFO       0x1
#define TRACE_WARNINGS   0x2
#define TRACE_ERRORS     0x4
#define TRACE_DEBUG      0x8
#define TRACE_TERMINAL   0x10

class TestServer;
typedef int (*P_ATF)(const char *,...);
typedef int (*P_TTF)(const TCHAR *,...);
typedef void (*P_MON)(void* userData, TestServer* pts);
/*------------------------------------------------------------------------------------------------
 exception trown by the library calls
------------------------------------------------------------------------------------------------*/
class TestServerException
{
public:   
    //---------------------------------------------------------------------------------------------
    // keep it simple on the stack. no mem allocation on heap in exception handlers
    //---------------------------------------------------------------------------------------------
    TestServerException(long excode, const TCHAR* lpszFormat, ...){
        va_list args;
        va_start(args, lpszFormat);
        int nBuf = _vsntprintf(_what, __sfz(TCHAR, _what), lpszFormat, args);
        va_end(args);
        _excode = excode;
        _winerror = os::ge_error();
    }
    TestServerException(const TCHAR* what, long excode, DWORD error=0){
        if(0 == error)
            _winerror = os::ge_error();
        _excode = excode;
        ::_tcscpy_s(_what, __sfz(TCHAR,_what), what);
    }
    const TCHAR* What()const{return _what;}
    DWORD        WinError()const{return _winerror;}
    long         Code()const{return _excode;}
    const TCHAR* Description(){::_stprintf(_desc,_T("%s: [%d], Exception code: %d\n"), _what, _winerror, _excode); return _desc;}
private:
    TCHAR       _desc[256];                // 
    TCHAR       _what[128];                // once for all. on static data section
    DWORD       _winerror;
    long        _excode;
};


/*------------------------------------------------------------------------------------------------
    base class test server.

    -use it in app
     TestServer pt = new TestServer_UTS(slot, host, password, config_file);
------------------------------------------------------------------------------------------------*/
class TestSrvPtr;
class term_debug;
class TestServer
{
public:
    friend class TestSrvPtr;
    //
    //  creation destruction methods
    //
    static void             InitializeServer();
    static void             UninitializeServer();

    //
    // trace debug functions
    //
    static void             SetTraceFunctios(P_ATF atf, P_TTF ttf);
    static void             SetTraceMode(DWORD dwMode);
    //
    //   public methods
    //
    virtual void            StopMonitorring(bool b_shutting_down=true)=0;
    virtual BOOL            IsOnLine() = 0;
    virtual START_STATE     StartTest(const TCHAR* serial)=0;
    virtual bool            ValidateSerialNumber(const TCHAR* serialnumber)=0;
    virtual START_STATE     AbortTest() = 0;
    virtual TEST_STATE      GetTestState()=0;
    virtual START_STATE     GetStartState()=0;
    virtual bool            AcknowledgeUserPrompt()=0;
    virtual bool            AcknowledgeUnitRemoved()=0;
    virtual int             IsSlotDisconnected()=0;
    virtual bool            UpdateCurrentInfo()=0;
    virtual void            Reset()=0;
    
    
    virtual TEST_STATE      GetHistoricalInfo(TCHAR* step, TCHAR* histserial)=0;
    virtual TEST_STATE      GetCurrentInfo(TCHAR* step, long& sectime, TCHAR* serial, TCHAR* result_id)=0;

    //
    // helper functions
    //_failed_step
    const   TCHAR*          ResultId()const{return _result_id.c_str();}
    const   TCHAR*          SerialString()const   {return _serial_no.c_str();}
    const   TCHAR*          FailedStepString()const {return _historical_failed_step.c_str();}
    const   TCHAR*          StatusString()const {return _step.c_str();}
    virtual void            OnTelnetDataReceived(size_t datalen) = 0;
    DWORD                   Tid(){return _tid;}
    const TCHAR*            TSlot()const{ return _username.c_str();}
    const TCHAR*            GetErrorString()const {return _error_string.c_str();}
    const char*             ASlot()const{ return _ausername.c_str();}
    void                    SetSimulationMode(bool mode){
        _sim_mode = mode;
        _sim_time = os::ge_gettick();
    }
    void                    SetMonitorFunction(void* pdata, P_MON pm){_puser_data=pdata; _pmon_function = pm;}

    static TestServer*      CreateServerInstance(SRV_TYPE st, const TCHAR* s, const TCHAR* h, 
                                                                    const TCHAR* p, const TCHAR* cfg, bool isbuffer);
    static void             DestroyServerInstance(TestServer*);



protected:
    TestServer();//
    TestServer(const TCHAR* slot, const TCHAR* host, const TCHAR* password);
    virtual ~TestServer(void);
    void                    _SetStartState(START_STATE);
    void                    _SetTestState(TEST_STATE);
    const TCHAR*            UTS_Hostname()const{return _hostname.c_str();}
    const TCHAR*            Username()const{return _username.c_str();}
    const TCHAR*            Password()const{return _password.c_str();}
    void _TidCheck()const     {
        if(_tid != ::GetCurrentThreadId())
        {
            ATRACEX("*** YOU CANNOT CALL/DESTROY THIS OBJECT FROM A THREAD THAT DID NOT CREATED THE OBJECT.***\n");
            ATRACEX("*** THIS OBJECT HAS TLS STATIC DATA***\n");
            assert(0); //you cannot delete the thread from another thread that created the thread
        }
    }
    static void         CreateSempahor();
protected:
    MutexCr          _mutex;
    tstring          _hostname;
    tstring          _username;
    astring          _ausername;
    tstring          _password;
    START_STATE      _start_state;
    TEST_STATE       _test_state;
    TEST_STATE       _historical_status;   
    tstring          _serial_no;
    tstring          _serial_input;
    tstring          _historical_failed_step;
    tstring          _step;
    tstring          _error_string;

    tstring          _result_id;
    tstring          _swdla_path;
    long             _elapsed_time;
    DWORD            _tid;
    bool             _sim_mode;   
    DWORD            _sim_time;
    P_MON            _pmon_function;
    void*            _puser_data;
    
public:
    static P_ATF        __aprintf;       // ansi trace function
    static P_TTF        __tprintf;       // tchar trace function
    static DWORD        __tr_mode;
	static tstring	    __configBuff;
    static MutexCr*     __mutex;
    static term_debug*  __ptd;  

};


/*------------------------------------------------------------------------------------------------
 - exported settings to script contenxt
------------------------------------------------------------------------------------------------*/
class Settings
{
public:
    Settings()
    {
    }
    ~Settings()
    {
    }
    static int          __http_retry;
    static DWORD        __http_tout_ms;
    static long         __max_https;
    static long         __max_telnets;
    static DWORD        __http_maxwait;
    static DWORD        __tel_maxwait;
    static DWORD        __tweak_flag;
    static int          __http_priority;
    static int          __telnet_priority;
};

DECLARE_INSTANCE_TYPE(Settings);


/*------------------------------------------------------------------------------------------------
 - fake - semaphore to avoid use of kernel objects
 - does not ensure FIFO. OUT is kindof random.
------------------------------------------------------------------------------------------------*/
class FakeSemaphore
{
public:
    FakeSemaphore(long &cur_eaters, long max_eaters, DWORD timeout, BOOL* pBreak):p_cur_eaters(&cur_eaters)
    {
        //
        // dont let more than max_eaters to get trough
        //
        if(Settings::__tweak_flag == 1)
        {
            DWORD   maxw = os::ge_gettick() + timeout;
            if(cur_eaters > max_eaters)
            {
                ATRACEX("%d - waiting to get trough \r\n", GetCurrentThreadId());
                while(cur_eaters > max_eaters && os::ge_gettick() < maxw)
                {
                    if(*pBreak)
                        return;
                    os::ge_sleep(16);
                }
                ATRACEX("%d - got trough \r\n", GetCurrentThreadId());
                
            }
        } 
        //
        // disperse eaters each Settings::__tweak_flag mseconds
        //
        else if(Settings::__tweak_flag > 1)
        {
            DWORD ct = os::ge_gettick();
            if(0==_last_hittime)
                _last_hittime=ct;
            long diff =(LONG)(ct - _last_hittime);
            if(diff < (long)Settings::__tweak_flag)
            {
                ATRACEX("%d - sleeping %d \r\n", GetCurrentThreadId(), Settings::__tweak_flag - diff);
                os::ge_sleep(max(512L, Settings::__tweak_flag - diff));
            }
            _last_hittime = os::ge_gettick();
        }
        ::InterlockedIncrement(&cur_eaters);
    }
    ~FakeSemaphore()
    {
        ::InterlockedDecrement(p_cur_eaters);
    }
    long            *p_cur_eaters;
    static DWORD    _last_hittime;
};

/*------------------------------------------------------------------------------------------------
 - smart pointer around increment decrement
------------------------------------------------------------------------------------------------*/
class PlusMinus
{
public:
    PlusMinus(long& value):p_value(&value)
    {
        ::InterlockedIncrement(p_value);
    }
    ~PlusMinus()
    {
        ::InterlockedDecrement(p_value);
    }
    long    *p_value;
};

/*------------------------------------------------------------------------------------------------
 - smart pointer around the server
------------------------------------------------------------------------------------------------*/
class TestSrvPtr
{
public: 
    TestSrvPtr(SRV_TYPE st, const TCHAR* s, const TCHAR* h, const TCHAR* p, const TCHAR* cfg, bool isbuffer);
    ~TestSrvPtr();
    TestServer*  operator->(){return _p;}
    TestServer*  Obj(){return _p;}
private:
    void* operator new(size_t sz){}                              // not on heap
    TestSrvPtr(const TestSrvPtr& r){};                           // not copyable, not passable by value
    TestSrvPtr& operator=(const TestSrvPtr& r){return *this;};   // not copyable
    TestServer* _p;
};

