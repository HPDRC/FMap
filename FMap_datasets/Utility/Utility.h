// Utility.h: interface for the Utility class.
//
//////////////////////////////////////////////////////////////////////
#include "stdio.h"
#include <afx.h>

#if !defined(AFX_UTILITY_H__FC2C380B_7A48_4B59_9F25_8D278ECADA99__INCLUDED_)
#define AFX_UTILITY_H__FC2C380B_7A48_4B59_9F25_8D278ECADA99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PI 3.1415926

#define  LOG printf

int  printfNothing(const char *, ...);
int round(double a);

// TODO: We should log Macro THIS_FILE, __LINE__ !!!!!!!!!!!

// int to indicate it is labelcq or areacq
//COLORREF str2colorref(CString, int);
//
//COLORREF bgr(COLORREF rgb);

#endif // !defined(AFX_UTILITY_H__FC2C380B_7A48_4B59_9F25_8D278ECADA99__INCLUDED_)
