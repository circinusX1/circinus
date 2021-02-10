Circinusscriptingengine
for
GPIOADCPWMI2CSPIUART&USB

Link:https://github.com/circinusX1/circinus

SquirrelscriptcontextontopofC++givingincrediblespeedand

extensibility.


Whynobonescriptofpythonscripts

EverytimeIgotnearnpm/nodejsorpipIraninneverending
dependenciesandmessycompromises.Iwantedtohavesomethingclean,toknowwhereeverycomponentresides,likeIhavethemforC/C++,includesandlibrarieseasytolocateeasytomaintainandmanage.


Integration

Itcanbeintegratedsidebysidewithawebserver.Fromthewebserveryoucanaccess'circinus'RESTserverinandoutand
wrapnicehtmlcodetocontrolandsupervisetheengine.
IthasanondemandDBinsqliteandalogeventsin/var/log/rembix/*




BlockDiagram


Thescriptlanguagesyntaxis100%isSquirrelScriptscript.

Itlookslike:(BlinkingaLED)



::using(eGPIO);


LED:=GPIO(26,DIR_OUT,LOW,"led");

varonoff=1;


functionmain(ctx)

{

returnrun(loop,1000);

}


functionloop(ctx,dev)

{

LED.set_value(onoff);

onoff=!onoff;

returntrue;

}




SDK
TYPES

string-alphanumericstringbetweendoublequotes
integer-integernumber
float-arealnumber
bool-atrueorfalsevalue
array-anarrayofelements,ofmaxim4Kelements
blob-ablobofdata


Constants

andglobalfunctions(definedinapis.cppfromline642&676)


void::using(flags|...);LoadsintoVMcontextwhatweneed)

where'flags'are:
eGPIO-loadsintoVMtheGPIOcontroller

ePWMM-loadsintoVMthePWM

eI2C-loadsintoVMtheI2C

eSPI-loadsintoVMtheSPI

eADC-loadsintoVMtheADC

eTIMER-loadsintoVMtheTIMER

eUART-loadsintoVMtheUART


eBASE-loadsintoVMthebasescriptclasstobeextended
inscript

eSRV-loadsintoVMtheRESTFULLTCPserver

eDB-loadsintoVMtheSQLITEinterface

eFILE-loadsintoVMtheFILEaccess

eSOLIB-loadsintoVMtheloaderformodulesandsolibraries

eSOCKET-loadsintoVMtherawclientsocket

eINPUT-loadsintoVMtheinputclass'SYSIN'toaccess

keyboardmouse&touch

eSUNRS-loadsintoVMthetimezonesunriseandsunsetAPI's
eCURL-ifiscompiledwith-DWITH_CURLloadstheCURL

classAPI's
eSSH-ifiscompiledwith-DWITH_SSHloadstheSSH

classAPI's
eUSB-ifiscompiledwith-DWITH_USBloadstheUSB

classAPI's







Functions

stringexecute(stringcommand);executesalinuxcommandand
returnstheresultasstring



bool::loadmod(stringlib,fullpathtomodule.so
stringdevname);theobjectnamegivenfor

themodule.
Havealookatbme280module.



Example:Toreadtemperatureusingbme280
adafruit
Createthei2cdevicefirstgiveita
nameinlastparameter
Createthepluginwhichisusingthe
i2candpassthenname

Thecolorsshowtherelation
betweenthebinding


:loadmod("modules/libbme280-Linux-armv7l.so","THP");//loadmod,andexportsTHPclass

i2c:=I2C(true,"/dev/i2c-2",0x77,"i2c");//builtinI2Cinstanceasi2c


thp:=THP("i2c","bmp280");//thepluginusestheI2Ctogetdata
vara=thp.temperature();




ctx_objectcontext();returnstheglobalengine
contextinstance.

seeCTXclassforit;smethods



stringi2a(integer);conversanintegertoastring



stringx2a(integer);anintegertohexstringrepresentation

x2a(16)->returns"F"

intuptime();returnsuptimeoftheengineinseconds



slog(string);logsastringasinformationto
/var/log/rembix/*.log



quit();breakstheengineflowandreloadsthe

scriptand
start'sitagain



intenable_wd(seconds);enablesthe/dev/watchodwatchdog.Youhave

tocallbark()functioninwithinthetimeout.

Checkyourwatch
dogformaximumtimeinsecondsthat

canbeset

returns0forsuccess,-1forerror.



stringget_strdate()/get_strtime()returnsthedateandtimeofthesystem

asstring



arrayget_date()returnsanarrayof6elementswith
year,month,date,
hour,minute,secondofthesystemtime



boolarrarr(array1,array2)compares2arrayofBYTES(0,255).
arraymaximumlengthis4k.Willskip

thevalues>0xFF



stringintarr2a(array,stringsep);convertsanarrayofnumericalvalues

onlyinto
astring.Eachelementinthestringis

separatedby
stringsep.



arrayarrcpyn(arraya,offset,arrayb);

copiesarraybintoarrayastarting

atoffsetoffset.

Iftheresultingarrayisgreater

then'a'length
willextendtheatothenewsize.



stringconsolein()Ifranfromconsoleandnotasaservice
prompts'>'andwaittheuserto

enterinput.

returnstheinput



arraya2arr(string)convertsahexstringas"CB2AC803"

whereeachoctet
takes2charactersalluppercaseto

thearray
withalltheoctetsconverted

tointegers.



sleep(ms)/suspend(ms)sleepsformsmilliseconds





stringi2xa(int)returnsa0xHEXstringrepresentation

oftheint.



boolis_file(path_to_file)returnstrueifthefileexists





intwd_pull();notifiesthewatchdog.returns0forsuccess



intrun(function,interval);runsthefunctioncallingitat

intervalinmilliseconds

returns0frsuccess,-1forerror.



exitapp()terminatestheappin2to3seconds.



println(string)printstoconsolethestring.Like:

println("thevalueis"+numeric_value+"ok!");



errorln(string)printdtostderrorinredthestring.











Castingnumericfunctions




S8(number)/S16(number)S32(number)converts(strips)thenumber

to8bits,16or32
assigned.Retainsthelowerbytes.



U8(number)/U16(number)U32(number)converts(strips)thenumber

to8bits,16or32
asunsigned.Retainsthelowerbytes.


BuiltinClasses

Peripherals

GPIO

Constructors

varled=GPIO(26,GPIOpin
DIR_OUT,direction
LOW,initialstate
"led");RESTJSONname,orreferenceto

getitbackfromCTX



varbutton=GPIO(19,pingpioNumber
DIR_IN,direction
HIGH,defaultstate,wouldnotifyonLOW
"led2")RESTJSONname,orreferenceto

getitbackfromCTX

varcounter=GPIO(13,gpionumber
-maxfreq,frequencyforintervaltoreadtheLOW/HIGHtransitions
"counter")atleast2.5timeshigherthanwhatwecount.
Canmeasureupto400HzGPIOinputs

varbuzzer=GPIO(6,Generatesatoneoffrequency.Max800Hz
frequency,
"tone")



*Forhigherfrequenciesupto100KhzfrequenciesuseaPWMpin

Methods



intGPIO::set_value(intval)setsgpioto1or0



intGPIO::get_value()returnsthegpiovalue



intGPIO::get_freq()forcounterreturnsthefrequency



intGPIO::set_freq(intfreq)forbuzzerchangesthefrequency



intGPIO::callback(function,risefall-1/1)installsacallback

function=>boolprototype(device)



intGPIO::set_invert(bool)true,gpioisinverted,false,

backtonormal
set_value/get_valuewouldreturn

thecomplement
value



intGPIO::set_toggle()toggglesthestate


intGPIO::set_name(name)changesthealiasnameofthedevice

intGPIO::get_name()returnsthename





PWM

Constructors

PWM(stringpath-to-pwm-sys,aka:/sys/class/pwm/pwmchip0/pwm1or/dev/pwm/pwm0

intfrequency,100maxforthepwmoftheboard

intduty_cycle%,initialdutycyclein%(0-100).if>100would
modulo-itto101

boolinvert,invertthedutycyclewhenwrittenorread

stringname)nameforreference,lookupandJSON



PWM(this,Customc-tor,whenderivedinscriptpass

thederivedinstance

stringpath-to-pwm-sys,sameasabove

intfrequency,pwmfrequency

intinitial_duty,

boolinvert,

stringname)







Example:forasecondconstructor.AcustomPWMthatstarts
amotorin2steps.Holdsit10secondsto50%

whenstartsorstops



classCustomPWMextendsPWM
{
_mymember1=0;//initializeeveyvariabletobindthentoinstance

_timer=0;

_state=false;


////amustmethod

constructor()
{
base.constructor(this,"/dev/pwm/pwm0",10000,0,false,"motor");

_mymember1=0;//heretoootherwiseisper

//classnotperinstance

_timer=uptime();

}



/*

somelogic

*/



functionstart(){

base.set_duty(50);

_timer=uptime();

_state=1;

}


functionstop(){

base.set_duty(50);

_timer=uptime();

_state=0;

}



functioncheck(){

if(_state!=-1)

{

if(uptime()-_timer>10)

{

if(_state==1){

base.set_duty(100);

}else{

base.set_duty(0);

}

_state=-1;//fullstoporstart

}

}



}

}

Andthemain


mypwm:=CustomPWM();

/*

or

varmypwmCustomPWM();

or

mypwm<-CustomPWM();

*/



functionmain(ctx)

{

mypwm.start();

returnrun(loop,1000);

}


functionloop(c,d)

{

mypwm.check();

returntrue;

}

Methods

PWM.::set_value(duty);setsdutyfrom0-100.Ifthevalueis>100willbe

module-edto101

PWM.::get_value();returnsthedutyin%

PWM.::call_back(function);installsacallbackthatiscalledwhenthepwm

valueischanged

function=>boolprototype(device)

PWM.::set_invert(bool);true,invertsthelogicsoaset_value(0)->PWM100%

falseput'sthelogictonormal

PWM.::get_name();returnsit'sname


ADC

Constructors

ADC(sysfile,pathtosysfileaka:
/sys/bus/iio/devices/iio:device0/in_voltage0_raw
(kernel4.4.155-ti-r155)
name)referencenamestring


ADC(SqObj,whenderived,Seethepwmexample.only

thefirsparameterisalwaysthis
sysfile,
name)



Methods

ADC::get_value();returnstherawvalueoftheAnalogueinput

ADC::call_back(function);installsawatchcallback.Thecallbackiscalled

whenthevaluechanges.



Example:



varmyadc=ADC("/sys/bus/iio/devices/iio:device0/in_voltage0_raw","batt");



functionmain(){

myadc.call_back(opps);

returnrun(looop,-1);//loopalwaysiscalledwhenadevicevaluechanges
//regardlesstheperiod(-1nevercalledperiodically)

}



functionloop(ctx,dev)

{

println("device"+dev.name()+"valuechange");

//youcanalsoreaditinmainloop

/*

vardev=ctx.get_dev("batt");

println("inloopvalueis:"+dev.get_value());

*/

returntrue;//truekeeptheprogramalive,false,endstheloop

//andrunreturnsandprogramexits

}



functionoops(dev)

{

println("inoppsvalueis:"+dev.get_value());

returntruue;//allwaysbutignoredinengine

}





SPI

Constructors

SPI(string,like/dev/spi/0:0
int,address

int,modeN/A

int,wordcount

int,frequency

string);

SPI(SqObj,string,int,int,int,int,string);



Methods

SPI::open()openstheSPIdevice

SPI::close()closesthedevice

SPI::call_back(function)installsthecallbacktomonitor

changes

SPI::write(array)writesthearrayofbytesusingfile

writetospi

devicefile.

arraySPI::read(bytes)readsbytesfromdevicefileusing

readofmaximum

bytes.returnsanarrayofbytes

arraySPI::ioctl(intIO_OP,ioctlintegervalue.
array,datasenttodeviceduringioctlas

arrayofbytes
intexpect)expectingbytesback

returnsthearraywithdatabeing

retrievedbyioctl

arraySPI::ioread(bytes)readsthedeviceusingioctl,maxbytes.
returnsdataread

intSPI::iowrite(array)writestodevicethearray,returns

byteswritten


I2C

Constructors

I2C(EI2CBUS,i2cbus012andsoon

intaddr,addressofthedeviceaka:0x11

stringname);JSON/RESTname



I2C(SqObj,

EI2CBUS,i2cbus012andsoon

intaddr,addressofthedeviceaka:0x11

stringname);JSON/RESTname



I2C(true,N/A

string,aka:/dev/i2c-1

intaddr,addressofthedeviceaka:0x11

stringname);JSON/RESTname



I2C(true,N/A

SqObj,

string,aka:/dev/i2c-1

intaddr,addressofthedeviceaka:0x11

stringname);JSON/RESTname



Methods


I2C::call_back(function)installsacallback.Calledwhenvaluechanges



I2C::open()opensdevice,ruenrntrueifsuccessful



I2C::close()closesdevice



I2C::autoopen(bool)true,youdon'thavetocallopen/close

eachwr/rd/ioctlwouldopen-read-io-write-then-close

false,incaseswhenthedevicehastobekeptopenandnotclosed

betweenrd/wr/ioctloperations

intI2C::write(array)writestodevicefiledescriptorasinafile



arrayI2C::read(bytes)readsasisreadingfromafilenotadevice
uptomaxbytes



arrayI2C::ioctl(ioflag,array,expect)performsadeviceioctl(flag,&array,output)

comparesthereadbyesinoutputwithexpecting

bytesandreturnsthearrayofbytes



boolI2C::setreg(intreg)setscurrentregisteraddresstoreg



boolI2C::writecmd(cmd)writescmdcommand,usuallyataddress0



arrayI2C::ioread(register,bytes)readfromregisteruptomaxbytes.

returnsthedata

intI2C::iowrite(register,array)writestoregisterthearrayofbytes.

returnwritten

bytes




ExampleOLED96


::using(eI2C|eGPIO|ePWMM|eSRV|eDB);

::include("modules/_oled96.inc");



print("Iferroris:Badfiledescriptor.Runassudo\n");

dbs:=DB(10000,1000,"./database");

json:=SRV(8000,"rest");

locals=Oled96("/dev/i2c-2",0x3c,"OLED96");



functionmain(a)

{

vark;

s.autoopen(false);//writereadwouldnotopen

if(s.xopen("128x64",0,0))

{

s.clear_display(0);

k=run(kk,1000);

}

s.xclose();

returnrun(kk,50);

}


varK=100;


functionkk(ctx,dev)

{

s.write_string(0,0,get_strtime()+":"+K,2);

K--;

returnK>0?true:false;

}



UART

Constructors


UART(E_TYPE,eSTRING,eBINARY

string,/dev/ttyUSB0forexample

int,BPSas19200

string,"8,n,1"like8bits,noparity1stop

string);JSONname



UART(SqObj,derivedclassinstance,always'this'
E_TYPE,asabove

string,

int,string,

string);



Methods


intUART::setcr(array)Setstheendoflinecharacterusedwhen
putsiscalled.aka:setcr([0xD]);



intUART::puts(string)appendstheendofstringandsendsasstring


intUART::puts_cb(string,function)




stringUART::gets(max_characters)readsuptomaxcharacters.returnswhatwasread


arrayUART::read(max_bytes)readsmaxbytes.returnarrayofbytes


intUART::write(array)


stringUART::expect_any(array,timeoutms)ifthearraysequenceisfoundinreceiveddata

returnsthestringthatcontainsthearray


stringUART::received()returnsleftovers


boolUART::write_cb(array,function)writesarraythencallsthecbasinputs_cb

butwitharrayofbytes


boolUART::expect_str(string,inttimeouts)waitsforthesequence


boolUART::expect_bin(array,inttimeouts)waitsforthearray


stringUART::pick_str();instantreadastring


arrayUART::pick_bin();instantreadbinary


voidUART::flush()clearspendingi/o



USB

Constructors

Methods

BASE

Constructors

Methods



*
CTX
thisclassisasingletonandtheinstanceofitispassedinloop
orreturnedfrom::context()globalAPI


instanceCTX::get_dev(stringdevname);returnsthedevicebyname

timerid=CTX::set_timer(callback_foo,foo(CTX,timer_id);

timout_ms,10...1000000,0toremovethetimer

user_timer_id);uniqueidpassedincb.Auniquenumber.


timerid=CTX::set_cron("cronsyntax",callback,id);TODO


Systemclasses

FILE
