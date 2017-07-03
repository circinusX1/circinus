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

#ifndef _X_SCONFIG_H
#define _X_SCONFIG_H

#include <string>
#include <vector>

#if defined (__FreeBSD__)
#ifdef type
#    undef type
#endif 
#endif 

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

#if defined (__FreeBSD__)
#define type(obj) ((obj)._type)
#endif

typedef    std::vector<std::string>     strarray_t;


class CFL
{
    strarray_t _vout;
    std::string _out;
    std::string _key;

public:
    typedef  strarray_t   strvect;
    CFL(const char* file, const char* key):_key(key)
    {
        std::ifstream s;
        std::string ss;
        s.open( file, std::ios::in);
        if(s.is_open())
        {
            while(!s.eof()){
                s >> ss;
                _vout.push_back(ss);
                ss.clear();
            }
            s.close();
        }
    }

	operator const char*()
	{
		std::string k;
		std::string v;
		for(const auto& a : _vout)
		{
			strarray_t kv;
			explode(a,'=', kv);
			if(kv.size()==2)
			{
				k = kv[0];
				v = kv[1];
				str_lrtrm(k);
				str_lrtrm(v);
				if(_key==k){
					_out = v;
					break;
				}
			}
		}
		return _out.c_str();

	}

    static std::string sreplace_all(std::string str,
                                   const std::string& from, const std::string& to)
    {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring_t of 'from'
        }
        return str;
    }

    static size_t explode(const std::string& s, char delim, strarray_t& a, char strip=0)
    {
        std::string st;
        std::istringstream ss(s);
        a.clear();
        while (getline(ss, st, delim))
        {
            if(strip && st.back()==strip)
                st=st.substr(0,st.length()-1);
            a.push_back(st);
        }
        if(a.size()==0)a.push_back(s);
        return a.size();
    }

    static size_t explode_trim(const std::string& s, char delim, strarray_t& a)
    {
        std::string st;
        std::istringstream ss(s);
        a.clear();
        while (getline(ss, st, delim))
        {
            while(st.back()=='\r' || st.back()=='\n')
            {
                st.pop_back();
            }
            a.push_back(st);
        }
        if(a.size()==0)
            a.push_back(s);
        return a.size();
    }

    static size_t explode(const std::string& s, const char* delim, strarray_t& a)
    {
        const char* pdelim=delim;
        std::string st;
        std::istringstream ss(s);
        a.clear();
        while(*pdelim)
        {
            if(s.find(*pdelim)!=std::string::npos)
            {
                while (getline(ss, st, *pdelim))
                {
                    a.push_back(st);
                }
                return a.size();
            }
            ++pdelim;
        }
        a.push_back(s);
        return 1;
    }


    // str_lrtrm from start (in place)
    static inline void str_ltrm(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // str_lrtrm from end (in place)
    static inline void str_rtrm(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // str_lrtrm from both ends (in place)
    static inline void str_lrtrm(std::string &s) {
        str_ltrm(s);
        str_rtrm(s);
    }

};

#endif // BBLCONFIG_H
