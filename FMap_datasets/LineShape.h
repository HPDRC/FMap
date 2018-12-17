#pragma once
#include "shapeobject.h"
//#include ""

class CLineShape :
	public CShape
{
private:
	 static double m_dConnectAcceptOffsetX;
	 static double m_dConnectAcceptOffsetY;

public:
	CLineShape(void)
	{
	};
	CLineShape(CShape& shape)
	{
		Assign(shape);
	};


public:
	virtual ~CLineShape(void){
		//CShape::~CShape();

	};

public:
	virtual ShapeUtility::ShapeType GetType()
	{
		return ShapeUtility::arc;
	};

	
	virtual double GetLength( void /* aCoordinateSystem*/)
	{
		
		/*!!! ??????????? pay attention 
		here we can not use this for most of the Coordinate system.
		longititude and latitude system.
		we should use a object aCoordinateSystem as parameter
		then we could Call aCoordinateSystem.Lenght(point1, point2);

		Assuming all system is linear  system(?????) is not correct here. 
		only to simplify system.
		*/

		double TotalDistance = 0;
		double dXOffset = 0;
		double dYOffset = 0;

		int nLinePartsCount = (int) m_aPointArray.GetCount() - 1;
		for(int iVertex = 0; iVertex < nLinePartsCount; iVertex++ )
		{
			dXOffset = m_aPointArray[iVertex].dX - m_aPointArray[iVertex + 1].dX;
			dYOffset = m_aPointArray[iVertex].dY - m_aPointArray[iVertex + 1].dY;
			TotalDistance += sqrt(( dXOffset * dXOffset) + (dYOffset * dYOffset));
		}

		return TotalDistance;
	}
	
	/*
	
	double TotalDistance = 0;
	for( int tVertex = 0; tVertex < (psShape->nVertices - 1); tVertex++ )
	{
		TotalDistance += sqrt(
			(psShape->padfX[tVertex] - psShape->padfX[tVertex + 1]) 
			* (psShape->padfX[tVertex] - psShape->padfX[tVertex + 1])
			+
			(psShape->padfY[tVertex] - psShape->padfY[tVertex + 1]) 
			* (psShape->padfY[tVertex] - psShape->padfY[tVertex + 1]) 
			); // end of square root
	}
	return TotalDistance;
}
	*/
private:


private:
	 //static const int i = 3;

	/*int nIndexEnd1;
	int nIndexEnd2;
	int nCountPoint1;
	int nCountPoint2;

	int nCount;*/

private:
	// not use the  real distance for perfomance "root" need more time
	static bool IsTwoPointConnected(CDoublePoint& pPoint1, CDoublePoint& pPoint2)
	{
		if ( ( abs(pPoint1.dX - pPoint2.dX) < m_dConnectAcceptOffsetX ) && 
			 ( abs(pPoint1.dY - pPoint2.dY) < m_dConnectAcceptOffsetY )
			)
		{
			return true;
		}

		return false;

	}

	

public:
	/*******************************************************
		connect shape2 to shape1, sequence is very important
		 all operation yield to perfomance !!!
			
	*******************************************************/

	 /*friend*/ static bool Connect( CLineShape* pShape1, CLineShape* pShape2, CLineShape* pShapeDest )
	{
		
		ASSERT( pShape1 != NULL );
		ASSERT( pShape2 != NULL );
		ASSERT( pShapeDest != NULL );
		
		int nIndexEnd1 = (int) pShape1->m_aPointArray.GetCount() - 1;
		int nIndexEnd2 = (int) pShape2->m_aPointArray.GetCount() - 1;

		//End VS Start
		if ( IsTwoPointConnected(pShape1->m_aPointArray[nIndexEnd1], pShape2->m_aPointArray[0] ) )
		{
			// ok do not need to rewind
			//LOG("Debug: 1 E-S  \n");
		}
		//End VS End 
		else if ( IsTwoPointConnected(pShape1->m_aPointArray[nIndexEnd1], pShape2->m_aPointArray[nIndexEnd2] ) )
		{
			pShape2->Rewind();
			//LOG("Debug: 2 E-E \n ");
		}
		//Start VS Start  
		else if ( IsTwoPointConnected(pShape1->m_aPointArray[0], pShape2->m_aPointArray[0]) )
		{
			//rewind shape 1
			pShape1->Rewind();
			//LOG("Debug: 3 S-S \n");
		}
		//Start VS End
		else if ( IsTwoPointConnected(pShape1->m_aPointArray[0], pShape2->m_aPointArray[nIndexEnd2] ) )
		{
			//rewind shape 1 and 2
			pShape1->Rewind();
			pShape2->Rewind();
			//LOG("Debug: 4 S-E \n");

		}
		//not connected
		else
		{
			return false; // two objects are not connected!
			//LOG("Debug: 0 NULL \n");
		}
	
		// then combine all into one Shape
		//new 
		
		 Combine( pShape1, pShape2, pShapeDest );	
		 return true;
	}


 private:
	 //calculate the linear distance of two points
	 double LengthTwoPoints(CDoublePoint& pPoint1, CDoublePoint& pPoint2)
	 {
		double dXOffset = pPoint1.dX - pPoint2.dX;
		double dYOffset = pPoint1.dY - pPoint2.dY;
		return  sqrt( ( dXOffset * dXOffset) + (dYOffset * dYOffset) );
	 }

	
public:

	/* Angle in radians.


	get the point position and the tangent radions

	*/
	bool getPosTangentRadianByDistance( double dDistanceFromStart, CDoublePoint* pPosition/*out*/, double* pdRadians/*out*/)
	{
		VERIFY( dDistanceFromStart >= 0 && dDistanceFromStart <= GetLength() );
		ASSERT( pPosition != NULL );
		ASSERT( pdRadians != NULL );
				
		int i = 0;
		double dSumDistance = 0;
		for(i = 0; dSumDistance <=  dDistanceFromStart; i++ )
		{
			dSumDistance += LengthTwoPoints( m_aPointArray[i], m_aPointArray[i + 1] );
		}

		// the exceeding distance is 
		double dExceedingDistance = dSumDistance - dDistanceFromStart; 
		ASSERT( dExceedingDistance >= 0 );

		/* the vertex should be between i -1 and i.
		calculate the distance between i -1 and i.*/
		double dLastPartLineDistance = LengthTwoPoints( m_aPointArray[i], m_aPointArray[i - 1] );
			
		// get the proportin to get the point.
		double tempProportion = dExceedingDistance/dLastPartLineDistance;
		// the offset of X and  Y
		double dOffsetX = m_aPointArray[i].dX - m_aPointArray[i-1].dX;
		double dOffsetY = m_aPointArray[i].dY - m_aPointArray[i-1].dY;
		

		//using the arithmetic of similar triangles	
		pPosition->dX =	m_aPointArray[i].dX - (dOffsetX)*tempProportion;
		pPosition->dY = m_aPointArray[i].dY - (dOffsetY)*tempProportion;

		double dAngleCos = dOffsetX / dLastPartLineDistance;//cos
		if ( dOffsetY >= 0 ) 
		{
			*pdRadians = acos(dAngleCos);
		}
		else
		{
			*pdRadians = 2*PI - acos(dAngleCos);
		}
		return true;
	}


	 





};
