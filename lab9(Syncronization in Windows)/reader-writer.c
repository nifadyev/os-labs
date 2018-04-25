#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

int Data = 0;
int ReadCount = 0;
HANDLE Access; // Exclusive data access for writing
HANDLE RC; // Data access for reading

DWORD WINAPI Read(LPVOID lpParam);
DWORD WINAPI Write(LPVOID lpParam);

int main()
{
    HANDLE threads[10];
    int i;

    srand(time(NULL));
    Access = CreateSemaphore(NULL, 1, 100, (LPCSTR)"Access");
    RC = CreateSemaphore(NULL, 1, 100, (LPCSTR)"RC");

    for (i = 0; i < 5; i++)
    {
        threads[i] = CreateThread(NULL, 0, Write, NULL, 0, NULL);
        threads[i + 5] = CreateThread(NULL, 0, Read, NULL, 0, NULL);
    }

    WaitForMultipleObjects(10, threads, TRUE, INFINITE);

    for (i = 0; i < 10; i++)
    {
        CloseHandle(threads[i]);
    }
    CloseHandle(Access);
    CloseHandle(RC);

    return 0;
}

DWORD WINAPI Read(LPVOID lpParam)
{
    int i;

    for(i = 0; i < 5; i++)
    {
        //Waiting for 1-2 seconds
        Sleep(rand() % 1000 + 1000);
        WaitForSingleObject(RC, INFINITE);
        ReadCount++;

        if (ReadCount == 1)
        {
            WaitForSingleObject(Access, INFINITE);
        }
        ReleaseSemaphore(RC, 1, NULL);  

        printf("Reader #%d starts reading. ReadCount = %d, Data = %d\n", 1, ReadCount, Data);
        //Waiting for 0,3 second
        Sleep(300);
        printf("Reader #%d ends reading. ReadCount = %d, Data = %d\n", 1, ReadCount, Data);
        WaitForSingleObject(RC, INFINITE);
        ReadCount--;

        if (ReadCount == 0)
        {
            ReleaseSemaphore(Access, 1, NULL);
        }

        ReleaseSemaphore(RC, 1, NULL);
    }

    ExitThread(EXIT_SUCCESS);
}

DWORD WINAPI Write(LPVOID lpParam)
{
    int i;

    for (i = 0; i < 5; i++) 
    {
        //Waiting for 1-2 seconds
        Sleep(rand() % 1000 + 1000);
        WaitForSingleObject(Access, INFINITE);
        printf("Writer #%d starts writing. ReadCount = %d, Data = %d\n", 1, ReadCount, Data);
        //Waiting for 0,3 second
        Sleep(3000);
        Data++;
        printf("Writer #%d ends writing. ReadCount = %d, Data = %d\n", 1, ReadCount, Data);

        ReleaseSemaphore(Access, 1, NULL);
    }

    ExitThread(EXIT_SUCCESS);
}
