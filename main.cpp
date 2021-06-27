#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#include <string>
#include <iostream>
#include <fstream>

#define BUFSIZE 4086

HANDLE g_hChildStd_IN[5];

void CreateChildProcess(std::string& elem, int i);

int _tmain()
{
	std::string* elements = new std::string[4];
	elements[0] = "C:\\Ó÷¸áà\\OS\\OS\\PipesWindows\\M\\Debug\\M";
	elements[1] = "C:\\Ó÷¸áà\\OS\\OS\\PipesWindows\\A\\Debug\\A";
	elements[2] = "C:\\Ó÷¸áà\\OS\\OS\\PipesWindows\\P\\Debug\\P";
	elements[3] = "C:\\Ó÷¸áà\\OS\\OS\\PipesWindows\\S\\Debug\\S";

	SECURITY_ATTRIBUTES saAttr;
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	HANDLE hFile = CreateFile("Text.txt", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile) {
		return 1;
	}

	g_hChildStd_IN[0] = hFile;
	g_hChildStd_IN[4] = GetStdHandle(STD_OUTPUT_HANDLE);


	bSuccess = ReadFile(g_hChildStd_IN[0], chBuf, BUFSIZE, &dwRead, NULL);
	if (!bSuccess) exit(-3);

	for (int i = 0; i < 4; i++) { 
		CreatePipe(&g_hChildStd_IN[i], &g_hChildStd_IN[i + 1], &saAttr, 0);
		CreateChildProcess(elements[i], i);
	}

	return 0;
}

void CreateChildProcess(std::string& element, int i) {
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_IN[i + 1];
	siStartInfo.hStdOutput = g_hChildStd_IN[i + 1];
	siStartInfo.hStdInput = g_hChildStd_IN[i];
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	bSuccess = CreateProcess(NULL,
		const_cast<char*>(element.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo,   
		&piProcInfo);  

	if (!bSuccess) { exit(-4); }
	else
	{
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);

		CloseHandle(g_hChildStd_IN[i]);
	}
}


