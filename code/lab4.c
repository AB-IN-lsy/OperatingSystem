/*
 * @Author: NEFU AB-IN
 * @Date: 2022-06-02 16:11:44
 * @LastEditTime: 2022-06-02 16:37:08
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define PRODUCER_NUM 5 //生产者数目
#define CONSUMER_NUM 5 //消费者数目
#define POOL_SIZE 11   //缓冲池大小

int pool[POOL_SIZE];   //缓冲区
int head = 0;          //缓冲池读取指针，即in
int rear = 0;          //缓冲池写入指针，即out
sem_t room_sem;        //同步信号量，表示缓冲区有可用，即s1
sem_t product_sem;     //同步信号量，表示缓冲区有可用，即s2
pthread_mutex_t mutex; //互斥信号量，即w1

void *producer_fun(void *arg)
{
    while (1)
    {
        sleep(1);
        sem_wait(&room_sem);
        pthread_mutex_lock(&mutex);
        //生产者往缓冲池中写入数据
        pool[rear] = 1;
        //指针移动，相当于模拟队列
        rear = (rear + 1) % POOL_SIZE;
        printf("producer %d write to pool\n", (int)arg);
        printf("pool size is %d\n", (rear - head + POOL_SIZE) % POOL_SIZE);
        pthread_mutex_unlock(&mutex);
        sem_post(&product_sem);
    }
}

void *consumer_fun(void *arg)
{
    while (1)
    {
        int data;
        sleep(1);
        sem_wait(&product_sem);
        pthread_mutex_lock(&mutex);
        //消费者从缓冲池读取数据
        data = pool[head];
        head = (head + 1) % POOL_SIZE;
        printf("consumer %d read from pool\n", (int)arg);
        printf("pool size is %d\n", (rear - head + POOL_SIZE) % POOL_SIZE);
        pthread_mutex_unlock(&mutex);
        sem_post(&room_sem);
    }
}

int main()
{
    pthread_t producer_id[PRODUCER_NUM];
    pthread_t consumer_id[CONSUMER_NUM];
    pthread_mutex_init(&mutex, NULL);                //初始化互斥量，默认的互斥锁属性
    int ret = sem_init(&room_sem, 0, POOL_SIZE - 1); //初始化信号量room_sem为缓冲池大小
    if (ret != 0)
    {
        printf("sem_init error");
        exit(0);
    }
    ret = sem_init(&product_sem, 0, 0);              //初始化信号量product_sem为0，开始时缓冲池中没有数据
    if (ret != 0)
    {
        printf("sem_init error");
        exit(0);
    }
    for (int i = 0; i < PRODUCER_NUM; i++)
    {
        //创建生产者线程
        ret = pthread_create(&producer_id[i], NULL, producer_fun, (void *)i);
        if (ret != 0)
        {
            printf("producer_id error");
            exit(0);
        }
        //创建消费者线程
        ret = pthread_create(&consumer_id[i], NULL, consumer_fun, (void *)i);
        if (ret != 0)
        {
            printf("consumer_id error");
            exit(0);
        }
    }
    for (int i = 0; i < PRODUCER_NUM; i++)
    {
        pthread_join(producer_id[i], NULL);
        pthread_join(consumer_id[i], NULL);
    }
    exit(0);
}

