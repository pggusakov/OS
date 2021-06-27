#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#include <string>
#include <iostream>
#include <fstream>

#define BUFSIZE 4086

HANDLE g_hChildStd_IN_Rd[5];
HANDLE g_hChildStd_IN_Wr[4];
HANDLE g_hChildStd_OUT_Rd[4];
HANDLE g_hChildStd_OUT_Wr[4];

void CreateChildProcess(std::string& elem, int i);
void WriteToPipe(int i);
void ReadFromPipe(int i);
void ErrorExit(PCTSTR);

int _tmain()
{
	std::string* elements = new std::string[4];
	elements[0] = "C:\\Ó÷¸áà\\OS\\OS\\PipesWindows\\M\\Debug\\M";
	elements[1] = "C:\\Ó÷¸áà\\OS\\OS\\PipesWindows\\A\\Debug\\A";
	elements[2] = "C:\\Ó÷¸áà\\OS\\OS\\PipesWindows\\P\\Debug\\P";
	elements[3] = "C:\\Ó÷¸áà\\OS\\OS\\PipesWindows\\S\\Debug\\S";

	SECURITY_ATTRIBUTES saAttr;

	// Set the bInheritHandle flag so pipe handles are inherited. 

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	HANDLE hFile = CreateFile("Text.txt", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile) {
		return 1;
	}

	g_hChildStd_IN_Rd[0] = hFile;
	g_hChildStd_IN_Rd[4] = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < 4; i++) {
		// Create a pipe for the child process's STDOUT. 
		CreatePipe(&g_hChildStd_OUT_Rd[i], &g_hChildStd_OUT_Wr[i], &saAttr, 0);

		// Create a pipe for the child process's STDIN. 
		CreatePipe(&g_hChildStd_IN_Rd[i], &g_hChildStd_IN_Wr[i], &saAttr, 0);

		// Create the child process. 

		CreateChildProcess(elements[i], i);

		// Write to the pipe that is the standard input for a child process. 
		// Data is written to the pipe's buffers, so it is not necessary to wait
		// until the child process is running before writing data.

		WriteToPipe(i);

		// Read from pipe that is the standard output for child process. 

		ReadFromPipe(i);
	}

	return 0;
}

void CreateChildProcess(std::string& element, int i) {
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr[i];
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr[i];
	siStartInfo.hStdInput = g_hChildStd_IN_Rd[i];
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 

	bSuccess = CreateProcess(NULL,
		const_cast<char*>(element.c_str()),     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

	 // If an error occurs, exit the application. 
	if (!bSuccess) {}
	else
	{
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);

		// Close handles to the stdin and stdout pipes no longer needed by the child process.
		// If they are not explicitly closed, there is no way to recognize that the child process has ended.

		CloseHandle(g_hChildStd_OUT_Wr[i]);
		CloseHandle(g_hChildStd_IN_Rd[i]);
	}
}


void WriteToPipe(int i) {
// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 

	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;

	for (;;)
	{
		bSuccess = ReadFile(g_hChildStd_IN_Rd[i], chBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess || dwRead == 0) break;

		bSuccess = WriteFile(g_hChildStd_IN_Wr[i], chBuf, dwRead, &dwWritten, NULL);
		if (!bSuccess) break;
	}

	// Close the pipe handle so the child process stops reading. 

	CloseHandle(g_hChildStd_IN_Wr[i]);
}

void ReadFromPipe(int i) {

	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;

	for (;;)
	{
		bSuccess = ReadFile(g_hChildStd_OUT_Rd[i], chBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess || dwRead == 0) break;

		bSuccess = WriteFile(g_hChildStd_IN_Rd[i + 1], chBuf,
			dwRead, &dwWritten, NULL);
		if (!bSuccess) break;
	}
}