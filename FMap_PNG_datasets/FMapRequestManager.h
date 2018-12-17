#pragma once
#include "stdafx.h"
#include "FMapRequest.h"
#include <string>
#include "TransInfoManager.h"
#include "FMapRequest.h"
#include "ColorManager.h"
#include "Utility\Transformer.h"
#include "configure.h"

extern Log logObj;

using namespace std;


class CFMapRequestManager
{
private:
	bool isXML, isKMLReq, isKMLUTMReq, isGMLReq, isGMLUTMReq, 
		isAtlasReq, isAtlasUTMReq, isUTM, isTransGIF, isGIF, isHighlight, isTxt, isPNG, isTransPNG;




public:
	CFMapRequestManager(void);
public:
	~CFMapRequestManager(void);

public:
	//void CalReqRect(void);


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

	std::string GetLatLongFiles(std::string str_filename);

	int GetLayerType(CString layer);

	CString GetTableName(CString name);
	CString GetFieldName(CString str, CString tablename);

	//bool GetPolygonColor(OverridedColor* color, CString str, CMapStringToPtr* color_table);
	bool GetPolygonColor(OverridedColor* color, CString str, CMapColorTable* color_table);
	bool GetLineStringColor(OverridedColor* color, CString str, CMapColorTable* color_table);

	bool GetPolygonLabelSize(int* pnsize, CString str);

	bool HasPointLable(CString str);
	bool HasPointIcon(CString str);

	bool HasLineStringLable(CString str);
	bool HasLineStringShape(CString str);

	bool HasPolygonLable(CString str);
	//bool HasPolygonBoundaryLine(CString str);
	bool HasPolygonFill(CString str);
	bool HasPolygonBoundary(CString str);

	//std::string DoubleToString(double d);

	double GetLatLongExt(double d, double doffset, bool isBigger);

	bool GetLatLongExtByPixel(double* dlong, double* dlat, int nlevel, bool isUpLeft, int nPixelOffset);

	bool HasLODData(int ndatatype);

	bool HasNorthernHemisphere(double lat1);

	bool HasNorthUTM(int utm);
	
};
