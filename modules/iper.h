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


#define SHALL_CTOR() \
    __eng_rw = __pinstance->get_devi(dev); \
    assert(__eng_rw);               \
    __eng_rw = __eng_rw;              \
    __pinstance->add_this(this, name); \
    _ratobj.BindCppObject(this);

#define SHALL_DTOR() \
    __eng_rw->iclose();  \
    __pinstance->remove_obj(_name.c_str());


    
/*  generic data / device ()
    has slots basic_string<uint8_t> storing raw data in each slot.
    each slot can have any length, for stirngs or buffers,
    each slot can store one type like int's floats
    [0,...] has associated type, data length, and data
*/
#define MAX_SLOTS   8
class  devdata_t
{
public:
    devdata_t(){
		::memset(_dl,0,sizeof(_dl));
	}
	devdata_t(const devdata_t& r){
		::memcpy(_dl,&r,sizeof(_dl));
	}
    template <size_t T>
    devdata_t(const uint8_t* d, size_t bytes, int index=0){
        assert(index<=MAX_SLOTS);
        _stor[index].assign(d,bytes);
    }
    devdata_t(const char* d,  int bytes, int index=0){
        assert(index<=MAX_SLOTS);
        _stor[index].assign((const uint8_t*)d,bytes);
    }
    devdata_t(const uint8_t* d,  int bytes, int index=0){
        assert(index<=MAX_SLOTS);
        _stor[index].assign(d,bytes);
    }
    template <typename T>devdata_t(const T& t, int index=0){
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

    devdata_t& operator=(const devdata_t &t){
        clear();
        for(size_t i=0;i<MAX_SLOTS;++i){
            _stor[i]=t._stor[i];
            _dl[i]=t._dl[i];
            _types[i]=t._types[i];
        }
        return *this;
    }
    bool operator==(const devdata_t &t){
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
    bool _mon_cbacks_call(size_t t=0)const{return _stor[t].length();}
    void fmt_hex(std::string& here, int index=0){
        char hex[4];
        for(size_t b=0; b < _stor[index].length(); ++b)
        {
            ::sprintf(hex,"%02X:",_stor[index].at(b));
            here+=hex;
        }
    }
    std::basic_string<uint8_t>& operator[](int index){return _stor[index];}
    
private:
	std::basic_string<uint8_t>  _stor[MAX_SLOTS];
	E_TYPE						_types[MAX_SLOTS];
	size_t						_dl[MAX_SLOTS];
};


// base class for module object
// main context
class IoOps;
class I_IDev;
class IInstance
{
public:
	virtual ~IInstance(){}
    virtual IoOps*  get_devi(const char*)=0;
    virtual void    add_this(I_IDev* o, const char* name)=0;
    virtual void    remove_obj(const char* name)=0;
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

class  I_IDev
{
public:

    I_IDev(const char* dev, const char* name):_name(name){
        __eng_rw = __pinstance->get_devi(dev); 
        __pinstance->add_this(this, name); 
    }

    virtual ~I_IDev(){
        if(__eng_rw)__eng_rw->iclose();  
        __pinstance->remove_obj(_name.c_str());
    }
    
	virtual const char* name()const=0;              // ret the name
	virtual const char* dev_key()const=0;			// the key
    virtual bool  _mon_cbacks_call(time_t tnow)=0;	// monitoring call
	virtual bool  set_value(const char* key, const char* value)=0;  // set if any
	virtual const char* get_value(const char* key)=0;	// get if any
	virtual const devdata_t& get_data()const=0;			// return all
	virtual void  sync(const char* filter=nullptr)=0;	// update internal state
	virtual Sqrat::Object object()const=0;				// return sq obj inst
    
protected:
    Sqrat::Object       _ratobj;
    std::string         _name;
    devdata_t           _data;

public:    
    static IInstance*   __pinstance;
    static HSKVM        __vm;
    static IoOps*       __eng_rw;
};

typedef bool (*devModPtr_t)(HSKVM vm, sq_api* ptrs, IInstance* pi, const char* name);
                                                                       \

#ifdef PLUGIN_LIB

#define MODULE_VARS() HSQAPI SQ_PTRS;                       \
                      IInstance*   I_IDev::__pinstance;    \
                      HSKVM        I_IDev::__vm;           \
                      IoOps*       I_IDev::__eng_rw

#define MUST_CALL()     _ratobj.BindCppObject(this);


#define IMPLEMENT_START_MODULE(ClassName_)	EXPORT bool start_module(HSKVM vm, sq_api* ptrs,	\
                                            IInstance* pi, const char* name){					\
    printf(" %s  %p %p  %s\n", __FUNCTION__, (void*)vm, (void*)ptrs, name);     				\
    I_IDev::__vm = vm;																			\
    SQ_PTRS = ptrs;																				\
    I_IDev::__pinstance = pi;																	\
    Sqrat::DefaultVM::Set(vm);																	\
    ClassName_::squit(name);																	\
    return true;}

#ifdef __cplusplus
extern "C" {
#endif

EXPORT bool start_module(HSKVM vm, sq_api* ptrs,  IInstance* pi, const char* name);

#ifdef __cplusplus
}
#endif

#define ALL_VIRTUALS()								\
	virtual const char* name()const;				\
	virtual const char* dev_key()const;				\
	virtual bool  _mon_cbacks_call(time_t tnow);			\
	virtual bool  set_value(const char* key, const char* value); \
	virtual const char* get_value(const char* key); \
	virtual const devdata_t& get_data()const;		\
	virtual void  sync(const char* filter=nullptr); \
	virtual Sqrat::Object object()const;			\
	virtual void  on_event(E_VENT e, const uint8_t* buff, int len, int options);



#endif //PLUGIN_LIB

#endif // PLUG_EMBIX_

