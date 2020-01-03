#ifndef AM335_H
#define AM335_H

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>

class gpio_chip
{
protected:
    gpio_chip(uint8_t na=0);
    virtual ~gpio_chip();

protected:
    static bool open_lib();
    static void close_lib();
    static const PIN* _get_pin(int pin);

protected:
    bool _pinMode(const PIN& pin,
                  unsigned char direction,
                  unsigned char mux,
                  unsigned char pull);
    bool _digitalWrite(const PIN& p, uint8_t mode);
    int  _digitalRead(const PIN& p);
    int  _analogRead(const PIN& p);

private:
    static uint32_t*    _mbase;
    static int      _initialized;
};

#endif // AM335_H
