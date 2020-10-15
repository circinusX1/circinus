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

#ifdef WITH_CURL
#ifndef COMCURL_H
#define COMCURL_H

#include <curl/curl.h>
#include "sqwrap.h"
#include "inst.h"
#include "dcore.h"
#include "logs.h"
#include "divais.h"

using namespace GenericHw;
class ComCurl : public DvCore, public Divais, private Reg<ComCurl>
{
public:

    typedef  CURLcode (*p_curl_easy_setopt)(CURL *_pcurl, CURLoption option, ...);
    typedef  CURL* (*p_curl_easy_init)(void);
    typedef  CURLcode (*p_curl_easy_perform)(CURL *_pcurl);
    typedef  void (*p_curl_easy_cleanup)(CURL *_pcurl);
    typedef CURLcode (*p_curl_multi_cleanup)(CURLM *multi_handle);
    typedef struct curl_slist* (*p_curl_slist_append)(struct curl_slist *, const char *);
    typedef void (*p_curl_slist_free_all)(struct curl_slist *);
    typedef CURLcode (*p_curl_easy_pause)(CURL *handle, int bitmask);

    ComCurl(int maxbuf, const char*);
    ComCurl(SqObj& o, int maxbuf, const char*);
    virtual ~ComCurl();
    void  set_url(const char* url, int auth, const char* cred);
    void  set_post(Sqrat::Array& keys, Sqrat::Array& vals);
    void  set_headers(Sqrat::Array& keys, Sqrat::Array& vals);
    void  set_put(const char* data);
    void  perform_cb(Sqrat::Function f, int tout);
    const char*   perform(int tout);
    bool iopen(int);
    void iclose();

    static void squit(SqEnvi& e){
        Sqrat::Class<ComCurl> cls(e.theVM(), _SC("CURL"));
        cls.Ctor<int, const char* >();
        cls.Ctor<SqObj&, int, const char* >();
        cls.Functor(_SC("close"), &ComCurl::iclose);
        cls.Functor(_SC("set_post"), &ComCurl::set_post);
        cls.Functor(_SC("set_headers"), &ComCurl::set_headers);
        cls.Functor(_SC("set_url"), &ComCurl::set_url);
        cls.Functor(_SC("set_put"), &ComCurl::set_put);
        cls.Functor(_SC("perform_cb"), &ComCurl::perform_cb);
        cls.Functor(_SC("perform"), &ComCurl::perform);

        Sqrat::RootTable().Bind(_SC("CURL"), cls);
    }

    static size_t writeCallbackHdr(char* buf, size_t size, size_t nmemb, void* up);
    static size_t writeCallback(char* buf, size_t size, size_t nmem_writingb, void* up);

protected:
	virtual bool	_write_now(const any_t& a);
	virtual size_t  _fecth(any_t& _curdata, const char* filter);
	virtual bool	_mon_pick(size_t t);
    size_t _receiving(char* buf, size_t nmemb);

private:
    size_t                  _maxbuff;
    void*                   _sohandler;
    p_curl_easy_setopt      _curl_easy_setopt = nullptr;
    p_curl_easy_init        _curl_easy_init = nullptr;
    p_curl_easy_perform     _curl_easy_perform = nullptr;
    p_curl_easy_cleanup     _curl_easy_cleanup = nullptr;
    p_curl_multi_cleanup    _curl_multi_cleanup = nullptr;
    p_curl_slist_append     _curl_slist_append = nullptr;
    p_curl_slist_free_all   _curl_slist_free_all = nullptr;
    p_curl_easy_pause       _curl_easy_pause = nullptr;
    std::string             _url;
    std::string             _cred;
    std::string             _data;
    std::string             _reply;
    struct curl_slist*      _headers = nullptr;
    int                     _auth;
    int                     _pp = 0;
    Sqrat::Function         _foo;
    bool                    _cb = false;
    CURL*                   _pcurl = nullptr;
};

#endif // COMCURL_H
#endif //#ifdef WITH_CURL
