#pragma once
#include "shapelayer.h"

class CPointLayer :
	public CShapeLayer
{
public:
	CPointLayer(void);
public:
	virtual ~CPointLayer(void);

public:
	
	virtual bool DrawTypeShape(void);
	virtual bool DrawTypeLabel(void);

	virtual ShapeUtility::ShapeType GetType()
	{
		return ShapeUtility::point;
	};
};
