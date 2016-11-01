#ifndef WAS_BCORE_H_
#define WAS_BCORE_H_

#include <stdint.h>
#include <cstring>
#include <string>
#include <map>
#include <cstdio>
#include <dirent.h>
#include <string>
#include <sstream>
#include <fstream>

#define S_NAN   "99999999"
#define I_NAN    99999999

namespace BusNs
{


enum LOGICV       {
         LOW  = 0,
         HIGH = 1
     };

enum PIN_DIR
{
    MUX_PIN = 0,
    IN_PIN  = 1,
    OUT_PIN = 2,
    IO_PIN  = 3
};


enum I2C_NUM            {
           I2C_0  = 0,
           I2C_1  = 1,
           I2C_2  = 2
       };

enum RW_MOD {
    M_RO = 0,
    M_WO = 1,
    M_RW = 2
};

template <typename T> inline std::string tostr(const T& t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}

class BusCore
{
public:
    BusCore();
    virtual ~BusCore();
    const std::string& sysfile()const
    {
        return  _sysfile;
    }
    static void initlib(const char* userhome);

private:
    std::string     _tmpfile;
    std::map<std::string,std::string>  _offlineval;

protected:
    std::string     _sysfile;
    std::string     _exec(std::string command);
    std::string& _getfile(const char* path, const char* gp)
    {
        _tmpfile = path;
        _tmpfile += gp;
        return _tmpfile;
    }

    std::string& _getfile(const char* path, const int gp)
    {
        _tmpfile = path;
        _tmpfile += std::to_string(gp);
        return _tmpfile;
    }

    std::string& _getfile(const char* path, const int gp, const char* what)
    {
        _tmpfile = path;
        _tmpfile += std::to_string(gp);
        _tmpfile += "/";
        _tmpfile += what;
        return _tmpfile;
    }

    std::string& _getfile(const char* path, const char* sp, const char* what)
    {
        _tmpfile = path;
        _tmpfile += sp;
        _tmpfile += what;
        return _tmpfile;
    }

    std::string _readfile(const std::string& file);
    bool _writefile(const std::string& file, const char* value, bool check=false);
};
}

#endif


