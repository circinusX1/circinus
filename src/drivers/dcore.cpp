/*
Copyright (c) 2014-2016 comarius.DPT All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/comarius. The name of the
https://github.com/comarius may not be used to endorse or promote
products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <fcntl.h>
#include <poll.h>
#include "../main.h"
#include "dcore.h"
#include "dlconfig.h"
#include "logs.h"


namespace GenericHw
{


DvCore::DvCore()
{

}

DvCore::~DvCore()
{
}

std::string DvCore::sh_exec(std::string command)
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



int DvCore::_readint(const std::string& file)
{
    int   ret = -1;
    _err.clear();
    FILE* pf = ::fopen(file.c_str(),"rb");
    if(pf)
    {
        ret = ::fread(&ret, sizeof(int), 1, pf);
        ::fclose(pf);
    }
    _err = strerror(errno);
    LOGE(_err << ", " << file);
    return ret;
}

size_t DvCore::_red(const std::string& file, uint8_t* pb, size_t l)const
{
    size_t   ret = 0;
    DvCore* ptr =  const_cast<DvCore*> (this);
    ptr->_err.clear();
    FILE* pf = ::fopen(file.c_str(),"rb");
    if(pf)
    {
        ret = ::fread(pb, sizeof(uint8_t), l, pf);
        ::fclose(pf);
    }
    ptr->_err = strerror(errno);
    LOGE(ptr->_err << ", " << file);
    return ret;
}

bool DvCore::_wrt(const std::string& file, const char* value, bool check)
{
    size_t ret;
    size_t len;

    _err.clear();
    FILE* pf = ::fopen(file.c_str(),"wb");
    if(pf)
    {
        len = ::strlen(value);
        ret = ::fwrite(value, 1, len, pf);
        ::fclose(pf);
        return ret==len;
    }
    _err = strerror(errno);
    LOGE(__FUNCTION__ << ": " << _err << " @ " << file );
    return false;
}

const std::string& DvCore::_red(const std::string& file)
{
    std::ifstream expFile;

    _content = S_NAN;
    _err.clear();
    try{
        expFile.open( file, std::ios::in);
        if(expFile.is_open())
        {
            LOGD3(__PRETTY_FUNCTION__ << " reading from " << file);
            expFile >> _content;
            expFile.close();
            if(!_content.empty())
            {
                size_t eos = _content.find_first_of('\n');
                if(eos != std::string::npos)
                {
                    _content = _content.substr(0,eos);
                }
            }
        }
        else
        {
            LOGW(__PRETTY_FUNCTION__ << " Cannot read: " << file);
            _err = strerror(errno);
        }
    }
    catch (std::exception& e)
    {
        LOGE(__PRETTY_FUNCTION__ <<" std exception "<< e.what());
        _err = e.what();
    }
    return _content;
}

bool DvCore::_config(const char* key, std::string& val)
{
	const char* s = CFL(CFGF,key);
	if(*s) {val  = s; return true;}
	return false;
}

}
