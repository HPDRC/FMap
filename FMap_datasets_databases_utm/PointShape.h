#pragma once
#include "shapeobject.h"

class CPointShape :
	public CShape
{
public:
	CPointShape(void)
	{};
public:
	virtual ~CPointShape(void)
	{
		//CShape::~CShape();
	};
	
public:
	virtual ShapeUtility::ShapeType GetType()
	{
		return ShapeUtility::point;
	};
	virtual double GetLength( void )
	{
		return 0;
	}
	
public:
	
	bool getPosTangentRadianByDistance( double dDistanceFromStart, CDoublePoint* pPosition/*out*/, double* pdRadians/*out*/)
	{
		//ASSERT(false);//delete
		//ASSERT( dDistanceFromStart >= 0 );
		ASSERT( pPosition != NULL );
		ASSERT( pdRadians != NULL );
				

		// the offset of X and  Y
		double dOffsetX = dDistanceFromStart;
		double dOffsetY = 0;//point label's letter always has the same Y 


		//using the arithmetic of similar triangles	
		pPosition->dX =	m_aPointArray[0].dX + dOffsetX;
		pPosition->dY = m_aPointArray[0].dY + dOffsetY;


		//pdRadians always is 0
		*pdRadians = 0;

		return true;
	}

};
