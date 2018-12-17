#pragma once

#include "stdafx.h"

class CDoubleRect
{

public:
	CDoubleRect(void);
	CDoubleRect(double dX1, double dY1, double dX2, double dY2 );
	CDoubleRect(std::string str_Long1, std::string str_Lat1, std::string str_Long2, std::string str_Lat2, bool isExtbyPixel );


public:
	~CDoubleRect(void);
public:
	// construction function
	
public:
	
	double dX1, dY1,  dX2, dY2;
	std::string str_ExtLong1, str_ExtLat1, str_ExtLong2, str_ExtLat2;
	std::string str_Long1, str_Lat1, str_Long2, str_Lat2;

};
