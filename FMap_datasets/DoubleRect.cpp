
#include "StdAfx.h"
#include "DoubleRect.h"


CDoubleRect::CDoubleRect(void)
{
	this->dX1 = 0.0;
	this->dX2 = 0.0;
	this->dY1 = 0.0;
	this->dY2 = 0.0;

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
