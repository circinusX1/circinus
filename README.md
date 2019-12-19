
# embix comming on: Jan 2020 




![logo](https://raw.githubusercontent.com/pangeea/xembe/master/doc/embixico.png)

## GPIO, PWM, SPI, I2C SCRIPTING LANGUAGE FOR LINUX / FREEBSD 
## NO DEPENDENCIES.
### C++ SUPER UBER PROFESSIONAL GRAGE CODING. 
### Targets: R-PI, BBB, Nano-PI and more  
### INDUSTRIAL GRADE SOFTWARE FOR PIO'S PWM'S I2C, SPI, UART & MUCH MORE
### FASTEST REACTION TIME ~ 1Ms (depending of your code)

    * No dependencies no obfuscated mumbo's jumbo's a'la nodejs and pyton. 

This is a self contained engine written in C++, I start it almost 14 years ago for a game engine automation. 
Then from controlling a game engine I took it and made a testing scripting adding UART telnet and ssh. 
Slowly I added USB, I2C, SPI, GPIO's, PWM and son on. 
Was heavily tested and runs clean with no memory leaks or fragmentation. 
It exposes API's to a SCRIPT language similar to JAVA/C++ . 
Is using  a modified Squirrel Script. 
The script engine was changed to get the syntax closer to java script. 
The differences are abount 15%.

 - Wiring PI (arduino) coding style.
 - Professional house keeping for heavy industrial projects.
 - If you have the sqlite installed it saves data into it.
 - Controlled by script or 
     - JSON over TCP as RESTFUL API's (for a web UI / or Qt UI)
 - Logs 
 - Can perform calls right into dynamic libraries.
     - Very easy to add new modules as dynamic libraries.
     - Code generator from any dynamic library to embix plugin

* for raspberry PI
* for beaglebone black & green
* for iMX6
* for nano pi
* for cherry pi
* for C.H.I.P


#### Using PIO's and pwm  on R-PI sample

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
                                    // the function is called right away. If there are many monitors, the loop
                                                        // will loop for each of them
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

#### a code for beaglebone

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

#### code for reading a sensor


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
    BMPx8x_PresConversion0  = 0x34;
    BMPx8x_Results  = 0xF6;
    BMPx8x_minDelay  = 4;//require 4.5ms *1000/700 'turbo mode fix'= 6.4-Retry =4.4
    BMPx8x_RetryDelay  = 2;//min delay for temp 4+2=6ms, max 4+2*20=44ms for pressure
    BMPx8x_OverSampling  = 3;
    bmp_ac1=0;
    bmp_ac2=0;
    bmp_ac3=0;
    bmp_ac4=0;
    bmp_ac5=0;
    bmp_ac6=0;
    bmp_b1=0;
    bmp_b2=0;
    bmp_b5=0;
    bmp_mb=0;
    bmp_mc=0;
    bmp_md=0;

    constructor (i2cbus,addr,name)
    {
        bmp_ac1=-1;
        bmp_ac2=-1;
        bmp_ac3=-1;
        bmp_ac4=0;
        bmp_ac5=0;
        bmp_ac6=0;
        bmp_b1=-1;
        bmp_b2=-1;
        bmp_b5=-1;
        bmp_mb=-1;
        bmp_mc=-1;
        bmp_md=-1;
        base.constructor(this, i2cbus, addr, name);
    }

    function   calibrate()
    {
        var  rValue = this.ioread(0xAA,22);
        assert(rValue.len()!=20,"error read");
        this.bmp_ac1=0s((rValue[0]<<8)|rValue[1]);
        this.bmp_ac2=0s((rValue[2]<<8)|rValue[3]);
        this.bmp_ac3=0s((rValue[4]<<8)|rValue[5]);
        this.bmp_ac4=0w((rValue[6]<<8)|rValue[7]);
        this.bmp_ac5=0w((rValue[8]<<8)|rValue[9]);
        this.bmp_ac6=0w((rValue[10]<<8)|rValue[11]);
        this.bmp_b1=0s((rValue[12]<<8)|rValue[13]);
        this.bmp_b2=0s((rValue[14]<<8)|rValue[15]);
        this.bmp_mb=0s((rValue[16]<<8)|rValue[17]);
        this.bmp_mc=0s((rValue[18]<<8)|rValue[19]);
        this.bmp_md=0s((rValue[20]<<8)|rValue[21]);
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
        var  up = ((rValues[0] *65536) + rValues[1] *256) + rValues[2] ;
        up = up >> (8-BMP180.BMPx8x_OverSampling);
        var  x1, x2, x3, b3, b6, p;
        var  b4, b7;
        b6 = this.bmp_b5 - 4000;
        x1 = 0i((this.bmp_b2 * (b6 * b6)/4096)/2048);
        x2 = 0i((this.bmp_ac2 * b6)/2048);
        x3 = 0i(x1 + x2);
        b3 = 0i(((((this.bmp_ac1)*4 + x3) << BMP180.BMPx8x_OverSampling) + 2)/4);
        x1 = 0i((this.bmp_ac3 * b6)/8192);
        x2 = 0i((this.bmp_b1 * ((b6 * b6)/4096))/65536);
        x3 = 0i(((x1 + x2) + 2)/4);
        b4 = (this.bmp_ac4 * (x3 + 32768))/32768;
        b7 = ((up - b3) * (50000>>BMP180.BMPx8x_OverSampling));
        if (b7 < 0x80000000)
            p = (b7<<1)/b4;
        else
            p = (b7/b4)*2;
        x1 = 0i((p/256) * (p/256));
        x1 = 0i((x1 * 3038)/65536);
        x2 = 0i((-7357 * p)/65536);
        p =  0i(p + (x1 + x2 + 3791)/16);
        return (p);
    }

    function   bmp_GetTemperature()
    {
        var  rValues;
        this.iowrite(BMPx8x_CtrlMeas, [BMP180.BMPx8x_TempConversion]);
        sleep (256+BMP180.BMPx8x_minDelay);
        if (WaitForConversion() == false)
            return -1;
        sleep (256+BMP180.BMPx8x_minDelay);
        rValues = this.ioread(BMP180.BMPx8x_Results,2);
        var  ut = (rValues[0]*256)+rValues[1];

        var  x1 = ((ut - this.bmp_ac6) * this.bmp_ac5)/32768;
        var  x2 = (this.bmp_mc * 2048) / (x1 + this.bmp_md);
        var  xbmp_b5 = x1 + x2;
        var  result = float((xbmp_b5 + 8.0)/1.6);
        return float(result);
    }
};

/*
  //uncomment to see all functions
foreach(k, d in getroottable())
{
 println(k +"="+ d);
 if(k=="class")
{
 foreach(kk, dd in d)
{
 println(""+ kk +"="+ dd);
}
}
}
*/

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

#### Controlling an OLED I2C display

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

... and the included file

```c++

include("./plugins/_oledfont.js");

system("config-pin P9.17 i2c");
system("config-pin P9.18 i2c");


class Oled96 extends I2C
{
    _oled_flip=false;     // unless they are not initialised they are / class
    _oled_type="";
    _iScreenOffset=0;
    _ucscreen=[];

    // 2, 0x3c
    constructor(i2cbus, address, name)
    {
        // bind vars / instance (not per class)
        _iScreenOffset = 0;
        _oled_flip = false;
        _oled_type = "";
        _ucscreen=array(1024,0);

        base.constructor(this, i2cbus, address, name); // on BBB I2C1 P9.17 P9.18
    }

    function close()
    {
        print("olce closing \n");
        base.write(0x00,[0xae]);
        base.close();
    }

    function open(oledtype, invert, flip)
    {
        print("call base.open\n");
        if(base.open(0))
        {
            local init;
            _oled_type = oledtype;
            if(oledtype="128x32")
            {
                init=[0xae,0xd5,0x80,0xa8,0x1f,0xd3,0x00,
                      0x40,0x8d,0x14,0xa1,0xc8,0xda,0x02,0x81,
                      0x7f,0xd9,0xf1,0xdb,0x40,0xa4,0xa6,0xaf];
            }
            else
            {
                init=[0xae,0xa8,0x3f,0xd3,0x00,0x40,0xa1,
                      0xc8,0xda,0x12,0x81,0xff,0xa4,0xa6,0xd5,
                      0x80,0x8d,0x14,0xaf,0x20,0x02];
            }
            base.write(0x00, init);
            if(invert)
            {
                base.write(0x00,[0xa7]);
            }
            if(flip)
            {
                base.write(0x00,[0xa0]);
                base.write(0x00,[0xc0]);
                _oled_flip=bflip;
            }
            return true;
        }
        return false;
    }

    function _set_pos(x,y)
    {
        _iScreenOffset = (y*128)+x;
        if (_oled_type != "64x32") // visible display starts at column 32, row 4
        {
            x += 32; // display is centered in VRAM, so this is always true
            if (_oled_flip == 0) // non-flipped display starts from line 4
                y += 4;
        }
        else if (_oled_type == "132x64") // SH1106 has 128 pixels centered in 132
        {
            x += 2;
        }

        base.write(0, [0xb0 | y]);                  // go to page Y
        base.write(0, [0x00 | (x & 0xf)]);          // // lower col addr
        base.write(0, [0x10 | ((x >> 4) & 0xf)]);   // upper col addr
    }

    function _write_block(arr)
    {
        base.write(0x40, arr);
        arrcpyn(_ucscreen, _iScreenOffset, arr);
        _iScreenOffset += arr.len();
    }

    function _set_pixel(x, y, ucColor)
    {
        local uc = [0];
        local ucOld = [0];

        local i = ((y >> 3) * 128) + x;
        if (i < 0 || i > 1023) // off the screen
            return -1;
        uc[0] = ucOld[0] = ucScreen[i];
        uc[0] = uc[0] & ~(0x1 << (y & 7));
        if (ucColor)
        {
            uc[0] = uc[0] | (0x1 << (y & 7));
        }
        if (uc[0] != ucOld[0]) // pixel changed
        {
            _set_pos(x, y>>3);
            _write_block(uc);
        }
        return 0;
    }

    function write_string(x, y, szMsg, iSize)
    {
        local i, iLen;
        local s = array(16);

//	print("type ================= " + typeof szMsg + "\n");
        iLen = szMsg.len();
        if (iSize >= 3) // draw 16x32 font
        {
            if (iLen+x > 8) iLen = 8-x;
            if (iLen < 0) return -1;
            x *= 16;
            for (i=0; i<iLen; i++)
            {
                s = ucFont.slice(9728 + (szMsg[i] * 64), 9728 + (szMsg[i] * 64)+16);
                _set_pos(x+(i*16), y);
                _write_block(s);

                _set_pos(x+(i*16), y+1);
                s = ucFont.slice(16 + 9728 + (szMsg[i] * 64), 16 + 9728 + (szMsg[i] * 64)+16);
//                _write_block(s+16, 16);
                _write_block(s);
                _set_pos(x+(i*16), y+2);

                s = ucFont.slice(32 + 9728 + (szMsg[i] * 64), 32 + 9728 + (szMsg[i] * 64)+16);
                _write_block(s);
                //			_set_pos(x+(i*16), y+3);
                //			_write_block(s+48, 16);
            }
        }
        else if (iSize >= 2) // draw 8x8 font
        {
            _set_pos(x*8, y);
            if (iLen + x > 16) iLen = 16 - x; // can't display it
            if (iLen < 0)return -1;

            for (i=0; i<iLen; i++)
            {
                s = ucFont.slice(szMsg[i] * 8, szMsg[i] * 8 +  8);
                _write_block(s); 			// write character pattern
            }
        }
        else // 6x8
        {
            _set_pos(x*6, y);
            if (iLen + x > 21) iLen = 21 - x;
            if (iLen < 0) return -1;
            for (i=0; i<iLen; i++)
            {
                s = ucSmallFont.slice(szMsg[i]*6, szMsg[i]*6+6);
                _write_block(s);
            }
        }
        return 0;
    }

    function clear_display(ucData)
    {
        local y;


        local iLines = (_oled_type == "128x32" || _oled_type == "64x32") ? 4:8;
        local iCols = (_oled_type == "64x32") ? 4:8;
        local temp = array(iCols*16, ucData);
        for (y=0; y<iLines; y++)
        {
            _set_pos(0,y); // set to (0,Y)
            _write_block(temp); // fill with data byte
        } // for y
        return 0;
    }
};
```

### using a C++ embix module
 
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
### calls functions right into a dynamic library

```cpp
::using(eSOLIB); // load what we use only 



lib := LIB("libwiringPi.so");
// grab functions
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

### new types for byte operations when signed or unsigned values are important. 

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


# Making an embix module

   * use this tool from https://github.com/comarius/elf_resolver  
        * and extract all functions & signatures and some helper code
     
libwiring.h

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

swiring.h

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

### a possible script for a custom device proxying data to a uart device

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
    //  or for all sensors   as:    http://IP/
    function get_json()                    // this is how you build a json, using the script table object
    {
        var json_obj = {
            etc = "xxxx",
            values = {
                data = _data,
                sensor = 4.5,
                flag = true,
            },
            notes = "whatever"
        }
        return json_obj;
    }

    // called upon RESTULL call as http://IP/onedev/k=v&k=v&k=v...  or POST http://IP/onedev with JSON post
    function set_json(json)            // decompose it and take action
    {
        foreach(k,v in json)
        {
            // send some over uart
            _uart->puts("set value=5"); 
        }
    }

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
mydev1 <- MyDev("onedev");
mydev2 := MyDev("another_dev");

///////////////////////////////////////////////////////////////
function main(o)
{
    println("MAIN ");
    return run_loop(program_loop,1000);
}

//////////////////////////////////////////////////////////////
function program_loop(ctx, dev)
{
    println("LOOP ");

    mydev1.read_device();
    mydev2.read_device();

    print(ctx.get_json(null) + "\n\n\n");
    return true;
}


```

### thr result peresented to JSON socket

```
    "another_dev":{
        "etc":"xxxx",
        "values":{
            "sensor":4.5,
            "flag":1,
            "data":[
                33,
                61,
                220,
                135,
                0
            ]
        },
        "notes":"whatever"
    },
    "onedev":{
        "etc":"xxxx",
        "values":{
            "sensor":4.5,
            "flag":1,
            "data":[
                14,
                130,
                116,
                65,
                0
            ]
        },
        "notes":"whatever"
    },
    "rs232":{
        "ukey":"rs232",
        "dpoints":[
        ]
    }

}
```

My daily Job eats all the time. Make it stop!

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=L9RVWU5NUZ4YG)

Thank you.


![logo](https://raw.githubusercontent.com/pangeea/xembe/master/doc/emb3.png)
