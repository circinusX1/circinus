#include "fastaccess.h"

/*
 * fast access
 * */
/*
bool gpio_export(EGPIO_PIN pin);
bool gpio_un_export(EGPIO_PIN pin);
bool gpio_config(EGPIO_PIN pin, EPIN_DIR dir);
bool gpio_set_value(EGPIO_PIN pin, int val);
int  gpio_get_value(EGPIO_PIN pin);

bool pwm_config(const char* pin, int period);
bool pwm_enable(const char* pin);
bool pwm_disable(const char* pin);
bool pwm_set_duty(const char* pin, int percent);
bool pwm_abs_setduty(const char* pin, int duty);

bool adc_get_value(const char* pin, int duty);

bool i2c_write(const char* pin, int addr, int reg, const SQArray& val);
bool i2c_read(const char* pin, int addr, int reg, int bytes);

bool spi_write(const char* pin, int addr, int freq, const SQArray& val);
bool spi_read(const char* pin, int addr, int bytes);

bool uart_puts(const char* pin, const char* val);
const char* uart_gets(const char* pin, int bytes);

bool uart_write(const char* pin, const SQArray& val);
SQArray uart_read(const char* pin, int bytes);

*/
