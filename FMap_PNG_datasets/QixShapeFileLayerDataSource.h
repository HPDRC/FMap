#pragma once
#include "layerdatasource.h"

class CQixShapeFileLayerDataSource :public CLayerDataSource
{

private:
	ShapeRecords records;
	CDoubleRect rectExtend;
	

public:	
	ShapeRecords& getShapeRecords(){ return records; }
	void setShapeRecords(ShapeRecords& shapeRecords)
	{
		records = shapeRecords;
	}

	

public:
	CQixShapeFileLayerDataSource(void);
    CQixShapeFileLayerDataSource(CString fileName, CString& sAccessMode = CString("rb"));

	///////////////////////////////////////////
	bool MergeVertices( int start, int nCount,
		double dX, double dY, 
		double* pdFirstX, double* pdFirstY, 
		double* pdLastX,  double* pdLastY, 
		CDoubleRect rect, CShape*  pShape);

	bool ReduceOuterPoints(CDoubleRect rect, CShape*  pShape);
	//////////////////////////////////////////


public:
	virtual ~CQixShapeFileLayerDataSource(void);

public:
	bool FromFile(CString fileName,  CString& sAccessMode = CString("rb"));



public:
	// basic interface of the datasource that should be implemented 
	virtual long GetRecoderCount(void);
	virtual ShapeUtility::ShapeType getShapeType(void);
	virtual CShape* getRecord(long lRecord, CStringArray& aPropertyFields);

	
	//		
	virtual bool SetExtend(CDoubleRect rect);
	CDoubleRect GetExtend();

private:
	SHPHandle 	hSHPFile;
	DBFHandle 	hDBFFile;

	SHPHandle GetSHPHandle(void);
	DBFHandle GetDBFHandle(void);

};
