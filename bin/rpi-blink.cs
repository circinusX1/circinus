// SCRIPT  TESTED
/**
# @ /boot/config.txt
dtoverlay=pwm,pin=18,func=2
dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=4
*/
::using(eGPIO);

var led = GPIO(23,  DIR_OUT, LOW, "led");

function main(c)
{
    for(var v=0;v<100;v++)
    {
        led.set_value(0);
        sleep(100);
        led.set_value(1);
        sleep(100);
    }
    return false;
}
