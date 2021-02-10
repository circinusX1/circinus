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


#ifndef DSERIAL_CPP
#define DSERIAL_CPP

#include <unistd.h>
#include <errno.h>
#include "dlconfig.h"
#include "logs.h"
#include "dserial.h"
#include <sys/ioctl.h>
#include <sys/file.h>


namespace GenericHw
{

DvSerial::DvSerial(const char* comname, // /dev/ttyUSB
                   int baudrate,
                   const char *mode):
    _port(0),
    _error(0)
{
    _mode = mode;
    _bouts = baudrate;
	_dev_node = comname;
}

DvSerial::~DvSerial()
{
    this->iclose();
}

#if defined (__linux__) || defined (__FreeBSD__)
bool DvSerial::iopen(int )
{
    int baudr,status;

    switch(_bouts)
    {
    case      50 : baudr = B50;
        break;
    case      75 : baudr = B75;
        break;
    case     110 : baudr = B110;
        break;
    case     134 : baudr = B134;
        break;
    case     150 : baudr = B150;
        break;
    case     200 : baudr = B200;
        break;
    case     300 : baudr = B300;
        break;
    case     600 : baudr = B600;
        break;
    case    1200 : baudr = B1200;
        break;
    case    1800 : baudr = B1800;
        break;
    case    2400 : baudr = B2400;
        break;
    case    4800 : baudr = B4800;
        break;
    case    9600 : baudr = B9600;
        break;
    case   19200 : baudr = B19200;
        break;
    case   38400 : baudr = B38400;
        break;
    case   57600 : baudr = B57600;
        break;
    case  115200 : baudr = B115200;
        break;
    case  230400 : baudr = B230400;
        break;
    case  460800 : baudr = B460800;
        break;
#if ! defined (__FreeBSD__)
    case  500000 : baudr = B500000;
        break;
    case  576000 : baudr = B576000;
        break;
    case  921600 : baudr = B921600;
        break;
    case 1000000 : baudr = B1000000;
        break;
    case 1500000 : baudr = B1500000;
        break;
    case 2000000 : baudr = B2000000;

        break;
    case 2500000 : baudr = B2500000;
        break;
    case 3000000 : baudr = B3000000;
        break;
    case 3500000 : baudr = B3500000;
        break;
    case 4000000 : baudr = B4000000;
        break;
#endif
    default      : LOGE("invalid baudrate");
        return false;
        break;
    }

    int cbits=CS8,
            cpar=0,
            ipar=IGNPAR,
            bstop=0;

    if(_mode.length() != 3)
    {
        LOGE("invalid mode " << _mode << std::endl);
        return(1);
    }

    switch(_mode[0])
    {
    case '8': cbits = CS8;
        break;
    case '7': cbits = CS7;
        break;
    case '6': cbits = CS6;
        break;
    case '5': cbits = CS5;
        break;
    default :
        LOGE("invalid number of data-bits " << _mode[0]);
        return(1);
        break;
    }

    switch(_mode[1])
    {
    case 'N':
    case 'n': cpar = 0;
        ipar = IGNPAR;
        break;
    case 'E':
    case 'e': cpar = PARENB;
        ipar = INPCK;
        break;
    case 'O':
    case 'o': cpar = (PARENB | PARODD);
        ipar = INPCK;
        break;
    default : LOGE("invalid parity " << _mode[1]);
        return(1);
        break;
    }

    switch(_mode[2])
    {
    case '1': bstop = 0;
        break;
    case '2': bstop = CSTOPB;
        break;
    default : LOGE("invalid number of stop bits " << _mode[2]);
        return(false);
        break;
    }

    /*
    http://pubs.opengroup.org/onlinepubs/7908799/xsh/termios.h.html
    http://man7.org/linux/man-pages/man3/termios.3.html
    */

	_port = ::open(_dev_node.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if(_port==-1)
    {
		LOGEX(strerror(errno) << " unable to open "<<_dev_node<<" com_port");
        return false;
    }

    /* lock access so that another process can't also use the _port */
    if(flock(_port, LOCK_EX | LOCK_NB) != 0)
    {
        ::close(_port);
        _port = 0;
        perror("Another process has locked the com");
        return false;
    }

    _error = tcgetattr(_port, &_oldTCSett);
    if(_error==-1)
    {
        ::close(_port);
        ::flock(_port, LOCK_UN);  /* free the _port so that others can use it. */
        ::perror("unable to read _portsettings ");
        _port = 0;
        return false;
    }
    memset(&_curTCSett, 0, sizeof(_curTCSett));  /* clear the new struct */

    _curTCSett.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD;
    _curTCSett.c_iflag = ipar;
    _curTCSett.c_oflag = 0;
    _curTCSett.c_lflag = 0;
    _curTCSett.c_cc[VMIN] = 0;      /* block untill n bytes are received */
    _curTCSett.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */

    cfsetispeed(&_curTCSett, baudr);
    cfsetospeed(&_curTCSett, baudr);

    _error = tcsetattr(_port, TCSANOW, &_curTCSett);
    if(_error==-1)
    {
        tcsetattr(_port, TCSANOW, &_oldTCSett);
        ::close(_port);
        flock(_port, LOCK_UN);  /* free the _port so that others can use it. */
        _port=0;
        perror("unable to adjust _portsettings ");
        return false;
    }

    /* http://man7.org/linux/man-pages/man4/tty_ioctl.4.html */

    if(ioctl(_port, TIOCMGET, &status) == -1)
    {
        tcsetattr(_port, TCSANOW, &_oldTCSett);
        flock(_port, LOCK_UN);  /* free the _port so that others can use it. */
        perror("unable to read _portstatus 1");
        return false;
    }

    status |= TIOCM_DTR;    /* turn on DTR */
    status |= TIOCM_RTS;    /* turn on RTS */

    if(ioctl(_port, TIOCMSET, &status) == -1)
    {
        ::tcsetattr(_port, TCSANOW, &_oldTCSett);
        ::flock(_port, LOCK_UN);  /* free the _port so that others can use it. */
        perror("unable to set _portstatus");
        return(false);
    }
    return(true);
}

int DvSerial::_async_read(unsigned char *buf, int size, int to, int sto)
{
    int     nbytes = 0;
    struct  timeval tv;
    fd_set  fds;

    do{
        FD_ZERO(&fds);
        FD_SET(_port, &fds);
        tv.tv_sec=0;
        tv.tv_usec=(8912);
        int sel = ::select(_port+1, &fds, NULL, NULL, &tv);
        if(sel < 0)
        {
            LOGE("::select error: " << errno);
            exit(0);
            return -1;
        }
        if(sel > 0 && FD_ISSET(_port, &fds))
        {
            int shot = ::read(_port, buf+nbytes, size-nbytes);
            if(shot > 0)
            {
                nbytes+=shot;
                continue;
            }
        }
        break;
    }while(nbytes < size);
    buf[nbytes] = 0;

	if(nbytes>0) on_event(eREAD, buf, nbytes);

    return nbytes;
}

size_t DvSerial::bread(uint8_t *buf, int size, int to)
{
    int     nbytes = 0;
    struct  timeval tv;
    fd_set  fds;

    do{
        FD_ZERO(&fds);
        FD_SET(_port, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = (to * 1000)+DEF_TOUT;
        int sel = ::select(_port+1, &fds, NULL, NULL, &tv);
        if(sel < 0)
        {
            LOGE("::select error: " << errno);
            return -1;
        }
        if(sel > 0 && FD_ISSET(_port, &fds))
        {
            int shot = ::read(_port, buf+nbytes, size-nbytes);
            if(shot > 0)
            {
                nbytes+=shot;
                continue;
            }
        }
        break;
    }while(nbytes < size);

    buf[nbytes] = 0;
	if(nbytes){
        LOGD3(__PRETTY_FUNCTION__ << " >" << (const char*)buf);
		on_event(eREAD, buf, nbytes);
	}

    return nbytes;
}

int DvSerial::bwrite(const uint8_t *buf, int size, int to)
{
    int sent  = 0;
    if(size<0)
    {
        return _com_setting((const char*)buf,to);
    }
    if(size)
    {
        time_t ct = ::tick_count() + to + 16;
        int left  = size;

        do{
            int n = ::write(_port, buf+sent, left);
            if(n < 0)
            {
                if(errno == EAGAIN)
                {
                    LOGE("... port send EAGAIN");
                    ::msleep(1);
                    continue;
                }
                else
                {
                    LOGE("... port send error. Trying to reopen ");
                    return -1;
                }
            }
            if( n > 0)
            {
                left -= n;
                sent += n;
            }
        }while(left>0 && ::tick_count() < ct);
    }
    LOGD3(__PRETTY_FUNCTION__ << "< " << (const char*)buf);

    if(sent>0){
        on_event(eWRITE, buf, sent);
    }
	return sent;
}

void DvSerial::iclose()
{
    if(_port>0)
    {
        this->flush();
        ::msleep(0xff);
        int status;
        if(ioctl(_port, TIOCMGET, &status) == -1)
        {
            perror("unable to read _portstatus Closing");
        }
        else
        {
            status &= ~TIOCM_DTR;    /* turn off DTR */
            status &= ~TIOCM_RTS;    /* turn off RTS */
            if(ioctl(_port, TIOCMSET, &status) == -1)
            {
                perror("unable to set _portstatus");
            }
        }
        tcsetattr(_port, TCSANOW, &_oldTCSett);
        ::close(_port);
        flock(_port, LOCK_UN);  /* free the _port so that others can use it. */
    }
    _port = 0;
}

/*
Constant  Description
TIOCM_LE        DSR (data set ready/line enable)
TIOCM_DTR       DTR (data terminal ready)
TIOCM_RTS       RTS (request to send)
TIOCM_ST        Secondary TXD (transmit)
TIOCM_SR        Secondary RXD (receive)
TIOCM_CTS       CTS (clear to send)
TIOCM_CAR       DCD (data carrier detect)
TIOCM_CD        see TIOCM_CAR
TIOCM_RNG       RNG (ring)
TIOCM_RI        see TIOCM_RNG
TIOCM_DSR       DSR (data set ready)

http://man7.org/linux/man-pages/man4/tty_ioctl.4.html
*/

int DvSerial::_is_dcd_enabled()
{
    int status;

    ioctl(_port, TIOCMGET, &status);
    if(status&TIOCM_CAR) return(1);
    else return(0);
}


int DvSerial::_is_cts_enabled()
{
    int status;

    ioctl(_port, TIOCMGET, &status);
    if(status&TIOCM_CTS) return(1);
    else return(0);
}


int DvSerial::_is_dsr_enabled()
{
    int status;

    ioctl(_port, TIOCMGET, &status);
    if(status&TIOCM_DSR) return(1);
    else return(0);
}


void DvSerial::_enable_dtr()
{
    int status;

    if(ioctl(_port, TIOCMGET, &status) == -1)
    {
        perror("unable to read _portstatus 3");
    }
    status |= TIOCM_DTR;    /* turn on DTR */
    if(ioctl(_port, TIOCMSET, &status) == -1)
    {
        perror("unable to set _portstatus");
    }
}


void DvSerial::_disable_dtr()
{
    int status;

    if(ioctl(_port, TIOCMGET, &status) == -1)
    {
        perror("unable to read _portstatus 4");
    }
    status &= ~TIOCM_DTR;    /* turn off DTR */
    if(ioctl(_port, TIOCMSET, &status) == -1)
    {
        perror("unable to set _portstatus");
    }
}


void DvSerial::_enable_rts()
{
    int status;

    if(ioctl(_port, TIOCMGET, &status) == -1)
    {
        perror("unable to read _portstatus 5");
    }
    status |= TIOCM_RTS;    /* turn on RTS */
    if(ioctl(_port, TIOCMSET, &status) == -1)
    {
        perror("unable to set _portstatus");
    }
}


void DvSerial::_disable_rts()
{
    int status;

    if(ioctl(_port, TIOCMGET, &status) == -1)
    {
        perror("unable to read _portstatus 6");
    }
    status &= ~TIOCM_RTS;    /* turn off RTS */
    if(ioctl(_port, TIOCMSET, &status) == -1)
    {
        perror("unable to set _portstatus");
    }
}

void DvSerial::flush()
{
    if(_port>0)
        tcflush(_port, TCIOFLUSH);
}

int DvSerial::_com_setting(const char* cmd, int how)
{
    if(!::strcmp(cmd,"RTS"))
    {
        if(how==0)
        {
            this->_disable_rts();
            return 0;
        }
        else if(how==1)
        {
            this->_enable_rts();
            return 0;
        }
        return -1;
    }
    else if(!::strcmp(cmd,"DTR"))
    {
        if(how==0)
        {
            this->_disable_dtr();
            return 0;
        }
        else if(how==1)
        {
            this->_enable_dtr();
            return 0;
        }
        return -1;
    }
    else if(!::strcmp(cmd,"DSR"))
    {
        return this->_is_dcd_enabled();
    }
    else if(!::strcmp(cmd,"DCD"))
    {
        return this->_is_dcd_enabled();
    }
    else if(!::strcmp(cmd,"CTS"))
    {
        return this->_is_cts_enabled();
    }
    return -1;
}
#endif

};//namespace


#endif //DSERIAL_CPP
