#pragma once
#include "doublePoint.h"
#include <afxtempl.h>
#include <string>

/*
#define SHPT_NULL	0
#define SHPT_POINT	1
#define SHPT_ARC	3
#define SHPT_POLYGON	5
*/

typedef CArray<CDoublePoint> CPointArray;
typedef CArray<int> CIntegerArray;



namespace ShapeUtility{

		enum ShapeType {
		unknown = 0,
		point = 1,
		arc = 3,
		polygon = 5,
	};


}


class CMapUtility
{
	
public:
	CMapUtility(void);
public:
	~CMapUtility(void);

	std::string DoubleToString(double d)
	{
		/*std::ostringstream ostr;
		if (!(ostr << d)) 
		{
		return "ERROR";
		}
		return ostr.str();*/

		char temp[30] = {0};
		sprintf_s(temp, "%f", d);

		return temp;

	}


};
