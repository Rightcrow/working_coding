#define GLOBAL_VALUE
#include "global_value.h"


volatile int sleep_down = 0;        //send sleep signal 
volatile unsigned int time_t_keep = 1;
volatile unsigned int record_ctl = 1;
volatile unsigned int break_flag = 0;
volatile unsigned int file_create = 1;
volatile unsigned int key = 0;
volatile unsigned int record_ctl_file_ok = 0;
volatile unsigned int video_ctl = 0; 
volatile unsigned int photo_ctl = 0; 
pthread_mutex_t video_ctl_mutex;