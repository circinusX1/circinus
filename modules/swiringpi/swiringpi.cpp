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
    Sqrat::RootTable(v).Func("waitForInterrupt",PF_waitForInterrupt);
    Sqrat::RootTable(v).Func("micros",PF_micros);
    Sqrat::RootTable(v).Func("piUnlock",PF_piUnlock);
    Sqrat::RootTable(v).Func("gpioClockSet",PF_gpioClockSet);
    //Sqrat::RootTable(v).Func("*wiringPiFindNode",PF_*wiringPiFindNode);
    Sqrat::RootTable(v).Func("wiringPiVersion",PF_wiringPiVersion);
    //Sqrat::RootTable(v).Func("piHiPri",PF_piHiPri);
    Sqrat::RootTable(v).Func("digitalWriteByte",PF_digitalWriteByte);
    Sqrat::RootTable(v).Func("digitalWriteByte2",PF_digitalWriteByte2);
    //Sqrat::RootTable(v).Func("wiringPiFailure",PF_wiringPiFailure);
    Sqrat::RootTable(v).Func("physPinToGpio",PF_physPinToGpio);
    Sqrat::RootTable(v).Func("digitalReadByte2",PF_digitalReadByte2);
    Sqrat::RootTable(v).Func("delay",PF_delay);
    Sqrat::RootTable(v).Func("delayMicroseconds",PF_delayMicroseconds);
    Sqrat::RootTable(v).Func("analogRead",PF_analogRead);
    Sqrat::RootTable(v).Func("pinModeAlt",PF_pinModeAlt);
    Sqrat::RootTable(v).Func("pwmSetRange",PF_pwmSetRange);
    Sqrat::RootTable(v).Func("pinMode",PF_pinMode);
    Sqrat::RootTable(v).Func("piBoardId",PF_piBoardId);
    Sqrat::RootTable(v).Func("getAlt",PF_getAlt);
    //Sqrat::RootTable(v).Func("*wiringPiNewNode",PF_*wiringPiNewNode);
    Sqrat::RootTable(v).Func("wiringPiSetupGpio",PF_wiringPiSetupGpio);
    Sqrat::RootTable(v).Func("piGpioLayout",PF_piGpioLayout);
    Sqrat::RootTable(v).Func("digitalWrite",PF_digitalWrite);
    Sqrat::RootTable(v).Func("digitalWrite8",PF_digitalWrite8);
    Sqrat::RootTable(v).Func("wiringPiSetupPhys",PF_wiringPiSetupPhys);
    Sqrat::RootTable(v).Func("pwmSetMode",PF_pwmSetMode);
    Sqrat::RootTable(v).Func("digitalReadByte",PF_digitalReadByte);
    Sqrat::RootTable(v).Func("analogWrite",PF_analogWrite);
    Sqrat::RootTable(v).Func("piBoardRev",PF_piBoardRev);
    Sqrat::RootTable(v).Func("wiringPiSetupSys",PF_wiringPiSetupSys);
    Sqrat::RootTable(v).Func("wiringPiSetup",PF_wiringPiSetup);
    Sqrat::RootTable(v).Func("wiringPiSetupPiFace",PF_wiringPiSetupPiFace);
    Sqrat::RootTable(v).Func("wiringPiSetupPiFaceForGpioProg",PF_wiringPiSetupPiFaceForGpioProg);
    Sqrat::RootTable(v).Func("pwmWrite",PF_pwmWrite);
    Sqrat::RootTable(v).Func("setPadDrive",PF_setPadDrive);
    Sqrat::RootTable(v).Func("pwmSetClock",PF_pwmSetClock);
    Sqrat::RootTable(v).Func("wpiPinToGpio",PF_wpiPinToGpio);
    Sqrat::RootTable(v).Func("pullUpDnControl",PF_pullUpDnControl);
    Sqrat::RootTable(v).Func("pwmToneWrite",PF_pwmToneWrite);
    Sqrat::RootTable(v).Func("digitalRead",PF_digitalRead);
    Sqrat::RootTable(v).Func("digitalRead8",PF_digitalRead8);
    Sqrat::RootTable(v).Func("millis",PF_millis);
    Sqrat::RootTable(v).Func("piLock",PF_piLock);


    return SQ_OK;
}


int test(const char* s,int k)
{
    printf("%s %s %d \n",__FUNCTION__,s,k);
    return 40;
}
