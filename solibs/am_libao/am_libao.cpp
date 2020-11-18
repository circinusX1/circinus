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
#include <sqrat.h>
#include "am_libao.h"

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
     Sqrat::RootTable(v).Functor("ao_append_global_option",PF_ao_append_global_option);
     Sqrat::RootTable(v).Functor("ao_append_option",PF_ao_append_option);
     Sqrat::RootTable(v).Functor("ao_close",PF_ao_close);
     Sqrat::RootTable(v).Functor("ao_default_driver_id",PF_ao_default_driver_id);
     Sqrat::RootTable(v).Functor("ao_driver_id",PF_ao_driver_id);
     Sqrat::RootTable(v).Functor("ao_driver_info",PF_ao_driver_info);
     Sqrat::RootTable(v).Functor("ao_driver_info_list",PF_ao_driver_info_list);
     Sqrat::RootTable(v).Functor("ao_file_extension",PF_ao_file_extension);
     Sqrat::RootTable(v).Functor("ao_free_options",PF_ao_free_options);
     Sqrat::RootTable(v).Functor("ao_initialize",PF_ao_initialize);
     Sqrat::RootTable(v).Functor("ao_is_big_endian",PF_ao_is_big_endian);
     Sqrat::RootTable(v).Functor("ao_open_file",PF_ao_open_file);
     Sqrat::RootTable(v).Functor("ao_open_live",PF_ao_open_live);
     Sqrat::RootTable(v).Functor("ao_play",PF_ao_play);
     Sqrat::RootTable(v).Functor("ao_shutdown",PF_ao_shutdown);

    return SQ_OK;
}
