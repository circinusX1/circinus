
::using(eGPIO|ePWM|eSRV|eI2C);

mainpower		:= PIO(20, DIR_OUT, LOW, "power");
gazvalve		:= PIO(21, DIR_OUT, LOW, "gaz");
flamefan		:= PIO(7,  DIR_OUT, LOW, "exhaust");
spark			:= PIO(8,  DIR_OUT, LOW, "spark");
thermostat		:= I2C(2,  0x55,     "thermoflame");
flamedet		:= I2C(2,  0x57,	 "flameir");
tempin			:= I2C(2,  0x58,	 "temp_in");
tempout		    := I2C(2,  0x59,	 "temp_out");
mainfan			:= PWM("0.0", 0,	 "main_fan");
condair         := PIO(8,  DIR_OUT, LOW, "condair_unit");

thermo_fan		:  PIO(22, DIR_IN, LOW, "t_fan"
thermo_heat		:  PIO(23, DIR_IN, LOW, "t_heat"
thermo_cool		:  PIO(24, DIR_IN, LOW, "t_cool"

SPARK_ON_TOUT	:=	3000;

MODES	<- {
    OFF = 0,
	COOL_ON = 0x1,
	HEAT_ON = 0x2,
	FAN_ON  = 0x8,
}

CUR_MODE := MODES.OFF;

HEATING_DATA <- {
    start_time = 0,
}

function main(ctx)
{
    thermo_fan.set_monitor(true);
	thermo_heat.set_monitor(true);
	thermo_cool.set_monitor(true);
	return run(loop,1000);
}

function loop(ctx,dev)
{
    if(devs)
	{
	    thermostat_event(dev);
	}
	switch(CUR_MODE)
	{
	    case HEAT_ON:
		    _heat_on();
			break;
		case COOL_ON:
		    _cool_on();
			break;
		case FAN_ON:
		    _fan_on();
			break;
		default:
		    break;
	}
}

function thermostat_event(dev)
{
   switch(dev)
    {
	    case thermo_fan:
		    if(dev.get_value()==HIGH)
			{
			    mainfan.set_duty(100);
			}
			else
			{
			    mainfan.set_duty(0);
			}
			break;
		case thermo_heat:
		    if(dev.get_value()==HIGH)
			{
			    _heat_on();
			}
			else
			{
			    _heat_off();
			}
			break;
		case thermo_cool:
		    if(dev.get_value()==HIGH)
			{
			    _cool_on();
			}
			else
			{
			    _cool_off();
			}
			break;
	}
}

function _heat_on()
{
    if(HEATING_DATA.start_time==0)
	{
	    HEATING_DATA.start_time = uptime(); // seconds
		flamefan.set_value(1);
		sleep(3000);
		spark.set_value(1);
		gazvalve.set_value(1);
		sleep(2000);
		var flame = flamedet.see_flame();
		if(flame==false)
		{
		    spark.set_value(0);
			gazvalve.set_value(0);
			ctx().set_timer(turnoff_fan,10000,1);
			CUR_MODE = MODES.OFF;
		}
		else
		{
		    spark.set_value(0);
			mainfan.set_value(1);
			CUR_MODE = MODES.HEAT;
		}
	else
	{
	    var flame = flamedet.see_flame();
		if(flame==false)
		{
		    spark.set_value(0);
			gazvalve.set_value(0);
			ctx().set_timer(turnoff_fan,10000,1);
		}
	}
}


function turnoff_fan()
{
    flamefan.set_value(0);
	return false; // destroy this timer
}
