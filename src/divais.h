#ifndef SHELL_H
#define SHELL_H

#include <vector>
#include "sqwrap.h"
#include "icombase.h"
#include "main.h"
#include "plugins.h"
#include "iodata.h"

class Device : public OsThread, public IDevice
{
public:
    Device();
    Device(const char* cred, const char* name, int mode);
    ~Device();
    void thread_main();

    //
    // string/binary read functions
    //
    const char*  gets();
    const char*  gets(int to);
    Sqrat::Array read();
    Sqrat::Array read(int to);

    //
    // string write functions
    //
    int puts(const char*);                                  // puts
    const char* putsgets(const char* s);                    // puts gets
    const char* putsgets(const char* s, int);               // puts gets timout
    int putsexpect(const char* p, const char* e);           // puts expect
    int putsexpect(const char* p, const char* e, int to);   // puts expect to
    int putscb(const char*, Sqrat::Function f);             // puts cb
    void publish();
    int testcb(Sqrat::Function f);
    //
    // string write functions appends nl
    //
    int putsnl(const char*);
    const char* putsnlgets(const char* s);
    const char* putsnlgets(const char* s, int);
    int putsnlexpect(const char* p, const char* e);
    int putsnlexpect(const char* p, const char* e, int to);
    int putsnlcb(const char*, Sqrat::Function f);

    //
    // string write functions multiline strings
    //
    int putsml(const char*);
    int putsmlcb(const char*, Sqrat::Function f);

    //
    // binary array
    //
    int write(Sqrat::Array a);
    Sqrat::Array writeread(Sqrat::Array a);
    Sqrat::Array writeread(Sqrat::Array a, int);
    int writeexpect(Sqrat::Array a, Sqrat::Array b);
    int writeexpect(Sqrat::Array a, Sqrat::Array b, int to);
    int writecb(Sqrat::Array a, Sqrat::Function f);

    int putctrl(char);

    int strexpect(const char*,int to);      // expects to see char*
    int strexpect(const char*);             //  expects to see char*
    int binexpect(Sqrat::Array a, int to);  //  expects to see a
    int binexpect(Sqrat::Array a);          //  expects to see a

    Sqrat::Array readmin(int);              // wait for minimum int bytes/chars
    Sqrat::Array readmin(int,int);          // then returns the value
    const char* getsmin(int);
    const char* getsmin(int,int);

    int expectany(int to);                  // any activity on outout from device
    int expectany();

    int sendkey(const char* key);
    int sendkeycb(const char* key, Sqrat::Function f);
    int sendkeyexpect(const char* key, const char* expect, int to);
    int sendkeyexpect(const char* key, const char* expect);

    int settimeout(int tos, int selectto);
    void setspeed(int apart);
    int seteol(const char* cmd);
    int open();
    int issteady(int atleast);
    void shellio();
    const char* coin();
    void printout(int to);
    int setbuff(int sz);
    int mode()const{return _binary;}
    int setbinary(int t){_binary=t; return 0;}
    bool isopen();
    int close();
    void setpriority(int pr);
    void addDev(IComm* pc);
    void remDev(IComm* pc);
    int  devEvent(IComm*, IDevice::D_MSG, cbyte*, int);
    void flush();
    void flushin();
    int sent();
    void autoflush(bool );
    void setdebug(int l);
    int binwrite(cbyte*, int);
    size_t  moveData(uint8_t* pb, size_t& len);
    const char* name()const;
    int setoptions(const char*);
    int reconnect(int to);
    int waitOnline(int tos);
    void discardDevData(const std::vector<size_t>& ids, int how);
    int  routeMessage(int msg, const uint8_t* data=0, size_t len=0);
    bool useptr()const{return _useptr;}
    int escape(const char* , int);
    int removeFile();
    int createFile();
protected:
    void _onReceived(cbyte* data, int len);
    void _onClosed();
    bstring  _toBinary(const char* string);
    std::string _toString(cbyte* binary, int len);
    int _waitCompletion();
    int _notifyWaiting();
    void _warnonSend();
private:
    IComm*  _createInstance(const char*, const char*);

private:
    bstring     _boutret;
    bstring     _tmp;
    IComm*      _pc;
    bool        _ended;
    int         _tout;
    int         _binary;
    bool        _cancall;
    int         _priority;
    std::string _eol;
    std::string _name;
    IoData      _io;
    bool        _devdatateractiv;
    SoEntry*    _plugin;
    char        _cred[128];
    int         _sent;
    bool        _useptr;
    Sqrat::Function _callback;
    bool         _autoflush;
};

inline std::istream& getlinecrlf(std::istream& is, std::string& t)
{
    t.clear();
    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case EOF:
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}


#endif // SHELL_H
