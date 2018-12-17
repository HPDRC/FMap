#include "StdAfx.h"
#include "doublerect.h"
//#include "ShapeObject.h"
#include "layerDataSource.h"
#include "DataBaseLayerDataSource.h"
#include "CoordinateTransformer.h"
#include "ShapeLayer.h"

CShapeLayer::CShapeLayer(void)
: m_sName(_T(""))
, m_sLabelField(_T(""))
{
	m_nIndexLabelField = 0;
}


CShapeLayer::~CShapeLayer(void)
{
	RemoveShapes();
}


// load data from data source , CdataSource is a data load interface
//all the previous data will be lost
bool CShapeLayer::LoadData(CLayerDataSource& dataSource)
{


	RemoveShapes();// free old data
	ASSERT( m_apShapes.GetSize() == 0 );// make sure all data deleted!!!

	if (GetType() != dataSource.getShapeType())
	{
		ASSERT(false);
		LOG("Warning: the type of the data from data source is \
			diffrent from the type of Current Layer, may cause problem when you process");
	};


	int nCount = dataSource.GetRecoderCount();
	//int nCount = (dataSource.getShapeRecords()).size(); wrong from mike

	ASSERT(nCount >= 0);
	
	m_apShapes.SetSize(nCount);
	CShape* pShape = NULL;

	for(int i = 0; i <nCount; i++)
	{
		pShape = dataSource.getRecord(i, m_asPropertiesField);
		
		if (pShape == NULL)
		{
			ASSERT("false");
			LOG("Error: Could not load data(index:[%d])from data source", i);
		}
				
		m_apShapes[i] = pShape;
	}

	return true;
}


//all the previous data will be lost
bool CShapeLayer::LoadDataFromMySQL(CDataBaseLayerDataSource& dataSource)
{
	RemoveShapes();// free old data

	//from MySQL
	dataSource.GetRecordFromMySQL(m_apShapes);


	return true;
}


bool CShapeLayer::LoadDataFromDatabase(CDataBaseLayerDataSource& dataSource)
{
	RemoveShapes();// free old data

	if(!dataSource.GetRecordFromDatabase(m_apShapes))
	{
		return false;
	}

	return true;
}


// free all shape objects 
void CShapeLayer::RemoveShapes(void)
{
	// free all the object 
	long nCount = getShapeObjectCount();

	for(int i = 0; i < nCount; i++)
	{
		delete m_apShapes[i] ;
	}

	m_apShapes.RemoveAll();
	m_apShapes.FreeExtra();
}




// Draw the Labels of shape layer 
bool CShapeLayer::DrawLabel(CMapCanvas* pCanvas)
{


	m_formatPaintTool.SetCanvas(pCanvas);

	/*CString color = m_asPropertiesField.GetAt(1);
	
	BYTE r = strtol(color.Left(2), NULL, 16);
	BYRE g = strtol(color.Mid(2, 2), NULL, 16);
	BYTE b = strtol(color.Right(2), NULL, 16);

	m_formatPaintTool.crLalelColor = RGB(r,g,b);*/
		
	m_formatPaintTool.BeginDrawLabel();
	DrawTypeLabel(); // down call to concreate subclass
	m_formatPaintTool.EndDrawLabel();


	return false;
}

	// Drew shape (point, line, polygon) on canvas
bool CShapeLayer::DrawShape(CMapCanvas* pCanvas)
{
	m_formatPaintTool.SetCanvas(pCanvas);

	m_formatPaintTool.BeginDrawShape();
	DrawTypeShape();// down call to concreate subclass
	m_formatPaintTool.EndDrawShape();

	return false;
}


// Transfer Coordinate System To
/*
bool CShapeLayer::TransferCoordinateSysTo(CCoordinateTransformer::CoordinateSystem destSystem)
{
	return false;
}
*/

// set the index of field for label
bool CShapeLayer::SetLabelFieldIndex(int iField)
{
	if ( iField > 0 && iField < GetPropertiesFieldCount() )
	{
		m_nIndexLabelField = iField;
		return true;
	}
	return false;
}

// get the count of properties fields
int CShapeLayer::GetPropertiesFieldCount(void)
{
	return (int)m_asPropertiesField.GetCount();
}

// get label field index
int CShapeLayer::GetLabelFieldIndex(void)
{
	return m_nIndexLabelField;
}


bool CShapeLayer::SetLayerName(CString layername)
{
	if( "" != layername )
	{
		m_sName = layername;
		return true;
	}
	return false;
}