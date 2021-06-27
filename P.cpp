#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cmath>

int main(void)
{
	int element = -1;
	
	std::cin >> element;
	while (element) {
		std::cin >> element;
		std::pow(element, 3);
		std::cout << element;
	}
	return 0;
}