// SCRIPT  TESTED

::import("./solibs/libardulike-OS-ARCH.so");

var LED=26;

function main(x)
{
    wiringPiSetupGpio () ;

    pinMode (LED, OUTPUT) ;

    for (;;)
    {
        digitalWrite (LED, 1) ;     // On
        delay (500) ;               // mS
        digitalWrite (LED, 0) ;     // Off
        delay (500) ;
    }

}
