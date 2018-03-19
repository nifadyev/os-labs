#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

//System V IPC (IPC - сокращение от InterProcess Communications).
//В группу System V IPC входят: очереди сообщений, разделяемая память и семафоры.
int main()
{
    const int sharedMemorySize = 4096;
    int sharedMemoryDescriptor;
    char filename[] = "server.c";
    key_t serverKey;
    int i = 0, previousSize = 0;
    char* pSharedMemory;

    if((serverKey = ftok(filename, 0)) < 0)
    {
        printf("Error! Cannot generate key\n");
        exit(-1);
    }

    if(sharedMemoryDescriptor = shmget(serverKey, sharedMemorySize, IPC_CREAT|IPC_EXCL|0666) < 0) // 0666 - разрешено все всем
    {
        if(errno != EEXIST)
        {
            printf("Error! Cannot create shared memory\n");
            exit(-1);
        }
        else if((sharedMemoryDescriptor = shmget(serverKey, sharedMemorySize, 0)) < 0)
        {
            printf("Error! Cannot find shared memory\n");
            exit(-1);
        }
    }

    if((pSharedMemory = (char *)shmat(sharedMemoryDescriptor, (char*)NULL, 0)) == (char *)(-1))
    {
        printf("Error! Cannot attach shared memory\n");
        exit(-1);
    }

    pSharedMemory[0] = (char)0;
    while (1)
    {
      if ((int)pSharedMemory[0] == -1)
      {
        printf("Server has been shut down\n");
        shmdt(pSharedMemory);
        shmctl(sharedMemoryDescriptor, IPC_RMID, NULL);
        exit(1);
      }

      else if ((int)pSharedMemory[0] != previousSize)
      {
        previousSize = (int)pSharedMemory[0];
        printf("Useful Data: ");

        for (i = 0; i < (int)pSharedMemory[0]; i++)
        {
          if (i % 3 == 0)
          {
            printf(" ");
          }
            printf("%c", pSharedMemory[i + 4]);
        }
        printf("\n");
      }

      else
      {
        continue;
      }
    }

    return 0;
}
