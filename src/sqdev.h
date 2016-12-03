#ifndef SQDEV_H
#define SQDEV_H

#include "main.h"
#include "busiface.h"
#include "buspwm.h"

class SerialTtty;
/**
 * @brief The SqDev class
 * base class for scripted device classes
 */
class SqDev : public IDevice
{
public:
    SqDev(WireBus::BUS b,const char* scrtype, const char* name, const char* values, Sqrat::Object o);
    virtual ~SqDev();
    int     pool(int flags);
    void    close();
    bool    open();
    int     bus()const;
    int     get_state();
    const char* getname(){return this->name().c_str();}
    int     set_state(int state);
    int     init(const char* ukey){
        return IDevice::_setinit(ukey);
    }
    int     set_access(int a){return IDevice::_set_access(a);}
    int     online(int o){return IDevice::_setonline(o);}
    int     getbus()const{return _bus;}

    static void expose(SqEnv& e){
        Sqrat::Class<SqDev> cls(e.theVM(), _SC("SqDev"));
        cls.Ctor<WireBus::BUS, const char*, const char*, const char*, Sqrat::Object >();
        cls.Func(_SC("init"), &SqDev::init);
        cls.Func(_SC("open"), &SqDev::open);
        cls.Func(_SC("close"), &SqDev::close);
        cls.Func(_SC("name"), &SqDev::getname);
        cls.Func(_SC("pool"), &SqDev::pool);
        cls.Func(_SC("bus"), &SqDev::getbus);
        cls.Func(_SC("getJson"), &SqDev::getJson);
        cls.Func(_SC("getSval"), &SqDev::getSval);
        cls.Func(_SC("setSval"), &SqDev::setSval);
        cls.Func(_SC("expose"), &IDevice::webexpose);
        Sqrat::RootTable().Bind(_SC("SqDev"), cls);
    }
    const char* getJson(const char* key, bool byusrkey);
    const char* getSval(const char* c);
    int setSval(const char* val);
private:
    std::string     _storage;
    std::string     _sqcls;
    Sqrat::Object   _o;
    int            _bus;
};

#endif // SQDEV_H
