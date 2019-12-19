

![logo](https://raw.githubusercontent.com/comarius/rembix/master/docs/embixico.png)

## GPIO, PWM, SPI, I2C SCRIPTING LANGUAGE FOR LINUX / FREEBSD 
##                    NO DEPENDENCIES. 
### REACTION TIME ~ 1ms (depending of your code)
##### Get automated systems up and running in a mater of few hours.
   
This is a self contained program written in C++. I start it almost 13 years ago using it 
for a game engine automation (see Getic). Then from controlling a game engine I 
extended to do automation scripting adding UART telnet and ssh (see othe projects here). 
Slowly I added SSH, TCP, I2C, ... GPIO's and so on.

    * Usage:
```
    rembix script_file.src
    
```

Where the script_file is a script like java/c++, but is not java neither c++. 

The intrinsec language features can be browsed at: 

    * http://squirrel-lang.org/   

or from 

   * https://developer.electricimp.com/squirrel/squirrelcrib

Rembix specific API's are distributed electonically as PDF. 
Also they can be picked from the source code if you dont need the manual. 


    * It targets:
        * Raspberry PI
        * Beaglebone(s)
        * iMX6
        * Nano PI
        * Cherry PI
        * C.H.I.P


### Sample code R-PI

```c++
::using(eGPIO|ePWM); // load what we use only 

var l1 = PIO(26,  DIR_OUT, LOW, "led");   // if you add a name the device apears in Json report and database, otherwise not
var l2 = PIO(19,  DIR_OUT, HIGH, "led2");
var l3 = PIO(13,  DIR_OUT, LOW, "led3");
var l4 = PIO(6,   DIR_OUT, HIGH, "led4");
var pb = PIO(21,  DIR_IN,  LOW, "button");
var tone = PIO(17,  440, "buzer");        // this is a regular PIO PIO and will play a tone 'E'  (max 600..700 hz)
var counter = PIO(4, -1000, "count");    // this PIO is connected to the buzzer to measure the frequency.
                                          // can measure up to 500 hz. 
                                         // the -1000 parameter indicates that it counts transitions for 1000 mseconds
var pwm = PWM("0.0", 1000, 100, false, "pwm");  // this is a PWM, on chip 0 pwm 0, running a 1000Hz with default 100% pwm

var json_interface = Json(8078);            // start a TCP Json server on port 8070
var database_sqlite  = Database("/usr/share/embix/db1"); //  will save all devices values in database. (if sqlite is not present on the system, it does nothing then reporting warnings)

var value = 1;
var k = 0;

function main(ctx)
{
    ctx.notify("appname");          // this is just a test
    pb.monitor(true);               // this PIO fires events into the loop when state changes. 
    l1.set_value(0);                // set this led to 0
    db.save_interval(15000);        // will save sensors/PIO's every 15 seconds
    return run_loop(loop,1000);     // run loop() calling it at 1000 ms interval, though when pb state changes
}

function loop(ctx, dev)                // dev is not null when the monitored device has a change in state.
{
    l1.set_value(value); 
    l2.set_value(!value);
    l3.set_value(value);
    l4.set_value(!value);
    pwm.set_duty(k++);
    value=!value;
    if(dev)   
    {
          println("dev = " + dev.get_value()); // dev is set ony when the monitorred pin is triggered
         return false; // end the program  
    }
    println("counter = " + counter.get_freq());  // this will print roughy 440
    return true;
}
```

#### Sample for Beaglebone 

```c++
/*
see GPIOs:
    root@hpp:/sys/class/gpio# cat /sys/kernel/debug/gpio
    gpiochip1: GPIOs 352-359, parent: hid/0003:10C4:EA90.0007, cp2112_gpio, can sleep
*/
setdbg(0xf);
reconfig_sys(eGPIO, ["/sys/class/gpio","gpio%d","gpiochip%d"]); //configure sys paths

var l1 = PIO(26,  DIR_OUT, LOW,  "led");
var l2 = PIO(19,  DIR_OUT, HIGH, "led2");
var l3 = PIO(13,  DIR_OUT, LOW,  "led3");
var l4 = PIO(6,   DIR_OUT, HIGH, "led4");

function main(ctx)
{
    l1.set_value(0);
    l2.set_value(1);
    l3.set_value(1);
    l4.set_value(1);
    return run_loop(loop,1000);
}

function loop(ctx, devs)
{
    return true;
}
```

#### Sample to read some I2c Sensor. 

```c++
/*
    ported from https://github.com/maasdoel/bmp18
   Embix 2017
*/

class BMP180 extends I2C /*this is an exposed class form the engine*/
{
    BMPx8x_I2CADDR  = 0x77;
    BMPx8x_CtrlMeas  = 0xF4;
    BMPx8x_TempConversion  = 0x2E;
    // removed for clarity
    constructor (i2cbus,addr,name)
    {
        bmp_ac1=-1;
        bmp_ac2=-1;
        // removed for clarity
        base.constructor(this, i2cbus, addr, name);
    }

    function   calibrate()
    {
        var  rValue = this.ioread(0xAA,22);
        assert(rValue.len()!=20,"error read");
        this.bmp_ac1=0s((rValue[0]<<8)|rValue[1]);
        // removed for clarity
}
    function   WaitForConversion()
    {
        var  d,counter=0;
        do{
            d = this.ioread(BMP180.BMPx8x_CtrlMeas,1);
            if(d.len()!=1){
                errorln("wait failed");
                return false;
            }
            counter++;
            sleep (100+BMP180.BMPx8x_RetryDelay);


        }while ( ((d[0] & 0x20) != 0)  &&  counter < 20 );
        return true;
    }

    function   bmp_GetPressure()
    {
        this.iowrite(BMPx8x_CtrlMeas,[(BMP180.BMPx8x_PresConversion0 + (BMP180.BMPx8x_OverSampling*64))]);
        sleep (BMP180.BMPx8x_minDelay);
        if (WaitForConversion() == false )
            return -1;
        var  rValues = this.ioread(BMP180.BMPx8x_Results,3);
        // removed for clarity
        p =  0i(p + (x1 + x2 + 3791)/16);
        return (p);
    }

    function   bmp_GetTemperature()
    {
        var  rValues;
         // removed for clarity
         var  result = float((xbmp_b5 + 8.0)/1.6);
        return float(result);
    }
};

function main(ctx)
{
    bmp080 := BMP180(1,BMP180.BMPx8x_I2CADDR,"bmp");

    if(bmp080.open(1))
    {
        bmp080.calibrate();
        return run_loop(loop,[bmp080]);
    }
    return false;
}

function loop(ctx,userdata)
{
    var  dev = userdata[0];
    var  p = dev.bmp_GetPressure();
    var  t = dev.bmp_GetTemperature();
    println("T=" + t + "mC, P="+p + "mB");
    sleep(500);
    return true;
}
```

#### Sample to control OLED96 display

```c++

include("plugins/oled96.js");
/*
sudo nano /lib/udev/rules.d/60-i2c-tools.rules
KERNEL=="i2c-0"     , GROUP="i2c", MODE="0660"
KERNEL=="i2c-[1-9]*", GROUP="i2c", MODE="0666"
sudo adduser $USER i2c
sudo service udev restart
*/

function main(a)
{

    local s = Oled96(2, 0x3c, "96");
    if(s.open("128x64",0,0))
    {
        s.write_string(0,0,"ABCDEF",1);
        kbhit();
        s.clear_display(0);
        s.write_string(0,0,"DEFGHIJKLMN",2);
        kbhit();
        s.clear_display(0);
        s.write_string(0,0,"GHIHGHISHAN",3);
        kbhit();
        s.clear_display(0);
        s.write_string(0,0,"JKLISHAN",1);
        s.write_string(0,1,"2341234134134134",1);
        kbhit();
        s.close();
    }
    return true;
}
```

### Extend rembix with custom modules calling wiringPi.so library, 
### so wiringPi/arduino style can be used
 
```cpp
::using(eSOLIB); // load what we use only 
::import("./modules/libardulike.so");

/*  the code here was pasted as it was taken form Wiring pi example */
var LED=26;

function main(x)
{
    wiringPiSetupGPIO () ;

    PinMode (LED, OUTPUT) ;

    for (;;)
    {
        digitalWrite (LED, 1) ;     // On
        delay (500) ;               // mS
        digitalWrite (LED, 0) ;     // Off
        delay (500) ;
    }
}

```

# Wow!
### Or call directin to wiringpi.so library using arduino style if you donr want rembix management features

```cpp
::using(eSOLIB); // load what we use only 

lib := LIB("libwiringPi.so");
lib.load("wiringPiSetupGpio",true,0);   // function name from the so ,function type has return value takes 0 params
lib.load("digitalWrite",false,2);       // function name from the so ,function type no return 2 parameters
lib.load("delay",0,1);                  // function name from the so ,function type no return takes one param
lib.load("PinMode",0,2);                // function name from the so ,function type no return takes one param

var LED = 26;
function main(x)
{
    wiringPiSetupGpio() ;
    PinMode (LED, OUTPUT) ;
    for (;;)
    {
        digitalWrite (LED, 1) ;     // On
        delay (500) ;               // mS
        digitalWrite (LED, 0) ;     // Off
        delay (500) ;
    }
    return false;
}

```

### I introduced into script new declarative tokens as 0o 0c 0s 0w 0i 0u for signed/usigned bit & strong typed operations

```cpp

   var v = 0x4444D4D4; print("hex size_t            " + v + "\n");    //  will print the unsigned int value
   var v = 0o4444D4D4; print("octet 8             " + v + "\n");       //  will print 212
   var v = 0c4444D4D4; print("char 8              " + v + "\n");         // will print -44
   var v = 0s4444D4D4; print("short 16            " + v + "\n");       // will print -11052
   var v = 0w4444D4D4; print("unsigned short 16   " + v + "\n");//     will print 54484
   var v = 0iDD44D4D4; print("integer 32          " + v + "\n");         //   will print the negative int value
   var v = 0uDD44D4D4; print("unsigned integer 32 " + v + "\n");  // will print the unsigned int value

// prints

hex size_t          1145361620
octet 8             212
char 8              -44
short 16            -11052
unsigned short 16   54484
integer 32          -582691628
unsigned integer 32 -582691628  // print allways prints int,

```


# Sample for making an embix module wrapper around an existent library.

   * use this tool from https://github.com/comarius/elf_resolver  
        * and extract all functions & signatures and some helper code
     
libwiring.h generated code using above script tool

```
#ifndef LIBR_RESOLVER_H
#define LIBR_RESOLVER_H
// Copyright Marius C. https://github/comarius (do not remove)
#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>


// -------------- constants ---------------
// review before compile...
#define	__WIRING_PI_H__
#define	UNU	__attribute__((unused))
#define	PI_GPIO_MASK	(0xFFFFFFC0)
#define	WPI_MODE_PINS		 0
#define	WPI_MODE_GPIO		 1
#define	WPI_MODE_GPIO_SYS	 2
#define	WPI_MODE_PHYS		 3
#define	WPI_MODE_PIFACE		 4
#define	WPI_MODE_UNINITIALISED	-1
#define	INPUT			 0
#define	OUTPUT			 1
#define	PWM_OUTPUT		 2
#define	GPIO_CLOCK		 3
#define	SOFT_PWM_OUTPUT		 4
#define	SOFT_TONE_OUTPUT	 5
#define	PWM_TONE_OUTPUT		 6
#define	LOW			 0
#define	HIGH			 1
#define	PUD_OFF			 0
#define	PUD_DOWN		 1
#define	PUD_UP			 2
#define	PWM_MODE_MS		0
#define	PWM_MODE_BAL		1
#define	INT_EDGE_SETUP		0
#define	INT_EDGE_FALLING	1
#define	INT_EDGE_RISING		2
#define	INT_EDGE_BOTH		3
#define	PI_MODEL_A		 0
#define	PI_MODEL_B		 1
#define	PI_MODEL_AP		 2
#define	PI_MODEL_BP		 3
#define	PI_MODEL_2		 4
#define	PI_ALPHA		 5
#define	PI_MODEL_CM		 6
#define	PI_MODEL_07		 7
#define	PI_MODEL_3B		 8
#define	PI_MODEL_ZERO		 9
#define	PI_MODEL_CM3		10
#define	PI_MODEL_ZERO_W		12
#define	PI_MODEL_3BP 		13
#define	PI_MODEL_3AP 		14
#define	PI_MODEL_CM3P 		16
#define	PI_VERSION_1		0
#define	PI_VERSION_1_1		1
#define	PI_VERSION_1_2		2
#define	PI_VERSION_2		3
#define	PI_MAKER_SONY		0
#define	PI_MAKER_EGOMAN		1
#define	PI_MAKER_EMBEST		2
#define	PI_MAKER_UNKNOWN	3
#define	PI_THREAD(X)	void *X (UNU void *dummy)
#define	WPI_FATAL	(1==1)
#define	WPI_ALMOST	(1==2)


// -------------- functions ---------------
// review before compile...
#define PF_waitForInterrupt (* (int (*)(int,int))                                  _ptr[0].ptr)
#define PF_micros (* (unsigned int (*)(void))                                      _ptr[1].ptr)
#define PF_piUnlock (* (void (*)(int))                                             _ptr[2].ptr)
#define PF_gpioClockSet (* (void (*)(int,int))                                     _ptr[3].ptr)
#define PF_wiringPiVersion (* (void (*)(int,int))                                  _ptr[5].ptr)
#define PF_digitalWriteByte (* (void (*)(int))                                     _ptr[7].ptr)
#define PF_digitalWriteByte2 (* (void (*)(int))                                    _ptr[8].ptr)
#define PF_physPinToGpio (* (int (*)(int))                                         _ptr[10].ptr)
#define PF_digitalReadByte2 (* (unsigned int (*)(void))                            _ptr[11].ptr)
#define PF_delay (* (void (*)(unsigned))                                           _ptr[12].ptr)
#define PF_delayMicroseconds (* (void (*)(unsigned))                               _ptr[13].ptr)
#define PF_analogRead (* (int (*)(int))                                            _ptr[14].ptr)
#define PF_pinModeAlt (* (void (*)(int,int))                                       _ptr[15].ptr)
#define PF_pwmSetRange (* (void (*)(unsigned))                                     _ptr[16].ptr)
#define PF_pinMode (* (void (*)(int,int))                                          _ptr[17].ptr)
#define PF_piBoardId (* (void (*)(int,int,int,int,int))                            _ptr[18].ptr)
#define PF_getAlt (* (int (*)(int))                                                _ptr[19].ptr)
#define PF_wiringPiSetupGpio (* (int (*)(void))                                    _ptr[21].ptr)
#define PF_piGpioLayout (* (int (*)(void))                                         _ptr[22].ptr)
#define PF_digitalWrite (* (void (*)(int,int))                                     _ptr[23].ptr)
#define PF_digitalWrite8 (* (void (*)(int,int))                                    _ptr[24].ptr)
#define PF_wiringPiSetupPhys (* (int (*)(void))                                    _ptr[25].ptr)
#define PF_pwmSetMode (* (void (*)(int))                                           _ptr[26].ptr)
#define PF_digitalReadByte (* (unsigned int (*)(void))                             _ptr[27].ptr)
#define PF_analogWrite (* (void (*)(int,int))                                      _ptr[28].ptr)
#define PF_piBoardRev (* (int (*)(void))                                           _ptr[29].ptr)
#define PF_wiringPiSetupSys (* (int (*)(void))                                     _ptr[30].ptr)
#define PF_wiringPiSetup (* (int (*)(void))                                        _ptr[31].ptr)
#define PF_wiringPiSetupPiFace (* (int (*)(void))                                  _ptr[32].ptr)
#define PF_wiringPiSetupPiFaceForGpioProg (* (int (*)(void))                       _ptr[33].ptr)
#define PF_pwmWrite (* (void (*)(int,int))                                         _ptr[34].ptr)
#define PF_setPadDrive (* (void (*)(int,int))                                      _ptr[35].ptr)
#define PF_pwmSetClock (* (void (*)(int))                                          _ptr[36].ptr)
#define PF_wpiPinToGpio (* (int (*)(int))                                          _ptr[37].ptr)
#define PF_pullUpDnControl (* (void (*)(int,int))                                  _ptr[38].ptr)
#define PF_pwmToneWrite (* (void (*)(int,int))                                     _ptr[39].ptr)
#define PF_digitalRead (* (int (*)(int))                                           _ptr[40].ptr)
#define PF_digitalRead8 (* (unsigned int (*)(int))                                 _ptr[41].ptr)
#define PF_millis (* (unsigned int (*)(void))                                      _ptr[42].ptr)
#define PF_piLock (* (void (*)(int))                                               _ptr[43].ptr)


// -------------- all funcs array ---------------
struct FUNCS_ {
  const char *name;
  void  (*ptr)(void);
};

inline const FUNCS_* load(void **ppdll)
{
    union
    {
    void *p;
    void (*fp)(void);
    } u;

    static struct FUNCS_ _funcs[] ={
        {"waitForInterrupt", nullptr},
        {"micros", nullptr},
        {"piUnlock", nullptr},
        {"gpioClockSet", nullptr},
        {"*wiringPiFindNode", nullptr},
        {"wiringPiVersion", nullptr},
        {"piHiPri", nullptr},
        {"digitalWriteByte", nullptr},
        {"digitalWriteByte2", nullptr},
        {"wiringPiFailure", nullptr},
        {"physPinToGpio", nullptr},
        {"digitalReadByte2", nullptr},
        {"delay", nullptr},
        {"delayMicroseconds", nullptr},
        {"analogRead", nullptr},
        {"pinModeAlt", nullptr},
        {"pwmSetRange", nullptr},
        {"pinMode", nullptr},
        {"piBoardId", nullptr},
        {"getAlt", nullptr},
        {"*wiringPiNewNode", nullptr},
        {"wiringPiSetupGpio", nullptr},
        {"piGpioLayout", nullptr},
        {"digitalWrite", nullptr},
        {"digitalWrite8", nullptr},
        {"wiringPiSetupPhys", nullptr},
        {"pwmSetMode", nullptr},
        {"digitalReadByte", nullptr},
        {"analogWrite", nullptr},
        {"piBoardRev", nullptr},
        {"wiringPiSetupSys", nullptr},
        {"wiringPiSetup", nullptr},
        {"wiringPiSetupPiFace", nullptr},
        {"wiringPiSetupPiFaceForGpioProg", nullptr},
        {"pwmWrite", nullptr},
        {"setPadDrive", nullptr},
        {"pwmSetClock", nullptr},
        {"wpiPinToGpio", nullptr},
        {"pullUpDnControl", nullptr},
        {"pwmToneWrite", nullptr},
        {"digitalRead", nullptr},
        {"digitalRead8", nullptr},
        {"millis", nullptr},
        {"piLock", nullptr},
        {nullptr, nullptr}
    };
    void  *dll_handle;
    if ((dll_handle = dlopen("/usr/lib/libwiringPi.so", RTLD_LAZY)) == 0)
    {
        perror("cannot load wiringpi\n");
        return nullptr;
    }
    *ppdll = dll_handle;
    struct FUNCS_ *fp = _funcs;
    for (; fp->name != nullptr; fp++)
    {
        u.p = dlsym(dll_handle, fp->name);
        if (u.fp == 0)
        {
            perror("cannot load:");
        }
        else
        {
            printf("LOADING FUNCTION %s \n", fp->name);
            fp->ptr = u.fp;
        }
    }
    return _funcs;
}
#endif // LIB_RESOLVER

```

swiring.h, This code you write.

```cpp
#if !defined(_SIMPLEMODULE_H_)
#define _SIMPLEMODULE_H_

#include "sqmodule.h"
#include "squirrel.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT SQRESULT sqmodule_load(HSKVM v, HSQAPI api);
EXPORT void sqmodule_unload(void);

int test(const char* ,int);

#ifdef __cplusplus
}
#endif

#endif

```
and swiring.cpp

```cpp

#include <dlfcn.h>
#include "swiringpi.h"
#include "sqrat.h"
#include "libwiring.h"

sq_api*          SQ_PTRS;
const FUNCS_*   _ptr; // global var
void*           _dll;

EXPORT void sqmodule_unload(void)
{
    dlclose(_dll);
}

// Module registration
SQRESULT sqmodule_load(HSKVM v, HSQAPI api)
{
    SQ_PTRS = api;
    _ptr = load(&_dll); // in main()
    printf("LOADING FUNCTIONS \n");
    Sqrat::RootTable(v).Func("waitForInterrupt",PF_waitForInterrupt);
    Sqrat::RootTable(v).Func("micros",PF_micros);
    Sqrat::RootTable(v).Func("piUnlock",PF_piUnlock);
    Sqrat::RootTable(v).Func("gpioClockSet",PF_gpioClockSet);
    //Sqrat::RootTable(v).Func("*wiringPiFindNode",PF_*wiringPiFindNode);
    Sqrat::RootTable(v).Func("wiringPiVersion",PF_wiringPiVersion);
    //Sqrat::RootTable(v).Func("piHiPri",PF_piHiPri);
    Sqrat::RootTable(v).Func("digitalWriteByte",PF_digitalWriteByte);
    Sqrat::RootTable(v).Func("digitalWriteByte2",PF_digitalWriteByte2);
    //Sqrat::RootTable(v).Func("wiringPiFailure",PF_wiringPiFailure);
    Sqrat::RootTable(v).Func("physPinToGpio",PF_physPinToGpio);
    Sqrat::RootTable(v).Func("digitalReadByte2",PF_digitalReadByte2);
    Sqrat::RootTable(v).Func("delay",PF_delay);
    Sqrat::RootTable(v).Func("delayMicroseconds",PF_delayMicroseconds);
    Sqrat::RootTable(v).Func("analogRead",PF_analogRead);
    Sqrat::RootTable(v).Func("pinModeAlt",PF_pinModeAlt);
    Sqrat::RootTable(v).Func("pwmSetRange",PF_pwmSetRange);
    Sqrat::RootTable(v).Func("pinMode",PF_pinMode);
    Sqrat::RootTable(v).Func("piBoardId",PF_piBoardId);
    Sqrat::RootTable(v).Func("getAlt",PF_getAlt);
    //Sqrat::RootTable(v).Func("*wiringPiNewNode",PF_*wiringPiNewNode);
    Sqrat::RootTable(v).Func("wiringPiSetupGpio",PF_wiringPiSetupGpio);
    Sqrat::RootTable(v).Func("piGpioLayout",PF_piGpioLayout);
    Sqrat::RootTable(v).Func("digitalWrite",PF_digitalWrite);
    Sqrat::RootTable(v).Func("digitalWrite8",PF_digitalWrite8);
    Sqrat::RootTable(v).Func("wiringPiSetupPhys",PF_wiringPiSetupPhys);
    Sqrat::RootTable(v).Func("pwmSetMode",PF_pwmSetMode);
    Sqrat::RootTable(v).Func("digitalReadByte",PF_digitalReadByte);
    Sqrat::RootTable(v).Func("analogWrite",PF_analogWrite);
    Sqrat::RootTable(v).Func("piBoardRev",PF_piBoardRev);
    Sqrat::RootTable(v).Func("wiringPiSetupSys",PF_wiringPiSetupSys);
    Sqrat::RootTable(v).Func("wiringPiSetup",PF_wiringPiSetup);
    Sqrat::RootTable(v).Func("wiringPiSetupPiFace",PF_wiringPiSetupPiFace);
    Sqrat::RootTable(v).Func("wiringPiSetupPiFaceForGpioProg",PF_wiringPiSetupPiFaceForGpioProg);
    Sqrat::RootTable(v).Func("pwmWrite",PF_pwmWrite);
    Sqrat::RootTable(v).Func("setPadDrive",PF_setPadDrive);
    Sqrat::RootTable(v).Func("pwmSetClock",PF_pwmSetClock);
    Sqrat::RootTable(v).Func("wpiPinToGpio",PF_wpiPinToGpio);
    Sqrat::RootTable(v).Func("pullUpDnControl",PF_pullUpDnControl);
    Sqrat::RootTable(v).Func("pwmToneWrite",PF_pwmToneWrite);
    Sqrat::RootTable(v).Func("digitalRead",PF_digitalRead);
    Sqrat::RootTable(v).Func("digitalRead8",PF_digitalRead8);
    Sqrat::RootTable(v).Func("millis",PF_millis);
    Sqrat::RootTable(v).Func("piLock",PF_piLock);


    return SQ_OK;
}


int test(const char* s,int k)
{
    printf("%s %s %d \n",__FUNCTION__,s,k);
    return 40;
}

```

### An extension module in a form of a script. Consult bin folder for latest versions

```java
::using(eUART|eJSON|eBASE); // load in VM only what we usee. New as Nov 29 !

class MyDev  extends BASE
{
    _json="";
    _data=[];
    _k  = 0;
    _uart = UART("/dev/ttyUSB0",115200,"8N1","rs232"); //we connect to some

    constructor(whatever)
    {
        _k    = 0;
        _json = "";
        _data = [1,2,3,0,0];
        _uart.sercr([0xA,0xD]);             // configure it for command line end [0xD] or  [0xA] or both 
        base.constructor(this, whatever);   // a must
    }

    // called form Json interface upon a restuall api as  http://IP/onedev  or  http://IP/another_dev  
    // called when db needs data to be saved in the database
    function get_data()       // return array of max 5 values as integers or strings
    {
        print("get_data\n");
        return [2,3,4,5];
        // return ["sdf","asdfasdf","adfasdfadf"];
    }
    
    // this is local foo
    function set_data(array)
    {
        this._data = array;
    }

    // go and read the data, 
    function read_device()
    {
        var xx;
        var sall;
        _uart.puts("enquire\n");
/*
        while(xx=_uart.gets(6)!=null)
        {
            sall+=string;
            msleep(128);
        }
*/
        _data[0] = rand() & 0xFF;
        _data[1] = rand() & 0xFF;
        _data[2] = rand() & 0xFF;
        _data[3] = rand() & 0xFF;
    }
};

// 2 devices
mydev1 <- MyDev("onedev");          // original assignment notation
// mydev2 := MyDev("another_dev");  // new assignment table notation


function main(o)
{
    println("MAIN ");
    return run_loop(program_loop,1000);
}

function program_loop(ctx, dev)
{
    println("LOOP ");
    mydev1.read_device();
    print(ctx.get_json(null) + "\n\n\n");
    return true;
}

```

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=L9RVWU5NUZ4YG)

The SDK documentation will be mailed on the PayPal associated email as PDF for a donation of minimum $25 USD. 

![logo](https://raw.githubusercontent.com/comarius/rembix/master/docs/emb3.png)


Credits:
    * https://github.com/albertodemichelis/squirrel
    * https://github.com/hakase-labs/sqrat
    * https://github.com/comarius/Easy-ssh_automation
    * https://github.com/comarius/xomata
    * https://github.com/comarius/scrite
    * https://github.com/comarius/The-Ultra-C-Monex-
    
    


