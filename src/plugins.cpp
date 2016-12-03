#include "plugins.h"



SoEntry::SoEntry(SoHandler ph):_sohndl(ph)
{
#ifdef __linux
    _so_get = (pGI)(dlsym(ph,"getInstance"));
    _so_release = (pRI )(dlsym(ph,"destroyInstance"));
#else
    _so_get = (pGI)GetProcAddress(ph,"getInstance");
    _so_release = (pRI)GetProcAddress(ph,"destroyInstance");
#endif
}

SoEntry::~SoEntry()
{
#ifdef __linux
    dlclose(_sohndl);
#else
    ::FreeLibrary(_sohndl);
#endif
}

/**
bool Plugins::load(const char* cred)
{
    std::string cons = cred;
    std::string plug = "plugins";

    std::string sub = cons.substr(0, cons.find(':');
    plug += PATH_SEP;
    plug += sub;
    plug += PATH_SEP;
    if(get_module(sub.c_str()) == 0)
    {
    #ifdef __linux
        plug += sub+".so";
        SoHandler h = dlopen(plug.c_str(), RTLD_LAZY);
    #else
        plug += sub+".dll";
        SoHandler h = LoadLibrary(plug.c_str());
    #endif
        if(h)
        {
            _mods[sub] = new SoEntry(h);
        }
    }
}

const SoEntry* Plugins::get_module(const char* proto)
{
    if(_mods.find(proto) != _mods.end())
        return _mods[proto];
    return 0;
}

Plugins::Plugins():_imods(0)
{
}


Plugins::~Plugins()
{
    SoMapIt it = _mods.begin();
    for(; it != _mods.end(); it++){
        delete it->second;
    }
}

*/
