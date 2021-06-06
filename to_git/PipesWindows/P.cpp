#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cmath>

#define BUFSIZE 4096 

int main(void)
{
	CHAR chBuf[BUFSIZE];
	DWORD dwRead, dwWritten;
	HANDLE hStdin, hStdout;
	BOOL bSuccess;

	/*hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);

	if (
		(hStdout == INVALID_HANDLE_VALUE) ||
		(hStdin == INVALID_HANDLE_VALUE)
		)
		ExitProcess(1);
	*/

	int element = 0;
	// Send something to this process's stdout using printf.
	scanf_s("%d", element);
	while (element) {
		scanf_s("%d", element);
		std::pow(element, 3);
		printf("%d ", element);
	}

	/*for (;;)
	{
		// Read from standard input and stop on error or no data.
		bSuccess = ReadFile(hStdin, chBuf, BUFSIZE, &dwRead, NULL);

		if (!bSuccess || dwRead == 0) {
			break;
		}

		// Write to standard output and stop on error.
		bSuccess = WriteFile(hStdout, chBuf, dwRead, &dwWritten, NULL);

		if (!bSuccess) {
			break;
		}
	}*/
	return 0;
}