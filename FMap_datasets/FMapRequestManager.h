#pragma once
#include "stdafx.h"
#include "FMapRequest.h"
#include <string>
#include "TransInfoManager.h"
#include "FMapRequest.h"

using namespace std;


class CFMapRequestManager
{
private:
	bool isXML, isKMLReq, isKMLUTMReq, isGMLReq, isGMLUTMReq, 
		isAtlasReq, isAtlasUTMReq, isUTM, isTransGIF, isGIF, isPNG, isHighlight, isTxt, isNorthHemisphere;

public:
	CFMapRequestManager(void);
public:
	~CFMapRequestManager(void);

public:
	//void CalReqRect(void);
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

	bool HasNewResSyntax(std::string overlays);

	bool IsResVisible(std::string *overlays, double curres);
	bool IsResVisibleDot(std::string overlays);

	std::string RefactLayer( bool hasGIF, std::string strReq, bool* hasTransCanvas, TransInfo *ptransInfo);

	CString InitRequest(CString strReq);

	bool hasGIFOutput( CString strTarImaType );
	bool HasTransGIF( CString strTarImaType );

	bool hasPNGOutput( CString strTarImaType );

	bool makeResVisible(std::string overlay, std::pair<double, double> range, 
		Overlays& overlays, double res);

	int GetLineWidth( int& lineWidth );

	bool HasLatLongReq(std::string lat1, std::string long1, std::string lat2, std::string long2, std::string level);
	bool HasLatLongReqinUTM(std::string lat1, std::string long1, std::string lat2, std::string long2, std::string prj, std::string reqwidth, std::string reqheight);

	std::string GetLatLongFiles(std::string str_filename);

	bool HasLatLongStreetRequest(std::string str_layer_postfix);

	bool HasFAADDRReq(CString* pstr);

	bool HasNorthUTM(int utm);

	
};
