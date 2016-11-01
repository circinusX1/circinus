/* Copyright (C) Marius C zirexix ltd
* You may use, distribute and modify this code under the
* terms of the APACHE license,
*
* You should have received a copy of the APACHE license with
* this file. If not, please visit www.github.com
*/


#include "busconfig.h"
#include "main.h"

bool ConfError=true;


BusConfig::BusConfig()
{
    //
    // startup script to generate config pins
    //
    std::string file = "/tmp/ks.config";
    if(::access(file.c_str(),0)==0)
        goto FOUND;
    file = "/etc/ks/ks.config";
    if(::access(file.c_str(),0)==0)
        goto FOUND;
    file = getenv("HOME"); file+="/.ks/ks.config";
    if(::access(file.c_str(),0)==0)
        goto FOUND;
    return;

FOUND:
    std::ifstream s;
    std::string ss;
    s.open( file.c_str(), std::ios::in);
    if(s.is_open())
    {
        while(!s.eof()){
            s >> ss;
            _vout.push_back(ss);
            ss.clear();
        }
        s.close();
        ConfError=false;
    }
}

/**
 * @brief BusConfig::operator const char *
 *
 * parses a file a=b no spaces no sections, simple a=b
 *
 *
 */

const char* BusConfig::operator [] (const char* key)
{
    for(const auto& a : _vout)
    {
        std::string line = a;
        if(a[0]=='#')continue;
        std::vector<std::string> kv;
        size_t coment = a.find_first_of('#');
        if(coment != (size_t)-1)
            line=line.substr(0,coment);
        split(line,'=', kv);
        if(kv.size()==2)
        {
            std::string k = kv[0];
            std::string v = kv[1];
            trim(k);
            trim(v);
            if(key==k){
                _out = v;
                break;
            }
        }
    }
    return _out.c_str();
}

BusConfig GCONF;
