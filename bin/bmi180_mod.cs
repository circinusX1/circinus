// SCRIPT  TESTED ON BBB

::use(eI2C|eSRV);
::loadmod("./modules/libbmi160-Linux-armv7l.so","ACCEL");

i2c    := I2C(true,"/dev/i2c2", 0x68, "i2c");
accel  := ACCEL("i2c","accel");
server := SRV(8000,"xxx");

function main(x)
{
    return run(l, -1);
}

function l(c, d)
{
    var data1 = accel.acceleration();
    var data2 = accel.rotation();

    do {
        println("ACC " + data1.pop());
    } while (data1.len() > 0);

    do {
        println("ACC " + data2.pop());
    } while (data2.len() > 0);
    return true;
}
