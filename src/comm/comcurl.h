/*
Copyright (c) 2014-2016 PANGEEA.DPT All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/pangeea. The name of the
https://github.com/pangeea may not be used to endorse or promote
products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifdef WITH_CURL
#ifndef COMCURL_H
#define COMCURL_H

#include "dcore.h"
#include "logs.h"

using namespace GenericHw;
class ComCurl : public DvCore
{
public:
    ComCurl();
};

#endif // COMCURL_H
#endif //#ifdef WITH_CURL
