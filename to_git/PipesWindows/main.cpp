#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#include <string>
#include <iostream>

#define BUFSIZE 5

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

BOOL start;
//открытие файла
HANDLE handleFile;
void CreateChildProcess(std::string name);
void WriteToPipe(void);
void ReadFromPipe(void);
void ErrorExit(LPCTSTR);

int _tmain(int argc, TCHAR* argv[])
{
	SECURITY_ATTRIBUTES saAttr;
	std::string* elements = new std::string[4];
	elements[0] = "C:\\”чЄба\\OS\\OS\\PipesWindows\\M\\Debug\\M";
	elements[1] = "C:\\”чЄба\\OS\\OS\\PipesWindows\\A\\Debug\\A";
	elements[2] = "C:\\”чЄба\\OS\\OS\\PipesWindows\\P\\Debug\\P";
	elements[3] = "C:\\”чЄба\\OS\\OS\\PipesWindows\\S\\Debug\\S";


	handleFile = CreateFile("text.txt", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	printf("\n->Start of parent execution.\n");

	// Set the bInheritHandle flag so pipe handles are inherited. 

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	g_hChildStd_OUT_Rd = handleFile;
	
	// Create the child process. 

	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			start = true;
		}
		else {
			start = false;
		}

		// Create a pipe for the child process's STDOUT. 
		if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0)) {
			ErrorExit(TEXT("StdoutRd CreatePipe"));
		}
		// Ensure the read handle to the pipe for STDOUT is not inherited.
		if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
			ErrorExit(TEXT("Stdout SetHandleInformation"));
		}
		// Create a pipe for the child process's STDIN. 
		if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) {
			ErrorExit(TEXT("Stdin CreatePipe"));
		}
		// Ensure the write handle to the pipe for STDIN is not inherited. 
		if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)) {
			ErrorExit(TEXT("Stdin SetHandleInformation"));
		}
		CreateChildProcess(elements[i]);
		WriteToPipe();
		ReadFromPipe();
	}

	printf("\n->End of parent execution.\n");
	return 0;
}

void CreateChildProcess(const std::string name)
{
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
	//TCHAR szCmdline[] = TEXT(name.c_str());
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL bSuccess = FALSE;

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.hStdError = g_hChildStd_OUT_Wr;
	si.hStdOutput = g_hChildStd_OUT_Wr;
	si.hStdInput = g_hChildStd_IN_Rd;
	si.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 

	bSuccess = CreateProcess(NULL,
		const_cast<char*>(name.c_str()),     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&si,  // STARTUPINFO pointer 
		&pi);  // receives PROCESS_INFORMATION 

	 // If an error occurs, exit the application. 
	if (!bSuccess) {
		ErrorExit(TEXT("CreateProcess"));
	}
	else
	{
		// Close handles to the child process and its primary thread.
		// Some applications might keep these handles to monitor the status
		// of the child process, for example. 

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		// Close handles to the stdin and stdout pipes no longer needed by the child process.
		// If they are not explicitly closed, there is no way to recognize that the child process has ended.

		CloseHandle(g_hChildStd_IN_Rd);
		CloseHandle(g_hChildStd_OUT_Wr);
	}
}

void WriteToPipe(void)

// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 
{
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;

	for (;;)
	{
		if (start) {
			bSuccess = ReadFile(handleFile, chBuf, BUFSIZE, &dwRead, NULL);
			if (!bSuccess || dwRead == 0) break;
		}
		else {
			bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
			if (!bSuccess || dwRead == 0) break;
		}

		bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, NULL);
		if (!bSuccess) break;
	}

	// Close the pipe handle so the child process stops reading. 

	if (!CloseHandle(g_hChildStd_IN_Wr)) {
		ErrorExit(TEXT("StdInWr CloseHandle"));
	}
}

void ReadFromPipe(void)

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
{
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;

	for (;;)
	{
		bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess || dwRead == 0) break;

		bSuccess = WriteFile(g_hChildStd_IN_Rd, chBuf,
			dwRead, &dwWritten, NULL);
		if (!bSuccess) break;
	}
}

void ErrorExit(LPCTSTR lpszFunction)

// Format a readable error message, display a message box, 
// and exit from the application.
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)& lpMsgBuf,
		0, NULL);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(1);
}