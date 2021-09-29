#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "membt.h"

void *mymalloc(int len)
{
    void *addr = malloc(len);
    record_bt(addr, len);
    return addr;
}

void myfree(void *addr)
{
    unrecord_bt(addr);
    free(addr);
}

void test3()
{
    //print_backtrace();

    int len = 20;
    void *addr = mymalloc(len);

    sleep(60);

    myfree(addr);
}

void test2()
{
    test3();
}

static void *test1(void *arg)
{
    // set thread name
    char buf[20];
    snprintf(buf, sizeof(buf), "thread%d", *(int *)arg);
    pthread_setname_np(pthread_self(), buf);

    test2();

    while(1){;}

    return NULL;
}

int main(void)
{
    int i;

    init_bt();

    // create 5 thread to monitor the backtrace status
    for (i = 0; i < 5; i++) {
        pthread_t tid;
        if (pthread_create(&tid, NULL, test1, &i) !=0) {
            perror("Failed to create pthread init_hostpad_ct()\n");
            exit(-1);
        }

        pthread_detach(tid);

        sleep(1);
    }

    sleep(80);

    return 0;
}
