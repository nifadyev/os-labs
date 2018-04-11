#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void (*p)(int);

void signalHandler(int signalNumber)
{
    if (signalNumber == SIGINT)
    {
        int n, i, pid;
        char val[10];
        srand(time(NULL));
        n = rand() % 10 + 1;

        for (i = 0; i < n; i++)
        {
            val[i] = rand() % ('z' - 'a') + 'a';
        }
        val[i + 1] = '\0';
        if (setenv("PARAM1", val, 1) == -1)
        {
            printf("Not enough memory p1");
            exit(-1);
        }
        n = rand() % 10 + 1;
        for (i = 0; i < n; i++)
        {
            val[i] = rand() % ('z' - 'a') + 'a';
        }
        val[i + 1] = '\0';
        if (setenv("PARAM2", val, 1) == -1)
        {
            printf("Not enough memory p2");
            exit(-1);
        }
        pid = fork();
        if (pid == -1)
        {
            printf("Error! Cannot create process\n");
            exit(EXIT_FAILURE);
        }
        if (pid  == 0)
        {
            if (execlp("../Documents/lab72", "lab72", NULL) == -1)
            {
                printf("Not exec");
                exit(-1);
            }
        }
        if (pid > 0)
        {

            printf("\nPARAM1=%s, PARAM2=%s\n", getenv("PARAM1"), getenv("PARAM2"));
            exit(1);
        }
    }
    if (signalNumber == SIGQUIT)
    {
        printf("Good\n");
        exit(EXIT_SUCCESS);
    }
}
int main()
{
    /* p =  */ (void)signal(SIGINT, signalHandler);
    (void)signal(SIGQUIT, signalHandler);
    while (1)
        ;
    return 0;
}
