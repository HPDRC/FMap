#pragma once
#include "doublePoint.h"
#include <afxtempl.h>
#include <string>
//#include <stack>

/*
#define SHPT_NULL	0
#define SHPT_POINT	1
#define SHPT_ARC	3
#define SHPT_POLYGON	5
*/

typedef CArray<CDoublePoint> CPointArray;
typedef CArray<int> CIntegerArray;
//typedef CList<CString> CStringList;//MFC default type



namespace ShapeUtility{

		enum ShapeType {
		unknown = 0,
		point = 1,
		arc = 3,
		polygon = 5,
	};


}

//std::string DoubleToString(double d)
//{
//	/*std::ostringstream ostr;
//	if (!(ostr << d)) 
//	{
//		return "ERROR";
//	}
//	return ostr.str();*/
//
//	char temp[30] = {0};
//	sprintf_s(temp, "%f", d);
//
//	return temp;
//
//}


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

//std::string IntDec2StrHex(int dec)
//{
//	int i = 0;
//	std::stack<int> remainder;
//	std::string hex, temp;
//	char hexDigits[] = { "0123456789ABCDEF" };
//
//	while (dec != 0)
//	{
//		remainder.push(dec % 16);
//		dec /= 16;
//		++i;
//	}
//
//	while (i != 0)
//	{
//		if (remainder.top() > 15)
//		{
//			temp = IntDec2StrHex(remainder.top());
//			hex += temp;
//		}
//		hex.push_back(hexDigits[remainder.top()]);
//		remainder.pop();
//		--i;
//	}
//	return hex;
//}

//CString ColorToString(Gdiplus::Color pixel)
//{
//     /*CString s = c.ToString();
//     s = s.Split(new char[]{'[',']'})[1];
//     string[] strings = s.Split(new char[]{'=',','});
//     if(strings.GetLength(0) > 7)
//     {
//          s = strings[1] + "," + strings[3] + "," + strings[5] + "," + strings[7];
//     }*/
//
//	std::string str;
//	int a = pixel.GetAlpha();
//	str = dec2hex(a);
//	
//
//    return s;
//} 
