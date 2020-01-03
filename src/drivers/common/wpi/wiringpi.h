#ifndef WIRINGPI_H
#define WIRINGPI_H

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>


// Mask for the bottom 64 pins which belong to the Raspberry Pi
//	The others are available for the other devices

#define	PI_GPIO_MASK	(0xFFFFFFC0)

// Handy defines

// wiringPi modes

#define	WPI_MODE_PINS		 0
#define	WPI_MODE_GPIO		 1
#define	WPI_MODE_GPIO_SYS	 2
#define	WPI_MODE_PHYS		 3
#define	WPI_MODE_PIFACE		 4
#define	WPI_MODE_UNINITIALISED	-1

// Pin modes

#define	INPUT			 0
#define	OUTPUT			 1
#define	PWM_OUTPUT		 2
#define	GPIO_CLOCK		 3
#define	SOFT_PWM_OUTPUT		 4
#define	SOFT_TONE_OUTPUT	 5
#define	PWM_TONE_OUTPUT		 6

#define	LOW			 0
#define	HIGH			 1

// Pull up/down/none

#define	PUD_OFF			 0
#define	PUD_DOWN		 1
#define	PUD_UP			 2

// PWM

#define	PWM_MODE_MS		0
#define	PWM_MODE_BAL		1

// Interrupt levels

#define	INT_EDGE_SETUP		0
#define	INT_EDGE_FALLING	1
#define	INT_EDGE_RISING		2
#define	INT_EDGE_BOTH		3

// Pi model types and version numbers
//	Intended for the GPIO program Use at your own risk.

#define	PI_MODEL_A		 0
#define	PI_MODEL_B		 1
#define	PI_MODEL_AP		 2
#define	PI_MODEL_BP		 3
#define	PI_MODEL_2		 4
#define	PI_ALPHA		 5
#define	PI_MODEL_CM		 6
#define	PI_MODEL_07		 7
#define	PI_MODEL_3B		 8
#define	PI_MODEL_ZERO		 9
#define	PI_MODEL_CM3		10
#define	PI_MODEL_ZERO_W		12
#define	PI_MODEL_3BP 		13
#define	PI_MODEL_3AP 		14
#define	PI_MODEL_CM3P 		16

#define	PI_VERSION_1		0
#define	PI_VERSION_1_1		1
#define	PI_VERSION_1_2		2
#define	PI_VERSION_2		3

#define	PI_MAKER_SONY		0
#define	PI_MAKER_EGOMAN		1
#define	PI_MAKER_EMBEST		2
#define	PI_MAKER_UNKNOWN	3


/////////////////////////////////////////////////////////////////////////////////
struct wpi_func
{
    const char *name;// function name
    void  (*ptr)(void);// Function pointer
};

extern struct wpi_func wpi_foos[];

#define wiringPiVersion  (*(void (*)(int*, int*)) wpi_foos[0].ptr);
#define wiringPiSetup  (*(int (*)(void)) wpi_foos[1].ptr);
#define wiringPiSetupSys  (*(int (*)(void)) wpi_foos[2].ptr);
#define wiringPiSetupGpio  (*(int (*)(void)) wpi_foos[3].ptr);
#define wiringPiSetupPhys  (*(int (*)(void)) wpi_foos[4].ptr);
#define pinModeAlt  (*(void (*)(int , int)) wpi_foos[5].ptr);

#define pinMode  (*(void (*)(int , int)) wpi_foos[6].ptr);
#define pullUpDnControl  (*(void (*)(int , int)) wpi_foos[7].ptr);
#define digitalRead  (*(int (*)(int)) wpi_foos[8].ptr);
#define digitalWrite  (*(void (*)(int , int)) wpi_foos[9].ptr);
#define digitalRead8  (*(unsigned int (*)(int)) wpi_foos[10].ptr);
#define digitalWrite8  (*(void (*)(int , int)) wpi_foos[11].ptr);
#define pwmWrite  (*(void (*)(int , int)) wpi_foos[12].ptr);
#define analogRead  (*(int (*)(int)) wpi_foos[13].ptr);
#define analogWrite  (*(void (*)(int , int)) wpi_foos[14].ptr);
#define wiringPiSetupPiFace  (*(int (*)(void))) wpi_foos[15].ptr);
#define wiringPiSetupPiFaceForGpioProg (*(int (*)(void))) wpi_foos[16].ptr);
#define wiringPiISR (*(int (*)(int,void (*function)(void))) wpi_foos[17].ptr);
#define delay (*(void (*)(unsigned int) wpi_foos[18].ptr);
#define delayMicroseconds (*(void (*)(unsigned int) wpi_foos[19].ptr);
#define millis (*(int (*)(void) wpi_foos[20].ptr);
#define micros (*(int (*)(void) wpi_foos[21].ptr);


class wiringpi
{
    static const char*_wpilib;
    static void*_handle;
public:

    wiringpi()
    {
    }
    virtual ~wiringpi()
    {
    }

    static bool load_library();
    static void unload_library();
protected:

    int wiringPiSetup ();
    int wiringPiSetupGpio ();
    int wiringPiSetupPhys ();
    int wiringPiSetupSys ();
    unsigned int millis ();
    unsigned int micros ();
    void delay (unsigned int howLong);
    void delayMicroseconds (unsigned int howLong);
    int wiringPiISR (int pin, int edgeType,  void (*function)()) ;
    bool pinMode (int pin, int mode);
    bool pullUpDnControl (int pin, int pud);
    bool digitalWrite (int pin, int value);
    int  pwmWrite (int pin, int value);
    int  digitalRead (int pin);
    int  analogRead (int pin);
    bool analogWrite (int pin, int value);
};


#endif // WIRINGPI_H
