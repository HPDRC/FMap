#pragma once
#include "layerdatasource.h"
#include <string>

	static const  CString strObjectDelimiter = "_";
	static  const CString strPropertyDelimiter = ",";


class CStringLayerDataSource :
	public CLayerDataSource
{


private:	

public:
	CStringLayerDataSource(void);

private:
	CStringArray m_aStrShapeObject;
	int nShapeOjbectCount;
public:
	bool FromString(CString strData);

public:
	// basic interface of the datasource that should be implemented 
	virtual long GetRecoderCount(void) ;
	virtual ShapeUtility::ShapeType getShapeType(void) ;
	virtual CShape* getRecord(long lRecord, CStringArray& aPropertyFields);

	//virtual bool GetRecordFromMySQL(CShapePtrArray& apShapes);


public:
	virtual ~CStringLayerDataSource(void);


};
