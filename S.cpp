#include <windows.h>
#include <stdio.h>
#include <iostream>

int main(void)
{
	int element = -1, sum = 0;
	std::cin >> element;

	while (element) {
		std::cin >> element;
		sum += element;
	}
	std::cout << sum;
	return 0;
}