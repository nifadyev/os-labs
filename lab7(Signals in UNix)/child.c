#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Child PARAM1: %s, PARAM2: %s\n", getenv("PARAM1"), getenv("PARAM2"));
    return 0;
}
