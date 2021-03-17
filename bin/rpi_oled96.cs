// SCRIPT  TESTED
// HOOK OLED LIKE HERE http://comarius.blogspot.com/2020/10/096-i2c-iic-spi-serial-128x64-white.html
//
::using(eI2C|eGPIO|ePWMM|eSRV);
::include("modules/_oled96.inc");

/*
@PI SHELL
sudo nano /lib/udev/rules.d/60-perip.rules
KERNEL=="gpiochip*", GROUP="gpio"
KERNEL=="pwmchip*", GROUP="gpio"
KERNEL=="i2c-0"     , GROUP="i2c", MODE="0660"
KERNEL=="i2c-[1-9]*", GROUP="i2c", MODE="0666"
# save

sudo adduser $USER i2c
sudo adduser $USER gpio
sudo adduser $USER pwm
sudo service udev restart

sudo apt-get install i2c-tools
ls /dev/i2c*
      /dev/i2c-1   1 is your i2c
i2cdetect -y -r 1  # detect OLED i2c address. '3c'
*/

json := SRV(8000,"rest");
local s = Oled96("/dev/i2c-2", 0x3c, "OLED96");

//s.autoopen(false);
function main(a)
{
    var k;
    s.autoopen(false);
    if(s.xopen("128x64",0,0))
    {
        s.set_cb(on_change,0xFF);
        s.clear_display(0);
        s.write_string(0,1, get_strtime() ,2);
        k = run(kk, 10000);
    }
    s.xclose();
    return k;
}

function kk(ctx,dev)
{
    println("kk called");
    s.write_string(0,1, get_strtime() ,2);
    return true;
}

function on_change(d,data)
{
    println("on_change:" + data);
    return true;
}

