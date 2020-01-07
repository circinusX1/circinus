#ifndef _BMP180IMPL_H
#define _BMP180IMPL_H

#include <stdbool.h>
class IoOps;
bool read_values(IoOps* pi2c, int& t, int& h, int& p);

#endif // _BMP180IMPL_H
