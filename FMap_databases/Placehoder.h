#pragma once



class CCircle
{

public:
	CCircle()
	{
		nXCenter = 0;
		nYCenter = 0;
		nRadius  = 0;
	}
	CCircle(int nXCenter, int nYCenter, int nRadius)
	{
		this->nXCenter =	nXCenter;
		this->nYCenter =	nYCenter;
		this->nRadius  =	nRadius ;
	}
	
	CCircle(const CCircle& circle)
	{
		nXCenter = circle.nXCenter;
		nYCenter = circle.nYCenter;
		nRadius  = circle.nRadius; 
	}
public:
	
	int nXCenter;
	int nYCenter;
	int nRadius;

};

typedef CList<CCircle> CCircleList;

class CPlacehoder
{
private:
	CCircleList m_holderCircleList;
	CDoubleRect m_rectBoundary;

public:
	bool hasBigSpaceForLabelConflict;
	bool isPointPlaceholder;


public:
	CPlacehoder(void)
	{
	};

public:
	~CPlacehoder(void)
	{
		m_holderCircleList.RemoveAll();

	};
public:
	void SetBoundary(CDoubleRect& rectBoundary )
	{
		m_rectBoundary = rectBoundary;
	}


public:
	// add a circle holder into the place holder
	void AddHolder(const CCircle& holderCircle)
	{
	
		////CCircle* pHolderToAdd = new CCircle(holderCircle);
		//if ( pHolderToAdd == NULL )
		//{
		//	ASSERT( false );
		//	LOG( "Fatal Error: Insufficent memory" );
		//	exit( 200 );
		//}
		m_holderCircleList.AddHead( holderCircle );
	};
	void AddHolder(const CCircleList& holderCircleList)
	{
		
		POSITION pos = holderCircleList.GetHeadPosition();
		while( pos != NULL )
		{
			m_holderCircleList.AddHead(holderCircleList.GetNext( pos )); 
		}

	};
public:
	// Is Place Available
	bool IsPlaceAvailable(CCircle& holderCircle)
	{
		POSITION pos = m_holderCircleList.GetHeadPosition();
		while( pos != NULL )
		{
			CCircle circleExist = m_holderCircleList.GetNext( pos ); 
			if (IsTwoCircleInterlaced(circleExist, holderCircle))
			{
				return false; // overlaped
			}
		}

		return true;
	};

	bool IsPlaceAvailable(const CCircleList& holderCircleList)
	{
/////////////////////////////////////////////////////////////////////
		POSITION posInputed = holderCircleList.GetHeadPosition();
		POSITION posRestInputed;
		
		while ( posInputed != NULL )
		{
		
			CCircle circleInputed = holderCircleList.GetNext( posInputed ); 

			if ( IsCircleInBoundary( circleInputed ) == false )
			{
				return false;

			}	


			////////////////////////////////////////////////////
            //conflict with label-self
			posRestInputed = posInputed;
			while( posRestInputed != NULL )
			{	
				CCircle circleRestInputed = holderCircleList.GetNext( posRestInputed );
				if (IsSelfTwoCircleInterlaced(circleInputed, circleRestInputed))
				{
					return false; // overlaped
				}	
			}
			////////////////////////////////////////////////////



			//m_holderCircleList.AddHead(CCircle(0,0,0));
			POSITION posExist = m_holderCircleList.GetHeadPosition();
			while( posExist != NULL )
			{			
				CCircle circleExist = m_holderCircleList.GetNext( posExist ); 
				if (IsTwoCircleInterlaced(circleExist, circleInputed))
				{
					return false; // overlaped
				}			
			}

		}
		return true;// no overlap 

	};


private:

	bool IsSelfTwoCircleInterlaced( CCircle& circle1, CCircle& circle2 )
	{
		int nXOffset = circle1.nXCenter - circle2.nXCenter;
		int nYOffset = circle1.nYCenter - circle2.nYCenter;


		int nDisTwoCenter = (circle1.nRadius + circle2.nRadius);

		if (( (nXOffset * nXOffset) + (nYOffset * nYOffset)) > 
			( nDisTwoCenter * nDisTwoCenter ) )
		{
			return false;
		}
		else
		{
			return true;
		}
	}


	bool IsTwoCircleInterlaced( CCircle& circle1, CCircle& circle2 )
	{
		int nXOffset = circle1.nXCenter - circle2.nXCenter;
		int nYOffset = circle1.nYCenter - circle2.nYCenter;
        int nDisTwoCenter = 0;

		if( hasBigSpaceForLabelConflict )
		{
            nDisTwoCenter = (int) (10 * (circle1.nRadius + circle2.nRadius));//10 times distance for SM
		}
		else if( isPointPlaceholder )
		{
			nDisTwoCenter = (int) (1.2 * (circle1.nRadius + circle2.nRadius));//1.2 times distance for point  
		}
		else
		{
			nDisTwoCenter = (int) (2 * (circle1.nRadius + circle2.nRadius));//2 times distance for line
		}

		if (( (nXOffset * nXOffset) + (nYOffset * nYOffset)) > 
			( nDisTwoCenter * nDisTwoCenter ) )
		{
			return false;
		}
		else
		{
			return true;
		}
	}




public:
	///////////////////////////////////////
	//new codes
	bool IsCircleInBoundary( CCircle& circle)
	{

		double	dCircleMinX = circle.nXCenter - circle.nRadius;
		double	dCircleMaxX = circle.nXCenter + circle.nRadius;
		double	dCircleMinY = circle.nYCenter - circle.nRadius;  //caution
		double	dCircleMaxY = circle.nYCenter + circle.nRadius;  //caution

		if( ( dCircleMinX > m_rectBoundary.dX1 ) &&
            ( dCircleMinX < m_rectBoundary.dX2 ) &&

			( dCircleMinY > m_rectBoundary.dY1 ) && 
            ( dCircleMinY < m_rectBoundary.dY2 ) &&

			( dCircleMaxX < m_rectBoundary.dX2 ) &&
			( dCircleMaxX > m_rectBoundary.dX1 ) &&
			
			( dCircleMaxY < m_rectBoundary.dY2 ) &&
            ( dCircleMaxY > m_rectBoundary.dY1 )
			)
		{
			return true;
		}

		return false;

	}
	///////////////////////////////////////


};
