// SCRIPT  TESTED
/**
# @ /boot/config.txt
dtoverlay=pwm,pin=18,func=2
dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=4
*/
::using(eGPIO|ePWMM|eSRV);

setdbg(0);

var l1 = GPIO(26,  DIR_OUT, LOW, "led");
var l2 = GPIO(19,  DIR_OUT, HIGH, "led2");
var l3 = GPIO(13,  DIR_OUT, LOW, "led3");
var l4 = GPIO(6,   DIR_OUT, HIGH, "led4");
var pb = GPIO(21,  DIR_IN,  LOW, "buton");
var tone = GPIO(17,  50, "tone");
var counter = GPIO(4, -1000, "count");
var pwm = PWM("0.0", 1000, 100, false, "pwm");
var srv = SRV(8000,"srv");
var value = 1;
var k = 0;

function main(ctx)
{
    //print(ctx +  typeof ctx + " the ctx");
    ctx.notify("appname");
    pb.set_cb(callback);
    l1.set_value(0);
    return run(loop,10000);
}

function loop(ctx, dev)
{
    l1.set_value(value);
    l2.set_value(!value);
    l3.set_value(value);
    l4.set_value(!value);
    pwm.set_value(k++);
    value=!value;
    println("counter = " + counter.get_freq());
    return true;
}

function callback(dev)
{
    println("dev = " + dev.get_value());
    return true;
}

