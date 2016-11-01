
#include <sys/stat.h>
#include "../main.h"
#include "buscore.h"

class RunCtx;
extern RunCtx*  PCTX;


namespace BusNs
{
    BusCore::BusCore()
    {

    }

    BusCore::~BusCore()
    {
    }


    std::string BusCore::_exec(std::string command)
    {
        FILE* pipe = popen(command.c_str(), "r");
        if ( pipe==NULL )
        {
            return "ERROR";
        }

        char buffer[128];
        std::string result = "";

        while( !feof(pipe) )
        {
            if( fgets(buffer, 128, pipe) != NULL )
            {
                result += buffer;
            }
        }
        pclose(pipe);
        return result;
    }


    std::string BusCore::_readfile(const std::string& file)
    {
        std::ifstream expFile;
        std::string   out = S_NAN;


        try{
            expFile.open( file, std::ios::in);
            if(expFile.is_open())
            {
                LOGDETAIL2(__PRETTY_FUNCTION__ << " reading from " << file);
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
                LOGDETAIL2(__PRETTY_FUNCTION__ << " Cannot read: " << file);
            }
        }
        catch (std::exception& e)
        {
            LOGERR(__PRETTY_FUNCTION__ <<" std exception "<< e.what());
        }
        return out;
    }

    bool BusCore::_writefile(const std::string& file, const char* value, bool check)
    {

        std::ofstream expFile;
        try{
            expFile.open( file, std::ios::out);
            if(expFile.is_open())
            {
                LOGDETAIL2(__PRETTY_FUNCTION__ << " writing:" << value << " to " << file);
                expFile << value;
                expFile.close();
                if(check==false){
                    return true;
                }
            }
            else
            {
                LOGDETAIL2("Cannot open " << file << "  " << errno);
            }
            if(_readfile(file)==value)
            {
                return true;
            }
            LOGDETAIL2("Writing value " << value << " to " << file << " failed " << _readfile(file));
            return false;
        }
        catch (std::exception& e)
        {
            LOGERR(__PRETTY_FUNCTION__ <<" std exception "<< e.what());
        }
        return false;
    }


    void BusCore::initlib(const char* userhome)
    {
    }

}

