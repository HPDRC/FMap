#pragma once

//#include <map>
//#include <afx.h>
//#include <string>
#include "StdAfx.h"
#include "FMapRequest.h"
#include <list>
#include "FMapRequestManager.h"
#include "fmap.h"


typedef CMap<CString, LPCTSTR, int, int> LayerPriorityTable;
typedef std::pair<CString, int> PrioLayer;
typedef std::list<PrioLayer> LayerList;

class CLayerPriority
{
public:

	//CMapStringToOb layerTable;
	LayerPriorityTable layerTable;

	LayerList list;

	std::list<PrioLayer>::iterator cur;
	std::list<PrioLayer>::iterator next;
	std::list<PrioLayer>::iterator tail;

	CFMapRequestManager req;

	bool ReorderLayer(Overlays& layers, CString& newLayers);

	bool InsertToPriorityList(CString& str, int priority);

	CLayerPriority(void);
	~CLayerPriority(void);
};

