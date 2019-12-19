/*
Copyright (c) 2014-2016 Marius.C All rights reserved.

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
#include "dsocket.h"
#include "dlconfig.h"
#include "logs.h"

namespace GenericHw
{
DvSocket::DvSocket(const char* file, int port)
{
	_dev_node = file;
	_port = port;
}

DvSocket::~DvSocket(){iclose();}

bool    DvSocket::iopen(int mode)
{
	if (this->create(_port) > 0)
	{
		return this->try_connect(_dev_node.c_str(),_port) != 0;
	}
	return false;
}

void    DvSocket::iclose()
{
	this->term();
}

size_t     DvSocket::bread(uint8_t* buff, int len, int offset)
{
	int ret = this->select_receive(buff, len, 256);
	if(ret>0) on_event(eREAD, buff, len);
	return ret;
}

int     DvSocket::bwrite(const uint8_t* buff, int len, int offset)
{
	int sent =  this->sendall(buff, len);
	if(sent>0) on_event(eWRITE, buff, sent);
	return sent;
}

void    DvSocket::flush()
{

}

}

