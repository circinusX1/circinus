/* Copyright (C) Marius C zirexix ltd
* You may use, distribute and modify this code under the
* terms of the APACHE license,
*
* You should have received a copy of the APACHE license with
* this file. If not, please visit www.github.com
*/


#include "../../main.h"
#include "busi2c.h"

namespace BusNs
{

    BusI2c::BusI2c(I2C_NUM i2c, uint8_t addr):_addr(addr)
    {
        std::string i2cpath = GCONF[ "i2c"];
        this->_portSysFs = i2cpath + tostr(static_cast<int>(i2c));
        this->_sysfile   = this->_portSysFs;
    }

    BusI2c::~BusI2c()
    {

    }

    void   BusI2c::setAddr(uint8_t addr)
    {
        _addr = addr;
    }

    bool BusI2c::write(int reg_addr, uint8_t *reg_data, uint16_t length)
    {
        if(PCTX->_Offline)
        {
            //GLOGX("Simulation Mode");
            return true;
        }

        int file = ::open(_sysfile.c_str(), O_RDWR);
        _error.clear();
        if(file < 0)
        {
              LOGDETAIL2("Error open I2c: " << _sysfile << ": " << int(_addr) << "[" << int(reg_addr) << "]"  << ", "<< strerror(errno) );
              ++_repeatedErr;
              return false;
        }
        int err = ioctl(file, I2C_SLAVE, _addr);
        if(err)
        {
            LOGDETAIL2("Error I2C_SLAVE i2c: "<<  _sysfile << ": " << int(_addr) << "[" << int(reg_addr) << "]"  << ", "<<  strerror(errno) );
            ::close(file);
            ++_repeatedErr;
            return false;
        }

        struct i2c_rdwr_ioctl_data packets;
        struct i2c_msg messages[1];

        uint8_t buff[length + 1];
        buff[0] = reg_addr;

        LOGDETAIL2("i2C->" << int(_addr) << "["<<reg_addr<<"]");
        uint16_t i;
        for ( i = 0; i < length; i++)
        {
            buff[i + 1] = reg_data[i];
            LOGDETAIL2(std::hex << int(buff[i]) << std::dec) ;
        }
        LOGDETAIL2(std::hex << int(buff[i]) << std::dec) ;
        LOGDETAIL2("----");

        messages[0].addr = _addr;
        messages[0].flags = 0;
        messages[0].len = length + 1;
        messages[0].buf = buff;

        packets.msgs = messages;
        packets.nmsgs = 1;
        if (ioctl(file, I2C_RDWR, &packets) < 0)
        {
            LOGDETAIL2("ErrorW I2C_RDWR i2c: " << _sysfile <<", "<< _sysfile << ": " << int(_addr) << "[" << int(reg_addr) << "]" << ", "<< length  << "bytes , " << strerror(errno) );
            ::close(file);
            return false;
        }
        ::close(file);
        _repeatedErr=0;
        return true;
    }

    bool  BusI2c::read(int reg_addr, uint8_t *reg_data, uint16_t length)
    {
        if(PCTX->_Offline)
        {
            //GLOGX("Simulation Mode");
            uint32_t  some = rand() % 100;
            memcpy(reg_data, &some, length);
            return true;
        }


        _error.clear();
        int file = ::open(_sysfile.c_str(), O_RDWR);
        if(file < 0)
        {
            LOGDETAIL2("Failed to open I2c:" <<  strerror(errno) );
            ++_repeatedErr;
            return false;
        }
        int err = ioctl(file, I2C_SLAVE, _addr);
        if(err)
        {
            LOGDETAIL2("Error I2C_SLAVE i2c:" << _sysfile << ": " << int(_addr) << "[" << int(reg_addr) << "]" << ", "<< strerror(errno) );
            ++_repeatedErr;
            ::close(file);
            return false;
        }

        struct i2c_rdwr_ioctl_data packets;
        struct i2c_msg messages[2];

        /*
         * In order to read a register, we first do a "dummy write" by writing
         * 0 bytes to the register we want to read from.  This is similar to
         * the packet in set_i2c_register, except it's 1 byte rather than 2.
         */
        messages[0].addr = _addr;
        messages[0].flags = 0;
        messages[0].len = 1;
        messages[0].buf = (__u8*)&reg_addr;

        /* The data will get returned in this structure */
        messages[1].addr = _addr;
        messages[1].flags = I2C_M_RD;///* | I2C_M_NOSTART*/;
        messages[1].len = length;
        messages[1].buf = reg_data;

        GLOGD("ioctling I2C_RDWR addr:" << _sysfile << ": " << int(_addr) << "[" << int(reg_addr) << "]"  << ", reg: " << int(reg_addr));

        /* Send the request to the kernel and get the result back */
        packets.msgs = messages;
        packets.nmsgs = 2;
        if (ioctl(file, I2C_RDWR, &packets) < 0)
        {
            LOGDETAIL2("ErrorR: I2C_RDWR i2c:" <<  _sysfile << ": " << int(_addr) << "[" << int(reg_addr) << "]" << ", " << length << " bytes, " << strerror(errno) );
            ::close(file);
            ++_repeatedErr;
            return false;
        }
        ::close(file);
        _repeatedErr=0;

        LOGDETAIL2("<-i2C" << int(_addr) << "["<<reg_addr<<"]");
        if(RunCtx::_Debug & 0x100)
        {
            for(int i=0;i<length;i++){
                LOGDETAIL2(std::hex << int(reg_data[i]) << std::dec);
            }
        }
        LOGDETAIL2("----");
        return true;
    }
}
