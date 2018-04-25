#include <stdio.h>
#include <Windows.h>
#include <conio.h>

int Data = 0;
//int WriteCount = 0;
int ReadCount = 0;
CRITICAL_SECTION cs;
HANDLE Access, RC;
HANDLE readingSemaphore, semaphore;


DWORD WINAPI Reader(LPVOID lpParam);
DWORD WINAPI Writer(LPVOID lpParam);

int main()
{
    HANDLE threads[10];
    //InitializeCriticalSection(&cs);
    int i;
    //int numbers[5] = { 1, 2, 3, 4, 5 };
    //readingSemaphore = CreateSemaphore(NULL, 0, 100, NULL);
    semaphore = CreateSemaphore(NULL, 0, 100, NULL);
    Access = CreateSemaphore(NULL, 1, 100, NULL);
    RC = CreateSemaphore(NULL, 1, 100, NULL);

    //ReleaseSemaphore(writingSemaphore, 1, NULL);
    for (i = 0; i < 5; i++)
    {
        threads[i] = CreateThread(NULL, 0, &Writer, NULL, 0/*&numbers[i]*/, NULL);
        threads[i + 5] = CreateThread(NULL, 0, &Reader, NULL,0/* &numbers[i]*/, NULL);
    }

    WaitForMultipleObjects(10, threads, TRUE, INFINITE);

    CloseHandle(readingSemaphore);
    CloseHandle(semaphore);
    CloseHandle(Access);
    CloseHandle(RC);
    for (i = 0; i < 10; i++)
    {
        CloseHandle(threads[i]);
    }
    //DeleteCriticalSection(&cs);

    return 0;
}

DWORD WINAPI Reader(LPVOID lpParam)
{
    int i = 0;
    //int n = *(int*)lpParam;

    while (5 > i++) {
        // P(S); V(S);
        WaitForSingleObject(RC, INFINITE);
        ReadCount++;

        if (ReadCount == 1)
        {
            WaitForSingleObject(Access, INFINITE);
        }
        ReleaseSemaphore(RC, 1, NULL);
        Sleep(rand() % 1000 + 1000);
        //EnterCriticalSection(&cs);

        printf("Reader N %d starts writing. ReadCount=%d, Data=%d\n", 1, ReadCount, Data);
        Sleep(3);

        printf("Reader N %d ends writing. ReadCount=%d, Data=%d\n", 1, ReadCount, Data);

        //OpenSemaphore()
        WaitForSingleObject(RC, NULL);
        ReadCount--;
        if (ReadCount == 0)
        {
            ReleaseSemaphore(Access, 1, NULL);
        }
        ReleaseSemaphore(RC, 1, NULL);
        //LeaveCriticalSection(&cs);
        //ReleaseSemaphore(readingSemaphore, 1, NULL);
    }

    ExitThread(EXIT_SUCCESS);
}

DWORD WINAPI Writer(LPVOID lpParam)
{
    int i = 0;
    //int n = *(int*)lpParam;
    while (5 > i++) 
    {
        //P(S); // Резервирование
        WaitForSingleObject(Access, NULL);
        Sleep(rand() % 1000 + 1000);
        
        //EnterCriticalSection(&cs);
        printf("Writer N %d starts writing. ReadCount=%d, Data=%d\n", 1, ReadCount, Data);
        Sleep(3);
        printf("Writer N %d ends writing. ReadCount=%d, Data=%d\n", 1, ReadCount, Data);
        Data++;

        // V(S); // Освобождение
        //LeaveCriticalSection(&cs);
        ReleaseSemaphore(Access, 1, NULL);
    }
    ExitThread(EXIT_SUCCESS);
}
