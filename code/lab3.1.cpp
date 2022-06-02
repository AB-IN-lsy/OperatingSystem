#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
int *count = 0;
sem_t *mutex1 = NULL;
sem_t *mutex2 = NULL;


int main()
{
    mutex1 = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    mutex2 = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    count = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    sem_init(mutex1, 1, 1);
    sem_init(mutex2, 1, 0);
    *count = 5;
    if (fork() == 0) // 子进程 
    {
        for (int i = 0; i < 10; i++)
        {
            sem_wait(mutex1);
            *count += 5;
            printf("child:%d\n", *count);

            sem_post(mutex2);
        }
        exit(0);
    }
    // 父进程
    for (int i = 0; i < 10; i++)
    {
        sem_wait(mutex2);
        printf("father:%d\n", *count);
        sem_post(mutex1);
    }

    sem_destroy(mutex1);
    sem_destroy(mutex2);
    munmap(mutex1, sizeof(sem_t));
    munmap(mutex2, sizeof(sem_t));
    munmap(count, sizeof(int));
    return 0;
}
