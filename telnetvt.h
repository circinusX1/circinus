/*------------------------------------------------------------------------------------------------
    Copyright FLEX COMPUTING / FLEXTRONCS 
    Author Marius Chincisan march-April 2009
    telnetvt.h - definition for telnet virtual client terminal class
-------------------------------------------------------------------------------------------------*/

#ifndef __TELNET_VT_H__
#define __TELNET_VT_H__

#include "os.h"
#include "sock.h"
#include "ustrings.h"
#include "teloptions.h"
#include <string>
#include <deque>

#include "utsscript/plus/sqplus.h"

/*-------------------------------------------------------------------------------------------------
    synched_queue
        - thread protected que 
-------------------------------------------------------------------------------------------------*/
template <typename T>class synched_queue
{
    std::deque<T>  q;      
    MutexCr        m;       
    CHEvent        e; 
public:
    void clear()
    {
        AutoLock sl(&m); 
        q.clear();
    }
    void push(const T & value){
        AutoLock sl(&m); 
        q.push_back(value);
        e.signal();
    }
    bool try_pop(T & value){
        AutoLock sl(&m); // NOTE: lock mutex
        if (q.size()){
            value = q.front();
            q.pop_front();
            if (q.size()){
                e.signal();
            }
            return true;
        }
        return false;
    }
    T wait_pop() {
        AutoLock sl(&m); // NOTE: lock mutex
        e.wait();

        T value = q.front();
        q.pop_front();
        if (q.size()){
            e.signal();
        }
        return value;
    }

    size_t size() {
        AutoLock sl(&m); 
        return q.size();
    }

    bool empty() const {
        AutoLock sl(&m); 
        return q.empty();
    }
};
#define CIRC_SIZE   4096

/*-------------------------------------------------------------------------------------------------
telnet class
    - opens a connection to a telnet host
    - negociate terminal type ansi, any other option wont be supported (no echo, no ga no noting)
    - starts a thread where talks to the server host and deals with messy options
---------------------------------------------------------------------------------------------------*/
class TestServer;
class term_debug;
class telnetvt : public osthread
{
public:
    friend class term_debug;
    telnetvt(){_notify=false;}
    telnetvt(const TCHAR* ip, int port);
    ~telnetvt();

    void  cleancache();
    BOOL  open();
    BOOL  reopen();
    void  close();
    void  flush();
    void  flush_over();
    void  put(const char* pb);
    void  put_one(const char* pb);
    void  put(BYTE* pb, int length);
    void  put(const std::string& pb);
    const char* get(std::string& out, int tout);
    const char* xget(const std::string& from, std::string& out);
    bool  wait(const char* substr, int tout);
    int   lasterror(){return _socket.error();}
    BOOL  isonline(){return _online;}
    bool  steady(long tout, long maximt);
    void  run();
    void  setserver(TestServer* pts, DWORD);
    void  disable();
    bool  wait_for_some(DWORD tout);  
    void  notify(bool bt)  {_notify = bt;}
    void  save_screen(FILE* pf);
    void  show_screen(bool msgbox, const char* caption);
    
private:
    void _flush_out();
    void _logbin(const char* info, int, bool ascii=true);
    int  _test(const char* substr);
    void _proc_options_data(char* buff, int& length, int& outlength);
    int  _process_tel_option(char* pStr, int length);
    int  _process_ansi_option(char* pStr, int);
    int  _sendoptresp(char* pStr, telopt_struct* po);


private:
    astring                 _ip;
    int                     _port;
    MutexCr                 _cs;    
    tcp_cli_sock            _socket;
    astring                 _screen;
    synched_queue<astring>  _inputs;
    BOOL                    _online;
    DWORD                   _last_update;
    TestServer*             _pts;
    DWORD                   _pthrid;
    BOOL                    _closing;
    bool                    _notify;
    bool                    _supress_errors;
};

/*-------------------------------------------------------------------------------------------------
    debug server terminal
        - used to test saved uts streams
        = play back server fo uts sessions
-------------------------------------------------------------------------------------------------*/
class   term_debug : public osthread
{
public:
    term_debug(){
        _reset_file_ptr=false;
        _port = 23001;
    }

    ~term_debug(){
        _srv.destroy();
        _socket.destroy();
        stop();
    }

    bool online(){
        return _socket.isopen();
    }

    void run()
    {
        while(running())
        {
            pool();
            os::ge_sleep(10);
        }
    }
    void issuestop()
    {
        _socket.destroy();
        _srv.destroy();
        __super::issuestop();
        os::ge_sleep(2000);
    }

    void reset_file_ptr();
    void pool();
    static void record(const char* local, int local_bytes, const char* slot);
private:
    fd_set          _wrset;
    fd_set          _rdset;
    tcp_cli_sock    _socket;
    tcp_srv_sock    _srv;
    int             _port;
    bool            _reset_file_ptr;
    MutexCr         _cs;    
};


#endif //__TELNET_VT_H__

