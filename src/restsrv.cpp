/*
Copyright (c) 2014-2016 Marius.C All rights reserved.

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

#include "restsrv.h"
#include "main.h"
#include "apis.h"
#include "divais.h"
#include "dcore.h"
#include "sqratThread.h"

RestSrv* Prest;

RestSrv::RestSrv(int port, const char* tcpp):_tcpx(tcpp),_port(port)
{
    ioTid = pthread_self();
    this->start_thread();
    Prest = this;
}

void RestSrv::thread_main()
{
    LOGI("IO THREAD " << MainThrId);
    ioTid = pthread_self();

    if(_listen()==false)
    {
        LOGEX("cannot listen on tcp port " << _port);
        LOGD1("Thread IO exits");
        return;
    }

    while(!this->is_stopped() && ApStat==RUNNING)
    {
        spinit();
        ::msleep(16);
    }
    LOGD1("Thread IO exits");
}

int RestSrv::ctx_it(const char* dev_key)
{
    std::ofstream expFile;
    try{
        expFile.open( "/tmp/s2wsocket", std::ios::out);
        if(expFile.is_open())
        {
            expFile << _port;
            expFile.close();
        }
    }
    catch (std::exception& e)
    {
        LOGEX(__PRETTY_FUNCTION__ <<" std exception "<< e.what());
    }
    if(!strcmp(dev_key,"thread"))
    {
        return this->start_thread();
    }
    else
    {
        if(_listen())
        {
            return 0;
        }
    }
    LOGEX("cannot listen on tcp port " << _port);
    return -1;
}

int RestSrv::reply(const char* reply)
{
    _outstr = reply;
    __bsqenv->notify();
    return 0;
}

bool  RestSrv::_listen()
{
    int ntry = 0;
AGAIN:
    if(_s.create(_port, SO_REUSEADDR, 0)>0)
    {
        fcntl(_s.socket(), F_SETFD, FD_CLOEXEC);
        _s.set_blocking(1);
        if(_s.listen(1)!=0)
        {
            LOGW("listen "<< (ntry+1) );
            _s.term();
            sleep(1);
            if(++ntry<10)
                goto AGAIN;
            return false;
        }
        LOGI("tcp listen: "<<_port);
        return true;
    }
    LOGW("create "<< (ntry+1));
    _s.term();
    sleep(1);if(++ntry<10) goto AGAIN; sleep(1);
    return false;
}

void RestSrv::_start_hdr(Tcli* cli)
{
    _hthdr(_outstr);

    //todo json
    cli->sendall(_outstr.c_str(), _outstr.length());
}

void RestSrv::_hthdr(std::string& resp)
{
    resp  = "HTTP/1.1 200 OK\r\n";
    resp += "Cache-Control : no-cache, private\r\n";
    resp += "Content-Length :" + std::to_string(_outstr.length());
    resp +=  "\r\n";
    resp += "Content-Type: application/json\r\n\r\n";
}

bool RestSrv::_toast(Tcli* cli)
{
    std::string resp;
    std::string path;
    size_t preq;
    size_t endr;
    size_t post = cli->_req.find("POST");
    size_t get = cli->_req.find("GET");
    if(post==0){
        size_t paths = cli->_req.find("POST ") + 5;  // read the treq string
        size_t pathe = cli->_req.find("HTTP") - 1;  //
        preq = cli->_req.find("\r\n\r\n");
        endr = cli->_req.length();
        path = cli->_req.substr(paths, pathe-paths);
        if(preq != std::string::npos)
            preq += 4;
    }
    if(get==0){
        preq = cli->_req.find("GET ") + 4;  // read the treq string
        endr = cli->_req.find("HTTP") - 1;  //
    }
    if(preq != std::string::npos && endr != std::string::npos)
    {
        _req = cli->_req.substr(preq, endr-preq);
        LOGD3("->" << _req);
        _cacheData = _req;
        //
        // defer to main thread any treq that has a !  or a  ?
        //
        if(!_req.empty())
        {
            _defer_query(path, _req); // iorequest from this thread
        }
        _req.clear();
        if(_outstr.empty())
            _outstr="{}";
        // make the HTTP response header
        _hthdr(resp);
        resp += _outstr;
        _outstr.clear();
        cli->sendall(resp.c_str(), resp.length());
        LOGD3("json:<- [" << resp << "]");
        cli->term();
        _mon_dirt = true;

    }
    else
    {
        if(!::strncmp(cli->_req.c_str(),"GET",3) ||
                !::strncmp(cli->_req.c_str(),"Hos",3))
        {
            cli->term();
            _mon_dirt = true;
        }
        _req = cli->_req;
        _defer_query(path,_req);
        if(_outstr.empty())
            _outstr="{}";
        cli->sendall(_outstr.c_str(), _outstr.length());
    }
    _req.clear();
    return true;
}

int RestSrv::spinit()
{
    _spin();
    return true;
}

void RestSrv::_clear()
{
    if(_mon_dirt)
    {
AGAIN:
        for(std::vector<Tcli*>::iterator s=_sons.begin();s!=_sons.end();++s)
        {
            if(_fatal){
                (*s)->term();
            }
            if((*s)->socket()<=0)
            {
                //LOGD3("Client:" << (*s)->getsocketaddr_str(addr) << " closed connection");
                delete (*s);
                _sons.erase(s);
                goto AGAIN;
            }
        }
    }
    _mon_dirt=false;
}

const char* RestSrv::treq()
{
    size_t andpos = _req.find_first_of('&');
    if(andpos != std::string::npos)
        _cached_reequest2 = _req.substr(andpos+1);
    else
        _cached_reequest2 = _req;
    return _cached_reequest2.c_str();
}

int RestSrv::bus()const
{
    return -1;
}

int  RestSrv::start_thread()
{
    return Mpthrd::start_thread();
}

void RestSrv::release_io(bool force)
{
}

void RestSrv::sig_it()
{
    _s.term();
    for(auto& s : _sons){
        LOGW("client gone");
        s->term();
        delete s;
    }
    _sons.clear();
    Mpthrd::sig_it();
}

void RestSrv::stop_it()
{
    Mpthrd::stop_it();
}

void RestSrv::_spin()
{
    fd_set  rd;
    int     ndfs = _s.socket();
    int     rt;
    char req[2048] = {0};

    _fatal = false;
    FD_ZERO(&rd);
    FD_SET(_s.socket(),  &rd);      // put the pistener
    for(auto& s : _sons)            // put all connected clients
    {
        if(s->socket()>0)
        {
            FD_SET(s->socket(),  &rd);
            ndfs = std::max(ndfs, s->socket());
        }
        else
            _mon_dirt = true;
    }
    timeval tv = {0, 0x1FF};        // select timout
    int is = ::select(ndfs+1, &rd, 0, 0, &tv);
    if(is ==-1) {
        LOGEX("EXIT_APP error: select" << errno <<", " <<strerror(errno));
        _s.term();
        sig_it();
        ApStat=EXIT_APP;
        _mon_dirt = true;
        _fatal = true;
        _clear();
        return;
    }
    if(is>0)
    {
        if(FD_ISSET(_s.socket(),  &rd))
        {
            Tcli* cs = new Tcli();
            if(_s.accept(*cs)>0)
            {
                if(_sons.size()>32)
                {
                    cs->term();
                    delete cs;
                    return;
                }
                else{
                    cs->set_blocking(0);
                    cs->_dbage = Clk.tick();
                    _sons.push_back(cs);
                }
            }
            else
            {
                cs->term();
                delete cs;
            }
        }
        for(auto& s : _sons)
        {
            if(s->socket()<=0)
                continue;
            if(FD_ISSET(s->socket(),  &rd))
            {
                rt = s->receive(req, sizeof(req)-2);
                if(rt==-1){
                    if(Clk.tick() - s->_dbage > 60)
                    {
                        s->term();
                        _mon_dirt = true;
                    }
                    continue;
                }
                if(rt==0)
                {
                    if(s->_req.find("\r\n\r\n")!=std::string::npos)
                    {
                        _toast(s);
                    }
                    s->term();
                    _mon_dirt = true;
                    continue;
                }
                req[rt] = 0;
                s->_dbage = Clk.tick();
                s->_req.append(req);
                if(::strstr(req,"\r\n\r\n"))
                {
                    LOGD3(req);
                    _toast(s);
                    s->term();
                    _mon_dirt = true;
                }
            }
        }
    }
    _clear();
}

void RestSrv::close()
{
    if(_hthread)
    {
        this->sig_it();
        ::msleep(100);
        this->stop_it();
    }
    _s.term();
    for(auto& s : _sons){ delete s; }
    _sons.clear();

}

RestSrv::~RestSrv()
{
    this->close();
    PSockSrv=nullptr;
}

int  RestSrv::_defer_to_device(I_IDev* pdev, const rapidjson::Document& value)
{
    any_t       data;
    bool        apply = false;
    devsmap_t devs;

    if(pdev && value.IsObject())
    {
        if(pdev)
        {
            std::string kvs;
            char loco[256];

            for (rapidjson::Value::ConstMemberIterator subnod =
                 value.MemberBegin(); subnod != value.MemberEnd();
                 ++subnod)
            {
                const char*     name = subnod->name.GetString();

                if(!::strcmp(name,"refresh")){ apply=true;continue; }
                kvs.append(name);
                kvs.append("=");

                if(subnod->value.IsInt()){
                    ::sprintf(loco,"%d&",subnod->value.GetInt());
                    kvs.append(loco);
                    continue;
                }
                if(subnod->value.IsString()){
                    ::sprintf(loco,"%s&",subnod->value.GetString());
                    kvs.append(loco);
                    continue;
                }
                if(subnod->value.IsArray() && subnod->value.Size())
                {
                    for (size_t i = 0; i < subnod->value.Size(); i++)
                    {
                        if(subnod->value[i].IsInt()){
                            sprintf(loco,"%d,",subnod->value[i].GetInt());
                            kvs.append(loco);
                        }
                        if(subnod->value[i].IsString()){
                            sprintf(loco,"%s,",subnod->value[i].GetString());
                            kvs.append(loco);
                        }
                    }
                    if(subnod != value.MemberEnd())
                        kvs.append("&");
                }
            }
            devs[pdev] = kvs;
        }
    }
    App->web_set_data(devs, apply);
    return -1;
}

int  RestSrv::_process_forjson_post(const std::string& path, const std::string& req)
{
    if(!path.substr(1).empty())
    {
        I_IDev* pdev = App->get_per(path.substr(1).c_str());
        if(pdev)
        {
            rapidjson::Document doc;
            doc.Parse(req.c_str());
            if(!doc.HasParseError())
            {
                this->_defer_to_device(pdev, doc);
            }
        }
    }
    else {
        rapidjson::Document doc;
        doc.Parse(req.c_str());
        if(!doc.HasParseError())
        {
            for (rapidjson::Value::ConstMemberIterator jnod =
                 doc.MemberBegin(); jnod != doc.MemberEnd();
                 ++jnod)
            {
                const   char* devname = jnod->name.GetString();
                I_IDev* pdev = App->get_per(devname);
                if(pdev)
                {
                    this->_defer_to_device(pdev, ( const rapidjson::Document&)jnod->value);
                }
            }
        }
    }
    return 0;
}

bool RestSrv::_get_add_devices(const std::string& qry, devsmap_t& refrdevs)
{
    bool refresh=false;
    if(qry[0]=='/')
    {
        if(qry[1]=='?')     // refresh all
        {
            refresh=true;
        }
        if(qry[1]=='e')    // get by category
        {
            EPERIPH  et = Divais::Get_cat(qry.substr(1).c_str());
            App->get_alldevs(refrdevs, et, refresh);
        }
        else if(qry[2]=='e')    // get by category
        {
            EPERIPH  et = Divais::Get_cat(qry.substr(2).c_str());
            App->get_alldevs(refrdevs, et, refresh);
        }
        else
        {
            App->get_alldevs(refrdevs, eNOCAT, refresh);
        }
    }
    return refresh;
}

bool RestSrv::_get_from_query(const std::string& req, devsmap_t& refrdevs)
{
    bool refresh;
    strarray_t       devs;
    CFL::explode(req.substr(1),'&',devs);       // /param=&param=
    for(auto& p : devs)
    {
        I_IDev* pdev;
        if(p[0]=='?'){
            p = p.substr(1);
            refresh = true;
        }else {
            refresh=false;
        }
        size_t      eq = p.find('=');
        std::string spath;
        std::string svalue;

        if(eq!=NOPOS){
            spath = p.substr(0,eq);
            svalue = p.substr(eq+1);
        }else{
            spath = p;
        }

        strarray_t       parts;
        size_t nparts = CFL::explode(spath,'/',parts);

        pdev = App->get_per(parts[0].c_str());
        if(!pdev){ LOGW("No such device" << p); continue;}
        if(refresh)
        {
            if(nparts==2)
            {
                std::string realval = parts[1]+"="; realval += svalue;
                refrdevs[pdev] = realval; //write
            }
            else
            {
                refrdevs[pdev]=READDEV;
            }
        }
        else {
            refrdevs[pdev]=EMPTYS;
        }
    }
    return refresh;
}

//  ?device/prop&?device/prop
//  /device                         // get all
//  /device/property                // get one
//  /device/property=sdfsdfsdfsd    // write one
//  /devide/property=4,4,4,4        // write one
//
int  RestSrv::_process_resful_request(const std::string& path, const std::string& req)
{
    devsmap_t refrdevs;
    bool      updates=false;

    if(req.at(0)=='r' || req.at(0)=='e')
    {
        if(req=="reboot") {ApStat = DO_EXIT; ::system("reboot"); return 0;}
        if(req=="reload") {ApStat = RELOAD_S; return 0;}
        if(req=="exit")   {ApStat = EXIT_APP; return 0;}
    }
    if(req=="/?" || req=="/" || req.find("/e")==0 || req.find("/?e")==0)
    {
        updates=_get_add_devices(req, refrdevs);
    }
    else
    {
        updates=_get_from_query(req, refrdevs);
    }

    if(updates){
        App->web_set_data(refrdevs, updates>0);
    }
    return _build_forjson( refrdevs);
}


int  RestSrv::_defer_query(const std::string& path, const std::string& req)
{
    // dejsonize
    if(req[0]=='{') // set
    {
        return _process_forjson_post(path, req);
    }

    else if(req[0]=='/') // allways get  (could be set if has =)
    {
        return _process_resful_request(path, req);
    }
    return false;
}

bool RestSrv::_build_forjson(const devsmap_t& devs)
{
    rapidjson::Document doc;
    doc.SetObject();

    for(const auto& a: devs)
    {
        _get_dev_forjson(a.first, doc);
    }

    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    doc.Accept(writer);
    _outstr = strbuf.GetString();
    return true;
}

void RestSrv::_get_dev_forjson(I_IDev* pd, rapidjson::Document&  doc)
{
    rapidjson::Document::AllocatorType& allc = doc.GetAllocator();
    rapidjson::Value device(rapidjson::kObjectType);
    rapidjson::Value kisland(rapidjson::kObjectType);
    kisland.SetObject();
    rapidjson::Value kname(pd->dev_key(), allc);
    std::string all = pd->get_value(SALLDATA);
    strarray_t          parts;
    CFL::explode(all,',',parts);
    strarray_t::const_iterator a = parts.begin();
    for(; a != parts.end(); a++)
    {
        rapidjson::Value kkey((const char*)a->c_str(),allc);
        a++;
        if(a==parts.end())break;
        rapidjson::Value kval((const char*)a->c_str(),allc);
        kisland.AddMember(kkey, kval, allc);
    }
    doc.AddMember(kname, kisland, allc);
}






