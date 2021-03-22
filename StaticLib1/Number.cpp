#include "pch.h"
#include "Number.h"
#include <cmath>

Number::Number(double v) {
	this->value = v;
}

double Number::operator+(Number num) {
	return this->value + num.value;
}

double Number::operator/(Number num) {
	return this->value / num.value;
}

double Number::operator*(Number num) {
	return this->value * num.value;
}

double Number::operator-(Number num) {
	return this->value - num.value;
}

double Number::operator=(Number num) {
	return this->value = num.value;
}

Number MakeNumber(double value) {
	return Number(value);
}

Number sqrt(Number num) {
	return Number(sqrt(num.value));
}

Number pow(Number num, Number deg) {
	return Number(std::pow(num.value, deg.value));
}
