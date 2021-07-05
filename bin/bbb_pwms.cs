// SCRIPT  TESTED ON Linux beaglebone 4.4.155-ti-r155 #1 SMP Thu May 23 05:50:27 UTC 2019 armv7l GNU/Linux
::use(eGPIO|ePWMM|eSRV);

setdbg(0xFF);


devs_a := [];

// whatever all the pins
execute("config-pin P9.14 pwm");
execute("config-pin P9.11 pwm");
execute("config-pin P9.22 pwm");
execute("config-pin P9.31 pwm");
execute("config-pin P9.21 pwm");
execute("config-pin P9.29 pwm");
execute("config-pin P9.42 pwm");
execute("config-pin P9.14 pwm");
execute("config-pin P8.36 pwm");
execute("config-pin P9.16 pwm");
execute("config-pin P8.34 pwm");
execute("config-pin P8.19 pwm");
execute("config-pin P8.45 pwm");
execute("config-pin P8.13 pwm");
execute("config-pin P8.46 pwm");
execute("config-pin P8.28 pwm");


devs_a.append(PWM("/sys/class/pwm/pwmchip0/pwm0",3000,3000,false,"c0p0"));

devs_a.append(PWM("/sys/class/pwm/pwmchip1/pwm0",3000,3000,false,"c1p0"));
devs_a.append(PWM("/sys/class/pwm/pwmchip1/pwm1",3000,3000,false,"c1p1"));

devs_a.append(PWM("/sys/class/pwm/pwmchip3/pwm0",3000,3000,false,"c3p0"));
devs_a.append(PWM("/sys/class/pwm/pwmchip3/pwm1",3000,3000,false,"c3p1"));

devs_a.append(PWM("/sys/class/pwm/pwmchip5/pwm0",3000,3000,false,"c5p0"));

devs_a.append(PWM("/sys/class/pwm/pwmchip6/pwm0",3000,3000,false,"c6p1"));
devs_a.append(PWM("/sys/class/pwm/pwmchip6/pwm1",3000,3000,false,"c6p2"));



function main(ctx)
{
    foreach (pwm in devs_a) {
        pwm.set_duty(100);
        print(pwm.get_name() + " = " + pwm.get_duty() + "\n");
    }
    suspend(1000);
    print("main \n");
    return run(loop,10);
}

duty:=0;
function loop(c,d)
{
    foreach (pwm in devs_a) {
        pwm.set_duty(duty);
        print(pwm.get_name() + " = " + pwm.get_duty() + "\n");
    }
    if(duty > 99)
        duty=0;
    duty =  duty + 5;
    return true;
}

