// SCRIPT  TESTED
::using(eSOLIB);

/**
  check your ao library location
  */
// lib := LIB("/usr/lib/x86_64-linux-gnu/libao.so");
lib := LIB("/usr/lib/arm-linux-gnueabihf/libao.so");


lib.load("ao_initialize",false,0);
lib.load("ao_default_driver_id",true,0);
lib.load("ao_driver_id",true,1);
lib.load("ao_open_live",true,3);
lib.load("ao_play",false,3);
lib.load("ao_close",0,1);
lib.load("ao_shutdown",0,0);

function main(x)
{
    // find a wav PCM file, ao does snot decode
    var file   = FILE(eBINARY, "./iris.wav","iris");
    var buffer = BUFF([4096]);                  //   file buffer
    var format = BUFF(['i''i','i','i','v']);    // format structure int, int int int void
    format.set([16,44100,2,1,0]);               // populate structure
    
    print(file + " file\n");

    ao_initialize();                            // init ao
    var default_driver = ao_default_driver_id();    
//    var default_driver = ao_driver_id("hw:0");
    
    var device = ao_open_live(default_driver, format.get(), 0);
    
    print(default_driver + " default_driver\n");
    print(device + " device\n");

    if(device && file.open(1))
    {
        while(1)
        {
            var bytes = file.fetch(buffer);     // fetch from file into buffer
            if(bytes)
                ao_play(device, buffer.get(), bytes);   // play it
            if(bytes < buffer.length())         // last frame
                break
        }
        file.close();
    }
    else
        println("no device or no mediafile");
    ao_close(device);
    ao_shutdown();
    println("done");
    return false;
}
