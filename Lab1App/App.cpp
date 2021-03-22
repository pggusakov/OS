#include <iostream>
#include "Number.cpp"
#include "..//Vector/Vector.h"

int main() {
	Number first = MakeNumber(6);
	Number second = MakeNumber(3);
	std::cout <<"Numbers: "<<first.value<<" "<<second.value
		<< "\nOperator +: " << first + second << "\n"
		<< "Operator *: " << first * second << "\n"
		<< "Operator /: " << first / second << "\n"
		<< "Operator -: " << first - second << "\n";
	Vector v1(first, second);
	std::cout << "Vector1 coordinates: " << v1.x.value << " " << v1.y.value;
	std::cout << "\nPolar coordinates: " << v1.PolarDistance().value << " " << v1.PolarAngle().value<<"\n";
	Vector v2(Number(2), Number(4));
	std::cout<<"Vector v2: "<< v2.x.value << " " << v2.y.value;
	std::cout << "\nVector v1+v2 coordinates: ";
	v1.Add(v2).print();
}

