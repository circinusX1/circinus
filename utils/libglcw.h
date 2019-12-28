#ifndef LIBR_RESOLVER_H
#define LIBR_RESOLVER_H
// Copyright Marius C. https://github/comarius (do not remove)
#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>


// -------------- constants ---------------
// review before compile...
#define _glfw3_h_
 #define _WIN32
  #define APIENTRY __stdcall
  #define APIENTRY
 #define WINGDIAPI __declspec(dllimport)
 #define GLFW_WINGDIAPI_DEFINED
 #define CALLBACK __stdcall
 #define GLFW_CALLBACK_DEFINED
   #define GL_GLEXT_LEGACY
 #define GLFWAPI __declspec(dllexport)
 #define GLFWAPI __declspec(dllimport)
 #define GLFWAPI __attribute__((visibility("default")))
 #define GLFWAPI
#define GLFW_VERSION_MAJOR          3
#define GLFW_VERSION_MINOR          2
#define GLFW_VERSION_REVISION       1
#define GLFW_TRUE                   1
#define GLFW_FALSE                  0
#define GLFW_RELEASE                0
#define GLFW_PRESS                  1
#define GLFW_REPEAT                 2
#define GLFW_KEY_UNKNOWN            -1
#define GLFW_KEY_SPACE              32
#define GLFW_KEY_APOSTROPHE         39  /* ' */
#define GLFW_KEY_COMMA              44  /* 
 */
#define GLFW_KEY_MINUS              45  /* - */
#define GLFW_KEY_PERIOD             46  /* . */
#define GLFW_KEY_SLASH              47  /* / */
#define GLFW_KEY_0                  48
#define GLFW_KEY_1                  49
#define GLFW_KEY_2                  50
#define GLFW_KEY_3                  51
#define GLFW_KEY_4                  52
#define GLFW_KEY_5                  53
#define GLFW_KEY_6                  54
#define GLFW_KEY_7                  55
#define GLFW_KEY_8                  56
#define GLFW_KEY_9                  57
#define GLFW_KEY_SEMICOLON          59  /* ; */
#define GLFW_KEY_EQUAL              61  /* = */
#define GLFW_KEY_A                  65
#define GLFW_KEY_B                  66
#define GLFW_KEY_C                  67
#define GLFW_KEY_D                  68
#define GLFW_KEY_E                  69
#define GLFW_KEY_F                  70
#define GLFW_KEY_G                  71
#define GLFW_KEY_H                  72
#define GLFW_KEY_I                  73
#define GLFW_KEY_J                  74
#define GLFW_KEY_K                  75
#define GLFW_KEY_L                  76
#define GLFW_KEY_M                  77
#define GLFW_KEY_N                  78
#define GLFW_KEY_O                  79
#define GLFW_KEY_P                  80
#define GLFW_KEY_Q                  81
#define GLFW_KEY_R                  82
#define GLFW_KEY_S                  83
#define GLFW_KEY_T                  84
#define GLFW_KEY_U                  85
#define GLFW_KEY_V                  86
#define GLFW_KEY_W                  87
#define GLFW_KEY_X                  88
#define GLFW_KEY_Y                  89
#define GLFW_KEY_Z                  90
#define GLFW_KEY_LEFT_BRACKET       91  /* [ */
#define GLFW_KEY_BACKSLASH          92  /* \ */
#define GLFW_KEY_RIGHT_BRACKET      93  /* ] */
#define GLFW_KEY_GRAVE_ACCENT       96  /* ` */
#define GLFW_KEY_WORLD_1            161 /* non-US #1 */
#define GLFW_KEY_WORLD_2            162 /* non-US #2 */
#define GLFW_KEY_ESCAPE             256
#define GLFW_KEY_ENTER              257
#define GLFW_KEY_TAB                258
#define GLFW_KEY_BACKSPACE          259
#define GLFW_KEY_INSERT             260
#define GLFW_KEY_DELETE             261
#define GLFW_KEY_RIGHT              262
#define GLFW_KEY_LEFT               263
#define GLFW_KEY_DOWN               264
#define GLFW_KEY_UP                 265
#define GLFW_KEY_PAGE_UP            266
#define GLFW_KEY_PAGE_DOWN          267
#define GLFW_KEY_HOME               268
#define GLFW_KEY_END                269
#define GLFW_KEY_CAPS_LOCK          280
#define GLFW_KEY_SCROLL_LOCK        281
#define GLFW_KEY_NUM_LOCK           282
#define GLFW_KEY_PRINT_SCREEN       283
#define GLFW_KEY_PAUSE              284
#define GLFW_KEY_F1                 290
#define GLFW_KEY_F2                 291
#define GLFW_KEY_F3                 292
#define GLFW_KEY_F4                 293
#define GLFW_KEY_F5                 294
#define GLFW_KEY_F6                 295
#define GLFW_KEY_F7                 296
#define GLFW_KEY_F8                 297
#define GLFW_KEY_F9                 298
#define GLFW_KEY_F10                299
#define GLFW_KEY_F11                300
#define GLFW_KEY_F12                301
#define GLFW_KEY_F13                302
#define GLFW_KEY_F14                303
#define GLFW_KEY_F15                304
#define GLFW_KEY_F16                305
#define GLFW_KEY_F17                306
#define GLFW_KEY_F18                307
#define GLFW_KEY_F19                308
#define GLFW_KEY_F20                309
#define GLFW_KEY_F21                310
#define GLFW_KEY_F22                311
#define GLFW_KEY_F23                312
#define GLFW_KEY_F24                313
#define GLFW_KEY_F25                314
#define GLFW_KEY_KP_0               320
#define GLFW_KEY_KP_1               321
#define GLFW_KEY_KP_2               322
#define GLFW_KEY_KP_3               323
#define GLFW_KEY_KP_4               324
#define GLFW_KEY_KP_5               325
#define GLFW_KEY_KP_6               326
#define GLFW_KEY_KP_7               327
#define GLFW_KEY_KP_8               328
#define GLFW_KEY_KP_9               329
#define GLFW_KEY_KP_DECIMAL         330
#define GLFW_KEY_KP_DIVIDE          331
#define GLFW_KEY_KP_MULTIPLY        332
#define GLFW_KEY_KP_SUBTRACT        333
#define GLFW_KEY_KP_ADD             334
#define GLFW_KEY_KP_ENTER           335
#define GLFW_KEY_KP_EQUAL           336
#define GLFW_KEY_LEFT_SHIFT         340
#define GLFW_KEY_LEFT_CONTROL       341
#define GLFW_KEY_LEFT_ALT           342
#define GLFW_KEY_LEFT_SUPER         343
#define GLFW_KEY_RIGHT_SHIFT        344
#define GLFW_KEY_RIGHT_CONTROL      345
#define GLFW_KEY_RIGHT_ALT          346
#define GLFW_KEY_RIGHT_SUPER        347
#define GLFW_KEY_MENU               348
#define GLFW_KEY_LAST               GLFW_KEY_MENU
#define GLFW_MOD_SHIFT           0x0001
#define GLFW_MOD_CONTROL         0x0002
#define GLFW_MOD_ALT             0x0004
#define GLFW_MOD_SUPER           0x0008
#define GLFW_MOUSE_BUTTON_1         0
#define GLFW_MOUSE_BUTTON_2         1
#define GLFW_MOUSE_BUTTON_3         2
#define GLFW_MOUSE_BUTTON_4         3
#define GLFW_MOUSE_BUTTON_5         4
#define GLFW_MOUSE_BUTTON_6         5
#define GLFW_MOUSE_BUTTON_7         6
#define GLFW_MOUSE_BUTTON_8         7
#define GLFW_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define GLFW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3
#define GLFW_JOYSTICK_1             0
#define GLFW_JOYSTICK_2             1
#define GLFW_JOYSTICK_3             2
#define GLFW_JOYSTICK_4             3
#define GLFW_JOYSTICK_5             4
#define GLFW_JOYSTICK_6             5
#define GLFW_JOYSTICK_7             6
#define GLFW_JOYSTICK_8             7
#define GLFW_JOYSTICK_9             8
#define GLFW_JOYSTICK_10            9
#define GLFW_JOYSTICK_11            10
#define GLFW_JOYSTICK_12            11
#define GLFW_JOYSTICK_13            12
#define GLFW_JOYSTICK_14            13
#define GLFW_JOYSTICK_15            14
#define GLFW_JOYSTICK_16            15
#define GLFW_JOYSTICK_LAST          GLFW_JOYSTICK_16
#define GLFW_NOT_INITIALIZED        0x00010001
#define GLFW_NO_CURRENT_CONTEXT     0x00010002
#define GLFW_INVALID_ENUM           0x00010003
#define GLFW_INVALID_VALUE          0x00010004
#define GLFW_OUT_OF_MEMORY          0x00010005
#define GLFW_API_UNAVAILABLE        0x00010006
#define GLFW_VERSION_UNAVAILABLE    0x00010007
#define GLFW_PLATFORM_ERROR         0x00010008
#define GLFW_FORMAT_UNAVAILABLE     0x00010009
#define GLFW_NO_WINDOW_CONTEXT      0x0001000A
#define GLFW_FOCUSED                0x00020001
#define GLFW_ICONIFIED              0x00020002
#define GLFW_RESIZABLE              0x00020003
#define GLFW_VISIBLE                0x00020004
#define GLFW_DECORATED              0x00020005
#define GLFW_AUTO_ICONIFY           0x00020006
#define GLFW_FLOATING               0x00020007
#define GLFW_MAXIMIZED              0x00020008
#define GLFW_RED_BITS               0x00021001
#define GLFW_GREEN_BITS             0x00021002
#define GLFW_BLUE_BITS              0x00021003
#define GLFW_ALPHA_BITS             0x00021004
#define GLFW_DEPTH_BITS             0x00021005
#define GLFW_STENCIL_BITS           0x00021006
#define GLFW_ACCUM_RED_BITS         0x00021007
#define GLFW_ACCUM_GREEN_BITS       0x00021008
#define GLFW_ACCUM_BLUE_BITS        0x00021009
#define GLFW_ACCUM_ALPHA_BITS       0x0002100A
#define GLFW_AUX_BUFFERS            0x0002100B
#define GLFW_STEREO                 0x0002100C
#define GLFW_SAMPLES                0x0002100D
#define GLFW_SRGB_CAPABLE           0x0002100E
#define GLFW_REFRESH_RATE           0x0002100F
#define GLFW_DOUBLEBUFFER           0x00021010
#define GLFW_CLIENT_API             0x00022001
#define GLFW_CONTEXT_VERSION_MAJOR  0x00022002
#define GLFW_CONTEXT_VERSION_MINOR  0x00022003
#define GLFW_CONTEXT_REVISION       0x00022004
#define GLFW_CONTEXT_ROBUSTNESS     0x00022005
#define GLFW_OPENGL_FORWARD_COMPAT  0x00022006
#define GLFW_OPENGL_DEBUG_CONTEXT   0x00022007
#define GLFW_OPENGL_PROFILE         0x00022008
#define GLFW_CONTEXT_RELEASE_BEHAVIOR 0x00022009
#define GLFW_CONTEXT_NO_ERROR       0x0002200A
#define GLFW_CONTEXT_CREATION_API   0x0002200B
#define GLFW_NO_API                          0
#define GLFW_OPENGL_API             0x00030001
#define GLFW_OPENGL_ES_API          0x00030002
#define GLFW_NO_ROBUSTNESS                   0
#define GLFW_NO_RESET_NOTIFICATION  0x00031001
#define GLFW_LOSE_CONTEXT_ON_RESET  0x00031002
#define GLFW_OPENGL_ANY_PROFILE              0
#define GLFW_OPENGL_CORE_PROFILE    0x00032001
#define GLFW_OPENGL_COMPAT_PROFILE  0x00032002
#define GLFW_CURSOR                 0x00033001
#define GLFW_STICKY_KEYS            0x00033002
#define GLFW_STICKY_MOUSE_BUTTONS   0x00033003
#define GLFW_CURSOR_NORMAL          0x00034001
#define GLFW_CURSOR_HIDDEN          0x00034002
#define GLFW_CURSOR_DISABLED        0x00034003
#define GLFW_ANY_RELEASE_BEHAVIOR            0
#define GLFW_RELEASE_BEHAVIOR_FLUSH 0x00035001
#define GLFW_RELEASE_BEHAVIOR_NONE  0x00035002
#define GLFW_NATIVE_CONTEXT_API     0x00036001
#define GLFW_EGL_CONTEXT_API        0x00036002
#define GLFW_ARROW_CURSOR           0x00036001
#define GLFW_IBEAM_CURSOR           0x00036002
#define GLFW_CROSSHAIR_CURSOR       0x00036003
#define GLFW_HAND_CURSOR            0x00036004
#define GLFW_HRESIZE_CURSOR         0x00036005
#define GLFW_VRESIZE_CURSOR         0x00036006
#define GLFW_CONNECTED              0x00040001
#define GLFW_DISCONNECTED           0x00040002
#define GLFW_DONT_CARE              -1


// -------------- functions ---------------
// review before compile...
//         GLFWAPI GLFWglproc glfwGetProcAddress(const char* procname);
#define PF_glfwGetProcAddress (* (GLFWAPI GLFWglproc (*)(const))                   _ptr[0].ptr)
//         GLFWAPI int glfwExtensionSupported(const char* extension);
#define PF_glfwExtensionSupported (* (GLFWAPI int (*)(const))                      _ptr[1].ptr)
//         GLFWAPI void glfwMakeContextCurrent(GLFWwindow* window);
#define PF_glfwMakeContextCurrent (* (GLFWAPI void (*)(GLFWwindow*))               _ptr[2].ptr)
//         GLFWAPI GLFWwindow* glfwGetCurrentContext(void);
#define PF_glfwGetCurrentContext (* (GLFWAPI GLFWwindow* (*)(void))                _ptr[3].ptr)
//         GLFWAPI void glfwSwapBuffers(GLFWwindow* window);
#define PF_glfwSwapBuffers (* (GLFWAPI void (*)(GLFWwindow*))                      _ptr[4].ptr)
//         GLFWAPI void glfwSwapInterval(int interval);
#define PF_glfwSwapInterval (* (GLFWAPI void (*)(int))                             _ptr[5].ptr)
//         GLFWAPI int glfwInit(void);
#define PF_glfwInit (* (GLFWAPI int (*)(void))                                     _ptr[6].ptr)
//         GLFWAPI void glfwDefaultWindowHints(void);
#define PF_glfwDefaultWindowHints (* (GLFWAPI void (*)(void))                      _ptr[7].ptr)
//         GLFWAPI void glfwTerminate(void);
#define PF_glfwTerminate (* (GLFWAPI void (*)(void))                               _ptr[8].ptr)
//         GLFWAPI void glfwDestroyWindow(GLFWwindow* window);
#define PF_glfwDestroyWindow (* (GLFWAPI void (*)(GLFWwindow*))                    _ptr[9].ptr)
//         GLFWAPI void glfwDestroyCursor(GLFWcursor* cursor);
#define PF_glfwDestroyCursor (* (GLFWAPI void (*)(GLFWcursor*))                    _ptr[10].ptr)
//         GLFWAPI void glfwGetVersion(int* major, int* minor, int* rev);
#define PF_glfwGetVersion (* (GLFWAPI void (*)(int*,int*,int*))                    _ptr[11].ptr)
//         GLFWAPI const char* glfwGetVersionString(void);
#define PF_const (* (GLFWAPI (*)(void))                                            _ptr[12].ptr)
//         GLFWAPI GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun cbfun);
#define PF_glfwSetErrorCallback (* (GLFWAPI GLFWerrorfun (*)(GLFWerrorfun))        _ptr[13].ptr)
//         GLFWAPI int glfwGetInputMode(GLFWwindow* window, int mode);
#define PF_glfwGetInputMode (* (GLFWAPI int (*)(GLFWwindow*,int))                  _ptr[14].ptr)
//         GLFWAPI void glfwSetInputMode(GLFWwindow* window, int mode, int value);
#define PF_glfwSetInputMode (* (GLFWAPI void (*)(GLFWwindow*,int,int))             _ptr[15].ptr)
//         GLFWAPI const char* glfwGetKeyName(int key, int scancode);
#define PF_const (* (GLFWAPI (*)(int,int))                                         _ptr[16].ptr)
//         GLFWAPI int glfwGetKey(GLFWwindow* window, int key);
#define PF_glfwGetKey (* (GLFWAPI int (*)(GLFWwindow*,int))                        _ptr[17].ptr)
//         GLFWAPI int glfwGetMouseButton(GLFWwindow* window, int button);
#define PF_glfwGetMouseButton (* (GLFWAPI int (*)(GLFWwindow*,int))                _ptr[18].ptr)
//         GLFWAPI void glfwGetCursorPos(GLFWwindow* window, double* xpos, double* ypos);
#define PF_glfwGetCursorPos (* (GLFWAPI void (*)(GLFWwindow*,double*,double*))     _ptr[19].ptr)
//         GLFWAPI void glfwSetCursorPos(GLFWwindow* window, double xpos, double ypos);
#define PF_glfwSetCursorPos (* (GLFWAPI void (*)(GLFWwindow*,double,double))       _ptr[20].ptr)
//         GLFWAPI GLFWcursorposfun glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun cbfun);
#define PF_glfwSetCursorPosCallback (* (GLFWAPI GLFWcursorposfun (*)(GLFWwindow*,GLFWcursorposfun))    _ptr[21].ptr)
//         GLFWAPI GLFWcursor* glfwCreateCursor(const GLFWimage* image, int xhot, int yhot);
#define PF_glfwCreateCursor (* (GLFWAPI GLFWcursor* (*)(const,int,int))            _ptr[22].ptr)
//         GLFWAPI GLFWcursor* glfwCreateStandardCursor(int shape);
#define PF_glfwCreateStandardCursor (* (GLFWAPI GLFWcursor* (*)(int))              _ptr[23].ptr)
//         GLFWAPI void glfwSetCursor(GLFWwindow* window, GLFWcursor* cursor);
#define PF_glfwSetCursor (* (GLFWAPI void (*)(GLFWwindow*,GLFWcursor*))            _ptr[24].ptr)
//         GLFWAPI GLFWcursorenterfun glfwSetCursorEnterCallback(GLFWwindow* window, GLFWcursorenterfun cbfun);
#define PF_glfwSetCursorEnterCallback (* (GLFWAPI GLFWcursorenterfun (*)(GLFWwindow*,GLFWcursorenterfun))    _ptr[25].ptr)
//         GLFWAPI GLFWkeyfun glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun cbfun);
#define PF_glfwSetKeyCallback (* (GLFWAPI GLFWkeyfun (*)(GLFWwindow*,GLFWkeyfun))    _ptr[26].ptr)
//         *  [key callback](@ref glfwSetKeyCallback) is not.  Characters do not map 1:1
#define PF_callback] (* (* [key (*)(@ref))                                         _ptr[27].ptr)
//         *  [key callback](@ref glfwSetKeyCallback) instead.
#define PF_callback] (* (* [key (*)(@ref))                                         _ptr[28].ptr)
//         *  [character callback](@ref glfwSetCharCallback) instead.
#define PF_callback] (* (* [character (*)(@ref))                                   _ptr[29].ptr)
//         GLFWAPI GLFWcharfun glfwSetCharCallback(GLFWwindow* window, GLFWcharfun cbfun);
#define PF_glfwSetCharCallback (* (GLFWAPI GLFWcharfun (*)(GLFWwindow*,GLFWcharfun))    _ptr[30].ptr)
//         *  [character callback](@ref glfwSetCharCallback).  Like the character
#define PF_callback] (* (* [character (*)(@ref))                                   _ptr[31].ptr)
//         *  [character with modifiers callback](@ref glfwSetCharModsCallback) that
#define PF_[character (* (* (*)(@ref))                                             _ptr[32].ptr)
//         GLFWAPI GLFWcharmodsfun glfwSetCharModsCallback(GLFWwindow* window, GLFWcharmodsfun cbfun);
#define PF_glfwSetCharModsCallback (* (GLFWAPI GLFWcharmodsfun (*)(GLFWwindow*,GLFWcharmodsfun))    _ptr[33].ptr)
//         GLFWAPI GLFWmousebuttonfun glfwSetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun cbfun);
#define PF_glfwSetMouseButtonCallback (* (GLFWAPI GLFWmousebuttonfun (*)(GLFWwindow*,GLFWmousebuttonfun))    _ptr[34].ptr)
//         GLFWAPI GLFWscrollfun glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun cbfun);
#define PF_glfwSetScrollCallback (* (GLFWAPI GLFWscrollfun (*)(GLFWwindow*,GLFWscrollfun))    _ptr[35].ptr)
//         GLFWAPI GLFWdropfun glfwSetDropCallback(GLFWwindow* window, GLFWdropfun cbfun);
#define PF_glfwSetDropCallback (* (GLFWAPI GLFWdropfun (*)(GLFWwindow*,GLFWdropfun))    _ptr[36].ptr)
//         GLFWAPI int glfwJoystickPresent(int joy);
#define PF_glfwJoystickPresent (* (GLFWAPI int (*)(int))                           _ptr[37].ptr)
//         GLFWAPI const float* glfwGetJoystickAxes(int joy, int* count);
#define PF_const (* (GLFWAPI (*)(int,int*))                                        _ptr[38].ptr)
//         GLFWAPI const unsigned char* glfwGetJoystickButtons(int joy, int* count);
#define PF_const (* (GLFWAPI (*)(int,int*))                                        _ptr[39].ptr)
//         GLFWAPI const char* glfwGetJoystickName(int joy);
#define PF_const (* (GLFWAPI (*)(int))                                             _ptr[40].ptr)
//         GLFWAPI GLFWjoystickfun glfwSetJoystickCallback(GLFWjoystickfun cbfun);
#define PF_glfwSetJoystickCallback (* (GLFWAPI GLFWjoystickfun (*)(GLFWjoystickfun))    _ptr[41].ptr)
//         GLFWAPI void glfwSetClipboardString(GLFWwindow* window, const char* string);
#define PF_glfwSetClipboardString (* (GLFWAPI void (*)(GLFWwindow*,const))         _ptr[42].ptr)
//         GLFWAPI const char* glfwGetClipboardString(GLFWwindow* window);
#define PF_const (* (GLFWAPI (*)(GLFWwindow*))                                     _ptr[43].ptr)
//         GLFWAPI double glfwGetTime(void);
#define PF_glfwGetTime (* (GLFWAPI double (*)(void))                               _ptr[44].ptr)
//         GLFWAPI uint64_t glfwGetTimerValue(void);
#define PF_glfwGetTimerValue (* (GLFWAPI uint64_t (*)(void))                       _ptr[45].ptr)
//         GLFWAPI uint64_t glfwGetTimerFrequency(void);
#define PF_glfwGetTimerFrequency (* (GLFWAPI uint64_t (*)(void))                   _ptr[46].ptr)
//         GLFWAPI void glfwSetTime(double time);
#define PF_glfwSetTime (* (GLFWAPI void (*)(double))                               _ptr[47].ptr)
//         GLFWAPI GLFWmonitor** glfwGetMonitors(int* count);
#define PF_glfwGetMonitors (* (GLFWAPI GLFWmonitor** (*)(int*))                    _ptr[48].ptr)
//         GLFWAPI GLFWmonitor* glfwGetPrimaryMonitor(void);
#define PF_glfwGetPrimaryMonitor (* (GLFWAPI GLFWmonitor* (*)(void))               _ptr[49].ptr)
//         GLFWAPI void glfwGetMonitorPos(GLFWmonitor* monitor, int* xpos, int* ypos);
#define PF_glfwGetMonitorPos (* (GLFWAPI void (*)(GLFWmonitor*,int*,int*))         _ptr[50].ptr)
//         GLFWAPI void glfwGetMonitorPhysicalSize(GLFWmonitor* monitor, int* widthMM, int* heightMM);
#define PF_glfwGetMonitorPhysicalSize (* (GLFWAPI void (*)(GLFWmonitor*,int*,int*))    _ptr[51].ptr)
//         GLFWAPI const char* glfwGetMonitorName(GLFWmonitor* monitor);
#define PF_const (* (GLFWAPI (*)(GLFWmonitor*))                                    _ptr[52].ptr)
//         GLFWAPI GLFWmonitorfun glfwSetMonitorCallback(GLFWmonitorfun cbfun);
#define PF_glfwSetMonitorCallback (* (GLFWAPI GLFWmonitorfun (*)(GLFWmonitorfun))    _ptr[53].ptr)
//         GLFWAPI const GLFWvidmode* glfwGetVideoModes(GLFWmonitor* monitor, int* count);
#define PF_const (* (GLFWAPI (*)(GLFWmonitor*,int*))                               _ptr[54].ptr)
//         GLFWAPI const GLFWvidmode* glfwGetVideoMode(GLFWmonitor* monitor);
#define PF_const (* (GLFWAPI (*)(GLFWmonitor*))                                    _ptr[55].ptr)
//         GLFWAPI void glfwSetGamma(GLFWmonitor* monitor, float gamma);
#define PF_glfwSetGamma (* (GLFWAPI void (*)(GLFWmonitor*,float))                  _ptr[56].ptr)
//         GLFWAPI void glfwSetGammaRamp(GLFWmonitor* monitor, const GLFWgammaramp* ramp);
#define PF_glfwSetGammaRamp (* (GLFWAPI void (*)(GLFWmonitor*,const))              _ptr[57].ptr)
//         GLFWAPI const GLFWgammaramp* glfwGetGammaRamp(GLFWmonitor* monitor);
#define PF_const (* (GLFWAPI (*)(GLFWmonitor*))                                    _ptr[58].ptr)
//         GLFWAPI int glfwVulkanSupported(void);
#define PF_glfwVulkanSupported (* (GLFWAPI int (*)(void))                          _ptr[59].ptr)
//         GLFWAPI const char** glfwGetRequiredInstanceExtensions(uint32_t* count);
#define PF_const (* (GLFWAPI (*)(uint32_t*))                                       _ptr[60].ptr)
//         GLFWAPI GLFWvkproc glfwGetInstanceProcAddress(VkInstance instance, const char* procname);
#define PF_glfwGetInstanceProcAddress (* (GLFWAPI GLFWvkproc (*)(VkInstance,const))    _ptr[61].ptr)
//         GLFWAPI int glfwGetPhysicalDevicePresentationSupport(VkInstance instance, VkPhysicalDevice device, uint32_t queuefamily);
#define PF_glfwGetPhysicalDevicePresentationSupport (* (GLFWAPI int (*)(VkInstance,VkPhysicalDevice,uint32_t))    _ptr[62].ptr)
//         GLFWAPI VkResult glfwCreateWindowSurface(VkInstance instance, GLFWwindow* window, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface);
#define PF_glfwCreateWindowSurface (* (GLFWAPI VkResult (*)(VkInstance,GLFWwindow*,const,VkSurfaceKHR*))    _ptr[63].ptr)
//         GLFWAPI GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
#define PF_glfwCreateWindow (* (GLFWAPI GLFWwindow* (*)(int,int,const,GLFWmonitor*,GLFWwindow*))    _ptr[64].ptr)
//         GLFWAPI void glfwWindowHint(int hint, int value);
#define PF_glfwWindowHint (* (GLFWAPI void (*)(int,int))                           _ptr[65].ptr)
//         GLFWAPI int glfwWindowShouldClose(GLFWwindow* window);
#define PF_glfwWindowShouldClose (* (GLFWAPI int (*)(GLFWwindow*))                 _ptr[66].ptr)
//         GLFWAPI void glfwSetWindowShouldClose(GLFWwindow* window, int value);
#define PF_glfwSetWindowShouldClose (* (GLFWAPI void (*)(GLFWwindow*,int))         _ptr[67].ptr)
//         GLFWAPI void glfwSetWindowTitle(GLFWwindow* window, const char* title);
#define PF_glfwSetWindowTitle (* (GLFWAPI void (*)(GLFWwindow*,const))             _ptr[68].ptr)
//         GLFWAPI void glfwSetWindowIcon(GLFWwindow* window, int count, const GLFWimage* images);
#define PF_glfwSetWindowIcon (* (GLFWAPI void (*)(GLFWwindow*,int,const))          _ptr[69].ptr)
//         GLFWAPI GLFWwindowiconifyfun glfwSetWindowIconifyCallback(GLFWwindow* window, GLFWwindowiconifyfun cbfun);
#define PF_glfwSetWindowIconifyCallback (* (GLFWAPI GLFWwindowiconifyfun (*)(GLFWwindow*,GLFWwindowiconifyfun))    _ptr[70].ptr)
//         GLFWAPI void glfwGetWindowPos(GLFWwindow* window, int* xpos, int* ypos);
#define PF_glfwGetWindowPos (* (GLFWAPI void (*)(GLFWwindow*,int*,int*))           _ptr[71].ptr)
//         GLFWAPI void glfwSetWindowPos(GLFWwindow* window, int xpos, int ypos);
#define PF_glfwSetWindowPos (* (GLFWAPI void (*)(GLFWwindow*,int,int))             _ptr[72].ptr)
//         GLFWAPI GLFWwindowposfun glfwSetWindowPosCallback(GLFWwindow* window, GLFWwindowposfun cbfun);
#define PF_glfwSetWindowPosCallback (* (GLFWAPI GLFWwindowposfun (*)(GLFWwindow*,GLFWwindowposfun))    _ptr[73].ptr)
//         GLFWAPI void glfwGetWindowSize(GLFWwindow* window, int* width, int* height);
#define PF_glfwGetWindowSize (* (GLFWAPI void (*)(GLFWwindow*,int*,int*))          _ptr[74].ptr)
//         GLFWAPI void glfwSetWindowSizeLimits(GLFWwindow* window, int minwidth, int minheight, int maxwidth, int maxheight);
#define PF_glfwSetWindowSizeLimits (* (GLFWAPI void (*)(GLFWwindow*,int,int,int,int))    _ptr[75].ptr)
//         GLFWAPI void glfwSetWindowSize(GLFWwindow* window, int width, int height);
#define PF_glfwSetWindowSize (* (GLFWAPI void (*)(GLFWwindow*,int,int))            _ptr[76].ptr)
//         GLFWAPI GLFWwindowsizefun glfwSetWindowSizeCallback(GLFWwindow* window, GLFWwindowsizefun cbfun);
#define PF_glfwSetWindowSizeCallback (* (GLFWAPI GLFWwindowsizefun (*)(GLFWwindow*,GLFWwindowsizefun))    _ptr[77].ptr)
//         GLFWAPI void glfwSetWindowAspectRatio(GLFWwindow* window, int numer, int denom);
#define PF_glfwSetWindowAspectRatio (* (GLFWAPI void (*)(GLFWwindow*,int,int))     _ptr[78].ptr)
//         GLFWAPI void glfwGetFramebufferSize(GLFWwindow* window, int* width, int* height);
#define PF_glfwGetFramebufferSize (* (GLFWAPI void (*)(GLFWwindow*,int*,int*))     _ptr[79].ptr)
//         GLFWAPI void glfwGetWindowFrameSize(GLFWwindow* window, int* left, int* top, int* right, int* bottom);
#define PF_glfwGetWindowFrameSize (* (GLFWAPI void (*)(GLFWwindow*,int*,int*,int*,int*))    _ptr[80].ptr)
//         GLFWAPI void glfwIconifyWindow(GLFWwindow* window);
#define PF_glfwIconifyWindow (* (GLFWAPI void (*)(GLFWwindow*))                    _ptr[81].ptr)
//         GLFWAPI void glfwRestoreWindow(GLFWwindow* window);
#define PF_glfwRestoreWindow (* (GLFWAPI void (*)(GLFWwindow*))                    _ptr[82].ptr)
//         GLFWAPI void glfwMaximizeWindow(GLFWwindow* window);
#define PF_glfwMaximizeWindow (* (GLFWAPI void (*)(GLFWwindow*))                   _ptr[83].ptr)
//         GLFWAPI void glfwShowWindow(GLFWwindow* window);
#define PF_glfwShowWindow (* (GLFWAPI void (*)(GLFWwindow*))                       _ptr[84].ptr)
//         GLFWAPI void glfwHideWindow(GLFWwindow* window);
#define PF_glfwHideWindow (* (GLFWAPI void (*)(GLFWwindow*))                       _ptr[85].ptr)
//         GLFWAPI void glfwFocusWindow(GLFWwindow* window);
#define PF_glfwFocusWindow (* (GLFWAPI void (*)(GLFWwindow*))                      _ptr[86].ptr)
//         GLFWAPI int glfwGetWindowAttrib(GLFWwindow* window, int attrib);
#define PF_glfwGetWindowAttrib (* (GLFWAPI int (*)(GLFWwindow*,int))               _ptr[87].ptr)
//         GLFWAPI GLFWmonitor* glfwGetWindowMonitor(GLFWwindow* window);
#define PF_glfwGetWindowMonitor (* (GLFWAPI GLFWmonitor* (*)(GLFWwindow*))         _ptr[88].ptr)
//         GLFWAPI void glfwSetWindowMonitor(GLFWwindow* window, GLFWmonitor* monitor, int xpos, int ypos, int width, int height, int refreshRate);
#define PF_glfwSetWindowMonitor (* (GLFWAPI void (*)(GLFWwindow*,GLFWmonitor*,int,int,int,int,int))    _ptr[89].ptr)
//         GLFWAPI void glfwSetWindowUserPointer(GLFWwindow* window, void* pointer);
#define PF_glfwSetWindowUserPointer (* (GLFWAPI void (*)(GLFWwindow*,void*))       _ptr[90].ptr)
//         GLFWAPI void* glfwGetWindowUserPointer(GLFWwindow* window);
#define PF_glfwGetWindowUserPointer (* (GLFWAPI void* (*)(GLFWwindow*))            _ptr[91].ptr)
//         GLFWAPI GLFWwindowclosefun glfwSetWindowCloseCallback(GLFWwindow* window, GLFWwindowclosefun cbfun);
#define PF_glfwSetWindowCloseCallback (* (GLFWAPI GLFWwindowclosefun (*)(GLFWwindow*,GLFWwindowclosefun))    _ptr[92].ptr)
//         GLFWAPI GLFWwindowrefreshfun glfwSetWindowRefreshCallback(GLFWwindow* window, GLFWwindowrefreshfun cbfun);
#define PF_glfwSetWindowRefreshCallback (* (GLFWAPI GLFWwindowrefreshfun (*)(GLFWwindow*,GLFWwindowrefreshfun))    _ptr[93].ptr)
//         GLFWAPI GLFWwindowfocusfun glfwSetWindowFocusCallback(GLFWwindow* window, GLFWwindowfocusfun cbfun);
#define PF_glfwSetWindowFocusCallback (* (GLFWAPI GLFWwindowfocusfun (*)(GLFWwindow*,GLFWwindowfocusfun))    _ptr[94].ptr)
//         *  [window focus callback](@ref glfwSetWindowFocusCallback) has been called.
#define PF_[window (* (* (*)(@ref))                                                _ptr[95].ptr)
//         *  [window focus callback](@ref glfwSetWindowFocusCallback) has been called.
#define PF_[window (* (* (*)(@ref))                                                _ptr[96].ptr)
//         GLFWAPI GLFWframebuffersizefun glfwSetFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun cbfun);
#define PF_glfwSetFramebufferSizeCallback (* (GLFWAPI GLFWframebuffersizefun (*)(GLFWwindow*,GLFWframebuffersizefun))    _ptr[97].ptr)
//         GLFWAPI void glfwPollEvents(void);
#define PF_glfwPollEvents (* (GLFWAPI void (*)(void))                              _ptr[98].ptr)
//         GLFWAPI void glfwWaitEvents(void);
#define PF_glfwWaitEvents (* (GLFWAPI void (*)(void))                              _ptr[99].ptr)
//         GLFWAPI void glfwWaitEventsTimeout(double timeout);
#define PF_glfwWaitEventsTimeout (* (GLFWAPI void (*)(double))                     _ptr[100].ptr)
//         GLFWAPI void glfwPostEmptyEvent(void);
#define PF_glfwPostEmptyEvent (* (GLFWAPI void (*)(void))                          _ptr[101].ptr)
//         *  (minimized) or maximized.  If the window is already restored, this function
#define PF_ (* (* (*)(minimized,this))                                             _ptr[102].ptr)


// -------------- all funcs array ---------------
struct FUNCS_ {
  const char *name;
  void  (*ptr)(void);
};

inline const FUNCS_* load()
{
    union
    {
    void *p;
    void (*fp)(void);
    } u;

    static struct FUNCS_ _funcs[] ={
        {"glfwGetProcAddress", nullptr},
        {"glfwExtensionSupported", nullptr},
        {"glfwMakeContextCurrent", nullptr},
        {"glfwGetCurrentContext", nullptr},
        {"glfwSwapBuffers", nullptr},
        {"glfwSwapInterval", nullptr},
        {"glfwInit", nullptr},
        {"glfwDefaultWindowHints", nullptr},
        {"glfwTerminate", nullptr},
        {"glfwDestroyWindow", nullptr},
        {"glfwDestroyCursor", nullptr},
        {"glfwGetVersion", nullptr},
        {"const", nullptr},
        {"glfwSetErrorCallback", nullptr},
        {"glfwGetInputMode", nullptr},
        {"glfwSetInputMode", nullptr},
        {"const", nullptr},
        {"glfwGetKey", nullptr},
        {"glfwGetMouseButton", nullptr},
        {"glfwGetCursorPos", nullptr},
        {"glfwSetCursorPos", nullptr},
        {"glfwSetCursorPosCallback", nullptr},
        {"glfwCreateCursor", nullptr},
        {"glfwCreateStandardCursor", nullptr},
        {"glfwSetCursor", nullptr},
        {"glfwSetCursorEnterCallback", nullptr},
        {"glfwSetKeyCallback", nullptr},
        {"callback]", nullptr},
        {"callback]", nullptr},
        {"callback]", nullptr},
        {"glfwSetCharCallback", nullptr},
        {"callback]", nullptr},
        {"[character", nullptr},
        {"glfwSetCharModsCallback", nullptr},
        {"glfwSetMouseButtonCallback", nullptr},
        {"glfwSetScrollCallback", nullptr},
        {"glfwSetDropCallback", nullptr},
        {"glfwJoystickPresent", nullptr},
        {"const", nullptr},
        {"const", nullptr},
        {"const", nullptr},
        {"glfwSetJoystickCallback", nullptr},
        {"glfwSetClipboardString", nullptr},
        {"const", nullptr},
        {"glfwGetTime", nullptr},
        {"glfwGetTimerValue", nullptr},
        {"glfwGetTimerFrequency", nullptr},
        {"glfwSetTime", nullptr},
        {"glfwGetMonitors", nullptr},
        {"glfwGetPrimaryMonitor", nullptr},
        {"glfwGetMonitorPos", nullptr},
        {"glfwGetMonitorPhysicalSize", nullptr},
        {"const", nullptr},
        {"glfwSetMonitorCallback", nullptr},
        {"const", nullptr},
        {"const", nullptr},
        {"glfwSetGamma", nullptr},
        {"glfwSetGammaRamp", nullptr},
        {"const", nullptr},
        {"glfwVulkanSupported", nullptr},
        {"const", nullptr},
        {"glfwGetInstanceProcAddress", nullptr},
        {"glfwGetPhysicalDevicePresentationSupport", nullptr},
        {"glfwCreateWindowSurface", nullptr},
        {"glfwCreateWindow", nullptr},
        {"glfwWindowHint", nullptr},
        {"glfwWindowShouldClose", nullptr},
        {"glfwSetWindowShouldClose", nullptr},
        {"glfwSetWindowTitle", nullptr},
        {"glfwSetWindowIcon", nullptr},
        {"glfwSetWindowIconifyCallback", nullptr},
        {"glfwGetWindowPos", nullptr},
        {"glfwSetWindowPos", nullptr},
        {"glfwSetWindowPosCallback", nullptr},
        {"glfwGetWindowSize", nullptr},
        {"glfwSetWindowSizeLimits", nullptr},
        {"glfwSetWindowSize", nullptr},
        {"glfwSetWindowSizeCallback", nullptr},
        {"glfwSetWindowAspectRatio", nullptr},
        {"glfwGetFramebufferSize", nullptr},
        {"glfwGetWindowFrameSize", nullptr},
        {"glfwIconifyWindow", nullptr},
        {"glfwRestoreWindow", nullptr},
        {"glfwMaximizeWindow", nullptr},
        {"glfwShowWindow", nullptr},
        {"glfwHideWindow", nullptr},
        {"glfwFocusWindow", nullptr},
        {"glfwGetWindowAttrib", nullptr},
        {"glfwGetWindowMonitor", nullptr},
        {"glfwSetWindowMonitor", nullptr},
        {"glfwSetWindowUserPointer", nullptr},
        {"glfwGetWindowUserPointer", nullptr},
        {"glfwSetWindowCloseCallback", nullptr},
        {"glfwSetWindowRefreshCallback", nullptr},
        {"glfwSetWindowFocusCallback", nullptr},
        {"[window", nullptr},
        {"[window", nullptr},
        {"glfwSetFramebufferSizeCallback", nullptr},
        {"glfwPollEvents", nullptr},
        {"glfwWaitEvents", nullptr},
        {"glfwWaitEventsTimeout", nullptr},
        {"glfwPostEmptyEvent", nullptr},
        {nullptr, nullptr}
    };
    void  *dll_handle;
    if ((dll_handle = dlopen("/usr/local/lib/libglfw3.a", RTLD_LAZY)) == 0)
    {
        perror("cannot load:");
        return nullptr;
    }
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
            fp->ptr = u.fp;
        }
    }
    return _funcs;
}
#endif // LIB_RESOLVER
/*
 add this to cpp 
const FUNCS_* _ptr; // global var
  _ptr = load(); // in main() 
*/
 
