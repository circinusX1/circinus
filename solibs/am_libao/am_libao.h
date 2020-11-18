#ifndef LIBR_RESOLVER_H
#define LIBR_RESOLVER_H
// Copyright Marius C. https://github/comarius (do not remove)
#include <cstdint>
#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>


// -------------- constants ---------------
// review before compile...
#define __AO_H__
#define AO_TYPE_LIVE 1
#define AO_TYPE_FILE 2
#define AO_ENODRIVER   1
#define AO_ENOTFILE    2
#define AO_ENOTLIVE    3
#define AO_EBADOPTION  4
#define AO_EOPENDEVICE 5
#define AO_EOPENFILE   6
#define AO_EFILEEXISTS 7
#define AO_EBADFORMAT  8
#define AO_EFAIL       100
#define AO_FMT_LITTLE 1
#define AO_FMT_BIG    2
#define AO_FMT_NATIVE 4

struct ao_option;
typedef struct ao_option{
  char *key;
  char *value;
  struct ao_option *next; /* number of audio channels */
} ao_option;


typedef struct {
  int  type; /* live output or file output? */
  char *name; /* full name of driver */
  char *short_name; /* short name of driver */
  char *comment; /* driver comment */
  int  preferred_byte_format;
  int  priority;
  char **options;
  int  option_count;
} ao_info;

typedef struct {
  int  bits; /* bits per sample */
  int  rate; /* samples per second (in a single channel) */
  int  channels; /* number of audio channels */
  int  byte_format; /* Byte ordering in sample, see constants below */
  char *matrix; /* channel input matrix */
} ao_sample_format;

struct ao_functions;
typedef struct {
  int  type; /* live output or file output? */
  int  driver_id;
  ao_functions *funcs;
  FILE *file; /* File for output if this is a file driver */
  int  client_byte_format;
  int  machine_byte_format;
  int  driver_byte_format;
  char *swap_buffer;
  int  swap_buffer_size; /* Bytes allocated to swap_buffer */
  void *internal; /* Pointer to driver-specific data */
} ao_device;

struct ao_functions {
	   int (*test)(void);
	   ao_info* (*driver_info)(void);
	   int (*device_init)(ao_device *device);
	   int (*set_option)(ao_device *device, const char *key,
						 const char *value);
	   int (*open)(ao_device *device, ao_sample_format *format);
	   int (*play)(ao_device *device, const char *output_samples,
						  uint32_t num_bytes);
	   int (*close)(ao_device *device);
	   void (*device_clear)(ao_device *device);
	   char* (*file_extension)(void);
};




// -------------- functions ---------------
// review before compile...

//int   ao_append_global_option(const char *key,const char *value);
#define PF_ao_append_global_option (* (int (*)(const char *key ,const char *value ))    _ptr[0].ptr)

//int          ao_append_option(ao_option **options,const char *key,const char *value);
#define PF_ao_append_option (* (int (*)(ao_option **options ,const char *key ,const char *value ))    _ptr[1].ptr)

//int                  ao_close(ao_device *device);
#define PF_ao_close (* (int (*)(ao_device *device ))                               _ptr[2].ptr)

//int      ao_default_driver_id(void);
#define PF_ao_default_driver_id (* (int (*)(void ))                                _ptr[3].ptr)

//int              ao_driver_id(const char *short_name);
#define PF_ao_driver_id (* (int (*)(const char *short_name ))                      _ptr[4].ptr)

//ao_info       *ao_driver_info(int driver_id);
#define PF_ao_driver_info (* (ao_info (*)(int driver_id ))                         _ptr[5].ptr)

//ao_info **ao_driver_info_list(int *driver_count);
#define PF_ao_driver_info_list (* (ao_info (*)(int *driver_count ))                _ptr[6].ptr)

//const char *ao_file_extension(int driver_id);
#define PF_ao_file_extension (* (const char (*)(int driver_id ))                   _ptr[7].ptr)

//void          ao_free_options(ao_option *options);
#define PF_ao_free_options (* (void (*)(ao_option *options ))                      _ptr[8].ptr)

//void ao_initialize(void);
#define PF_ao_initialize (* (void (*)(void ))                                      _ptr[9].ptr)

//int          ao_is_big_endian(void);
#define PF_ao_is_big_endian (* (int (*)(void ))                                    _ptr[10].ptr)

//ao_device*       ao_open_file(int driver_id,const char *filename,int overwrite,ao_sample_format *format,ao_option *option);
#define PF_ao_open_file (* (ao_device* (*)(int driver_id ,const char *filename ,int overwrite ,ao_sample_format *format ,ao_option *option ))    _ptr[11].ptr)

//ao_device*       ao_open_live(int driver_id,ao_sample_format *format,ao_option *option);
#define PF_ao_open_live (* (ao_device* (*)(int driver_id ,ao_sample_format *format ,ao_option *option ))    _ptr[12].ptr)

//int                   ao_play(ao_device *device,char *output_samples,uint32_t num_bytes);
#define PF_ao_play (* (int (*)(ao_device *device ,char *output_samples ,uint32_t num_bytes ))    _ptr[13].ptr)

//void ao_shutdown(void);
#define PF_ao_shutdown (* (void (*)(void ))                                        _ptr[14].ptr)


// -------------- all funcs array ---------------
struct FUNCS_ {
  const char *name;
  void  (*ptr)(void);
};

inline const FUNCS_* load(void** dll)
{
    union
    {
    void *p;
    void (*fp)(void);
    } u;

    static struct FUNCS_ _funcs[] ={
        {"ao_append_global_option", nullptr},
        {"ao_append_option", nullptr},
        {"ao_close", nullptr},
        {"ao_default_driver_id", nullptr},
        {"ao_driver_id", nullptr},
        {"ao_driver_info", nullptr},
        {"ao_driver_info_list", nullptr},
        {"ao_file_extension", nullptr},
        {"ao_free_options", nullptr},
        {"ao_initialize", nullptr},
        {"ao_is_big_endian", nullptr},
        {"ao_open_file", nullptr},
        {"ao_open_live", nullptr},
        {"ao_play", nullptr},
        {"ao_shutdown", nullptr},
        {nullptr, nullptr}
    };
    //void  *dll_handle;
    if ((*dll = dlopen("/usr/lib/x86_64-linux-gnu/libao.so", RTLD_LAZY)) == 0)
    {
        perror("cannot load:");
        return nullptr;
    }
    struct FUNCS_ *fp = _funcs;
    for (; fp->name != nullptr; fp++)
    {
        u.p = dlsym(*dll, fp->name);
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
