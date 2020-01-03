/*
Copyright (c) 2014-2016 comarius.DPT All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/comarius. The name of the
https://github.com/comarius may not be used to endorse or promote
products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef REST_SRV
#define REST_SRV

#include <unistd.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <mutex>
#include <condition_variable>
#include <sqrat.h>
#include <sqwrap.h>
#include "osthread.h"
#include "sock.h"
#include "divais.h"
#include "../modules/iper.h"
#include "rapidjson/document.h"

#define MAX_QUEUE   65535

class Tcli : public tcp_clis
{
public:
    Tcli(){}
    bool term(bool be=true)
    {
        _req.clear();
        tcp_clis::term();
        return true;
    }
    std::string _req;
    size_t      _dbage = 0;
};

class RestSrv : public Mpthrd
{
public:
    RestSrv(int port, const char* name);
    ~RestSrv();
    int   reply(const char* str);
    int   ctx_it(const char* dev_key);
    void  close();
    int bus()const;
    const char* treq();
    const char* getJson(const char* key, bool fetchnow=false);
    int spinit();
    int port()const{return _port;}
    int addField(const char* name, SqMemb f);
    static void squit(SqEnvi& e){
        Sqrat::Class<RestSrv> cls(e.theVM(), _SC("SRV"));
        cls.Ctor<int, const char*>();
        cls.Functor(_SC("treq"), &RestSrv::treq);
        cls.Functor(_SC("reply"), &RestSrv::reply);
        cls.Functor(_SC("ctx_it"), &RestSrv::ctx_it);
        Sqrat::RootTable().Bind(_SC("SRV"), cls);
    }

public:
    pthread_t    ioTid;
    virtual void sig_it();

protected:
    virtual void thread_main();
    virtual int  start_thread();
    virtual void stop_it();
    void _hthdr(std::string& resp);
    void _start_hdr(Tcli* cli);
    int  _defer_query(const std::string& path, const std::string& req);
    int  _defer_to_device(I_IDev* pdev,  const rapidjson::Document& jnod);
    bool _build_forjson(const devsmap_t& refrdevs, bool);
    int  _process_forjson_post(const std::string& path, const std::string& req);
    int  _process_resful_request(const std::string& path, const std::string& req);
    bool _get_add_devices(const std::string&, devsmap_t& devs);
    int _get_from_query(const std::string&, devsmap_t& devs);
    void _get_dev_forjson(I_IDev* pd, rapidjson::Document&  doc);
private:
    bool _toast(Tcli*);
    bool _main_process(Tcli*);
    void _tcp();
    bool _listen();
    void _spin();
    void _clear();

private:
    std::vector<Tcli*> _kons;
    socklen_t           al;
    int                 sfd;
    struct sockaddr_un  cpp_addr;
    std::string         _req;
    std::string         _outstr;
    std::string         _cached_reequest2;
    int                 _port=0;
    tcp_srv_sock        _s;
    bool                _fatal=false;
    bool                _locked=false;
    bool                _blocksync=false;
    std::mutex*         _m;
    bool                _mon_dirt=false;
    std::condition_variable* _muxcon;
};

extern RestSrv* Prest;
#endif // REST_SRV
