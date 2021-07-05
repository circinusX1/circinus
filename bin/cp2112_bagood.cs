// SCRIPT  TESTED

::use(eGPIO|eSRV);

json := SRV(8000,"rest");

led:=GPIO(357, DIR_OUT, LOW, "led");
led2:=GPIO(358, DIR_OUT, LOW, "led2");
button:=GPIO(359, DIR_IN, HIGH, "but");
level:=1

function main(ctx)
{
    led.set_value(HIGH);
    button.set_cb(pushed);
    return run(loop,100);
}

function loop(ctx)
{
    led.set_value(level);
    level=!level;
    return true;
}

function pushed(dev)
{
    led.set_value(dev.get_value());
    return true;
}
