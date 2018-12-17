
#include "StdAfx.h"
#include "DoublePoint.h"


CDoublePoint::CDoublePoint(void)
{
	
	this->dX = 0.0;
	this->dY = 0.0;
	this->iIndex = 0.0;
}

CDoublePoint::~CDoublePoint(void)
{
}

CDoublePoint::CDoublePoint(double dX, double dY)
{
	this->dX = dX;
	this->dY = dY;
}

CDoublePoint::CDoublePoint(double dX1, double dY1, int iIndex1)
{
	this->dX     = dX1;
	this->dY     = dY1;
	this->iIndex = iIndex1;
}
