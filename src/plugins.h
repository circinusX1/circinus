#ifndef PLUGINS_H
#define PLUGINS_H

#include "osthread.h"
#include "icombase.h"
#include <map>
#ifdef __linux
#include <dlfcn.h>
#endif
#define MAX_IFACES  32
//------------------------------------------------------------------------------

#ifdef __linux
typedef void* SoHandler;
#else
typedef HINSTANCE SoHandler;
#endif

struct SoEntry
{
    SoEntry(SoHandler);
    ~SoEntry();
    SoHandler    _sohndl;
    pGI          _so_get;
    pRI          _so_release;
};

/**
typedef std::map<std::string, SoEntry*> SoMap;
typedef std::map<std::string, SoEntry*>::const_iterator SoMapIt;

class Plugins
{
public:
    Plugins();
    virtual ~Plugins();
    const SoEntry* get_module(const char* ext);
    bool  load(const char* mod);

protected:
private:
    SoMap   _mods;
    size_t  _imods;
};
*/

#endif // PLUGINS_H
