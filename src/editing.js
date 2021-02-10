Circinus scripting engine
for GPIO ADC PWM I2C SPI UART & USB

Link: https://github.com/circinusX1/circinus

Squirrel script context on top of C++ giving incredible speed and
extensibility.
Why no bone script of python scripts
Every time I got near npm/nodejs or pip I ran in never ending
dependencies and messy compromises. I wanted to have something clean, to know where every component resides, like I have them for C/C++, includes  and libraries easy to locate easy to maintain and manage.
Integration
It can be integrated side by side with a web server. From the web server you can access 'circinus'  REST server in and out and
wrap nice html code to control and supervise the engine.
It has an ondemand DB in sqlite and a log events in /var/log/rembix/*

Block Diagram

The script language syntax is 100% is Squirrel Script script.
It looks like: (Blinking a LED)
::using(eGPIO);
LED := GPIO(26,  DIR_OUT, LOW, "led");
var onoff = 1;
function main(ctx)
{
    return run(loop,1000);
}
function loop(ctx, dev)
{
    LED.set_value(onoff);
    onoff = !onoff;
    return true;
}
SDK
TYPES
string     - alphanumeric string between double quotes
integer   -  integer number
float       - a real number
bool       - a true or false value
array     - an array of elements, of maxim 4 K elements
blob       - a blob of data
Constants
and global functions (defined in apis.cpp from line 642 & 676)
void ::using(flags | ...);  Loads into VM context what we need)
where 'flags' are:
    eGPIO    - loads into VM the GPIO controller
    ePWMM    - loads into VM the PWM
    eI2C     - loads into VM the I2C
    eSPI     - loads into VM the SPI
    eADC     - loads into VM the ADC
    eTIMER   - loads into VM the TIMER
    eUART    - loads into VM the UART
    eBASE    - loads into VM the base script class to be extended
                 in script
    eSRV     - loads into VM the RESTFULL TCP server
    eDB      - loads into VM the SQLITE interface
    eFILE    - loads into VM the FILE access
    eSOLIB   - loads into VM the loader for modules and so libraries
    eSOCKET  - loads into VM the raw client socket
    eINPUT   - loads into VM the input class 'SYSIN' to access
                keyboard mouse & touch
    eSUNRS   - loads into VM the time zone sun rise and sun set API's
    eCURL    - if is compiled with -DWITH_CURL loads the CURL
                class API's
    eSSH     - if is compiled with -DWITH_SSH loads the SSH
                class API's
    eUSB     - if is compiled with -DWITH_USB loads the USB
                class API's
Functions
string execute(string command);            executes a linux command and
                                           returns the result as string
bool ::loadmod(string lib,                 full path to module.so
               string devname);            the object name given for
                                           the module.
                                           Have a look at bme280 module.
 Example: To read temperature using bme280
           adafruit
             Create the i2c device first give it a
               name in last parameter
             Create the plugin which is using the
                i2c and pass then name
             The colors show the relation
                between the binding
:loadmod("modules/libbme280-Linux-armv7l.so","THP"); // load mod, and exports THP class
i2c    := I2C(true, "/dev/i2c-2", 0x77, "i2c");      // built in I2C instance as i2c
thp    := THP("i2c", "bmp280");                      // the plugin uses the I2C to get data
var a = thp.temperature();
ctx_object context();               returns the global engine
                                    context instance.
                                    see CTX class for it;s methods
string i2a(integer);                convers an integer to a string
string x2a(integer);                an integer to hex string representation
                                    x2a(16) - > returns "F"
int uptime();                       returns uptime of the engine in seconds
slog(string);                       logs a string as information to
                                    /var/log/rembix/*.log
quit();                             breaks the engine flow and reloads the
                                    script and
                                    start's it again
int enable_wd(seconds);             enables the /dev/watchod watchdog. You have
                                    to call bark() function in within the timeout.
                                    Check your watch
                                    dog for maximum time in seconds that
                                    can be set
                                    returns 0 for success, -1 for error.
string get_strdate()/get_strtime()  returns the date and time of the system
                                    as string
array get_date()                    returns an array of 6 elements with
                                    year, month, date,
                                    hour, minute, second of the system time
bool arrarr(array1,array2)          compares 2 array of BYTES (0,255).
                                    array maximum length is 4k. Will skip
                                        the values > 0xFF
string intarr2a(array, string sep); converts an array of numerical values
                                    only into
                                    a string. Each element in the string is
                                    separated by
                                    string sep.
array arrcpyn(array a, offset, array b);
                                    copies array b into array a starting
                                    at offset offset.
                                    If the resulting array is greater
                                    then 'a' length
                                    will extend the a to the new size.
string consolein()                  If ran from console and not as a service
                                    prompts '>' and wait the user to
                                    enter input.
                                    returns the input
array a2arr(string)                 converts a hex string as "CB2AC803"
                                    where each octet
                                    takes 2 characters all uppercase to
                                    the array
                                    with all the octets converted
                                    to integers.
sleep(ms) / suspend(ms)             sleeps for ms milliseconds
string i2xa(int)                    returns a 0xHEX string representation
                                    of the int.
bool is_file(path_to_file)          returns true if the file exists
int wd_pull();                        notifies the watch dog. returns 0 for success
int run(function, interval);         runs the function calling it at
                                        interval in milliseconds
                                     returns 0 fr success, -1 for error.
exitapp()                            terminates the app in 2 to 3 seconds.
println(string)                      prints to console the string. Like:
                                     println("the value is" + numeric_value + " ok!");
errorln(string)                      printd to stderror in red the string.
Casting numeric functions
S8(number) / S16(number) S32(number) converts (strips) the number
                                     to 8 bits, 16 or 32
                                     as signed. Retains the lower bytes.
U8(number) / U16(number) U32(number) converts (strips) the number
                                     to 8 bits, 16 or 32
                                     as unsigned. Retains the lower bytes.
Built in Classes
Peripherals
GPIO
Constructors
var led = GPIO(26,                   GPIO pin
               DIR_OUT,              direction
               LOW,                  initial state
               "led");               REST JSON name, or reference to
                                    get it back from CTX
var button = GPIO(19,                pin gpio Number
                  DIR_IN,            direction
                  HIGH,              default state, would notify on LOW
                  "led2")            REST JSON name, or reference to
                                    get it back from CTX
var counter = GPIO(13,              gpio number
                   -maxfreq,        frequency fo rinterval to read the LOW/HIGH transitions
                   "counter")       at least 2.5 times higher than what we count.
                                    Can measure up to 400 Hz GPIO inputs
var buzzer  = GPIO(6,               Generates a tone of frequency. Max 800Hz
                   frequency,
                   "tone")
* For higher frequencies up to 100Khz frequencies use a PWM pin
 Methods
int GPIO::set_value(int val)                     sets gpio to 1 or 0
int  GPIO::get_value()                            returns the gpio value
int  GPIO::get_freq()                             for counter returns the frequency
int  GPIO::set_freq(int freq)                     for buzzer changes the frequency
int  GPIO::callback(function, risefall -1/1)      installs a callback
                                                  function => bool prototype(device)
int  GPIO::set_invert(bool)                       true, gpio is inverted, false,
                                                    back to normal
                                                set_value/get_value would return
                                                    the complement
                                                value
int  GPIO::set_toggle()                           togggles the state
int  GPIO::set_name(name)                         changes the alias name of the device
int  GPIO::get_name()                             returns the name
PWM
Constructors
PWM(string path-to-pwm-sys,        aka: /sys/class/pwm/pwmchip0/pwm1 or /dev/pwm/pwm0
    int frequency,                 100 max for the pwm of the board
    int duty_cycle%,               initial duty cycle in % (0-100). if >100 would
                                   modulo-it to 101
    bool invert,                   invert the duty cycle when written or read
    string name)                   name for reference, lookup and JSON
PWM(this,                          Custom c-tor, when derived in script pass
                                   the derived instance
    string path-to-pwm-sys,        same as above
    int frequency,                 pwm frequency
    int initial_duty,
    bool invert,
    string name)
Example: for a second constructor. A custom PWM that starts
         a motor in 2 steps. Holds it 10 seconds to 50%
        when starts or stops
class CustomPWM extends PWM
{
    _mymember1 = 0; // initialize evey variable to bind then to instance
    _timer = 0;
    _state = false;
    // // a must method
    constructor()
    {
        base.constructor(this, "/dev/pwm/pwm0",10000,0,false, "motor");
        _mymember1 = 0;        // here too otherwise is per
                               //class not per instance
        _timer = uptime();
    }
    /*
         some logic
    */
    function start(){
        base.set_duty(50);
        _timer = uptime();
        _state = 1;
    }
    function stop(){
        base.set_duty(50);
       _timer = uptime();
       _state = 0;
    }
    function check(){
        if(_state != -1)
        {
            if(uptime() - _timer > 10)
            {
                if(_state==1){
                     base.set_duty(100);
                }else{
                     base.set_duty(0);
                }
                _state=-1; //full stop or start
            }
        }
    }
}
 And the main
mypwm := CustomPWM();
/*
 or
var  mypwm CustomPWM();
 or
mypwm <- CustomPWM();
*/
function main(ctx)
{
    mypwm.start();
    return run(loop,1000);
}
function loop(c,d)
{
    mypwm.check();
    return true;
}
Methods
PWM.::set_value(duty);              sets duty from 0-100. If the value is > 100 will be
                                    module-ed to 101
PWM.::get_value();                  returns the duty in %
PWM.::call_back(function);          installs a callback that is called when the pwm
                                    value is changed
                                    function => bool prototype(device)
PWM.::set_invert(bool);             true, inverts the logic so a set_value(0) - > PWM 100%
                                    false put's the logic to normal
PWM.::get_name();                   returns it's name
ADC
Constructors
ADC(sysfile,                     path to sys file aka:
                                 /sys/bus/iio/devices/iio:device0/in_voltage0_raw
                                  (kernel 4.4.155-ti-r155)
    name)                        reference name string
ADC(SqObj,                      when derived, See the pwm example. only
                                the firs parameter is always this
    sysfile,
    name)
Methods
ADC::get_value();                     returns the raw value of the Analogue input
ADC::call_back(function);             installs a watch callback. The callback is called
                                      when the value changes.
Example:
var myadc = ADC("/sys/bus/iio/devices/iio:device0/in_voltage0_raw","batt");
function main(){
    myadc.call_back(opps);
    return run(looop,-1);    // loop always is called when a device value changes
                             // regardless the period (-1 never called periodically)
}
function loop(ctx, dev)
{
    println("device " + dev.name() + " value change");
    // you can also read it in main loop
/*
    var dev = ctx.get_dev("batt");
    println("in loop value is:" + dev.get_value());
*/
    return true;            // true keep the program alive, false, ends the loop
                            // and run returns and program exits
}
function oops(dev)
{
    println("in opps value is:" + dev.get_value());
    return truue;           // allways but ignored in engine
}
SPI
Constructors
SPI(string,                             like /dev/spi/0:0
    int,                                address
    int ,                               mode N / A
    int,                                word count
    int,                                frequency
    string);
SPI(SqObj, string, int, int , int, int, string);
Methods
SPI::open()                               opens the SPI device
SPI::close()                              closes the device
SPI::call_back(function)                  installs the callback to monitor
                                            changes
SPI::write(array)                         writes the array of bytes using file
                                            write  to spi
                                          device file.
array SPI::read(bytes)                    reads bytes from device file using
                                            read of maximum
                                          bytes. returns an array of bytes
array SPI::ioctl(int IO_OP,               ioctl integer value.
           array,                         data sent to device during ioctl as
                                            array of bytes
           int expect)                    expecting bytes back
                                          returns the array with data being
                                            retrieved by ioctl
array SPI::ioread(bytes)                  reads the device using ioctl, max bytes.
                                          returns data read
int SPI::iowrite(array)                   writes to device the array, returns
                                            bytes written
I2C
Constructors
I2C(EI2CBUS,                              i2c bus 0 1 2 and so on
    int addr,                             address of the device aka: 0x11
    string name);                         JSON/REST name
I2C(SqObj,
    EI2CBUS,                              i2c bus 0 1 2 and so on
    int addr,                             address of the device aka: 0x11
    string name);                         JSON/REST name
I2C(true,                                 N/A
    string ,                              aka: /dev/i2c-1
    int addr,                             address of the device aka: 0x11
    string name);                         JSON/REST name
I2C(true,                                 N/A
    SqObj,
    string ,                              aka: /dev/i2c-1
    int addr,                             address of the device aka: 0x11
    string name);                         JSON/REST name
Methods
I2C::call_back(function)                 installs a callback. Called when value changes
I2C::open()                              opens device, ruenrn true if successful
I2C::close()                             closes device
I2C::autoopen(bool)                      true, you don't have to call open / close
                                         each wr/rd/ioctl would open-read-io-write-then-close
                                         false, in cases when the device has to be kept open and not closed
                                         between rd/wr/ioctl operations
int I2C::write(array)                    writes to device file descriptor as in a file
array I2C::read(bytes)                   reads as is reading from a file not a  device
                                         up to max bytes
array I2C::ioctl(ioflag, array, expect)  performs a device ioctl(flag, &array, output)
                                         compares the read byes in output with expecting
                                         bytes and returns the array of bytes
bool I2C::setreg(int reg)                sets current register address to reg
bool I2C::writecmd(cmd)                  writes cmd command, usually at address 0
array I2C::ioread(register, bytes)       read from register up to max bytes.
                                            returns the data
int I2C::iowrite(register, array)        writes to register the array of bytes.
                                         return written
                                         bytes
Example OLED96
::using(eI2C|eGPIO|ePWMM|eSRV|eDB);
::include("modules/_oled96.inc");
print( "If error is: Bad file descriptor. Run as sudo \n");
dbs   := DB(10000, 1000, "./database");
json := SRV(8000,"rest");
local s = Oled96("/dev/i2c-2", 0x3c, "OLED96");
function main(a)
{
    var k;
    s.autoopen(false);          // write read would not open
    if(s.xopen("128x64",0,0))
    {
        s.clear_display(0);
        k = run(kk, 1000);
    }
    s.xclose();
    return run(kk,50);
}
var K=100;
function kk(ctx,dev)
{
    s.write_string(0,0, get_strtime() + ":" + K ,2);
    K--;
    return K > 0 ? true : false;
}
UART
Constructors
UART(E_TYPE,                   eSTRING, eBINARY
    string,                    /dev/ttyUSB0 for example
    int,                       BPS as 19200
    string,                    "8,n,1"  like 8 bits, no parity 1 stop
    string);                   JSON name
UART(SqObj,                    derived class instance, always 'this'
    E_TYPE,                    as above
    string,
    int, string,
    string);
Methods
int UART::setcr(array)                    Sets the end of line character used when
                                          puts is called. aka: setcr([0xD]);
int UART::puts(string)                    appends the end of string and sends as string
int UART::puts_cb(string, function)
string UART::gets(max_characters)        reads up to max characters. returns what was read
array UART::read(max_bytes)              reads max bytes. return array of bytes
int UART::write(array)
string UART::expect_any(array, timeout ms) if the array sequence is found in received data
                                           returns the string that contains the array
string UART::received()                    returns leftovers
bool UART::write_cb(array, function)       writes array then calls the cb as in puts_cb
                                           but with array of bytes
bool UART::expect_str(string, int timeouts)  waits for the sequence
bool UART::expect_bin(array, int timeouts)   waits for the array
string UART::pick_str();                      instant read a string
array UART::pick_bin();                       instant read binary
void UART::flush()                            clears pending i/o
USB
Constructors
Methods
BASE
Constructors
Methods
*
CTX
 this class is a singleton and the instance of it is passed in loop
or returned from ::context() global API
instance CTX::get_dev(string devname);             returns the device by name
timerid = CTX::set_timer(callback_foo,             foo(CTX, timer_id);
                        timout_ms,                 10...1000000, 0 to remove the timer
                        user_timer_id);            unique id passed in cb. A unique number.
timerid = CTX::set_cron("cron syntax", callback, id);  TODO
System classes
FILE
