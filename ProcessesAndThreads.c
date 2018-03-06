#include <stdio.h> 
#include <windows.h> 

volatile long long int Var/* = 0*/;
const int iterations = 100000000; // 100'000'000
const int threadsNumber = 20;

void Task1();
DWORD WINAPI ThreadProcPlus(LPVOID lpParam);
DWORD WINAPI ThreadProcMinus(LPVOID lpParam);
void Task2();



int main()
{
	Task1();
	Task2();

	return 0;
}



void Task1()
{
	STARTUPINFO startInfo;
	PROCESS_INFORMATION procInfo;
	ZeroMemory(&startInfo, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);
	ZeroMemory(&procInfo, sizeof(procInfo));

	// �������� �������� �������.
	if (!CreateProcess
	(
		NULL,                 // ��� ����� ������ (������������ ��������� ������).
		"notepad.exe",        // ��������� ������.
		NULL,                 // ���������� �������� �� �����������.
		NULL,                 // ���������� ������ �� �����������.
		FALSE,                // ��������� ������������ ����������� � FALSE.
		0,                    // ������� �������� ���.
		NULL,                 // ����������� ���� ������������ ��������.
		NULL,                 // ����������� ��������� ������� ��������.
		&startInfo,           // ��������� �� ��������� STARTUPINFO.
		&procInfo)            // ��������� �� ��������� PROCESS_INFORMATION.
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}
	else
	{
		printf("pid: %lu\n", procInfo.dwProcessId);
		//printf("thread_id: %lu\n", procInfo.dwThreadId);
	}

	// ����� �� ��� ���, ���� �������� ������� �� ������ �� ������.
	WaitForSingleObject(procInfo.hProcess, INFINITE);

	// ������� ����������� �������� � ������.
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);
}

DWORD WINAPI ThreadProcPlus(LPVOID lpParam)
{
	DWORD i;

	for (i = 0; i < iterations; i++) 
	{
		Var++;
	}

	ExitThread(0);
}

DWORD WINAPI ThreadProcMinus(LPVOID lpParam)
{
	DWORD i;

	for (i = 0; i < iterations; i++)
	{
		Var--;
	}

	ExitThread(0);
}

void Task2()
{
	HANDLE threads[20];
	DWORD i;

	printf("Startup value of Var: %i\n", Var);

	for (i = 0; i < threadsNumber; i += 2)
	{
		threads[i] = CreateThread(NULL, 0, &ThreadProcPlus, NULL, 0, NULL);
		threads[i + 1] = CreateThread(NULL, 0, &ThreadProcMinus, NULL, 0, NULL);
	}

	WaitForMultipleObjects(threadsNumber, threads, TRUE, INFINITE);

	printf("Final value of Var: %i\n", Var);

	for (i = 0; i < threadsNumber; i++)
	{
		CloseHandle(threads[i]);
	}
}

