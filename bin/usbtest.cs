
::use(eUSB);

function main(c)
{
    var usb = USB(eTEXT, "454:1292","usb");
    var arr = usb.enumerate();
    for(var i=0;i<arr.size();i++)
        println(arr[i]);
    return false;
}


