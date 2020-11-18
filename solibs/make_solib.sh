#!/usr/bin/php
<?php

$header="";
$libo="";
if($argc>1)
    $libo = $argv[1];

echo "searching lib{$libo}.so  and {$libo}.h\n";

if($argc>=3)
    $header = $argv[2];

if(!empty($libo))
{
        $l = explode("\n",trim(shell_exec("find /usr -name lib{$libo}.so"),"\n"));
        $h = explode("\n",trim(shell_exec("find /usr -name {$libo}.h"),"\n"));
        echo "\nsearching lib{$libo}.so  and {$libo}.h \n";

        if(count($l)!=1 || count($h)!=1)
        {
 	        var_dump($l);
            var_dump($h);
            die("\more than one header of lib or not at all was found in /usr/*\n");
        }
        $header = $h[0];
        $lib = $l[0];
        echo "\nfound LIB:{$lib} and H:{$header}\n";
}
else
{
	die ("pass the library name with no extension and without the lib prefix eg: pthread\n");
}

echo $lib."\n";
if(!is_file($header)){
    echo "\nsearching  lib{$libo}.h \n";
    $h = explode("\n",trim(shell_exec("find /usr -name lib{$libo}.h"),"\n"));
    if(count($h)!=1)
    {
        var_dump($h);
        die("\more than one header or no header of lib was found in /usr/*");
    }
    $header = $h[0];
}

if(!is_file($lib)){
    die("no lib {$lib} found \n");
}


//echo "//readelf -s {$lib} | grep FUNC | grep DEFAULT | grep GLOBAL | grep -v 'GLIBC' | grep -v '__' | awk {'print $8'} | awk -F '@' '{print $1}'";
//$foos = shell_exec("readelf -s {$lib} | grep FUNC | grep DEFAULT | grep GLOBAL | grep -v 'GLIBC' | grep -v '__' | awk {'print $8'} | awk -F '@' '{print $1}'");

echo "// parsing: nm -D  {$lib} | grep ' T ' | awk '{print $3}'";
$foos = shell_exec("nm -D  {$lib} | grep ' T ' | awk '{print $3}'\n");
$arr = explode("\n",$foos);

echo "found {$header} \n";


$hdr = explode("\n",file_get_contents($header));

$funcs="";
$consts="";

foreach ($hdr as $h)
{
    if(strstr($h,"#define"))
    {
        $consts.=$h.",";
    }
}

$structs="";
$in=false;
for ($struct=0; $struct<count($hdr); $struct++)
{
    if(strstr($hdr[$struct],"struct"))
    {
         $structs.=$hdr[$struct]."\n";
         $in=true;
         continue;       
    }
    if($in)
    {
         $structs.=$hdr[$struct]."\n";
         if(strstr($hdr[$struct],"}")){
            $in=false;
        }
    }
}


$keywords = array("enum","const","null","mutable","volatile");


$liste = array();
$coment=false;
foreach ($arr as $f)
{
    for($lh=0;$lh<count($hdr);$lh++)
    {
     	$h = $hdr[$lh];

       
        if(strstr($h, "/*") && strstr($h, "*/")) { $coment=false; continue;}
        if(strstr($h, "/*")) { $coment=true; continue;}
        if(strstr($h, "*/")) { $coment=false; continue;}

        if($coment) {
            echo "COMENT: {$h} \n";            
            continue;
        }
        
        if(!empty($f) && strstr($h, $f) && strstr($h,"("))
        {
            //echo " ----------  fline = ".$h. "\n";

	        if(strstr($h,"(*")) continue; //ptr to foo

            $hh = str_replace("extern","",$h);
            if(!in_array($hh, $liste, true))
	        {
  	           if(strstr($hh,")")){
                    if(!in_array($hh,$keywords))
                         array_push($liste, trim($hh));
               }
	    	   else
	    	   {
	       		do{
	    			$lh++;
	    			$h = trim($hdr[$lh]);
	    		        $h = str_replace("extern","",$h);
	    			$hh.=$h;
	    		}while(!strstr($h,");"));
                  $ht = trim($hh);
                  if(!in_array($ht,$keywords))
                      array_push($liste, trim($hh));
	       	  }
            }
        }
    }
}

//print_r($liste);

//die();

echo "#ifndef LIBR_RESOLVER_H\n";
echo "#define LIBR_RESOLVER_H\n";
echo "// Copyright Marius C. https://github/comarius (do not remove)\n";
echo "#include <stdio.h>\n";
echo "#include <errno.h>\n";
echo "#include <dlfcn.h>\n\n\n";

echo "// -------------- constants ---------------\n";
echo "// review before compile...\n";
$tk=strtok($consts,",");
do{
    echo $tk."\n";
}while($tk=strtok(","));
echo "\n\n";

echo "// -------------- structs ---------------\n";
echo $structs;

echo "// -------------- functions ---------------\n";
echo "// review before compile...\n";
$X=0;
foreach($liste as $l)
{
    echo "\n//" .$l . "\n";

    $l = str_replace("\t"," ",$l);
    //$l = str_replace(" ","",$l);
    $l = str_replace(", ",",",$l);
    $fp  = explode("(", $l);
    $left = trim($fp[0]);
    $f   = preg_split('/\s+/', $left);
    $right = trim($fp[1]);

    $pars   = explode(",", $right);
    $line  = "#define PF_";

     if(count($f) > 0 && count($f) != 3)
     {
         while($f[1][0]=='*')
            $f[1]=substr($f[1],1);
         if(!in_array($f[1],$keywords)){
            $funcs .= $f[1] .",";
            $line .= $f[1] ." (* (". $f[0] ." (*)(";
        }
     }
     else if(count($f) > 1)
     {
        while($f[2][0]=='*')
            $f[2]=substr($f[2],1);
        if(!in_array($f[2],$keywords)){
             $funcs .= $f[2] .",";
             $line .= $f[2] ." (* (". $f[0]." ".$f[1] ." (*)(";
        }
     }

     $ips= 0;

    foreach($pars as $p)
    {
        $pp = ltrim($p);
        $pp = rtrim($pp);
        $pp = str_replace(")","",$pp);
        $pp = str_replace(";","",$pp);

        $ppp = preg_split('/\s+/', $pp);
        if($ips) $line .= ",";
        foreach($ppp as $pt)
		    $line .= $pt . " ";
	    ++$ips;
    }
    $line .=")) ";

    $line = str_pad($line,80," " );
    $line .= "   _ptr[".$X."].ptr)\n";

    $X++;
    echo $line;
}



echo "\n\n// -------------- all funcs array ---------------\n";

echo "struct FUNCS_ {\n";
echo "  const char *name;\n";
echo "  void  (*ptr)(void);\n";
echo "};\n\n";

echo "inline const FUNCS_* load()\n";
echo "{\n";
echo "    union\n";
echo "    {\n";
echo "    void *p;\n";
echo "    void (*fp)(void);\n";
echo "    } u;\n\n";
echo "    static struct FUNCS_ _funcs[] ={\n";
$tk=strtok($funcs,",");
do{
    echo "        {\"".$tk."\", nullptr},\n";
}while($tk=strtok(","));
echo "        {nullptr, nullptr}\n    };\n";

echo "    void  *dll_handle;\n";
echo "    if ((dll_handle = dlopen(\"{$lib }\", RTLD_LAZY)) == 0)\n";
echo "    {\n";
echo "        perror(\"cannot load:\");\n";
echo "        return nullptr;\n";
echo "    }\n";
echo "    struct FUNCS_ *fp = _funcs;\n";
echo "    for (; fp->name != nullptr; fp++)\n";
echo "    {\n";
echo "        u.p = dlsym(dll_handle, fp->name);\n";
echo "        if (u.fp == 0)\n";
echo "        {\n";
echo "            perror(\"cannot load:\");\n";
echo "        }\n";
echo "        else\n";
echo "        {\n";
echo "            fp->ptr = u.fp;\n";
echo "        }\n";
echo "    }\n";
echo "    return _funcs;\n";
echo "}\r\n#endif // LIB_RESOLVER\r\n";

echo "/*\n ";
echo "add this to cpp \n";
echo "const FUNCS_* _ptr; // global var\n ";
echo " _ptr = load(); // in main() \n";
echo "*/\n ";

echo "//----------------------------------------------------------------------\n";
$tk=strtok($funcs,",");
echo "/*\n";
do{
    echo "  Sqrat::RootTable(v).Func(\"{$tk}\",PF_{$tk});\n";
}while($tk=strtok(","));
echo "*/\n";



?>

