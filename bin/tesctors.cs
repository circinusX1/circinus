// SCRIPT  TESTED

::using(eADC|eI2C|eSRV);

// i2c ///////////////////////////////////////////


class I2cEx extends I2C
{
    _value = 0;
    constructor(sysfile, addr, name)
    {
        _value = 0;
        base.constructor(true, this, sysfile, addr, name);
    }
};


i2c1 := I2C(2, 0x23, "ctor by i2c number");
i2c2 := I2C(true, "/dev/i2c-1", 0x23, "ctor by path number");
i2c3 := I2cEx("/dev/i2c-1", 0x23, "ctor by path number");


// adc ///////////////////////////////////////////

class AdcEx extends ADC
{
    _value = 0;
    constructor(sysfile, name)
    {
        _value = 0;
        base.constructor(this, sysfile, name);
    }

    function calculate()
    {
        print("calculate() called\n");
        var v = base.get_value();
        _value = v*1000;
        return _value;              // do some scalling
    }
};


system ("echo 3333 > /tmp/adc0"); // fake sys files
system ("echo 4003 > /tmp/adc1");
system ("echo 1003 > /tmp/adc2");

var alive=true;
var default_adc = ADC("/tmp/adc0","internal");
var extended_adc = AdcEx("/tmp/adc1","extend");
var sys_adc = ADC("/tmp/adc2","watched");

//gpio
var g1 = GPIO(4,DIR_OUT,1,"gpio1");




function main(cx)
{
    var d1 = default_adc.get_value();
    var d2 = extended_adc.calculate();

    sys_adc.get_value();
    var d3 = sys_adc.on_event(watch);

    print("d1 = " + d1 + " and d2=" + d2 + " d3 = " + d3 + "\n");

    return run(program,1000);
}

function program(c,d)
{
    return alive;
}

function watch(dev)
{
    print("analog event value = " +  dev.get_value() + "\n");
    alive=false;
    return true;
}

