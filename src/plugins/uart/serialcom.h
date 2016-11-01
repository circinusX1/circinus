#ifndef SERIALCOM_H
#define SERIALCOM_H

#include "icombase.h"

#include "serialport.h"


class SerialComm : public IComm
{
public:
    SerialComm();
    SerialComm(IDevice* pd, const char* cred, const char* name);
    virtual ~SerialComm();

    int open();
    int close();
    int sendTo(cbyte*, int);
    int recFrom(unsigned char* buff, int bytes);
    void setTout(int to, int selto);
    bool isOpen();
    void debug(int d);
    void setspeed(int ms);
    int reopen(const char* c);
    void flush();
    int esc(const char* dt, int how);
    const char* name()const;
    IDevice::D_MSG getMode()const;
    int setOptions(const char*);
    const char* getOptions(const char* op);
    int removeFile(); //just for files devices
    int createFile();
    void publish();
private:
    std::string _credentials;
    SerialPort  _device;
    int         _timout;
    IDevice*    _pdev;
    bool        _debug;
    std::string _name;
    int         _seltoms;
};


#endif // SERIALCOM_H
