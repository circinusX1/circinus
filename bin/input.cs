// SCRIPT  TESTED ON R-PI
::using(eSYSIN|eSRV);

json := SRV(8000,"rest");
mouse := SYSIN(eMOUSE,"/dev/input/event4","mice");

ctx().set_priority(-20);
ctx().set_priority(20);
/**
reads touch pad. WIP
*/
function main(x)
{

    mouse.set_cb(callback);
    mouse.open(0);
    return run(foo,1000);
}

function foo(c,d)
{
    return true;
}

function callback(d)
{
    var xy = d.get_mouse();
    println("x=" + xy[0] + " y=" + xy[1]);
    return true;
}
