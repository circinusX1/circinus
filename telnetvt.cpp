/*------------------------------------------------------------------------------------------------
Copyright FLEX COMPUTING / FLEXTRONCS 
Author Marius Chincisan march-April 2009
telnetvt.h - implementation for telnet virtual client terminal class
-------------------------------------------------------------------------------------------------*/

#include "os.h"
#include "telnetvt.h"
#include "teloptions.h"
#include "testserver.h"



static long     __telnet_sessions = 0;
/*------------------------------------------------------------------------------------------------
- telnet options 
-------------------------------------------------------------------------------------------------*/
telopt_struct __telOpttions[]=
{
    {{TC_IAC, TC_SB, TOPT_TERM, TERM_SEND, TC_IAC, TC_SE}, 6, {TC_IAC, TC_SB, TOPT_TERM, TERM_IS, 'A', 'N', 'S','I', TC_IAC, TC_SE}, 10},
    {{TC_IAC, TC_WILL, TOPT_TERM , TERM_SEND}, 3, {TC_IAC, TC_DO, TOPT_TERM }, 3},
    {{TC_IAC, TC_DO, TOPT_TERM }, 3, {TC_IAC, TC_WILL, TOPT_TERM }, 3},
    {{TC_IAC, TC_WILL, TC_IAC}, 2, {TC_IAC, TC_DONT, 0}, 3}, // dont support
    {{TC_IAC, TC_DO,   TC_IAC}, 2, {TC_IAC, TC_WONT, 0}, 3},
    {{TC_IAC, TC_WONT, TC_IAC}, 2, {TC_IAC, TC_DONT, 0}, 3},
    {{TC_IAC, TC_DONT, TC_IAC}, 2, {TC_IAC, TC_WONT, 0}, 3},
    {{0}, 0, {0}, 0},
};



/*------------------------------------------------------------------------------------------------
- ansi terminal specific negociation, support eraseline/things like this
-------------------------------------------------------------------------------------------------*/
telopt_struct __ansiOpttions[]=
{
    //{{0x1B, 0x5B, 0x72, 0x1B, 0x5B}, 5, {0x1B, 0x5B, 0x34, 0x35, 0x3B, 0x38, 0x30, 0x52, 0x00}, 9},
    {{0x1B, 0x37, 0x1B, 0x5B, 0x72}, 5, {0x1B, 0x5B, 0x34, 0x35, 0x3B, 0x38, 0x30, 0x52, 0x00}, 9},
    {{0x1b, 0x5b, 0x34, 0x35, 0x3b, 0x31, 0x48}, 7, {0}, 0}, // skip this buggy one
    {{0}, 0, {0}, 0},
};

/*------------------------------------------------------------------------------------------------
- trl sequence mapping
-------------------------------------------------------------------------------------------------*/
struct  skctrlmap
{
    char ssek[16];
    char smap[8];
    int  length;
} __sctrlmapping[]=
{
    {"@ENTER",   {0x13, 0x10}, 2},
    {"@BREAK",   {0x3}, 1},
    {"@END",     {0x1B,0x5B,0x34,0x7E}, 4}, //
    {"@ESC",     {0x1B}, 1},
    {"",""},
};

//------------------------------------------------------------------------------------------------
// log binary function for debug purpose only
//------------------------------------------------------------------------------------------------

#define _TEST_ONLINE() if(_online==false)\
                throw TestServerException(T_CONN_OFFLINE1, eCON_BROKEN);
/*------------------------------------------------------------------------------------------------
constructor
- holds references to login host and port where to connect
-------------------------------------------------------------------------------------------------*/
telnetvt::telnetvt(const TCHAR* ip, int port)
{
    _ip     = ustrings::t2a(ip);
    _port   = (port); 
    _online = (FALSE);
    _closing= FALSE;
    _notify = false;
    _supress_errors = false;
}

/*------------------------------------------------------------------------------------------------
closes the connection. waits for thread to exit
-------------------------------------------------------------------------------------------------*/
telnetvt::~telnetvt()
{
    if(_online)
    {
        close();
    }
}


/*------------------------------------------------------------------------------------------------
    - if debugmode & TRACE_TERMINAL delete the bin uts received data
-------------------------------------------------------------------------------------------------*/
void  telnetvt::cleancache()
{
    _unlink(MKSTR("testmode/%s.utb", _pts->ASlot()));

    if(TestServer::__tr_mode & TRACE_TERMINAL)
    {
        if(TestServer::__ptd)
            TestServer::__ptd->reset_file_ptr();

    }
}

/*------------------------------------------------------------------------------------------------
opens a conenction to the telnet host.
-returns true for success, false for - cannot connect.
- get last OS error calling telnetvt::lasterror()
-------------------------------------------------------------------------------------------------*/
BOOL  telnetvt::open()
{
/* test elimination of escape codes
    char __buff[]={0x53 ,0x1b ,0x5b ,0x30 ,0x3b ,0x37 ,0x6d ,0x0f ,0x74 ,0x61 ,0x72 ,0x74 ,0x20 ,0x54 ,0x65 ,0x73 ,0x74 ,0x20 ,0x1b ,0x5b
                   ,0x35 ,0x34 ,0x43 ,0x1b ,0x5b ,0x30 ,0x6d ,0x0e ,0x78 ,0x1b ,0x5b ,0x30 ,0x3b ,0x37 ,0x6d ,0x0,0xf ,0x20 ,0x0a ,0x1b ,0x5b ,0x36
                   ,0x39 ,0x44 ,0x1b ,0x5b ,0x30 ,0x6d ,0x0e ,0x78 ,0x0f ,0x20 ,0x1b ,0x5b ,0x30 ,0x3b ,0x34 ,0x6d ,0x0f};

    char* inb = __buff;
    int outb;
    printf("%s\r\n\r\n", inb);
    int   inlen = sizeof(__buff);
    _proc_options_data(inb, inlen, outb);
    printf("%s", inb);
*/
    _supress_errors = false;
    _closing        = FALSE;

    //
    // wait here to limit the seessions to maximum Settings::__max_telnets
    //
    FakeSemaphore   fs(__telnet_sessions, Settings::__max_telnets, Settings::__http_maxwait, &_closing);
  
    if (0 == this->osthread::start())
    {
        DWORD ct  = os::ge_gettick()+30000;             // 30 seconds to connect
        while(os::ge_gettick() < ct)
        {
            if(_online)
                break;
            os::ge_sleep(128);
        }
    }
    os::ge_sleep(2048);
    _closing = !_online;
    return _online;
}

/*------------------------------------------------------------------------------------------------
closes the connection, (effect -> thread ends) then reopens, (effect -> thread restart)
- returns trua in case of success, otherwise returns false
------------------------------------------------------------------------------------------------*/
BOOL  telnetvt::reopen()
{
    close();
    flush();
    return open();
}

/*------------------------------------------------------------------------------------------------
    - destroys the socket, notify the thread to end
------------------------------------------------------------------------------------------------*/
void  telnetvt::disable()
{
    _supress_errors = true;
    if(TestServer::__tr_mode & TRACE_DEBUG) 
            TestServer::__aprintf( S_DISABLE_TELNET, _pts->ASlot());
    _closing = TRUE;
    _flush_out();
    this->osthread::issuestop();
    _socket.destroy();
    _screen.clear();
    _supress_errors = false;
}

/*------------------------------------------------------------------------------------------------
destroys the socket, ends the receive/send thread
------------------------------------------------------------------------------------------------*/
void  telnetvt::close()
{
    _supress_errors = true;
    _closing = TRUE;
    _flush_out();
    if(TestServer::__tr_mode & TRACE_DEBUG) 
        TestServer::__aprintf( S_CLOSING_TELNET, _pts->ASlot());

    this->osthread::issuestop();
    _socket.destroy();
    this->osthread::stop();
    _online = false;
    _supress_errors = false;
}

/*------------------------------------------------------------------------------------------------
 set a parent thread id and parent uts server to notify calling OnData.
------------------------------------------------------------------------------------------------*/
void  telnetvt::setserver(TestServer* pts, DWORD tid)
{
    _pts    = pts;
    _pthrid = tid;
}

/*------------------------------------------------------------------------------------------------
 test if the caracter is readable. isdigit and isalpga crashes when the chat is somewhere above 150
------------------------------------------------------------------------------------------------*/
static bool _isreadable(unsigned char c)
{
    char alowable[]="~!@#$%^&*()_+-={[}}|:;\"'<,>.?/ \r\n\\";
    if(::strchr(alowable, c))
        return true;
    if(c<=20 && c != 9)
        return false;
    if(c>'z')
        return false;
    return true;
}

/*------------------------------------------------------------------------------------------------
thread function.
-async receive/send of queued data.
------------------------------------------------------------------------------------------------*/
void  telnetvt::run()
{
    if(TestServer::__tr_mode & TRACE_DEBUG) 
        TestServer::__aprintf( S_ENTERING_TELNET_THREAD, _pts->ASlot());

    int         local_bytes = 0;
    int         asciibytes;
    char        local[CIRC_SIZE+1];
    char        prelocal[CIRC_SIZE+1] = {0};
    int         similar = 0;
    timeval     tv   = {0, 50000};           // 50 milliseconds
    fd_set      rdset, wrset;
    int         bytes;
    bool        datareceived = false;
    astring     out;

   
    PlusMinus  plusminus(__telnet_sessions);
    

    TRACEX(_T("------------------> telnets = %d %s\r\n"), __telnet_sessions, _pts->TSlot());

    _online = false;
    int err = _socket.connect(_ip.c_str(), _port);
    _last_update = os::ge_gettick();
    
    try{
        if(NO_ERROR == err )
        {
            _socket.setblocking(1);             // set the block mode for FD_SET operations

            while(_socket.isopen())
            {
                _online = true;
                
                FD_ZERO(&rdset); FD_ZERO(&wrset);
                FD_SET(_socket.socket(), &rdset);
                FD_SET(_socket.socket(), &wrset);
                int ndfs = (int)_socket.socket() + 1;
                if(::select(ndfs, &rdset, &wrset, 0, &tv))
                {
                    if(FD_ISSET(_socket.socket(), &rdset))
                    {
                        assert(CIRC_SIZE - local_bytes > 0);
                        int bytes = _socket.receive(local, CIRC_SIZE - 1);
                        if(bytes <= 0) // connection closed
                        {
                            if(TestServer::__tr_mode & TRACE_WARNINGS && !_supress_errors)  
                                TestServer::__aprintf( S_TEL_UTS_RECEIVE_ERR2, _pts->ASlot(), _ip.c_str(), _socket.error());
                            _socket.destroy();
                            _online = false;
                            break;
                        }
                        if(bytes > 0)
                        {
                            local_bytes = bytes;
                            local[local_bytes] = 0;

                            if(TestServer::__tr_mode & TRACE_TERMINAL)
                            {
                                term_debug::record(local, local_bytes, _pts->ASlot());
                            }

                            _proc_options_data(local, local_bytes, asciibytes);
                            if(asciibytes)
                            {
                                if(!strcmp(prelocal, local))
                                {
                                    similar++; // start ignoring it cause is a blinking thing
                                }
                                else
                                {
                                    similar = 0;
                                    strcpy(prelocal, local); //cacheit
                                }

                                if(similar < 2 )
                                {
                                    _logbin(local, asciibytes);

                                    AutoLock sl(&_cs); 
                                    _screen += local;
                                    _last_update = os::ge_gettick();
                                    datareceived = true;
                                }
                                else
                                {
                                    __noop;
                                    /*
                                    if(TestServer::__tr_mode & TRACE_DEBUG) 
                                    {
                                        TestServer::__aprintf( S_TEL_REC_SAME_DATA, _pts->ASlot());
                                    }
                                    */

                                }
                            }
                        }
                    }
                    if(FD_ISSET(_socket.socket(), &wrset))
                    {
                        out.clear();
                        if(_inputs.try_pop(out))
                        {
                            int len = (int)out.length();
                            bytes = _socket.send(out.c_str(), len);
                            if(bytes != len)
                            {
                                _socket.destroy();
                                _online = false;
                                if(TestServer::__tr_mode & TRACE_WARNINGS && !_supress_errors) 
                                    TestServer::__aprintf( S_TEL_UTS_SEND_ERR2, _pts->ASlot(), _ip.c_str(), _socket.error());
                                break;
                            }
                            if(len && TestServer::__tr_mode & TRACE_DEBUG) 
                            {
                                if(out[0]==(char)-1)
                                    TestServer::__aprintf( S_SENDING_TO_UTS1, _pts->ASlot(), "-TO-");
                                else
                                    TestServer::__aprintf( S_SENDING_TO_UTS1, _pts->ASlot(), out.c_str());
                            }
                        }
                    }
                    if(true == datareceived && !_closing)
                    {
                        datareceived = false;
                        if(_notify)
                            _pts->OnTelnetDataReceived(::strlen(local));
                    }
                }
                os::ge_sleep(32);    
            } //send rec
        }
        else
        {
            if(TestServer::__tr_mode & TRACE_WARNINGS) 
                TestServer::__aprintf( S_FAILED_TO_CONNECT2, _pts->ASlot(), _ip.c_str(), _socket.error());
        }
    }catch(int& k)
    {
        if(TestServer::__tr_mode & TRACE_WARNINGS) 
            TestServer::__aprintf( S_CLOSED_BY_US, _pts->ASlot(), k);

    }
    _socket.destroy();
    _online = FALSE;
    TRACEX(_T("<------------------ telnets = %d %s\r\n"), __telnet_sessions, _pts->TSlot());
    this->osthread::leave();
    if(TestServer::__tr_mode & TRACE_DEBUG) 
        TestServer::__aprintf( S_LEAVING_TEL_THREAD, _pts->ASlot());
}

/*------------------------------------------------------------------------------------------------
-redirects to put(byte length)
------------------------------------------------------------------------------------------------*/
void telnetvt::put(const char* pb)
{
    _TEST_ONLINE();
    if(_closing)
        return;
    
    skctrlmap *ptop = __sctrlmapping;
    while(ptop->smap[0])
    {
        if(!strcmp(ptop->ssek, pb))
        {
            put((BYTE*)ptop->smap, ptop->length);
            return;
        }
        ++ptop;
    }
    put((BYTE*)pb, strlen(pb));
}

/*------------------------------------------------------------------------------------------------
-redirects to put(byte length)
------------------------------------------------------------------------------------------------*/
void telnetvt::put_one(const char* pb)
{
    _TEST_ONLINE();
    if(_closing)
        return;
    
    skctrlmap *ptop = __sctrlmapping;
    while(ptop->smap[0])
    {
        if(!strcmp(ptop->ssek, pb))
        {
            for(int c=0;c<ptop->length;c++)
            {
                put((BYTE*)&ptop->smap[c], 1);
                os::ge_sleep(64);
            }
            return;
        }
        ++ptop;
    }
    for(size_t c=0;c<strlen(pb);c++)
    {
        put((BYTE*)&pb[c], 1);
        os::ge_sleep(64);
    }
}

/*------------------------------------------------------------------------------------------------
-redirects to put(byte length)
------------------------------------------------------------------------------------------------*/
void telnetvt::put(const astring& pb)
{
    if(_closing)    
        return;
    _TEST_ONLINE();
    put((BYTE*)pb.c_str(), pb.length());
    os::ge_sleep(128);
    _last_update = os::ge_gettick();
}
/*------------------------------------------------------------------------------------------------
-locks the input queue andplaces the data to be sent to telnet session
------------------------------------------------------------------------------------------------*/
void telnetvt::put(BYTE* pb, int length)
{
    if(_closing)    
        return;
    _TEST_ONLINE();

    astring loco;
    while(length--)
    {
        loco += (char)(*pb);
        ++pb;
    }
    _inputs.push(loco);
    _last_update = os::ge_gettick();
}

/*------------------------------------------------------------------------------------------------
- wait maximum tout millisecoinds to find in the received data the 'substr'
- 'substr' is the very last recveived string.
------------------------------------------------------------------------------------------------*/
bool telnetvt::wait(const char* substr, int tout)
{
    _TEST_ONLINE();
    unsigned long ct = os::ge_gettick() + tout;
    while(os::ge_gettick() <= ct || tout==0)        // tout==0 allow for test only
    {
        if(_test(substr))
        {
            return true;
        }
        _TEST_ONLINE();
        if(tout==0)
            break;
        os::ge_sleep(256);
    }
    return false;
}

bool  telnetvt::wait_for_some(DWORD tout)
{
    _TEST_ONLINE();
    unsigned long ct = os::ge_gettick() + tout;
    while(os::ge_gettick() <= ct || tout==0)        // tout==0 allow for test only
    {
        if(!_screen.empty())
            return true;
        os::ge_sleep(256);
    }
    return false;
}

/*------------------------------------------------------------------------------------------------
-saves _screen buffer into the file
------------------------------------------------------------------------------------------------*/
void  telnetvt::save_screen(FILE* pf)
{
    fwrite(_screen.c_str(), _screen.length(), 1, pf);
}

/*------------------------------------------------------------------------------------------------
-waits tout, then returns the reeived string.
-clear the buffered internal received data
------------------------------------------------------------------------------------------------*/
const char* telnetvt::get(astring& out, int tout)
{
    _TEST_ONLINE();
    unsigned long ct = os::ge_gettick() + tout;
    while(os::ge_gettick() < ct)
    {
        os::ge_sleep(256);
		_TEST_ONLINE();
    }

    AutoLock    sl(&_cs); 
    out = _screen;
    return out.c_str();
}

/*------------------------------------------------------------------------------------------------
-cler all input / output buffers.
------------------------------------------------------------------------------------------------*/
void telnetvt::flush()
{
    _inputs.clear();
    AutoLock    sl(&_cs); 
    _screen.clear();
}

/*------------------------------------------------------------------------------------------------
-cler all input / output buffers.
------------------------------------------------------------------------------------------------*/
void telnetvt::flush_over()
{
    _inputs.clear();
    AutoLock    sl(&_cs); 
    _screen.clear();
    /*
    if(_screen.length() > 32768)
    {
        astring  screen = _screen.substr(_screen.length()-8912, 8912);
        _screen =  screen;
    }
    */

}

/*------------------------------------------------------------------------------------------------
- tests if the 'substr' matches the last received
- returns true if the match is found, otherwise returns false
------------------------------------------------------------------------------------------------*/
int telnetvt::_test(const char* sequence)
{
    _TEST_ONLINE();

    astring     tof   = sequence;
    astring     chunk;
    size_t      pstart = 0;
    size_t      csep  = tof.find('&');
    
    // search all in the secreen
    if(csep != astring::npos)  // more than one string
    {
        AutoLock    sl(&_cs); 
        int         loops   = 0;

        while(1)
        {
            csep = tof.find_first_of('&', pstart);
            ++loops;
            chunk =  tof.substr(pstart, csep-pstart);

            if(_screen.find(chunk) != astring::npos)
                --loops;
            pstart = csep + 1;
            if(csep==astring::npos)
                break;

        }
        return (0 == loops) ? 1 : 0;  //all found
        
    }

    csep   = tof.find('|');
    // search any of in the screen
    if(csep != astring::npos)  // more than one string
    {
        AutoLock    sl(&_cs); 
        while(1)
        {
            csep = tof.find_first_of('|', pstart);
            chunk =  tof.substr(pstart, csep-pstart);
            if(_screen.find(chunk) != astring::npos)
                return 1;
            pstart = csep + 1;
            if(csep==astring::npos)
                break;
        }
        return 0;
    }

    AutoLock    sl(&_cs); 
    if(_screen.find(tof) != astring::npos)
    {
        return 1;
    }
    return 0;
}

/*------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------*/
const char* telnetvt::xget(const astring& from, astring& out)
{
    _TEST_ONLINE();
    AutoLock    sl(&_cs); 

    size_t nfind = _screen.find(from);
    if(nfind == astring::npos)
        return "*";
    size_t endpos = nfind;
    while(endpos < _screen.length() && _screen.at(endpos) != '\n')
        endpos++;
    out = _screen.substr(nfind, endpos-nfind);
    return out.c_str();
}

/*------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------*/
void telnetvt::_flush_out()
{
    DWORD tmax = os::ge_gettick() + 2000;
    while(_inputs.size() && os::ge_gettick() < tmax)
    {
        os::ge_sleep(256);
    }
}

/*------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------*/
bool  telnetvt::steady(long tout, long maximt)
{
    if(maximt < tout)
        maximt = 180000;

    _last_update = os::ge_gettick();
    DWORD ct =  _last_update + maximt;
    while(os::ge_gettick() < ct)    // no more than maximt
    {
        if(os::ge_gettick() - _last_update > (DWORD)tout)
        {
            return true; // no activity for last tout milliseconds
        }
        os::ge_sleep(256);
        _TEST_ONLINE();
    }
    return false;
}

/*------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------*/
void  telnetvt::show_screen(bool msgbox, const char* caption)
{
    if(msgbox)
    {
        ::MessageBoxA(GetActiveWindow(), _screen.c_str(), caption, MB_OK|MB_ICONINFORMATION);
    }
    else
    {
        TestServer::__aprintf("\r\n----------------------%s---------------------\r\n", caption);
        TestServer::__aprintf(_screen.c_str());
        TestServer::__aprintf("\r\n---------------------------------------------------------\r\n");
    }
}

/*------------------------------------------------------------------------------------------------
-delas with thelnet options
- estracts all telnet bytes from incomming stream, wipes them out from incomming stream, 
- process the options
------------------------------------------------------------------------------------------------*/
void telnetvt::_proc_options_data(char* buff, int& length, int& asciibytes)
{
    int     skip;
    char    *psrc = buff, *pdst = buff;

    asciibytes = 0;
    while(length > 0)
    {
        unsigned char  c  = (unsigned char)(psrc[0]); 
        skip     = 0;

        if(c == TC_IAC ) //telnet options + terminal options
        {
            skip = _process_tel_option(psrc, length);
        }
        else if(c == ANSI_ESC_START)
        {
            skip = _process_ansi_option(psrc, length);
        }
        else if(c < 0x20)
        {
            if(c != '\r' && c!= '\n')
                skip = 1;
            if(c==0x0E && length>1)
                skip = 2;
        }
        else if(!_isreadable(c))
        {
            skip = 1;
        }

        if(skip)
        {
            if(skip == -1)
                break;
            psrc    += skip;
            length  -= skip;
        }
        else
        {
            *pdst++ = *psrc++;
            asciibytes++;
            length--;
        }
    }
    if(length==-1)
        length=0;
    *pdst++ = 0;

}


/*------------------------------------------------------------------------------------------------
- helper foo to telnet options
------------------------------------------------------------------------------------------------*/
int telnetvt::_sendoptresp(char* pStr, telopt_struct* po)
{
    int tos = po->inLen;
    if(po->inPatern[2] == TC_IAC)
    {
        po->outPatern[2] = pStr[2];
        tos++;
    }
    if(po->outLen)
    {
        put(po->outPatern, po->outLen);
    }
    return tos;
}

/*------------------------------------------------------------------------------------------------
- helper foo to telnet options
------------------------------------------------------------------------------------------------*/
int   telnetvt::_process_tel_option(char* pStr, int length)
{
    telopt_struct*  po = __telOpttions;

    while(po->inLen)
    {
        if(length >= po->inLen &&  !memcmp(po->inPatern, pStr, po->inLen))
        {
            return _sendoptresp(pStr, po);
        }
        po++;
    }
    return 0;
}

/*------------------------------------------------------------------------------------------------
- helper foo to telnet options
------------------------------------------------------------------------------------------------*/
int   telnetvt::_process_ansi_option(char* pStr, int length)
{
    int skip          = 0;
    telopt_struct* po = __ansiOpttions;

    //
    // see if we vave someting ansi console option
    //
    while(po->inLen)
    {
        if((length >= po->inLen) && !memcmp(po->inPatern, pStr, po->inLen))
        {
            skip += _sendoptresp(pStr, po);
            break;
        }
        po++;
    }
    if(skip)
        return skip;

    //
    // skip all esc up to m, 0 or another esc
    //
    unsigned char  c  = (unsigned char)(pStr[0]); 

    do{
        c  = (unsigned char)(pStr[0]); 
        --length;
        ++skip;
        *pStr++;

    }while(length && c != ANSI_ESC_END && c != ' ' );
    return skip; 
}


/*------------------------------------------------------------------------------------------------
- logs binary data in and out    
------------------------------------------------------------------------------------------------*/
void    telnetvt::_logbin(const char* info, int len, bool ascii)
{
    if(!(TestServer::__tr_mode & TRACE_DEBUG)) 
        return;

    astring      sl; 
    astring      sr; 
    char        bl[32];
    char        br[32];
    char        c[8] = {0};

    if(ascii)
    {
        TestServer::__aprintf(S_DATA, _pts->ASlot(), info);
        return;
    }

    for(int i=0;i < len;i++)
    {
        c[0] = (char)info[i];
        sprintf(bl,"%02X ",(BYTE)c[0]);

        if((c[0] >='0' && c[0] <= 'z') || c[0]==' ')
        {
            sprintf(br, "%c", c[0]);
        }
        else
        {
            sprintf(br,"%c",'.');
        }
        sl+=bl;
        sr+=br;
        if(sr.length()==16)
        {
            astring s2 = sl;
            s2 += "        ";
            s2 += sr;
            s2 += "\r\n";
            TestServer::__aprintf( S_DATABIN , _pts->ASlot(), s2.c_str());
            sl.clear();
            sr.clear();
        }
    }
    if(!sr.empty())
    {
        while(sr.length()<16)
        {
            sr+=".";
            sl+=".. ";
        }
        astring s2 = sl;
        s2 += "        ";
        s2 += sr;
        s2 += "\r\n";
        TestServer::__aprintf( "%s",s2.c_str());
        sl.clear();
        sr.clear();
    }
}


//=======================================================================================
void term_debug::pool()
{
    int             ndfs ;
    char            local[1204];
    char            fname[256];
    timeval         tv = {0, 1000};
    DWORD           tsend = os::ge_gettick();
    bool            paused = false;

    if(!_socket.isopen())
    {
        if(!_srv.isopen())
        {
            if(0==_srv.create(23001))
            {
                _srv.setblocking(1);
                if(-1==_srv.listen())
                {
                    _srv.destroy();
                    return ;
                }
            }
        }
        FD_ZERO(&_wrset);FD_ZERO(&_rdset);
        FD_SET(_srv.socket(), &_wrset);
        FD_SET(_srv.socket(), &_rdset);
        ndfs = (int)_srv.socket() + 1;
        //fd_set          _rdset;
        if(::select(ndfs, &_rdset, &_wrset, 0, &tv))
        {
            if(FD_ISSET(_srv.socket(), &_wrset))
            {
                _srv.accept(_socket);
            }
            
            if(FD_ISSET(_srv.socket(), &_rdset))
            {
                _srv.accept(_socket);
            }
        }
        char ok[256];
        FILE* pf;
        while(_socket.isopen())
        {
            strcpy(ok, "slot-###: to debug slot ###\r\n-'`'      : to exit\r\n'h'     : to hold frame\r\n'c'     : to continue\r\n");
            _socket.send(ok, strlen(ok)); //discard

            memset(fname,0, sizeof(fname));

            while(1)
            {
                char chunk[128];
                int rb = _socket.receive(chunk, 127); //discard
                if(rb <= 0)
                {
                    break;
                }
                chunk[rb] = 0;
                strcat(fname, chunk);
                char* pcrlf = strchr(fname,'\r');
                if(pcrlf)
                {
                    *pcrlf = 0;
                    break;
                }
                if(!running())
                    throw 2;

            }
            if(strlen(fname) < 4)
            {
                break;
            }
            BYTE buff[4096];

            sprintf((char*)buff,"\r\n[waiting for %s \r\n", fname);
            _socket.send((char*)buff, strlen((char*)buff));

            bool  sentrend=false;
            size_t file_offset = 0;
            _socket.setblocking(1);             // set the block mode for FD_SET operations
            long bytes;
             
            try{
                while(_socket.isopen())
                {
                    if(!running())
                        throw 2;
                    if(os::ge_gettick() > tsend && !paused)
                    {
                        AutoLock al(&_cs);
                        if(_reset_file_ptr)
                        {

                            file_offset = 0;
                            _reset_file_ptr=false;
                            
                            strcpy((char*)buff, "\r\n *****[playing from begining due a new action or new recorded session] *****\r\n");
                            bytes = strlen((char*)buff);

                            if(_socket.send(buff, bytes) != bytes)
                            {
                                throw(1);
                            }
                        }
                        if(pf = fopen(MKSTR("testmode/%s.utb",fname),"rb"))
                        {
                            fseek(pf, 0, SEEK_END);
                            size_t   fl = ftell(pf);

                            if(file_offset < fl)
                            {
                                fseek(pf, file_offset, SEEK_SET);
                                                                
                                if(!feof(pf))
                                {
                                    fread(&bytes, sizeof(long), 1, pf);
                                    fread(buff, 1, bytes, pf);

                                    if(_socket.send(buff, bytes) != bytes)
                                    {
                                        throw(1);
                                    }
                                    file_offset = ftell(pf);
                                }
                                sentrend=true;
                            }
                            fclose(pf);
                            pf = 0;
                        }
                        
                        tsend = os::ge_gettick() + 512;
                        os::ge_sleep(256);
                    }
                    FD_ZERO(&_rdset);
                    FD_SET(_socket.socket(), &_rdset);
                    ndfs = (int)_socket.socket() + 1;
                    if(::select(ndfs, &_rdset, 0, 0, &tv))
                    {
                        if(FD_ISSET(_socket.socket(), &_rdset))
                        {
                            if(_socket.receive(local, 1024) > 0)
                            {
                                if(local[0]=='h')
                                {
                                    paused = true;
                                }
                                else if(local[0]=='c')
                                {
                                    paused = false;
                                }
                                else if(local[0]=='`' || local[0]=='~')
                                {
                                    throw(1);
                                }
                            }
                            else
                                throw(1);
                        }
                    }
                    os::ge_sleep(10);
                }

            }catch(int i)
            {
                i;
            }
            strcpy(ok, "\r\n--------------------clossing connection------------------\r\n");
            _socket.send(ok, strlen(ok)); //discard
            _socket.destroy();
            if(pf) fclose(pf);
        
            else
            {
                sprintf(ok, "such a file ' testmode/%s.utb' does not exist\r\n", local);
            }
            os::ge_sleep(256);
        }
        _socket.destroy();
    }
}

//=======================================================================================
void   term_debug::record(const char* local, int local_bytes, const char* slot)
{
    if(TestServer::__ptd)
    {
        AutoLock al(&TestServer::__ptd->_cs);
        FILE* pf = fopen(MKSTR("testmode/%s.utb", slot),"ab");
        if(pf)
        {
            long sizee = local_bytes;
            fwrite(&sizee, 1, sizeof(long), pf);
            fwrite(local, 1, local_bytes, pf);
            fclose(pf);
        }
        return; 
    }
    FILE* pf = fopen(MKSTR("testmode/%s.utb",slot),"ab");
    if(pf)
    {
        long sizee = local_bytes;
        fwrite(&sizee, 1, sizeof(long), pf);
        fwrite(local, 1, local_bytes, pf);
        fclose(pf);
    }
    
}

//=======================================================================================
void   term_debug::reset_file_ptr()
{
    AutoLock al(&_cs);
    _reset_file_ptr = true;
}
