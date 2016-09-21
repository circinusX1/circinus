

#include <unistd.h>
#include <string>
#include <deque>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "sqwrap.h"
#include "main.h"

#ifdef __linux
#   include <set>
#   include <sys/ioctl.h>
#   include <termios.h>
#   include <libgen.h>
#else
#   include <stdlib.h>
#   include <stdio.h>
#endif
#include "demo.h"

static int GlobalCall(const char* s);
/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    SqEnv               sq;         // the script environment C++
    std::string         script;

    if(argc < 2)
    {
        std::cout << "pass a script name \n";
        return 1;
    }
    else
    {
        script = argv[1];           // we pass the script.tcc  name
    }


    /**
      add global function GlobalCall for the script
      from the script the right paramters have to be passed
      see script.tcc line 19
      */
    Sqrat::RootTable(sq.theVM()).Func("GlobalCall", &GlobalCall);

    /**
      add classes and their methods for the script
    */
    Demo::export2Script(sq);

    try{
        MyScript scr = sq.compile_script(script.c_str());
        scr.run_script();

        /**
         * @brief mimics a script like arduino sketches with setup() and loop() functions
         *
         */
        Sqrat::Function f = Sqrat::RootTable().GetFunction(_SC("setup"));
        if(!f.IsNull())
        {
            Sqrat::SharedPtr<int>   srv;
            int                     rv;

            if(argc==2)
                srv = f.Evaluate<int>(0);
            else
                srv = f.Evaluate<int>(argv[2]);
            if(srv.Get()==0)
            {
                throw (Sqrat::Exception("function setup must return True or False "));
            }
            else
                rv = (*srv.Get());

            if(rv == 1)
            {
                Sqrat::Function fl = Sqrat::RootTable().GetFunction(_SC("loop"));
                if(fl.IsNull())
                {
                    std::cout << "function 'var loop()' not found" << std::endl;
                }
                else
                {
                    srv = fl.Evaluate<int>();
                    if(srv.Get()==0)
                    {
                        throw (Sqrat::Exception("function loop must return True to continue"));
                    }
                }
            }
        }
        else
        {
            std::cout << "function 'setup(param2)' not found " << std::endl;
        }
    }
    catch(Sqrat::Exception ex)
    {
        std::cout << "Main: " << ex.Message();
        ::msleep(50000);
    }

    std::cout << "\npress a key to exit \n";
    getchar();

    return 0;
}

void exit_app()
{
    exit(0);
}

int GlobalCall(const char* s)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    printf("GlobalCall demo called with: '%s' \n", s);
    return 0;
}
