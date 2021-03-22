#include "pch.h"
#include <cmath>
#include <iostream>

Number Vector::PolarDistance() {
	return sqrt(pow(this->x, 2) + pow(this->y, 2));
}

Number Vector::PolarAngle() {
	return Number(atan(this->x / this->y));
}

Vector Vector::Add(Vector v) {
	return Vector(this->x + v.x, this->y + v.y);
}

void Vector::print() {
	std::cout << this->x.value << " " << this->y.value;
}