#include "StdAfx.h"
#include "PolygonLayer.h"

CPolygonLayer::CPolygonLayer(void)
{
	strLabel    = "";
	HasHLID     = false;
	HasHLCenter = false;
}

CPolygonLayer::~CPolygonLayer(void)
{
}



// Draw the Labels of shape layer 
bool CPolygonLayer::DrawTypeLabel( )
{
	
	int nCount = (int) getShapeObjectCount();

	for( int i = 0; i< nCount; i++ )
	{
		m_formatPaintTool.DrawLabel( getShapeObject( i ), ShapeUtility::polygon );
	}
	

	return false;
}

// Drew shape (point, line, polygon) on canvas
bool CPolygonLayer::DrawTypeShape( )
{

	int nCount = (int) getShapeObjectCount();
	//for highlight
	CPolygonShape* pPolyShape;
	//int iHighlightIndex = 0;//null
	typedef CArray<int> CIntHLIndexArray;
    CIntHLIndexArray iHLIndexArray;
	int nPen            = 0;//null
    CString strSHPLabel;
	CLabelManager AreaLabel;
	bool isHLLabel = false;

	for( int i = 0; i< nCount; i++ )
	{

		m_formatPaintTool.DrawShape( getShapeObject( i ), ShapeUtility::polygon, "", 0 );

		//for highlight
		if( HasHLID )
		{
			pPolyShape    = (CPolygonShape*) getShapeObject( i );
			strSHPLabel   = AreaLabel.ExtractAreaLabel( pPolyShape );
			isHLLabel     = AreaLabel.HasHighlightLabel(strSHPLabel, strLabel);
			if( isHLLabel )
			{
				//iHighlightIndex = i;
				iHLIndexArray.Add(i);
				nPen            = 8;
			}
		}

	}

	if( 8 == nPen )
	{
		for(int j=0; j<iHLIndexArray.GetSize(); j++)
		{	
			m_formatPaintTool.DrawShape( getShapeObject( iHLIndexArray.GetAt(j) ), ShapeUtility::polygon, "0000FF", nPen);
		}
		//m_formatPaintTool.DrawShape( getShapeObject( iHighlightIndex ), ShapeUtility::polygon, "0000FF", nPen);
	}

	//for highlightCenter
	if( HasHLCenter )
	{
	  CCoordinatesList areacoordinateslist;

	  for(int i = 0; i< nCount; i++)
	  {
	     CalculateAllCoordinates( (CPolygonShape*)getShapeObject(i), &areacoordinateslist, i );
	  }

	  CDoublePoint centercoordinates = GetCenterPoint( this->m_rect );
	  int iHightlightCenter = GetCenterAreaIndex(&areacoordinateslist, centercoordinates);

      m_formatPaintTool.DrawShape( getShapeObject( iHightlightCenter ), ShapeUtility::polygon, "0000FF", 8);

	  areacoordinateslist.RemoveAll();

	}
	
	
	return false;

}

void CPolygonLayer::CalculateAllCoordinates(CPolygonShape* pPShape, CCoordinatesList* pList, int iIndex1 )
{

	double dLengthPolygon = pPShape->GetLength();//double type length of street
	int    nPolygonParts  = 10;//define 10 parts to calculate the center of area
	double dStartDistance = dLengthPolygon / nPolygonParts;//draw label on 1/10 of length
	double dRadianLetter  = 0;
    
	CDoublePoint posPolygonLetterTotalLength, posPolygonCenter, posLetter;//add each part of area into total value

	for(int i = 0; i < nPolygonParts; i++ )
	{
		pPShape->getPosTangentRadianByDistance(
			(i+1)* dStartDistance * (99.0/100.0), &posLetter, &dRadianLetter );

		posPolygonLetterTotalLength.dX = posPolygonLetterTotalLength.dX + posLetter.dX;
		posPolygonLetterTotalLength.dY = posPolygonLetterTotalLength.dY + posLetter.dY;

	}
	
	//Polygon center pos
	posPolygonCenter.dX = posPolygonLetterTotalLength.dX / nPolygonParts;
	posPolygonCenter.dY = posPolygonLetterTotalLength.dY / nPolygonParts;
	posPolygonCenter.iIndex = iIndex1;

	pList->AddHead( posPolygonCenter );

}

int CPolygonLayer::GetCenterAreaIndex(CCoordinatesList* clist, CDoublePoint centerpoint)
{
	ASSERT( clist->GetSize() != 0 );
    //ASSERT( centerpoint != NULL );

	int nAreaCoordinatesNumber = clist->GetSize();
	CDoublePoint temppoint;
	double dMinDistace = 40000;//max distance should be 400
	double dTemp = 0.0;
	int iCenterAreaIndex = 0;

	POSITION pos = clist->GetHeadPosition();
	while( pos != NULL )
	{
		temppoint = clist->GetNext( pos );
		dTemp     = DistanceTwoPoints(temppoint, centerpoint);

		if( dMinDistace >= dTemp)
		{
			dMinDistace      = dTemp;
			iCenterAreaIndex = temppoint.iIndex;
		}
	}

	return iCenterAreaIndex;

}

CDoublePoint CPolygonLayer::GetCenterPoint( CDoubleRect rect )
{
	//ASSERT( rect != NULL);

	CDoublePoint center;
	center.dX = ( (rect.dX2 - rect.dX1) / 2 );
	center.dY = ( (rect.dY2 - rect.dY1) / 2 );

	return center;
}