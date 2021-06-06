#include <windows.h>
#include <stdio.h>
#include <iostream>

int main(void)
{
	CHAR chBuf;
	DWORD dwRead, dwWritten;
	HANDLE hStdin, hStdout;
	BOOL bSuccess;

	/*hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);

	if ((hStdout == INVALID_HANDLE_VALUE) ||
		(hStdin == INVALID_HANDLE_VALUE)) {
		ExitProcess(1);
	}
	*/
	int element = -1;
	// Send something to this process's stdout using printf.
	scanf_s("%d", element);
	while (element) {
		scanf_s("%d", element);
		element *= 7;
		printf("%d ", element);
	}
	return 0;
}