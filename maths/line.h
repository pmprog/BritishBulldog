
#pragma once

#include "vector2.h"

class Line
{
	public:
		Vector* Points[2];

		Line( Vector* A, Vector* B );
		Line( float x1, float y1, float x2, float y2 );
		~Line();

		float GetSlope();
		float GetIntercept();

		Vector* GetIntersection( Line* IntersectsWith );
};