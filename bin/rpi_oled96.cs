// SCRIPT  TESTED
// HOOK OLED LIKE HERE http://comarius.blogspot.com/2020/10/096-i2c-iic-spi-serial-128x64-white.html
//
::using(eI2C|eGPIO|ePWMM|eSRV|eDB);
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
local s = Oled96("/dev/i2c-1", 0x3c, "OLED96");

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
  //  println("L1=" + l1.set_toggle());
  //  println("L2=" + l2.set_toggle());
  //  sleep(1000);

   // ctx.get_dev("led2").set_toggle();
    K--;
    return true;
}

