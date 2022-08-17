#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "video_record_with_fastboot.h"
#include "tcp_thread.h"
#include <sys/time.h>
#include "uart_thread.h"
#include "ini.h"

pthread_t uart_thread, tcp_thread, main_thread;

void Init(void)
{
    creat_CamPIR_ini();
    sleep_down = 1;
    pthread_mutex_init(&video_ctl_mutex , NULL);

}

void Fastboot_Video_Pic_Get(void)
{
    VideoCaptureMain();

}

void Uart_SleepDown_Send(void)
{

    SleepSet();
  
}

void *TCP_Thread(void)
{
    
    printf("first tcp thread \n");
    //sleep(10)
    
    
    TCP_Main();
    
    {
        break_flag = 1;
        key = 'q'; //video thread out key
    }
    
    printf("TCP MAIN OUT \n");
    return NULL;
}

void *Test_Thread(void)
{
    printf("first uart thread \n");
    
    struct timeval t1;
    while(1)
    {
        t1.tv_sec = 10;
        int err;
        do {
            err = select(0, NULL, NULL, NULL, &t1);
        } while(err<0);

        pthread_mutex_lock(&video_ctl_mutex); 
        record_ctl =1;
        pthread_mutex_unlock(&video_ctl_mutex); 

        if (break_flag == 1)
            break;

        printf("uart continus\n");
    }
    printf("UART MAIN OUT \n");
    return NULL;
}

int uart_sleep_back = 0;
void *Uart_Thread(void)
{
    printf("first uart thread \n");
    
    
    uart_sleep_back = Uart_Main();

    printf("UART MAIN OUT \n");
    return NULL;
}

void *Main_Thread(void)
{
    Fastboot_Video_Pic_Get();

    printf("MAIN THREAD OUT\n");
    
    return NULL;
}

int main(void)
{
    Init();
    
    printf("\n----------------------------\nwelcome video capture world!\n-------------------------\n");

    int ret;
    ret = pthread_create(&tcp_thread, NULL, (void*)TCP_Thread, NULL);
    if (ret != 0)
    {
        perror("creat tcp thread failed!\n");
        return -1;
    }
    ret = pthread_create(&uart_thread, NULL, (void*)Uart_Thread, NULL);
    if (ret != 0)
    {
        perror("creat uart thread failed!\n");
        return -1;
    }
    ret = pthread_create(&main_thread, NULL, (void*)Main_Thread, NULL);
    if (ret != 0)
    {
        perror("creat main thread failed!\n");
        return -1;
    }

    
    pthread_join(tcp_thread, NULL);
    pthread_join(main_thread, NULL);

    //Uart_SleepDown_Send();
    struct timeval t1;
    t1.tv_sec = 5;
    while (uart_sleep_back != 1)
    {
        Uart_SleepDown_Send();
        int err;
        do {
            err = select(0, NULL, NULL, NULL, &t1);
            printf("select err:%d\n", err);
        } while(err<0);

        printf("sleep signal send!\n");
    }
    pthread_join(uart_thread, NULL);

    return 1;

}
