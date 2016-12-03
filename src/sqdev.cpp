#include "main.h"
#include "sqdev.h"
#include "serialttty.h"

SqDev::SqDev(WireBus::BUS bus,
             const char* scrtype, // script user type
             const char* name,
             const char* values,
             Sqrat::Object o):IDevice(name,scrtype),_o(o)
{
    _bus = bus;
    PCTX->add(this);
    GLOGD("System adding:" << name);
    _datapoints=values;

}

SqDev::~SqDev()
{
    GLOGD("System removing:" << _name);
    _o.AddRef(); // sqrat bug
}

int     SqDev::pool(int flags)
{
    Sqrat::Function f = Sqrat::Function(_o, "pool");
    if(!f.IsNull())
    {
        try{
            Sqrat::SharedPtr<int>   srv;
            srv = f.Evaluate<int>(flags);
            return (*srv.Get());
        }
        catch(Sqrat::Exception ex)
        {
            __alive=RESTART;
            GLOGFATAL( __PRETTY_FUNCTION__<<": "<< ex.Message());
            GLOGFATAL(__sqerror);
            ::msleep(2);
        }
    }

    return -1;
}

void    SqDev::close()
{
    ;
}

bool    SqDev::open()
{
    return true;
}

int     SqDev::bus()const
{
    return 0;
}

const char* SqDev::getJson(const char* key, bool byusrid)
{
    //Sqrat::Object classobj = Sqrat::RootTable().GetSlot(_sqcls.c_str());

    _storage.clear();
    Sqrat::Function f = Sqrat::Function(_o, "getJson");
    if(!f.IsNull()){
        try{
            Sqrat::SharedPtr<std::string>   srv = f.Evaluate<std::string>(key,byusrid);
            _storage =  *srv.Get();
        }catch(Sqrat::Exception ex){
            __alive=RESTART;
            GLOGFATAL( __FUNCTION__ << ex.Message());
            GLOGFATAL(__sqerror);
            ::msleep(2);
        }
    }
   // GLOGE(_storage);
    return _storage.c_str();
}

int SqDev::setSval(const char* val)
{
    std::vector<std::string> kv;
    bblconfig::split(val,'=',kv);
    if(kv.size()==2)
    {
        Sqrat::Function f = Sqrat::Function(_o, "setSval");//classobj.GetSlot( "getJson" ).Cast<Sqrat::Function>();
        if(!f.IsNull())
        {
            try{
                Sqrat::SharedPtr<int>   srv;

                if( (kv[1].length()>1 && ::isdigit(kv[1][1])) ||  (kv[1].length()==1 && ::isdigit(kv[1][0])) )
                    srv = f.Evaluate<int>(kv[0].c_str(),::atol(kv[1].c_str()));
                else
                    srv = f.Evaluate<int>(kv[0].c_str(),kv[1].c_str());
                return (*srv.Get());
            }catch(Sqrat::Exception ex){
                GLOGFATAL( __FUNCTION__ << ex.Message());
                GLOGFATAL(__sqerror);
                return -1;
            }
        }
    }
    return 0;
}

const char* SqDev::getSval(const char* c)
{

    Sqrat::Function f = Sqrat::Function(_o, "getSval");
    _storage.clear();
    try{
        Sqrat::SharedPtr<char*>   srv;
        srv = f.Evaluate<char*>(c);
        _storage =  (*srv.Get());
    }catch(Sqrat::Exception ex){
        __alive=RESTART;
        GLOGE( __FUNCTION__ << ex.Message());
        ::msleep(2);
    }
    return _storage.c_str();
}

int SqDev::get_state()
{
    return 0;
}

int SqDev::set_state(int state)
{
    return 0;
}

