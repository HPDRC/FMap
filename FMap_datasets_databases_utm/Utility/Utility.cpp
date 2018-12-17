// Utility.cpp: implementation of the Utility class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "Utility.h"

static int color[16] = {0,1,2,3,4,5,6,7,8,9,'A','B','C','D','E','F'};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int  printfNothing(const char *, ...)
{
	//do nothing.
	return 1;
}

int getRandomColor()
{
	return rand()%16;
}

int round(double a) 
{
	return int(a + 0.5);
}

//COLORREF str2colorref(CString str, int type)
//{
//	if(str.GetAt(0) == 'K' && type < 2)
//	{
//		if(type == 0)	// areacq field in dbf file
//		{
//			str.SetAt(0, '7');
//			str.SetAt(2, '7');
//			str.SetAt(4, '7');
//		}
//		else	// labelcq field in dbf file
//		{
//			str.SetAt(0, 'F');
//			str.SetAt(2, 'F');
//			str.SetAt(4, 'F');
//		}
//	}
//
//	if(str.GetAt(0) == 'H')
//	{
//		return RGB(0,0,255);
//	}
//
//	BYTE r = strtol((const char*)LPCTSTR(str.Left(2)), NULL, 16);
//	BYTE g = strtol((const char*)LPCTSTR(str.Mid(2, 2)), NULL, 16);
//	BYTE b = strtol((const char*)LPCTSTR(str.Right(2)), NULL, 16);
//
//	return RGB(r, g, b);
//}
//
//// change rgb color to bgr
//COLORREF bgr(COLORREF rgb)
//{
//	return RGB(GetBValue(rgb), GetGValue(rgb), GetRValue(rgb));
//}
