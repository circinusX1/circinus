/*
    Copyright FLEX COMPUTING / FLEXTRONCS 
    Author Marius Chincisan Aug 2008
    OsConsole Interface Implementation. Handles console input and output pipes
*/
#ifndef __SOCK_H__
#define __SOCK_H__

#include "os.h"

#pragma warning (disable: 4267)
#pragma warning (disable: 4244)




#ifndef _WIN32
    typedef int SOCKET;
#else
    typedef int socklen_t;
#endif //


#define MAKE_IP(a_,b_,c_,d_) (DWORD)( (a_ <<24) | (b_<<16) | (c_<<8) | d_)


//---------------------------------------------------------------------------------------
typedef BOOL (*CancelCB)(void* pVoid, DWORD time);

#ifdef WIN32
    #define EINPROGRESS         WSAEINPROGRESS
    #define WOULDBLOCK          WSAEWOULDBLOCK
#else
    #define WOULDBLOCK          EINPROGRESS
#endif //

//---------------------------------------------------------------------------------------
// ver 1.5 for broadcasting UDP packets
struct SMachineName
{
    char    sMachine[32];
};

//---------------------------------------------------------------------------------------
class sock
{
public:

    static      BOOL DefCBCall(void*,DWORD);
    static void Init();
    static void Uninit();
    static char*  GetLocalIP();
    static int  GetHostIp(const char* hostname, char* szip, DWORD* dwip=0);
    static BOOL CTime(void* pT, DWORD time);

    sock();
    virtual ~sock();
    virtual SOCKET  create(int opt=0);
    virtual void    destroy();
    virtual int     send(const char* buff, const int length, int port=0, const char* ip=0  )=0;
    virtual int     send(const char* buff, const int length, const struct sockaddr_in& rsin)=0;
    virtual int     send(const BYTE* buff, const int length, int port=0, const char* ip=0  )=0;
    virtual int     send(const BYTE* buff, const int length, const struct sockaddr_in& rsin)=0;
    virtual int     receive(BYTE* buff, int length, int port=0, const char* ip=0  )=0;
    virtual int     receive(BYTE* buff, int length, struct sockaddr_in& rsin)=0;
    virtual int     select_receive(BYTE* buff, int length, struct sockaddr_in& rsin, int toutms);
    virtual int     receive(char* buff, int length, int port=0, const char* ip=0  )=0;
    virtual int     receive(char* buff, int length, struct sockaddr_in& rsin)=0;
    virtual int     select_receive(char* buff, int length, struct sockaddr_in& rsin, int toutms);

    
    void      detach(){s_socket=-1;}
    int       setblocking(DWORD block);
    int       setoption(int option, int optionsize);
    int       getoption(int option);
    const SOCKET&   socket()const {return s_socket;}
    const bool      isopen()const{return (int)s_socket!=-1;}
    const int       error()const{return _error;}
    BOOL      IsBlocking(){return _blocking;}

protected:
    DWORD           _flags;
    SOCKET          s_socket;
    int       _error;
    BOOL      _bClose;
    static  DWORD   _tout;
    int       _blocking;
    
};

//---------------------------------------------------------------------------------------
class tcp_srv_sock;
class tcp_sock : public sock
{
public:
    friend class tcp_srv_sock;
    tcp_sock();
    virtual ~tcp_sock(){};
    virtual SOCKET  create(int opt=0);
    virtual int     send(const char* buff, const int length, int port=0, const char* ip=0  );
    virtual int     send(const char* buff, const int length, const struct sockaddr_in& rsin){return send(buff, length);};
    virtual int     send(const BYTE* buff, const int length, int port=0, const char* ip=0  );
    virtual int     send(const BYTE* buff, const int length, const struct sockaddr_in& rsin){return send(buff, length);}
    virtual int     receive(BYTE* buff, int length, int port=0, const char* ip=0  );
    virtual int     receive(BYTE* buff, int length, struct sockaddr_in& rsin){return receive(buff,length);}
    virtual int     receive(char* buff, int length, int port=0, const char* ip=0  );
    virtual int     receive(char* buff, int length, struct sockaddr_in& rsin){return receive(buff,length);}

    char*           AddrIP();
    char*           getsocketaddr(char* pAddr);
    int       listen(int maxpending=16);
    struct          sockaddr_in& Rsin(){return _remote_sin;}
    struct          sockaddr_in& Lsin(){return _local_sin;}
protected:
    struct sockaddr_in	_local_sin;	   // source
	struct sockaddr_in	_remote_sin;          // dest
    char          _sip[128];
};


//---------------------------------------------------------------------------------------
class tcp_cli_sock;
class tcp_srv_sock : public tcp_sock
{
    int _port;
public:
    tcp_srv_sock();
    virtual ~tcp_srv_sock(){};
    SOCKET     accept(tcp_cli_sock& cliSock);
    virtual SOCKET   create(int opt=0);
};

//---------------------------------------------------------------------------------------
class tcp_cli_sock : public tcp_sock
{
public:
    explicit tcp_cli_sock(){_hostent=0;_connecting=0;}
    explicit tcp_cli_sock(const tcp_cli_sock& s):tcp_sock(){
        s_socket       = s.s_socket;
        _error      = s._error;
        _bClose     = s._bClose;
        _blocking   = s._blocking;
        ::memcpy(&_local_sin, &s._local_sin, sizeof(_local_sin));
        ::memcpy(&_remote_sin, &s._remote_sin, sizeof(_remote_sin));
    }
    tcp_cli_sock& operator =   (const tcp_cli_sock& s){
        if(this != &s){
      s_socket       = s.s_socket;
      _error      = s._error;
      _bClose     = s._bClose;
      _blocking   = s._blocking;
      ::memcpy(&_local_sin, &s._local_sin, sizeof(_local_sin));
      ::memcpy(&_remote_sin, &s._remote_sin, sizeof(_remote_sin));
        }
        return *this;
    }
    virtual ~tcp_cli_sock(){};
    virtual SOCKET  create(int opt=0);
	int       try_connect(const char* sip, int port);
    int       raw_connect(DWORD uip4, int port);
    int       raw_connect(const char* suip4, int port);
    int       openconnection(const char* sip, int port);
    int       connect(const char* sip, int port, CancelCB cbCall=sock::DefCBCall, void* pUser=0);
    int       i4connect(DWORD ip, int port, CancelCB cbCall=sock::DefCBCall, void* pUser=0);
    int       s4connect(const char* sip, int port, CancelCB cbCall=sock::DefCBCall, void* pUser=0);
    DWORD   getremoteip(){return (DWORD)_remote_sin.sin_addr.s_addr;}
    struct hostent* gethostent(){return _hostent;}
    int       isconnecting(){
        return _connecting;
    }
    void      setconnected(){
        assert(s_socket!=-1); 
        _connecting = 0;
    }

    void      destroy();
protected:
    struct hostent  *_hostent;
    int       _connecting;
};

//---------------------------------------------------------------------------------------
class udp_sock : public sock
{
public:
    udp_sock():_connected(0){}
    virtual ~udp_sock(){destroy();}
    virtual void    destroy(){sock::destroy();_connected=0;};
    virtual SOCKET  create(int opt=0);
    virtual int     send(const char* buff, const int length, int port=0, const char* ip=0  );
    virtual int     send(const char* buff, const int length, const struct sockaddr_in& rsin);
    virtual int     send(const BYTE* buff, const int length, int port=0, const char* ip=0  );
    virtual int     send(const BYTE* buff, const int length, const struct sockaddr_in& rsin);
    virtual int     receive(BYTE* buff, int length, int port=0, const char* ip=0  );
    virtual int     receive(BYTE* buff, int length, struct sockaddr_in& rsin);
    virtual int     receive(char* buff, int length, int port=0, const char* ip=0  );
    virtual int     receive(char* buff, int length, struct sockaddr_in& rsin);

    struct sockaddr_in& Rsin(){return _remote_sin;}
    struct sockaddr_in& Lsin(){return _local_sin;}
    void      SetRsin(const struct sockaddr_in& in){::memcpy(&_remote_sin, &in, sizeof(struct sockaddr_in));}
    int       connect(const char* sip, int port, CancelCB cbCall=sock::DefCBCall, void* pUser=0);
    
protected:
    BOOL          _connected;
    struct sockaddr_in	_local_sin;
    struct sockaddr_in	_remote_sin;          // dest
    char          _sip[128];
};


//---------------------------------------------------------------------------------------
class udp_server_sock : public udp_sock
{
public:
    udp_server_sock(){_sip[0]=0;}
    virtual ~udp_server_sock(){};
    virtual SOCKET  create(int opt=0);
    sockaddr_in&    remote(){return _remote_sin;}
    void      remote(sockaddr_in& s){memcpy(&_remote_sin,&s,sizeof(s));}

    char*           AddrIP();
};

//---------------------------------------------------------------------------------------
class udp_client_sock : public udp_sock
{
public:
    udp_client_sock(){}
    virtual ~udp_client_sock(){}
    virtual SOCKET   create(int opt=0);

};


//---------------------------------------------------------------------------------------
class udp_group_sock : public udp_sock
{
/*
    struct ip_mreq    _mcastGrp;
    BOOL         _groupmember;

public:
    udp_group_sock(int opt=0){};
    virtual ~udp_group_sock(){destroy();};
    virtual int  create(int opt=0);
    int     join(const char* ipGrp, int port);

    virtual int     send(BYTE* buff, int length, int port=0, const char* ip=0  );
    virtual int     receive(BYTE* buff, int length, int port=0, const char* ip=0  );
    virtual void    destroy();
    */
};

#define IS_SOCKK_ERR(err_)  err_ == WSAECONNRESET   ||\
                err_ == WSAECONNABORTED ||\
                err_ == WSAESHUTDOWN    ||\
                err_ == WSAETIMEDOUT    ||\
                err_ == WSAECONNREFUSED ||\
                err_ == WSAEOPNOTSUPP   ||\
                err_ == WSAENETDOWN     ||\
                err_ == -1




class WsaInit
{
public:
    WsaInit()
    {
        sock::Init();
    }
    ~WsaInit()
    {
        sock::Uninit();
    }
};


class IP2String
{
public:
   IP2String(DWORD dw){
       ::t_sprintf( _s,"%u.%u.%u.%u",
                (int)(dw  & 0xFF),
                (int)((dw >>8)& 0xFF),
                (int)((dw >>16) & 0xFF),
                (int)((dw >>24)&0xFF));
   }
   operator const char*(){return _s;}
private:
   static char _s[128];
};
#define IP2STR(dwip_)   (const char*)IP2String(dwip_)

#endif // !
