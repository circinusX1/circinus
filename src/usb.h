#ifndef PHYDGET_H
#define PHYDGET_H

#include "icombase.h"
#include <string>
#include <libusb-1.0/libusb.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#endif

class  PUsb : public IComm
{
public:
    PUsb(IDevice* pdev,  const char* constr, const char* name);
    virtual ~PUsb();
    int open();
    int close();
    void setspeed(int ms);
    int sendTo(const uint8_t*, int);
    int recFrom(unsigned char* bytes, int room);
    void debug(int d);
    void setTout(int to,int selto);
    bool isOpen();
    int setOptions(const char*);
    const char* getOptions(const char*);
    void flush();
    int reopen(const char* where);
    const char* name()const;
    IDevice::D_MSG getMode()const;
    void testin(const char* param);
    int esc(const char*,int);
    int removeFile();
    int createFile();
    void publish();

protected:
    size_t  _enumerate(size_t& elems);
    int     _fetch(int dev);
    int    _discard(uint8_t * dest , size_t len);

public:


    bool        _debug;
    std::string _fname;
    IDevice*    _pdev;
    bool        _open;
    std::string _name;
    std::string _prevdata;
    int         _tout = 32;
    libusb_device           **_devs;
    libusb_device_handle    *_dev_handle;

    uint8_t*                _large;
    int                     _bytes = 0;
    int                     _v = 0;
    int                     _p = 0;

};

#endif // PHYDGET_H
