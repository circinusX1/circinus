#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <dlfcn.h>
#include "inst.h"
#include "database.h"
#include "restsrv.h"
#include "apis.h"
#include "swdog.h"
#include "sqratTable.h"
#include "sqratObject.h"
#include "sqratimport.h"
#include "sqwrap.h"
#include "sunset.h"
#include "filedev.h"
#include "i2cdev.h"
#include "dadc.h"
#include "pwmdev.h"
#include "gpiodev.h"
#include "spidev.h"
#include "uartdev.h"
#include "usbdev.h"
#include "adcdev.h"
#include "solib.h"
#include "divais.h"
#include "sunset.h"
#include "scrbase.h"
#include "rawsock.h"
#include "comcurl.h"
#include "comssh.h"
#include "inputsys.h"
#include "inst.h"


int         Secret;
int         ThrLock;
int         Loggran = 0x1F;
int         Wdto;
size_t      OTime;
size_t      Locks;
uint64_t    HddRoom;
pthread_t   MainThrId;
std::string Program;
std::string EmbVersion;
std::string Arhic;
std::string OutStr;
std::mutex  MuxLog;
Database*   Pdb;
RestSrv*    PSockSrv;
CurrTime    Clk;
#ifdef DEBUG
int         DbgMuxLock=0;
#endif

int quit(const char* why)
{
    LOGW(why);
    ApStat=RELOAD_S;
    return 0;
}

int m_server(const char* dom)
{
    return -1;
}

std::string arr2a(SqArr& a, char sep)
{
    std::string out;
    int     sz = a.GetSize();
    uint16_t ptr[ARR_MAX];
    char    duplet[4];

    if(sz==0){return "";}
    else if(sz>=ARR_MAX)
    {
        LOGW("array to large");
        return "";
    }
    out.clear();
    a.GetArray(ptr, sz);
    for(int i=0;i<sz;i++)
    {
        if(sep==' ')
            ::sprintf(duplet,"%02X",int(ptr[i]));
        else
        {
            if(i<sz-1)
                ::sprintf(duplet,"%02X%c",int(ptr[i]),sep);
            else
                ::sprintf(duplet,"%02X",int(ptr[i]));
        }
        out.append(duplet);
    }
    return out;
}

SqArr& arrcpyn(SqArr& a, int off, SqArr& b)
{
    int sz = b.GetSize();
    if(a.GetSize() < off + b.GetSize())
        a.Resize(off + b.GetSize());

    for(int i=0;i<sz;i++)
    {
        Sqrat::SharedPtr<uint8_t> pv = b.GetValue<uint8_t>(i);
        a.SetValue(i+off, *pv.Get());
    }
    return a;
}

bool arrarr(SqArr& a, SqArr& b)
{
    int     sza = a.GetSize();
    int     szb = b.GetSize();
    uint16_t ptra[ARR_MAX];
    uint16_t ptrb[ARR_MAX];

    if(sza>=ARR_MAX ||szb>=ARR_MAX )
    {
        LOGW("array to large");
        return "";
    }

    a.GetArray(ptra, sza);
    b.GetArray(ptrb, szb);
    if(sza==szb)
    {
        for(int e=0;e<szb;++e)
        {
            if(ptra[e]>256||ptrb[e]>256)continue;
            if(ptra[e]!=ptrb[e])return false;
        }
        return true;
    }
    return false;
}
/*
bool arrarr(SqArr a, SqArr b)
{
    int     sza = a.GetSize();
    int     szb = b.GetSize();
    uint16_t ptra[ARR_MAX];
    uint16_t ptrb[ARR_MAX];

    if(sza>=ARR_MAX ||szb>=ARR_MAX )
    {
        LOGW("array to large");
        return "";
    }

    a.GetArray(ptra, sza);
    b.GetArray(ptrb, szb);
    if(sza==szb)
    {
        for(int e=0;e<szb;++e)
        {
            if(ptra[e]==256||ptrb[e]==256)continue;
            if(ptra[e]!=ptrb[e])return false;
        }
        return true;
    }
    return false;
}
*/

int subarr(SqArr& a, SqArr& b)
{
    int     sza = a.GetSize();
    uint16_t ptra[ARR_MAX];
    int     szb = b.GetSize();
    uint16_t ptrb[ARR_MAX];

    a.GetArray(ptra, sza);
    b.GetArray(ptrb, szb);
    if(szb>=sza)
    {
        for(int k=0;k<=(szb-sza);k++)
        {
            if(::memcmp(ptrb+k,ptra,sza)==0)
            {
                return k;
            }
        }
    }
    return -1;
}

const char* bprint(const uint8_t* ptr, size_t sz)
{
    static std::string out;
    char    duplet[4];

    if(sz==0)return "";
    out.clear();
    out += " ";
    for(size_t i=0;i<sz;i++)
    {
        ::sprintf(duplet,"%02X:",int(ptr[i]));
        out.append(duplet);
    }
    out.append("    ");
    out.append(std::to_string(sz));
    return out.c_str();
}

int suspend(int d)
{
    LOGD1(__FUNCTION__);
    ::msleep(d);
    return d;
}

int reload_scr()
{
    ApStat = RELOAD_S;
    return 0;
}

int exitapp(const char* message)
{
    ApStat=DO_EXIT;
    return 0;
}

std::string execute(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(::popen(cmd, "r"), ::pclose);
    if (pipe)
    {
        while (!feof(pipe.get()))
        {
            if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
                result += buffer.data();
        }
    }
    return result;
}

int setdbg(int32_t d)
{
    Loggran = d;
    return d;
}

std::string kbhit()
{
    std::string s;
    std::cout << ">";
    std::cout.flush();
    std::cin >> s;
    return s;
}

int sys_reboot()
{
    ::system("sudo reboot");
    return 0;
}

bool is_file(const char* fname)
{
    return ::access(fname,0)==0;
}

int uptime()
{
    return time(0)-OTime;
}

int  slog(const char* s)
{
    LOGI(s);
    return 0;
}

int enable_wd(int seconds)
{
    Wdto = seconds;
    return wd_pull(WDIOC_SETTIMEOUT,seconds);
}

const char* get_strdate()
{
    static char sd[32];
    ::strcpy(sd,str_time());
    return sd;
}

const char* get_strtime()
{
    static char sd[64];
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strftime(sd,63, "%H:%M:%S", timeinfo) ;
    return sd;
}

int8_t s8(uint8_t value)
{
    return int8_t(value);
}

int16_t s16(uint16_t value)
{
    return int16_t(value);
}

int32_t s32(int32_t value)
{
    return int32_t(value);
}

int64_t s64(uint64_t value)
{
    return int64_t(value);
}
uint8_t u8(int8_t value)
{
    return int8_t(value);
}

uint16_t u16(int16_t value)
{
    return int16_t(value);
}

int32_t u32(int32_t value)
{
    return int32_t(value);
}

float r32(size_t value)
{
    return float(value);
}

double r64(size_t value)
{
    return (double)(value);
}



uint64_t u64(int64_t value)
{
    return int64_t(value);
}

unsigned int uu(int value)
{
    return (unsigned int)(value);
}

int ii(unsigned int value)
{
    return (int)(value);
}

SqArr get_date()
{
    SqArr       ra(VM(), 16);
    time_t      rawtime;
    struct      tm * timeinfo;
    char        buffer[64];
    char        *ptok, *pbuffer = buffer;
    int         i = 0;

    time ( &rawtime );
    timeinfo = ::localtime ( &rawtime );
    ::strftime(buffer, sizeof(buffer)-1, "%Y %m %d %H %M %S", timeinfo);

    while((ptok = ::strtok(pbuffer, " ")) != nullptr)
    {
        ra.SetValue(i++, ptok);
        pbuffer = nullptr;
    }
    return ra;
}

void make_dir(const std::string& path)
{
    if(::access(path.c_str(),0)!=0)
    {
        std::string cmd = "mkdir -p ";
        cmd += path;
        ::system(cmd.c_str());
        if(::access(path.c_str(),0)!=0)
        {
            LOGW("Cannot create path for file at: " << path);
        }
    }
}

static char StaticSr[16];
const char*  i2a(int k)
{
    ::strcpy(StaticSr, std::to_string(k).c_str());
    return StaticSr;
}

const char*  x2a(int k)
{
    ::sprintf(StaticSr, "%X", k);
    return StaticSr;
}

const char* i2xa(int k)
{
    char d[16];

    sprintf(d,"0x%X",k);
    OutStr=d;
    return OutStr.c_str();
}

const char* consolein()
{
    char line[128]={0};
    std::cout << std::endl <<"rembix> " << std::flush;
    std::cin.getline(line,sizeof(line)-1);
    OutStr = line;
    return OutStr.c_str();
}

SqArr a2arr(const char* str)
{
    char         tmp[32]={0};
    int          val;
    size_t       sl = ::strlen(str);
    SqArr ra(VM(),sl/2);
    int          advance = 0;

    for(size_t i = 0 ; i < sl; i+=2)
    {
        ::strncpy(tmp, &str[i],2);
        ::sscanf(tmp,"%02X",&val);
        ra.SetValue(advance, uint8_t(val));
        ++advance;
    }
    return ra;
}

int  wd_pull(unsigned long int ctl, int flags)
{
    int rv = -1;
    if(Wdto)
    {
        if(::access("/dev/watchdog",0)!=0)
        {
            LOGE("no device /dev/watchdog");
            Wdto=0;
            return rv;
        }
        int fd = ::open("/dev/watchdog", O_WRONLY);
        if(fd>0)
        {
            rv = ::ioctl(fd,ctl,&flags);
            if(rv)
            {
                LOGE(__FUNCTION__ << " " << strerror(rv));
                LOGE("watchdog off" << strerror(errno));
                Wdto = 0;
            }
            ::close(fd);
        }
        else
        {
            LOGE("open /dev/watchdog failed" << strerror(errno));
            Wdto = 0;
        }
    }
    return rv;
}

/**
 * @brief run_loop
 * @param f
 * @param pulseme
 * @return
 */
int run_loop(SqMemb& f, int pulseme)
{
    size_t now = tick_count();
    size_t then = now;
    size_t nowwd = now;
    SqArr  b(App->psqvm(), 0);
    std::vector<I_IDev*> dirty;

    try{
        Sqrat::SharedPtr<bool> srv;
        while(ApStat==RUNNING)
        {
            if(dirty.size()){  dirty.clear(); }
            then = tick_count();
            App->check_devs(dirty, then);

            if(dirty.size())
            {
                now = then;
                for(const auto& a: dirty)
                {
                    a->on_event   ();
                }
            }
            if ((pulseme>0 && (then - now >= (size_t)pulseme)) ||
                __bsqenv->snap_.load()==true)
            {
                if(__bsqenv->snap_.load()==true)
                {
                    __bsqenv->snap_=false;
                    App->comit_devs();
                }
                else {
                    App->comit_devs();
                    now = then;
                    srv = f.Fcall<bool>(App, false);
                    __bsqenv->notify();
                    if(*srv.Get()==false)
                        break;
                }
                __bsqenv->notify();
            }
            if(Wdto &&  (then - nowwd) > (size_t)(Wdto*1000))
            {
                nowwd=then;
                wd_pull(WDIOC_KEEPALIVE,Wdto);
            }
            App->call_backs(now);

        }
    }
    catch(Sqrat::Exception& ex)
    {
        ApStat=RESTART;
        LOGEX(ex.Message());
        LOGEX(SqErrStr);
    }
    __bsqenv->notify();
    return 0;
}

void println(const char* text)
{
    fprintf(stdout,"%s\n",text);
}

void errorln(const char* text)
{
    fprintf(stderr,"[0;31m%s[0m\n",text);
}

void _exit_app()
{
    ApStat=EXIT_APP;
}

static void reconfig_sys(EPERIPH e, SqArr& a)
{
    char c[3][128] = {"","",""};

    for(int i=0;i<a.GetSize();i++)
    {
        Sqrat::SharedPtr<const char*> sp = a.GetValue<const char*>(i);
        ::strcpy(c[i], *sp.Get());
    }
    switch(e)
    {
    case eGPIO:
        DvGpio::config((char*)c[0],(char*)c[1]);
        break;
    case ePWMM:
        DvPwm::config((char*)c[0],(char*)c[1],(char*)c[2]);
        break;
    case eI2C:
        DvI2c::config((char*)c[0],(char*)c[1]);
        break;
    case eSPI:
        DvSpi::config((char*)c[0],(char*)c[1]);
        break;
    case eADC:
        DvAdc::config((char*)c[0],(char*)c[1]);
        break;
    default: break;
    }
}

static Inst* context()
{
    return App;
}

bool loadmod(const char* lib, const char* devname)
{
    void* module  = ::dlopen(lib, RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD);
    if (module == nullptr)
    {
        module = ::dlopen(lib, RTLD_NOW | RTLD_LOCAL);
        if (module == nullptr)
        {
            LOGE("cannot load DLL" << lib << dlerror());
            return false;
        }
    }

    devModPtr_t startmod = (devModPtr_t)::dlsym(module, "start_module");
    if (startmod == nullptr)
    {
        LOGE("cannot load startmod" << __FUNCTION__ << dlerror());
        ::dlclose(module);
        module = nullptr;
        return false;
    }
    __sq_env->add_dll(module);
    return startmod(__vm,  SQ_PTRS, App, devname);
}

void usingop(int32_t flags)
{
    SqEnvi* sq = App->scr_env();

    if(flags & eGPIO)   GpioDev::squit(*sq);
    if(flags & ePWMM)   PwmDev::squit(*sq);
    if(flags & eI2C)    I2CDev::squit(*sq);
    if(flags & eSPI)    SpiDev::squit(*sq);
    if(flags & eADC)    AdcDev::squit(*sq);
    if(flags & eTIMER)  __noop;
    if(flags & eUART)   UartDev::squit(*sq);
    if(flags & eHTTP)   RestSrv::squit(*sq);
    if(flags & eBASE)   ScrBase::squit(*sq);
    if(flags & eSRV)   RestSrv::squit(*sq);
    if(flags & eDB)  Database::squit(*sq);
    if(flags & eFILE)   FileDev::squit(*sq);
    if(flags & eSOLIB)  SoLib::squit(*sq);
    if(flags & eSOCKET)   RawSock::squit(*sq);
    if(flags & eINPUT)   InputSys::squit(*sq);
    //if(flags & eDEVMODULE) ModuDev::squit(*sq);
    if(flags & eSUNRS)   SunTimes::squit(*sq);
#ifdef WITH_CURL
    if(flags & eCURL)   ComCurl::squit(*sq);
#endif //#ifdef WITH_USB
#ifdef WITH_SSH
    if(flags & eSSH)    SshComm::squit(sq);
#endif //#ifdef WITH_USB
#ifdef WITH_USB
    if(flags & eUSB) UsbDev::sqprintlnuit(sq);
#endif //#ifdef WITH_USB
}

void globals_expose(SqEnvi& sq)
{
	// Create VM, register other libs, etc...
	sqrat_register_importlib(sq.theVM());
	//usingop(0xFFFFFF);

	Sqrat::ConstTable(sq.theVM())
			.Const("eKEYBOARD",eKEYBOARD)
			.Const("eMOUSE",eMOUSE)
			.Const("eTOUCH",eTOUCH)
			.Const("eJOYSTIC",eJOYSTIC)
			.Const("NOKEY","*")
			.Const("INPUT", 0)
			.Const("OUTPUT", 1)
			.Const("DIR_IN", eIn)
			.Const("DIR_OUT", eOut)
			.Const("HIGH", 1)
			.Const("LOW", 0)
			.Const("ON_0",1)
			.Const("eGPIO",eGPIO)
			.Const("ePWMM",ePWMM)
			.Const("eI2C",eI2C)
			.Const("eSPI",eSPI)
			.Const("eSYSIN",eINPUT)
			.Const("eADC",eADC)
			.Const("eTIMER",eTIMER)
			.Const("eUART",eUART)
			.Const("eHTTP",eHTTP)
			.Const("eSSH",eSSH)
			.Const("eBASE",eBASE)
			.Const("eSRV",eSRV)
			.Const("eDB",eDB)
			.Const("eFILE",eFILE)
			.Const("eSOLIB",eSOLIB)
			.Const("eSOLIB",eUSB)
			.Const("eSOCKET",eSOCKET)
			.Const("eDEVMODULE",eDEVMODULE)
			.Const("eCURL",eCURL)
			.Const("eSUNRS",eSUNRS)

			.Const("eVOID",eVOID)
			.Const("eINT",eINT)
			.Const("eINT64",eINT64)
			.Const("eUINT",eINT)
			.Const("eUINT64",eINT64)
			.Const("eREAL",eREAL)
			.Const("eSTRING",eSTRING)
			.Const("eBINARY",eBINARY);

	Sqrat::ConstTable(sq.theVM())
			.Const("CURLAUTH_NONE", 0)
			.Const("CURLAUTH_BASIC", 1)
			.Const("CURLAUTH_DIGEST", 2)
			.Const("CURLAUTH_NEGOTIATE", 4);

	Sqrat::RootTable(sq.theVM()).Functor("execute",&execute);
	Sqrat::RootTable(sq.theVM()).Functor("loadmod",&loadmod);
	Sqrat::RootTable(sq.theVM()).Functor("context",&context);
	Sqrat::RootTable(sq.theVM()).Functor("i2a",&i2a);
	Sqrat::RootTable(sq.theVM()).Functor("x2a",&x2a);
	Sqrat::RootTable(sq.theVM()).Functor("uptime", &uptime);
	Sqrat::RootTable(sq.theVM()).Functor("slog", &slog);
	Sqrat::RootTable(sq.theVM()).Functor("quit", &quit);
	Sqrat::RootTable(sq.theVM()).Functor("enable_wd", &enable_wd);
	Sqrat::RootTable(sq.theVM()).Functor("get_strdate", &get_strdate);
	Sqrat::RootTable(sq.theVM()).Functor("get_strtime", &get_strtime);
	Sqrat::RootTable(sq.theVM()).Functor("get_date", &get_date);
	Sqrat::RootTable(sq.theVM()).Functor("arrarr", &arrarr);
	Sqrat::RootTable(sq.theVM()).Functor("arr2a", &arr2a);
	Sqrat::RootTable(sq.theVM()).Functor("arrcpyn", &arrcpyn);
	Sqrat::RootTable(sq.theVM()).Functor("subarr", &subarr);
	Sqrat::RootTable(sq.theVM()).Functor("bprint", &bprint);
	Sqrat::RootTable(sq.theVM()).Functor("consolein", &consolein);
	Sqrat::RootTable(sq.theVM()).Functor("a2arr", &a2arr);
	Sqrat::RootTable(sq.theVM()).Functor("suspend", &suspend);
	Sqrat::RootTable(sq.theVM()).Functor("sleep", &suspend);
	Sqrat::RootTable(sq.theVM()).Functor("setdbg", &setdbg);
	Sqrat::RootTable(sq.theVM()).Functor("i2xa", &i2xa);
	Sqrat::RootTable(sq.theVM()).Functor("is_file", &is_file);
	Sqrat::RootTable(sq.theVM()).Functor("bark", &wd_pull);
	Sqrat::RootTable(sq.theVM()).Functor("run", &run_loop);
	Sqrat::RootTable(sq.theVM()).Functor("exitapp", &exitapp);
	Sqrat::RootTable(sq.theVM()).Functor("println", &println);
	Sqrat::RootTable(sq.theVM()).Functor("errorln", &errorln);
	Sqrat::RootTable(sq.theVM()).Functor("reconfig_sys", &reconfig_sys);
	Sqrat::RootTable(sq.theVM()).Functor("S8", &s8);
	Sqrat::RootTable(sq.theVM()).Functor("S16", &s16);
	Sqrat::RootTable(sq.theVM()).Functor("R32", &r32);
	Sqrat::RootTable(sq.theVM()).Functor("R64", &r64);
	Sqrat::RootTable(sq.theVM()).Functor("S32", &s32);
	Sqrat::RootTable(sq.theVM()).Functor("S64", &s64);
	Sqrat::RootTable(sq.theVM()).Functor("U8", &u8);
	Sqrat::RootTable(sq.theVM()).Functor("U16", &u16);
	Sqrat::RootTable(sq.theVM()).Functor("U32", &u32);
	Sqrat::RootTable(sq.theVM()).Functor("U64", &u64);
	Sqrat::RootTable(sq.theVM()).Functor("I", &ii);
	Sqrat::RootTable(sq.theVM()).Functor("U", &uu);
	Sqrat::RootTable(sq.theVM()).Functor("using", &usingop);
}

