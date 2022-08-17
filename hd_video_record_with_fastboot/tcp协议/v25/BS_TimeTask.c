#include "lib/BS_FunInterface.h"

static bool flag = true;
timer_t timeid;

void Task(int i)
{
    printf("task start\n");
    flag = false;
}

void CreateTimer()
{
    /*创建定时器*/
    /*环境结构体*/
    struct sigevent evp;
    int ret = 0;

    /*参数全置0*/
    memset(&evp, 0, sizeof(struct sigevent));

    /*设置环境结构体参数*/
    /*绑定定时器*/
    /*设置定时器到期后触发的行为是为发送信号*/
    /*设置信号为用户自定义信号1*/
    /*绑定产生信号后调用度函数*/
    evp.sigev_value.sival_ptr = &timeid;
    evp.sigev_notify = SIGEV_SIGNAL;
    evp.sigev_signo = SIGUSR1;
    signal(SIGUSR1, Task);

    /*创建定时器*/
    /*返回0成功，-1失败*/
    ret = timer_create(CLOCK_REALTIME, &evp, &timeid);

    if (ret == 0)
    {
        printf("timer_create ok\n");
    }

}

void InitTimer()
{
    int ret = 0;

    /*启动器结构体*/
    struct itimerspec ts;

    /*定时器启动后定时周期为1s，0纳秒*/
    ts.it_interval.tv_sec = 1;
    ts.it_interval.tv_nsec = 0;

    /*过多久之后开始启动定时器，10s，0纳秒*/
    ts.it_value.tv_sec = 1;
    ts.it_value.tv_nsec = 0;

    /*根据设的启动器参数，初始化定时器*/
    /*timeid-定时器的id，指定初始化度定时器，由timer_create产生*/
    /*0：表示使用的相对时间，可理解为正常系统时间*/
    /*ts：设置定时周期和启动周期*/
    /*NULL: 通常这么设就可以了*/
    ret = timer_settime(timeid, 0, &ts, NULL);

    if (ret == 0)
    {
        printf("timer_settime ok\n");
    }
}
