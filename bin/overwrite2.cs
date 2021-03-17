// SCRIPT  NOT TESTED
::using(eUART|eSRV|eBASE); // load in VM only what we usee. New as Nov 29 !

class MyDev  extends BASE
{
    _retparams="";
    _data=[];
    _k  = 0;
    _uart = UART(eSTRING, "/dev/ttyUSB0",115200,"8N1","rs232");

    constructor(whatever)
    {
        _k    = 0;
        _retparams = "";
        _data = [1,2,3,0,0];

        base.constructor(this, eSTRING, whatever);
        _uart.setcr([0x0D]);
    }

    function get_value(what)
    {
        var json_obj = {
            data = _data,
        }
        return json_obj;
    }

    function set_value(jdoc)
    {
        foreach(k, d in jdoc)
        {
            if(typeof d=="array")
            {
                println("array " + k + " has " + d.len() +" elements");
                for(var i=0;i<d.len();i++)
                {
                    print(" elem: " i + ":" +d[i]);
                }
            }
            else
                println(k + " = " + d);
        }
        this._retparams=json;
    }

    function set_value() // return array of max 5
    {
        print("set_value\n");
        return [2,3,4,5];
    }

    function write_data(array)
    {
        this._data=array;
    }

    function read_device()
    {
        var xx;
        var sall;
        _uart.puts("enquire\n");
/*
        while(xx=_uart.gets(6)!=null)
        {
            sall+=string;
            msleep(128);
        }
        */
        _data[0] = rand() & 0xFF;
        _data[1] = rand() & 0xFF;
    }
};

devs <- [];
for(local l=0;l<12;l++)
{
    devs.append(MyDev("DEVICE"+l));
}



//json := SRV(8000,"SRV");

function main(o)
{
    println("MAIN ");
    return run(program_loop,2000);
}

function program_loop(ctx, dev)
{
    ctx.notify("sdfsf");
    var d = ctx.get_dev("rs232");
    d.set_cb(callback);
    d.gets(100);
    println("LOOPS");
    return false;
}

function callback(obj)
{
    return true;
}
