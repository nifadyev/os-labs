#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("Child PARAM1: %s, PARAM2: %s\n", getenv("PARAM1"), getenv("PARAM2"));
    return 0;
}