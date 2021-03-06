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
 SCRIPT  TESTED ON BBB  I2C-2. started as sudo due the i2c dev
*/


::using(eI2C|eGPIO|ePWMM|eSRV|eDB);
::include("modules/_oled96.inc");


print( "If error is: Bad file descriptor. Run as sudo \n");
dbs   := DB(10000, 1000, "./database");
json := SRV(8008,"rest");
local s = Oled96("/dev/i2c-2", 0x3c, "OLED96");


function main(a)
{
    var k;
    s.autoopen(false);          // write read-ops woud not open. xopen open i2c once
    if(s.xopen("128x64",0,0))
    {
        s.clear_display(0);
        k = run(kk, 1000);
    }
    s.xclose();
    return true;
}

var K=100;

function kk(ctx,dev)
{
    s.write_string(0,0, get_strtime() + ":" + K ,2);
    K--;
    return K > 0 ? true : false;
}

