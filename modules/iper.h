#ifndef PERIPHERIAL_INTERFACE
#define PERIPHERIAL_INTERFACE

#include <iostream>
#include <squirrel.h>
#include <sqrat.h>
#include <sqmodule.h>

#define ALLDATA '*'
#define SALLDATA "*"
#define STDSTR0  std::string("");

typedef enum E_TYPE
{
    eVOID,
    eINT,
    eINT64,
    eUINT,
    eUINT64,
    eREAL,  //double
    eSTRING,
    eBINARY,
    eSTRUCT
}E_TYPE;

typedef enum E_VENT
{
	eREAD,
	eWRITE,
	eIOCTL,
	eCLOSE,

}E_VENT;

typedef enum EPERIPH{
    eNOCAT=0,
    eGPIO,
    ePWMM,
    eI2C,
    eSPI,
    eADC,
    eTIMER,
    eUART,
    eHTTP,
    eSSH,
    eBASE,
    eSRV,
    eDB,
    eFILE,
    eSOLIB,
    eUSB,
    eSOCKET,
    eDEVMODULE,
    eSCRIPT,
    eCURL,
    eSUNRS,
    eINPUT,
    eOUTPUT,
}EPERIPH;




template <int SZ>class fastbuf_t
{
    uint8_t     _loco[SZ] = {0};
    uint8_t*    _ploco=nullptr;
public:
    fastbuf_t(size_t sz){ if(sz<SZ)_ploco=_loco; }
    ~fastbuf_t(){if(_ploco != _loco) delete[] _ploco; }
    operator uint8_t*(){return _ploco;}
    operator char*(){return (char*)_ploco;}
    operator const uint8_t*(){return _ploco;}
    operator const char*(){return (char*)_ploco;}
    uint8_t operator[](size_t index){return _ploco[index];}
    uint8_t operator[](int index){return _ploco[index];}
};

/*  generic data / device ()
    has slots basic_string<uint8_t> storing raw data in each slot.
    each slot can have any length, for stirngs or buffers,
    each slot can store one type like int's floats
    [0,...] has associated type, data length, and data
*/
#define MAX_SLOTS   10
class  any_t
{
public:
	any_t(){
		::memset(_dl,0,sizeof(_dl));
	}
    template <size_t T>
	any_t(fastbuf_t<T>& d, size_t bytes, int index=0){
        assert(index<=MAX_SLOTS);
        _stor[index].assign((const uint8_t*)d,bytes);
    }
	any_t(const char* d,  int bytes, int index=0){
        assert(index<=MAX_SLOTS);
        _stor[index].assign((const uint8_t*)d,bytes);
    }
	any_t(const uint8_t* d,  int bytes, int index=0){
        assert(index<=MAX_SLOTS);
        _stor[index].assign(d,bytes);
    }
    template <typename T>any_t(const T& t, int index=0){
        assert(index<=MAX_SLOTS);
        _stor[index].assign((uint8_t*)(&t),_dl[index]==0?sizeof(T):_dl[index]);
    }
    void let(const uint8_t* p, size_t l, int index=0){
        assert(index<=MAX_SLOTS);
        _stor[index].assign(p,l);
    }
    void let(const char* p, size_t l, int index=0){
        assert(index<=MAX_SLOTS);
        _stor[index].assign((const uint8_t*)p,l);
    }
    template <typename T> void let(const T& t, int index=0){
        assert(index<=MAX_SLOTS);
        size_t szt = sizeof(T);
        _stor[index].assign((const uint8_t*)(&t),
                            _dl[index]==0 ? szt : _dl[index]);
    }
    template <class T>
    std::string to_string(int index=0)const{
        assert(index<=MAX_SLOTS);
        if(_stor[index].length())
            return std::to_string(*((T*)(_stor[index].data())));
        return STDSTR0;
    }
    template<typename T> const T& to_t(int index=0)const{
        assert(index<=MAX_SLOTS);
        return *((const T*)_stor[index].data());
    }
    int atoi(int index=0){return ::atoi((const char*)_stor[index].c_str()); }
    const char* c_chars(int index=0)const{
        assert(index<=MAX_SLOTS);
        return (const char*) _stor[index].data();
    }
    const uint8_t* c_bytes(size_t &len, int index=0)const{
        assert(index<=MAX_SLOTS);
        len = _stor[index].length();
        return (const uint8_t*) _stor[index].data();
    }
    void clear(){
        for(int i=0;i<MAX_SLOTS;++i)
        {
            _stor[i].clear();
            _dl[i] = 0;
            _types[i]=eVOID;
        }
    }
    template <class X>
    void pusht(const X& b, int index=0, int sz= sizeof(X)){
        _stor[index].append((uint8_t*)&b,_dl[index]==0?sz:_dl[index]);
        //std::cout << "len = " << (int)_stor[index].length() << "\n";
    }
    void push( const char* b, int index=0){
        _stor[index].append((const uint8_t*)b,::strlen(b));
    }
    size_t size()const{return MAX_SLOTS;}
    size_t length(int index=0)const{
        return _stor[index].length();
    }
    const uint8_t* c_bytes(int index=0)const {return (const uint8_t*)_stor[index].data();}
	any_t& operator=(const any_t &t){
        clear();
        for(size_t i=0;i<MAX_SLOTS;++i){
            _stor[i]=t._stor[i];
            _dl[i]=t._dl[i];
            _types[i]=t._types[i];
        }
        return *this;
    }
	bool operator==(const any_t &t){
        if(t.length()!=length())return false;
        for(size_t i=0;i<MAX_SLOTS;++i){
            if(_stor[i]==t._stor[i])continue;
            return false;
        }
        return true;
    }
    E_TYPE peer_of(int index=0)const{return _types[index];}
    void typeit(E_TYPE e, int index=0){
        _types[index]=e;
        if(e== eBINARY)
            _dl[index] = 1;
    }
    bool is_dirty()const{return _stor[0].length();}
    void fmt_hex(std::string& here, int index=0){
        char hex[4];
        for(size_t b=0; b < _stor[index].length(); ++b)
        {
            ::sprintf(hex,"%02X:",_stor[index].at(b));
            here+=hex;
        }
    }
private:
    std::basic_string<uint8_t>  _stor[MAX_SLOTS];
	E_TYPE						_types[MAX_SLOTS];
	size_t						_dl[MAX_SLOTS];
};


// base class for module object
class  I_IDev
{
public:
	virtual ~I_IDev(){}
	virtual const char* name()const=0;
	virtual const char* dev_key()const=0;
	virtual bool  is_dirty(size_t t)=0;
	virtual bool  set_value(const char* key, const char* value)=0;
	virtual const char* get_value(const char* key)=0;
	virtual const any_t& get_data()const=0;
    virtual void  sync(const char* filter=nullptr)=0;
    virtual Sqrat::Object object()const=0;
};

// base class for GPIO's I2C devices
class IoOps
{
public:
	IoOps(){}
	virtual ~IoOps(){}
	virtual bool	iopen(int rm=0)=0;
	virtual void	iclose()=0;
	virtual size_t  bread(uint8_t* buff, int len, int options=0)=0;
	virtual int     bwrite(const uint8_t* buff, int len, int options=0)=0;
	virtual void    on_event(E_VENT e, const uint8_t* buff, int len, int options=0)=0;
	virtual const char* err_desc()const=0;
	virtual EPERIPH peer_of()const=0;
	virtual E_TYPE  data_of()const=0;
};

// main context
class IInstance
{
public:
	virtual ~IInstance(){}
    virtual IoOps*  get_devi(const char*)=0;
    virtual void    add_this(I_IDev* o, const char* name)=0;
};


typedef bool (*devModPtr_t)(HSKVM vm, sq_api* ptrs, IInstance* pi, const char* name);


#define IMPLEMENT_START_MODULE(ClassName_)	EXPORT bool start_module(HSKVM vm, sq_api* ptrs,	\
	                                        IInstance* pi, const char* name){					\
	printf(" %s  %p %p  %s\n", __FUNCTION__, vm, ptrs, name);									\
	__vm = vm;																					\
	SQ_PTRS = ptrs;																				\
	__pi = pi;																					\
	Sqrat::DefaultVM::Set(vm);																	\
	ClassName_::squit(name);																	\
	return true;}																				\


#endif // PLUG_EMBIX_

