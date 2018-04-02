#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define SEND_LENGTH 1
// #define SEND_RAND_STRING 2
enum
{
    SEND_LENGTH = 1,
    RECEIVE_LENGTH = 1,
    SEND_RAND_STRING,
    RECEIVE_STRING
};

int main()
{
    key_t key;
    char *filename = "lab6-1.c";
    int messageQueueDescriptor;
    int i, maxLength;
    int sent = 0;
    int send = 0, receive = 0;

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

    // Maybe rights should be changed to 0600 or 0666
    if ((messageQueueDescriptor = msgget(key, 0)) == -1)
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

    srand(time(NULL));
    maxLength = rand() % 20 + 20;
    rand_length.messageType = SEND_LENGTH;
    rand_length.length = maxLength;

    // Sending max length
    send = msgsnd(messageQueueDescriptor, &rand_length, sizeof(int), 0);
    if (send < 0)
    {
        printf("Error! Cannot send length to second prog\n");
        msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
        exit(EXIT_FAILURE);
    }

    // Receiving empty message.It allows this prog to start sending strings to message queue
    receive = msgrcv(messageQueueDescriptor, &rand_length, 0, RECEIVE_LENGTH, 0);
    if (receive < 0)
    {
        printf("Cannot receive message from prog2\n");
        msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < maxLength; i++)
    {
        // Sending message
        if (sent == 0)
        {
            rand_string.messageType = SEND_RAND_STRING;
            rand_string.message[i] = rand() % ('z' - 'a') + 'a';
            rand_string.message[i + 1] = '\0';

            send = msgsnd(messageQueueDescriptor, &rand_string, strlen(rand_string.message) + 1, 0);
            if (send < 0)
            {
                printf("Error! Cannot send string to second prog\n");
                msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
                exit(EXIT_FAILURE);
            }
            printf("Step #%d: Was sent: %s\n", i, rand_string.message);

            sent = 1;
        }

        // Receiving message
        else
        {
            receive = msgrcv(messageQueueDescriptor, &rand_string, maxLength + 1, RECEIVE_STRING, 0);

            printf("Step #%d: Was recieved: %s\n", i, rand_string.message);

            if (receive < 0)
            {
                printf("Cannot receive string from prog1\n");
                msgctl(messageQueueDescriptor, IPC_RMID,
                       (struct msqid_ds *)NULL);
                exit(EXIT_FAILURE);
            }

            sent = 0;
        }

        // Finishing sending and receiving messages through message queue
        if (strlen(rand_string.message) == maxLength)
        {
            printf("WELL DONE. STOPPING AFTER SENDING AND RECEIVING\n");

            // Sending empty char message as a stop signal for prog2
            rand_string.messageType = SEND_RAND_STRING;
            strcpy(rand_string.message, "\0");

            send = msgsnd(messageQueueDescriptor, &rand_string, 0, 0);
            if (send < 0)
            {
                printf("Cannot send empty finishing message to queue\n");
                msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
        }
    }

    // Should not be printed if everything worked correctly
    printf("STOPPING AT THE END. PROBABLY SMTH WENT WRONG\n");

    return 0;
}