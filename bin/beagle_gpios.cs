// SCRIPT  TESTED  ON BBB

::using(eGPIO|ePWMM);

system("config-pin P9.41 gpio");
system("config-pin P9.42 gpio");
system("config-pin P9.41 out");
system("config-pin P9.42 in-");

led    := GPIO(20, DIR_OUT, LOW, "led");
button := GPIO(7, DIR_IN, LOW, "buton");


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



