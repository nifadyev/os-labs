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
    int i, maxLength;

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

    srand(time(NULL));
    maxLength = rand() % 20 + 20;
    rand_length.messageType = SEND_LENGTH;
    rand_length.length = maxLength;

    if(msgsnd(messageQueueDescriptor, 
    (struct length_msg_buffer *)&rand_length, 4, 0) < 0)
    {
        printf("Error! Cannot send length to second prog\n");
        msgctl(messageQueueDescriptor, IPC_RMID,
         (struct msqid_ds *) NULL);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("%d #%ld\n", rand_length.length, rand_length.messageType);
    }

    i = 0;
    if(msgrcv(messageQueueDescriptor, 
    (struct length_msg_buffer *)&rand_length, 0, 0, 0) < 0)
    {
            printf("Cannot receive message from prog2\n");
            msgctl(messageQueueDescriptor, IPC_RMID,
            (struct msqid_ds *) NULL);
            exit(EXIT_FAILURE);
    }
    // if(rand_length.messageType != 254)
    // {
    //     printf("Cannot get correct message from prog2\n");
    //     msgctl(messageQueueDescriptor, IPC_RMID,
    //         (struct msqid_ds *) NULL);
    //     exit(EXIT_FAILURE);
    // }
    while(1)
    {
        if(strlen(rand_string.message) == maxLength)
        {
            break;
        }
        rand_string.messageType = SEND_RAND_STRING;
        rand_string.message[i] = rand() % ('z' - 'a') + 'a';
        rand_string.message[i + 1] = '\0';
        
        if(msgsnd(messageQueueDescriptor, 
            (struct string_msg_buffer *)&rand_string, strlen(rand_string.message) + 1, 0) < 0)
        {
            printf("Error! Cannot send string to second prog\n");
            msgctl(messageQueueDescriptor, IPC_RMID,
                    (struct msqid_ds *) NULL);
            exit(EXIT_FAILURE);
        }
        printf("Step #%d: Was sent: %s\n", i, rand_string.message);
        i++;
    }

    //if(msgrcv)
    // if(msgctl(messageQueueDescriptor, IPC_RMID,
    //      (struct msqid_ds *) NULL) < 0)
    // {
    //     printf("Error! Cannot delete message queue\n");
    //     exit(EXIT_FAILURE);
    // }
    rand_string.messageType = 255;
    if (msgsnd(messageQueueDescriptor, (struct string_msg_buffer *) &rand_string, 0, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(messageQueueDescriptor, IPC_RMID, 
            (struct msqid_ds *) NULL);
        exit(-1);
    }

    return 0;
}