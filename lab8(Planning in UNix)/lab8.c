#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int stringLength;
    char *string;
    int currentPosition;
    int isFinished;
} para;

int f(para *t, int num, int col)
{
    int i, b = 0, count = 0;
    printf("\n%d", num + 1);

    for (i = 0; i < col; i++)
    {
        if (t[i].currentPosition >= t[i].stringLength)
        {
            printf("\tx");
            t[i].isFinished = 1;
            count += t[i].isFinished;
            continue;
        }

        if (t[i].string[t[i].currentPosition] == '0' && b == 0)
        {
            printf("\t0");
            b = 1;
            t[i].currentPosition++;
        }

        else if (t[i].string[t[i].currentPosition] == '0' && b != 0)
        {
            printf("\t.");
        }
        else if (t[i].string[t[i].currentPosition] == '-')
        {
            printf("\t|");
            t[i].currentPosition++;
        }
        count += t[i].isFinished;
    }
    if (count == col)
        return 0;

    return 1;
}

int main()
{
    int threadNumber;
    int i;
    char inputFile[] = "input.txt";
    char **strings;
    para *t;
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
    t = (para *)malloc(threadNumber * sizeof(para));
    for (i = 0; i < threadNumber; i++)
    {
        t[i].stringLength = atoi(strings[i * 2]);
        t[i].string = strings[i * 2 + 1];
        t[i].currentPosition = 0;
        t[i].isFinished = 0;
    }
    //	for(i=0;i<threadNumber;i++)
    // {
    //	printf("%d\n",t[i].stringLength);
    //	printf("%s\n",t[i].string);
    // }
    i = 0;
    while (f(t, i++, threadNumber) && i < 20)
        ;
    return 0;
}
