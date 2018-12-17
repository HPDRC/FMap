#include "StdAfx.h"
#include "PointLayer.h"

CPointLayer::CPointLayer(void)
{
}

CPointLayer::~CPointLayer(void)
{
}


// Draw the Labels of shape layer 
bool CPointLayer::DrawTypeLabel( )
{
	
	int nCount = (int) getShapeObjectCount();
	for( int i = 0; i< nCount; i++ )
	{
		m_formatPaintTool.DrawLabel( getShapeObject( i ), ShapeUtility::point );
	}
	

	return false;
}

// Drew shape (point, line, polygon) on canvas
bool CPointLayer::DrawTypeShape( )
{

	int nCount = (int) getShapeObjectCount();
	//m_formatPaintTool.picto = ;
	for( int i = 0; i< nCount; i++ )
	{
		m_formatPaintTool.DrawShape( getShapeObject( i ), ShapeUtility::point, "", 0 );
	}
	
	return false;
}


