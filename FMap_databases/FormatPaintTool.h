#pragma once
#include "afxwin.h"
//#include "shaperecords.h"
#include "TransInfoManager.h"
#include "ColorManager.h"
#include "CoordinateTransformer.h"
#include "utility/Utility.h"
#include "ColorManager.h"
#include "configure.h"
#include "fmap.h"
#include <io.h>
#include <afx.h>
#include "CoordinateTransformer.h"
#include "LabelManager.h"
#include "shapeObject.h"
#include "lineShape.h"
#include "PointShape.h"
#include "PolygonShape.h"
#include "MapCanvas.h"
#include "Placehoder.h"


class CShape;
class CMapCanvas;
class CPlacehoder;

class CFormatPaintTool
{

public:
	//create a holder
	CPlacehoder* m_pPlaceHolder;

private:
		CPen defaultPen/*mainPen*/;
		CPen* oldPen ;
		CFont* old_font;

public:
	CFormatPaintTool(void);
public:
	virtual ~CFormatPaintTool(void);

public:
	COLORREF crShapeColor;
	COLORREF crLalelColor;

	TransInfo transInfo;
	bool hasTransInfo;
	OverridedColor colorInfo;

	CString picto;

	int nldensity;  //1 is most dense, 3 is least dense
	//int nfontsize;  //1 is the regular, 2 is smaller one
	int nlinewidth; //1 is regular, 2 is regular+1
	int nfontheight;
	int nfontwidth;
	int niconsize;

	std::string str_font_style;

private:

	double dEnlargeRatio;

	bool isXMLOutput;//XML Output status
	bool isXMLUTM;//XML UTM coordinates

	bool isGIFOutput;
	bool isPNGOutput;

	//highlight Area
	CString strHLArea;
	bool isHLArea;

	//Lat, Long
	bool isLatLong;

public:
	bool hasStreets, hasSechwys, hasMajHwys, hasDetailroads, hasMainroads;

//	CFont 
public:
	// the font of the label
	CFont labelFont;
public:

	void setTransInfo(TransInfo trans)
	{
		transInfo = trans;
		hasTransInfo = true;
	}

	void setLetterEnlargeRatio(double dLER)
	{
		dEnlargeRatio = dLER;
	}

	void setXMLOutput( bool isXML )
	{
		isXMLOutput = isXML;
	}

	void setXMLUTM( bool isUTM )
	{
		isXMLUTM = isUTM;
	}

	void setGIFOutput( bool isGIF)
	{
        isGIFOutput = isGIF;
	}

	void setPNGOutput( bool isPNG)
	{
		isPNGOutput = isPNG;
	}

	void setHighlightAreaID( CString strID, bool isID)
	{
		strHLArea = strID;
        isHLArea  = isID;
	}

	void setLatLong( bool isLatLongCoordinates)
	{
		isLatLong = isLatLongCoordinates;
	}


	void setPolygonColor(OverridedColor color)
	{
		this->colorInfo.line  = color.line;
		this->colorInfo.area  = color.area;
		this->colorInfo.label = color.label;
	}

	void setLineStringColor(OverridedColor color)
	{
		this->colorInfo.line  = color.line;
		this->colorInfo.area  = "";
		this->colorInfo.label = "";
	}

	void setLabelDensity( bool labeldensity)
	{
		nldensity = labeldensity;
	}

	void setLineWidth( int width )
	{
		this->nlinewidth = width;
	}

	void setFontSize( int height, int fwidth )
	{
		this->nfontheight = height;
		this->nfontwidth  = fwidth;
	}

	/*void setFontHeight( int height )
	{
		this->nfontheight = height;
	}

	void setFontWidth( int fwidth )
	{
		this->nfontwidth = fwidth;
	}*/

	void setIconSize( int isize )
	{
		this->niconsize = isize;
	}

	void setFontStyle( std::string font )
	{
		this->str_font_style = font;
	}

	/////////////////////////////////////////////
	//set the street type
	//void setStreets(void)
	//{
 //       hasStreets = true;
	//}
	//void setSechwys(void)
	//{
	//	hasSechwys = true;
	//}
	//void setMajHwys(void)
	//{
	//	hasMajHwys = true;
	//}
	//void setDetailroads(void)
	//{
	//	hasDetailroads = true;
	//}
	//void setMainroads(void)
	//{
	//	hasMainroads = true;
	//}
	///////////////////////////////////////////////


public:
	// prepare to draw shape 
	bool BeginDrawShape(void);
	// end Draw Shape 
	bool EndDrawShape(void);

	bool BeginDrawLabel(void);

	bool EndDrawLabel(void);

public:

    //bool DrawShape(CShape* pShape, ShapeUtility::ShapeType shapeType);
	bool DrawShape(CShape* pShape, ShapeUtility::ShapeType shapeType, CString strColor, int nPenWidth);

	bool DrawLabel(CShape* pShape, ShapeUtility::ShapeType shapeType);
	// the canvas to be drown on
	CMapCanvas* m_pCanvas;
	// the transformer that make dMap to dLat/dlong
	CCoordinateTransformer* m_pTransformer;


public:
	bool SetCanvas(CMapCanvas* pCanvas);
	bool SetTransformer(CCoordinateTransformer* pTransformer);


	bool GetLabelDensityLevel(int nldl, bool* pIsPutBottom, bool* pIsPutRight, 
		bool* pIsPutUp,bool* pIsPutLeft)
	{
		if(2 == nldl)
		{
			*pIsPutUp = false;
			*pIsPutLeft = false;
		}
		else if(1 == nldl)
		{
			*pIsPutRight = false;
			*pIsPutUp = false;
			*pIsPutLeft = false;
		}
		else
		{
			//if(( 3 == nldl) || ( 0 == nldl))
			//the most positions
			//return true;
		}
		return true;
	}


};