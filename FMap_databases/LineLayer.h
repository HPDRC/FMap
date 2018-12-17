#pragma once
#include "shapelayer.h"
#include "LabelManager.h"

class CLineLayer :
	public CShapeLayer
{
public:
	CLineLayer(void);
public:
	virtual ~CLineLayer(void);
public:
	bool MergePartsTo(CShapeLayer* pShapeLayer);

public:
	virtual ShapeUtility::ShapeType GetType()
	{
		return ShapeUtility::arc;
	};

	/*
*/

	virtual bool DrawTypeShape(void);
	virtual bool DrawTypeLabel(void);



};
