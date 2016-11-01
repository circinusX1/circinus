#ifndef SSH_H
#define SSH_H


#include <icombase.h>
#include <string>
#ifndef _WIN32
#include <unistd.h>
#include <libssh/libssh.h>  // sudo apt-get install libssh-dev
#else
#include <Windows.h>
#endif

class EXPORT_CLS Ssh : public IComm
{
public:
    Ssh();
    Ssh(IDevice* pdev, const char* cred, const char* name);
    virtual  ~Ssh();
    virtual int open();
    virtual int close();
    virtual int reopen(const char* where);
    virtual void setspeed(int ms);
    virtual int sendTo(cbyte*, int);
    virtual int recFrom(unsigned char* bytes, int room);
    virtual void debug(int d);
    virtual void setTout(int to,int selt);
    virtual bool isOpen();
    virtual void flush();
    virtual int esc(const char* dt, int how);
    virtual int setOptions(const char*);
    virtual const char* getOptions(const char*);
    virtual void publish();
    virtual IDevice::D_MSG getMode()const;
    virtual const char* name()const;

private:
    int _verify_knownhost();

private:
    ssh_session _ssh_session;
    ssh_channel _channel;
    IDevice*    _dev;
    std::string _cred;
    std::string _name;
    std::string _user;
    std::string _pass;
    std::string _host;
    std::string _certFile;
    uint32_t    _port;
};

#endif // SSH_H
