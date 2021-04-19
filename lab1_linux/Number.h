#pragma once

class Number {
public:
	Number(double value);

	double operator+(Number num);
	double operator/(Number num);
	double operator*(Number num);
	double operator-(Number num);
	double operator=(Number num);

	double value;
};

Number MakeNumber(double val);

Number sqrt(Number num);
Number pow(Number num, Number deg);