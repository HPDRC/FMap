#pragma once
#include "stdafx.h"
#include "FMapRequest.h"
#include <string>
#include "TransInfoManager.h"
#include "FMapRequest.h"
#include "ColorManager.h"
#include "Utility\Transformer.h"
#include "configure.h"
#include "CoordinateTransformer.h"

#include <map>

extern Log logObj;

using namespace std;


class CFMapRequestManager
{
private:
	bool isXML, isKMLReq, isKMLUTMReq, isGMLReq, isGMLUTMReq, 
		isAtlasReq, isAtlasUTMReq, isUTM, isTransGIF, isGIF, isHighlight, isTxt, isPNG, isTransPNG,
		isSourceDateReq;




public:
	CFMapRequestManager(void);
public:
	~CFMapRequestManager(void);

public:
	//void CalReqRect(void);

	//for performance, generate two tables
	map<int, double> levelToResTable;
	map<double, int> resToLevelTable;
	//CMap<int, int, double, double> levelResTable; 
	void InitLevelResTabel(void);

	CTransformer transformer;

	std::string InitFilledAreaReq( std::string strAreaReq, bool* pHasFilledArea );
	bool CFMapRequestManager::HasFilledAreaReq(std::string strAreaReq, bool hasFilledAreaReq);

	bool HasXML( CString strTarImaType );
	bool HasUTM( CString strTarImaType );

	bool HasKMLReq( CString strTarImaType );
	bool HasKMLUTMReq( CString strTarImaType );

	bool HasGMLReq( CString strTarImaType );
	bool HasGMLUTMReq( CString strTarImaType );

	bool HasAtlasReq( CString strTarImaType );
	bool HasAtlasUTMReq( CString strTarImaType );

	bool HasHighlight( CString strHigh );
	bool HasTxt( CString strTxt );
	bool HasHighlightCenter( CString strHL );

	std::string RefactLayer( bool hasGIF, bool hasPNG, std::string strReq, bool* hasTransCanvas, TransInfo *ptransInfo);

	CString InitRequest(CString strReq);

	bool hasGIFOutput( CString strTarImaType );
	bool hasPNGOutput( CString strTarImaType );
	bool HasTransGIF( CString strTarImaType );
	bool HasTransPNG( CString strTarImaType );

	bool makeResVisible(std::string overlay, std::pair<double, double> range, 
		Overlays& overlays, double res);

	bool HasLatLongReq(std::string lat1, std::string long1, std::string lat2, std::string long2, std::string level);
	bool HasLatLongReqinUTM(std::string lat1, std::string long1, std::string lat2, std::string long2, std::string prj, std::string reqwidth, std::string reqheight);
	bool HasQuadKey(CString quadkey);

	std::string GetLatLongFiles(std::string str_filename);

	int GetLayerType(CString layer);

	CString GetTableName(CString name);
	CString GetFieldName(CString str, CString tablename);

	bool GetDateAndSource(CString layername, CString *source, CString *date);

	//bool GetPolygonColor(OverridedColor* color, CString str, CMapStringToPtr* color_table);
	bool GetPolygonColor(OverridedColor* color, CString str, CMapColorTable* color_table);
	bool GetLineStringColor(OverridedColor* color, CString str, CMapColorTable* color_table);

	bool GetPolygonLabelSize(int* pnsize, CString str);

	bool GetLevelFromURL(CString& quadkey, int& nlevel, std::string& str_level );
	bool GetWidthHeight(const bool& hasLatLong, const bool& hasQuadKey, 
		const std::string& XN, const std::string& YN, int& nWidth, int& nHeight, 
		const double& x1, const double& x2, const double& y1, const double& y2, const double& res);

	bool GetLatLongFromX1Y1X2Y2(const std::string& X1, const std::string& X2,
		const std::string& Y1, const std::string& Y2, 
		const std::string& UTMZ1, std::string& UTMZ2, 
		Gdc_Coord_3d& gdc1, Gdc_Coord_3d gdc2);

	bool GetLatLongFromXNYN(const std::string& XN, const std::string& YN, 
		const std::string& UTMZ, const double& res,  
		Gdc_Coord_3d& gdc1, Gdc_Coord_3d gdc2);

	bool GetLatLongFromLatLong(const std::string& str_lat1, const std::string& str_lat2,  
	const std::string& str_long1, const std::string& str_long2,  
	Gdc_Coord_3d& gdc1, Gdc_Coord_3d& gdc2);

	bool GetLatLongFromQuadKey(const CString& str_quadkey, /*int& nlevel, */const CCoordinateTransformer& transformer,
		Gdc_Coord_3d& gdc1, Gdc_Coord_3d& gdc2);

	bool GetQuadKeyFromLatLong(const CDoubleRect& rcSourceExt, const CCoordinateTransformer& transformer, 
		CString& str_quadkey, const int& nlevel, int& nMSOffsetX, int& nMSOffsetY);

	int GetLineWidth( CString layer );

	bool HasPointLable(CString str);
	bool HasPointIcon(CString str);

	bool GetPointFontSize(CString str, int* pnheight, int* pnwidth);
	bool GetPointIconSize(CString str, int* pniconsize);

	bool HasLineStringLable(CString str);
	bool HasLineStringShape(CString str);

	bool HasPolygonLable(CString str);
	//bool HasPolygonBoundaryLine(CString str);
	bool HasPolygonFill(CString str);
	bool HasPolygonBoundary(CString str);
	bool HasAreaSourceDateReq(CString layername);
	bool HasPolygonDraw(CString str);

	//std::string DoubleToString(double d);

	double GetLatLongExt(double d, double doffset, bool isBigger);

	bool GetLatLongExtByPixel(double* dlong, double* dlat, int nlevel, bool isUpLeft, int nPixelOffset);

	bool HasLODData(int ndatatype);

	bool HasNorthernHemisphere(double lat1);


	bool HasNewResSyntax(std::string overlays, bool* isLevelResSyntax);

	bool IsResVisible(std::string *overlays, double curres);
	bool IsResVisible(CString *layername, double curres);
	bool IsResVisible(CString *layername, int level);


	int GetLevelFromRes(double& res);
	double GetResFromLevel(int& level);

	int GetLayersNum(std::string& overlays);

	
};
