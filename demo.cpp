
#include <stdio.h>
#include "demo.h"

Demo::Demo()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

Demo::Demo(int custom):_value(custom)
{
    std::cout << __PRETTY_FUNCTION__<< custom << "\n";
}

Demo::~Demo()
{
    std::cout << __PRETTY_FUNCTION__<< _value << "\n";
}

int Demo::Method(int k, const char* s)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    printf(" Demo::Method called %d %s custom=%d \n",k, s,_value);
    return _value;
}

/**
 * @brief Demo::export2Script, exports this class to script
 * @param sq
 */
void Demo::export2Script(SqEnv&  sq)
{
    Sqrat::Class<Demo> cls(sq.theVM(), _SC("Demo")); // export class name Demo as Demo
    cls.Ctor<int>();                                 // export custom constructor Demo(int)
    cls.Ctor();                                      // export default cinstructor to scrpt Demo()
    cls.Func(_SC("Method"), &Demo::Method);          // exports the Method to script
    Sqrat::RootTable().Bind(_SC("Demo"), cls);       // bind the class to the script VM
}
