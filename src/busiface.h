#ifndef BUSIFACE_H
#define BUSIFACE_H


#include "buses/gpio/busgpio.h"
#include "buses/i2c/busi2c.h"
#include "buses/pwm/buspwm.h"
#include "buses/adc/busadc.h"


using namespace BusNs;


class BusIface
{
public:
    typedef enum _BUS{
        SPI=0,
        I2C,
        GPIO,
        PWMM,
        ADC,
        GPIOC,
    }BUS;

    BusIface(BusIface::BUS, I2C_NUM i2cbus, int i2caddress, PIN_DIR pd);
    BusIface(BusIface::BUS, const char*, LOGICV v, PIN_DIR pd);
    BusIface(BusIface::BUS, const char*, LOGICV v, PIN_DIR d, int rev);
    BusIface(BusIface::BUS, const char*, int period, int duty, PIN_DIR pd);
    BusIface(BusIface::BUS, const char*, int period, int duty, PIN_DIR pd, int inv);

    virtual ~BusIface();
    const std::string sysfile()const;
    void    setaddr(uint8_t addr);
    bool    busopen(int mode=0);
    void    busclose();
    int     busread(uint8_t* buff, int len, int anyreg=0);
    bool    buswrite(const uint8_t* buff, int len, int anyreg=0);
    bool    buswrite(int state);
    void    busflush();

protected:
    const char*     _sbus()const;
    const char*     _sdir()const;

protected:
    BusPwm*         _pwm  = nullptr;
    BusI2c*          _pi2c = nullptr;
    BusGpio*        _pgpio = nullptr;
    BusAdc*         _padc = nullptr;

protected:
    int             _bus=0;
    int             _def=0;
    bool            _ok=false;
    PIN_DIR         _dire = IN_PIN;
    std::string     _system;
};
#endif // BUSIFACE_H
