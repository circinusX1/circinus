function main(x)
{

foreach(k, d in getroottable())
{
    println(k +"="+ d);
    if(k=="class")
    {
        foreach(kk, dd in d)
        {
            println(""+ kk +"="+ dd);
        }
    }
}

}
