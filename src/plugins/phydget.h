#ifndef PHYDGET_H
#define PHYDGET_H


#include <icombase.h>
#include <stdint.h>
#include <phidget22.h> // ./libphidget22-1.0.0.20171106/src/ or build and install
#include <string>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

class EXPORT_CLS Phydget : public IComm
{
public:
    Phydget(IDevice* pdev,  const char* constr, const char* name);
    virtual ~Phydget();
    int open();
    int reopen(const char* where);
    int close();
    void setspeed(int ms);
    int sendTo(cbyte*, int);
    int recFrom(unsigned char* bytes, int room);
    void debug(int d);
    void setTout(int to);
    bool isOpen();
    void flush();
    const char* name()const;
    IDevice::D_MSG getMode();
    int setOptions(const char*);
    int esc(const char*, int);
    IDevice::D_MSG getMode()const;
public:
    IDevice*                    _pdev;
    PhidgetDigitalInputHandle   _chIa;
    PhidgetDigitalInputHandle   _chIb;

    PhidgetDigitalOutputHandle  _chOa;
    PhidgetDigitalOutputHandle  _chOb;
    std::string                 _constr;
    int                         _debug;
    std::string                 _name;
    std::string                 _label;
    int                         _serial;
    std::string                 _rw;
    std::string                 _lastdata;
    bool                        _changed;
    bool                        _attached;
};

#endif // PHYDGET_H

