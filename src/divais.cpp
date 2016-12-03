
#include <iostream>
#include <regex>
#include <stdlib.h>
#include "main.h"
#include "divais.h"
#include "usb.h"
#include "pgpio.h"
/*
#include "ppwm.h"
#include "pi2c.h"
#include "pspi.h"
#include "padio.h"
*/
#include "serialcom.h"
#ifdef __linux
#   include <dlfcn.h>
#else
#   include <windows.h>
#endif
/*
#include "sqvm.h"
#include "sqobject.h"
#include "sqfuncproto.h"
*/

#define SOMESEC         10000
#define SOMEMSECS       8
#define TRIMEXPECT      1024
#define RECBUFF         1024
#define READMAX         1024

static Sqrat::Function NullFunc;

inline bool wait_to(size_t expire, int future)
{
    if(RunCtx::_Alive)
    {
        IDLE_THREAD();
        if(future==-1)
        {
            return true;
        }
        return ::tick_count() < expire;
    }
    return false;
}

inline bool wait_to2(size_t expire, int future)
{
    if(RunCtx::_Alive)
    {
        IDLE_THREAD();
        if(future==-1)
        {
            return true;
        }
        return ::tick_count() < expire;
    }
    return false;
}

Device::Device():_tout(600),
                _binary(IDevice::eTEXT),
                _cancall(false),
                _priority(64),
                _eol("\r\n"),
                _plugin(0),
                _sent(0),
                _useptr(false),
                _autoflush(true)
{
    _pc = 0;
}

Device::Device(const char* cred, const char* name, int mode):_pc(0),_ended(false),
                                  _tout(600),
                                  _binary(mode),
                                  _cancall(false),
                                  _priority(64),
                                  _eol("\r\n"),
                                  _plugin(0),
                                  _sent(0),
                                  _useptr(false),
                                  _autoflush(true)
{
    if(name[0]=='0' || !::strncmp(name,"(null",5))
    {
        char fakename[32];
        ::sprintf(fakename,"%p", this);
        _name = fakename;
        _useptr = true;
    }
    else
        _name = name;
    ::strcpy(_cred,cred);
    open();
    LOGDETAIL("Creating device " << _name);
}

Device::~Device()
{
    LOGDETAIL(" Destroying device " << _name);

    RunCtx::_MsgQue.remove(this);
    if(_pc)
    {
        _pc->close();
    }
    if(!this->is_stopped())
    {
        this->signal_to_stop();
        ::msleep(1024);
        this->stop_thread();
    }
    while(!_ended ) //hack hack.. to investigate
        ::msleep(256);
    if(_plugin){
        if(_pc)
            _plugin->_so_release(_pc);
        delete _plugin;
    }
    else
        delete _pc;


}

/**
 * @brief Device::_onClosed
 */
void Device::_onClosed()
{
    this->routeMessage(IDevice::eCLOSED);
    LOGDETAIL("Device " << _name << " Is closed");
}

/**
 * @brief Device::_onReceived
 * @param pdata
 * @param len
 */
void Device::_onReceived(cbyte* pdata, int len)
{
    this->routeMessage(_binary, pdata, len);
    if(_binary == IDevice::eBINARY){
        if(RunCtx::_ShellOn)
        {
            std::cout << RunCtx::toString(pdata, len) << "\n";
        }
        {LOGREC(_name <<" -> " << RunCtx::toString(pdata, len) );}
    }
    else{
        if(RunCtx::_ShellOn)
        {
            std::cout << (const char*)pdata << "\n";
        }
        {LOGREC(_name <<" -> " << (const char*)pdata );}
    }
}


/**
 * @brief Device::devEvent
 * @param pc
 * @param ev
 * @param pdata
 * @param len
 * @return
 */
int  Device::devEvent(IComm* pc, D_MSG ev, cbyte* pdata, int len)
{
    if(ev==IDevice::eTEXT || ev==IDevice::eBINARY )
    {
        if(_binary == IDevice::eBINARY)
            {LOGDETAIL(_name <<" devEvent " << RunCtx::toString(pdata, len));}
        else
            {LOGDETAIL(_name <<" devEvent " << (const char*)pdata);}
    }
    return this->routeMessage(ev, pdata, len);
}

/**
 * @brief Device::isopen
 * @return
 */
bool Device::isopen()
{
    return _pc && _pc->isOpen();
}

/**
 * @brief Device::escape, xustom  "DTS",0/1
 * @param esc
 * @param how
 * @return
 */
int Device::escape(const char* esc, int how)
{
    if(_pc && _pc->isOpen())
        return _pc->esc(esc,  how);
    return -1;
}


/**
 * @brief Device::putctrl
 * @param key
 * @return
 */
int Device::putctrl(char key)
{
    uint8_t buf[2]= {0};

    _warnonSend();
    buf[0] = key-'A'+1;
    _io.lockDevWrite(buf,1);
    _waitCompletion();
    return 0;
}

/**
 * @brief Device::settimeout
 * @param to
 * @return
 */
int Device::settimeout(int to, int selto)
{
    _tout = to;
    _pc->setTout(to,selto);
    return 0;
}

/**
 * @brief Device::seteol
 * @param cmd
 * @return
 */
int Device::seteol(const char* cmd)
{
    _eol = cmd;
    return 0;
}

/**
 * @brief Device::putsnl
 * @param cmd
 * @return
 */
int Device::putsnl(const char* cmd)
{
    bstring b;

    _warnonSend();
    b.append((cbyte*)cmd, ::strlen(cmd));
    b.append((cbyte*)_eol.data(), _eol.length());
    _io.lockDevWrite(b);
    _waitCompletion();
    return 0;
}

/**
 * @brief Device::putsml
 * @param mlines
 * @return
 */
int Device::putsml(const char* mlines)
{
    std::stringstream    s(mlines);
    std::string          line;

    _warnonSend();
    while(getlinecrlf(s,line))
    {
        if(line[0]=='%')            // send key
            sendkey(line.c_str()+1);
        else if(line[0]=='^')       // send Ctrl Key
            putctrl(line[1]);
        else
            putsnl(line.c_str());
    }
    _waitCompletion();
    return 0;
}


int Device::sendkey(const char* key)
{
    bstring bin = RunCtx::toBinary(key);
    _warnonSend();
    _io.lockDevWrite(bin);
    _waitCompletion();
    return 0;

}

int Device::sendkeyexpect(const char* key, const char* expect)
{
    bstring bin = RunCtx::toBinary(key);
    _warnonSend();
    _io.lockDevWrite(bin);
    _waitCompletion();
    return strexpect(expect);
}

int Device::sendkeyexpect(const char* key, const char* expect, int to)
{
    bstring bin = RunCtx::toBinary(key);
    _warnonSend();
    _io.lockDevWrite(bin);
    _waitCompletion();
    return strexpect(expect, to);
}

int Device::sendkeycb(const char* key, Sqrat::Function f)
{
    bstring bin = RunCtx::toBinary(key);
    _warnonSend();
    _callback = f;
    _io.lockDevWrite(bin);
    int rv = RunCtx::loopit();
    _callback = NullFunc;
    return rv;
}

/**
 * @brief Device::setspeed, bytes apart to be sent in device implementation
 * @param apart
 */
void Device::setspeed(int apart)
{
    _pc->setspeed(apart);
}

/**
 * @brief Device::puts, a string
 * @param cmd
 * @return
 */
int Device::puts(const char* cmd)
{
    _warnonSend();
    _io.lockDevWrite((uint8_t*)cmd, ::strlen(cmd));
    _waitCompletion();
    return 0;
}

int Device::_waitCompletion()
{
    size_t fut = ::tick_count() + 30000;
    while(wait_to(fut,30000) && _io.dataToDev())
    {
        ::msleep(1000);
        LOGDETAIL2("waiting to send data to device \n");
    }
    if(_io.dataToDev())
    {
        LOGERR("Sending to device timeout. Device thread blocked/dead ?");
        exit(0);
    }
    return 0;
}

/**
 * @brief Device::strexpect
 * @param expr
 * @return
 */
int Device::strexpect(const char* expr)
{
    return strexpect(expr, _tout);
}

/**
 * @brief Device::flushInput
 */
void Device::flushin()
{
   _io.copyAllFromDev();
}

/**
 * @brief Device::strexpect, does not wipes the received data
 * @param expr
 * @param to
 * @return
 */
int Device::strexpect(const char* expr, int to)
{
    size_t      expire = ::tick_count() + size_t(to);
    std::string accum;

    while(wait_to(expire, to))
    {
        if(_io.fromDevDataLen())
        {
            bstring buf = _io.copyAllFromDev();

            accum.clear();
            accum.append((const char*)buf.data());
            if(expr[0] == '?')
            {
                std::regex self_regex(expr+1,
                                      std::regex_constants::ECMAScript |
                                      std::regex_constants::icase);
                if (std::regex_search(accum, self_regex))
                {
                    LOGDETAIL(__FUNCTION__<<" regular expression found ");
                    return 0;
                }
            }
            else
            {
                if(accum.find(expr) != std::string::npos)
                {
                    LOGDETAIL(__FUNCTION__<<" string found ");
                    return 0;
                }
            }
            if(accum.length() > TRIMEXPECT)
            {
                LOGWARN(__FUNCTION__ << " trimming recieved buffer because exceeded 1K")
                std::string trunc = accum.substr(accum.length()-::strlen(expr));
                accum = trunc;
            }
        }
    }
    LOGDETAIL (__FUNCTION__ << " Timeout");
    return 1;
}


const char* Device::getsmin(int chars)
{
    return getsmin(chars,_tout);
}

const char* Device::getsmin(int chars,int to)
{
    size_t      ammnt  = 0;
    size_t      expire = ::tick_count() + size_t(to);
    bool        got = false;

    while(wait_to(expire, to))
    {
        ammnt = _io.fromDevDataLen();
        if(ammnt>=(size_t)chars)
        {
            LOGDETAIL (__FUNCTION__ << " got " << ammnt << " bytes");
            got = true;
            break;
        }
    }
    if(false == got)
        LOGDETAIL (__FUNCTION__ << " timeout, returning "<<ammnt<<" bytes");
    return this->gets(to);
}


/**
 * @brief Device::readmin
 * @param bytes
 * @return
 */
Sqrat::Array Device::readmin(int bytes)
{
    return readmin(bytes, _tout);
}

/**
 * @brief Device::readmin, wait max bytes and return the received buffer
 * @param bytes
 * @param to
 * @return
 */
Sqrat::Array Device::readmin(int bytes, int to)
{
    size_t      ammnt  = 0;
    size_t      expire = ::tick_count() + size_t(to);
    bool        got = false;

    while(wait_to(expire, to))
    {
        ammnt = _io.fromDevDataLen();
        if(ammnt>=(size_t)bytes)
        {
            LOGDETAIL (__FUNCTION__ << " got " << ammnt << " bytes");
            got = true;
            break;
        }
    }
    if(false == got)
        LOGDETAIL (__FUNCTION__ << " timeout, returning "<<ammnt<<" bytes");
    return this->read(to);
}

/**
 * @brief Device::binexpect
 * @param a
 * @param to
 * @return
 */
int Device::binexpect(Sqrat::Array a, int to)
{
    int        sz = a.GetSize();
    uint8_t    ptr[256];
    bstring    accum;
    size_t     expire = ::tick_count() + size_t(to);
    a.GetArray(ptr, sz);

    while(wait_to(expire, to))
    {
        accum.clear();
        accum.append(_io.copyAllFromDev());
        if(!accum.empty())
        {
            if(accum.find(ptr, 0, sz) != bstring::npos)
            {
                LOGDETAIL (__FUNCTION__ << " found expected array");
                return 0;
            }
        }
        if(accum.length() > TRIMEXPECT)
        {
            bstring trunc = accum.substr(accum.length()-sz);
            accum = trunc;
            LOGDETAIL (__FUNCTION__ << " trimming output due 1K overflow");
        }
    }
    LOGDETAIL (__FUNCTION__ << " timeout");
    return 1;
}

/**
 * @brief Device::binexpect
 * @param a
 * @return
 */
int Device::binexpect(Sqrat::Array a)
{
    return binexpect(a, _tout);
}

/**
 * @brief Device::expectany
 * @return any input
 */
int Device::expectany()
{
    return expectany(_tout);
}

/**
 * @brief Device::expectany, does not empty the received buffer
 * @param to
 * @return
 */
int Device::expectany(int to)
{
    size_t  now = ::tick_count() ;
    size_t  expire = now + size_t(to);
    int     rv = 1;

    while(wait_to(expire, to))
    {
        size_t tin = _io.fromDevDataLen();
        if(tin)
        {
            rv = 0;
            LOGDETAIL (__FUNCTION__ << " got "<<tin<<" bytes");
            break;
        }
    }
    if(rv)
    {
        LOGDETAIL (__FUNCTION__ << " timeout");
    }
    return rv;
}

/**
 * @brief Device::setdebug
 * @param l
 */
void Device::setdebug(int l)
{
    if(_pc)
        _pc->debug(l);
}

/**
 * @brief Device::read
 * @return
 */
Sqrat::Array Device::read()
{
    return read(_tout);
}

/**
 * @brief Device::sent, returns what the thread flushed to device
 * @return
 */
int Device::sent()
{
    int sent = _sent;
    _sent = 0;
    return sent;
}

/**
 * @brief Device::read
 * @param to
 * @return
 */
Sqrat::Array Device::read(int to)
{
    bstring in;
    size_t fut = ::tick_count() + to;

    while(_io.fromDevDataLen()==0 && wait_to(fut,to));
    in = _io.getAllFromDev();
    Sqrat::Array ra(RunCtx::_Penv->theVM(), in.size());
    for(size_t idx = 0 ; idx < in.size(); idx++)
    {
        ra.SetValue(idx, uint8_t(in.at(idx)));
    }
    LOGREC(RunCtx::toString((cbyte*)in.data(), in.length()));
    return ra;
}

/**
 * @brief Device::gets
 * @return
 */
const char* Device::gets()
{
    return Device::gets(_tout);
}

/**
 * @brief Device::gets
 * @param to
 * @return
 */
const char* Device::gets(int to)
{
    size_t fut = ::tick_count() + to;

    while(_io.fromDevDataLen()==0 && wait_to(fut,to));
    _boutret.clear();
    if(_io.fromDevDataLen())
        _boutret = _io.getAllFromDev();
    return (const char*)_boutret.data();
}

/**
 * @brief Device::binwrite
 * @param p
 * @param l
 * @return
 */
int Device::binwrite(cbyte* p, int l)
{
    bstring  binstr;
    binstr.append(p,l);
    _io.lockDevWrite(binstr);
    this->_waitCompletion();
    return 0;
}

/**
 * @brief Device::write
 * @param a
 * @return
 */
int Device::write(Sqrat::Array a)
{
    int     sz = a.GetSize();
    uint8_t ptr[1024];

    a.GetArray(ptr, sz);
    bstring  binstr;
    binstr.append(ptr,sz);
    _io.lockDevWrite(binstr);
    this->_waitCompletion();
    return 0;
}

/**
 * @brief Device::name
 * @return
 */
const char* Device::name()const
{
    if(_pc)
        return _pc->name();
    return "";
}

/**
 * @brief Device::writeread
 * @param a
 * @return
 */
Sqrat::Array Device::writeread(Sqrat::Array a)
{
    return writeread(a, _tout);
}

/**
 * @brief Device::writeexpect
 * @param a
 * @param e
 * @return
 */
int Device::writeexpect(Sqrat::Array a, Sqrat::Array e)
{
    write(a);
    return binexpect(e);
}

/**
 * @brief Device::writeexpect
 * @param a
 * @param e
 * @param to
 * @return
 */
int Device::writeexpect(Sqrat::Array a, Sqrat::Array e, int to)
{
    write(a);
    return binexpect(e, to);
}

/**
 * @brief Device::putsexpect
 * @param p
 * @param e
 * @return
 */
int Device::putsexpect(const char* p, const char* e)
{
    puts(p);
    return strexpect(e);
}

/**
 * @brief Device::putsexpect
 * @param p
 * @param e
 * @param to
 * @return
 */
int Device::putsexpect(const char* p, const char* e, int to)
{
    puts(p);
    return strexpect(e, to);
}

/**
 * @brief Device::putsnlexpect
 * @param p
 * @param e
 * @return
 */
int Device::putsnlexpect(const char* p, const char* e)
{
    putsnl(p);
    return strexpect(e);
}

/**
 * @brief Device::putsnlexpect
 * @param p
 * @param e
 * @param to
 * @return
 */
int Device::putsnlexpect(const char* p, const char* e, int to)
{
    putsnl(p);
    return strexpect(e, to);
}

/**
 * @brief Device::writeread
 * @param a
 * @param to
 * @return
 */
Sqrat::Array Device::writeread(Sqrat::Array a, int to)
{
    this->write(a);
    return this->read(to);
}

/**
 * @brief Device::putsgets
 * @param s
 * @return
 */
const char* Device::putsgets(const char* s)
{
    return putsgets(s, _tout);
}

/**
 * @brief Device::putsgets
 * @param s
 * @param to
 * @return
 */
const char* Device::putsgets(const char* s, int to)
{
    this->puts(s);
    return this->gets(to);
}

/**
 * @brief Device::putsnlgets
 * @param s
 * @return
 */
const char* Device::putsnlgets(const char* s)
{
    return putsgets(s, _tout);
}

/**
 * @brief Device::putsnlgets
 * @param s
 * @param to
 * @return
 */
const char* Device::putsnlgets(const char* s, int to)
{
    this->putsnl(s);
    return this->gets(to);
}

/**
 * @brief Device::noout
 * expect until for to there is no activity
 * @param to
 * @return
 */
int Device::issteady(int atleast)
{
    size_t nsz = _io.fromDevDataLen();
    do{
        ::msleep(atleast);
        if(_io.fromDevDataLen() == nsz)
        {
            break;
        }
        nsz = _io.fromDevDataLen();
    }while(1);
    return 0;
}

void Device::printout(int to)
{
    size_t fut = ::tick_count() + to;

    while(wait_to(fut,to))
    {
        if(_io.fromDevDataLen())
        {
            _boutret = _io.getAllFromDev();
            std::cout << (const char*)_boutret.c_str() << std::flush;
            fut = ::tick_count() + to;
        }
    }
}

void Device::setpriority(int pr)
{
    _priority = pr;
}

void    Device::addDev(IComm* pc)
{
    RunCtx::_Devices.insert(this);
}

/**
 * @brief Device::remDev
 * @param pc
 */
void    Device::remDev(IComm* pc)
{
    (void)(pc);
    RunCtx::_Devices.erase(this);
}

void Device::autoflush(bool a)
{
    _waitCompletion();
    _autoflush=a;
}

/**
 * @brief Device::flush
 */
void Device::flush()
{
    _io.reset();
    _pc->flush();
}

/**
 * @brief Device::moveData
 * @param pb
 * @param len
 * @return
 */
size_t  Device::moveData(uint8_t* pb, size_t& len)
{
    bstring r = _io.getAllFromDev();
    if(r.length())
    {
        if(len < r.length())
        {
            LOGERR(__FUNCTION__ << " buffer to small");
        }
        ::memcpy(pb, r.data(), std::min(r.length(), len));
    }
    return r.length();
}

/**
 * @brief Device::reconnect
 * @param to
 * @return
 */
int Device::reconnect(int to)
{
    if(_pc)
    {
        size_t fut = ::tick_count() + to;
        while(wait_to(fut,to))
        {
            if(_pc->isOpen())
                _pc->close();
            if(_pc->open() == 0)
            {
                return 0;
            }
            ::msleep(512);
        }
        return 1;
    }
    return -1;
}

/**
 * @brief Device::_createInstance, loads so and calls into
 * @param cred
 * @param name
 * @return
 */
IComm*  Device::_createInstance(const char* cred, const char* name)
{
    std::string cons = cred;
    size_t colum = cons.find(':');
    std::string sub = cons.substr(0,colum);
    std::string args = cons.substr(colum+3);


    std::string fullfile;
#ifdef __linux
    fullfile = "plugins/" + sub + "/lib";
    fullfile += sub;

    fullfile += ".so";
    SoHandler h = ::dlopen(fullfile.c_str(), RTLD_LAZY);
    if(h==0)
    {
        LOGERR("Cannot load:" << fullfile << ": " <<::dlerror());

        fullfile = "lib";
        fullfile += sub;
        fullfile += ".so";
        h = ::dlopen(fullfile.c_str(), RTLD_LAZY);
    }
#else
    fullfile += sub;
#ifdef DEBUG
    fullfile += "D";
#endif
    fullfile += ".dll";
    SoHandler h = LoadLibrary(fullfile.c_str());
#endif
    if(h)
    {
        _plugin = new SoEntry(h);
        return _plugin->_so_get(this, args.c_str(), name, 0);
    }
    else
    {
#ifdef __linux
        LOGERR("Cannot load:" << fullfile << ": " <<::dlerror());
#else
        LOGERR("Cannot load:" << fullfile << ": "<< GetLastError());
#endif
    }
    return 0;
}

/**
 * @brief Device::setbuff
 * @param sz
 * @return
 */
int Device::setbuff(int sz)
{
    _io.setbuff(sz);
    return 0;
}

/**
 * @brief Device::setoptions
 * @param c
 * @return
 */
int Device::setoptions(const char* c)
{
    if(_pc)
        return _pc->setOptions(c);
    return 1;
}

/**
 * @brief Device::close
 * @return
 */
int Device::close()
{
    if(_pc)
    {
        _pc->close();
        stop_thread();
        LOGERR("Thread" << _name << "stopped");

        if(_plugin)
        {
            if(_pc)
                _plugin->_so_release(_pc);
        }
        else
            delete _pc;
        _pc = 0;
    }
    return 0;
}

/**
 * @brief Device::open
 * @return
 */
int Device::open()
{
    if(_pc)
    {
        LOGERR("Device is open. Close the decie first");
        return 1;
    }

    if(!strncmp(_cred,"tty://",6))
    {
        _pc = new SerialComm(this, _cred + 6, _name.c_str());
        _pc->debug(_Debug);
        _cancall = false;
        if(this->start_thread())
        {
            LOGERR("Cannot start thread. Errno: "<< errno << std::endl);
            ::exit(errno);
        }
        while(_cancall==false)
        {
            ::msleep(128);
        }
        return 0;
    }
    else if(!strncmp(_cred,"susb://",7))
    {
        _pc = new PUsb(this, _cred + 7, _name.c_str());
        _pc->debug(_Debug);
        _cancall = false;
        if(this->start_thread())
        {
            LOGERR("Cannot start thread. Errno: "<< errno << std::endl);
            ::exit(errno);
        }
        while(_cancall==false)
        {
            ::msleep(128);
        }
        return 0;
    }

    if(!strncmp(_cred,"gpio://",7))
    {

    }
    if(!strncmp(_cred,"pwmo://",7))
    {

    }
    if(!strncmp(_cred,"adio://",7))
    {

    }
    if(!strncmp(_cred,"spio://",7))
    {

    }
    if(!strncmp(_cred,"i2cio://",8))
    {

    }

    _pc = _createInstance(_cred, _name.c_str());
    if(_pc)
    {
        _pc->debug(_Debug);
        _cancall = false;
        if(this->start_thread())
        {
            LOGERR("Cannot start thread. Errno: "<< errno << std::endl);
            ::exit(errno);
        }
        while(_cancall==false)
        {
            ::msleep(128);
        }
        return 0;
    }
    static const char NoDeviceStr[] = " No device plugin was found";
    RunCtx::callScript(this, eERROR, NoDeviceStr);
    throw Sqrat::Exception("Cannot load the device");
    return 1;
}

/**
 * @brief Device::discardDevData
 * @param id
 * @param how
 */
void Device::discardDevData(const std::vector<size_t>& ids, int how)
{
    if(how==1)
    {
        this->_io.removefromDevq(ids);
    }
    this->_io.trim();
}

/**
 * @brief IoData::accumDevData
 * @param msg
 */
void IoData::accumDevData(const CntPtr<DevMsg>& msg)
{
    AutoLock l(&_tmut);
    _fromdevq.push_back(msg);
    _fromdevlen += msg->_data.length();
    if(_fromdevlen > _recsz)
    {
        LOGWARN("Trimmming input due was not read or discarded in loop()");
        while(_fromdevlen > _recsz)
        {
            CntPtr<DevMsg> msg(_fromdevq.front());

            _fromdevlen -= _fromdevq.front().ptr()->_data.length();
            _fromdevq.pop_front();
            ((Device*)msg->_pc)->routeMessage(IDevice::eDISCARD, msg->_data.data(), msg->_data.length());
        }
    }
    _timein = ::tick_count();
}

int Device::testcb(Sqrat::Function f)
{
    f.Evaluate<int>(" ===== parameter from testcb ==== \n");
    return 0;
}

/**
 * @brief Device::putscb
 * @param s
 * @param f
 * @return
 */
int Device::putscb(const char* s, Sqrat::Function f)
{
    _warnonSend();
    _callback = f;
    this->putsml(s);
    int rv = RunCtx::loopit();
    _callback = NullFunc;
    return rv;
}

/**
 * @brief Device::putsnlcb
 * @param s
 * @param f
 * @return
 */


int Device::putsnlcb(const char* s, Sqrat::Function f)
{
    _warnonSend();
/*
    HSQOBJECT ho = f.GetFunc();
    SQFunctionProto* pp = (SQFunctionProto*)&ho._unVal;
    SQObjectPtr pc =
    const char* pn = (const char*)pc._unVal.;
*/
    _callback = f;
    this->putsnl(s);
    int rv = RunCtx::loopit();
    _callback = NullFunc;
    return rv;
}

/**
 * @brief Device::writecb
 * @param a
 * @param f
 * @return
 */
int Device::writecb(Sqrat::Array a, Sqrat::Function f)
{
    _warnonSend();
    _callback = f;
    this->write(a);

    int rv = RunCtx::loopit();
    _callback = NullFunc;
    return rv;
}

/**
 * @brief Device::putsmlcb
 * @param s
 * @param f
 * @return
 */
int Device::putsmlcb(const char* s, Sqrat::Function f)
{
    _warnonSend();
    _callback = f;
    this->putsml(s);
    int rv = RunCtx::loopit();
    _callback = NullFunc;
    return rv;
}

/**
 * @brief Device::waitOnline
 * @param tos
 * @return
 */
int Device::waitOnline(int to)
{
    size_t expire = ::tick_count() + (size_t)(to*1000);
    while(wait_to(expire, to) && _pc->isOpen()==false)
    {
        ::msleep(256);
        LOGDETAIL(__FUNCTION__ << "Waiting for device to be open");
    }
    return _pc->isOpen()==true ? 0 :1;
}

/**
 * @brief Device::routeMessage
 * @param msg
 * @param pdata
 * @param len
 * @return
 */
int  Device::routeMessage(int msg, const uint8_t* pdata, size_t len)
{
    int             rv = 0;
    CntPtr<DevMsg>  smsg(new DevMsg(this, msg, pdata, len));

    if(!_callback.IsNull())
        smsg->setFunc(_callback);

    if(msg==IDevice::eTEXT ||
       msg==IDevice::eBINARY )
    {
        _io.accumDevData(smsg);
    }

    if(RunCtx::_looPing || !_callback.IsNull())
    {
        PCTX->_MsgQue.push(smsg);
        IDLE_THREAD();
    }
    return rv;
}

/**
 * @brief Device::thread_main
 */
void Device::thread_main()
{
    int      rv = 0;
    int      lop=1;
    bstring  temp;
    unsigned char   shot[RECBUFF];
    bool     opened = false;

    RunCtx::addThreadSpin(_name,"*",1);

    while(!_bstop && RunCtx::_Alive)
    {
        if(_pc->isOpen() == false)
        {
            if(opened == true)
            {
                _onClosed();
                _cancall = false;
                opened = false;
            }
            _pc->open();
            ::msleep(1024);
            _cancall = true;
            continue;
        }
        else
        {
            if(opened ==false)
            {
                opened = true;
                this->routeMessage(IDevice::eOPENDED);
                LOGDETAIL("Device:" << _name << " is open");
            }
        }
        rv = 0;
        if(_io.dataToDev())
        {
            temp = _io.lockDevToSend();
            rv = _pc->sendTo(temp.data(), temp.length());
            if(rv == -1)
            {
                _pc->close();
                LOGERR("Send failed. Device might be closed!.");
            }
            else if(rv==0)
            {
                RunCtx::addThreadSpin(_name, "<", lop);
                lop=0;
                _sent = temp.length();

                std::string txt = RunCtx::toString(temp.data(), _sent);
                LOGSEND(_pc->name() << ", " << txt);

            }
        }
        int bytes = _pc->recFrom(shot,sizeof(shot)-1);
        if(bytes == -1)
        {
             LOGERR("Rec failed. Device might be closed!.");
            _pc->close();
            continue;
        }
        else if(bytes > 0)
        {
            shot[bytes] = 0;
            this->_onReceived(shot, bytes);
            RunCtx::addThreadSpin(_name, ">", lop);
            lop=0;
        }
        if(lop % 8 == 0)
            RunCtx::addThreadSpin(_name," ",lop);
        ++lop;
        IDLE_THREAD();
        ::msleep(RunCtx::_LoopSpin);
    }

    if(_pc->isOpen())
        _pc->close();
    _ended = true;
    RunCtx::addThreadSpin(_name,"*",0);
}

/**
 * @brief Device::_warnonSend
 */
void Device::_warnonSend()
{
    if(_Debug)
    {
        if(_io.fromDevDataLen() && _autoflush==false)
        {
            LOGWARN("There is data in the received buffer! Read it or flushin() it...");
        }
        else
            _io.reset();
    }
}

void Device::publish()
{
    _pc->publish();
}
