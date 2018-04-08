#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <time.h>
#include <string.h>

int main()
{
    enum
    {
        SEND_TO_FIRST = 1,
        RECEIVE_FROM_SECOND = 1,
        SEND_TO_SECOND = 2,
        RECIEVE_FROM_FIRST = 2,
        SEND_LENGTH = 3,
        RECEIVE_LENGTH = 3
    };
    key_t key;
    int flag = 1; // 1 - "first" run, 2 - "second" run
    int messageQueueID;
    char path[] = "ping-pong.c";
    int i, len, maxLength;

    struct MessageBuffer
    {
        long type;
        char message[44];
    } messageBuffer;

    srand(time(NULL));

    if ((key = ftok(path, 0)) < 0)
    {
        printf("Failed to get key\n");
        return EXIT_FAILURE;
    }

    // Creating message queue
    if ((messageQueueID = msgget(key, 0600 | IPC_CREAT | IPC_EXCL)) < 0)
    {
        // If message queue already exist then connect to it
        if (errno == EEXIST)
        {
            if ((messageQueueID = msgget(key, 0)) < 0)
            {
                printf("Cannot get msqid\n");
                exit(EXIT_FAILURE);
            }
        }
        flag = 2;
    }

    if (flag == 1)
    {
        maxLength = rand() % 20 + 20;
        printf("maxLength = %d\n", maxLength);

        // Writing max length into first 4 elements of the message string
        messageBuffer.message[0] = messageBuffer.message[1] = messageBuffer.message[2] = messageBuffer.message[3] = maxLength;
        messageBuffer.type = SEND_LENGTH;

        if (msgsnd(messageQueueID, &messageBuffer, 5, 0) < 0)
        {
            printf("Cannot send message to queue\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (msgrcv(messageQueueID, &messageBuffer, 5, RECEIVE_LENGTH, 0) < 0)
        {
            printf("Cannot recieve message to queue\n");
            exit(EXIT_FAILURE);
        }

        maxLength = messageBuffer.message[0];
        printf("maxLength = %d\n", maxLength);

        // Creating first message
        messageBuffer.message[4] = rand() % ('z' - 'a') + 'a';
        messageBuffer.message[5] = '\0';
        messageBuffer.type = SEND_TO_FIRST;

        if (msgsnd(messageQueueID, &messageBuffer, strlen(messageBuffer.message) + 1, 0) < 0)
        {
            printf("Cannot send message to queue\n");
            exit(EXIT_FAILURE);
        }
    }

    while (1)
    {
        // Finishing execution
        if (strlen(messageBuffer.message) >= maxLength + 4)
        {
            // Printing final string
            printf("Final String: ");
            //FIXME: sometimes print extra letter at the end
            for (i = 0; i < maxLength + 4; i++)
            {
                printf("%c", messageBuffer.message[i + 4]);
            }
            printf("\n");

            // And deleting message queue
            if (msgctl(messageQueueID, IPC_RMID, NULL) < 0)
            {
                printf("Cannot delete queue\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }

        //Penultimate step
        if (strlen(messageBuffer.message) >= maxLength + 3)
        {
            // Exiting program and finishing execution in second program
            printf("Finished successfully\n");
            exit(EXIT_SUCCESS);
        }

        // Receiving messages from queue
        if (msgrcv(messageQueueID, &messageBuffer, maxLength + 4, ((flag == 1) ? RECEIVE_FROM_SECOND : RECIEVE_FROM_FIRST), 0) < 0)
        {
            printf("Cannot recieve message to queue\n");
            exit(EXIT_FAILURE);
        }
        printf("Recieved: %s\n", messageBuffer.message);

        // Creating message
        len = strlen(messageBuffer.message);

        // Uncomment if necessary
        //printf("Useful data(without first 4 elements) = %d\n", len - 4);
        messageBuffer.message[len] = rand() % ('z' - 'a') + 'a';
        messageBuffer.message[len + 1] = '\0';

        // Choosing the type of message
        messageBuffer.type = ((flag == 1) ? SEND_TO_SECOND : SEND_TO_FIRST);

        // Sending message
        if (msgsnd(messageQueueID, &messageBuffer, strlen(messageBuffer.message) + 1, 0) < 0)
        {
            printf("Cannot send message to queue\n");
            exit(EXIT_FAILURE);
        }
        printf("Sent: %s\n", messageBuffer.message);
    }

    return 0;
}
