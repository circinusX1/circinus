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



#include <sys/stat.h>
#include "main.h"
#include "dfile.h"
#include "dlconfig.h"
#include "logs.h"

namespace GenericHw
{
DvFile::DvFile(const char* file):_pf(nullptr)
{
	_dev_node = file;
}

DvFile::~DvFile(){iclose();}

bool    DvFile::iopen(int mode)
{
	std::string smode;
	_mode = mode;
	if((mode & 0x1))
		smode="rb";
	else if((mode & 0x2))
		smode="wb";
	else if((mode & 0x3))
		smode="rw";
	else if((mode & 0x6))
		smode="a";

	_pf=::fopen(_dev_node.c_str(), smode.c_str());
	return _pf != nullptr;
}

void    DvFile::iclose()
{
	if(_pf)
		::fclose(_pf);
	_pf=0;
}

size_t     DvFile::bread(uint8_t* buff, int len, int offset)
{
	size_t rv = -1;
	if(_mode & O_WRONLY && _pf)
	{
		if(offset>0)
		{
			::fseek(_pf, offset, SEEK_SET);
		}
		rv = ::fread(buff, sizeof(uint8_t), len, _pf);
		if(rv>0) 
			on_event(eREAD, buff, len, offset);
	}
	return rv;
}

int     DvFile::bwrite(const uint8_t* buff, int len, int offset)
{
	if(_mode & O_RDONLY && _pf)
	{
		if(offset>0)
		{
			::fseek(_pf, offset, SEEK_SET);
		}
		int rv = ::fwrite(buff, sizeof(uint8_t), len, _pf);
		if(rv>0) on_event(eWRITE, buff, len, offset);
	}
	return -1;
}

void    DvFile::flush()
{
	if(_pf)
		::fflush(_pf);
}

}

