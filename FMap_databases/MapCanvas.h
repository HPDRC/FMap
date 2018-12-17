#pragma once

#include <string>
//#include "MapUtility.h"
#include <stack>
//#include <afxtempl.h>
#include "ImagePixelAccessOptimizer.h"

using namespace std;


class CShape;
class CFormatPaintTool;

//pair<double, double> coordinates;
//typedef CList<coordinates> CArcPointList;

class CXMLCoordinates
{
public:

	double longitude;
	double latitude;
	double altitude;

public:
    CXMLCoordinates()
	{
		longitude = 0.0;
        latitude  = 0.0;
		altitude  = 0.0;
	}
	CXMLCoordinates(double lgd, double ltd)
	{
		this->longitude = lgd;
		this->latitude  = ltd;
	}
	CXMLCoordinates(double lgd, double ltd, double atd)
	{
		this->longitude = lgd;
		this->latitude  = ltd;
		this->altitude  = atd;
	}

	CXMLCoordinates& operator=(const CXMLCoordinates& coor)
	{
		longitude = coor.longitude;
		latitude = coor.latitude;
		altitude = coor.altitude;

		return *this;
	}
};

typedef CList<CXMLCoordinates> CArcPointList;


class CXMLData
{
public:
	
	int nXMLDataType;
	string strSourceName;
	double dX;
	double dY;
	double dRotation;

	CArcPointList m_arcPointList;

	//COLORREF labelColor;

public:
	CXMLData()
	{
		nXMLDataType  = 0;
		strSourceName = "";
		dX = 0.0;
		dY = 0.0;
		dRotation = 0.0;
	}
	CXMLData(int nType, string strName, double dX, double dY)
	{
		this->nXMLDataType  = nType;
		this->strSourceName = strName;
		this->dX = dX;
		this->dY = dY;
	}
	CXMLData(int nType, string strName, double dX, double dY, double dRotation)
	{
		this->nXMLDataType  = nType;
		this->strSourceName = strName;
		this->dX = dX;
		this->dY = dY;
		this->dRotation = dRotation;
	}

	CXMLData(const CXMLData& data)
	{
		nXMLDataType = data.nXMLDataType;
		strSourceName = data.strSourceName;
		dX = data.dX; dY = data.dY;
		dRotation = data.dRotation;

		POSITION pos = data.m_arcPointList.GetHeadPosition();
		while( pos != NULL )
		{
			m_arcPointList.AddTail( data.m_arcPointList.GetNext( pos ) );
		}		 
	}

	CXMLData& operator=(const CXMLData& data)
	{
		nXMLDataType = data.nXMLDataType;
		strSourceName = data.strSourceName;
		dX = data.dX; dY = data.dY;
		dRotation = data.dRotation;

		POSITION pos = data.m_arcPointList.GetHeadPosition();
		while( pos != NULL )
		{
			m_arcPointList.AddTail( data.m_arcPointList.GetNext( pos ) );
		}

		return *this;
	}

	CXMLData(int nType, string strName, CArcPointList &arcPointList)
	{
		this->nXMLDataType  = nType;
		this->strSourceName = strName;
		dX = 0.0;
		dY = 0.0;
		dRotation = 0.0;

		POSITION pos = arcPointList.GetHeadPosition();
		while( pos != NULL )
		{
			m_arcPointList.AddTail( arcPointList.GetNext( pos ) );
		}
	}
	
};

typedef CList<CXMLData> CXMLDataList;

class CMapCanvas
{
public:
	CMapCanvas(void);
public:
	~CMapCanvas(void);

public:
	//counter for all points, arcs, areas
	int nTotalPoint, nTotalArc, nTotalArea, nTotalIcon;
	bool hasTransGIF;
	bool hasTransPNG;

private:

	CImage m_Image;
	// map dc // generally it from a image
	CDC* m_pDCCanvas;

	//CMapStringToPtr* pColorTable;
	CMapColorTable* pColorTable;

	//xml data list
	CXMLDataList m_XMLDataList;

public:

//	bool DrawShape(CShape* pShapeObject, CFormatPaintTool* pFormat);
	
//CDoubleRect* pRectExt,
	bool InitCanvas(int nWidth, int nHeight, COLORREF crBKColor, bool bTransparence, int nBPP, bool hasGIFOutput, TransInfo* seastrans);
public:
	// save canvas to image
	bool SaveToFile(CString& sFile);

	bool SaveToXMLFile(CString& sXMLFile);
	bool SaveToXMLStream();

	bool SaveToTxtFile(CString& sXMLFile);


	CImage& getImage()
	{
		return m_Image;
	}

	//add xml data to list
	void AddXMLData(const CXMLData& xmlData)
	{
		m_XMLDataList.AddHead( xmlData );
	}

	bool SaveToStream();
	int ConstructGif(char* bufout);
	int ConstructPNG(char* bufout);
	//int ConstructTransGif(char* bufout);
	bool SaveToGIFStream();
	bool SaveToGIFFile(CString& sFile);

	bool SaveToPNGStream();
	bool SaveToPNGFile( CString& sFile );

	IStream* MakeGIFTrans(IStream* pStream);
	IStream* MakePNGTrans(IStream* pStream);

	bool setTransGIF( bool hasTransGIFOutput );
	bool setTransPNG( bool hasTransPNGOutput );

	//bool SetColorTable( CMapStringToPtr* colortable );
	bool SetColorTable( CMapColorTable* colortable );
	std::string IntDec2StrHex(int dec);
	CString ColorToString(Gdiplus::Color pixel);

public:
	void ReleaseCanvas(void);
public:
	CDC* getCanvasDC(void);
};
