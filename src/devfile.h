#ifndef DEVFILE_H
#define DEVFILE_H




#include <cstring>
#include <string>
#include <map>
#include <cstdio>
#include <dirent.h>
#include <string>
#include <sstream>
#include <fstream>

#define S_NAN   "0.10001"
#define I_NAN    0.10001


#if __cplusplus >= 201103L && defined(_GLIBCXX_USE_C99)

# else
#   ifndef STDTOSTRING_H
#   define STDTOSTRING_H

#include <string>
#include <sstream>
#include <stdlib.h>



using namespace std;
namespace std
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }

    inline long stoll(const std::string& str)
    {
        return ::atol(str.c_str());
    }

}
#   endif
#endif


template <typename T> inline std::string tostr(const T& t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}

class DevFile
{
public:
    DevFile(const char* sysfile);
    virtual ~DevFile();
    const std::string& sysfile()const
    {
        return  _sysfile;
    }

protected:
    std::string _readfile();
    bool _writefile(const char* value, bool check=false);
private:
    std::string  _sysfile;
};

#endif // DEVFILE_H
