#include "ColorManager.h"
//#include "afx.h"
//#include "StdAfx.h"


COLORREF str2colorref(CString str, int type)
{
	if(str.GetAt(0) == 'K' && type < 2)
	{
		if(type == 0)	// areacq field in dbf file
		{
			str.SetAt(0, '7');
			str.SetAt(2, '7');
			str.SetAt(4, '7');
		}
		else	// labelcq field in dbf file
		{
			str.SetAt(0, 'F');
			str.SetAt(2, 'F');
			str.SetAt(4, 'F');
		}
	}

	/*if(str.GetAt(0) == 'H')
	{
	return RGB(0,0,255);
	}*/

	if(str.GetAt(0) == 'H')
	{
		return RGB(0,255,0);
	}

	BYTE r = (BYTE) strtol((const char*)LPCTSTR(str.Left(2)), NULL, 16);
	BYTE g = (BYTE) strtol((const char*)LPCTSTR(str.Mid(2, 2)), NULL, 16);
	BYTE b = (BYTE) strtol((const char*)LPCTSTR(str.Right(2)), NULL, 16);

	return RGB(r, g, b);
}

// change rgb color to bgr
COLORREF bgr(COLORREF rgb)
{
	return RGB(GetBValue(rgb), GetGValue(rgb), GetRValue(rgb));
	//return rgb;
}




CColorManager::CColorManager(void)
{
	m_arccolor = bgr( RGB(0, 255, 255) );
	M_BLACK    = 0x00000000;
	M_WHITE    = RGB(255,255,255);
}

CColorManager::~CColorManager(void)
{
}

COLORREF CColorManager::GetDefaultArcColor()
{
	return bgr( RGB(0, 255, 255) );
}

void CColorManager::SetArcColor(OverridedColor colorInfo)
{
	//ASSERT(colorInfo != NULL);
	BYTE r = (BYTE) strtol(colorInfo.line.substr(0,2).c_str(), NULL, 16);
	BYTE g = (BYTE) strtol(colorInfo.line.substr(2,2).c_str(), NULL, 16);
	BYTE b = (BYTE) strtol(colorInfo.line.substr(4).c_str(), NULL, 16);

	m_arccolor = bgr( RGB(r, g, b) );
}

COLORREF CColorManager::GetArcColor()
{
	ASSERT(m_arccolor != NULL);
	return this->m_arccolor;
}



void CColorManager::SetAreaColor(CString colorStr)
{
	//ASSERT(colorStr != NULL);
	m_areacolor = bgr( str2colorref(colorStr, 0) );// this is areacq
}

COLORREF CColorManager::GetAreaColor()
{
	//ASSERT(m_areacolor != NULL);
	return m_areacolor;
}



void CColorManager::SetAreaBorderColor(CString colorStr)
{
	//ASSERT(colorStr != "");
	m_areabordercolor = bgr(str2colorref(colorStr, 3));
}
COLORREF CColorManager::GetAreBorderColor()
{
	//ASSERT(m_areabordercolor != NULL);
	return m_areabordercolor;
}


void CColorManager::SetPointLabelColor(OverridedColor colorInfo, CString strLabelColor, int nVolumnSize)
{
	ASSERT(m_pointlabelcolor != NULL);

	if( colorInfo.label != "" )
	{
		BYTE r = (BYTE) strtol(colorInfo.label.substr(0,2).c_str(), NULL, 16);
		BYTE g = (BYTE) strtol(colorInfo.label.substr(2,2).c_str(), NULL, 16);
		BYTE b = (BYTE) strtol(colorInfo.label.substr(4).c_str(), NULL, 16);

		m_pointlabelcolor = RGB(r,g,b);
		if( m_pointlabelcolor < 20 )
		{
			m_pointlabelcolor = RGB(r,g,b);
		}

	}
	//color comes from the DBF
	else if( nVolumnSize >= 2 )
	{															
		m_pointlabelcolor = bgr( str2colorref(strLabelColor, 3) );	// color from dbf

		if(m_pointlabelcolor < 20)
		{
			m_pointlabelcolor = RGB(255,255,255);
		}
	}
	//set default color
	else
	{
		m_pointlabelcolor = RGB(255,255,255);
	}

}

COLORREF CColorManager::GetPointLabelColor()
{
	ASSERT( m_pointlabelcolor );
	return m_pointlabelcolor;
}


COLORREF CColorManager::AddShadow( COLORREF color )
{
	ASSERT( color != NULL );

	BYTE r = (GetRValue(color) & ~0x03) | 0x01;
	BYTE g = (GetGValue(color) & ~0x03) | 0x03;
	BYTE b = (GetBValue(color) & ~0x03) | 0x03;

	return RGB(r, g, b);
}

void CColorManager::SetArcLabelColor(OverridedColor colorInfo, bool hasStreets, 
									 bool hasSechwys, bool hasMajHwys, 
									 bool hasDetailroads, bool hasMainroads)
{
	if(colorInfo.label != "")
	{
		BYTE r = (BYTE) strtol(colorInfo.label.substr(0,2).c_str(), NULL, 16);
		BYTE g = (BYTE) strtol(colorInfo.label.substr(2,2).c_str(), NULL, 16);
		BYTE b = (BYTE) strtol(colorInfo.label.substr(4).c_str(), NULL, 16);

		m_arclabelcolor = RGB(r,g,b);
		if( m_arclabelcolor < 20 )
		{
			m_arclabelcolor = RGB(r,g,b);
		}

	}
	//color comes from the DBF
	//else if("" != pShape->m_aProperties[2])
	//{															// dummy
	//	color = bgr(str2colorref(pShape->m_aProperties[2], 3));	// color from dbf

	//	if(color < 20)
	//		color = RGB(100,255,100);
	//}
	else if( hasStreets )
	{
		//color = RGB(100,255,100);//green
		m_arclabelcolor = RGB(0,255,255);//yellow
	}
	else if( hasSechwys )
	{
		//color = RGB(255,255,0);//blue
		m_arclabelcolor = RGB(100,255,100);//green
	}
	else if( hasMajHwys )
	{
		//color = RGB(0,255,255);//yellow
		m_arclabelcolor = RGB(100,255,100);//green
	}
	else if( hasDetailroads )
	{
		//color = RGB(255,255,255);//white
		m_arclabelcolor = RGB(0,255,255);//yellow
	}
	else if( hasMainroads )
	{
		//color = RGB(0,255,255);//yellow
		m_arclabelcolor = RGB(100,255,100);//green
	}
	//set the default color
	else
	{
		m_arclabelcolor = RGB(100,255,100);//green
	}

}

COLORREF CColorManager::GetArcLabelColor()
{
	ASSERT( m_arclabelcolor != NULL );
	return m_arclabelcolor;
}

void CColorManager::SetAreaLabelColor(OverridedColor colorInfo, CString strAreaColor, int nColumnSize)
{
	//color comes from request
	if(colorInfo.label != "")
	{
		BYTE r = (BYTE) strtol(colorInfo.label.substr(0,2).c_str(), NULL, 16);
		BYTE g = (BYTE) strtol(colorInfo.label.substr(2,2).c_str(), NULL, 16);
		BYTE b = (BYTE) strtol(colorInfo.label.substr(4).c_str(), NULL, 16);

		m_arealabelcolor = RGB(r,g,b);
		if( m_arealabelcolor < 20 )
		{
			m_arealabelcolor = RGB(r,g,b);
		}

	}
	//color comes from the DBF
	else if(nColumnSize >= 2)
	{															// this is labelcq
		m_arealabelcolor = bgr(str2colorref(strAreaColor, 1));	// color from dbf

		if(m_arealabelcolor < 20)
			m_arealabelcolor = bgr(RGB(255,255,255));
	}
	else
	{
		m_arealabelcolor = bgr(RGB(255,255,255));
	}
}

COLORREF CColorManager::GetAreaLabelColor()
{
	ASSERT(m_arealabelcolor != NULL);
	return m_arealabelcolor;
}



void CColorManager::DrawPNGPointLabelShadow(CDC* pDC, double& dX, double& dY, CString& sLetter/*, CFont& font, int& nFontHeight*/)
{
	pDC->SetTextColor( RGB(55,55,3) );

	int npixel = 1;

	pDC->TextOut( round (dX - npixel),
		round (dY - npixel),
		sLetter );
	pDC->TextOut( round (dX + npixel),
		round (dY + npixel),
		sLetter );					
	pDC->TextOut( round (dX - npixel),
		round (dY + npixel),
		sLetter );					
	pDC->TextOut( round (dX + npixel),
		round (dY - npixel),
		sLetter );
	pDC->TextOut( round (dX - npixel),
		round (dY),
		sLetter );
	pDC->TextOut( round (dX + npixel),
		round (dY),
		sLetter );
	pDC->TextOut( round (dX ),
		round (dY + npixel),
		sLetter );
	pDC->TextOut( round (dX ),
		round (dY - npixel),
		sLetter );


	/*double dTRX = (round (dX + npixel - 1) + round (dX + npixel))/2;
	double dTRY = (round (dY + npixel - 1) + round (dY + npixel))/2;
	double dBLX = (round (dX - npixel - 1) + round (dX - npixel))/2;
	double dBLY = (round (dY + npixel - 1) + round (dY + npixel))/2;







	pDC->TextOut( dBLX, dBLY, sLetter );
	pDC->TextOut( dBLX, dBLY - npixel, sLetter );
	pDC->TextOut( dTRX + npixel - 1, dTRY, sLetter );
	pDC->TextOut( dTRX + npixel - 1, dTRY - npixel, sLetter );*/

}


void CColorManager::DrawPNGLineLabelShadow(CDC* pDC, double& dX, double& dY, CString& sLetter, CFont& font, int& nFontHeight)
{


	pDC->SetTextColor( RGB(55,55,3) );

	int npixel = 1;

	pDC->TextOut( round (dX - npixel),
		round (dY - npixel),
		sLetter );
	pDC->TextOut( round (dX + npixel),
		round (dY + npixel),
		sLetter );					
	pDC->TextOut( round (dX - npixel),
		round (dY + npixel),
		sLetter );					
	pDC->TextOut( round (dX + npixel),
		round (dY - npixel),
		sLetter );
	pDC->TextOut( round (dX - npixel),
		round (dY),
		sLetter );
	pDC->TextOut( round (dX + npixel),
		round (dY),
		sLetter );
	pDC->TextOut( round (dX ),
		round (dY + npixel),
		sLetter );
	pDC->TextOut( round (dX ),
		round (dY - npixel),
		sLetter );

	pDC->TextOut( round (dX ),
		round (dY - npixel - npixel),
		sLetter );
	pDC->TextOut( round (dX - npixel),
		round (dY -npixel - npixel),
		sLetter );


	double dTRX = (round (dX + npixel - 1) + round (dX + npixel))/2;
	double dTRY = (round (dY + npixel - 1) + round (dY + npixel))/2;
	double dBLX = (round (dX - npixel - 1) + round (dX - npixel))/2;
	double dBLY = (round (dY + npixel - 1) + round (dY + npixel))/2;







	pDC->TextOut( dBLX, dBLY, sLetter );
	pDC->TextOut( dBLX, dBLY - npixel, sLetter );
	pDC->TextOut( dTRX + npixel - 1, dTRY, sLetter );
	pDC->TextOut( dTRX + npixel - 1, dTRY - npixel, sLetter );


	//pDC->TextOut( dBLX, dBLY-1, sLetter );

	//	VERIFY(font.CreateFont(
	//				nFontHeight,               // nHeight
	//				0,                         // nWidth
	//				0,                         // nEscapement unit in 0.1
	//				0,                         // nOrientation
	//				600,//                  // nWeight
	//				FALSE,                     // bItalic
	//				FALSE,                     // bUnderline
	//				0,                         // cStrikeOut
	//				ANSI_CHARSET,              // nCharSet
	//				OUT_DEFAULT_PRECIS,        // nOutPrecision
	//				CLIP_DEFAULT_PRECIS,       // nClipPrecision
	//				ANTIALIASED_QUALITY,       //DEFAULT_QUALITY,           // nQuality
	//				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	//				"Lucida Console"));        //verdana

	//pDC->SetTextColor( COLORREF("000000") );
	//pDC->TextOut( round( dX ), round( dY ), sLetter );

	//VERIFY(font.CreateFont(
	//				nFontHeight,               // nHeight
	//				0,                         // nWidth
	//				0,                         // nEscapement unit in 0.1
	//				0,                         // nOrientation
	//				FW_BOLD,//                  // nWeight
	//				FALSE,                     // bItalic
	//				FALSE,                     // bUnderline
	//				0,                         // cStrikeOut
	//				ANSI_CHARSET,              // nCharSet
	//				OUT_DEFAULT_PRECIS,        // nOutPrecision
	//				CLIP_DEFAULT_PRECIS,       // nClipPrecision
	//				ANTIALIASED_QUALITY,       //DEFAULT_QUALITY,           // nQuality
	//				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	//				"Lucida Console")); 


	/*pDC->SetTextColor( RGB(55,55,3) );

	int npixel = 1;

	pDC->TextOut( round (dX - npixel),
		round (dY - npixel),
		sLetter );
	pDC->TextOut( round (dX + npixel),
		round (dY + npixel),
		sLetter );					
	pDC->TextOut( round (dX - npixel),
		round (dY + npixel),
		sLetter );					
	pDC->TextOut( round (dX + npixel),
		round (dY - npixel),
		sLetter );
	pDC->TextOut( round (dX - npixel),
		round (dY),
		sLetter );
	pDC->TextOut( round (dX + npixel),
		round (dY),
		sLetter );
	pDC->TextOut( round (dX ),
		round (dY + npixel),
		sLetter );
	pDC->TextOut( round (dX ),
		round (dY - npixel),
		sLetter );


	double dTRX = (round (dX + npixel - 1) + round (dX + npixel))/2;
	double dTRY = (round (dY + npixel - 1) + round (dY + npixel))/2;
	double dBLX = (round (dX - npixel - 1) + round (dX - npixel))/2;
	double dBLY = (round (dY + npixel - 1) + round (dY + npixel))/2;







	pDC->TextOut( dBLX, dBLY, sLetter );
	pDC->TextOut( dBLX, dBLY - npixel, sLetter );
	pDC->TextOut( dTRX + npixel - 1, dTRY, sLetter );
	pDC->TextOut( dTRX + npixel - 1, dTRY - npixel, sLetter );*/

}



void CColorManager::DrawPointLabelShadow(CDC* pDC, double dX, double dY, CString sLetter)
{
	pDC->SetTextColor( RGB(55,55,3) );
	/*pDC->TextOut( (int) (posLetterBottom.dX - 1),
	(int) (posLetterBottom.dY - 1),
	sLetter );
	pDC->TextOut( (int) (posLetterBottom.dX + 1),
	(int) (posLetterBottom.dY + 1),
	sLetter );					
	pDC->TextOut( (int) (posLetterBottom.dX - 1),
	(int) (posLetterBottom.dY + 1),
	sLetter );					
	pDC->TextOut( (int) (posLetterBottom.dX + 1),
	(int) (posLetterBottom.dY - 1),
	sLetter );
	pDC->TextOut( (int) (posLetterBottom.dX - 1),
	(int) (posLetterBottom.dY),
	sLetter );
	pDC->TextOut( (int) (posLetterBottom.dX + 1),
	(int) (posLetterBottom.dY),
	sLetter );*/

	/*pDC->TextOut( (int) (dX - 2),
		(int) (dY - 2),
		sLetter );
	pDC->TextOut( (int) (dX + 2),
		(int) (dY + 2),
		sLetter );					
	pDC->TextOut( (int) (dX - 2),
		(int) (dY + 2),
		sLetter );					
	pDC->TextOut( (int) (dX + 2),
		(int) (dY - 2),
		sLetter );
	pDC->TextOut( (int) (dX - 2),
		(int) (dY),
		sLetter );
	pDC->TextOut( (int) (dX + 2),
		(int) (dY),
		sLetter );
	pDC->TextOut( (int) (dX ),
		(int) (dY + 2),
		sLetter );
	pDC->TextOut( (int) (dX ),
		(int) (dY - 2),
		sLetter );

	double dTRX = ((int) (dX + 1) + (int) (dX + 2))/2;
	double dTRY = ((int) (dY + 1) + (int) (dY + 2))/2;
	double dBLX = ((int) (dX - 1) + (int) (dX - 2))/2;
	double dBLY = ((int) (dY + 1) + (int) (dY + 2))/2;*/


	int npixel = 2;

	pDC->TextOut( round (dX - npixel),
		round (dY - npixel),
		sLetter );
	pDC->TextOut( round (dX + npixel),
		round (dY + npixel),
		sLetter );					
	pDC->TextOut( round (dX - npixel),
		round (dY + npixel),
		sLetter );					
	pDC->TextOut( round (dX + npixel),
		round (dY - npixel),
		sLetter );
	pDC->TextOut( round (dX - npixel),
		round (dY),
		sLetter );
	pDC->TextOut( round (dX + npixel),
		round (dY),
		sLetter );
	pDC->TextOut( round (dX ),
		round (dY + npixel),
		sLetter );
	pDC->TextOut( round (dX ),
		round (dY - npixel),
		sLetter );

	double dTRX = (round (dX + npixel - 1) + round (dX + npixel))/2;
	double dTRY = (round (dY + npixel - 1) + round (dY + npixel))/2;
	double dBLX = (round (dX - npixel - 1) + round (dX - npixel))/2;
	double dBLY = (round (dY + npixel - 1) + round (dY + npixel))/2;







	pDC->TextOut( dBLX, dBLY, sLetter );
	pDC->TextOut( dBLX, dBLY - npixel, sLetter );
	pDC->TextOut( dTRX + npixel - 1, dTRY, sLetter );
	pDC->TextOut( dTRX + npixel - 1, dTRY - npixel, sLetter );
}

void CColorManager::DrawLineLabelShadow(CDC* pDC, double dX, double dY, CString sLetter)
{
	pDC->SetTextColor( RGB(55,55,3) );
	//pDC->TextOut( (int) (posLetter.dX - 1),
	//	(int) (posLetter.dY - 1),
	//	sLetter );
	//pDC->TextOut( (int) (posLetter.dX + 1),
	//	(int) (posLetter.dY + 1),
	//	sLetter );					
	//pDC->TextOut( (int) (posLetter.dX - 1),
	//	(int) (posLetter.dY + 1),
	//	sLetter );					
	//pDC->TextOut( (int) (posLetter.dX + 1),
	//	(int) (posLetter.dY - 1),
	//	sLetter );
	//pDC->TextOut( (int) (posLetter.dX - 1),
	//	(int) (posLetter.dY),
	//	sLetter );
	//pDC->TextOut( (int) (posLetter.dX + 1),
	//	(int) (posLetter.dY),
	//	sLetter );
	//pDC->TextOut( (int) (posLetter.dX),
	//	(int) (posLetter.dY + 1 ),
	//	sLetter );
	//pDC->TextOut( (int) (posLetter.dX),
	//	(int) (posLetter.dY - 1 ),
	//	sLetter );

	/*pDC->TextOut( (int) (dX - 2),
		(int) (dY - 2),
		sLetter );
	pDC->TextOut( (int) (dX + 2),
		(int) (dY + 2),
		sLetter );					
	pDC->TextOut( (int) (dX - 2),
		(int) (dY + 2),
		sLetter );					
	pDC->TextOut( (int) (dX + 2),
		(int) (dY - 2),
		sLetter );
	pDC->TextOut( (int) (dX - 2),
		(int) (dY),
		sLetter );
	pDC->TextOut( (int) (dX + 2),
		(int) (dY),
		sLetter );
	pDC->TextOut( (int) (dX ),
		(int) (dY + 2),
		sLetter );
	pDC->TextOut( (int) (dX ),
		(int) (dY - 2),
		sLetter );

	double dTRX = ((int) (dX + 1) + (int) (dX + 2))/2;
	double dTRY = ((int) (dY + 1) + (int) (dY + 2))/2;
	double dBLX = ((int) (dX - 1) + (int) (dX - 2))/2;
	double dBLY = ((int) (dY + 1) + (int) (dY + 2))/2;

	pDC->TextOut( dBLX, dBLY, sLetter );
	pDC->TextOut( dBLX, dBLY - 2, sLetter );
	pDC->TextOut( dTRX + 1, dTRY, sLetter );
	pDC->TextOut( dTRX + 1, dTRY - 2, sLetter );

	pDC->TextOut( dTRX, (int) (dY + 2), sLetter );
	pDC->TextOut( dTRX, (int) (dY - 2), sLetter );
	pDC->TextOut( dTRX - 2, (int) (dY + 2), sLetter );
	pDC->TextOut( dTRX - 2, (int) (dY - 2), sLetter );*/







	pDC->TextOut( round (dX - 2),
		round (dY - 2),
		sLetter );
	pDC->TextOut( round (dX + 2),
		round (dY + 2),
		sLetter );					
	pDC->TextOut( round (dX - 2),
		round (dY + 2),
		sLetter );					
	pDC->TextOut( round (dX + 2),
		round (dY - 2),
		sLetter );
	pDC->TextOut( round (dX - 2),
		round (dY),
		sLetter );
	pDC->TextOut( round (dX + 2),
		round (dY),
		sLetter );
	pDC->TextOut( round (dX ),
		round (dY + 2),
		sLetter );
	pDC->TextOut( round (dX ),
		round (dY - 2),
		sLetter );

	double dTRX = (round (dX + 1) + round (dX + 2))/2;
	double dTRY = (round (dY + 1) + round (dY + 2))/2;
	double dBLX = (round (dX - 1) + round (dX - 2))/2;
	double dBLY = (round (dY + 1) + round (dY + 2))/2;

	pDC->TextOut( dBLX, dBLY, sLetter );
	pDC->TextOut( dBLX, dBLY - 2, sLetter );
	pDC->TextOut( dTRX + 1, dTRY, sLetter );
	pDC->TextOut( dTRX + 1, dTRY - 2, sLetter );

	pDC->TextOut( dTRX, round (dY + 2), sLetter );
	pDC->TextOut( dTRX, round (dY - 2), sLetter );
	pDC->TextOut( dTRX - 2, round (dY + 2), sLetter );
	pDC->TextOut( dTRX - 2, round (dY - 2), sLetter );














	//pDC->TextOut( (int) (posLetter.dX - 1),
	//	(int) (posLetter.dY - 1),
	//	sLetter );
	//pDC->TextOut( (int) (posLetter.dX + 1),
	//	(int) (posLetter.dY + 1),
	//	sLetter );					
	//pDC->TextOut( (int) (posLetter.dX - 1),
	//	(int) (posLetter.dY + 1),
	//	sLetter );					
	//pDC->TextOut( (int) (posLetter.dX + 1),
	//	(int) (posLetter.dY - 1),
	//	sLetter );

	/*pDC->TextOut( (int) (posLetter.dX - 2),
	(int) (posLetter.dY - 2),
	sLetter );
	pDC->TextOut( (int) (posLetter.dX + 2),
	(int) (posLetter.dY + 2),
	sLetter );					
	pDC->TextOut( (int) (posLetter.dX - 2),
	(int) (posLetter.dY + 2),
	sLetter );					
	pDC->TextOut( (int) (posLetter.dX + 2),
	(int) (posLetter.dY - 2),
	sLetter );*/
}

void CColorManager::DrawAreaLabelShadow(CDC* pDC, double dX, double dY, CString sLetter)
{
	pDC->SetTextColor( RGB(55,55,3) );
	/*pDC->TextOut( (int) (dX - 2),
		(int) (dY - 2),
		sLetter );
	pDC->TextOut( (int) (dX + 2),
		(int) (dY + 2),
		sLetter );					
	pDC->TextOut( (int) (dX - 2),
		(int) (dY + 2),
		sLetter );					
	pDC->TextOut( (int) (dX + 2),
		(int) (dY - 2),
		sLetter );
	pDC->TextOut( (int) (dX - 2),
		(int) (dY),
		sLetter );
	pDC->TextOut( (int) (dX + 2),
		(int) (dY),
		sLetter );
	pDC->TextOut( (int) (dX),
		(int) (dY + 2 ),
		sLetter );
	pDC->TextOut( (int) (dX),
		(int) (dY - 2 ),
		sLetter );

	double dTRX = ((int) (dX + 1) + (int) (dX + 2))/2;
	double dTRY = ((int) (dY + 1) + (int) (dY + 2))/2;
	double dBLX = ((int) (dX - 1) + (int) (dX - 2))/2;
	double dBLY = ((int) (dY + 1) + (int) (dY + 2))/2;*/



	pDC->TextOut( round (dX - 2),
		round (dY - 2),
		sLetter );
	pDC->TextOut( round (dX + 2),
		round (dY + 2),
		sLetter );					
	pDC->TextOut( round (dX - 2),
		round (dY + 2),
		sLetter );					
	pDC->TextOut( round (dX + 2),
		round (dY - 2),
		sLetter );
	pDC->TextOut( round (dX - 2),
		round (dY),
		sLetter );
	pDC->TextOut( round (dX + 2),
		round (dY),
		sLetter );
	pDC->TextOut( round (dX),
		round (dY + 2 ),
		sLetter );
	pDC->TextOut( round (dX),
		round (dY - 2 ),
		sLetter );

	double dTRX = (round (dX + 1) + round (dX + 2))/2;
	double dTRY = (round (dY + 1) + round (dY + 2))/2;
	double dBLX = (round (dX - 1) + round (dX - 2))/2;
	double dBLY = (round (dY + 1) + round (dY + 2))/2;




	pDC->TextOut( dBLX,     dBLY,     sLetter );
	pDC->TextOut( dBLX,     dBLY - 2, sLetter );
	pDC->TextOut( dTRX + 1, dTRY,     sLetter );
	pDC->TextOut( dTRX + 1, dTRY - 2, sLetter );
	pDC->TextOut( dTRX,     dTRY - 1, sLetter );
	pDC->TextOut( dTRX + 1, dTRY - 1, sLetter );
	pDC->TextOut( dBLX,     dBLY - 1, sLetter );
}




int CColorManager::RoundColor(int ncolor)
{
	int ncolorgrid  = 0;
	int ngridsize   = 51;
	int ngridnumber = 5;

	for(int i=0;i<=ngridnumber;i++)
	{
		ncolorgrid = ngridsize*i;

		if( abs(ncolorgrid-ncolor) < (ngridsize/2) )
		{
			ncolor = ncolorgrid;
			break;
		}
	}

	return ncolor;
}

COLORREF CColorManager::GetGifPaletteColor(COLORREF gifcolor)
{
	int nred, ngreen, nblue;
	
	nred = RoundColor((int)GetRValue(gifcolor));
	ngreen = RoundColor((int)GetGValue(gifcolor));
	nblue = RoundColor((int)GetBValue(gifcolor));

	return RGB(nred,ngreen,nblue);
}

