
#pragma once

class Vector
{
	public:
		float x;
		float y;

		Vector(float X, float Y);

		float DistanceBetween( Vector* compare );
};