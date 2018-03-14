#include <stdio.h>
#include <windows.h>

int main()
{
		STARTUPINFO startInfo;
		PROCESS_INFORMATION procInfo;
		ZeroMemory(&startInfo, sizeof(startInfo));
		startInfo.cb = sizeof(startInfo);
		ZeroMemory(&procInfo, sizeof(procInfo));

		// Запустим дочерний процесс.
		if (!CreateProcess
		(
			NULL,				  // Нет имени модуля (используется командная строка).
			"notepad.exe",        // Командная строка.
			NULL,                 // Дескриптор процесса не наследуемый.
			NULL,                 // Дескриптор потока не наследуемый.
			FALSE,                // Установим наследование дескриптора в FALSE.
			0,                    // Флажков создания нет.
			NULL,                 // Используйте блок конфигурации родителя.
			NULL,                 // Используйте стартовый каталог родителя.
			&startInfo,           // Указатель на структуру STARTUPINFO.
			&procInfo)            // Указатель на структуру PROCESS_INFORMATION.
			)
		{
			printf("Error! Process can't be created");
			return;
		}

		// Ждать до тех пор, пока дочерний процесс не выйдет из работы.
		WaitForSingleObject(procInfo.hProcess, INFINITE);

		// Закроем дескрипторы процесса и потока.
		CloseHandle(procInfo.hProcess);
		CloseHandle(procInfo.hThread);

	return 0;
}