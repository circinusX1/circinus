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

#ifndef FAST_MATH
#define FAST_MATH

#include <stdint.h>
extern const char gDigitsLut[200];

#define BEGIN2(n) \
    do { \
    int t = (n); \
    if(t < 10) *p++ = '0' + t; \
    else { \
    t *= 2; \
    *p++ = gDigitsLut[t]; \
    *p++ = gDigitsLut[t + 1]; \
    } \
    } while(0)
#define MIDDLE2(n) \
    do { \
    int t = (n) * 2; \
    *p++ = gDigitsLut[t]; \
    *p++ = gDigitsLut[t + 1]; \
    } while(0)
#define BEGIN4(n) \
    do { \
    int t4 = (n); \
    if(t4 < 100) BEGIN2(t4); \
    else { BEGIN2(t4 / 100); MIDDLE2(t4 % 100); } \
    } while(0)
#define MIDDLE4(n) \
    do { \
    int t4 = (n); \
    MIDDLE2(t4 / 100); MIDDLE2(t4 % 100); \
    } while(0)
#define BEGIN8(n) \
    do { \
    uint32_t t8 = (n); \
    if(t8 < 10000) BEGIN4(t8); \
    else { BEGIN4(t8 / 10000); MIDDLE4(t8 % 10000); } \
    } while(0)
#define MIDDLE8(n) \
    do { \
    uint32_t t8 = (n); \
    MIDDLE4(t8 / 10000); MIDDLE4(t8 % 10000); \
    } while(0)
#define MIDDLE16(n) \
    do { \
    uint64_t t16 = (n); \
    MIDDLE8(t16 / 100000000); MIDDLE8(t16 % 100000000); \
    } while(0)

void u32toa_branchlut2(uint32_t x, char* p) {

    if(x < 100000000) BEGIN8(x);
    else { BEGIN2(x / 100000000); MIDDLE8(x % 100000000); }
    *p = 0;

}
void i32toa_branchlut2(int32_t x, char* p) {

    uint64_t t;
    if(x >= 0) t = x;
    else *p++ = '-', t = -uint32_t(x);
    u32toa_branchlut2(t, p);

}
void u64toa_branchlut2(uint64_t x, char* p) {

    if(x < 100000000) BEGIN8(x);
    else if(x < 10000000000000000) { BEGIN8(x / 100000000); MIDDLE8(x % 100000000); }
    else { BEGIN4(x / 10000000000000000); MIDDLE16(x % 10000000000000000); }
    *p = 0;

}

void i64toa_branchlut2(int64_t x, char* p) {

    uint64_t t;
    if(x >= 0) t = x;
    else *p++ = '-', t = -uint64_t(x);
    u64toa_branchlut2(t, p);

}



#endif //FAST_MATH


