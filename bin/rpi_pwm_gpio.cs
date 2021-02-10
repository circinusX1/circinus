/**
SCRIPT  TESTED
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

//var l1 = GPIO(21,  DIR_OUT, HIGH, "led1");
//var l2 = GPIO(20,  DIR_OUT, HIGH, "led2");


var edge = GPIO(14, DIR_IN, HIGH, "detect");

var pwm0 = PWM("/sys/class/path_to_pwm0/folder", 1000, 100, false, "pwm1"); //GPIO 13
var pwm1 = PWM("/sys/class/path_to_pwm1/folder", 1000, 100, false, "pwm2"); //GPIO 18
//var pb = GPIO(21,  DIR_IN,  LOW, "buton");

function main(ctx)
{
    edge.on_event(callback);
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
