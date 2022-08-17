#include "lib/BS_FunInterface.h"


int* thread(void* arg)
{
    pthread_t new_thid;

    new_thid = pthread_self();

    printf("this is new thread, thread ID = %d\n", new_thid);

    return NULL;
}

int main()
{
    printf("ready to start\n");

    pthread_t thid;

    printf("main thread, ID is %d\n", pthread_self());

    if (pthread_create(&thid, NULL, (void*)thread, NULL) != 0)
    {
        printf("thread creation failed\n");
        exit(1);
    }

    pthread_exit(thid);
    sleep(1);
    exit(0);

}