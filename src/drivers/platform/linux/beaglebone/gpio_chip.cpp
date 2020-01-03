#ifdef beaglebone
#include "gpio_chip.h"
#include "logs.h"

int gpio_chip::_initialized = 0;
uint32_t* gpio_chip::_mbase;

gpio_chip::gpio_chip(uint8_t debug)
{
}

gpio_chip::~gpio_chip()
{
}

bool gpio_chip::open_lib()
{
	int fd = ::open("/dev/mem", O_RDWR);
	if(fd == -1) {
		LOGE("Unable to open /dev/mem");
		return false;
	}
	_mbase = (uint32_t*)::mmap(NULL, MMAP_SIZE,
							   PROT_READ | PROT_WRITE, MAP_SHARED,
							   fd, MMAP_OFFSET);
	if(_mbase == MAP_FAILED)
	{

		close(fd);
		perror("Unable to _mbase /dev/mem");
		return false;
	}
	_mbase[(CM_WKUP_ADC_TSC_CLKCTRL-MMAP_OFFSET)/4] |= CM_WKUP_MODULEMODE_ENABLE;

	// wait for the enable to complete
	while(!(_mbase[(CM_WKUP_ADC_TSC_CLKCTRL-MMAP_OFFSET)/4] &
			CM_WKUP_MODULEMODE_ENABLE)) {
		::usleep(1000);
		// waiting for adc clock module to initialize
		//printf("Waiting for CM_WKUP_ADC_TSC_CLKCTRL to enable with MODULEMODE_ENABLE\n");
	}
	// software reset, set bit 1 of sysconfig high?
	// make sure STEPCONFIG write protect is off
	_mbase[(ADC_CTRL-MMAP_OFFSET)/4] |= ADC_STEPCONFIG_WRITE_PROTECT_OFF;

	// set up each ADCSTEPCONFIG for each ain pin
	_mbase[(ADCSTEPCONFIG1-MMAP_OFFSET)/4] = 0x00<<19 | ADC_AVG16;
	_mbase[(ADCSTEPDELAY1-MMAP_OFFSET)/4]  = (0x0F)<<24;
	_mbase[(ADCSTEPCONFIG2-MMAP_OFFSET)/4] = 0x01<<19 | ADC_AVG16;
	_mbase[(ADCSTEPDELAY2-MMAP_OFFSET)/4]  = (0x0F)<<24;
	_mbase[(ADCSTEPCONFIG3-MMAP_OFFSET)/4] = 0x02<<19 | ADC_AVG16;
	_mbase[(ADCSTEPDELAY3-MMAP_OFFSET)/4]  = (0x0F)<<24;
	_mbase[(ADCSTEPCONFIG4-MMAP_OFFSET)/4] = 0x03<<19 | ADC_AVG16;
	_mbase[(ADCSTEPDELAY4-MMAP_OFFSET)/4]  = (0x0F)<<24;
	_mbase[(ADCSTEPCONFIG5-MMAP_OFFSET)/4] = 0x04<<19 | ADC_AVG16;
	_mbase[(ADCSTEPDELAY5-MMAP_OFFSET)/4]  = (0x0F)<<24;
	_mbase[(ADCSTEPCONFIG6-MMAP_OFFSET)/4] = 0x05<<19 | ADC_AVG16;
	_mbase[(ADCSTEPDELAY6-MMAP_OFFSET)/4]  = (0x0F)<<24;
	_mbase[(ADCSTEPCONFIG7-MMAP_OFFSET)/4] = 0x06<<19 | ADC_AVG16;
	_mbase[(ADCSTEPDELAY7-MMAP_OFFSET)/4]  = (0x0F)<<24;
	_mbase[(ADCSTEPCONFIG8-MMAP_OFFSET)/4] = 0x07<<19 | ADC_AVG16;
	_mbase[(ADCSTEPDELAY8-MMAP_OFFSET)/4]  = (0x0F)<<24;

	// enable the ADC
	_mbase[(ADC_CTRL-MMAP_OFFSET)/4] |= 0x01;
	return false;
}

void gpio_chip::close_lib()
{
	if(_mbase != MAP_FAILED)
		::munmap(_mbase, MMAP_SIZE);
}


bool gpio_chip::_pinMode(const PIN& pin, unsigned char direction,
						 unsigned char mux, unsigned char pull)
{
	int pin_data = 0;
	FILE *fp = NULL;
	char muxfile[64];

	pin_data |=  mux; // set the mux mode
	// set up the pull up/down resistors
	if(pull == DISABLED) pin_data |= 1 << 3;
	if(pull == PULLUP)   pin_data |= 1 << 4;
	pin_data |= direction << 5; // set up the pin direction
	// write the pin_data
	sprintf(muxfile, "%s/%s", CONFIG_MUX_PATH, pin.mux);
	// open the file
	if((fp = fopen(muxfile, "w")) == NULL) {
		LOGE("Cannot set pin mode");
		return false;
	}
	fprintf(fp, "%x", pin_data);
	fclose(fp);

	return true;
}

bool gpio_chip::_digitalWrite(const PIN& p, uint8_t mode)
{
	_mbase[(p.gpio_bank-MMAP_OFFSET+GPIO_OE)/4] &= ~(1<<p.bank_id);
	if(mode == 1)
		_mbase[(p.gpio_bank-MMAP_OFFSET+GPIO_DATAOUT)/4] |= 1<<p.bank_id;
	else
		_mbase[(p.gpio_bank-MMAP_OFFSET+GPIO_DATAOUT)/4] &= ~(1<<p.bank_id);

	return true;
}

int gpio_chip::_digitalRead(const PIN& p)
{
	return (_mbase[(p.gpio_bank-MMAP_OFFSET+GPIO_DATAIN)/4] &
			(1<<p.bank_id))>>p.bank_id;
}

int gpio_chip::_analogRead(const PIN& p)
{
	_mbase[(ADC_STEPENABLE-MMAP_OFFSET)/4] |= (0x01<<(p.bank_id+1));

	// return the the FIFO0 data register
	return _mbase[(ADC_FIFO0DATA-MMAP_OFFSET)/4] & ADC_FIFO_MASK;
}

/**
 * @brief resolvepin
 * @param pin  901  P9.1
 * @return
 */
const PIN* gpio_chip::_get_pin(int pin)
{
	char spin[8];

	PIN	pins[]={
		USR0 ,USR1 ,USR2 ,USR3 ,P8_3 ,P8_4 ,P8_5 ,P8_6 ,P8_7 ,
		P8_8 ,P8_9 ,P8_10 ,P8_11 ,P8_12 ,P8_13 ,P8_14 ,P8_15 ,
		P8_16 ,P8_17 ,P8_18 ,P8_19 ,P8_20 ,P8_21 ,P8_22 ,P8_23 ,
		P8_24 ,P8_25 ,P8_26 ,P8_27 ,P8_28 ,P8_29 ,P8_30 ,P8_31 ,
		P8_32 ,P8_33 ,P8_34 ,P8_35 ,P8_36 ,P8_37 ,P8_38 ,P8_39 ,
		P8_40 ,P8_41 ,P8_42 ,P8_43 ,P8_44 ,P8_45 ,P8_46 ,P9_11 ,
		P9_12 ,P9_13 ,P9_14 ,P9_15 ,P9_16 ,P9_17 ,P9_18 ,P9_19 ,
		P9_20 ,P9_21 ,P9_22 ,P9_23 ,P9_24 ,P9_25 ,P9_26 ,P9_27 ,
		P9_28 ,P9_29 ,P9_30 ,P9_31 ,P9_33 ,P9_35 ,P9_36 ,P9_37 ,
		P9_38 ,P9_39 ,P9_40 ,P9_41 ,P9_42, P9_50 };

	if(pin>=900)
		::sprintf(spin, "P%d_%d", pin/100, pin-900);
	else
		::sprintf(spin, "P%d_%d", pin/100, pin-800);
	PIN* p = pins;
	for(;p->name;p++)
	{
		if(!::strcmp(spin,p->pname))
			return p;
	}
	return nullptr;
};
#endif //BEAGLEBONE
