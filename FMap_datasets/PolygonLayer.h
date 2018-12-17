#pragma once
#include "shapelayer.h"

class CPolygonLayer :
	public CShapeLayer
{
public:
	CPolygonLayer(void);
public:

	CString strLabel;
	bool HasHLID;
	bool HasHLCenter;
	CDoubleRect m_rect;

	virtual ~CPolygonLayer(void);

public:

	virtual bool DrawTypeShape(void);
	virtual bool DrawTypeLabel(void);

	virtual ShapeUtility::ShapeType GetType()
	{
		return ShapeUtility::polygon;
	};

	void SetHighlightAreaValues(CString strL, bool isHL)
	{
		strLabel = strL;
        HasHLID  = isHL;
	}

	void setHighlightBoundary( CDoubleRect rectBoundary, bool isHLC )
	{
		m_rect      = rectBoundary;
		HasHLCenter = isHLC;
	}

	CDoublePoint GetCenterPoint( CDoubleRect rect );

	//for highlighCenter
	typedef CList<CDoublePoint> CCoordinatesList;
	void CalculateAllCoordinates( CPolygonShape* pPShape, CCoordinatesList* list, int iIndex );
	int GetCenterAreaIndex(CCoordinatesList* clist, CDoublePoint centerpoint);

	double DistanceTwoPoints(CDoublePoint point1, CDoublePoint point2)
	{
		double dXOffset = point1.dX - point2.dX;
		double dYOffset = point1.dY - point2.dY;

		return sqrt( ( dXOffset * dXOffset) + (dYOffset * dYOffset) );
	}

};
