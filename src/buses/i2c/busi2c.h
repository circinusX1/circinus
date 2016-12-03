/* Copyright (C) Marius C zirexix ltd
* You may use, distribute and modify this code under the
* terms of the APACHE license,
*
* You should have received a copy of the APACHE license with
* this file. If not, please visit www.github.com
*/

#ifndef WAS_BWASI2C_H_
#define WAS_BWASI2C_H_
/**
  chnage freq for I2C as

  dtc -I dtb  -O dts  -o ~/am335x-boneblack-wireless.dts /boot/dtbs/dtbs/4.9.78-ti-r94/am335x-boneblack-wireless.dtb

  Edit the source file.
Look for I2C definitions with a
clock-frequency parameter. 0x61A80 is 400KHz and
0x186A0 is 100KHz.
Change the second I2C (the one with clock-frequency of 100KHz) to 0x61a80.


  sudo dtc -I dts  -O dtb   -o /boot/dtbs/4.9.78-ti-r94/am335x-boneblack-wireless.dtb ~/am335x-boneblack-wireless.dts

check I2C frequensies :  dmesg | grep 48


*/

#include "../buscore.h"
#include <iostream>

#include <cstring>
#include <string>
#include <fstream>
#include <cstdint>
#include <unistd.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>


namespace BusNs
{


    class BusI2c : public BusCore
    {
        private:
            std::string     _portSysFs;
            uint8_t         _addr;
            std::string     _error;
            int             _repeatedErr = 0;
        public:
            BusI2c(I2C_NUM i2c, uint8_t addr);
            virtual ~BusI2c();
            void setAddr(uint8_t addr);
            bool write(int reg_addr, uint8_t *reg_data, uint16_t length);
            bool read(int reg_addr, uint8_t *reg_data, uint16_t length);
     };
}

#endif /*  */
