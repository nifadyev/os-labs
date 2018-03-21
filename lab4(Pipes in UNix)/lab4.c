#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int fd1[2], fd2[2]; // in/out chanels for pipes
	pid_t childPID1, childPID2;
	int pipe1, pipe2;

	pipe1 = pipe(fd1);
	childPID1 = fork();

	if(pipe1 < 0)
	{
		printf("Error! Cannot create pipe\n");
		return EXIT_FAILURE;
	}
	if(childPID1 < 0)
	{
		printf("Error! Cannot create child process\n");
		return EXIT_FAILURE;
	}

	else if(childPID1 == 0) // Child process
	{
		pipe(fd2);
		childPID2 = fork();

		if(pipe2 < 0)
		{
			printf("Error! Cannot create pipe\n");
			return EXIT_FAILURE;
		}
		if (childPID2 < 0)
		{
			printf("Error! Cannot create grandchild\n");
			return EXIT_FAILURE;
		}

		else if (childPID2 == 0) // Grandchild process
		{
      // Closing stdin
      close(0);
      dup2(fd1[0], 0);
      close(fd1[0]);
      close(fd1[1]);

      // Closing stdiout
      close(1);
      dup2(fd2[1], 1);
      close(fd2[0]);
      close(fd2[1]);

			execl("/bin/ls", "ls", "-la", "/dev", NULL);
		}
		else // Child process
		{
      // Closing stdin
			close(0);
      dup2(fd2[0], 0);
      close(fd2[0]);
      close(fd2[1]);

      // Closing stdiout
			close(1);
			dup2(fd1[1], 1);
			close(fd1[0]);
			close(fd1[1]);

			execlp("sort", "sort", NULL);
		}
	}
	else // Parent
	{
    // Closing stdin
		close(0);
		dup2(fd1[0], 0);
		close(fd1[0]);
		close(fd1[1]);
    
		execlp("more", "more", NULL);
	}

	return 0;
}
