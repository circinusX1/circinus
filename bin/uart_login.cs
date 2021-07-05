// SCRIPT  TESTED ON R-PI
::use(eUART);

port := UART(eSTRING,"/dev/ttyUSB0",115200,"8N1","webname");
setdbg(0xFF);
function main(x)
{
    //port.setcr([0x13,0x10]);
    if(port.open(3))
    {
        port.set_cb(monitor);
        port.putsln("");
    }
    return run(loop,100);
}

function loop(ctx, dev)
{
    return true;
}


function monitor(d, r)
{
    println("GOT + [" + r + "]");

    if(r.find("login")!=null)
    {
        d.putsln("debian");
    }
    else if(r.find("Password")!=null)
    {
        d.putsln("temppwd");
    }
    if(r.find("~$")!=null)
    {
        port.set_cb(live);
        d.putsln("ls -lR");
    }
    return true;
}


function live(d, r)
{
    println("LIVE + [" + r + "]");
    d.putsln("exit");
    d.putsln("");
    var got = port.expect_any(["login","Password","beaglebone"]);
    println ("got->" + got);
    return true;
}
