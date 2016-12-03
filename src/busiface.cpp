#include "busiface.h"


#include "main.h"
#include "busiface.h"

BusIface::BusIface(BusIface::BUS t, I2C_NUM i2cbus, int i2caddress, PIN_DIR d):_bus(t),_dire(d)
{
    _pi2c = new BusI2c(i2cbus, i2caddress);
}

BusIface::BusIface(BusIface::BUS t, uint32_t pin, LOGICV v, PIN_DIR d):_bus(t),_dire(d)
{
    _pgpio = new BbbGpio(pin, d);
    if(d != IN_PIN){
        _pgpio->setValue(std::to_string(v).c_str());
        _def = v;
    }else{
        //std::string pullmode = config-pin $pin $mod;
    }
}

BusIface::BusIface(BusIface::BUS t, uint32_t pin, LOGICV v, PIN_DIR d, int rev):_bus(t),_dire(d)
{
    _pgpio = new BbbGpio(pin, d);
    if(d != IN_PIN){
        if(rev)
            _pgpio->setValue(std::to_string(!(int(v))).c_str());
        else
            _pgpio->setValue(std::to_string(v).c_str());
        _def = v;
    }
    else
    {
        //pull TODO
    }
}

BusIface::BusIface(BusIface::BUS t, pwmName pwmpin, int period,
                                         int duty,
                                         PIN_DIR d):_bus(t),_dire(d)
{
   _pwm = new BusPwm(pwmpin);
   _pwm->setPeriod(period);
   _def = (period * duty) / 100 ;
   _pwm->setDutyCycle( _def);
}

BusIface::BusIface(BusIface::BUS t, pwmName pwmpin, int period,
                                         int duty,
                                         PIN_DIR d, int inverse):_bus(t),_dire(d)
{
   _pwm = new BusPwm(pwmpin);
   _pwm->setPeriod(period);
   _pwm->setInverse(inverse);
   _def = (period * duty) / 100 ;
   _pwm->setDutyCycle( _def);
}



BusIface::BusIface(BusIface::BUS b, adcName adc, PIN_DIR d):_bus(b),_dire(d)
{
    _padc =  new BbbAdc(adc);
}

const std::string BusIface::sysfile()const
{
    if(_pwm)
        return _pwm->sysfile();
    if(_pi2c)
        return _pi2c->sysfile();
    if(_pgpio)
        return _pgpio->sysfile();
    if(_padc)
        return _padc->sysfile();
    return "/dev/null";
}

bool    BusIface::busopen(int mode)
{
    return _ok=true;
}

void    BusIface::busclose()
{
    _ok=false;
}

void    BusIface::setaddr(uint8_t addr)
{
    if(_pi2c){
        _pi2c->setAddr(addr);
    }
}

int    BusIface::busread(uint8_t* buff, int len, int anyreg)
{
    if(_pwm)
    {
        ::sprintf((char*)buff,"%d", _pwm->getDutyCycle());
        return 1;
    }
    if(_pi2c){
        return _pi2c->read(anyreg, buff, len);
    }
    if(_pgpio){
        std::string s = _pgpio->getValue();
        ::strcpy((char*)buff, s.c_str());
        return 1;
    }
    if(_padc){
        std::string s = _padc->getValue();
        ::strcpy((char*)buff, s.c_str());
        return 1;
    }
    return -1;
}

bool    BusIface::buswrite(const uint8_t* buff, int len, int anyreg)
{
    if(PCTX->_Offline)
    {
        return true;
    }

    if(_pi2c){
        return _pi2c->write(anyreg, (uint8_t*)buff, len);
    }
    return false;
}

bool    BusIface::buswrite(int state)
{
    if(_pgpio){
        return _pgpio->setValue(std::to_string(state).c_str());
    }
    else if(_pwm){
        _pwm->setDutyCycle(state);
        return true;
    }
    return false;
}

const char* BusIface::_sbus()const
{
    static const char *buses[]={"spi","i2c","gpio","pwmm","adc","gpioc","sq","out of bounds",0};
    return buses[_bus];
}


const char* BusIface::_sdir()const
{
    static const char *directs[]={"-","r","w","rw","","",""};
    return directs[_dire];
}

void BusIface::busflush()
{
    if(_puart){
        return _puart->flushRXTX();
    }
}


BusIface::~BusIface()
{
    _ok=false;
    delete _pwm;
    delete _pi2c;
    delete _pgpio;
    delete _padc;
}


/*
s8 BusIface::I2C_read(u8 device_addr,
                         u8 register_addr,
                         u8 *register_data,
                         u8 read_length, void* pthis)
{
    return ((BusIface*)pthis)->_i2c.readBlock(register_addr, register_data, read_length);
}

s8 BusIface::I2C_write(u8 device_addr,
                          u8 register_addr,
                          u8 *register_data,
                          u8 w_length, void* pthis)
{
    return ((BusIface*)pthis)->_i2c.writeBlock(register_addr, register_data, w_length);
}
*/

