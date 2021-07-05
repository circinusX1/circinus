// SCRIPT  TESTED
::use(eSOLIB);

/**
  check your ao library location
  */
lib := LIB("/usr/lib/x86_64-linux-gnu/libao.so");
mpg := LIB("/usr/lib/x86_64-linux-gnu/libmpg123.so");

lib.load("ao_initialize",false,0);
lib.load("ao_default_driver_id",true,0);
lib.load("ao_open_live",true,3);
lib.load("ao_play",false,3);
lib.load("ao_close",0,1);
lib.load("ao_shutdown",0,0);


mpg.load("mpg123_init",true,0);
mpg.load("mpg123_exit",false,0);
mpg.load("mpg123_new",true,2);
mpg.load("mpg123_outblock",true,1);
mpg.load("mpg123_open",true,2);
mpg.load("mpg123_getformat",true,4);
mpg.load("mpg123_open_feed",true,1);
mpg.load("mpg123_close",true,1);
mpg.load("mpg123_delete",false,1);
mpg.load("mpg123_read",true,4);
mpg.load("mpg123_encsize",true,1);

class Mp3Cls
{
    _mh = null;
    _frmlen = 0;
    _bits  = 0;
    _rate = null;
    _encod = null;
    _chann = null;
    
    constructor(){
        _mh     = null;
        _frmlen = 0;
        _bits   = 0;
        _rate   = BUFF(['P']); 
        _encod  = BUFF(['P']); 
        _chann  = BUFF(['P']); 
        mpg123_init();
    }
    
    
    function open(file)
    {
        var err =  BUFF(['P']);
        _mh = mpg123_new(0, err.get());
        
        println("mh = " + _mh + " err+ " + err.deref());
        if(_mh != null)
        {
            _frmlen = mpg123_outblock(_mh);
            println("frmlen = " + _frmlen);
        }
        var err = mpg123_open(_mh, file);
        if(err!=-1)
        {
            ::mpg123_getformat(_mh, _rate.get(), _chann.get(), _encod.get());
            _bits = mpg123_encsize(_encod.deref()) * 8;
        }
        return _bits;
    }
    
    function bits(){return _bits;}
    function rate(){return _rate.deref();}
    function channels(){return _chann.deref();}
    function encoding(){return _encod.deref();}
    
    function read(buff, length)
    {
        if(_mh)
        {
            var read=BUFF (['P']);
            mpg123_read(_mh, buff, length, read.get());
            return read.deref();
        }
        return 0;
    }
    
    function tearit(){
        if(_mh){
            mpg123_close(_mh);
            mpg123_delete(_mh);
        }
        mpg123_exit();        
    }
}

function main(ctx)
{
    var param = ctx.args();
    if(param==null || param.len()!=3)
    {
        println("Expecting parameter: 'dev #' or 'hw #'. Find it with aplay -l");
        println("to play on default 'dev 0', to play on hw: 'hw 0' ");
        return false;
    }
    
    var mp3 = Mp3Cls();
    println("using device " + param[1] + " hw:" + param[2]);
    //          https://www.youtube.com/watch?v=ZK5rCuuXhmQ
    if(mp3.open("/home/marius/Music/dafface.mp3"))
    {
        println("FILE IS OPEN");
        var buffer = BUFF([4096]);                  // file buffer
        var format  = BUFF(['I''I','I','I','P']);    // format structure int, int int int void
        var options = BUFF(['P','P','P']);
        //#define AO_FMT_NATIVE 4
        
        format.set([mp3.bits(), mp3.rate(), mp3.channels(),4,0]);               // populate structure
        options.set([ param[1], param[2] ]);        // param[0] is the script name
        options.get();
        
        ao_initialize();                            // init ao
        var default_driver = ao_default_driver_id();    
        print(default_driver + " default_driver\n");
        var device = ao_open_live(default_driver, format.get(), options.get());
        print(device + " device\n");
        
        if(device)
        {
            println("FILE PLAYING");
            while(1)
            {
                var bytes = mp3.read(buffer.get(),4096);     // fetch from file into buffer
                if(bytes)
                    ao_play(device, buffer.get(), bytes);   // play it
                if(bytes < buffer.length())         // last frame
                    break
            }
        }
        ao_close(device);
        ao_shutdown();
        mp3.tearit();
        println("done");
    }
    return false;
}
