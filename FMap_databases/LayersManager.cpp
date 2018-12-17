#include "StdAfx.h"
#include "FormatPaintTool.h"
#include "shapeLayer.h"
#include "LayersManager.h"

CLayersManager::CLayersManager(void)
{
}

CLayersManager::~CLayersManager(void)
{
	RemoveAllDate();
}

bool CLayersManager::RemoveAllDate(void)
{

	POSITION pos = m_layerList.GetHeadPosition();
	int nCount = (int) m_layerList.GetCount();
	CShapeLayer* pLayer;
	for (int i=0;i < nCount; i++)
	{
		 pLayer = m_layerList.GetNext(pos);
		if ( pLayer != NULL )
		{
			delete pLayer;
			pLayer = NULL;
		}
		
	}
	m_layerList.RemoveAll();
	return true;
}





// Create a new layers
CShapeLayer* CLayersManager::CreateLayer(CString& sLayerName, CString& sShapeDataFileName, CStringArray* pPropertiesField, CDoubleRect& rectExt, CFormatPaintTool* pFormatTool)
{
	//TODO:  use datasource load data to a new layer object

	return NULL;
}

bool CLayersManager::AddLayer(CShapeLayer* pNewLayer)
{
	m_layerList.AddTail( pNewLayer );
	
	return false;
}

bool CLayersManager::AddLayer(CString& sLayerName, CString& sShapeDataFileName, CStringArray* pPropertiesField, CDoubleRect& rectExt, CFormatPaintTool* pFormatTool)
{
	CShapeLayer* pNewLayer = CreateLayer(sLayerName, sShapeDataFileName, pPropertiesField, rectExt, pFormatTool);
	if ( pNewLayer == NULL )
	{
		LOG( "ERROR: could not load the shape layer: %s from Shape File: %s", sLayerName, sShapeDataFileName );
		ASSERT( false );
		return false;
	}
	
	return  AddLayer( pNewLayer );
}

