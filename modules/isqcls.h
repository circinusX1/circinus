#ifndef PERASCLS_INTERFACE
#define PERASCLS_INTERFACE


#include <iostream>
#include <squirrel.h>
#include <sqrat.h>
#include <sqmodule.h>


class  I_SqCls
{
public:
    I_SqCls(){}
    I_SqCls(const char* dev, const char* name):_name(name){
    }

    virtual ~I_SqCls(){
    }
protected:
    Sqrat::Object       _ratobj;
    std::string         _name;

public:    
    static HSKVM        __vm;
};

typedef bool (*devClsPtr_t)(HSKVM vm, sq_api* ptrs, const char* name);
                                                                       \

#ifdef PLUGIN_LIB

#define MODULE_VARS() HSQAPI SQ_PTRS;                       \
                      HSKVM        I_SqCls::__vm;           

#define MUST_CALL()     _ratobj.BindCppObject(this);


#define IMPLEMENT_START_SLASS(ClassName_)	EXPORT bool start_sqclass(HSKVM vm, sq_api* ptrs,	\
                                            const char* name){					\
    printf(" %s  %p %p  %s\n", __FUNCTION__, (void*)vm, (void*)ptrs, name);     				\
    I_SqCls::__vm = vm;																			\
    SQ_PTRS = ptrs;																				\
    Sqrat::DefaultVM::Set(vm);																	\
    ClassName_::squit(name);																	\
    return true;}

#ifdef __cplusplus
extern "C" {
#endif

EXPORT bool start_sqclass(HSKVM vm, sq_api* ptrs,  const char* name);

#ifdef __cplusplus
}
#endif

#endif //PLUGIN_LIB

#endif // PLUG_EMBIX_

