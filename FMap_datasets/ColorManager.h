#pragma once
#include <afx.h>
#include <string>
#include "StdAfx.h"

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
	void DrawPNGPointLabelShadow(CDC* pDC, double& dX, double& dY, CString& sLetter/*, CFont& font, int& nFontHeight*/);
	
	
	void DrawLineLabelShadow(CDC* pDC, double dX, double dY, CString sLetter);
	void DrawPNGLineLabelShadow(CDC* pDC, double& dX, double& dY, CString& sLetter, CFont& font, int& nFontHeight);

	void DrawAreaLabelShadow(CDC* pDC, double dX, double dY, CString sLetter);


	int RoundColor(int ncolor);
	COLORREF GetGifPaletteColor(COLORREF gifcolor);
	

};
