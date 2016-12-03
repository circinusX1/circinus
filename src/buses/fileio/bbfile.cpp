/* Copyright (C) 2018 Pegmatis.Inc - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the APACHE license, (author: Marius Chincisan)
*
* You should have received a copy of the APACHE license with
* this file. If not, please visit www.github.com, Author Marius Chincisan
*/

#include <sys/stat.h>
#include "../../main.h"
#include "bbfile.h"
#include "logs.h"

namespace BusNs
{
BBFile::BBFile(const char* file)
{
    this->_portSysFs = file;
    this->_sf = this->_portSysFs;
    _load();
}

BBFile::~BBFile()
{
    _entries.clear();
}

bool BBFile::write(int specific, uint8_t *reg_data, uint16_t length)
{
    _verb = (const char*)reg_data;
    if(_verb=="\r" || _verb=="\r\n" )
    {
        _verb="prompt";
    }
    else
    {
        size_t crlf = _verb.find('\r');
        if(crlf != std::string::npos)
        {
            _verb=_verb.substr(0,crlf);
        }
        crlf = _verb.find('=');
        if(crlf != std::string::npos)
        {
            std::string key = _verb.substr(0,crlf);
            std::string val = _verb.substr(crlf+1);
            std::map<std::string, std::string >::iterator it = _entries.find(key);
            if(it != _entries.end())
            {
                _entries[key]=val;
                _save();
            }
        }
    }
    return true;
}

int  BBFile::read(uint8_t *reg_data, uint16_t length)
{
#if defined (__linux__)
    struct stat64 fstat;
    ::stat64(_portSysFs.c_str(), &fstat);
#endif

#if defined (__FreeBSD__)
    struct stat fstat;
    ::stat(_portSysFs.c_str(), &fstat);
#endif

    if(fstat.st_mtim.tv_sec != _fstat.st_mtim.tv_sec)
    {
        GLOGI("Reloading simulation file");
        _load();
    }

    if(_verb.empty())
    {
        return 0;
    }
    const auto& it = _entries.find(_verb);
    if(it != _entries.end())
    {
        //::sprintf((char*)reg_data,"\r\n%s %s\r\ns2w>",_verb.c_str(), it->second.c_str());
        ::sprintf((char*)reg_data,"%s\r\n%s %s\r\ns2w>",_verb.c_str(), _verb.c_str(), it->second.c_str());
    }
    else
    {
        if(_verb=="help")
        {
            char line[128];
            for (const auto& e:_entries)
            {
                sprintf(line,"%s=%s\n",e.first.c_str(),e.second.c_str());
                if(::strlen((const char*)reg_data) > length - strlen(line))
                {
                    break;
                }
                ::strcat((char*)reg_data,line);
            }
            _verb.clear();
            return  ::strlen((char*)reg_data);
        }
        ::strcpy((char*)reg_data,"E \nerr>");
    }
    _verb.clear();
    return  ::strlen((char*)reg_data);
}

void BBFile::_load()
{
    std::ifstream ifile(_portSysFs.c_str());

    if (ifile.is_open())
    {
        std::string line;
        std::vector<std::string> kv;

        _entries.clear();
        while (getline(ifile, line))
        {
            kv.clear();
            BblCfg::strSpitTrim(line,'=',kv);
            if(kv.size()==2)
            {
                _entries[kv[0]]=kv[1];
            }
        }
        ifile.close();
#if defined __linux__
        ::stat64(_portSysFs.c_str(), &_fstat);
#endif 

#if defined __FreeBSD__
        ::stat(_portSysFs.c_str(), &_fstat);
#endif 

    }
    else {
        GLOGFATAL("File:" << _portSysFs << " cannot be opened");
    }
}

void BBFile::_save()
{
    std::ofstream ofile(_portSysFs.c_str());

    if (ofile.is_open())
    {
        for (const auto& e:_entries)
        {
            char line[128];
            sprintf(line,"%s=%s\n",e.first.c_str(),e.second.c_str());
            ofile.write(line, ::strlen(line));
        }
        ofile.write("\n", 1);
        ofile.close();
    }
}

}
