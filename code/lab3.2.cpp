#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int c = 5;

sem_t mux1;
sem_t mux2;

// 线程一
void *thread1(void *arg)
{
    for (int i = 0; i < 10; i++)
    {
        sem_wait(&mux1);
        c += 5;
        sem_post(&mux2);
    }
}

// 线程二
void *thread2(void *arg)
{
    for (int i = 0; i < 10; i++)
    {
        sem_wait(&mux2);
        printf("c is %d\n", c);
        sem_post(&mux1);
    }
}

int main()
{
    pthread_t th1;
    pthread_t th2;
    int ret = sem_init(&mux1, 0, 1);
    if (ret != 0)
    {
        printf("sem_init error");
        exit(0);
    }
    ret = sem_init(&mux2, 0, 0);
    if (ret != 0)
    {
        printf("sem_init error");
        exit(0);
    }
    pthread_create(&th1, NULL, thread1, NULL);
    pthread_create(&th2, NULL, thread2, NULL);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    exit(0);
}

