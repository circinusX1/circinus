// SCRIPT  TESTED

::using(eSOLIB|eGPIO|ePWMM);

setdbg(0xff);

lib := LIB("libwiringPi.so");

lib.load("wiringPiSetupGpio",true,0);   // has return value takes 0 params
lib.load("digitalWrite",false,2);       // no return 2 parameters
lib.load("delay",0,1);                  // har return takes one param
lib.load("pinMode",0,2);                // har return takes one param

var LED = 26;
function main(x)
{
    wiringPiSetupGpio() ;
    pinMode (LED, OUTPUT) ;
    for (;;)
    {
        digitalWrite (LED, 1) ;     // On
        delay (500) ;               // mS
        digitalWrite (LED, 0) ;     // Off
        delay (500) ;
    }
    return false;
}
