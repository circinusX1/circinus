/*------------------------------------------------------------------------------------------------
Copyright FLEX COMPUTING / FLEXTRONCS 
Author Marius Chincisan march-April 2009
-------------------------------------------------------------------------------------------------*/
#include "testserver.h"
#include "testserveruts.h"


/*------------------------------------------------------------------------------------------------
    - compile time messages
------------------------------------------------------------------------------------------------*/
#ifdef _DEBUG
        #pragma message ("*** BUILDING UNICODE DEBUG STATICALLY-MULTITHREADED LIBRARY   /lib/scstubd.lib")
#else
        #pragma message ("*** BUILDING ANSI RELEASE STATICALLY-MULTITHREADED LIBRARY   /lib/scstub.lib")
#endif //
 
static bool __initialized = false;
MutexCr*        TestServer::__mutex      = 0;
int             Settings::__http_retry   = 5;
DWORD           Settings::__http_tout_ms = 16000;
long            Settings::__max_https    = 400;
long            Settings::__max_telnets  = 400;
DWORD           Settings::__http_maxwait = 30000;
DWORD           Settings::__tel_maxwait  = 120000;
DWORD           Settings::__tweak_flag   = 0;
int             Settings::__http_priority = 0;
int             Settings::__telnet_priority = 0;


DWORD           FakeSemaphore::_last_hittime = 0;
/*----------------------------------------------------------------------------------------------
    - default flags and tracing functions
-----------------------------------------------------------------------------------------------*/
tstring	        TestServer::__configBuff;
term_debug*     TestServer::__ptd = 0;  
P_ATF TestServer::__aprintf  = &ATRACEX;       // ansi trace function
P_TTF TestServer::__tprintf  = &TRACEX;       // tchar trace function
DWORD TestServer::__tr_mode  = TRACE_INFO|TRACE_WARNINGS|TRACE_ERRORS|TRACE_DEBUG|TRACE_TERMINAL;
////DWORD TestServer::__tr_mode  = TRACE_WARNINGS|TRACE_ERRORS;

/*----------------------------------------------------------------------------------------------
 - def for log debug strings friendly statuses strings
----------------------------------------------------------------------------------------------*/
static const TCHAR* __s_states[64]=
{
    _T("eOFFLINE"),
    _T("eCONNECTING"),
    _T("eSUCCESS "),
    _T("eALREADY_MODE"),
    _T("eTEST_IN_USE"),
    _T("eUTSFLOWERROR"),
    _T("eLOGIN_AUTHENTICATION_FAIL"),
    _T("eCANNOT_RESOLVE_HOSTNAM"),
    _T("*"),
    
};

const TCHAR* __t_states[64] =
{
    _T("eNOT_CONNECTED"),
    _T("eINPROGRESS"),
    _T("eRUNNING"),
    _T("eNOT_RUNNING"),
    _T("ePASSED"),
    _T("ePASSED_REMOVE_UNIT"),
    _T("eFAILED"),
    _T("eFAILED_REMOVE_UNIT"),
    _T("eWAITING_FOR_USER_ACTION"),
    _T("eWAITING_FOR_USER_PROMPT"),
    _T("eEMPTY"),
    _T("eERR_INTERACTIVE_MODE"),
    _T("eNOT_CONFIGURED"),
    _T("eKEEP_TELNET"),
};

/*----------------------------------------------------------------------------------------------
 - def c-tor
----------------------------------------------------------------------------------------------*/
TestServer::TestServer():_start_state(eCANNOT_CONNECT),
                             _test_state(eNOT_CONNECTED),
                             _historical_status(eEMPTY),
                             _tid(::GetCurrentThreadId()),
                             _elapsed_time(0),
                             _sim_mode(false),
                             _pmon_function(0),
                             _puser_data(0)
{
    
}

/*----------------------------------------------------------------------------------------------
 - custom c-tor
----------------------------------------------------------------------------------------------*/
TestServer::TestServer(const TCHAR* slot, const TCHAR* host, 
                       const TCHAR* password):_username(slot),
                                                _hostname(host),
                                                _password(password),
                                                _start_state(eCANNOT_CONNECT),
                                                _test_state(eNOT_CONNECTED),
                                                _historical_status(eEMPTY),
                                                _elapsed_time(0),
                                                _tid(::GetCurrentThreadId()),
                                                _sim_mode(false),
                                                _pmon_function(0),
                                                _puser_data(0)
{
    _ausername = ustrings::t2a(_username);
}


TestServer::~TestServer(void)
{
    _TidCheck();
}

/*----------------------------------------------------------------------------------------------
    - allows to overwrite the trace functions. If main applicatin is making its logs or output 
         it should overwrite this function
            
        - P_ATF is a pointer to a function: ANSI characters as int *ptr(const char*,...)
        - T_TTF is a pointer to a function: TCHAR characters as int *ptr(const TCHAR*,...)
-----------------------------------------------------------------------------------------------*/
void    TestServer::SetTraceFunctios(P_ATF atf, P_TTF ttf)
{
    if(atf) TestServer::__aprintf = atf;    // ansi logs
    if(ttf) TestServer::__tprintf = ttf;    // printf logs
}

/*----------------------------------------------------------------------------------------------
    - sets the reace mode for the server library
    - trace modes are:
        #define TRACE_INFO          0x1
        #define TRACE_WARNINGS      0x4
        #define TRACE_ERRORS        0x4
        #define TRACE_DEBUG         0x8
        #define TRACE_CFGERR        0x10
-----------------------------------------------------------------------------------------------*/

void    TestServer::SetTraceMode(DWORD dwMode)
{
    TestServer::__tr_mode = dwMode;
}


/*----------------------------------------------------------------------------------------------
    - redirect the set states here to be able to log the state chanbge from one point
-----------------------------------------------------------------------------------------------*/
void  TestServer::_SetStartState(START_STATE ss)
{
    _start_state = ss;
    if(TestServer::__tr_mode & TRACE_INFO)
            TestServer::__tprintf( T_START_STATE_TXT, TSlot(), __s_states[(int)_start_state + 2]); // enum starts at -1
}

/*----------------------------------------------------------------------------------------------
    - redirect the set states here to be able to log the state chanbge from one point
-----------------------------------------------------------------------------------------------*/
void  TestServer::_SetTestState(TEST_STATE ts)
{
    if(_test_state != ts)
    {
        _test_state = ts;
        if(TestServer::__tr_mode & TRACE_INFO)
                TestServer::__tprintf( T_TEST_STATE_TXT, TSlot(), __t_states[(int)_test_state + 2]); // enum starts at -1
    }
}


static long __servers = 0;

/*----------------------------------------------------------------------------------------------
    - creates a desired server instance
-----------------------------------------------------------------------------------------------*/
TestServer* TestServer::CreateServerInstance(SRV_TYPE st, const TCHAR* s, const TCHAR* h, 
                                             const TCHAR* p, const TCHAR* cfg, bool isbuffer)
{
    TestServer* prv = 0;

    if(__initialized == false)
    {
        throw TestServerException(T_SRV_NOT_INITIALIZED, eSRV_NOT_INITIALIZED);
    }
	if(isbuffer)
	{
		TestServer::__configBuff = cfg;
	}
	
    switch(st)
    {
        case eSRV_UTS:
            {
                AutoLock    al(TestServer::__mutex);
                ++__servers;
                ATRACEX("***********************************THERE ARE %d SERVERS\n", __servers);
                prv =  new TestServer_UTS(s, h, p, cfg);
            }
            break;
        default:
        case eSRV_FELX_TAP:
            break;
        case eSRV_DB:
            break;
    }
    if(prv)
    {
        return prv;
    }
    throw TestServerException(T_SRV_NO_CODE, eSRV_NOT_IMPLEMENTED);
    return 0;
}

/*----------------------------------------------------------------------------------------------
    - destroys the instance
-----------------------------------------------------------------------------------------------*/
void        TestServer::DestroyServerInstance(TestServer* ps)
{
    AutoLock    al(TestServer::__mutex);
    if(ps)
    {
        delete ps;
        --__servers;
        ATRACEX("********************************************~THERE ARE %d SERVERS\n", __servers);
    }
}

/*----------------------------------------------------------------------------------------------
    - initializes static and dependen tata and libraries testserver uses
-----------------------------------------------------------------------------------------------*/
void             TestServer::InitializeServer()
{
    __initialized = true;
    TestServer::__mutex = new MutexCr;
    sock::Init();        

    if(TestServer::__tr_mode & TRACE_TERMINAL)
    {
        if(0 == __ptd)
        {
            __ptd = new term_debug();
            __ptd->start();
        }
    }

}

/*----------------------------------------------------------------------------------------------
    - cleans private library data. uninitialize the libraries test server uses
-----------------------------------------------------------------------------------------------*/
void             TestServer::UninitializeServer()
{
    if(__ptd)
    {
        __ptd->issuestop();
        delete __ptd;
    }
    sock::Uninit();     
    delete TestServer::__mutex;
}

/*----------------------------------------------------------------------------------------------
    - create test server
-----------------------------------------------------------------------------------------------*/
TestSrvPtr::TestSrvPtr(SRV_TYPE st, const TCHAR* s, const TCHAR* h, const TCHAR* p, const TCHAR* cfg, bool isbuffer)
{
    _p = TestServer::CreateServerInstance(st, s, h, p, cfg, isbuffer);
}

/*----------------------------------------------------------------------------------------------
    - destroy test server
-----------------------------------------------------------------------------------------------*/
TestSrvPtr::~TestSrvPtr()
{
    TestServer::DestroyServerInstance(_p);
}

