#pragma once

#include "doublerect.h"

/*

this class presents a shapeOjbect
*/
class CDoublePoint;

/*
Warning!!! using the data structure may cause the performance problem!!!!
Could retrive raw data, it will call function "getAt" 
each time though it is inline function.

CArry::m_pData[nIndex] to improve it

*/
//typedef CArray<CDoublePoint,CDoublePoint> CPointArray;

class CShapeLayer;

class CShape
{


public: //data

	//CArray<CString> 
	CStringArray m_aProperties;
	CPointArray m_aPointArray;


	//..............
	//int* nStartNum;
	CIntegerArray m_aPartsStartingIndex;
	int m_nParts;


	//for new PointsArray
	CPointArray m_aRectPointArray;
	//CDoubleRect rectExtend;





public:
	CShape(void)
	{
		m_nParts = 0;
	};

	CShape(CShape& shape)
	{
		Assign(shape);
	};

public:
	virtual ~CShape(void)
	{
		//Warning !!! Performence or memory leak!!!
		clearAllDate();
	};



private:
	void clearAllDate()
	{
		m_aPointArray.RemoveAll();
		m_aProperties.RemoveAll();
		m_aPartsStartingIndex.RemoveAll();

		m_aRectPointArray.RemoveAll();
	}
public:

	/*virtual bool SetExtend(CDoubleRect rect)
	{	
	rectExtend = rect;
	return true;

	};
	CDoubleRect GetExtend()
	{
	return this->rectExtend;
	};*/


public:
	// get the length of this ShapeObject
	virtual long getPointCount(void)
	{
		return (long)m_aPointArray.GetCount();
	}

	virtual long getPropertiesCount(void)
	{
		return (long)m_aProperties.GetCount();
	}


	virtual double GetLength()
	{
		VERIFY(false); 
		return 0;
	};
	//virtual double GetLength(ShapeUtility::ShapeType shapeType);
	virtual ShapeUtility::ShapeType GetType() 
	{ 
		return ShapeUtility::unknown;
	};

protected:
	void  Assign(CShape& shapeObjectSrc)
	{
		clearAllDate(); //memory leak?
		int nCountPoints = shapeObjectSrc.getPointCount();
		int nCountProperties = shapeObjectSrc.getPropertiesCount();

		m_aPointArray.SetSize(nCountPoints);

		for( int i = 0; i< nCountPoints; i++ )
		{
			m_aPointArray[i] = shapeObjectSrc.m_aPointArray[i];
		}

		m_aProperties.SetSize(nCountProperties);

		for( int j = 0; j< nCountProperties; j++ )
		{
			m_aProperties[j] = shapeObjectSrc.m_aProperties[j];
		}
	}

public:
	CShape& operator=(CShape& shapeObjectSrc){
		Assign(shapeObjectSrc);
		return *this;
	}
	// This function will reverse any rings 

public:

	//virtual friend CShape operator+( const CShape& shapeOther ) = 0

	//rewind the shape object's points
	virtual void Rewind(void)
	{

		int nCount = (int) m_aPointArray.GetCount();
		CDoublePoint pointSwap;

		int nHalfCount = nCount /2;
		int nIndexNum = nCount - 1;
		for(int i = 0; i < nHalfCount; i++ )
		{

			pointSwap = m_aPointArray[i];
			m_aPointArray[i] = m_aPointArray[ nIndexNum - i];
			m_aPointArray[ nIndexNum - i] = pointSwap;
		}

	};

	/*******************************************************
	append shape2 to shape1, sequence is very important
	*******************************************************/
	static void Combine( CShape* pShape1, CShape* pShape2, CShape* pShapeDest )
	{

		CShape* pShapeCombine = pShapeDest;
		ASSERT( pShapeCombine != NULL);
		if ( pShapeCombine == NULL )
		{
			LOG( "Fatal Error, invalid  NULL pointer" );
			VERIFY(false);
			exit(10);
		}

		int nCountPoint1 = (int) pShape1->m_aPointArray.GetCount();
		int nCountPoint2 = (int) pShape2->m_aPointArray.GetCount();

		pShapeCombine->m_aPointArray.SetSize( nCountPoint1 + nCountPoint2 );

		for (int i= 0; i < nCountPoint1; i++ )
		{
			pShapeCombine->m_aPointArray[i].dX = pShape1->m_aPointArray[i].dX;
			pShapeCombine->m_aPointArray[i].dY = pShape1->m_aPointArray[i].dY;
		}

		for (int j= 0; j < nCountPoint2; j++ )
		{
			pShapeCombine->m_aPointArray[ nCountPoint1 + j ].dX = pShape2->m_aPointArray[j].dX;
			pShapeCombine->m_aPointArray[ nCountPoint1 + j ].dY = pShape2->m_aPointArray[j].dY;
		}


		//then combine the properties object...!!! only for Name is ok here!!! ???
		int nCountProperties = pShape1->getPropertiesCount();
		(pShapeCombine->m_aProperties).SetSize(nCountProperties);

		for( int j = 0; j< nCountProperties; j++ )
		{
			(pShapeCombine->m_aProperties)[j] = pShape1->m_aProperties[j];
		}

	}

	// combine two objects together

	//virtual /*friend*/ bool Connect( CShape* pShape1, CShape* pShape2 )
	//{

	//	ASSERT(false);
	//	return false;
	//}

	int MergeVertices( 
		bool isLeft, bool isRight,bool isUpper,bool isBottom,bool isQuad,
		int i, int nCount,double dX, double dY, double* pdFirstX, double* pdFirstY, 
		double* pdLastX,  double* pdLastY, CDoubleRect rect )
	{	
		int num = 0;

		if( ((int)*pdFirstX == 0) && ((int)*pdFirstY == 0) )
		{
			*pdFirstX = dX;
			*pdFirstY = dY;

			CDoublePoint newpoint(*pdFirstX, *pdFirstY);

			this->m_aRectPointArray.Add(newpoint);

			num++;
		}
		else
		{
			*pdLastX = dX;
			*pdLastY = dY;
			if(i + 1 < nCount)
			{
				if( (this->m_aPointArray[i+1].dX < rect.dX1) && isLeft ) 
				{
					//continue
				}
				else if((this->m_aPointArray[i+1].dX > rect.dX2) && isRight)
				{
					//continue
				}
				else if((this->m_aPointArray[i+1].dX > rect.dX1) &&
					(this->m_aPointArray[i+1].dX < rect.dX2) &&
					(this->m_aPointArray[i+1].dY < rect.dY1) &&
					isBottom)
				{
					//continue
				}
				else if((this->m_aPointArray[i+1].dX > rect.dX1) &&
					(this->m_aPointArray[i+1].dX < rect.dX2) &&
					(this->m_aPointArray[i+1].dY > rect.dY2) &&
					isUpper)
				{
					//continue
				}
				else 
				{
					CDoublePoint newpoint(*pdLastX, *pdLastY);
					this->m_aRectPointArray.Add(newpoint);
					*pdFirstX = 0.0;
					*pdFirstY = 0.0;

					num++;
				}
			}
		}
		return num;
	}

	////////////////////////////////////////////////////////////////////////

	bool ReduceOuterPoints(CDoubleRect rect)
	{	
		//CPointArray aPointArrayTemp;
		//aPointArrayTemp.Copy( m_aPointArray );

		double dX    = 0.0;
		double dY    = 0.0;

		bool isLeft   = false;
		bool isRight  = false;
		bool isUpper  = false;
		bool isBottom = false;
		bool isQuad   = false;

		double dFirstLeftX   = 0.0;
		double dFirstLeftY   = 0.0;
		double dLastLeftX    = 0.0;
		double dLastLeftY    = 0.0;

		double dFirstRightX  = 0.0;
		double dFirstRightY  = 0.0;
		double dLastRightX   = 0.0;
		double dLastRightY   = 0.0;

		double dFirstUpperX  = 0.0;
		double dFirstUpperY  = 0.0;
		double dLastUpperX   = 0.0;
		double dLastUpperY   = 0.0;

		double dFirstBottomX = 0.0;
		double dFirstBottomY = 0.0;
		double dLastBottomX  = 0.0;
		double dLastBottomY  = 0.0;

		int totalnum = 0;

		int nCount   = (int) this->m_aPointArray.GetCount();

		for(int i=0; i < nCount; i++)
		{
			dX = this->m_aPointArray[i].dX;
			dY = this->m_aPointArray[i].dY;

			if( dX < rect.dX1 )
			{
				//if( ((int)dFirstLeftX == 0) && ((int)dFirstLeftY == 0) )
				//{
				//	dFirstLeftX = dX;
				//	dFirstLeftY = dY;

				//	this->m_aRectPointArray[i].dX = dX;
				//	this->m_aRectPointArray[i].dY = dY;
				//}
				//else
				//{
				//	dLastLeftX = dX;
				//	dLastLeftY = dY;
				//	if(i + 1 <= nCount)
				//	{
				//		if( (pShape->m_aPointArray[i+1].dX < rect.dX1) ) 
				//		{
				//			//continue;
				//		}
				//		else 
				//		{
				//			this->m_aRectPointArray[i].dX = dLastLeftX;
				//			this->m_aRectPointArray[i].dY = dLastLeftY;
				//			dFirstLeftX = 0.0;
				//			dFirstLeftY = 0.0;
				//		}
				//	}
				//}
				isLeft = true;

				totalnum += MergeVertices( 
					isLeft, isRight,isUpper,isBottom,isQuad,
					i, nCount, dX, dY, 
					&dFirstUpperX, &dFirstUpperY, 
					&dLastUpperX,  &dLastUpperY, rect );

				isLeft = false;

			}
			else if( dX > rect.dX2)
			{
				isRight = true;

				totalnum += MergeVertices( 
					isLeft, isRight,isUpper,isBottom,isQuad,
					i, nCount, dX, dY, 
					&dFirstUpperX, &dFirstUpperY, 
					&dLastUpperX,  &dLastUpperY, rect );

				isRight = false;
			}
			else
			{
				//if( dY < rect.dY1 )
				//{	
				//	isBottom = true;

				//	totalnum += MergeVertices(
				//		isLeft, isRight,isUpper,isBottom,isQuad,
				//		i, nCount, dX, dY, 
				//		&dFirstUpperX, &dFirstUpperY, 
				//		&dLastUpperX,  &dLastUpperY, rect );

				//	isBottom = false;

				//}
				//else if( dY > rect.dY2)
				//{
				//	isUpper = true;

				//	totalnum += MergeVertices( 
				//		isLeft, isRight,isUpper,isBottom,isQuad,
				//		i, nCount, dX, dY, 
				//		&dFirstBottomX, &dFirstBottomY, 
				//		&dLastBottomX,  &dLastBottomY, rect );

				//	isUpper = false;
				//}
				//else//quad
				{
					isQuad = true;

					CDoublePoint newpoint(dX, dY);
					this->m_aRectPointArray.Add(newpoint);
					isQuad = false;
					totalnum++;
				}
			}
			//return true;
		}

		totalnum;
		//this->m_aRectPointArray.Copy( m_aPointArray );
		/*dX = this->m_aRectPointArray[0].dX;
		dY = this->m_aRectPointArray[0].dY;
		CDoublePoint newpoint(dX, dY);
		this->m_aRectPointArray.Add(newpoint);*/
		return true;
	}

};
