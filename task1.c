#include <stdio.h>
#include <windows.h>

int main()
{
		STARTUPINFO startInfo;
		PROCESS_INFORMATION procInfo;
		ZeroMemory(&startInfo, sizeof(startInfo));
		startInfo.cb = sizeof(startInfo);
		ZeroMemory(&procInfo, sizeof(procInfo));

		// �������� �������� �������.
		if (!CreateProcess
		(
			NULL,				  // ��� ����� ������ (������������ ��������� ������).
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
			printf("Error! Process can't be created");
			return;
		}

		// ����� �� ��� ���, ���� �������� ������� �� ������ �� ������.
		WaitForSingleObject(procInfo.hProcess, INFINITE);

		// ������� ����������� �������� � ������.
		CloseHandle(procInfo.hProcess);
		CloseHandle(procInfo.hThread);

	return 0;
}