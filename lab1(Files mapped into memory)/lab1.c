#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>

int main(int argc, char* argv[], char** env)
{
  int fd = open(argv[1], O_RDWR);
  int length = lseek(fd, 0, SEEK_END);
  char* ptr = NULL;
  char temp;
  int i = 0;

  ptr = (char*)mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  for (i = 0; i < length - 1; i += 2)
  {
    temp = ptr[i];
    ptr[i] = ptr[i + 1];
    ptr[i + 1] = temp;
  }

  munmap(ptr, length);
  close(fd);
  
  return 0;
}
