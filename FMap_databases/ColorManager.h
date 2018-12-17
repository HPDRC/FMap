#pragma once
#include <afx.h>
#include <string>
#include "StdAfx.h"

//typedef CMap<CString, LPCTSTR, Gdiplus::Color, Gdiplus::Color> CMapColorTable; 
typedef CMap<long, long, Gdiplus::Color, Gdiplus::Color> CMapColorTable; 

COLORREF str2colorref(CString, int);

COLORREF bgr(COLORREF rgb);

struct OverridedColor
{
	std::string line;
	std::string label;
	std::string area;

	OverridedColor() 
	{
	}
};




class CColorManager
{
private:

	//Gdiplus::Color m_arccolor;
	COLORREF m_arccolor;
	COLORREF m_areacolor;
	COLORREF m_areabordercolor;
	COLORREF m_pointlabelcolor;
	COLORREF m_arclabelcolor;
	COLORREF m_arealabelcolor;

public:
	COLORREF M_BLACK;
	COLORREF M_WHITE;

public:
	CColorManager(void);
public:
	~CColorManager(void);

public:

	COLORREF GetDefaultArcColor();
	void SetArcColor(OverridedColor colorInfo);
	COLORREF GetArcColor();

	void SetAreaColor(CString colorStr);
	COLORREF GetAreaColor();
	void SetAreaBorderColor(CString colorStr);
	COLORREF GetAreBorderColor();

	void SetPointLabelColor(OverridedColor colorInfo, CString strLabelColor, 
		int nVolumnSize);
	COLORREF GetPointLabelColor();

	COLORREF AddShadow( COLORREF color );

	void SetArcLabelColor(OverridedColor colorInfo, bool hasStreets, bool hasSechwys, 
		bool hasMajHwys, bool hasDetailroads, bool hasMainroads);
	COLORREF GetArcLabelColor();

	void SetAreaLabelColor(OverridedColor colorInfo, CString strAreaColor, int nColumnSize);
	COLORREF GetAreaLabelColor();

	void DrawPointLabelShadow(CDC* pDC, double dX, double dY, CString sLetter);
	void DrawLineLabelShadow(CDC* pDC, double dX, double dY, CString sLetter);
	void DrawAreaLabelShadow(CDC* pDC, double dX, double dY, CString sLetter);


	int RoundColor(int ncolor);
	COLORREF GetGifPaletteColor(COLORREF gifcolor);

	long GetPNGColorTableIndex( Gdiplus::Color pixel )
	{
		long index = pixel.GetRed()+pixel.GetGreen()*1000+pixel.GetBlue()*1000000;
		return index;
	}
	

};
