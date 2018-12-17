#pragma once
#include "layerdatasource.h"

class CDataBaseLayerDataSource :
	public CLayerDataSource
{
public:

	CDataBaseLayerDataSource(void);
public:
	virtual ~CDataBaseLayerDataSource(void);


public:
	// basic interface of the datasource that should be implemented 
	virtual long GetRecoderCount(void) ;
	virtual ShapeUtility::ShapeType getShapeType(void) ;
	virtual CShape* getRecord(long lRecord, CStringArray& aPropertyFields);



};
