
dofile("included.tcc")

local demo = Demo();    // local variable to script.tcc
demo1 <- Demo(7);       // global variable across multiple script files
                        // if this is defined in ina include file would be visible


function setup(param)
{
    local temp = Demo(5);
    return 1;
}                           // temp class would be sestructed because get's out of scope

function loop()
{
    local temp2 = Demo(6);

    print("we are in loop function \n");

    GlobalCall("passed from script");

    local v1 = demo.Method(1,"demo global 1");
    print("v1 is " + v1 + "\n");

    local v2 = demo1.Method(2,"demo global 1");
    print("v2 is " + v2 + "\n");

    local v3 = temp2.Method(3, "local demo");
    print("v3 is " + v3 + "\n");

    local v4 = demo_obj_from_include.Method(4,"calling demo global from include\n");

    print("v4 is " + v4 + "\n");
    return 0;
}
