#include <stdio.h>
#include <signal.h>
//#include <sys/types.h>
//#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Handling the signals
void signalHandler(int signalNumber)
{
    // SIGINT = Ctrl + C (in terminal)
    if (signalNumber == SIGINT)
    {
        int randomSize, PID, i;
        char environmentVariable[10];
        srand(time(NULL));

        // Filling environment variable PARAM1 with random characters
        randomSize = rand() % 10 + 1;
        for (i = 0; i < randomSize; i++)
        {
            environmentVariable[i] = rand() % ('z' - 'a') + 'a';
        }
        environmentVariable[i + 1] = '\0';

        // Setting environment variable PARAM1 with argument 1
        // (if name does exist in the environment,then its value is changed to value if argument is nonzero)
        if (setenv("PARAM1", environmentVariable, 1) == -1)
        {
            printf("Error! Cannot Set environment variable\n");
            exit(EXIT_FAILURE);
        }

        // Filling environment variable PARAM2 with random characters
        randomSize = rand() % 10 + 1;
        for (i = 0; i < randomSize; i++)
        {
            environmentVariable[i] = rand() % ('z' - 'a') + 'a';
        }
        environmentVariable[i + 1] = '\0';

        // Setting environment variable PARAM2 with argument 1
        if (setenv("PARAM2", environmentVariable, 1) == -1)
        {
            printf("Error! Cannot Set environment variable\n");
            exit(EXIT_FAILURE);
        }

        // Create child process
        PID = fork();
        if (PID == -1)
        {
            printf("Error! Cannot create process\n");
            exit(EXIT_FAILURE);
        }

        // Parent process
        if (PID > 0)
        {
            printf("\nParent PARAM1=%s, PARAM2=%s\n", getenv("PARAM1"), getenv("PARAM2"));
            exit(EXIT_SUCCESS);
        }

        // Child process
        if (PID == 0)
        {
            // Change ../Documents/ to your path
            if (execlp("../Documents/child", "child", NULL) == -1)
            {
                printf("Error! Cannot execute mentioned file\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    // SIGQUIT = Ctrl + 4 or Ctrl + / (in terminal)
    if (signalNumber == SIGQUIT)
    {
        printf("Exited manually\n");
        exit(EXIT_SUCCESS);
    }
}

int main()
{
    // Waiting for Ctrl + C 
    signal(SIGINT, signalHandler);

    // Waiting for Ctrl + 4 or Ctrl + /
    signal(SIGQUIT, signalHandler);
    while (1);

    return 0;
}
