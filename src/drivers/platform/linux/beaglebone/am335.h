/**
 * @file am335x.h
 * @author Ethan Hayon
 *
 * This file contains important config for TI am335x ARM Cortex-A8 SoC
 *
 * PLEASE DO NOT MODIFY
 *
 * Licensed under the MIT License (MIT)
 * See MIT-LICENSE file for more information
 */

#ifndef _AM335X_H_
#define _AM335X_H_

#include <stdint.h>

#define MMAP_OFFSET (0x44C00000)
#define MMAP_SIZE   (0x481AEFFF-MMAP_OFFSET)

/* Clock Module Memory Registers */
#define CM_WKUP (0x44E00400)
#define CM_WKUP_ADC_TSC_CLKCTRL (CM_WKUP+0xBC)
#define CM_WKUP_MODULEMODE_ENABLE (0x02)
#define CM_WKUP_IDLEST_DISABLED (0x03<<16)

#define CM_PER (0x44E00000)
#define CM_PER_EPWMSS1_CLKCTRL (CM_PER+0xCC)
#define CM_PER_EPWMSS0_CLKCTRL (CM_PER+0xD4)
#define CM_PER_EPWMSS2_CLKCTRL (CM_PER+0xD8)


/* GPIO Memory Registers */
#define GPIO_REGISTER_SIZE (4)

#define GPIO0 	(0x44E07000)
#define GPIO1		(0x4804C000)
#define GPIO2		(0x481AC000)
#define GPIO3		(0x481AE000)

#define GPIO_CLEARDATAOUT (0x190)
#define GPIO_SETDATAOUT   (0x194)
#define GPIO_OE			      (0x134)
#define GPIO_DATAOUT      (0x13C)
#define GPIO_DATAIN       (0x138)

/* Analog Digital Converter Memory Registers */
#define ADC_TSC (0x44E0D000)

#define ADC_CTRL (ADC_TSC+0x40)
#define ADC_STEPCONFIG_WRITE_PROTECT_OFF (0x01<<2)
#define ADC_STEPENABLE (ADC_TSC+0x54)

#define ADCSTEPCONFIG1 (ADC_TSC+0x64)
#define ADCSTEPDELAY1  (ADC_TSC+0x68)
#define ADCSTEPCONFIG2 (ADC_TSC+0x6C)
#define ADCSTEPDELAY2  (ADC_TSC+0x70)
#define ADCSTEPCONFIG3 (ADC_TSC+0x74)
#define ADCSTEPDELAY3  (ADC_TSC+0x78)
#define ADCSTEPCONFIG4 (ADC_TSC+0x7C)
#define ADCSTEPDELAY4  (ADC_TSC+0x80)
#define ADCSTEPCONFIG5 (ADC_TSC+0x84)
#define ADCSTEPDELAY5  (ADC_TSC+0x88)
#define ADCSTEPCONFIG6 (ADC_TSC+0x8C)
#define ADCSTEPDELAY6  (ADC_TSC+0x90)
#define ADCSTEPCONFIG7 (ADC_TSC+0x94)
#define ADCSTEPDELAY7  (ADC_TSC+0x98)
#define ADCSTEPCONFIG8 (ADC_TSC+0x9C)
#define ADCSTEPDELAY8  (ADC_TSC+0xA0)

#define ADC_AVG0  (0x000)
#define ADC_AVG2  (0x001)
#define ADC_AVG4  (0x010)
#define ADC_AVG8  (0x011)
#define ADC_AVG16 (0x100)

#define ADC_FIFO0DATA (ADC_TSC+0x100)
#define ADC_FIFO_MASK (0xFFF)

typedef struct s_PWM {
  const char muxmode; /*!< mux mode, 0-7, see am335x technical manual */
  const char *name;   /*!< name of pwm pin, i.e.: "EHRPWM2B" */
  const char *path;   /*!< path to the pwm, i.e.: "ehrpwm.2:1" */
} PWM;

#define NO_PWM {0,nullptr,nullptr}

typedef struct s_PIN {
  const char*         pname;
  const char *        name;   /*!< readable name of pin, i.e.: "GPIO1_21", see beaglebone user guide */
  unsigned int        gpio_bank; /*!< which of the four gpio banks is this pin in, i.e.: GPIO1, r 0x4804C000 */
  uint8_t             gpio; /*!< pin number on the am335x processor */
  uint8_t             bank_id; /*!< pin number within each bank, should be 0-31 */
  const char *        mux;    /*!< file name for setting mux */
  uint8_t             eeprom; /*!< position in eeprom */
  unsigned char       pwm_present; /*!< whether or not this pin can be used for PWM */
  PWM                 pwm;      /*!< pwm struct if pwm_present is true */
} PIN;

#define TRUE 1
#define FALSE 0

#define USR0  ((PIN){"USR0",  "GPIO1_21",	   GPIO1,     0,   21,   "",				0,     FALSE, NO_PWM   })
#define USR1  ((PIN){"USR1",  "GPIO1_22",	   GPIO1,     0,   22,   "",				0,     FALSE, NO_PWM   })
#define USR2  ((PIN){"USR2",  "GPIO1_23",	   GPIO1,     0,   23,   "",				0,     FALSE, NO_PWM   })
#define USR3  ((PIN){"USR3",  "GPIO1_24",	   GPIO1,     0,   24,   "",				0,     FALSE, NO_PWM   })
#define P8_3  ((PIN){"P8_3",   "GPIO1_6",	   GPIO1,     38, 	6,	 "gpmc_ad6",        26,    FALSE, NO_PWM   })
#define P8_4  ((PIN){"P8_4",   "GPIO1_7",     GPIO1,     39, 	7,	 "gpmc_ad7",        27,    FALSE, NO_PWM   })
#define P8_5  ((PIN){"P8_5",   "GPIO1_2",     GPIO1,     34, 	2,	 "gpmc_ad2",        22,    FALSE, NO_PWM   })
#define P8_6  ((PIN){"P8_6",   "GPIO1_3",     GPIO1,     35, 	3,	 "gpmc_ad3",        23,    FALSE, NO_PWM   })
#define P8_7  ((PIN){"P8_7",   "TIMER4",      GPIO2,     66, 	2, 	 "gpmc_advn_ale",   41,    FALSE, NO_PWM   })
#define P8_8  ((PIN){"P8_8",   "TIMER7",      GPIO2,     67, 	3,	 "gpmc_oen_ren",    44,    FALSE, NO_PWM   })
#define P8_9  ((PIN){"P8_9",   "TIMER5",      GPIO2,     69, 	5,	 "gpmc_ben0_cle",   42,    FALSE, NO_PWM   })
#define P8_10 ((PIN){"P8_10",   "TIMER6",      GPIO2,     68, 	4,	 "gpmc_wen",        43,    FALSE, NO_PWM   })
#define P8_11 ((PIN){"P8_11",   "GPIO1_13",    GPIO1,     45, 	13,	 "gpmc_ad13",       29,    FALSE, NO_PWM   })
#define P8_12 ((PIN){"P8_12",   "GPIO1_12",    GPIO1,     44, 	12,	 "gpmc_ad12",       28,    FALSE, NO_PWM   })
#define P8_13 ((PIN){"P8_13",   "EHRPWM2B",    GPIO0,     23, 	23,	 "gpmc_ad9",        15,    TRUE,  (PWM){4, "EHRPWM2B", "ehrpwm.2:1"}})
#define P8_14 ((PIN){"P8_14",   "GPIO0_26",    GPIO0,     26, 	26,	 "gpmc_ad10",       16,    FALSE, NO_PWM   })
#define P8_15 ((PIN){"P8_15",   "GPIO1_15",    GPIO1,     47, 	15,	 "gpmc_ad15",       31,    FALSE, NO_PWM   })
#define P8_16 ((PIN){"P8_16",   "GPIO1_14",    GPIO1,     46, 	14,	 "gpmc_ad14",       30,    FALSE, NO_PWM   })
#define P8_17 ((PIN){"P8_17",   "GPIO0_27",    GPIO0,     27, 	27,	 "gpmc_ad11",       17,    FALSE, NO_PWM   })
#define P8_18 ((PIN){"P8_18",   "GPIO2_1",     GPIO2,     65, 	1,	 "gpmc_clk",        40,    FALSE, NO_PWM   })
#define P8_19 ((PIN){"P8_19",   "EHRPWM2A",    GPIO0,     22, 	22,	 "gpmc_ad8",        14,    TRUE,  (PWM){4, "EHRPWM2A", "ehrpwm.2:0"}})
#define P8_20 ((PIN){"P8_20",   "GPIO1_31",    GPIO1,     63, 	31,	 "gpmc_csn2",       39,    FALSE, NO_PWM   })
#define P8_21 ((PIN){"P8_21",   "GPIO1_30",    GPIO1,     62, 	30,	 "gpmc_csn1",       38,    FALSE, NO_PWM   })
#define P8_22 ((PIN){"P8_22",   "GPIO1_5",     GPIO1,     37, 	5,	 "gpmc_ad5",        25,    FALSE, NO_PWM   })
#define P8_23 ((PIN){"P8_23",   "GPIO1_4",     GPIO1,     36, 	4,	 "gpmc_ad4",        24,    FALSE, NO_PWM   })
#define P8_24 ((PIN){"P8_24",   "GPIO1_1",     GPIO1,     33, 	1,	 "gpmc_ad1",        21,    FALSE, NO_PWM   })
#define P8_25 ((PIN){"P8_25",   "GPIO1_0",     GPIO1,     32, 	0,	 "gpmc_ad0",        20,    FALSE, NO_PWM   })
#define P8_26 ((PIN){"P8_26",   "GPIO1_29",    GPIO1,     61, 	29,	 "gpmc_csn0",       37,    FALSE, NO_PWM   })
#define P8_27 ((PIN){"P8_27",   "GPIO2_22",    GPIO2,     86, 	22,	 "lcd_vsync",       57,    FALSE, NO_PWM   })
#define P8_28 ((PIN){"P8_28",   "GPIO2_24",    GPIO2,     88, 	24,	 "lcd_pclk",        59,    FALSE, NO_PWM   })
#define P8_29 ((PIN){"P8_29",   "GPIO2_23",    GPIO2,     87, 	23,	 "lcd_hsync",       58,    FALSE, NO_PWM   })
#define P8_30 ((PIN){"P8_30",   "GPIO2_25",    GPIO2,     89, 	25,	 "lcd_ac_bias_en",  60,    FALSE, NO_PWM   })
#define P8_31 ((PIN){"P8_31",   "UART5_CTSN",  GPIO0,     10, 	10,	 "lcd_data14",       7,    FALSE, NO_PWM   })
#define P8_32 ((PIN){"P8_32",   "UART5_RTSN",  GPIO0,     11, 	11,	 "lcd_data15",       8,    FALSE, NO_PWM   })
#define P8_33 ((PIN){"P8_33",   "UART4_RTSN",  GPIO0,     9,  	9,	 "lcd_data13",       6,    FALSE, NO_PWM   })
#define P8_34 ((PIN){"P8_34",   "UART3_RTSN",  GPIO2,     81, 	17,	 "lcd_data11",      56,    TRUE,  (PWM){2, "EHRPWM1B", "ehrpwm.1:1"}})
#define P8_35 ((PIN){"P8_35",   "UART4_CTSN",  GPIO0,     8,  	8,	 "lcd_data12",       5,    FALSE, NO_PWM   })
#define P8_36 ((PIN){"P8_36",   "UART3_CTSN",  GPIO2,     80, 	16,	 "lcd_data10",      55,    TRUE,  (PWM){2, "EHRPWM1A", "ehrpwm.1:0"}})
#define P8_37 ((PIN){"P8_37",   "UART5_TXD",   GPIO2,     78, 	14,	 "lcd_data8",       53,    FALSE, NO_PWM   })
#define P8_38 ((PIN){"P8_38",   "UART5_RXD",   GPIO2,     79, 	15,	 "lcd_data9",       54,    FALSE, NO_PWM   })
#define P8_39 ((PIN){"P8_39",   "GPIO2_12",    GPIO2,     76, 	12,	 "lcd_data6",       51,    FALSE, NO_PWM   })
#define P8_40 ((PIN){"P8_40",   "GPIO2_13",    GPIO2,     77, 	13,	 "lcd_data7",       52,    FALSE, NO_PWM   })
#define P8_41 ((PIN){"P8_41",   "GPIO2_10",    GPIO2,     74, 	10,	 "lcd_data4",       49,    FALSE, NO_PWM   })
#define P8_42 ((PIN){"P8_42",   "GPIO2_11",    GPIO2,     75, 	11,	 "lcd_data5",       50,    FALSE, NO_PWM   })
#define P8_43 ((PIN){"P8_43",   "GPIO2_8",     GPIO2,     72, 	8,	 "lcd_data2",       47,    FALSE, NO_PWM   })
#define P8_44 ((PIN){"P8_44",   "GPIO2_9",     GPIO2,     73, 	9,	 "lcd_data3",       48,    FALSE, NO_PWM   })
#define P8_45 ((PIN){"P8_45",   "GPIO2_6",     GPIO2,     70, 	6,	 "lcd_data0",       45,    TRUE,  (PWM){3, "EHRPWM2A", "ehrpwm.2:0"}})
#define P8_46 ((PIN){"P8_46",   "GPIO2_7",     GPIO2,     71, 	7,	 "lcd_data1",       46,    TRUE,  (PWM){3, "EHRPWM2B", "ehrpwm.2:1"}})

#define P9_11 ((PIN){"P9_11",   "UART4_RXD",   GPIO0,     30,  30,   "gpmc_wait0",       18,   FALSE, NO_PWM})
#define P9_12 ((PIN){"P9_12",   "GPIO1_28",    GPIO1,     60,  28,   "gpmc_ben1",        36,   FALSE, NO_PWM})
#define P9_13 ((PIN){"P9_13",   "UART4_TXD",   GPIO0,     31,  31,   "gpmc_wpn",         19,   FALSE, NO_PWM})
#define P9_14 ((PIN){"P9_14",   "EHRPWM1A",    GPIO1,     50,  18,   "gpmc_a2",          34,   TRUE, (PWM){6, "EHRPWM1A", "ehrpwm.1:0"}})
#define P9_15 ((PIN){"P9_15",   "GPIO1_16",    GPIO1,     48,  16,   "mii1_rxd3",        32,   FALSE, NO_PWM})
#define P9_16 ((PIN){"P9_16",   "EHRPWM1B",    GPIO1,     51,  19,   "gpmc_a3",          35,   TRUE, (PWM){6, "EHRPWM1B", "ehrpwm.1:1"}})
#define P9_17 ((PIN){"P9_17",   "I2C1_SCL",    GPIO0,      5,   5,   "spi0_cs0",          3,   FALSE, NO_PWM})
#define P9_18 ((PIN){"P9_18",   "I2C1_SDA",    GPIO0,      4,   4,   "spi0_d1",           2,   FALSE, NO_PWM})
#define P9_19 ((PIN){"P9_19",   "I2C2_SCL",    GPIO0,     13,  13,   "uart1_rtsn",        9,   FALSE, NO_PWM})
#define P9_20 ((PIN){"P9_20",   "I2C2_SDA",    GPIO0,     12,  12,   "uart1_ctsn",       10,   FALSE, NO_PWM})
#define P9_21 ((PIN){"P9_21",   "UART2_TXD",   GPIO0,      3,   3,   "spi0_d0",           1,   TRUE, (PWM){3, "EHRPWM0B", "ehrpwm.0:1"}})
#define P9_22 ((PIN){"P9_22",   "UART2_RXD",   GPIO0,      2,   2,   "spi0_sclk",         0,   TRUE, (PWM){3, "EHRPWM0A", "ehrpwm.0:0"}})
#define P9_23 ((PIN){"P9_23",   "GPIO1_17",    GPIO1,     49,  17,   "gpmc_a1",          33,   FALSE, NO_PWM})
#define P9_24 ((PIN){"P9_24",   "UART1_TXD",   GPIO0,     15,  15,   "uart1_txd",        12,   FALSE, NO_PWM})
#define P9_25 ((PIN){"P9_25",   "GPIO3_21",    GPIO3,    117,  21,   "mcasp0_ahclkx",    66,   FALSE, NO_PWM})
#define P9_26 ((PIN){"P9_26",   "UART1_RXD",   GPIO0,     14,  14,   "uart1_rxd",        11,   FALSE, NO_PWM})
#define P9_27 ((PIN){"P9_27",   "GPIO3_19",    GPIO3,    115,  19,   "mcasp0_fsr",       64,   FALSE, NO_PWM})
#define P9_28 ((PIN){"P9_28",   "SPI1_CS0",    GPIO3,    113,  17,   "mcasp0_ahclkr",    63,   TRUE, (PWM){4, "ECAPPWM2", "ecap.2"}})
#define P9_29 ((PIN){"P9_29",   "SPI1_D0",     GPIO3,    111,  15,   "mcasp0_fsx",       61,   TRUE, (PWM){1, "EHRPWM0B", "ehrpwm.0:1"}})
#define P9_30 ((PIN){"P9_30",   "SPI1_D1",     GPIO3,    112,  16,   "mcasp0_axr0",      62,   FALSE, NO_PWM})
#define P9_31 ((PIN){"P9_31",   "SPI1_SCLK",   GPIO3,    110,  14,   "mcasp0_aclkx",     65,   TRUE, (PWM){1, "EHRPWM0A", "ehrpwm.0:0"}})
#define P9_33 ((PIN){"P9_33",   "AIN4",        0,          4,   4,   "",                 71,   FALSE, NO_PWM})
#define P9_35 ((PIN){"P9_35",   "AIN6",        0,          6,   6,   "",                 73,   FALSE, NO_PWM})
#define P9_36 ((PIN){"P9_36",   "AIN5",        0,          5,   5,   "",                 72,   FALSE, NO_PWM})
#define P9_37 ((PIN){"P9_37",   "AIN2",        0,          2,   2,   "",                 69,   FALSE, NO_PWM})
#define P9_38 ((PIN){"P9_38",   "AIN3",        0,          3,   3,   "",                 70,   FALSE, NO_PWM})
#define P9_39 ((PIN){"P9_39",   "AIN0",        0,          0,   0,   "",                 67,   FALSE, NO_PWM})
#define P9_40 ((PIN){"P9_40",   "AIN1",        0,          1,   1,   "",                 68,   FALSE, NO_PWM})
#define P9_41 ((PIN){"P9_41",   "CLKOUT2",     GPIO0,     20,  20,   "xdma_event_intr1", 13,   FALSE, NO_PWM})
#define P9_42 ((PIN){"P9_42",   "GPIO0_7",     GPIO0,      7,   7,   "ecap0_in_pwm0_out", 4,   TRUE, (PWM){0, "ECAPPWM0", "ecap.0"}})
#define P9_50 ((PIN){nullptr,   nullptr,      0 ,          0,   0,   nullptr, 0,   FALSE, NO_PWM})

#define INPUT    ((unsigned char)(1))
#define OUTPUT   ((unsigned char)(0))
#define PULLUP   ((unsigned char)(1))
#define PULLDOWN ((unsigned char)(0))
#define DISABLED ((unsigned char)(2))

#define CONFIG_MUX_PATH "/sys/kernel/debug/omap_mux"



#endif
