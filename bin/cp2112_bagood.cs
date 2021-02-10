// SCRIPT  TESTED

::using(eGPIO|eSRV);

json := SRV(8000,"rest");

led:=GPIO(357, DIR_OUT, LOW, "led");
button:=GPIO(358, DIR_IN, HIGH, "but");
level:=1

function main(ctx)
{
    led.set_value(HIGH);
    button.set_cb(pushed);
    return run(loop,-1);
}

function loop(ctx, dev)
{
    println("event"+ dev);
    return true;
}

function pushed(dev)
{
    led.set_value(dev.get_value());
    return true;
}
