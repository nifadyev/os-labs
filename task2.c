#include <stdio.h>
#include <windows.h>

volatile int Var;
const int iterations = 100000000;
const int threadsNumber = 20;

DWORD WINAPI PlusPlus(LPVOID lpParam);
DWORD WINAPI MinusMinus(LPVOID lpParam);

int main()
{
	HANDLE threads[20];
	int i;

	printf("Start value of Var: %d\n", Var);

	for (i = 0; i < threadsNumber / 2; i++)
	{
		threads[i] = CreateThread(NULL, 0, &PlusPlus, NULL, 0, NULL);
		threads[i + 10] = CreateThread(NULL, 0, &MinusMinus, NULL, 0, NULL);
	}

	WaitForMultipleObjects(threadsNumber, threads, TRUE, INFINITE);
	
	for (i = 0; i < threadsNumber; i++)
	{
		CloseHandle(threads[i]);
	}
	
	printf("Final value of Var: %d\n", Var);

	return 0;
}

DWORD WINAPI PlusPlus(LPVOID lpParam)
{
	int i;
	for (i = 0; i < iterations; i++)
	{
		Var++;
	}

	ExitThread(0);
}

DWORD WINAPI MinusMinus(LPVOID lpParam)
{
	int i;
	for (i = 0; i < iterations; i++)
	{
		Var--;
	}

	ExitThread(0);
}