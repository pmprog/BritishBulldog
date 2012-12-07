
#include "vector2.h"
#include <math.h>

Vector::Vector(float X, float Y)
{
	x = X;
	y = Y;
}

float Vector::DistanceBetween( Vector* compare )
{
	return sqrt( ((compare->x - x) * (compare->x - x)) + ((compare->y - y) * (compare->y - y)) );
}
