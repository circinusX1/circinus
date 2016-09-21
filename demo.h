#ifndef DEMO_H
#define DEMO_H

#include "sqwrap.h"

class Demo
{
    int _value=0;
public:
    Demo();
    Demo(int custom);
    ~Demo();
    static void export2Script(SqEnv&  sq);
    int Method(int k, const char* s);
};

#endif // DEMO_H
