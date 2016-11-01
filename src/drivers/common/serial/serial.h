/* Copyright (C) 2018 Pegmatis.Inc - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the APACHE license, (author: Marius Chincisan)
*
* You should have received a copy of the APACHE license with
* this file. If not, please visit www.github.com, Author Marius Chincisan
*/

#ifndef TTY_SERIAL_H
#define TTY_SERIAL_H

#include "buscore.h"
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/file.h>
#include <errno.h>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <cstdint>


namespace BusNs
{
class SerialPort : public BusCore
{
public:
    SerialPort(const char*, int, const char *);
    SerialPort(){};
    ~SerialPort();
    void resetUsb();
    int openPort();
    int readTo(unsigned char *, int, int to=100, int sto=2);
    int sendByte(unsigned char);
    int sendBuff(const uint8_t* , int, int);
    void closePort();
    void strPuts(const char *);
    int dcdEnabled();
    int ctsEnabled();
    int dsrEnabled();
    void enableDTR();
    void disableDTR();
    void enableRTS();
    void disableRTS();
    int esc(const char* rd, bool how);
    void flushRX();
    void flushTX();
    void flushRXTX();
    int getPortNr(const char *);
    bool isOpen();
    void setWrTime(int ms);
    int writeBybyBy(unsigned char *, int, int);

#if defined (__linux__) || defined(__FreeBSD__)
    struct termios _curTCSett;
    struct termios _oldTCSett;
    int            _port;
#else
    HANDLE         _port;
#endif
    int         _error;
    std::string _mode;
    int         _bouts=0;
    int         _wrdelay=1000;
};
};

#endif // SERIAL_H
