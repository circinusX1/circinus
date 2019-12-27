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

#ifdef WITH_CURL

#include <dlfcn.h>
#include "dcore.h"
#include "database.h"
#include "apis.h"
#include "comcurl.h"

ComCurl::ComCurl():Divais(eSTRING, eCURL)
{
#ifdef DYNALL
    std::string libr = ::execute("find /usr -name libcur*.so");
    if(libr.empty()) {
        LOGE("cannot find any libcurl*.so library installed. Disabling the db feature");
        return;
    }
    libr.pop_back();
    _sohandler = ::dlopen(libr.c_str(), RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD);
    if(_sohandler==nullptr) {
        LOGE("cannot load " << libr <<":"<< dlerror() <<", disabling curl feature");
        return;
    }
    _curl_easy_setopt  = (p_curl_easy_setopt)::dlsym(_sohandler, "curl_easy_setopt");
    _curl_easy_init    = (p_curl_easy_init)::dlsym(_sohandler, "curl_easy_init");
    _curl_easy_perform = (p_curl_easy_perform)::dlsym(_sohandler, "curl_easy_perform");
    _curl_easy_cleanup = (p_curl_easy_cleanup)::dlsym(_sohandler, "curl_easy_cleanup");
    _curl_multi_cleanup= (p_curl_multi_cleanup)::dlsym(_sohandler, "curl_multi_cleanup");
    _curl_slist_append  = (p_curl_slist_append)::dlsym(_sohandler, "p_curl_slist_append");
    _curl_slist_free_all= (p_curl_slist_free_all)::dlsym(_sohandler, "curl_slist_free_all");
#else
    _curl_easy_setopt  = (p_curl_easy_setopt)curl_easy_setopt;
    _curl_easy_init    = (p_curl_easy_init)curl_easy_init;
    _curl_easy_perform = (p_curl_easy_perform)curl_easy_perform;
    _curl_easy_cleanup = (p_curl_easy_cleanup)curl_easy_cleanup;
    _curl_multi_cleanup= (p_curl_multi_cleanup)curl_multi_cleanup;
    _curl_slist_append  = (p_curl_slist_append)curl_slist_append;
    _curl_slist_free_all= (p_curl_slist_free_all)curl_slist_free_all;
#endif
}

void ComCurl::set_url(const char* url, int auth, const char* cred)
{
    _url = url;
    _auth = auth;
    _cred = cred;
}

ComCurl::~ComCurl()
{
   if( _sohandler)
       ::dlclose(_sohandler);
    iclose();
}

void  ComCurl::set_headers(Sqrat::Array& keys, Sqrat::Array& vals)
{
    if(_headers)
        _curl_slist_free_all(_headers);
    std::string fk = *keys.GetValue<const char*>(0).Get();
    fk += ":";
    fk += *vals.GetValue<const char*>(0).Get();
    _headers = _curl_slist_append(nullptr, fk.c_str());

    for(int i=1; i< keys.GetSize();++i)
    {
        fk = *keys.GetValue<const char*>(i).Get();
        fk += ":";
        fk += *vals.GetValue<const char*>(0).Get();
       _headers = _curl_slist_append(_headers, fk.c_str());
    }
}

void ComCurl::set_post(Sqrat::Array& keys, Sqrat::Array& vals)
{
    _data.clear();
    for(int i=0; i< keys.GetSize();++i)
    {
        _data += *keys.GetValue<const char*>(i).Get();
        _data += "=";
        _data += *vals.GetValue<const char*>(i).Get();
        _data += "&";
    }
    _pp=2;
}

void ComCurl::set_put(const char* data)
{
    _data = data;
    _pp = 1;
}

bool ComCurl::iopen(int)
{
    return false;
}

void ComCurl::iclose()
{
    if(_headers)
        _curl_slist_free_all(_headers);
    if(_pcurl)
        _curl_multi_cleanup(_pcurl);
    _pcurl=nullptr;
    return;
}

int   ComCurl::perform(Sqrat::Function f)
{
    _pcurl = _curl_easy_init();

    _foo = f;
#ifdef CURL_TRACE
    _curl_easy_setopt(_pcurl, CURLOPT_VERBOSE, 1L);
    _curl_easy_setopt(_pcurl, CURLOPT_DEBUGDATA, this);
    _curl_easy_setopt(_pcurl, CURLOPT_DEBUGFUNCTION, VClient::catchHeaders);
#else
    _curl_easy_setopt(_pcurl, CURLOPT_VERBOSE, 0L);
#endif

    _curl_easy_setopt(_pcurl, CURLOPT_FOLLOWLOCATION, 1L);
    _curl_easy_setopt(_pcurl, CURLOPT_NOPROGRESS, 1L);
    _curl_easy_setopt(_pcurl, CURLOPT_URL, _url.c_str());
    _curl_easy_setopt(_pcurl, CURLOPT_CONNECTTIMEOUT, 10);
    _curl_easy_setopt(_pcurl, CURLOPT_ACCEPTTIMEOUT_MS, 8000);
    _curl_easy_setopt(_pcurl, CURLOPT_BUFFERSIZE, 4096);
    if(_auth != CURLAUTH_NONE)
    {
        _curl_easy_setopt(_pcurl, CURLOPT_USERPWD, _cred.c_str());
        _curl_easy_setopt(_pcurl, CURLOPT_HTTPAUTH, _auth);
    }

    if(_pp==2 && !_data.empty())
    {
        _curl_easy_setopt(_pcurl, CURLOPT_CUSTOMREQUEST, "POST");
        _curl_easy_setopt(_pcurl, CURLOPT_POSTFIELDSIZE, (long)_data.length());
        _curl_easy_setopt(_pcurl, CURLOPT_POSTFIELDS, _data.c_str());
    }
    else if(_pp==1 && !_data.empty())
    {
        _curl_easy_setopt(_pcurl, CURLOPT_CUSTOMREQUEST, "PUT");
        _curl_easy_setopt(_pcurl, CURLOPT_POSTFIELDSIZE, (long)_data.length());
        _curl_easy_setopt(_pcurl, CURLOPT_POSTFIELDS, _data.c_str());
    }
    _curl_easy_setopt(_pcurl, CURLOPT_WRITEFUNCTION, &ComCurl::writeCallback);
    _curl_easy_setopt(_pcurl, CURLOPT_WRITEDATA, this);
    if(_headers)
    {
        _curl_easy_setopt(_pcurl, CURLOPT_HTTPHEADER, _headers);
    }
    int rv =  _curl_easy_perform(_pcurl);
    if(_headers){
        _curl_slist_free_all(_headers);
        _headers=nullptr;
    }
    _curl_multi_cleanup(_pcurl);
    _pcurl=nullptr;
    return rv;
}

// receiving
size_t ComCurl::_receiving(char* buf, size_t nmemb)
{
    try{
        fastbuf_t<512>  ptr(nmemb+1);
        ::strncpy(ptr, buf, nmemb);
        ((char*)ptr)[nmemb] = 0;

        bool rv = *_foo.Fcall<bool>((const char*)ptr).Get();
        if(rv==false)
        {
            iclose();
            return 0;
        }
    }
    catch(Sqrat::Exception ex)
    {
        ApStat=RESTART;
        LOGEX( __PRETTY_FUNCTION__ << ex.Message()  << " ");
        LOGEX(SqErrStr);
        return 0;
    }
    return nmemb;
}

size_t ComCurl::writeCallbackHdr(char* buf, size_t size, size_t nmemb, void* up)
{
    UNUS(buf);
    UNUS(up);
    ::msleep(1);
    return size*nmemb; //tell _curl how many bytes we handled
}

size_t ComCurl::writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{
    ComCurl* pthis = reinterpret_cast<ComCurl*>(up);
    ::msleep(1);
    if(pthis->_receiving(buf,nmemb))
    {
        return size*nmemb; //tell _curl how many bytes we handled
    }
    return 0;
}

bool	ComCurl::_write_now(const any_t& a)
{
    return false;
}

size_t  ComCurl::_read_now(any_t& _curdata, const char* filter)
{
    return -1;
}

bool	ComCurl::_touch_it(size_t t)
{
    return false;
}

#endif //#ifdef WITH_CURL
