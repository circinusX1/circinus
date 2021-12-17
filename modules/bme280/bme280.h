#ifndef BMP180_H
#define BMP180_H

#include <string.h>
#include "../iper.h"


class EXPORT Bme280 : public I_IDev
{
public:
    Bme280(const char* dev, const char* name);
    virtual ~Bme280();
    static void   squit(const char* name);
    Sqrat::Array  acceleration();
    Sqrat::Array  rotation();


    int	temperature();
    int humidity();
    int pressure();
    void monitor(bool);
    Sqrat::Array thp();

    ALL_VIRTUALS();


private:
    char            _retbuff[32];
    int             _tph[3];
    bool            _dirty = false;
    bool            _montorit = false;
};

#endif // BMP180_H
