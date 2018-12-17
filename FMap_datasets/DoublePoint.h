#pragma once

class CDoublePoint
{
public:
	CDoublePoint(void);
	CDoublePoint(double dX1, double dY1);
public:
	~CDoublePoint(void);
public:

	double dX, dY;
	
	//for highlightCenter
	int iIndex;
	CDoublePoint(double dX1, double dY1, int iIndex1);

};
