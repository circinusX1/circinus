#include "wiringpi.h"

const char* wiringpi::_wpilib ="/usr/lib/libwiringPi.so";
void* wiringpi::_handle = 0;


struct wpi_func _wpi_foos[]={
{"wiringPiVersion", 0},
{"wiringPiSetup", 0},
{"wiringPiSetupSys", 0},
{"wiringPiSetupGpio", 0},
{"wiringPiSetupPhys", 0},
{"pinModeAlt", 0},
{"pinMode", 0},
{"pullUpDnControl", 0},
{"digitalRead", 0},
{"digitalWrite", 0},
{"digitalRead8", 0},
{"digitalWrite8", 0},
{"pwmWrite", 0},
{"analogRead", 0},
{"analogWrite", 0},
{"wiringPiSetupPiFace", 0},
{"wiringPiSetupPiFaceForGpioProg", 0},
{"wiringPiISR", 0},
{"delay", 0},
{"delayMicroseconds", 0},
{"millis", 0},
{"micros", 0},
{0,0}
};


wiringpi::wiringpi()
{

}

bool wiringpi::load_library();
{
union
{
    void *p;
    void (*fp)();
} u;

_handle =::dlopen (_wpilib, RTLD_LAZY);
if (!_handle)
{
    return false;
}
struct wpi_func* pw =_wpi_foos;
for (fp = sw; fp->name != 0; fp++)
{
    u.p =::dlsym(_handle, fp->name);
    if (u.fp == 0)
    {
        LOGE("Cannot load function:"<< fp->name);
    }
    else
    {
        fp->ptr = u.fp;
    }
}

void wiringpi::unload_library()
{
    dlclose(_handle);
}


int wiringpi::wiringPiSetup ()
{
}

int wiringpi::wiringPiSetupGpio ()
{
}

int wiringpi::wiringPiSetupPhys ()
{
}

int wiringpi::wiringPiSetupSys ()
{
}

unsigned int wiringpi::millis ()
{
}

unsigned int wiringpi::micros ()
{
}

void wiringpi::delay (unsigned int howLong)
{
}

void wiringpi::delayMicroseconds (unsigned int howLong)
{
}

int wiringpi::wiringPiISR (int pin, int edgeType, void (*function)())
{
}

bool wiringpi::pinMode (int pin, int mode)
{
}

bool wiringpi::pullUpDnControl (int pin, int pud)
{
}

bool wiringpi::digitalWrite (int pin, int value)
{
}

int wiringpi::pwmWrite (int pin, int value)
{
}

int  wiringpi::digitalRead (int pin)
{
}

int  wiringpi::analogRead(int pin)
{
}

bool wiringpi::analogWrite(int pin, int value)
{
}
