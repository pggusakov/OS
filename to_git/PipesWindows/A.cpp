#include <windows.h>
#include <stdio.h>
#include <iostream>

int main(void)
{
	CHAR chBuf[sizeof(int)];
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

	int element = -1;
	// Send something to this process's stdout using printf.
	scanf_s("%d", element);
	while (element) {
		scanf_s("%d", element);
		element += 4;
		printf("%d ", element);
	}

	/*for (;;)
	{
		// Read from standard input and stop on error or no data.
		bSuccess = ReadFile(hStdin, chBuf, sizeof(int), &dwRead, NULL);

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