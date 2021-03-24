// SCRIPT  TESTED
::using(eSOLIB);

/**
  check your ao library location
  */
lib := LIB("/usr/lib/x86_64-linux-gnu/libao.so");

lib.load("ao_initialize",false,0);
lib.load("ao_default_driver_id",true,0);
lib.load("ao_open_live",true,3);
lib.load("ao_play",false,3);
lib.load("ao_close",0,1);
lib.load("ao_shutdown",0,0);

function main(ctx)
{
    var param = ctx.args();
    if(param==null || param.len()!=3)
    {
        println("Expecting parameter: 'dev #' or 'hw #'. Find it with aplay -l");
        println("to play on default 'dev 0', to play on hw: 'hw 0' ");
        return false;
    }
    println("using device " + param[1] + " hw:" + param[2]);
    
    // find a wav PCM file, ao does snot decode
    var file   = FILE(eBINARY, "/home/marius/Music/iris.wav","iris");
    var buffer = BUFF([4096]);                  // file buffer
    
    var format  = BUFF(['I''I','I','I','P']);    // format structure int, int int int void
    var options = BUFF(['P','P','P']);
    format.set([16,44100,2,1,0]);               // populate structure
    options.set([ param[1], param[2] ]);        // param[0] is the script name
    options.get();
    
    ao_initialize();                            // init ao
    var default_driver = ao_default_driver_id();    
    print(default_driver + " default_driver\n");
    var device = ao_open_live(default_driver, format.get(), options.get());
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
    ao_close(device);
    ao_shutdown();
    println("done");
    return false;
}
