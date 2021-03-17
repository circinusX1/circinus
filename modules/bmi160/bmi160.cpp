/*
THIS SOFTWARE IS PROVIDED BY mcomariusATg.ailDOTcom 'AS IS' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL mcomariusATgmailDOTcom BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*/

#include <assert.h>
#include <unistd.h>
#include "sqratUtil.h"
#include "bmi160.h"
#include "_bmi160.h"

sq_api*             SQ_PTRS;
static IoOps*       __pobj;
static IInstance*   __pi;
static HSKVM        __vm;

/* required C function by bosh sensor driver implementation */
void linux_delay(uint32_t period)
{
    ::usleep(period*1000);
}

int8_t linux_write(uint8_t devaddr, uint8_t regdevaddr, uint8_t *reg_data, uint16_t length)
{
    if(__pobj->iopen())
    {
        __pobj->bwrite(reg_data, (int)length, (int)regdevaddr);
        __pobj->iclose();
        return 0;
    }
    return -1;
}

int8_t  linux_read(uint8_t devaddr, uint8_t regdevaddr, uint8_t *reg_data, uint16_t length)
{
    if(__pobj->iopen())
    {
        __pobj->bread(reg_data, (int)length, (int)regdevaddr);
        __pobj->iclose();
        return 0;
    }
    return -1;
}

/**
 * @brief Bmi160::Bmi160
 * @param dev - the device id-name used by BME sensor.
 * @param name-key for the sensor.
 */
Bmi160::Bmi160(const char* dev, const char* name):_name(name)
{
    SHALL_CTOR();
}

Bmi160::~Bmi160()
{
    SHALL_DTOR();
}

/**
 * @brief Bmi160::acceleration
 * @return  array of 3 values with acceleration
 */
Sqrat::Array	Bmi160::acceleration()
{
	get_ar((double*)_acc,(double*)_rot, &linux_read, &linux_write, &linux_delay);
	Sqrat::Array a(__vm, 3);
	a.SetValue(0 , _acc[0]);
	a.SetValue(1 , _acc[1]);
	a.SetValue(2 , _acc[2]);
    _dirt=true;
	return a;
}

/**
 * @brief Bmi160::rotation
 * @return  array of 3 values with rotation
 */
Sqrat::Array	Bmi160::rotation()
{
	get_ar((double*)_acc,(double*)_rot, &linux_read, &linux_write, &linux_delay);
	Sqrat::Array a(__vm, 3);
	a.SetValue(0 , _rot[0]);
	a.SetValue(1 , _rot[1]);
	a.SetValue(2 , _rot[2]);
    _dirt=true;
	return a;

}

/**
 * @brief Bmi160::squit
 * @param name of the class exposed to the script.
 */
void  Bmi160::squit(const char* name)
{
	Sqrat::Class<Bmi160> cls(__vm, name);
	cls.Ctor<const char*, const char*>();
	cls.Functor(_SC("acceleration"), &Bmi160::acceleration);
	cls.Functor(_SC("rotation"), &Bmi160::rotation);
	Sqrat::RootTable(__vm).Bind(_SC(name), cls);
}

void Bmi160::on_event(E_VENT e, const uint8_t* buff, int len, int options=0)
{
}

/**
 * @brief Bmi160::name
 * @return  the name
 */
const char* Bmi160::name()const
{
    return _name.c_str();
}


bool Bmi160::notify_ifdirty(time_t t)
{
    return _dirt;
}

/**
 * @brief Bmi160::dev_key
 * @return the unique id name
 */
const char* Bmi160::dev_key()const
{
    return _name.c_str();
}

/**
 * @brief Bmi160::object
 * @return the script associated instance if is used
 */
Sqrat::Object Bmi160::object()const
{
    return _o;
}

/**
 * @brief Bmi160::set_value called from the restfull web interface
 * @param key
 * @param value
 * @return the sensor does not support set.
 */
bool  Bmi160::set_value(const char* key, const char* value)
{
    return false;
}

/**
 * @brief Bmi160::get_value, called from the restfull web interface
 * @param key
 * @return the string represenstion of the value
 */
const char* Bmi160::get_value(const char* key)
{
	if(!strcmp(key,"acceleration"))
	{
		::sprintf(_retbuff,"%4.4f,%4.4f,%4.4f",_acc[0],_acc[1],_acc[2]);
	}
	else if(!strcmp(key,"rotation"))
	{
		::sprintf(_retbuff,"%4.4f,%4.4f,%4.4f",_rot[0],_rot[1],_rot[2]);
	}
	else {
		::sprintf(_retbuff,"%4.4f,%4.4f,%4.4f,%4.4f,%4.4f,%4.4f",
					_acc[0],_acc[1],_acc[2], _rot[0],_rot[1],_rot[2]);
	}
	return _retbuff;
}

/**
 * @brief Bmi160::get_data
 * @return the abstract data devdata_t
 */
const devdata_t& Bmi160::get_data()const
{
    return _data;
}

/**
 * @brief Bmi160::sync, sync the device with _data variable.
 * @param filter, not used here
 */
void  Bmi160::sync(const char* filter)
{
	get_ar((double*)_acc,(double*)_rot, &linux_read, &linux_write, &linux_delay);
	_data.clear();
	_data.typeit(eREAL,0);
	_data.pusht(_acc[0]);
	_data.pusht(_acc[1]);
	_data.pusht(_acc[2]);
	_data.pusht(_rot[1]);
	_data.pusht(_rot[2]);
	_data.pusht(_rot[3]);
}

/**
		The way of exporting the class to the script
*/

#ifdef __cplusplus
extern "C" {
#endif

IMPLEMENT_START_MODULE(Bmi160);

#ifdef __cplusplus
}
#endif
