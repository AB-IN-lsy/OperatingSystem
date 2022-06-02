#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
int *count = 0;
sem_t *mutex1 = NULL;
sem_t *mutex2 = NULL;
int finish = 0;
int main()
{
    mutex1 = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    mutex2 = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    count = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    sem_init(mutex1, 1, 1);
    sem_init(mutex2, 1, 0);
    *count = 0;
    if (fork() == 0)
    {
        while (1)
        {
            printf("child continue!\n");
            sem_wait(mutex1);
            *count += 2;
            printf("child:%d\n", *count);
            finish++;
            sem_post(mutex2);
            if (finish == 10)
                break;
        }
        printf("child Finish!\n");
        exit(0);
    }

    while (1)
    {
        printf("father continue!\n");
        sem_wait(mutex2);
        *count -= 1;
        printf("father:%d\n", *count);
        finish++;
        sem_post(mutex1);
        if (finish == 10)
            break;
    }
    sem_destroy(mutex1);
    sem_destroy(mutex2);
    munmap(mutex1, sizeof(sem_t));
    munmap(mutex2, sizeof(sem_t));
    munmap(count, sizeof(int));
    printf("Finished All!\n");
    return 0;
}

