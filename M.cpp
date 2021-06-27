#include <windows.h>
#include <stdio.h>
#include <iostream>

int main(void)
{
	int element = -1;
	std::cin >> element;

	while (element) {
		std::cin >> element;
		element *= 7;
		std::cout << element;
	}
	return 0;
}
