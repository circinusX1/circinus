#ifndef HTTPS_H
#define HTTPS_H

#include "icombase.h"
#include <curl/curl.h>
#include <string>

#ifdef __linux
#   define EXPORT_CLS __attribute__((visibility("default")))
#else
#   define EXPORT_CLS __declspec(dllexport)
#endif

class EXPORT_CLS Https : public IComm
{

public:
    Https();
    Https(IDevice*, const char*);
    virtual ~Https();
    int open();
    int close();
    int sendTo(const uint8_t*, int);
    int recFrom(unsigned char* bytes, int room);
    void debug(int d);
    void setTout(int to, int na);
    bool isOpen();
    int copyFile(const char* fileS, const char* fileD, bool);
    void setspeed(int);
    void flush();
    void publish(){}
    int reopen(const char*);
    int esc(const char*,int);
    int setOptions(const char*);
    int removeFile();
    int createFile();
    const char* name()const;
    IDevice::D_MSG getMode()const;
protected:
    static size_t writeCallbackHdr(char* buf, size_t size, size_t nmemb, void* up);
    static size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);


private:
    CURL    *_curl;
    IDevice *_pd;
    std::string _https;
    std::string _cert;
    std::string _userpass;
    std::string _payload;
    std::string _data;
    bool        _open;
};

#endif // HTTPS_H
