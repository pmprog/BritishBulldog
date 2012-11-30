
#include "line.h"

Line::Line( Vector* A, Vector* B )
{
	Points[0] = new Vector( A->x, A->y );
	Points[1] = new Vector( B->x, B->y );
}

Line::Line( float x1, float y1, float x2, float y2 )
{
	Points[0] = new Vector( x1, y1 );
	Points[1] = new Vector( x2, y2 );
}

Line::~Line()
{
	delete Points[1];
	delete Points[0];
}

float Line::GetSlope()
{
	if( Points[0]->x == Points[1]->x )
		return 999999999.0f;
	return (Points[1]->y - Points[0]->y) / (Points[1]->x - Points[0]->x);
}

float Line::GetIntercept()
{
	return Points[0]->y - (GetSlope() * Points[0]->x);
}


Vector* Line::GetIntersection( Line* IntersectsWith )
{
	float m[2];
	float b[2];

	m[0] = GetSlope();
	m[1] = IntersectsWith->GetSlope();
	b[0] = GetIntercept();
	b[1] = IntersectsWith->GetIntercept();

	float tmp;

	if( Points[0]->x == Points[1]->x )
	{
		tmp = (m[1] * Points[0]->x) + b[1];
		if( (tmp >= Points[0]->y && tmp <= Points[1]->y) || (tmp <= Points[0]->y && tmp >= Points[1]->y) )
			return new Vector( Points[0]->x, tmp );
	} else if ( IntersectsWith->Points[0]->x == IntersectsWith->Points[1]->x ) {
		tmp = (m[0] * IntersectsWith->Points[0]->x) + b[0];
		if( (tmp >= IntersectsWith->Points[0]->y && tmp <= IntersectsWith->Points[1]->y) || (tmp <= IntersectsWith->Points[0]->y && tmp >= IntersectsWith->Points[1]->y) )
			return new Vector( IntersectsWith->Points[0]->x, tmp );
	} else {
		tmp = (b[1] - b[0]) / (m[0] - m[1]);
		if( tmp >= (Points[0]->x <= Points[1]->x ? Points[0]->x : Points[1]->x) && (Points[0]->x >= Points[1]->x ? Points[0]->x : Points[1]->x) )
			if( tmp >= (IntersectsWith->Points[0]->x <= IntersectsWith->Points[1]->x ? IntersectsWith->Points[0]->x : IntersectsWith->Points[1]->x) && (IntersectsWith->Points[0]->x >= IntersectsWith->Points[1]->x ? IntersectsWith->Points[0]->x : IntersectsWith->Points[1]->x) )
				return new Vector( tmp, (m[0] * tmp) + b[0] );
	}

	return 0;
}
