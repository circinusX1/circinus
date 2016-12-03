
#include "devfile.h"
#include "icombase.h"

DevFile::DevFile(const char* file):_sysfile(file)
{

}

DevFile::~DevFile()
{
}

std::string DevFile::_readfile()
{
    std::ifstream expFile;
    std::string   out = S_NAN;

    try{
        expFile.open(_sysfile.c_str(), std::ios::in);
        if(expFile.is_open())
        {
            LOGDETAIL(__PRETTY_FUNCTION__ << " reading from " << _sysfile);
            expFile >> out;
            expFile.close();
            if(!out.empty())
            {
                size_t eos = out.find_first_of('\n');
                if(eos != std::string::npos)
                {
                    out = out.substr(0,eos);
                }
            }
        }
        else
        {
            LOGWARN(__PRETTY_FUNCTION__ << " Cannot read: " << _sysfile);
        }
    }
    catch (std::exception& e)
    {
        LOGERR(__PRETTY_FUNCTION__ <<" std exception "<< e.what());
    }
    return out;
}

bool DevFile::_writefile(const char* value, bool check)
{
    std::ofstream expFile;
    try{
        expFile.open( _sysfile.c_str(), std::ios::out);
        if(expFile.is_open())
        {
            LOGDETAIL(__PRETTY_FUNCTION__ << " writing:" << value << " to " << _sysfile);
            expFile << value;
            expFile.close();
            if(check==false){
                return true;
            }
        }
        else
        {
            LOGWARN("Cannot open " << _sysfile << "  " << errno);
        }
        if(_readfile()==value)
        {
            return true;
        }
        LOGWARN("Writing value " << value << " to " << _sysfile << " failed " << _sysfile);
        return false;
    }
    catch (std::exception& e)
    {
        LOGERR(__PRETTY_FUNCTION__ <<" std exception "<< e.what());
    }
    return false;
}
