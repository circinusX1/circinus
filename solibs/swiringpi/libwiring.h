#ifndef LIBR_RESOLVER_H
#define LIBR_RESOLVER_H
// Copyright Marius C. https://github/comarius (do not remove)
#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>


// -------------- constants ---------------
// review before compile...
#define	__WIRING_PI_H__
#define	UNU	__attribute__((unused))
#define	PI_GPIO_MASK	(0xFFFFFFC0)
#define	WPI_MODE_PINS		 0
#define	WPI_MODE_GPIO		 1
#define	WPI_MODE_GPIO_SYS	 2
#define	WPI_MODE_PHYS		 3
#define	WPI_MODE_PIFACE		 4
#define	WPI_MODE_UNINITIALISED	-1
#define	INPUT			 0
#define	OUTPUT			 1
#define	PWM_OUTPUT		 2
#define	GPIO_CLOCK		 3
#define	SOFT_PWM_OUTPUT		 4
#define	SOFT_TONE_OUTPUT	 5
#define	PWM_TONE_OUTPUT		 6
#define	LOW			 0
#define	HIGH			 1
#define	PUD_OFF			 0
#define	PUD_DOWN		 1
#define	PUD_UP			 2
#define	PWM_MODE_MS		0
#define	PWM_MODE_BAL		1
#define	INT_EDGE_SETUP		0
#define	INT_EDGE_FALLING	1
#define	INT_EDGE_RISING		2
#define	INT_EDGE_BOTH		3
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
#define	PI_THREAD(X)	void *X (UNU void *dummy)
#define	WPI_FATAL	(1==1)
#define	WPI_ALMOST	(1==2)


// -------------- functions ---------------
// review before compile...
//         int  waitForInterrupt    (int pin, int mS) ;
#define PF_waitForInterrupt (* (int (*)(int,int))                                  _ptr[0].ptr)
//         unsigned int micros            (void) ;
#define PF_micros (* (unsigned int (*)(void))                                      _ptr[1].ptr)
//         void piUnlock            (int key) ;
#define PF_piUnlock (* (void (*)(int))                                             _ptr[2].ptr)
//         void gpioClockSet        (int pin, int freq) ;
#define PF_gpioClockSet (* (void (*)(int,int))                                     _ptr[3].ptr)
//         struct wiringPiNodeStruct *wiringPiFindNode (int pin) ;
//#define PF_*wiringPiFindNode (* (struct wiringPiNodeStruct (*)(int))               _ptr[4].ptr)
//         void wiringPiVersion	(int *major, int *minor) ;
#define PF_wiringPiVersion (* (void (*)(int,int))                                  _ptr[5].ptr)
//         int piHiPri (const int pri) ;
//#define PF_piHiPri (* (int (*)(const))                                             _ptr[6].ptr)
//         void digitalWriteByte    (int value) ;
#define PF_digitalWriteByte (* (void (*)(int))                                     _ptr[7].ptr)
//         void digitalWriteByte2   (int value) ;
#define PF_digitalWriteByte2 (* (void (*)(int))                                    _ptr[8].ptr)
//         int wiringPiFailure (int fatal, const char *message, ...) ;
//#define PF_wiringPiFailure (* (int (*)(int,const,...))                             _ptr[9].ptr)
//         int  physPinToGpio       (int physPin) ;
#define PF_physPinToGpio (* (int (*)(int))                                         _ptr[10].ptr)
//         unsigned int  digitalReadByte2    (void) ;
#define PF_digitalReadByte2 (* (unsigned int (*)(void))                            _ptr[11].ptr)
//         void         delay             (unsigned int howLong) ;
#define PF_delay (* (void (*)(unsigned))                                           _ptr[12].ptr)
//         void         delayMicroseconds (unsigned int howLong) ;
#define PF_delayMicroseconds (* (void (*)(unsigned))                               _ptr[13].ptr)
//         int  analogRead          (int pin) ;
#define PF_analogRead (* (int (*)(int))                                            _ptr[14].ptr)
//         void digitalWriteByte2   (int value) ;
//         void pinModeAlt          (int pin, int mode) ;
#define PF_pinModeAlt (* (void (*)(int,int))                                       _ptr[15].ptr)
//         void pwmSetRange         (unsigned int range) ;
#define PF_pwmSetRange (* (void (*)(unsigned))                                     _ptr[16].ptr)
//         void pinModeAlt          (int pin, int mode) ;
//         void pinMode             (int pin, int mode) ;
#define PF_pinMode (* (void (*)(int,int))                                          _ptr[17].ptr)
//         void piBoardId           (int *model, int *rev, int *mem, int *maker, int *overVolted) ;
#define PF_piBoardId (* (void (*)(int,int,int,int,int))                            _ptr[18].ptr)
//         int  getAlt              (int pin) ;
#define PF_getAlt (* (int (*)(int))                                                _ptr[19].ptr)
//         struct wiringPiNodeStruct *wiringPiNewNode  (int pinBase, int numPins) ;
//#define PF_*wiringPiNewNode (* (struct wiringPiNodeStruct (*)(int,int))            _ptr[20].ptr)
//         int  wiringPiSetupGpio   (void) ;
#define PF_wiringPiSetupGpio (* (int (*)(void))                                    _ptr[21].ptr)
//         int  piGpioLayout        (void) ;
#define PF_piGpioLayout (* (int (*)(void))                                         _ptr[22].ptr)
//         int  waitForInterrupt    (int pin, int mS) ;
//         void digitalWrite        (int pin, int value) ;
#define PF_digitalWrite (* (void (*)(int,int))                                     _ptr[23].ptr)
//         void digitalWrite8       (int pin, int value) ;
#define PF_digitalWrite8 (* (void (*)(int,int))                                    _ptr[24].ptr)
//         void digitalWriteByte    (int value) ;
//         void digitalWriteByte2   (int value) ;
//         int  wiringPiSetupPhys   (void) ;
#define PF_wiringPiSetupPhys (* (int (*)(void))                                    _ptr[25].ptr)
//         void pwmSetMode          (int mode) ;
#define PF_pwmSetMode (* (void (*)(int))                                           _ptr[26].ptr)
//         unsigned int  digitalReadByte     (void) ;
#define PF_digitalReadByte (* (unsigned int (*)(void))                             _ptr[27].ptr)
//         unsigned int  digitalReadByte2    (void) ;
//         void analogWrite         (int pin, int value) ;
#define PF_analogWrite (* (void (*)(int,int))                                      _ptr[28].ptr)
//         int  piBoardRev          (void) ;	// Deprecated
#define PF_piBoardRev (* (int (*)(void))                                           _ptr[29].ptr)
//         int  wiringPiSetupSys    (void) ;
#define PF_wiringPiSetupSys (* (int (*)(void))                                     _ptr[30].ptr)
//         int  wiringPiSetup       (void) ;
#define PF_wiringPiSetup (* (int (*)(void))                                        _ptr[31].ptr)
//         int  wiringPiSetupSys    (void) ;
//         int  wiringPiSetupGpio   (void) ;
//         int  wiringPiSetupPhys   (void) ;
//         int  wiringPiSetupPiFace (void) ;
#define PF_wiringPiSetupPiFace (* (int (*)(void))                                  _ptr[32].ptr)
//         int  wiringPiSetupPiFaceForGpioProg (void) ;	// Don't use this - for gpio program only
#define PF_wiringPiSetupPiFaceForGpioProg (* (int (*)(void))                       _ptr[33].ptr)
//         void pwmWrite            (int pin, int value) ;
#define PF_pwmWrite (* (void (*)(int,int))                                         _ptr[34].ptr)
//         void setPadDrive         (int group, int value) ;
#define PF_setPadDrive (* (void (*)(int,int))                                      _ptr[35].ptr)
//         void pwmSetClock         (int divisor) ;
#define PF_pwmSetClock (* (void (*)(int))                                          _ptr[36].ptr)
//         int  wpiPinToGpio        (int wpiPin) ;
#define PF_wpiPinToGpio (* (int (*)(int))                                          _ptr[37].ptr)
//         void pullUpDnControl     (int pin, int pud) ;
#define PF_pullUpDnControl (* (void (*)(int,int))                                  _ptr[38].ptr)
//         void pwmToneWrite        (int pin, int freq) ;
#define PF_pwmToneWrite (* (void (*)(int,int))                                     _ptr[39].ptr)
//         int  digitalRead         (int pin) ;
#define PF_digitalRead (* (int (*)(int))                                           _ptr[40].ptr)
//         unsigned int  digitalRead8        (int pin) ;
#define PF_digitalRead8 (* (unsigned int (*)(int))                                 _ptr[41].ptr)
//         unsigned int  digitalReadByte     (void) ;
//         unsigned int  digitalReadByte2    (void) ;
//         void         delayMicroseconds (unsigned int howLong) ;
//         unsigned int millis            (void) ;
#define PF_millis (* (unsigned int (*)(void))                                      _ptr[42].ptr)
//         void piLock              (int key) ;
#define PF_piLock (* (void (*)(int))                                               _ptr[43].ptr)
//         unsigned int micros            (void) ;
//         int piHiPri (const int pri) ;
//         int  digitalRead         (int pin) ;
//         unsigned int  digitalRead8        (int pin) ;
//         unsigned int  digitalReadByte     (void) ;
//         unsigned int  digitalReadByte2    (void) ;
//         int  getAlt              (int pin) ;
//         void pwmSetMode          (int mode) ;
//         void digitalWrite        (int pin, int value) ;
//         void digitalWrite8       (int pin, int value) ;
//         void digitalWriteByte    (int value) ;
//         void digitalWriteByte2   (int value) ;
//         void pwmWrite            (int pin, int value) ;
//         int  wpiPinToGpio        (int wpiPin) ;
//         void pwmSetClock         (int divisor) ;
//         int  waitForInterrupt    (int pin, int mS) ;
//         void         delayMicroseconds (unsigned int howLong) ;
//         struct wiringPiNodeStruct *wiringPiFindNode (int pin) ;
//         int  piGpioLayout        (void) ;
//         void         delay             (unsigned int howLong) ;
//         void         delayMicroseconds (unsigned int howLong) ;
//         int  waitForInterrupt    (int pin, int mS) ;
//         void analogWrite         (int pin, int value) ;
//         unsigned int  digitalReadByte     (void) ;
//         unsigned int  digitalReadByte2    (void) ;
//         int wiringPiFailure (int fatal, const char *message, ...) ;
//         void digitalWriteByte    (int value) ;
//         void digitalWriteByte2   (int value) ;
//         void piUnlock            (int key) ;
//         int  wiringPiSetupSys    (void) ;
//         void pwmToneWrite        (int pin, int freq) ;
//         int  physPinToGpio       (int physPin) ;
//         void piBoardId           (int *model, int *rev, int *mem, int *maker, int *overVolted) ;
//         void piLock              (int key) ;
//         void pinModeAlt          (int pin, int mode) ;
//         void pinMode             (int pin, int mode) ;
//         int  wiringPiSetup       (void) ;
//         int  wiringPiSetupSys    (void) ;
//         int  wiringPiSetupGpio   (void) ;
//         int  wiringPiSetupPhys   (void) ;
//         int  wiringPiSetupPiFace (void) ;
//         int  wiringPiSetupPiFaceForGpioProg (void) ;	// Don't use this - for gpio program only
//         void pullUpDnControl     (int pin, int pud) ;
//         int  piBoardRev          (void) ;	// Deprecated
//         void setPadDrive         (int group, int value) ;
//         void wiringPiVersion	(int *major, int *minor) ;
//         unsigned int millis            (void) ;
//         void gpioClockSet        (int pin, int freq) ;
//         void pwmSetRange         (unsigned int range) ;
//         void pinModeAlt          (int pin, int mode) ;
//         struct wiringPiNodeStruct *wiringPiNewNode  (int pinBase, int numPins) ;
//         int  analogRead          (int pin) ;
//         int  wiringPiSetupGpio   (void) ;
//         int  wiringPiSetupPhys   (void) ;
//         unsigned int  digitalReadByte2    (void) ;
//         void digitalWriteByte2   (int value) ;


// -------------- all funcs array ---------------
struct FUNCS_ {
  const char *name;
  void  (*ptr)(void);
};

inline const FUNCS_* load(void **ppdll)
{
    union
    {
    void *p;
    void (*fp)(void);
    } u;

    static struct FUNCS_ _funcs[] ={
        {"waitForInterrupt", nullptr},
        {"micros", nullptr},
        {"piUnlock", nullptr},
        {"gpioClockSet", nullptr},
        {"*wiringPiFindNode", nullptr},
        {"wiringPiVersion", nullptr},
        {"piHiPri", nullptr},
        {"digitalWriteByte", nullptr},
        {"digitalWriteByte2", nullptr},
        {"wiringPiFailure", nullptr},
        {"physPinToGpio", nullptr},
        {"digitalReadByte2", nullptr},
        {"delay", nullptr},
        {"delayMicroseconds", nullptr},
        {"analogRead", nullptr},
        {"pinModeAlt", nullptr},
        {"pwmSetRange", nullptr},
        {"pinMode", nullptr},
        {"piBoardId", nullptr},
        {"getAlt", nullptr},
        {"*wiringPiNewNode", nullptr},
        {"wiringPiSetupGpio", nullptr},
        {"piGpioLayout", nullptr},
        {"digitalWrite", nullptr},
        {"digitalWrite8", nullptr},
        {"wiringPiSetupPhys", nullptr},
        {"pwmSetMode", nullptr},
        {"digitalReadByte", nullptr},
        {"analogWrite", nullptr},
        {"piBoardRev", nullptr},
        {"wiringPiSetupSys", nullptr},
        {"wiringPiSetup", nullptr},
        {"wiringPiSetupPiFace", nullptr},
        {"wiringPiSetupPiFaceForGpioProg", nullptr},
        {"pwmWrite", nullptr},
        {"setPadDrive", nullptr},
        {"pwmSetClock", nullptr},
        {"wpiPinToGpio", nullptr},
        {"pullUpDnControl", nullptr},
        {"pwmToneWrite", nullptr},
        {"digitalRead", nullptr},
        {"digitalRead8", nullptr},
        {"millis", nullptr},
        {"piLock", nullptr},
        {nullptr, nullptr}
    };
    void  *dll_handle;
    if ((dll_handle = dlopen("/usr/lib/libwiringPi.so", RTLD_LAZY)) == 0)
    {
        perror("cannot load wiringpi\n");
        return nullptr;
    }
    *ppdll = dll_handle;
    struct FUNCS_ *fp = _funcs;
    for (; fp->name != nullptr; fp++)
    {
        u.p = dlsym(dll_handle, fp->name);
        if (u.fp == 0)
        {
            perror("cannot load:");
        }
        else
        {
            printf("LOADING FUNCTION %s \n", fp->name);
            fp->ptr = u.fp;
        }
    }
    return _funcs;
}
#endif // LIB_RESOLVER
