// SCRIPT  TESTED

::using(eI2C|eSRV);
::loadmod("modules/libbme280-Linux-armv7l.so","THP");

i2c    := I2C(true, "/dev/i2c-2", 0x77, "i2c");  // the i2c THP is using
thp    := THP("i2c", "bmp280"); // pass the i2c name as first parameter
server := SRV(8000,"X");

function main(x)
{
    return run(mainloop,1000);
}

function showth(dev)
{
    var a = dev.thp();
    println("THP = " + a[0] +
            "  "  +    a[1] +
            "  "  +    a[2] );
}

function mainloop(ctx, notused)
{
    // read one by one every 1000 ms
    println("T=" + thp.temperature() + " m*C  H=" +
                   thp.humidity()   + " x100 %  P=" +
                   thp.pressure()   + " mm Hg ");
    return true;

}
