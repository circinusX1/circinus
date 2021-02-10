// SCRIPT  TESTED ON R-PI
::using(eUART);

port := UART(eSTRING,"/dev/ttyUSB0",115200,"8N1","webname");
setdbg(0);
function main(x)
{
    //port.setcr([0x13,0x10]);
    if(port.open(3))
    {
        port.on_event(monitor);
        port.putsln("");
    }
    return run(loop,100);
}

function loop(ctx, dev)
{
    return true;
}


function monitor(d)
{
    var r = d.gets();
    println("GOT + [" + r + "]");

    if(r.find("login")!=null)
    {
        d.putsln("debian");
    }
    else if(r.find("Password")!=null)
    {
        d.putsln("temppwd");
    }
    if(r.find("beaglebone")!=null)
    {
        port.on_event(live);
        d.putsln("ls -lR");

    }
    return true;
}


function live(d)
{
    var r = d.gets();
    println("LIVE + [" + r + "]");
    return true;
}
