
#include <iostream>
#include "main.h"
#include "serialcom.h"

SerialComm::SerialComm():_pdev(0)
{

}

SerialComm::SerialComm(IDevice* pd, const char* cred, const char* name):IComm(),_pdev(pd),_name(name)
{
    //COM,115200,8N1
    _timout = 64;
    _credentials = cred;
    pd->addDev(this);
    _seltoms=2;
}

SerialComm::~SerialComm()
{
    close();
    if(_pdev)
        _pdev->remDev(this);
}

const char* SerialComm::name()const
{
    return _name.c_str();
}
const char* SerialComm::getOptions(const char* op)
{
    if(!::strcmp(op,"CTS"))
    {
        return _device.IsCTSEnabled() ? "true" : "false";
    }
    else if(!::strcmp(op,"DCD"))
    {
        return _device.IsDCDEnabled() ? "true" : "false";
    }
    else if(!::strcmp(op,"RTS"))
    {
        return _device.IsDSREnabled() ? "true" : "false";
    }
    return "";
}

int SerialComm::setOptions(const char* pred)
{
    char loco[32];

    ::strncpy(loco,pred,31);
    char* pe = strchr(loco,'=');
    if(pe)
    {
        *pe=0;
        std::string pred=pe++;
        std::string sop=pe;
        bool op = (sop=="true");
        if(pred=="DTR")
        {
            op ? _device.enableDTR() : _device.disableDTR();
            return 0;
        }
        else if(pred=="RTS")
        {
            op ? _device.enableRTS() : _device.disableRTS();
            return 0;
        }
    }
    LOGDETAIL("Option should be: DRT=true/false or RTS=true/false");
    return -1;
}

int SerialComm::esc(const char* dt, int how)
{
    return _device.esc(dt, how);
}

int SerialComm::reopen(const char* where)
{
    close();
    _credentials = where;
    return open();
}

int SerialComm::open()
{
    char        credentials[128];
    ::strcpy(credentials,_credentials.c_str());
    const char* dev = ::strtok(credentials,",");
    int speed = ::atoi(::strtok(0,","));
    const char* mode = ::strtok(0,",");
    return _device.OpenComport(dev, speed, mode);
}

int SerialComm::close()
{
    if(_device._port!=0)
        _device.CloseComport();
    _device._port = 0;
    return 0;
}

void SerialComm::flush()
{
    _device.flushRXTX();
}

void SerialComm::setspeed(int ms)
{
    _device.setWrTime(ms);
}

bool SerialComm::isOpen()
{
#ifdef __linux
    return _device._port > 0;
#else
    return _device._port > (HANDLE)0;
#endif
}

int SerialComm::sendTo(cbyte* data, int length)
{
    if(_device.isOpen())
        return _device.SendBuf((unsigned char*)data, length, _timout);
    return -1;
}

int SerialComm::recFrom(unsigned char* buff, int bytes)
{
    if(_device.isOpen())
        return _device.Read(buff,bytes,_timout,_seltoms);
    return -1;
}

void SerialComm::setTout(int to, int sto){_timout = to; _seltoms=sto;}

void SerialComm::debug(int d)
{
    _debug=d;
}

IDevice::D_MSG SerialComm::getMode()const
{
    return IDevice::eBINARY;
}

int SerialComm::removeFile()
{
    return -1;
}

int SerialComm::createFile()
{
    return -1;
}

void SerialComm::publish()
{

}
