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
    char string[40] = {0};
    int messageQueueDescriptor;
    int i, maxLength;
    int sent;
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

    if ((messageQueueDescriptor = msgget(key, 0)) == -1)
    {
        if (errno != EEXIST)
        {
            printf("Error! Cannot create shared memory\n");
            exit(-1);
        }
        else if ((messageQueueDescriptor = msgget(key, 0)) < 0)
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

    send = msgsnd(messageQueueDescriptor, &rand_length, sizeof(int), 0);
    if (send < 0)
    {
        printf("Error! Cannot send length to second prog\n");
        msgctl(messageQueueDescriptor, IPC_RMID,
               (struct msqid_ds *)NULL);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Size: %d Message Type: %ld\n", rand_length.length, rand_length.messageType);
    }

    receive = msgrcv(messageQueueDescriptor, &rand_length, 0, 1, 0);
    if (receive < 0)
    {
        printf("Cannot receive message from prog2\n");
        msgctl(messageQueueDescriptor, IPC_RMID,
               (struct msqid_ds *)NULL);
        exit(EXIT_FAILURE);
    }

    sent = 0;
    for (i = 0; i < maxLength; i++)
    {
        // if (strlen(rand_string.message) == maxLength)
        // {
        //     rand_string.messageType = 255;
        //     send = msgsnd(messageQueueDescriptor, /*(struct string_msg_buffer *)*/ &rand_string, 0, 0);
        //     if (send < 0)
        //     {
        //         printf("Can\'t send message to queue\n");
        //         msgctl(messageQueueDescriptor, IPC_RMID,
        //                (struct msqid_ds *)NULL);
        //         exit(EXIT_FAILURE);
        //     }
        //     printf("Stopping\n");
        //     break;
        // }
        if (sent == 0)
        {
            printf("Current length: %d\n", receive);

            rand_string.messageType = 2;
            rand_string.message[i] = rand() % ('z' - 'a') + 'a';
            rand_string.message[i + 1] = '\0';

            printf("\t\t\tRandom letter in message: %c\n", rand_string.message[i]);
            printf("\t\t\tCurrent message: %s\n", rand_string.message);
            printf("\t\t\tCurrent message length: %ld\n", strlen(rand_string.message));

            send = msgsnd(messageQueueDescriptor, &rand_string, strlen(rand_string.message) + 1, 0);
            if (send < 0)
            {
                printf("Error! Cannot send string to second prog\n");
                msgctl(messageQueueDescriptor, IPC_RMID,
                       (struct msqid_ds *)NULL);
                exit(EXIT_FAILURE);
            }
            printf("Step #%d: Was sent: %s\n", i, rand_string.message);

            sent = 1;
            continue;
        }
        else
        {
            receive = msgrcv(messageQueueDescriptor, &rand_string, maxLength + 1, 3, 0);
            printf("Current length: %d\n", receive);

            if (receive < 0)
            {

                printf("\t\t\tCurrent message: %s\n", rand_string.message);
                printf("\t\t\tCurrent message length: %ld\n", strlen(rand_string.message));

                printf("Step #%d: Was recieved: %s\n", i, rand_string.message);
                if (strlen(rand_string.message) == maxLength)
                {
                    printf("Well done\n");
                    rand_string.messageType = 255;
                    send = msgsnd(messageQueueDescriptor, &rand_string, 0, 0);
                    if (send < 0)
                    {
                        printf("Can\'t send message to queue\n");
                        msgctl(messageQueueDescriptor, IPC_RMID,
                               (struct msqid_ds *)NULL);
                        exit(-1);
                    }
                    msgctl(messageQueueDescriptor, IPC_RMID,
                           (struct msqid_ds *)NULL);
                    exit(EXIT_SUCCESS);
                }
                printf("Cannot receive string from prog1\n");
                msgctl(messageQueueDescriptor, IPC_RMID,
                       (struct msqid_ds *)NULL);
                exit(EXIT_FAILURE);
            }
            if (rand_string.messageType == 3)
            {

                printf("\t\t\tCurrent message: %s\n", rand_string.message);
                printf("\t\t\tCurrent message length: %ld\n", strlen(rand_string.message));
                printf("Step #%d: Was recieved: %s\n", i, rand_string.message);
            }
            sent = 0;
        }

        if (strlen(rand_string.message) == maxLength)
        {
            printf("Well done here\n");
            rand_string.messageType = 255;
            send = msgsnd(messageQueueDescriptor, &rand_string, 0, 0);
            if (send < 0)
            {
                printf("Can\'t send message to queue\n");
                msgctl(messageQueueDescriptor, IPC_RMID,
                       (struct msqid_ds *)NULL);
                exit(-1);
            }
            exit(EXIT_SUCCESS);
        }
    }

    rand_string.messageType = 255;
    send = msgsnd(messageQueueDescriptor, &rand_string, 0, 0);
    if (send < 0)
    {
        printf("Can\'t send message to queue\n");
        msgctl(messageQueueDescriptor, IPC_RMID,
               (struct msqid_ds *)NULL);
        exit(-1);
    }

    rand_length.messageType = 255;
    send = msgsnd(messageQueueDescriptor, &rand_length, 0, 0);
    if (send < 0)
    {
        printf("Can\'t send message to queue\n");
        msgctl(messageQueueDescriptor, IPC_RMID,
               (struct msqid_ds *)NULL);
        exit(-1);
    }
    printf("Stopping at the end\n");

    return 0;
}