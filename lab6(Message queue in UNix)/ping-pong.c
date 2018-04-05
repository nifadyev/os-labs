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
    enum {SEND_TO_FIRST = 1, RECEIVE_FROM_FIRST = 1, SEND_TO_SECOND = 2, RECIEVE_FROM_FIRST = 2, SEND_LENGTH = 3, RECEIVE_LENGTH = 3};
    key_t key;
    int flag = 1; // 1 - "first" run, 2 - "second" run
    int messageQueueID;
    char path[] = "ping-pong.c";
    int i, len, maxsize;
    int usefullData = 0;

    struct MessageBuffer
    {
        long type; // 1- type to recieve/send len,  2 - send to/recieve from second , 3 - send to/recieve from first
        char message[44];
    } messageBuffer;

    srand(time(NULL));

    if ((key = ftok(path, 0)) < 0)
    {
        printf("Failed to get key\n");
        return EXIT_FAILURE;
    }

    if ((messageQueueID = msgget(key, 0600 | IPC_CREAT | IPC_EXCL)) < 0)
    {
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
        maxsize = rand() % 20 + 20;
        printf("maxsize = %d\n", maxsize);

        messageBuffer.message[0] = messageBuffer.message[1] = messageBuffer.message[2] = messageBuffer.message[3] = maxsize;
        messageBuffer.type = 1;

        if (msgsnd(messageQueueID, &messageBuffer, 5, 0) < 0)
        {
            printf("Cannot send message to queue\n");
            //msgctl(msqid,IPC_RMID)
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (msgrcv(messageQueueID, &messageBuffer, 5, 1, 0) < 0)
        {
            printf("Cannot recieve message to queue\n");
            exit(EXIT_FAILURE);
        }

        maxsize = (int)messageBuffer.message[0];
        printf("maxSize = %d\n", maxsize);
        messageBuffer.message[4] = rand() % ('z' - 'a') + 'a';
        messageBuffer.message[5] = '\0';
        messageBuffer.type = 2;

        if (msgsnd(messageQueueID, &messageBuffer, strlen(messageBuffer.message) + 1, 0) < 0)
        {
            printf("Cannot send message to queue\n");
            exit(EXIT_FAILURE);
        }
    }

    for (;;)
    {
        if (flag == 1)
        {
            if (strlen(messageBuffer.message) >= maxsize + 4)
            {
                printf("Final String: ");
                for (i = 0; i < maxsize + 4; i++)
                {
                    printf("%c", messageBuffer.message[i + 4]);
                }
                printf("\n");

                if (msgctl(messageQueueID, IPC_RMID, NULL) < 0)
                {
                    printf("Cannot delete queue\n");
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
            }

            if (strlen(messageBuffer.message) >= maxsize + 3)
            {
                printf("Finished successfully\n");
                exit(EXIT_SUCCESS);
            }

            if ((usefullData = msgrcv(messageQueueID, &messageBuffer, maxsize + 4, 2, 0)) < 0)
            {
                printf("No usefull data\n");
                exit(-1);
            }
            printf("Usefull Data: %d\n", usefullData);
            printf("Recieved: %s\n", messageBuffer.message);

            len = strlen(messageBuffer.message);
            printf("Len = %d\n", len);
            messageBuffer.message[len] = rand() % ('z' - 'a') + 'a';
            messageBuffer.message[len + 1] = '\0';
            messageBuffer.type = 3;

            if (msgsnd(messageQueueID, &messageBuffer, strlen(messageBuffer.message) + 1, 0) < 0)
            {
                printf("Cannot send message to queue\n");
                continue;
            }
            printf("Sent: %s\n", messageBuffer.message);
        }

        if (flag == 2)
        {
            if (strlen(messageBuffer.message) >= maxsize + 4)
            {
                printf("Final String: ");
                for (i = 0; i < maxsize + 4; i++)
                {
                    printf("%c", messageBuffer.message[i + 4]);
                }
                printf("\n");

                if (msgctl(messageQueueID, IPC_RMID, NULL) < 0)
                {
                    printf("Cannot delete queue\n");
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
            }

            if (strlen(messageBuffer.message) >= maxsize + 3)
            {
                printf("Finished successfully\n");
                exit(EXIT_SUCCESS);
            }

            if ((usefullData = msgrcv(messageQueueID, &messageBuffer, maxsize + 4, 3, 0)) < 0)
            {
                printf("No usefull data\n");
                exit(-1);
            }
            printf("Usefull Data: %d\n", usefullData);
            printf("Recieved: %s\n", messageBuffer.message);

            len = strlen(messageBuffer.message);
            printf("Len = %d\n", len);
            messageBuffer.message[len] = rand() % ('z' - 'a') + 'a';
            messageBuffer.message[len + 1] = '\0';
            messageBuffer.type = 2;

            if (msgsnd(messageQueueID, &messageBuffer, strlen(messageBuffer.message) + 1, 0) < 0)
            {
                printf("Cannot send message to queue\n");
                continue;
                //exit(EXIT_FAILURE);
            }
            printf("Sent: %s\n", messageBuffer.message);
        }
    }

    return 0;
}
