/*
#include <string>
#include <vector>
#include "dlconfig.h"
#include "dcore.h"
#include "dynamor.h"

using namespace GenericHw;

dynamor::dynamor()
{

}


bool    dynamor::elfit(const char* library)
{
    std::string libname = "lib"; libname+=library; libname += ".so";
    std::string header  = library; header += ".h";

    std::string  libs = DvCore::sh_exec("find /usr/ -name " + libname);
    if(!libs.empty())libs=libs.substr(0,libs.length()-1);
    std::string  hdrfile = DvCore::sh_exec("find /usr -name " + header);
    if(!hdrfile.empty())hdrfile=hdrfile.substr(0,hdrfile.length()-1);

    if(::access(hdrfile.c_str(),0)==0  && ::access(libs.c_str(),0)==0)
    {
        std::string grep = "nm -D  "; grep += libs; grep += " | grep ' T ' | awk '{print $3}'" ;
        std::string foos =  DvCore::sh_exec(grep);
        if(!foos.empty())
        {
            std::vector<std::string> afoos;
            CFL::explode(foos, "\n", afoos);
            if(afoos.size())
            {
                for(std::string& foo: afoos) {
                    _find_signature(hdrfile, foo);
                }
            }
        }
    }
    return false;
}

void dynamor::_find_definitions(const std::string& headers)
{

}

void dynamor::_find_signature(const std::string& hdrfile, const std::string& foo)
{

    FILE* pf = fopen(hdrfile.c_str(),"wb");
    if(pf)
    {
        char line[256];
        while(!::feof(pf))
        {
            line[0]=0;
            if(!fgets(line,sizeof(line)-1,pf) || ::feof(pf))
            {
                break;
            }
            if(::strstr(line,""));
        }
        ::fclose(pf);
    }
}





*/
