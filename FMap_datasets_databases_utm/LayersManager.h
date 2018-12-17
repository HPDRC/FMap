#pragma once
#include "shapelayer.h"
class CFormatPaintTool;
class CShapeLayer;

typedef CList<CShapeLayer* > CLayerList;

class CLayersManager
{
public:
	CLayersManager(void);
public:
	~CLayersManager(void);
public:
	// Create a new layers
	static CShapeLayer* CreateLayer(CString& sLayerName, CString& sShapeDataFileName, CStringArray* pPropertiesField, CDoubleRect& rectExt, CFormatPaintTool* pFormatTool);
	bool AddLayer(CString& sLayerName, CString& sShapeDataFileName, CStringArray* pPropertiesField, CDoubleRect& rectExt, CFormatPaintTool* pFormatTool);
	bool AddLayer(CShapeLayer* pNewLayer);

public:

	// the array of the layers
	CLayerList m_layerList;
private:
	
	bool RemoveAllDate(void);
};
