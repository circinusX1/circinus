/* Copyright (C) Marius C zirexix ltd
* You may use, distribute and modify this code under the
* terms of the APACHE license,
*
* You should have received a copy of the APACHE license with
* this file. If not, please visit www.github.com
*/

#ifndef BUSCONFIG_H
#define BUSCONFIG_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

class BusConfig
{
    std::vector<std::string> _vout;
    std::string _out;
    std::string _def;

public:
    BusConfig();

    const char* operator [] (const char* key);

    static size_t split(const std::string& s, char delim, std::vector<std::string>& a)
    {
        std::string st;
        std::istringstream ss(s);
        a.clear();
        while (getline(ss, st, delim))
        {
            a.push_back(st);
        }
        return a.size();
    }

    // trim from start (in place)
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

};

extern BusConfig GCONF;
extern bool ConfError;
#endif // BUSCONFIG_H
