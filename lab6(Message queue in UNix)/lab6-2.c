#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    SEND_LENGTH = 1,
    RECEIVE_LENGTH = 1,
    RECEIVE_STRING,
    SEND_RAND_STRING
};

int main()
{
    key_t key;
    char *filename = "lab6-1.c";
    int messageQueueDescriptor;
    int i, maxLength, length = 0;
    int received = 0; // 0 - not received, 1 - received

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

    if ((key = ftok(filename, 0)) < 0)
    {
        printf("Error! Cannot generate key\n");
        exit(EXIT_FAILURE);
    }

    if ((messageQueueDescriptor = msgget(key, IPC_CREAT | 0600)) == -1)
    {
        if (errno != EEXIST)
        {
            printf("Error! Cannot create message queue\n");
            exit(EXIT_FAILURE);
        }
        else if ((messageQueueDescriptor = msgget(key, 0)) < 0)
        {
            printf("Error! Cannot find message queue\n");
            exit(EXIT_FAILURE);
        }
        // printf("Error! Cannot get message queue descriptor\n");
        // exit(EXIT_FAILURE);
    }

    // Waiting for max length from prog1
    if ((length = msgrcv(messageQueueDescriptor, &rand_length, 4, RECEIVE_LENGTH, 0)) < 0)
    {
        printf("Cannot receive length from queue\n");
        msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
        exit(EXIT_FAILURE);
    }

    maxLength = rand_length.length + 1;

    // printf("%d #%ld\n", rand_length.length, rand_length.messageType);
    // printf("rand_length.length: %d\nmaxLength: %d\nlength: %d\n", rand_length.length, maxLength, length);

    //Sending empty message to prog1. It allows prog1 to start sending strings to message queue
    if (msgsnd(messageQueueDescriptor, &rand_length, 0, 0) < 0)
    {
        printf("Error! Cannot send length to second prog\n");
        msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
        exit(EXIT_FAILURE);
    }

    for (i = 0;; i++)
    {
        //FIXME: Makes one extra step
        // if ((strlen(rand_string.message) == maxLength))
        // {
        //     printf("SHUTTING DOWN AT THE TOP OF THE CYCLE\n");
        //     msgrcv(messageQueueDescriptor, &rand_length, 0, 2, 0);
        //     msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
        //     exit(EXIT_SUCCESS);
        // }

        // Receiving message
        if (received == 0)
        {
            //printf("safasfsaf\n");
            //printf("Length before: %d\n", length);

            length = msgrcv(messageQueueDescriptor, &rand_string, maxLength, RECEIVE_STRING, 0);

            //printf("Length after: %d\n", length);
            if (length < 0)
            {
                printf("Step #%d. Cannot receive string from prog1\n", i);
                msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
                exit(EXIT_FAILURE);
            }

            // Stopping prog2 if length of useful data received from message queue is 0
            if (length == 0)
            {
                printf("SHUTTIING DOWN WHILE RECEIVING\n");
                printf("FINAL STRING: %s\n", rand_string.message);
                msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
                exit(EXIT_SUCCESS);
            }

            printf("Step #%d: Was recieved: %s\n", i, rand_string.message);
            received = 1;
        }

        // Sending message
        else
        {
            rand_string.messageType = SEND_RAND_STRING;
            rand_string.message[i] = rand() % ('z' - 'a') + 'a';
            rand_string.message[i + 1] = '\0';

            if (msgsnd(messageQueueDescriptor, &rand_string, strlen(rand_string.message) + 1, 0) < 0)
            {
                printf("Error! Cannot send string to first prog\n");
                msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
                exit(EXIT_FAILURE);
            }
            
            printf("Step #%d: Was sent: %s\n", i, rand_string.message);
            
            received = 0;
        }
    }

    return 0;
}