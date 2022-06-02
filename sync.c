#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
char *ptype = NULL;
int main()
{
    int pid = 0, i = 0;
    int type = -1; // 1 tiger   2 rabbit

    ptype = (char *)mmap(NULL, sizeof(char) * 32, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if (ptype == NULL)
    {
        printf("mmap error!\n");
        return 0;
    }
    pid = fork();
    if (pid == 0)
    {
        for (i = 0; i < 10; i++)
            printf("i am cooker ,i get %s\n", ptype);

        exit(0);
    }
    else
    {
        pid = fork();
        if (pid == 0)
        {
            for (i = 0; i < 10; i++)
                printf("i am zoo mannager, i get %s\n", ptype);

            exit(0);
        }
        srand(1);
        for (i = 0; i < 10; i++)
        {
            if (rand() % 2)
                strcpy(ptype, "tiger");
            else
                strcpy(ptype, "rabbit");
            printf("i am hunter ,i put %s\n", ptype);
        }

        sleep(20);
        munmap(ptype, sizeof(char) * 32);
    }
    return 0;
}
