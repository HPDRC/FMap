#pragma once

#include "shapeObject.h"
#include "FormatPaintTool.h"
#include "CoordinateTransformer.h"
#include "MapCanvas.h"

//typedef CArray<CDoublePoint,CDoublePoint> CPointArray;
class CLayerDataSource;
class CShape;
//typedef CArray<CShape, CShape> CShapeArray;

typedef CArray<CShape*> CShapePtrArray;

class CShapeLayer
{

public:
	CShapeLayer(void);
public:
	virtual ~CShapeLayer(void);

public:
	// extend of the shape date  
	//CDoubleRect rectExtend;
	// set the extend 
	//virtual bool SetExtend(CDoubleRect rectExtend);


	int m_nIndexLabelField;
	CStringArray m_asPropertiesField;// be carfull with the max_size 

//protected:
	CShapePtrArray m_apShapes;//// be carfull with the max_size and Free the Object anytime, keep in mind!!!


	// the name of the layer
	CString m_sName;

	// the label that will be displayed on the map
	CString m_sLabelField;

	/*
	May supports Classified display of shape, like map-serve does!!  
	// the display format of the layer	*/
	CFormatPaintTool m_formatPaintTool;
public:
	// Draw the Labels of shape layer 
	 bool DrawLabel(CMapCanvas* pCanvas);

	// Drew shape (point, line, polygon) on canvas
	 bool DrawShape(CMapCanvas* pCanvas);

protected:
	virtual bool DrawTypeShape(void) = 0;
	virtual bool DrawTypeLabel(void) = 0;

	virtual ShapeUtility::ShapeType GetType() = 0;
	
	virtual bool MergePartsTo(CShapeLayer* pShapeLayer) 
	{
		ASSERT(false); 
		return false;// not implemented
	}


// Transfer Coordinate System To
//	bool TransferCoordinateSysTo(CCoordinateTransformer::CoordinateSystem destSystem);
public:
	// set the index of field for label
	bool SetLabelFieldIndex(int iField);
	// set the layer name
	bool SetLayerName(CString layername);
public:
	// get the count of properties fields
	int GetPropertiesFieldCount(void);
public:
	// get label field index
	int GetLabelFieldIndex(void);

	
public:

	virtual long getShapeObjectCount(void)
	{
		return (long)m_apShapes.GetCount();
	}
public:
	CShape* getShapeObject(long lIndexShape)
	{

		if (lIndexShape > getShapeObjectCount() &&lIndexShape < 0)
		{
			ASSERT(false);
			return NULL;
		}
		else
		{
			ASSERT( m_apShapes[lIndexShape] != NULL );
			return m_apShapes[lIndexShape];
		}
	}
public:
	//TODO: if lIndexShape == -1 should add object!!!
	bool SetShapeObject( CShape* pShape, long lIndexShape )
	{
		// if not whithin the range of array
		if (lIndexShape > getShapeObjectCount() &&lIndexShape < 0)
		{
			ASSERT(false);
			return false;
		}
		
		ASSERT( pShape != NULL );
		// if the pointer is NULL
		if ( pShape == NULL )
		{
			return false;
		}
		else
		{
			// Free the Object first, if the object is not NULL .
			if ( m_apShapes[lIndexShape] != NULL )
			{
				delete m_apShapes[lIndexShape];
			}
			m_apShapes[lIndexShape] = pShape;
			return true;
		}
			
	}
	bool AddShapeObject( CShape* pShape )
	{
		// if not whithin the range of array
		ASSERT( pShape != NULL );
		// if the pointer is NULL
		if ( pShape == NULL )
		{
			return false;
		}
		else
		{
			// Free the Object first, if the object is not NULL .
			
			m_apShapes.Add( pShape );
			return true;
		}
			
	}
public:
	// load data from data source , CdataSource is a data load interface
	bool LoadData(CLayerDataSource& dataSource);

	// free all shape objects 
	void RemoveShapes(void);

};
