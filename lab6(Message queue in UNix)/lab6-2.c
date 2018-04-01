#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEND_LENGTH 1
#define SEND_RAND_STRING 2

int main()
{
    key_t key;
    char *filename = "lab6-1.c";
    int messageQueueDescriptor;
    int i, maxLength, length;

    struct length_msg_buffer
    { 
        long messageType;
        int length; 
    } rand_length;

    struct string_msg_buffer
    { 
        long messageType;
        char message[40]; 
    } rand_string;
    

    if((key = ftok(filename, 0)) < 0)
    {
        printf("Error! Cannot generate key\n");
        exit(EXIT_FAILURE);
    }

    if((messageQueueDescriptor = msgget(key, IPC_CREAT | 0666)) == -1)
    {
        if(errno != EEXIST)
        {
            printf("Error! Cannot create shared memory\n");
            exit(-1);
        }
        else if((messageQueueDescriptor = msgget(key, 0)) < 0)
        {
            printf("Error! Cannot find shared memory\n");
            exit(-1);
        }
        // printf("Error! Cannot get message queue descriptor\n");
        // exit(EXIT_FAILURE);
    }

    //TODO: fix msgrcv returns 0 or big number
    if((length = msgrcv(messageQueueDescriptor, 
    (struct length_msg_buffer *)&rand_length, maxLength, 0, MSG_NOERROR)) < 0)
    {
            printf("Cannot receive length from queue\n");
            msgctl(messageQueueDescriptor, IPC_RMID,
            (struct msqid_ds *) NULL);
            exit(EXIT_FAILURE);
    }
    else
    {
        //maxLength = rand_length.length;
        //rand_length.length = length;
        //rand_length.messageType = 1;
        printf("%d #%ld\n", rand_length.length, rand_length.messageType);
        printf("rand_length.length: %d\nmaxLength: %d\nlength: %d\n", rand_length.length, maxLength, length);
        //rand_length.messageType = 254;
        if(msgsnd(messageQueueDescriptor, 
            (struct length_msg_buffer *)&rand_length, 0, 0) < 0)
        {
            printf("Error! Cannot send length to second prog\n");
            msgctl(messageQueueDescriptor, IPC_RMID,
                (struct msqid_ds *) NULL);
            exit(EXIT_FAILURE);
        }
    }

    maxLength = rand_length.length;
    i = 0;
    length = 0;
    while(1)
    {

        if((length = msgrcv(messageQueueDescriptor, 
            (struct string_msg_buffer *)&rand_string, maxLength, 0, MSG_NOERROR)) < 0)
        {
            printf("Cannot receive string from prog1\n");
            msgctl(messageQueueDescriptor, IPC_RMID,
                (struct msqid_ds *) NULL);
            exit(EXIT_FAILURE);
        }
        if(rand_string.messageType == 255)
        {
                msgctl(messageQueueDescriptor, IPC_RMID,
                (struct msqid_ds *) NULL);
                exit(EXIT_SUCCESS);
        }
        printf("Step #%d: Was recieved: %s\n", i, rand_string.message);
        // if(length == maxLength)
        // {
        //     if(msgsnd(messageQueueDescriptor, 
        //     (struct length_msg_buffer *)&rand_length, 4, 0) < 0)
        //     {
        //         printf("Cannot send length from queue\n");

        //         exit(EXIT_FAILURE);
        //     }
        //     break;
        // }
    }


    // if(msgctl(messageQueueDescriptor, IPC_RMID,
    //      (struct msqid_ds *) NULL) < 0)
    // {
    //     printf("Error! Cannot delete message queue\n");
    //     exit(EXIT_FAILURE);
    // }

    return 0;
}