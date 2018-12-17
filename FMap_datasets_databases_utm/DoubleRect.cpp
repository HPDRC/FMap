
#include "StdAfx.h"
#include "DoubleRect.h"


CDoubleRect::CDoubleRect(void)
{
	this->dX1 = 0.0;
	this->dX2 = 0.0;
	this->dY1 = 0.0;
	this->dY2 = 0.0;

	this->str_ExtLong1 = "";
	this->str_ExtLat1  = "";
	this->str_ExtLong2 = "";
	this->str_ExtLat2  = "";

	this->str_Long1 = "";
	this->str_Lat1  = "";
	this->str_Long2 = "";
	this->str_Lat2  = "";

}

CDoubleRect::~CDoubleRect(void)
{

}

// construction function
CDoubleRect::CDoubleRect(double dX1, double dY1, double dX2, double dY2)
{
	this->dX1 = dX1;
	this->dX2 = dX2;
	this->dY1 = dY1;
	this->dY2 = dY2;
}


CDoubleRect::CDoubleRect(std::string str_Long1, std::string str_Lat1, 
						 std::string str_Long2, std::string str_Lat2, 
						 bool isExtbyPixel)
{

	if(isExtbyPixel)
	{
		this->str_ExtLong1 = str_Long1;
		this->str_ExtLat1  = str_Lat1;
		this->str_ExtLong2 = str_Long2;
		this->str_ExtLat2  = str_Lat2;
	}
	else
	{
		this->str_Long1 = str_Long1;
		this->str_Lat1  = str_Lat1;
		this->str_Long2 = str_Long2;
		this->str_Lat2  = str_Lat2;
	}

}