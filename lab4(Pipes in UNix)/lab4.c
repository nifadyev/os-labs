#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
 
int main()
{
        int pipe1,pipe2;
        int fd1[2],fd2[2];
        pid_t child,grandchild;
 
        pipe1=pipe(fd1);
        child=fork();
        if(pipe1<0)
        {
            printf("Failed to create pipe\n");
            return EXIT_FAILURE;
        }
        if(child<0)
        {
            printf("Failed to fork\n");
            return EXIT_FAILURE;
        }

        else if(child == 0) // Child process
        {
            pipe2=pipe(fd2);
            grandchild=fork();

            if(pipe2<0)
            {
                printf("Failed to create pipe\n");
                return EXIT_FAILURE;
            }
            if(grandchild < 0)
            {
                printf("Failed to fork\n");
                return EXIT_FAILURE;
            }

            else if (grandchild == 0) // Grandchild process
            {
                close (1);
                dup2 (fd2[1],1);
                close (fd2[0]);
                close (fd2[1]);

                if(execl("/bin/ls", "ls", "-la", "/dev", NULL) == -1)
                {
                    printf("Exec error\n");
                    return EXIT_FAILURE;
                }
            }
            else // Child process
            {
                close (0);
                dup2 (fd2[0],0);
                close (fd2[0]);
                close (fd2[1]);

                close (1);
                dup2(fd1[1],1);
                close (fd1[0]);
                close (fd1[1]);

                if(execlp ("sort", "sort", NULL) == -1)
                {
                    printf("Exec error\n");
                    return EXIT_FAILURE;
                }
            }
        }
        else
        {
            close (0);
            dup2 (fd1[0],0);
            close (fd1[0]);
            close (fd1[1]);

            if(execlp ("more", "more", NULL) == -1)
            {
                printf("Exec Error\n");
                return EXIT_FAILURE;
            }
        }
 
    return 0;
}