// SCRIPT  TESTED
/**
# @ /boot/config.txt
dtoverlay=pwm,pin=18,func=2
dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=4
*/
::use(ePWMM|eSRV);
/*

if(::is_file("/sys/class/pwm/pwmchip0/export")==false)
{
    errorln("The PWM is not enabled in /boot/config.txt");
    exitapp("");
}

var whoami=::execute("whoami");
if(whoami=="root")
{
    println("exporting pwm 0")
    ::execute("echo 0 > /sys/class/pwm/pwmchip0/export");
}
else
{
    errorln("PWM access requires root privileges. Restart as root not as:" + whoami);
    exitapp("");
}
if(::is_file("/sys/class/pwm/pwmchip0/pwm0")==false)
{
    errorln("/sys/class/pwm/pwmchip0/pwm0 does not exist. Please export it");
    exitapp("");
}
    */

var pwmdev = "/home/marius/tmp/pwm0"; // PC test
//var pwmdev = "/sys/class/pwm/pwmchip0/pwm0";
var pwm    = PWM(pwmdev,10000, 0, false, "pwm");
var srv    = SRV(8008,"srv");
var duty   = 0;
var sinus  = 0;

function main(ctx)
{
    pwm.set_duty(10);
    return run(loop,-1);
}

function loop(ctx, dev)
{
    duty = sin(sinus)*100;
    sinus += 3.14/45.0;
    //pwm.set_duty(duty);
    return true;
}
