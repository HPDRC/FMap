#pragma once
#include "doublerect.h"

class CMapView
{
public:
	CMapView(void);
public:
	~CMapView(void);
public:
	// Extend of the View in original reference system.
	CDoubleRect rectExtend;
};
