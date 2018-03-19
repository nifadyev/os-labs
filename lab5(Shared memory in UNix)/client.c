//#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int sharedMemoryDescriptor;
  char filename[] = "lab5.c";
  key_t clientKey;     /* IPC ключ */
  int i = 0;
  char* pSharedMemory;

  if((clientKey = ftok(filename, 0)) < 0)
  {
      printf("Error! Cannot generate key\n");
      exit(-1);
  }

  if((sharedMemoryDescriptor = shmget(clientKey, 4096, 0666)) < 0)
  {
      if(errno != EEXIST)
      {
          printf("Error! Cannot create shared memory\n");
          exit(-1);
      }
      else if((sharedMemoryDescriptor = shmget(clientKey, 4096, 0)) < 0)
      {
          printf("Error! Cannot find shared memory\n");
          exit(-1);
      }
  }

  if((pSharedMemory = (char *)shmat(sharedMemoryDescriptor, (char*)NULL, 0)) == (char *)(-1))
  {
      printf("Error! Cannot shared memory\n");
      exit(-1);
  }

  pSharedMemory[0] = 0;
  for (i = 0; i < 3; i++)
  {
    switch(i)
    {
      case 0:
        pSharedMemory[4] = pSharedMemory[5] = pSharedMemory[6] ='A';
      case 1:
        pSharedMemory[7] = pSharedMemory[8] = pSharedMemory[9] ='B';
      case 2:
        pSharedMemory[10] = pSharedMemory[11] = pSharedMemory[12] ='C';
    }

    pSharedMemory[0] += (char)3;
    printf("Shared memory size: %d\n", pSharedMemory[0]);
    sleep(5);
  }

  for (i = 0; i < 9; i++)
  {
    printf("%c ", pSharedMemory[i + 4]);
  }

  pSharedMemory[0] = (char)(-1);

  if(shmdt(pSharedMemory) < 0)
  {
      printf("Can't detach shared memory\n");
      exit(-1);
  }

  printf("\nClient has been left\n");
  return 0;
}
