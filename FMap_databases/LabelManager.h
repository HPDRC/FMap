#pragma once
#include "stdafx.h"
#include "PointShape.h"
#include "lineShape.h"
#include "PolygonShape.h"

CString ExtractPictoFile( CString file, CString dir );


class CLabelManager
{
public:
	CLabelManager(void);
public:
	~CLabelManager(void);

public:

	CString ExtractPointLabel( CPointShape* pPointShape );

	CString ExtractArcLabel( CLineShape* pLineShape );

	CString ExtractAreaLabel( CPolygonShape* pPolygonShape );

	int ExtractAntiAliasingDegree(int* nAADgree);

	bool HasHighlightLabel(CString strL1, CString strL2);

	//CString ExtractPictoFile( CString file );
};
