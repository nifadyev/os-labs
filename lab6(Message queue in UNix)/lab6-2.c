#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEND_LENGTH 1
#define SEND_RAND_STRING 3

int main()
{
    key_t key;
    char *filename = "lab6-1.c";
    int messageQueueDescriptor;
    int i, maxLength, length;
    int received;

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

    if ((length = msgrcv(messageQueueDescriptor, &rand_length, 4, SEND_LENGTH, 0)) < 0)
    {
        printf("Cannot receive length from queue\n");
        msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
        exit(EXIT_FAILURE);
    }

    maxLength = rand_length.length + 1;
    printf("%d #%ld\n", rand_length.length, rand_length.messageType);
    printf("rand_length.length: %d\nmaxLength: %d\nlength: %d\n", rand_length.length, maxLength, length);
    if (msgsnd(messageQueueDescriptor, &rand_length, 0, 0) < 0)
    {
        printf("Error! Cannot send length to second prog\n");
        msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
        exit(EXIT_FAILURE);
    }

    //maxLength = rand_length.length + 1;
    //i = 0;
    length = 0;
    received = 0;
    //srand(time(NULL));

    for (i = 0;; i++)
    {
        // if(i == maxLength + 1)
        // {
        //     break;
        // }
        if ((strlen(rand_string.message) == maxLength))
        {
            printf("Shutting down fsdf\n");
            msgrcv(messageQueueDescriptor, &rand_length, 0, 255, 0);
            
            msgctl(messageQueueDescriptor, IPC_RMID, (struct msqid_ds *)NULL);
            exit(EXIT_SUCCESS);
        }
        if (received == 0)
        {
            printf("safasfsaf\n");
            printf("Length before: %d\n", length);
            length = msgrcv(messageQueueDescriptor, &rand_string, maxLength, 0, 0);
            printf("Length after: %d\n", length);
            if (length < 0)
            {
                //printf("Step #%d:\n", i);
                printf("Step #%d. Cannot receive string from prog1\n", i);
                msgctl(messageQueueDescriptor, IPC_RMID,
                       (struct msqid_ds *)NULL);
                exit(EXIT_FAILURE);
            }
            if (rand_string.messageType == 255)
            {
                printf("Shutting down\n");
                printf("Final string: %s\n", rand_string.message);
                msgctl(messageQueueDescriptor, IPC_RMID,
                       (struct msqid_ds *)NULL);
                exit(EXIT_SUCCESS);
            }
            if (rand_string.messageType == 2)
            {
                printf("Step #%d: Was recieved: %s\n", i, rand_string.message);
            }
            received = 1;

            //if(length == )
            //continue;
        }

        else
        {
            printf("Current length: %d\n", length);
            rand_string.messageType = 3;
            rand_string.message[i] = rand() % ('z' - 'a') + 'a';
            rand_string.message[i + 1] = '\0';

            printf("\t\t\tRandom letter: %c\n", rand_string.message[i]);
            printf("\t\t\tCurrent message: %s\n", rand_string.message);

            if (msgsnd(messageQueueDescriptor, &rand_string, strlen(rand_string.message) + 1, 0) < 0)
            {
                printf("Error! Cannot send string to first prog\n");
                msgctl(messageQueueDescriptor, IPC_RMID,
                       (struct msqid_ds *)NULL);
                exit(EXIT_FAILURE);
            }
            printf("Current message length: %ld\n", strlen(rand_string.message));
            printf("Step #%d: Was sent: %s\n", i, rand_string.message);
            received = 0;

            // if (length >= maxLength)
            // {
            //     break;
            // }
        }
    }
    //TODO: Убрать лишнюю отправку в конце

    return 0;
}