#ifndef BMP180_H
#define BMP180_H

#include <string.h>
#include "bme280_global.h"
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
	const char* name()const;
	const char* dev_key()const;
	bool  is_dirty(size_t t);
	bool  set_value(const char* key, const char* value);
	const char* get_value(const char* key);
	const any_t& get_data()const;
	void  sync(const char* filter=nullptr);
	Sqrat::Object object()const;


private:
    IoOps*          _ird;
    std::string     _name;
    any_t           _data;
    char            _retbuff[32];
    int             _tph[3];
    bool            _dirty = false;
    Sqrat::Object   _o;
    bool            _montorit = false;
};

#endif // BMP180_H
