#pragma once

#include "fmap.h"

class WKBManager
{
public:
	WKBManager(void);
	~WKBManager(void);

	private:
	WKBManager(const WKBManager&);
	WKBManager& operator=(const WKBManager&);


private:
	enum wkbByteOrder 
	{
		wkbXDR=0,
		wkbNDR=1
	};
	/*const char* wkb_;
	unsigned size_;
	unsigned pos_;
	wkbByteOrder byteOrder_;
	bool needSwap_;
	wkbFormat format_;*/

public:
	enum wkbGeometryType 
	{
		wkbPoint=1,
		wkbLineString=2,
		wkbPolygon=3,
		wkbMultiPoint=4,
		wkbMultiLineString=5,
		wkbMultiPolygon=6,
		wkbGeometryCollection=7
	};





};
