#ifndef PHYDGET_H
#define PHYDGET_H


#include <icombase.h>
#include <string>
#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#endif

class EXPORT_CLS PFile : public IComm
{
public:
    PFile(IDevice* pdev,  const char* constr, const char* name);
    virtual ~PFile();
    int open();
    int close();
    void setspeed(int ms);
    int sendTo(const uint8_t*, int);
    int recFrom(unsigned char* bytes, int room);
    void debug(int d);
    void setTout(int to,int t);
    bool isOpen();
    int setOptions(const char*);
    void flush();
    int reopen(const char* where);
    int copyFile(const char* fileS, const char* fileD, bool);
    const char* name()const;
    IDevice::D_MSG getMode()const;
    void testin(const char* param);
    int esc(const char*,int);
    int removeFile();
    int createFile();
    void publish();

public:


    bool        _debug;
    std::string _fname;
    IDevice*    _pdev;
    bool        _open;
    std::string _name;
    std::string _prevdata;
};

#endif // PHYDGET_H
