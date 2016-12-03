/* Copyright (C) 2018 Pegmatis.Inc - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the APACHE license, (author: Marius Chincisan)
*
* You should have received a copy of the APACHE license with
* this file. If not, please visit www.github.com, Author Marius Chincisan
*/

#ifndef _BBFILE_H_
#define _BBFILE_H_

#include "buscore.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string>
#include <fstream>
#include <cstdint>
#include <map>


namespace BusNs
{
class BBFile : virtual public BBSysFs
{
private:
    std::string     _portSysFs;
    std::map<std::string, std::string > _entries;
    std::string _verb;
#if defined (__linux__)
	struct stat64 _fstat;
#endif
#if defined (__FreeBSD__)
   	struct stat _fstat;
#endif

public:
    BBFile(const char* fname);
    virtual ~BBFile();
    bool write(int specific, uint8_t *reg_data, uint16_t length);
    int read(uint8_t *reg_data, uint16_t length);

private:
    void _load();
    void _save();
};

}

#endif /*  */
