#ifndef __GLOBAL_VALUE__
#define __GLOBAL_VALUE__
#include "main.h"
#include <pthread.h>

#ifdef GLOBAL_VALUE
#define GLOBAL_EXT
#else
#define GLOBAL_EXT extern
#endif

GLOBAL_EXT volatile int sleep_down;
GLOBAL_EXT volatile unsigned int time_t_keep;
GLOBAL_EXT volatile unsigned int record_ctl;
GLOBAL_EXT volatile unsigned int photo_ctl;
GLOBAL_EXT volatile unsigned int break_flag;
GLOBAL_EXT volatile unsigned int file_create;
GLOBAL_EXT volatile unsigned int key;
GLOBAL_EXT volatile unsigned int record_ctl_file_ok; 
GLOBAL_EXT volatile unsigned int video_ctl; 
GLOBAL_EXT pthread_mutex_t video_ctl_mutex;




#endif