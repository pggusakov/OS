#pragma once
#include "../StaticLib1/Number.h"

#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif

extern "C" VECTOR_API class Vector {
public:
	VECTOR_API Vector(Number x, Number y) :x(x), y(y) {};

	VECTOR_API Number PolarDistance();
	VECTOR_API Number PolarAngle();
	VECTOR_API Vector Add(Vector v);
	VECTOR_API void print();

	Number x;
	Number y;
};
