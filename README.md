
### Amutrion, Industrial Strength HAL Library for Linux (Scripting Engine in C++)

#### No dependencies,
#### Written in C++
#### Scripted: Java/C++ like syntax (Uses superfast scripting engine used in collest game engines 'http://www.squirrel-lang.org/')
#### S O L I D / K I S S. architecture
#### No obfuscated syntax. Straight up readable API's.

   * HAL LAYER FOR LINUX (see https://github.com/circinusX1/amutrion/tree/master/src/drivers  FOLDER)
       * I2C
       * SPI
       * GPIO
       * PWM
       * ADC
       * UART
          * OPTIONAL RESFULL SERVER (JSON)
       

##### Published: 1 Oct 2017

```
   git pull https://github.com/circinusX1/amutrion
   cd amution
   ./make.sh  # On the target or chroot of your platform. WIll generate the bynary as $OS_$ARCH_amutrion
   cd bin
   Some samples runs only on R-PI or BBB
```

# Syntax, Like Java

```C++
var l1 = PIO(26,  DIR_OUT, LOW, "led");         // GPIO
l1.set_value(0);
var pwm = PWM("0.0", 1000/*freq*/, 100/*initial load*/, false/*reverse*/, "pwm"/*JSON name*/);  // PWM
pwm.set_value(80);
li.call_back(my_function, ON_RISE|ON_FALL);
```


#### SDK
[!SDK](https://github.com/circinusX1/amutrion/wiki)


#### VIDEO
[![video](https://raw.githubusercontent.com/circinusX1/amutrion/master/docs/rrembdemo.gif)](https://youtu.be/ZMlJj0PciWA)


## RASPBERRY-PI GPIO, PWM, SPI, I2C, UART FRAMEWORK.  
### script them(GPIO PWM I2C SPI ADC UART USB FILE-IO & /dev/input/ TCP SSH CURL)
### access them REST-FULLY {JSON IN & OUT}
### All written in C++
#### REACTION TIME ~ 1ms
##### Get automated systems up and running in a mater of few hours.
###### Optional system dependecies (sqlite / CURL)
   
This is a self contained program written in C++. I start it almost 13 years ago using it 
for a game engine automation (see Getic). Then from controlling a game engine I 
extended to do automation scripting adding UART telnet and ssh (see othe projects here). 
Slowly I added SSH, TCP, I2C, ... GPIO's and so on.

    * Usage:
```
    amutrion script_file.src
    
```

Where the script_file is a script like java/c++, but is not java neither c++. 

The Language features can be seen at: 
   * http://squirrel-lang.org/   
   * https://developer.electricimp.com/squirrel/squirrelcrib

    * It targets:
        * Raspberry PI
        * Beaglebone(s)
        * iMX6
        * Nano PI
        * Cherry PI
        * C.H.I.P

### What you can do with it

You can automate a system in matter of few hours depending of the complexity of the automation. 
You get out of the box a JSON output over a socket, you can build a web UI in few moves. 
For example you can make a STOVE or a HVAC or any automation funcy device. It allows fine tunning
from scripted API's to target almost any ARM device that the kernel exposes the peripherials to either 
/sys or /dev filestem. 


### Sample code R-PI
```c++
::using(eGPIO|ePWM|eSRV);

var l1 = PIO(26,  DIR_OUT, LOW, "led"); 
var l2 = PIO(19,  DIR_OUT, HIGH, "led2");
var l3 = PIO(13,  DIR_OUT, LOW, "led3");
var l4 = PIO(6,   DIR_OUT, HIGH, "led4");
var pb = PIO(21,  DIR_IN,  LOW, "button");
var tone = PIO(17,  440, "buzer");       // tone
var counter = PIO(4, -1000, "count");    // frequence-meter
                                        
var pwm = PWM("0.0", 1000, 100, false, "pwm");  // this is a PWM, on chip 0 pwm 0, running a 1000Hz with default 100% pwm

var json_interface = SRV(8078);            // start a TCP Json server on port 8070
var database_sqlite  = Database("/usr/share/embix/db1");

var value = 1;
var k = 0;

function main(ctx)
{
    pb.monitor(callback);           // calls callback when the pb state is changes
    l1.set_value(0);                // set this led to 0
    db.save_interval(15000);        // will save sensors/PIO's every 15 seconds
    return run(loop,1000);     // run loop() calling it at 1000 ms interval, though when pb state changes
}

function loop(ctx, dev)             // each of the devices fires the loop with the dev=true when a state in the dev changes.
                                    // to get the new value you shoule prepare the dev with the 'dev.monitor(callback);' hook. 
{
    l1.set_value(value); 
    l2.set_value(!value);
    l3.set_value(value);
    l4.set_value(!value);
    pwm.set_duty(k++);
    value=!value;
    println("counter = " + counter.get_freq());  // this will print roughy 440
    if(dev){
      // opps the dev dev
    }
    
    return true;
}

/* this has changed in all flavours*/
function  callback(dev)
{
    println("dev = " + dev.get_value()); // dev is not null only when the monitorred pin is triggered
    return false; // end the program  
}


```

#### Sample to control OLED96 display

```c++

include("plugins/oled96.scr");
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

### Extend amutrion with custom modules. 
### Let's use wiringpi.so module wrapped around wiringPI.so. 
### so wiringPi/arduino style coding can be used
 
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
### Or call directin to wiringpi.so library using arduino style.
#### Import library, get functions and call them.

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

### New declarative tokens as 0o 0c 0s 0w 0i 0u for signed/usigned bit & strong typed binary operations

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

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=L9RVWU5NUZ4YG)   [goes to meeiot]

Donations are owsom' appreciated 

### Embeded RESFULL / JSON server to control every device (Can be turned on and off)

    *Custom application & scripts....contact me here: https://www.meeiot.org/?p=contact.php    
    

![logo](https://raw.githubusercontent.com/circinusX1/rembix/master/docs/emb3.png)


Credits:

    *https://github.com/albertodemichelis/squirrel
    *https://github.com/hakase-labs/sqrat
    *https://github.com/circinusX1/Easy-ssh_automation
    *https://github.com/circinusX1/xomata
    *https://github.com/circinusX1/scrite
    *https://github.com/circinusX1/The-Ultra-C-Monex-
    
    

![logo](https://raw.githubusercontent.com/circinusX1/rembix/master/docs/embixico.png)



[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=L9RVWU5NUZ4YG)   [donations are taken care by meeiot]



#### Build from sources
```
./make.sh   (on each HW target, otherwise setup a toolhain accordingly)

```


# LICENSE ADDON

    * Not for commercial witout written consent


[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=L9RVWU5NUZ4YG)   [donations are taken care by meeiot domain]

    

###  You can check my reverse ssh online service and online key value database at 

[reverse ssh as a service](http://www.mylinuz.com)

[key value database as a service](https://www.meeiot.org)


###  Examples:



```cpp

// PWM EXAMPLE

::using(eGPIO|ePWMM|eSRV);

setdbg(0xFF);

// reconfig_sys(ePWMM, ["/sys/class/pwm/","","pwm-%d:%d"]);     // use /sys/class/pwm/pwm-C:P
reconfig_sys(ePWMM, ["/dev/pwm/","",""]);                    // use /dev/pwm/PWM_PIN

var pwm = PWM("0.0", 30000, 30, false, "pwm");

function main(ctx)
{
   var pwm = PWM("ecap0", 30000, 50, false, "pwm");
   return run(l,10);
}

var x=0;
function l(c,d)
{
    pwm.set_value(x++);
    if(x>99)
        x=0;
    return true;
}

///////////////////////////////////////////////////////////////////////


// SCRIPT  TESTED   R-PI GPIO's

::using(eGPIO|ePWM);

system("config-pin P9.41 gpio");
system("config-pin P9.42 gpio");
system("config-pin P9.41 out");
system("config-pin P9.42 in-");

led    := PIO(20, DIR_OUT, LOW, "led");
button := PIO(7, DIR_IN, LOW, "buton");


function main(ctx)
{
    //int, int, int, const char*
    led.set_value(LOW);
    led.set_value(HIGH);
    led.set_value(LOW);
    return run(loop,10);
}

function loop(ctx,devs)
{
    led.set_value(HIGH);
    suspend(200);
    led.set_value(LOW);
    suspend(200);
    return true;
}


//////////////////////////////////////////////////////////////


// SCRIPT  TESTED   BME280 SENSOR

::using(eI2C|eSRV);
::loadmod("./modules/libbme280.so","THP");

i2c    := I2C(8, 0x77, "i2c");  // the i2c THP is using
thp    := THP("i2c", "bmp280"); // pass the i2c name as first parameter
server := SRV(8000,"X");

function main(x)
{
    thp.monitor(showth);
    return run(mainloop,-1);
}

function showth(dev)
{
    var a = dev.thp();
    println("THP = " + a[0] +
            "  " + a[1] +
            "  " +  a[2] );
}

function mainloop(ctx, notused)
{
    // read one by one every 1000 ms
    println("T=" + thp.temperature() + " m*C  H=" +
                 thp.humidity()   + " x100 %  P=" +
                 thp.pressure()   + " mm Hg ");
    return true;

}


//////////////////////////////////////////////////////////
// SCRIPT  TESTED. CP2112 MUX

::using(eI2C|eGPIO|eUART|eSRV);

l1 := PIO(506,  DIR_OUT, LOW, "led");
l2 := PIO(507,  DIR_OUT, HIGH, "led2");
i2c := I2C(11, 0x68, "i2c");
json := SRV(8000,"rest");


function main(ctx)
{
    i2c.autoopen(false);
    if(i2c.open(2)){
        var arr = i2c.ioread(0, 4);
        i2c.close();
        println("reg: " + 0 + " = " + arr2a(arr,','));
    }
    println("L1=" + l1.set_toggle());
    println("L2=" + l2.set_toggle());
    return run(loop,2000);
}

function loop(ctx, dev)
{
    println("L1=" + l1.set_toggle());
    println("L2=" + l2.set_toggle());
    if(i2c.open(2)){
        var arr = i2c.ioread(0, 4);
        i2c.close();
        println("reg: " + 0 + " = " + arr2a(arr,','));
    }
    return true;
}


///////////////////////////////////////////////////////////////////


// SCRIPT  TESTED   WEB ACCESS TO ESP96 LIGHT CONTROL

::using(eCURL|eSRV|eBASE|eBASE);

setdbg(0xFF);

class XX extends BASE
{
    _value = "";
    curl   = CURL(2000,"led");
    constructor()
    {
        _value = "xx";
        base.constructor(this, eSTRING, "wrapp");
    }

    // get sample    get JSON ->>>>>>>
    function get_value(key)
    {
       curl.set_url("192.168.1.223",0,"");
       var html = curl.perform(4000);
       if(html.find("Off"))
            this._value="ON";
       else
            this._value="OFF";

        return { /*dump test */
            LED = this._value,
            CMDS = ["ON","OFF"],
            VALS = [0,1,2,3,4,5],
            REALS = [3.4,5.6,0.7777]
        };
    }

    function set_value(k,v)
    {
        curl.set_url("192.168.1.223/LED="+v,0,"");
        println("--------sending ---->" + "192.168.1.223/LED="+v )
        var html = curl.perform(4000);
        return true;
    }
};


var xx = XX();
var srv = SRV(8000,"ddd");

function main(o)
{
    return run(loo,-1);
}

function loo(x,d)
{
    return true;
}


/////////////////////////////////////////////////////////////////////////



/**
SCRIPT  TESTED  PWM PLAY 
# @ add this: to /boot/config.txt
dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=4
dtoverlay=pwm-2chan,pin=18,func=2,pin2=19,func2=2

reference: R-PI  https://librpip.frasersdev.net/peripheral-config/pwm0and1/

R-PI  PWM PINS  GPIO 12 13 18  19
                PWM  0  1   0   1
                FUNC 4  4   2   2
then check $ lsmod | grep pwm
              pwm_bcm2835 2631 2

then cd /sys/class/pwm
ls
    use PWM class as
                PWM("X.Y")   where X os pwnchipX and Y is pwm-Y
*/

::using(eGPIO|ePWMM|eSRV);

var srv = SRV(8000,"srv");

//var l1 = PIO(21,  DIR_OUT, HIGH, "led1");
//var l2 = PIO(20,  DIR_OUT, HIGH, "led2");


var edge = PIO(14, DIR_IN, HIGH, "detect");

var pwm0 = PWM("0.0", 1000, 100, false, "pwm1"); //GPIO 13
var pwm1 = PWM("0.1", 1000, 100, false, "pwm2"); //GPIO 18
//var pb = PIO(21,  DIR_IN,  LOW, "buton");

function main(ctx)
{
    edge.call_back(callback, RISE);
    return run(loop,2000);
}

var K=0;
var I=1;
function loop(ctx, dev)
{
    pwm0.set_value(K);
    pwm1.set_value(K*2);
    K+=I;
    if(K>99 || K<1)
        I=-I;
    return true;
}

function callback(dev)
{
    println("edge rised " + dev.get_value());
}



// SCRIPT  TESTED ON OLED DISPLAY TEST

::using(eI2C|eGPIO|ePWMM|eSRV|eDB);
::include("modules/_oled96.inc");

/*
sudo nano /lib/udev/rules.d/60-perip.rules
KERNEL=="gpiochip*", GROUP="gpio"
KERNEL=="pwmchip*", GROUP="gpio"
KERNEL=="i2c-0"     , GROUP="i2c", MODE="0660"
KERNEL=="i2c-[1-9]*", GROUP="i2c", MODE="0666"
#
sudo adduser $USER i2c
sudo adduser $USER gpio
sudo adduser $USER pwm
sudo service udev restart
*/

dbs   := DB(10000, 1000, "./database");
json := SRV(8000,"rest");
local s = Oled96(11, 0x3c, "OLED96");

l1 := PIO(506,  DIR_OUT, LOW, "led");
l2 := PIO(507,  DIR_OUT, HIGH, "led2");

//s.autoopen(false);
function main(a)
{
    var k;
    s.autoopen(false);
    if(s.xopen("128x64",0,0))
    {
        s.clear_display(0);
        k = run(kk, 1000);
    }
    s.xclose();
    return k;
}

var K=10;
function kk(ctx,dev)
{
    s.write_string(0,1, get_strtime() ,2);
    K++;
    println("L1=" + l1.set_toggle());
    println("L2=" + l2.set_toggle());
    sleep(1000);

    ctx.get_dev("led2").set_toggle();
    K--;
    return K > 0 ? true : false;
}

```


```






