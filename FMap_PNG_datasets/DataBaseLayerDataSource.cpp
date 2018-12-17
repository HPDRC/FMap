#include "DataBaseLayerDataSource.h"

CDataBaseLayerDataSource::CDataBaseLayerDataSource(void)
{
}

CDataBaseLayerDataSource::~CDataBaseLayerDataSource(void)
{
}

long  CDataBaseLayerDataSource::GetRecoderCount(void)
{
	ASSERT(false);// no implemneted
	return 0;
}

ShapeUtility::ShapeType CDataBaseLayerDataSource::getShapeType(void) 
{
	ASSERT(false);// no implemneted	
	return ShapeUtility::arc;
}
CShape*  CDataBaseLayerDataSource::getRecord(long lRecord, CStringArray& aPropertyFields)
{
	ASSERT(false);// no implemneted
	return NULL;
}

