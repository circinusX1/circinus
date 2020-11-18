
#include <assert.h>
#include <unistd.h>
#include "sqratUtil.h"
#include "_bme280impl.h"
#include "bme280.h"


sq_api*             SQ_PTRS;
static IoOps*       __pobj;
static IInstance*   __pi;
static HSKVM        __vm;


Bme280::Bme280(const char* dev, const char* name):_name(name)
{
    SHALL_CTOR();
}

Bme280::~Bme280()
{
    SHALL_DTOR();
}

/**
 * @brief Bme280::acceleration
 * @return  array of 3 values with acceleration
 */
int	Bme280::temperature()
{
    if(!_montorit)   sync();
    return _tph[0];
}

int Bme280::humidity()
{
    if(!_montorit)   sync();
    return _tph[1];
}

int Bme280::pressure()
{
    if(!_montorit)   sync();
    return _tph[2];
}

Sqrat::Array Bme280::thp()
{
    if(!_montorit)   sync();
    Sqrat::Array a(__vm, 3);
    a.SetValue(0,_tph[0]);
    a.SetValue(1,_tph[1]);
    a.SetValue(2,_tph[2]);
    return a;
}

/**
 * @brief Bme280::squit
 * @param name of the class exposed to the script.
 */
void  Bme280::squit(const char* name)
{
	printf("SQRAT CLASS = %s \n", name);
	Sqrat::Class<Bme280> cls(__vm, name);
	cls.Ctor<const char*, const char*>();
	cls.Functor(_SC("temperature"), &Bme280::temperature);
	cls.Functor(_SC("pressure"), &Bme280::pressure);
	cls.Functor(_SC("humidity"), &Bme280::humidity);
	cls.Functor(_SC("thp"), &Bme280::thp);
	cls.Functor(_SC("monitor"), &Bme280::monitor);
	Sqrat::RootTable(__vm).Bind(_SC(name), cls);
}

/**
 * @brief Bme280::name
 * @return  the name
 */
const char* Bme280::name()const
{
    return _name.c_str();
}

/**
 * @brief Bme280::dev_key
 * @return the unique id name
 */
const char* Bme280::dev_key()const
{

    return _name.c_str();
}

/**
 * @brief Bme280::object
 * @return the script associated instance if is used
 */
Sqrat::Object Bme280::object()const
{
    return _o;
}

void Bme280::monitor(bool m)
{
    _montorit = m;
}
/**
 * @brief Bme280::is_dirty
 * @param t  further usage
 * @return if this device is monitorred periodically for value changes.
 */
bool  Bme280::is_dirty(size_t)
{
    if(_montorit)
    {
        sync();
        bool dirt = _dirty;
        _dirty = false;
        return dirt;
    }
    return false;
}

/**
 * @brief Bme280::set_value called from the restfull web interface
 * @param key
 * @param value
 * @return the sensor does not support set.
 */
bool  Bme280::set_value(const char* , const char* )
{
    return false;
}

/**
 * @brief Bme280::get_value, called from the restfull web interface
 * @param key
 * @return the string represenstion of the value
 */
const char* Bme280::get_value(const char* key)
{
	if(!strcmp(key,"temperature"))
	{
		::sprintf(_retbuff,"%d",_tph[0]);
	}
	else if(!strcmp(key,"humidity"))
	{
		::sprintf(_retbuff,"%d",_tph[1]);
	}
	else if(!strcmp(key,"pressure"))
	{
		::sprintf(_retbuff,"%d",_tph[2]);
	}
	else
	{
		::sprintf(_retbuff,"temp=%d&hum=%d&pres=%d", _tph[0],_tph[1],_tph[2]);
	}
	return _retbuff;
}

/**
 * @brief Bme280::get_data
 * @return the abstract data any_t
 */
const any_t& Bme280::get_data()const
{

    return _data;
}

/**
 * @brief Bme280::sync, sync the device with _data variable.
 * @param filter, not used here
 */
void  Bme280::sync(const char*)
{
	int					tph[3];
	read_values(__pobj, tph[0], tph[1], tph[2]);
	_dirty = ::memcmp(tph, _tph, sizeof(tph));
	::memcpy(_tph, tph, sizeof(tph));
	_data.clear();
	_data.typeit(eINT,0); // slot 0
	_data.typeit(eINT,1); // slot 1
	_data.typeit(eINT,2);  // slot 2
	_data.pusht(_tph[0],0); // add t p and h
	_data.pusht(_tph[1],1);
	_data.pusht(_tph[2],2);

}

void  Bme280::on_event()
{
}


/**
		The way of exporting the class to the script
*/

#ifdef __cplusplus
extern "C" {
#endif

IMPLEMENT_START_MODULE(Bme280);

#ifdef __cplusplus
}
#endif
