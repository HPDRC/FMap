#pragma once
#include "shapeobject.h"

class CPolygonShape :
	public CShape
{
public:
	CPolygonShape(void)
	{};
public:
	 virtual ~CPolygonShape(void)
	 {
		CShape::~CShape();
	 };

	virtual ShapeUtility::ShapeType GetType()
	{
		return ShapeUtility::polygon;
	};

	
	virtual double GetLength(void)
	{
		
		double TotalDistance = 0;
		double dXOffset = 0;
		double dYOffset = 0;

		int nCount = (int) m_aPointArray.GetCount();
		for(int iVertex = 0; iVertex < nCount - 1; iVertex++ )
		{
			dXOffset = m_aPointArray[iVertex].dX - m_aPointArray[iVertex + 1].dX;
			dYOffset = m_aPointArray[iVertex].dY - m_aPointArray[iVertex + 1].dY;
			TotalDistance += sqrt(( dXOffset * dXOffset) + (dYOffset * dYOffset));
		}
		// add the last part which starts from end to end
		dXOffset = m_aPointArray[nCount - 1].dX - m_aPointArray[0].dX;
		dYOffset = m_aPointArray[nCount - 1].dY - m_aPointArray[0].dY;
		TotalDistance += sqrt( ( dXOffset * dXOffset) + (dYOffset * dYOffset) );
		

		return TotalDistance;
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


	bool getPosTangentRadianByDistance( double dDistanceFromStart, CDoublePoint* pPosition/*out*/, double* pdRadians/*out*/)
	{
		ASSERT( dDistanceFromStart >= 0 && dDistanceFromStart <= GetLength() );
		ASSERT( pPosition != NULL );
		ASSERT( pdRadians != NULL );
				
		int i = 0;//should initialize it as 0
		double dExceedSumDistance = 0.0;//distance from 0 to i+1
		double dSumDistance = 0.0;//distance from 0 to i
		double dExceedingDistance = 0.0;
		int nArraySize = (int) (m_aPointArray.GetSize());
		bool hasNextPointOverflow = false;

		for(i = 0; dExceedSumDistance < dDistanceFromStart; i++ )
		{
			if( (i+1) >= nArraySize )// if i+1 overflows the array size, then break
			{
				hasNextPointOverflow = true;
				break;
			}
			dExceedSumDistance += LengthTwoPoints( m_aPointArray[i], m_aPointArray[i + 1] );

		}

		if( hasNextPointOverflow )
		{
			pPosition->dX =	m_aPointArray[0].dX;
			pPosition->dY = m_aPointArray[0].dY;
			return true;
		}
		else
		{
			dSumDistance = dExceedSumDistance;/* - LengthTwoPoints( m_aPointArray[i], m_aPointArray[i - 1] )*/
		}
		

		// the exceeding distance is 
		dExceedingDistance = dSumDistance - dDistanceFromStart;
        //dExceedingDistance = dDistanceFromStart - dSumDistance;

		ASSERT( dExceedingDistance >= 0 );

		/* the vertex should be between i -1 and i.
		calculate the distance between i -1 and i.*/
		if(0 >= i)
		{
            return false;
			//i = 1;
		}
		double dLastPartLineDistance = LengthTwoPoints( m_aPointArray[i], m_aPointArray[i - 1] );
			
		// get the proportin to get the point.
		double tempProportion = dExceedingDistance/dLastPartLineDistance;
		// the offset of X and  Y
		double dOffsetX = m_aPointArray[i].dX - m_aPointArray[i-1].dX;
		double dOffsetY = m_aPointArray[i].dY - m_aPointArray[i-1].dY;
		

		//using the arithmetic of similar triangles	
		pPosition->dX =	m_aPointArray[i].dX - (dOffsetX)*tempProportion;
		pPosition->dY = m_aPointArray[i].dY - (dOffsetY)*tempProportion;

		//pdRadians always is 0;
		*pdRadians = 0;

		return true;
	}


public:
	
	bool getPolygonLetterPos( double dDistanceFromStart, double dX, double dY, CDoublePoint* pPosition/*out*/, double* pdRadians/*out*/)
	{
		//ASSERT( dDistanceFromStart >= 0 );
		ASSERT( pPosition != NULL );
		ASSERT( pdRadians != NULL );
				

		// the offset of X and  Y
		double dOffsetX = dDistanceFromStart;
		double dOffsetY = 0;//point label's letter always has the same Y 


		//using the arithmetic of similar triangles	
		pPosition->dX =	dX + dOffsetX;
		pPosition->dY = dY + dOffsetY;


		//pdRadians always is 0
		*pdRadians = 0;

		return true;
	}


};
