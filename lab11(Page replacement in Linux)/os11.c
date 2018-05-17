#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_HANDLING_NUMBER 1000
#define MAX_PAGE_NUMBER 100

struct TaskInformation // input file structure
{
    int handlings[1000]; // обращения
    int physicalPageNumber;
    int virtualPageNumber; // virtual address space pages
    int handlingNumber;
};

struct PhysicalMemory
{
    int physicalPages[100];
    int currentPhysicalPage;
};

int Find(int elem, int a[], int size);

int main(int argc, char **argv)
{
    char *inputFilePath = argv[1];
    char *outputFilePath = argv[2];
    FILE *inputFile;
    FILE *outputFile;
    struct TaskInformation fifo;
    struct PhysicalMemory memory;
    int pageFaultNumber = 0;
    int i, j;

    if (argc != 3)
    {
        printf("Enter input and output file names\n");
        exit(EXIT_FAILURE);
    }

    inputFile = fopen(inputFilePath, "r");
    if (inputFile == NULL)
    {
        printf("Cannot open input file\n");
        exit(EXIT_FAILURE);
    }

    outputFile = fopen(outputFilePath, "w");
    if (outputFile == NULL)
    {
        printf("Cannot open output file\n");
        exit(EXIT_FAILURE);
    }

    // Reading from file
    fscanf(inputFile, "%i", &fifo.physicalPageNumber);
    fscanf(inputFile, "%i", &fifo.virtualPageNumber);
    fscanf(inputFile, "%i", &fifo.handlingNumber);
    for (i = 0; i < fifo.handlingNumber; i++)
    {
        fscanf(inputFile, "%i", &fifo.handlings[i]);
    }

    // Printing info from file
    printf("PPages:%i \n", fifo.physicalPageNumber);
    printf("VAPPages:%i \n", fifo.virtualPageNumber);
    printf("MemCount:%i \n", fifo.handlingNumber);
    for (i = 0; i < fifo.handlingNumber; i++)
    {
        printf(" %i ", fifo.handlings[i]);
    }
    printf("\n");

    memory.currentPhysicalPage = 0;
    for (i = 0; i < fifo.physicalPageNumber; i++)
    {
        memory.physicalPages[i] = -1;
    }

    // TODO: Make a function from this for loop
    // Main part
    for (i = 0; i < fifo.handlingNumber; i++)
    {
        // if page was not found in physical memory
        // if (!Find(fifo.handlings[i], memory.physicalPages, fifo.physicalPageNumber))
        // {
        //     memory.physicalPages[memory.currentPhysicalPage] = fifo.handlings[i];
        //     memory.currentPhysicalPage++;

        //     for (j = 0; j < fifo.physicalPageNumber; j++)
        //     {
        //         if (memory.physicalPages[j] > 0)
        //         {
        //             printf("%i ", memory.physicalPages[j]);
        //             fprintf(outputFile, "%i ", memory.physicalPages[j]);
        //         }
        //         else
        //         {
        //             printf("F ");
        //             fprintf(outputFile, "F ");
        //         }
        //     }
        //     printf("\n");
        //     fprintf(outputFile, "\n");

        //     if (memory.currentPhysicalPage == fifo.physicalPageNumber)
        //     {
        //         memory.currentPhysicalPage = 0;
        //     }
        //     pageFaultNumber++;
        // }
        // else
        // {
        //     for (j = 0; j < fifo.physicalPageNumber; j++)
        //     {
        //         if (memory.physicalPages[j] > 0)
        //         {
        //             printf("%i ", memory.physicalPages[j]);
        //             fprintf(outputFile, "%i ", memory.physicalPages[j]);
        //         }
        //         else
        //         {
        //             printf("F ");
        //             fprintf(outputFile, "F ");
        //         }
        //     }
        //     printf("\n");
        //     fprintf(outputFile, "\n");
        // }

        if (!Find(fifo.handlings[i], memory.physicalPages, fifo.physicalPageNumber))
        {
            memory.physicalPages[memory.currentPhysicalPage] = fifo.handlings[i];
            memory.currentPhysicalPage++;
            pageFaultNumber++;
        }

        for (j = 0; j < fifo.physicalPageNumber; j++)
        {
            if (memory.physicalPages[j] > 0)
            {
                printf("%i ", memory.physicalPages[j]);
                fprintf(outputFile, "%i ", memory.physicalPages[j]);
            }
            else
            {
                printf("F ");
                fprintf(outputFile, "F ");
            }
        }
        printf("\n");
        fprintf(outputFile, "\n");

        if (memory.currentPhysicalPage == fifo.physicalPageNumber)
        {
            memory.currentPhysicalPage = 0;
        }
    }

    printf("%i \n", pageFaultNumber);
    fprintf(outputFile, "%i \n", pageFaultNumber);
    fclose(outputFile);
    fclose(inputFile);

    return 0;
}

int Find(int elem, int a[], int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        if (a[i] == elem)
            return 1;
    }

    return 0;
}
