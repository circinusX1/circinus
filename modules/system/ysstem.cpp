
#include <assert.h>
#include <unistd.h>
#include "sqratUtil.h"
#include "_sysimpl.h"
#include "ysstem.h"


MODULE_VARS();


OsSys::OsSys(const char* dev, const char* name):I_IDev(dev,name)
{
    MUST_CALL();
}

OsSys::~OsSys()
{

}


void  OsSys::on_event(E_VENT e, const uint8_t* buff, int len, int options=0)
{
}

/**
 * @brief OsSys::squit
 * @param name of the class exposed to the script.
 */

void  OsSys::squit(const char* name)
{
	printf("SQRAT CLASS = %s \n", name);
	Sqrat::Class<OsSys> cls(__vm, name);
	cls.Ctor<const char*, const char*>();

	cls.Functor(_SC("pwd"), &OsSys::pwd);
	cls.Functor(_SC("cd"), &OsSys::cd);
        
    Sqrat::RootTable(__vm).Bind(_SC(name), cls);
}

/**
 * @brief OsSys::name
 * @return  the name
 */
const char* OsSys::name()const
{
    return _name.c_str();
}

/**
 * @brief OsSys::dev_key
 * @return the unique id name
 */
const char* OsSys::dev_key()const
{

    return _name.c_str();
}

/**
 * @brief OsSys::object
 * @return the script associated instance if is used
 */
Sqrat::Object OsSys::object()const
{
    return _ratobj;
}

/**
 * @brief OsSys::is_dirty
 * @param t  further usage
 * @return if this device is monitorred periodically for value changes.
 */
bool  OsSys::_mon_cbacks_call(time_t t)
{
    return false;
}

/**
 * @brief OsSys::set_value called from the restfull web interface
 * @param key
 * @param value
 * @return the sensor does not support set.
 */
bool  OsSys::set_value(const char* , const char* )
{
    return false;
}

/**
 * @brief OsSys::get_value, called from the restfull web interface
 * @param key
 * @return the string represenstion of the value
 */
const char* OsSys::get_value(const char* key)
{
	return "";
}

/**
 * @brief OsSys::get_data
 * @return the abstract data devdata_t
 */
const devdata_t& OsSys::get_data()const
{
    return _data;
}

/**
 * @brief OsSys::sync, sync the device with _data variable.
 * @param filter, not used here
 */
void  OsSys::sync(const char*)
{
}


const char* OsSys::pwd()
{
	return "";
}

bool  OsSys::cd(const char* wd)
{
	return true;
}


/**
		The way of exporting the class to the script
*/

#ifdef __cplusplus
extern "C" {
#endif

IMPLEMENT_START_MODULE(OsSys);

#ifdef __cplusplus
}
#endif
