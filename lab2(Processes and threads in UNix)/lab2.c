#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t kidPID;
    int wstatus;

    kidPID = fork();
    if(kidPID < 0)
    {
	     printf("Error. Child process creation failed");
	      return 1;
    }
    else if(kidPID == 0) // In Child process
    {
	     printf("\nProgram running in child process. KidPID = %d, ParentPID = %d\n", getpid(), getppid());
	      execl("/bin/ls","/bin/ls", "-la", "/tmp", NULL);
    }

    else // In Parent Process
    {
	     wait(&wstatus);
	     printf("\nProgram running in parent process.ParentPID = %d, kidPID = %d\n", getpid(), kidPID);

	     if(WIFEXITED(wstatus))
	     {
	            printf("\nChild process has successfully been finished. Exit code = %d\n", WEXITSTATUS(wstatus));
	            exit(EXIT_SUCCESS);
	     }
	     else
       {
      	    printf("Child procces failed wit exit status %d\n", WEXITSTATUS(wstatus));
      	    exit(EXIT_FAILURE);
       }
    }

    return 0;
}
