#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int stringLength;
    char *data;
    int currentPosition;
    int isFinished;
} Thread;

// String proccessing or time interval proccessing
int f(Thread *threads, int timeInterval, int threadNumber)
{
    int i, finishedThreads = 0;
    int b = 0;        // flag, which tells whether we visited this row/column or not
    int isActive = 0; // does thread still running?
    printf("\n%d", timeInterval + 1);

    for (i = 0; i < threadNumber; i++)
    {
        if (threads[i].currentPosition >= threads[i].stringLength)
        {
            printf("\tx");
            threads[i].isFinished = 1;
            finishedThreads++;

            continue;
        }

        if (threads[i].data[threads[i].currentPosition] == '0' && b == 0)
        {
            //printf("\t0HERE");
            printf("\t0");
            isActive = 1;
            b = 1;
            threads[i].currentPosition++;
        }

        else if (threads[i].data[threads[i].currentPosition] == '0' /*  && b != 0 */)
        {
            //printf("\t.THERE");
            printf("\t.");
        }

        else if (threads[i].data[threads[i].currentPosition] == '-')
        {
            isActive = 0;
            printf("\t|");
            threads[i].currentPosition++;
        }
        //finishedThreads += threads[i].isFinished;
    }

    if (finishedThreads == threadNumber)
    {
        return 0;
    }

    return 1;
}

int main()
{
    int threadNumber;
    int i;
    char inputFile[] = "input.txt";
    char **strings;
    Thread *threads;
    FILE *file;
    if ((file = fopen(inputFile, "r")) == NULL)
    {
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d\n", &threadNumber);
    if (threadNumber < 1 || threadNumber > 100)
    {
        printf("Error! Wrong range\n");
        exit(EXIT_FAILURE);
    }

    strings = (char **)malloc(threadNumber * 2 * sizeof(char *));
    for (i = 0; i < threadNumber * 2; i++)
    {
        strings[i] = (char *)malloc(/* threadNumber */ 50 * sizeof(char));
    }

    for (i = 0; i < threadNumber * 2; i++)
    {
        fscanf(file, "%s\n", strings[i]);
    }
    threads = (Thread *)malloc(threadNumber * sizeof(Thread));

    for (i = 0; i < threadNumber; i++)
    {
        threads[i].stringLength = atoi(strings[i * 2]);
        threads[i].data = strings[i * 2 + 1];
        threads[i].currentPosition = 0;
        threads[i].isFinished = 0;
    }

    for (i = 0; f(threads, i, threadNumber) /*  && i < 20 */; i++)
        // {
        //     printf("\n%d iteration\n", i);
        // }
        ;

    return 0;
}
