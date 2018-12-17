#include "stdafx.h"
#include "doubleRect.h"
#include "shaperecords.h"
#include "ShapeLayer.h"
#include "ShapeObject.h"

#pragma once

class CDoubleRect;
class CShape;

// presents a DataSource //could be regarded as a O-O wrap of shaplib
class CLayerDataSource
{

public: //data
	CDoubleRect rectExtend; // the extend of the data 

public:
	CLayerDataSource(void);


public:
	~CLayerDataSource(void);


	
public:
	
	virtual bool SetExtend(CDoubleRect rectExtend);

	virtual CDoubleRect GetExtend()
	{
		return this->rectExtend;
	};






public:
	// basic interface of the datasource that should be implemented 
	virtual long GetRecoderCount(void) = 0;
	virtual ShapeUtility::ShapeType getShapeType(void) =0 ;
	virtual CShape* getRecord(long lRecord, CStringArray& aPropertyFields) = 0;

	//virtual bool GetRecordFromMySQL(CShapePtrArray& apShapes) = 0;

protected:
	//TODO: implement two functions !!!
	//void* getSHPRecord(long lRecord);
	//void* getDBFRecord(long lRecord);


	

	/*
	SHPRewindObject()
	int SHPRewindObject( SHPHandle hSHP, SHPObject *psObject );

	hSHP:                 The shapefile (not used at this time).
	psObject:		The object to deallocate.

	This function will reverse any rings necessary in order to enforce 
	the shapefile restrictions on the required order of inner and outer rings 
	in the Shapefile specification. It returns TRUE if a change is made and FALSE 
	if no change is made. Only polygon objects will be affected though any object 
	may be passed. 

	*/


	/*
	SHPComputeExtents()
	void SHPComputeExtents( SHPObject * psObject );

	psObject:		An existing shape object to be updated in place.

	This function will recompute the extents of this shape, replacing the existing 
	values of the dfXMin, dfYMin, dfZMin, dfMMin, dfXMax, dfYMax, dfZMax, and dfMMax 
	values based on the current set of vertices for the shape. This function is 
	automatically called by SHPCreateObject() but if the vertices of an existing 
	object are altered it should be called again to fix up the extents.
	*/
};
