/*
THIS SOFTWARE IS PROVIDED BY mcomariusATg.ailDOTcom 'AS IS' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL mcomariusATgmailDOTcom BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*/

#include <dlfcn.h>
#include "swiringpi.h"
#include "sqrat.h"
#include "libwiring.h"

sq_api*          SQ_PTRS;
const FUNCS_*   _ptr; // global var
void*           _dll;

EXPORT void sqmodule_unload(void)
{
    dlclose(_dll);
}

// Module registration
SQRESULT sqmodule_load(HSKVM v, HSQAPI api)
{
    SQ_PTRS = api;
    _ptr = load(&_dll); // in main()
    printf("LOADING FUNCTIONS \n");
    Sqrat::RootTable(v).Functor("waitForInterrupt",PF_waitForInterrupt);
    Sqrat::RootTable(v).Functor("micros",PF_micros);
    Sqrat::RootTable(v).Functor("piUnlock",PF_piUnlock);
    Sqrat::RootTable(v).Functor("gpioClockSet",PF_gpioClockSet);
    //Sqrat::RootTable(v).Functor("*wiringPiFindNode",PF_*wiringPiFindNode);
    Sqrat::RootTable(v).Functor("wiringPiVersion",PF_wiringPiVersion);
    //Sqrat::RootTable(v).Functor("piHiPri",PF_piHiPri);
    Sqrat::RootTable(v).Functor("digitalWriteByte",PF_digitalWriteByte);
    Sqrat::RootTable(v).Functor("digitalWriteByte2",PF_digitalWriteByte2);
    //Sqrat::RootTable(v).Functor("wiringPiFailure",PF_wiringPiFailure);
    Sqrat::RootTable(v).Functor("physPinToGpio",PF_physPinToGpio);
    Sqrat::RootTable(v).Functor("digitalReadByte2",PF_digitalReadByte2);
    Sqrat::RootTable(v).Functor("delay",PF_delay);
    Sqrat::RootTable(v).Functor("delayMicroseconds",PF_delayMicroseconds);
    Sqrat::RootTable(v).Functor("analogRead",PF_analogRead);
    Sqrat::RootTable(v).Functor("pinModeAlt",PF_pinModeAlt);
    Sqrat::RootTable(v).Functor("pwmSetRange",PF_pwmSetRange);
    Sqrat::RootTable(v).Functor("pinMode",PF_pinMode);
    Sqrat::RootTable(v).Functor("piBoardId",PF_piBoardId);
    Sqrat::RootTable(v).Functor("getAlt",PF_getAlt);
    //Sqrat::RootTable(v).Functor("*wiringPiNewNode",PF_*wiringPiNewNode);
    Sqrat::RootTable(v).Functor("wiringPiSetupGpio",PF_wiringPiSetupGpio);
    Sqrat::RootTable(v).Functor("piGpioLayout",PF_piGpioLayout);
    Sqrat::RootTable(v).Functor("digitalWrite",PF_digitalWrite);
    Sqrat::RootTable(v).Functor("digitalWrite8",PF_digitalWrite8);
    Sqrat::RootTable(v).Functor("wiringPiSetupPhys",PF_wiringPiSetupPhys);
    Sqrat::RootTable(v).Functor("pwmSetMode",PF_pwmSetMode);
    Sqrat::RootTable(v).Functor("digitalReadByte",PF_digitalReadByte);
    Sqrat::RootTable(v).Functor("analogWrite",PF_analogWrite);
    Sqrat::RootTable(v).Functor("piBoardRev",PF_piBoardRev);
    Sqrat::RootTable(v).Functor("wiringPiSetupSys",PF_wiringPiSetupSys);
    Sqrat::RootTable(v).Functor("wiringPiSetup",PF_wiringPiSetup);
    Sqrat::RootTable(v).Functor("wiringPiSetupPiFace",PF_wiringPiSetupPiFace);
    Sqrat::RootTable(v).Functor("wiringPiSetupPiFaceForGpioProg",PF_wiringPiSetupPiFaceForGpioProg);
    Sqrat::RootTable(v).Functor("pwmWrite",PF_pwmWrite);
    Sqrat::RootTable(v).Functor("setPadDrive",PF_setPadDrive);
    Sqrat::RootTable(v).Functor("pwmSetClock",PF_pwmSetClock);
    Sqrat::RootTable(v).Functor("wpiPinToGpio",PF_wpiPinToGpio);
    Sqrat::RootTable(v).Functor("pullUpDnControl",PF_pullUpDnControl);
    Sqrat::RootTable(v).Functor("pwmToneWrite",PF_pwmToneWrite);
    Sqrat::RootTable(v).Functor("digitalRead",PF_digitalRead);
    Sqrat::RootTable(v).Functor("digitalRead8",PF_digitalRead8);
    Sqrat::RootTable(v).Functor("millis",PF_millis);
    Sqrat::RootTable(v).Functor("piLock",PF_piLock);


    return SQ_OK;
}


int test(const char* s,int k)
{
    printf("%s %s %d \n",__FUNCTION__,s,k);
    return 40;
}
