;// FMapDraw.cpp : Defines the entry point for the console application.
//


#define _CRTDBG_MAP_ALLOC
//#include<stdlib.h>
#include "stdafx.h"
#include "shapeObject.h"
#include "LayerDataSource.h"
#include "QixShapeFileLayerDataSource.h"
#include "StringLayerDataSource.h"
#include "PolygonLayer.h"
#include "PointLayer.h"
#include "lineshape.h"
#include "lineLayer.h"
#include "MapCanvas.h"
#include "CoordinateTransformer.h"
#include "shaperecords.h"
#include "TransInfoManager.h"
#include "configure.h"
#include "two_convert.h"
#include "maptree.h"
#include "FMapRequest.h"
#include "fmap.h"
#include "ImageTransformer.h"
#include <iostream>
#include <string>
#include <queue>
#include "atlutil.h"
#include <atlbase.h>
#include <atlstr.h>
#include "FMapDraw.h"
#include "placehoder.h"
#include <crtdbg.h> 
#include <math.h>
#include "FMapRequestManager.h"
#include ".\\Utility\\Transformer.h"


Configure configure;
Log logObj(configure);



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;


using namespace std;


//void DrawArea(double dX1, double dY1, double dX2, double dY2, char sShpSrcFile[], char sFieldName[], CDC* pDC);
//bool MergerStreet( ,double dX1, double dY1, double dX2, double dY2)

double CLineShape::m_dConnectAcceptOffsetX = 0.0001; // here is hard code
double CLineShape::m_dConnectAcceptOffsetY = 0.0001; // here is hard code
CPlacehoder g_placeHolder;	

//check the query to see whether it has two parts of steet, like "street"
bool checkQuery(const char *query)
{
	const char *pos = query;
	int cnt = 0;
	while ((pos = strstr(pos + 1, "street")) != NULL)
	{
		cnt ++;
	}
	if (cnt == 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//this function is used to change "...street._n+street.1_..." to "street.1n"
//this function sucks...sorry about that
void updateQuery(const char *query, char *newQuery)
{
	const char *pos = strstr(query, "street");
	*newQuery = '\0';
	strncat(newQuery, query, pos - query);

	//two parameters indicate whether draw label or line
	char para1, para2;
	char mark = '_';
	const char *paraPos = strstr(pos, ".");
	if (*(paraPos + 1) == mark)
	{
		para2 = *(paraPos + 2);
	}
	else
	{
		para1 = *(paraPos + 1);
	}

	pos = strstr(pos + 1, "street");
	paraPos = strstr(pos, ".");
	if (*(paraPos + 1) == mark)
	{
		para2 = *(paraPos + 2);
	}
	else
	{
		para1 = *(paraPos + 1);
	}

	strcat(newQuery, "street");
	strcat(newQuery, ".");
	strncat(newQuery, &para1, 1);
	strncat(newQuery, &para2, 1);
	strcat(newQuery, paraPos + 3);
	strcat(newQuery, "\0");

	return;
}
int main(/*int argc, TCHAR* argv[], TCHAR* envp[]*/)
{
	/*void* ptr = calloc(480000, 1);
    printf("Content-type: image/ppm\n\n");
	fwrite(ptr,480000,1,stdout);
	free(ptr);
	exit(0);*/
//test environment above

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#ifdef UNICODE
	ASSERT(FALSE);
#else

#endif

	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		logObj.initLogObj(configure);

		logObj.varWriteToLog(_INFO, "ss", "Start a new record: ", getenv("QUERY_STRING"));

#ifdef DEBUG
		fprintf(stderr, "Request String: %s\n", getenv("QUERY_STRING"));
#endif

#ifdef TERRA_NODE
		string query = getenv("QUERY_STRING");
        if (query.find("TargetImageType") == -1)
        {
            query = query + "&TargetImageType=ppm";
        }
		char newQuery[2048] = {0};
		strcpy(newQuery, query.c_str());
		if (checkQuery(query.c_str()))
		{
			updateQuery(query.c_str(), newQuery);
		}

		RequestString requestString(newQuery);		
        
#else
		//string query("TargetImageType=png&style=BT&Composite=wtown-0-100-+incorp-0-100-.00v+street-0-100-._n+street-4-100-.2_&X1=591360&Y1=4515840&UTMZ1=18&X2=599040&Y2=4508160&UTMZ2=18&res=19.2&shadow=1");
		string query("X1=538479.275705&Y1=2899871.275966&X2=592083.655068&Y2=2866611.047741&RES=38.400000&UTMZ1=17&UTMZ2=17&COMPOSITE=street-0-30-.l_+street-30-100-.l_+street-0-100-._n");
		char newQuery[2048] = {0};
		strcpy(newQuery, query.c_str());
		if (checkQuery(query.c_str()))
		{
			updateQuery(query.c_str(), newQuery);
		}
		RequestString requestString(newQuery);
		//RequestString requestString("TargetImageType=gif&style=BT&Composite=flpropertiesvalue-0-0.3-&X1=578998&Y1=2914497&UTMZ1=17&X2=579059&Y2=2914436&UTMZ2=17&res=0.15");
		//RequestString requestString("TargetImageType=gif&Composite=FA_Point-_1.a+FA_Point-_1.o+FA_Parcel-_1.t00&X1=375999.6&Y1=3773199.6&UTMZ1=11&X2=376399.2&Y2=3772800&UTMZ2=11&res=1.2");

		//RequestString requestString("TargetImageType=gif&Composite=FA_Parcel-_1.t00&X1=357600&Y1=3516799.2&UTMZ1=13&X2=358399.2&Y2=3516000&UTMZ2=13&res=2.4");

#endif

		string comp = requestString.getValue("COMPOSITE");

		CFMapRequestManager reqManager;

		//initialize the request, decode
		//comp = reqManager.InitRequest(comp.c_str());

		//set drawing the filled area first
		bool hasFilledAreaReq = false;
		hasFilledAreaReq = reqManager.HasFilledAreaReq( comp, hasFilledAreaReq );
		//comp = reqManager.InitFilledAreaReq( comp, &hasFilledAreaReq );

		//set XML
		CString strTarImaType  = requestString.getValue("TargetImageType").c_str();
		bool hasXMLOutput = reqManager.HasXML( strTarImaType );
		bool hasXMLUTM    = reqManager.HasUTM( strTarImaType );
		//set GIF
		bool hasTransGIFOutput  = reqManager.HasTransGIF( strTarImaType );
		bool hasGIFOutput       = reqManager.hasGIFOutput( strTarImaType );

		bool hasPNGOutput       = reqManager.hasPNGOutput( strTarImaType );

		//for all, ramb and seas layers
		bool hasTransCanvas = true;
		TransInfo seastrans;
		comp = reqManager.RefactLayer( hasGIFOutput, comp, &hasTransCanvas, &seastrans);

		bool hasLatLongStreetRequest = false;

		//bool hasFAADDR = false;

		//for highlightArea
		CString strHighlightAreaID = requestString.getValue("HighID").c_str();
		strHighlightAreaID = reqManager.InitRequest(strHighlightAreaID);
		bool hasHighlightArea      = reqManager.HasHighlight( strHighlightAreaID );
		//bool hasHighlightCenter    = reqManager.HasHighlightCenter( requestString.getValue("Highlight").c_str() );

		//484-490:add by Felix:for the shadow control
		string shadowStr = requestString.getValue("shadow").c_str();
		bool hasShadow = true;
		if (shadowStr == "0")
		{
			hasShadow = false;
		}

		//set txt
		bool hasTxt = reqManager.HasTxt( strTarImaType );


		string XN = requestString.getValue("XN");
		string YN = requestString.getValue("YN");


		//Lat, Long
		string str_lat1  = requestString.getValue("lat1");
		string str_long1 = requestString.getValue("long1");
		string str_lat2  = requestString.getValue("lat2");
		string str_long2 = requestString.getValue("long2");	
		string str_level = requestString.getValue("level");
		int nlevel       = atoi(str_level.c_str());
		//set Lat, Long
		bool hasLatLong = reqManager.HasLatLongReq(str_lat1, str_long1, str_lat2, str_long2, str_level);


		string str_prj = requestString.getValue("prj");
		string str_reqwidth = requestString.getValue("width");
		string str_reqheight = requestString.getValue("height");
		bool hasLatLonginUTM = reqManager.HasLatLongReqinUTM(str_lat1, str_long1, str_lat2, str_long2, str_prj, str_reqwidth, str_reqheight);


		int nlabeldensity = 0;
		string str_ldensity = requestString.getValue("labeldensity");
		if( "" != str_ldensity)
		{
			sscanf(str_ldensity.c_str(), "%d", &nlabeldensity);
		}
		


		int nReqWidth, nReqHeight;
		nReqWidth = 0;
		nReqHeight = 0;
		sscanf(str_reqwidth.c_str(), "%d", &nReqWidth);
		sscanf(str_reqheight.c_str(), "%d", &nReqHeight);

		rectObj rect;
		double x1,x2,y1,y2,res;
		int xn,yn;
		int utmz, utmz1, utmz2;
		Gdc_Coord_3d gdc1;
		Gdc_Coord_3d gdc2;
		
		string RES = requestString.getValue("RES");

		//process SM
		string SM = requestString.getValue("SM");
		double dLetterEnlargeRatio = 1, dSM = 0, dRES = 0;

		if("" != SM)
		{
			dRES = atof( RES.c_str() );
			dSM = atof( SM.c_str() );
			dLetterEnlargeRatio = dSM / dRES;
			//RES = SM;
		}


		if( (RES == "") && (!hasLatLong) )
		{
			CGIReply::replyText("Error: Resolution parameter not specified!");
			exit(1);
		}

		sscanf(RES.c_str(), "%lf", &res);

		if( hasLatLong || hasLatLonginUTM )
		{

			double dLat1  = atof(str_lat1.c_str());
			double dLat2  = atof(str_lat2.c_str());
			double dLong1 = atof(str_long1.c_str());
			double dLong2 = atof(str_long2.c_str());

			gdc1.longitude = dLong1;
			gdc2.latitude = dLat2;
			gdc2.longitude = dLong2;
			gdc1.latitude = dLat1;

		}
		else if( XN == "" || YN == "" )
		{
			string X1 = requestString.getValue("X1");
			string X2 = requestString.getValue("X2");
			string Y1 = requestString.getValue("Y1");
			string Y2 = requestString.getValue("Y2");

			if(X1 == "" || X2 == "" || Y1 == "" || Y2 == "")
			{
				CGIReply::replyText("Error: XN YN or X1 Y1 parameter not specified!");
				exit(1);
			}

			string UTMZ1 = requestString.getValue("UTMZ1");
			string UTMZ2 = requestString.getValue("UTMZ2");
			if("" == UTMZ2)
			{
				UTMZ2 = UTMZ1;
			}

			if(UTMZ1 == "")
			{
				CGIReply::replyText("Error: UTMZ1 parameter not specified!");
				exit(1);
			}

			sscanf(X1.c_str(), "%lf", &x1);
			sscanf(Y1.c_str(), "%lf", &y1);
			sscanf(X2.c_str(), "%lf", &x2);
			sscanf(Y2.c_str(), "%lf", &y2);

			sscanf(UTMZ1.c_str(), "%d", &utmz1);
			sscanf(UTMZ2.c_str(), "%d", &utmz2);

			bool isNorthUTM;
			isNorthUTM = reqManager.HasNorthUTM(utmz1);
			Utm_Coord_3d utmCoord1(x1, y1, 0, abs(utmz1), isNorthUTM/*not sure*/);
			isNorthUTM = reqManager.HasNorthUTM(utmz2);
			Utm_Coord_3d utmCoord2(x2, y2, 0, abs(utmz2), isNorthUTM);

			Utm_To_Gdc_Converter::Init();
			Utm_To_Gdc_Converter::Convert(utmCoord1, gdc1);
			Utm_To_Gdc_Converter::Convert(utmCoord2, gdc2);
		}
		else
		{
			string UTMZ = requestString.getValue("UTMZ1");
			if(UTMZ == "")
			{
				CGIReply::replyText("Error: UTMZ parameter not specified!");
				exit(1);
			}


			sscanf(XN.c_str(), "%d", &xn);
			sscanf(YN.c_str(), "%d", &yn);
			sscanf(UTMZ.c_str(), "%d", &utmz);

			x1 = xn*TILE_SIZE*res;	// 400 is the tile size
			x2 = (xn+1)*TILE_SIZE*res;
			y1 = yn*TILE_SIZE*res;
			y2 = (yn-1)*TILE_SIZE*res;

			Utm_Coord_3d utmCoord1(x1, y1, 0, utmz, true/*not sure*/);
			Utm_Coord_3d utmCoord2(x2, y2, 0, utmz, true);

			//Utm_Coord_3d utmCoord1(x1, y1, 0, utmz, false/*not sure*/);
			//Utm_Coord_3d utmCoord2(x2, y2, 0, utmz, false);

			Utm_To_Gdc_Converter::Init();
			Utm_To_Gdc_Converter::Convert(utmCoord1, gdc1);
			Utm_To_Gdc_Converter::Convert(utmCoord2, gdc2);
		}


		//the scope of longitude and latitude
		if(gdc1.longitude < -180 || gdc2.longitude >= 180
			|| gdc1.latitude < -90 || gdc2.latitude >= 90)
		{
			CGIReply::replyText("XN YN or X1 Y2 coordinates error, please check these parameters!");
			exit(1);
		}

		rect.minX = gdc1.longitude;
		rect.minY = gdc2.latitude;
		rect.maxX = gdc2.longitude;
		rect.maxY = gdc1.latitude;



		//for canvas and boundary
		CMapCanvas canvas;
		int nWidth = 0;
		int nHeight = 0;

		if( hasLatLong )  //Lat, Long
		{
			nWidth  = 256;
			nHeight = 256;
			if(nReqWidth != 0)
			{
				nWidth  = nReqWidth;
				nHeight = nReqHeight;
			}
		}
		else if( XN != "" || YN != "" )	// BT
		{
			nWidth  = 400;
			nHeight = 400;
		}
		else if( hasLatLonginUTM )//lat long in UTM
		{
			//int nReqWidth, nReqHeight;

			//sscanf(str_reqwidth.c_str(), "%d", &nReqWidth);
			//sscanf(str_reqheight.c_str(), "%d", &nReqHeight);
			nWidth  = nReqWidth;
		    nHeight = nReqHeight;
		}
		else // DT
		{	
			nWidth  = (int) ((x2-x1)/res);
			nHeight = (int) ((y1-y2)/res);
		}
		//else if(XN == "")	// DT
		//{	
		//	nWidth  = (int) ((x2-x1)/res);
		//	nHeight = (int) ((y1-y2)/res);
		//}
		//else			// BT
		//{
		//	nWidth = 400;
		//	nHeight = 400;
		//}

		//canvas.InitCanvas(nWidth, nHeight, RGB(255, 255, 255), true, 24 );
        
		//canvas.InitCanvas( nWidth, nHeight, RGB(0, 0, 255), hasTransCanvas, 24, hasGIFOutput, &seastrans);

		if( hasPNGOutput )
		{
			canvas.InitCanvas( nWidth, nHeight, RGB(0, 0, 255), hasTransCanvas, 32, hasPNGOutput, &seastrans);
		}
		else
		{
			canvas.InitCanvas( nWidth, nHeight, RGB(0, 0, 255), hasTransCanvas, 24, hasGIFOutput, &seastrans);
		}

		CCoordinateTransformer transformer;

		//set Lat, Long to transformer
		transformer.SetLatLong( hasLatLong );

		/* from source to dest coordinate system transformer */
		transformer.SetSourceDestCoordinateSys(CCoordinateTransformer::GCS, 
			CCoordinateTransformer::LINEAR_VIEW);

		CDoubleRect rcSourceExt, rcDestExt;	// BUG
		rcSourceExt.dX1 = rect.minX;
		rcSourceExt.dX2 = rect.maxX;
		rcSourceExt.dY1 = rect.minY;
		rcSourceExt.dY2 = rect.maxY;

		rcDestExt.dX1 = 0;
		rcDestExt.dY1 = 0;
		rcDestExt.dX2 = nWidth;
		rcDestExt.dY2 = nHeight;

		int nMSOffsetX = 0, nMSOffsetY = 0;
		if( hasLatLong )
		{
			int nPX1 = 0, nPX2 =0, nPY1 = 0, nPY2 = 0;
			transformer.m_pLinearTransformer->LatLongToPixelXY( 
				rcSourceExt.dY1, rcSourceExt.dX1, nlevel, &nPX1, &nPY1, 0, 0 );
			transformer.m_pLinearTransformer->LatLongToPixelXY( 
				rcSourceExt.dY2, rcSourceExt.dX2, nlevel, &nPX2, &nPY2, 0, 0 );
            nMSOffsetX = nPX1;
			nMSOffsetY = nPY2;
		}


		transformer.SetLinearConvertParameters( rcSourceExt, rcDestExt ,  true, false);
		//set the boundary of the placeholder for detect boundary confliction.
		g_placeHolder.SetBoundary( rcDestExt );


#ifdef DEBUG
		fprintf(stderr, "Composite: %s\n", comp.c_str());
#endif
        //////////////////////////////////////////////////////////
		//customized points
		Overlays overlays(comp);
		string sStrOverlay = overlays.getNextOverlay();	
		CString strPoints;
		bool hasStrPoint = false;
		while(sStrOverlay != "")
		{
			strPoints = sStrOverlay.c_str();

			int nStrStartPoz = 0;
			int nStrEndPoz = strPoints.GetLength() - 1;

			if( ('_' == strPoints.GetAt(nStrStartPoz))&&('_' == strPoints.GetAt(nStrEndPoz)) )
			{
				hasStrPoint = true;
				break;
			}

			sStrOverlay = overlays.getNextOverlay();
		}
		///////////////////////////////////////////////////////////

		reqManager.makeResVisible( "dining",      make_pair(0,3),   overlays, res);
		reqManager.makeResVisible( "hotel",       make_pair(1,16),  overlays, res);
		reqManager.makeResVisible( "ramb",        make_pair(0,5),   overlays, res);
		reqManager.makeResVisible( "biz",         make_pair(0,2),   overlays, res);
		reqManager.makeResVisible( "realestate",  make_pair(0,5),   overlays, res);
		reqManager.makeResVisible( "blk_pop",     make_pair(0,41),  overlays, res);
		reqManager.makeResVisible( "flproplines", make_pair(0,4.9), overlays, res);

		string overlay = overlays.getNextOverlay();
		//vector<string> files;
		vector<ShapeRecords> shapeRecords;
		
		while(overlay != "")
		{


			vector<string> files;

			TransInfo transInfo;
			OverridedColor color;
			ShapeRecords shapeRecord;
			string preOverlay;
			//string postfixOverlay;
			int nPosLastDigit = 0;
			double dResRange  = 0.0;
			double dLastDigit = 0.0;
			////////////////////////////////////////////////////////
			bool hasHLInfo = false;
			hasLatLongStreetRequest = false;

			//trim overlay, first find .xxx, then -xxx
			int iDot = (int) (overlay.find('.'));
			//if( reqManager.HasNewResSyntax(overlay) 
			//	&& reqManager.IsResVisibleDot(overlay))
			//{
			//	//this new syntax
			//	int x = 0;
			//}
			////this is old syntax
			//else 
			if( (iDot != string::npos) && (!reqManager.IsResVisibleDot(overlay)) )
			{
				// trans or override info exists
				string info = overlay.substr(iDot+1);
				//overlay only has the part before dot
				overlay = overlay.substr(0, iDot);

				hasLatLongStreetRequest = reqManager.HasLatLongStreetRequest(info);

				if( (info.at(0) == 'c') && (info.at(1) == '.') && (info.size() == HIGHLIGHTCENTER_LENGTH) )
                //if (overlay.find(".c.") != string::npos)
				{
					hasHLInfo = true;
					shapeRecord.SetHighlightCenterInfo( hasHLInfo );
					int iSecondDot = (int) (info.find('.'));
					if(iSecondDot != string::npos)
					{ 
						info = info.substr( iSecondDot + 1 );
					}
				}
				else
				{
					hasHLInfo = false;
					shapeRecord.SetHighlightCenterInfo( hasHLInfo );
				}

				switch(info.size()){
				case OVERRIDE_COLOR_LENGTH:	// line, label override color
					color.line = info.substr(0, 6);
					color.label = info.substr(6);
					shapeRecord.setColorInfo(color);
					break;
				case OVERRIDE_COLOR_LENGTH/2:		// line overrided color
					color.line = info.substr(0, 6);
					shapeRecord.setColorInfo(color);
					break;
				case AREA_TRANSPARENCY_LENGTH:		// area transparency info
					transInfo.type = TransInfo::area;
					transInfo.values[0] = transInfo.char2int(info[0]);
					transInfo.values[1] = transInfo.char2int(info[1]);
					transInfo.values[2] = transInfo.char2int(info[2]);
					shapeRecord.setTransInfo(transInfo);
					shapeRecord.setColorInfo(color);
					break;

				case AREA_COLOR_LENGTH:		// area color info
					transInfo.type = TransInfo::area;
					transInfo.values[0] = transInfo.char2int(info[0]);
					transInfo.values[1] = transInfo.char2int(info[1]);
					transInfo.values[2] = transInfo.char2int(info[2]);

					color.line = info.substr(3, 6);
					color.area = info.substr(9, 6);
					color.label = info.substr(15, 6);
					shapeRecord.setTransInfo(transInfo);
					shapeRecord.setColorInfo(color);

					break;

				case LINE_TRANSPARENCY_LENGTH:		// line transparency info
					transInfo.type = TransInfo::line;
					transInfo.values[0] = transInfo.char2int(info[0]);
					transInfo.values[1] = transInfo.char2int(info[1]);
					shapeRecord.setTransInfo(transInfo);

					break;

				case POINT_ICON_LENGTH:		// point info
					transInfo.type = TransInfo::point;
					transInfo.values[0] = transInfo.char2int(info[0]);
					//transInfo.values[1] = transInfo.char2int(info[1]);
					shapeRecord.setTransInfo(transInfo);
				}
			}

			//// find -xxx
			int iHyphen = (int) (overlay.find('-'));
			//new syntax
			if( reqManager.HasNewResSyntax(overlay) )
			{
				if(!reqManager.IsResVisible(&overlay, res))
				{
					overlay = overlays.getNextOverlay();
					continue;
				}
			}
			//old syntax
			else if(iHyphen != string::npos)
			{
				// two digits which decide the visibility of the overlay
				string visib = overlay.substr(iHyphen+1);
				overlay = overlay.substr(0, iHyphen);

				pair<int, int> range;
				double dMiniRes = 0.0; 
				double dMaxRes  = 0.0;

				if( ('_' == visib[0]) && ('_' != visib[1]) )
				{
					range.first  = 0;
					dMaxRes  = TransInfo::char2int(visib[1]);
					range.second = (int) (pow( 2,dMaxRes+1 ));
				}
				else if( ( '_' == visib[1] ) && ('_' != visib[0]) )
				{
					range.second = 0;
					dMiniRes = TransInfo::char2int(visib[0]);
					range.first  = (int) (pow( 2,dMiniRes ));
				}
				else if( ( '_' == visib[1] ) && ('_' == visib[0]) )
				{
					range.first  = 0;
					range.second = 1;
				}
				else
				{
					dMiniRes = TransInfo::char2int(visib[0]);
					dMaxRes  = TransInfo::char2int(visib[1]);
					/*range.first = TransInfo::char2int(visib[0]);
					range.second = TransInfo::char2int(visib[1]);*/
					range.first  = (int) (pow( 2,dMiniRes ));
					range.second = (int) (pow( 2,dMaxRes+1));
				}

				if(res < range.first || res >= range.second)	// should not request this overlay, drop it
				{
					overlay = overlays.getNextOverlay();
					continue;
				}
			}
			else if(hasLatLong){}
			else //find the last digit of overlay to define resolution
			{
				//if( iDot != string::npos )
				//{
				//	preOverlay = overlay.substr(0, iDot);
				//}
				//else
				//{
                    preOverlay = overlay;
				//}

				nPosLastDigit  = ( (int)(preOverlay.length()) ) - 1;
				string strLast = preOverlay.substr( nPosLastDigit );
				dLastDigit     = TransInfo::char2int( strLast[0] );
				if( '_' == strLast[0] )
				{
					dResRange = 8;
					overlay   = overlay.substr(0, nPosLastDigit);
				}
				else if( '0'==strLast[0] || '1'==strLast[0] || 
					     '2'==strLast[0] || '3'==strLast[0] ||
					     '4'==strLast[0] || '5'==strLast[0] )
				{
					dResRange = pow( 2,dLastDigit+5 );
					overlay   = overlay.substr(0, nPosLastDigit);
				}
				else
				{
					dResRange = res + 1;
				}

				if( res >= dResRange )	// should not request this overlay, drop it
				{
					overlay = overlays.getNextOverlay();
					continue;
				}

			}


			/*Overlay2Qix o2q(overlay, rect.minX, rect.maxX, rect.minY, rect.maxY, res);

			files = o2q.getQixFile(configure.getLevelOneDir(), configure.getLevelTwoDir());*/
			if( hasLatLong || hasLatLongStreetRequest )
			{
				string str_Files = reqManager.GetLatLongFiles(overlay);
				//files = str_Files;
				files.push_back(str_Files);
			}
			else
			{
				Overlay2Qix o2q(overlay, rect.minX, rect.maxX, rect.minY, rect.maxY, res);
				files = o2q.getQixFile(configure.getLevelOneDir(), configure.getLevelTwoDir());
                if (overlay.find("street") != -1)
                {
                    files.push_back("\\\\131.94.133.246\\e$\\streetdata\\Sechwys\\40.0\\-88.0.qix");
                    files.push_back("\\\\131.94.133.246\\e$\\streetdata\\MajHwys\\40.0\\-88.0.qix");
                }
			}


			/*CString test = files.at(0).c_str();
			logObj.varWriteToLog(_INFO, "ss", "shp file name: ", test);*/



			for(int i = 0; i < (int)(files.size()); i++)
			{
				char* ids = NULL;
				ShapeRecords record = shapeRecord;

				string baseName = files[i].substr(0, files[i].length()-3);
				record.setShpBaseName(baseName);

				char name[MAX_PATH_LENGTH] = {0};
				strcpy(name, baseName.c_str());
				ids = msSearchDiskTree(name, rect, 0);

				int number, type;
				double min[4];
				double max[4];
				SHPHandle handle = SHPOpen(name, "rb");
				SHPGetInfo(handle, &number, &type, min, max);

				msFilterTreeSearch(handle, number, ids, rect, record);

				/*int nsize = record.size();
				char* test = new char[100];
				itoa(nsize,test,10);
				logObj.varWriteToLog(_INFO, "ss", "record size: ", test);
				delete test;*/

				SHPClose(handle);

				record.setShpType(type);
				free(ids);
				shapeRecords.push_back(record);
			}

			overlay = overlays.getNextOverlay();
		}


		logObj.reInit();

		//CShape* pShape = NULL;*/
		CQixShapeFileLayerDataSource dataSource, dataSourcePoints, dataSourceArea;
		//////////////////////////////////////
		dataSource.SetExtend( rcSourceExt );
        //////////////////////////////////////

		CLineLayer layerStreet, layerStreetMerged;
		CPointLayer layerPoints;
		CPolygonLayer layerArea;

		//Line		
		layerStreet.m_asPropertiesField.Add( CString("HIGHWAY_NM") );//name
		layerStreet.m_asPropertiesField.Add( CString("ST_NAME") );//name
		layerStreet.m_asPropertiesField.Add( CString("streername") );//name
		layerStreet.m_asPropertiesField.Add( CString("LabelCQ") );//color of the label
		layerStreet.m_asPropertiesField.Add( CString("LineCQ") );//color of the line
		layerStreet.m_asPropertiesField.Add( CString("NAME1") );//samotrails name
		layerStreet.m_asPropertiesField.Add( CString("labeltq") );//new name

		layerStreet.m_asPropertiesField.Add( CString("NAME") );//wm_Water_line
		layerStreet.m_asPropertiesField.Add( CString("LOCAL_NUM") );//wm_Minor_routes
		layerStreet.m_asPropertiesField.Add( CString("ID") );//wm_Railroad_track

        layerStreet.m_asPropertiesField.Add( CString("POLYGON_NM") );//polyline
		layerStreet.m_asPropertiesField.Add( CString("OBJECTID") );//fiber_optics

		//layerStreet.m_asPropertiesField.Add( CString("st_nm_base") );

		//Point
		layerPoints.m_asPropertiesField.Add( CString("labeltq") );//name
		layerPoints.m_asPropertiesField.Add( CString("LABELCQ") );
		layerPoints.m_asPropertiesField.Add( CString("PictoFQ") );//picto
		layerPoints.m_asPropertiesField.Add( CString("column1") );//another name
		layerPoints.m_asPropertiesField.Add( CString("column10") );//dining&business point picto 
		layerPoints.m_asPropertiesField.Add( CString("OWNER") );
		layerPoints.m_asPropertiesField.Add( CString("ADDR") );

		layerPoints.m_asPropertiesField.Add( CString("column6") );//hotel point's stars
		layerPoints.m_asPropertiesField.Add( CString("LABEL1TQ") );//mulr second label part
		layerPoints.m_asPropertiesField.Add( CString("CITYNAME") );//city name
		layerPoints.m_asPropertiesField.Add( CString("STAGE") );//rtgauges name
		layerPoints.m_asPropertiesField.Add( CString("SHORTNAME") );//ptrim name
		layerPoints.m_asPropertiesField.Add( CString("FLOW") );//rtgauges name
		layerPoints.m_asPropertiesField.Add( CString("NAME1") );//samotrails name

		layerPoints.m_asPropertiesField.Add( CString("Offset") );//wm_Time_Zones name
		layerPoints.m_asPropertiesField.Add( CString("NAME") );//wm_Airports name
		layerPoints.m_asPropertiesField.Add( CString("LOCAL_NAME") );//wm_Capitals name
		layerPoints.m_asPropertiesField.Add( CString("Site") );//IDP
		layerPoints.m_asPropertiesField.Add( CString("ID") );//wm_Glacier name
		layerPoints.m_asPropertiesField.Add( CString("Name") );//wm_Great_lake name



		//Area
		layerArea.m_asPropertiesField.Add( CString("labeltq") );//name
		layerArea.m_asPropertiesField.Add( CString("LABELCQ") );
		layerArea.m_asPropertiesField.Add( CString("BORDERCQ") );
		layerArea.m_asPropertiesField.Add( CString("AREACQ") );
		layerArea.m_asPropertiesField.Add( CString("NAME") );//for new area name
		layerArea.m_asPropertiesField.Add( CString("TRACT") );//for bg name
		layerArea.m_asPropertiesField.Add( CString("ZIP") );//for zip area

		layerArea.m_asPropertiesField.Add( CString("Admin_Name") );//wm_Admin
		layerArea.m_asPropertiesField.Add( CString("ID") );//wm_Glacier
		layerArea.m_asPropertiesField.Add( CString("POLYGON_NM") );//polygon
		layerArea.m_asPropertiesField.Add( CString("Cntry_Name") );//wm_nations
		layerArea.m_asPropertiesField.Add( CString("PARCEL_ID") );//wm_Great_lake name




		if( hasFilledAreaReq )
		{
			for(int i = 0; i < (int) (shapeRecords.size()); i++)
			{
				ShapeRecords record = shapeRecords[i];

				CString sFile(record.getShpBaseName().c_str());
				dataSource.FromFile(sFile);
				dataSource.setShapeRecords(record);
				int type = record.getShpType();

				if( ShapeUtility::polygon == type )
				{
					if(record.hasTransInfo())
						layerArea.m_formatPaintTool.setTransInfo(record.getTransInfo());
					// set dbf attributes
					layerArea.LoadData(dataSource);
					//for MS coordinates convert
					transformer.Convert( &layerArea, nlevel, nMSOffsetX, nMSOffsetY );
					//holder
					//layerArea.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
					//setSM
					layerArea.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
					layerArea.m_formatPaintTool.setGIFOutput( hasGIFOutput );
					layerArea.m_formatPaintTool.setPNGOutput( hasPNGOutput );
					layerArea.m_formatPaintTool.setLatLong( hasLatLong );
					layerArea.m_formatPaintTool.setLabelDensity(nlabeldensity);
					//add by Felix
					layerArea.m_formatPaintTool.hasShadow = hasShadow;

					if(record.hasColorInfo())
					{
						layerArea.m_formatPaintTool.colorInfo = record.getColorInfo();
					}
					//set transformer
					//layerArea.m_formatPaintTool.SetTransformer( &transformer );

					//set highlightArea
					if( hasHighlightArea )
					{
						//layerArea.m_formatPaintTool.setHighlightAreaID( strHighlightAreaID, hasHighlightArea);
						layerArea.SetHighlightAreaValues( strHighlightAreaID, hasHighlightArea );
					}
					//set highlightCenter
					if( record.GetHighlightCenterInfo() )
					{
						layerArea.setHighlightBoundary( rcDestExt, record.GetHighlightCenterInfo() );
					}
					else
					{
						layerArea.HasHLCenter = false;
					}

					if( !layerArea.DrawShape(&canvas) )
					{
						continue;
					}
					//layerArea.DrawShape( &canvas );
				}
			}
		}

		////////////////////////////////////////////////////////////
		//customized layer should be here
		if( hasStrPoint )  //process the customised points
		{
			CString decodedStrPt = reqManager.InitRequest(strPoints);
			strPoints = decodedStrPt;

			CStringLayerDataSource strDataSource;
			CPointLayer strLayerPoints;
			strLayerPoints.m_asPropertiesField.Add( CString("name") );//name,do not use it

			strDataSource.FromString( strPoints );

			strLayerPoints.LoadData( strDataSource );

			//XML output
			//set XML output into formatPaintTool
			strLayerPoints.m_formatPaintTool.setXMLOutput( hasXMLOutput );
			strLayerPoints.m_formatPaintTool.setXMLUTM( hasXMLUTM );
			strLayerPoints.m_formatPaintTool.setLabelDensity(nlabeldensity);

			//GIF output
			strLayerPoints.m_formatPaintTool.setGIFOutput( hasGIFOutput );
			strLayerPoints.m_formatPaintTool.setPNGOutput( hasPNGOutput );
			
			//add by Felix
			strLayerPoints.m_formatPaintTool.hasShadow = hasShadow;

			transformer.Convert( &strLayerPoints );

			//set holder
			strLayerPoints.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
			//set transformer
			strLayerPoints.m_formatPaintTool.SetTransformer( &transformer );

			strLayerPoints.DrawShape( &canvas );
			strLayerPoints.DrawLabel( &canvas );	
		}
		/////////////////////////////////////////////////////////////

		//queue<CLineLayer> mergedStreets;
		// traverse all the layers in the vector
		for(int i = 0; i < (int) (shapeRecords.size()); i++)
		{
			ShapeRecords record = shapeRecords[i];

			CString sFile(record.getShpBaseName().c_str());
			dataSource.FromFile(sFile);
			dataSource.setShapeRecords(record);
			int type = record.getShpType();

			switch(type){
				case ShapeUtility::arc:	//line
					{
						//shapeLayer  = new CLineLayer();
						bool hasTransInfo = false;
						bool hasColorInfo = false;
						TransInfo info;

						if(record.hasTransInfo())
						{
							hasTransInfo = true;

							info = record.getTransInfo();
							layerStreet.m_formatPaintTool.transInfo = info;	

						}
						if(record.hasColorInfo())
						{
							hasColorInfo = true;
							//layerStreet.m_formatPaintTool.colorInfo = record.getColorInfo();
						}

						layerStreet.LoadData(dataSource);
						layerStreet.SetLabelFieldIndex(0); // set street name						
						logObj.writeToLog(_INFO, "Beging merge.");
						layerStreet.MergePartsTo(&layerStreetMerged);
						logObj.writeToLog(_INFO, "End merge.");
						//GIF output
						layerStreetMerged.m_formatPaintTool.setGIFOutput( hasGIFOutput );
						layerStreetMerged.m_formatPaintTool.setPNGOutput( hasPNGOutput );
						//Arc XML output
						//set XML output into formatPaintTool
						layerStreetMerged.m_formatPaintTool.setXMLOutput( hasXMLOutput );
						layerStreetMerged.m_formatPaintTool.setXMLUTM( hasXMLUTM );
						
						//add by Felix
						layerStreetMerged.m_formatPaintTool.hasShadow = hasShadow;

						transformer.Convert( &layerStreetMerged );

						//set holder
						layerStreetMerged.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
						//set transformer
					    layerStreetMerged.m_formatPaintTool.SetTransformer( &transformer );
						logObj.writeToLog(_INFO, "bEGINGing draw label.");
						//setSM
						layerStreetMerged.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );

						layerStreetMerged.m_formatPaintTool.setLineWidth( reqManager.GetLineWidth( info.values[0] ));

						//layerStreetMerged.m_formatPaintTool.setLabelDensity(nlabeldensity);

						if(hasTransInfo)
						{
							if(info.values[0] == 'l' || info.values[0] == 'L' ||
								info.values[0] == 'w' || info.values[0] == 'W'||
								info.values[0] == 1 || info.values[0] == 2 || info.values[0] == 3)
								layerStreetMerged.DrawShape( &canvas );

							//if(info.values[1] == 'n' || info.values[1] == 'N')
							//		layerStreetMerged.DrawLabel( &canvas );
						}
						else if(hasColorInfo)
						{
							layerStreetMerged.m_formatPaintTool.colorInfo = record.getColorInfo();
							layerStreetMerged.DrawShape( &canvas );
							//layerStreetMerged.DrawLabel( &canvas );
						}
						else if( hasXMLOutput )  //XML Output
						{
							layerStreetMerged.DrawShape( &canvas );
						}

						logObj.writeToLog(_INFO, "Ending draw label.");
						//layerStreetMerged.m_formatPaintTool.crLalelColor = ;
						break;

					}
				case ShapeUtility::point:	// point
					if( (record.getTransInfo().values[0] == 'l') || 
						(record.getTransInfo().values[0] == 'L') )
					{
						break;
					}

					char c = record.getTransInfo().values[0];

					//for FA_Owner
					if( (record.getTransInfo().values[0] == 'o') || 
						(record.getTransInfo().values[0] == 'O'))
					{
						layerPoints.m_formatPaintTool.iSpecifiedLableColumn = FA_OWNER;
					}
					//for FA_ADDR
					if((record.getTransInfo().values[0] == 'a') ||
						(record.getTransInfo().values[0] == 'A'))
					{
						layerPoints.m_formatPaintTool.iSpecifiedLableColumn = FA_ADDR;
					}

					// set dbf attributes
					layerPoints.LoadData(dataSource);
					//GIF output
					layerPoints.m_formatPaintTool.setGIFOutput( hasGIFOutput );
					layerPoints.m_formatPaintTool.setPNGOutput( hasPNGOutput );
					//picto XML output
					//set XML output into formatPaintTool
					layerPoints.m_formatPaintTool.setXMLOutput( hasXMLOutput );
					layerPoints.m_formatPaintTool.setXMLUTM( hasXMLUTM );

					layerPoints.m_formatPaintTool.setLabelDensity(nlabeldensity);
					
					//add by Felix
					layerPoints.m_formatPaintTool.hasShadow = hasShadow;

					//transformer.Convert( &layerPoints );
					//for MS coordinates convert
					transformer.Convert( &layerPoints, nlevel, nMSOffsetX, nMSOffsetY );
					//set holder
					layerPoints.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
					//set SM
					layerPoints.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
					//set transformer
					layerPoints.m_formatPaintTool.SetTransformer( &transformer );


					/*if((!record.hasTransInfo()) || (record.getTransInfo().values[0] == 'i') || 
						(record.getTransInfo().values[0] == 'I'))*/
					//{
					layerPoints.DrawShape( &canvas );
					//}
					//layerPoints.DrawShape( &canvas );
					break;
				//case ShapeUtility::polygon:	// area
				//	//shapeLayer  = new CPolygonLayer();
				//	if(record.hasTransInfo())
				//		layerArea.m_formatPaintTool.setTransInfo(record.getTransInfo());
				//	// set dbf attributes
				//	layerArea.LoadData(dataSource);
				//	transformer.Convert( &layerArea);
				//	//holder
				//	//layerArea.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
				//	//setSM
				//	layerArea.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
				//	//set transformer
				//	//layerArea.m_formatPaintTool.SetTransformer( &transformer );
				//	layerArea.DrawShape( &canvas );
			}		
		}

		CQixShapeFileLayerDataSource dataSource1;
		////////////////////////////////////
		dataSource1.SetExtend(rcSourceExt);
		////////////////////////////////////

		CLineLayer layerStreet1, layerStreetMerged1;
		CPointLayer layerPoints1;
		CPolygonLayer layerArea1;

		//Line
		//layerStreet1.m_asPropertiesField.Add( CString("streername") );//name
		////layerStreet1.m_asPropertiesField.Add( CString("ST_NAME") );//name
		//layerStreet1.m_asPropertiesField.Add( CString("LabelCQ") );//color of the label
		//layerStreet1.m_asPropertiesField.Add( CString("LineCQ") );//color of the line

		layerStreet1.m_asPropertiesField.Add( CString("HIGHWAY_NM") );//name
		layerStreet1.m_asPropertiesField.Add( CString("ST_NAME") );//name
		layerStreet1.m_asPropertiesField.Add( CString("streername") );//name
		layerStreet1.m_asPropertiesField.Add( CString("LabelCQ") );//color of the label
		layerStreet1.m_asPropertiesField.Add( CString("LineCQ") );//color of the line
		layerStreet1.m_asPropertiesField.Add( CString("NAME1") );//samotrails name
		layerStreet1.m_asPropertiesField.Add( CString("labeltq") );//new name

		layerStreet1.m_asPropertiesField.Add( CString("NAME") );//wm_Water_line
		layerStreet1.m_asPropertiesField.Add( CString("LOCAL_NUM") );//wm_Minor_routes
		layerStreet1.m_asPropertiesField.Add( CString("ID") );//wm_Railroad_track

		layerStreet1.m_asPropertiesField.Add( CString("POLYGON_NM") );
		layerStreet1.m_asPropertiesField.Add( CString("OBJECTID") );//fiber_optics
		//layerStreet.m_asPropertiesField.Add( CString("st_nm_base") );

		//Point
		layerPoints1.m_asPropertiesField.Add( CString("labeltq") );//name
		layerPoints1.m_asPropertiesField.Add( CString("LABELCQ") );
		layerPoints1.m_asPropertiesField.Add( CString("PictoFQ") );//picto
		layerPoints1.m_asPropertiesField.Add( CString("column1") );//another name
		layerPoints1.m_asPropertiesField.Add( CString("column10") );//dining&business point picto 
		layerPoints1.m_asPropertiesField.Add( CString("OWNER") );//wm_Great_lake name
		layerPoints1.m_asPropertiesField.Add( CString("ADDR") );//wm_Great_lake name
		layerPoints1.m_asPropertiesField.Add( CString("column6") );//hotel point stars picto
		layerPoints1.m_asPropertiesField.Add( CString("LABEL1TQ") );//mulr second label part
		layerPoints1.m_asPropertiesField.Add( CString("CITYNAME") );//city name
		layerPoints1.m_asPropertiesField.Add( CString("STAGE") );//rtgauges name
		layerPoints1.m_asPropertiesField.Add( CString("SHORTNAME") );//ptrim name
		layerPoints1.m_asPropertiesField.Add( CString("FLOW") );//rtgauges name
		layerPoints1.m_asPropertiesField.Add( CString("NAME1") );//samotrails name

		layerPoints1.m_asPropertiesField.Add( CString("Offset") );//wm_Time_Zones name
		layerPoints1.m_asPropertiesField.Add( CString("NAME") );//wm_Airports name
		layerPoints1.m_asPropertiesField.Add( CString("LOCAL_NAME") );//wm_Capitals name
		layerPoints1.m_asPropertiesField.Add( CString("Site") );//IDP
		layerPoints1.m_asPropertiesField.Add( CString("ID") );//wm_Glacier name
		layerPoints1.m_asPropertiesField.Add( CString("Name") );//wm_Great_lake name


		//Area
		layerArea1.m_asPropertiesField.Add( CString("labeltq") );//name
		layerArea1.m_asPropertiesField.Add( CString("LABELCQ") );
		layerArea1.m_asPropertiesField.Add( CString("BORDERCQ") );
		layerArea1.m_asPropertiesField.Add( CString("AREACQ") );
		layerArea1.m_asPropertiesField.Add( CString("NAME") );//for area name
		layerArea1.m_asPropertiesField.Add( CString("TRACT") );//for bg name
		layerArea1.m_asPropertiesField.Add( CString("ZIP") );//for zip area

		layerArea1.m_asPropertiesField.Add( CString("Admin_Name") );//wm_Admin
		layerArea1.m_asPropertiesField.Add( CString("ID") );//wm_Glacier

		layerArea1.m_asPropertiesField.Add( CString("POLYGON_NM") );//polygon
		layerArea1.m_asPropertiesField.Add( CString("Cntry_Name") );//wm_nations
		layerArea1.m_asPropertiesField.Add( CString("PARCEL_ID") );//wm_Great_lake name

		// traverse all the layers in the vector, draw labels
		for(int i = 0; i < (int) (shapeRecords.size()); i++)
		{
			ShapeRecords record = shapeRecords[i];
			CString sFile(record.getShpBaseName().c_str());	
			dataSource1.FromFile(sFile);
			dataSource1.setShapeRecords(record);
			int type = record.getShpType();

			switch(type){
				case ShapeUtility::arc:	//line
					{
						bool hasTransInfo = false;
						bool hasColorInfo = false;
						layerStreetMerged1.m_formatPaintTool.hasStreets = false;
						layerStreetMerged1.m_formatPaintTool.hasSechwys = false;
						layerStreetMerged1.m_formatPaintTool.hasMajHwys = false;
						layerStreetMerged1.m_formatPaintTool.hasMainroads = false;
						layerStreetMerged1.m_formatPaintTool.hasDetailroads = false;
						
						//add by Felix
						layerStreetMerged1.m_formatPaintTool.hasShadow = hasShadow;

						TransInfo info;
						if(record.hasTransInfo())
						{
							hasTransInfo = true;

							info = record.getTransInfo();
							layerStreet1.m_formatPaintTool.transInfo = info;	

						}
						if(record.hasColorInfo())
						{
							hasColorInfo = true;
							//layerStreet.m_formatPaintTool.colorInfo = record.getColorInfo();
						}
						layerStreet1.LoadData(dataSource1);
						layerStreet1.SetLabelFieldIndex(0); // set street name						
						//logObj.writeToLog(_INFO, "Beging merge.");
						layerStreet1.MergePartsTo(&layerStreetMerged1);
						//logObj.writeToLog(_INFO, "End merge.");
						transformer.Convert( &layerStreetMerged1 );
						//set holder
						layerStreetMerged1.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
						//setSM
						layerStreetMerged1.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
						//GIF output
						layerStreetMerged1.m_formatPaintTool.setGIFOutput( hasGIFOutput );
						layerStreetMerged1.m_formatPaintTool.setPNGOutput( hasPNGOutput );

						layerStreetMerged1.m_formatPaintTool.setLabelDensity(nlabeldensity);

						//set street type
						if( sFile.Find("streets") > 0 )
						{
							layerStreetMerged1.m_formatPaintTool.hasStreets = true;
						}
						else if( sFile.Find("Sechwys") > 0 )
						{
							layerStreetMerged1.m_formatPaintTool.hasSechwys = true;
						}
						else if( sFile.Find("MajHwys") > 0 )
						{
							layerStreetMerged1.m_formatPaintTool.hasMajHwys = true;
						}
						else if( sFile.Find("Mainroads") > 0)
						{
							layerStreetMerged1.m_formatPaintTool.hasMainroads = true;
						}
						else if( sFile.Find("detailroads") > 0)
						{
							layerStreetMerged1.m_formatPaintTool.hasDetailroads = true;
						}

						logObj.writeToLog(_INFO, "bEGINGing draw label.");

						layerStreetMerged1.m_formatPaintTool.SetTransformer( &transformer );

						if(hasTransInfo)
						{
							//if(info.values[0] == 'l' || info.values[0] == 'L')
							//		layerStreetMerged.DrawShape( &canvas );

							if(info.values[1] == 'n' || info.values[1] == 'N' ||
								info.values[1] == 'w' || info.values[1] == 'W')
								layerStreetMerged1.DrawLabel( &canvas );
						}
						else if(hasColorInfo)
						{
							layerStreetMerged1.m_formatPaintTool.colorInfo = record.getColorInfo();
							//layerStreetMerged.DrawShape( &canvas );
							layerStreetMerged1.DrawLabel( &canvas );
						}
						//else if( hasXMLOutput )  //XML Output
						//{
						//	//layerStreetMerged1.m_formatPaintTool.SetTransformer( &transformer );
						//	layerStreetMerged1.DrawShape( &canvas );
						//}
						logObj.writeToLog(_INFO, "Ending draw label.");
						//layerStreetMerged.m_formatPaintTool.crLalelColor = ;
						break;

					}
				case ShapeUtility::point:	// point

					if( (record.getTransInfo().values[0] == 'i') || 
						(record.getTransInfo().values[0] == 'I'))
					{
						break;
					}

					//for FA_Owner
					if( (record.getTransInfo().values[0] == 'o') || 
						(record.getTransInfo().values[0] == 'O'))
					{
						layerPoints1.m_formatPaintTool.iSpecifiedLableColumn = FA_OWNER;
					}
					//for FA_ADDR
					if( (record.getTransInfo().values[0] == 'a') || 
						(record.getTransInfo().values[0] == 'A'))
					{
						layerPoints1.m_formatPaintTool.iSpecifiedLableColumn = FA_ADDR;
					}

					// set dbf attributes
					layerPoints1.LoadData(dataSource1);
					//point XML output
					//set XML output into formatPaintTool
					layerPoints1.m_formatPaintTool.setXMLOutput( hasXMLOutput );
					layerPoints1.m_formatPaintTool.setXMLUTM( hasXMLUTM );
					//set GIF output
					layerPoints1.m_formatPaintTool.setGIFOutput( hasGIFOutput );
					layerPoints1.m_formatPaintTool.setPNGOutput( hasPNGOutput );
					//transformer.Convert( &layerPoints1 );
					//for MS coordinates convert
					transformer.Convert( &layerPoints1, nlevel, nMSOffsetX, nMSOffsetY );
					//set holder
					layerPoints1.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
					//set SM
					layerPoints1.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
                    //set transformer
					layerPoints1.m_formatPaintTool.SetTransformer( &transformer );

					layerPoints1.m_formatPaintTool.setLabelDensity(nlabeldensity);

					// add by Felix
					layerPoints1.m_formatPaintTool.hasShadow = hasShadow;

					/*if((!record.hasTransInfo()) || (record.getTransInfo().values[0] == 'l') || 
						(record.getTransInfo().values[0] == 'L'))*/
					//{
					layerPoints1.DrawLabel( &canvas );
					//}
					//layerPoints1.DrawLabel( &canvas );
					break;
				case ShapeUtility::polygon:	// area
					//shapeLayer  = new CPolygonLayer();
					if(record.hasTransInfo())
						layerArea1.m_formatPaintTool.setTransInfo(record.getTransInfo());
					//set dbf attributes
					layerArea1.LoadData( dataSource1 );
					//set GIF output
					layerArea1.m_formatPaintTool.setGIFOutput( hasGIFOutput );
                    //XML output
					layerArea1.m_formatPaintTool.setXMLOutput( hasXMLOutput );
					layerArea1.m_formatPaintTool.setXMLUTM( hasXMLUTM );

					layerArea1.m_formatPaintTool.setLabelDensity(nlabeldensity);
					
					//add by Felix
					layerArea1.m_formatPaintTool.hasShadow = hasShadow;

					//for MS coordinates convert
					transformer.Convert( &layerArea1, nlevel, nMSOffsetX, nMSOffsetY );
					//set holder
					layerArea1.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
					//setSM
					layerArea1.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
					layerArea1.m_formatPaintTool.SetTransformer( &transformer );
					layerArea1.DrawLabel( &canvas );
			}		
		}
		// destroy shape objects here, otherwise memory leak
		for(int i = 0; i < (int) (shapeRecords.size()); i++)
			shapeRecords[i].destroyRecords();

		logObj.writeToLog(_INFO, "begining transforming image.");

#ifdef TERRA_NODE

		if( hasPNGOutput )
		{
			canvas.setTransPNG( hasPNGOutput );
			canvas.SaveToPNGStream();
		}
		else if( hasXMLOutput )
		{
			canvas.SaveToXMLStream();
		}
		else if( hasGIFOutput )
		{
			canvas.setTransGIF( hasTransGIFOutput );
			canvas.SaveToGIFStream();
		}
		else
		{
			canvas.SaveToStream();
		}

#else


		if( hasPNGOutput )
		{
			CString sImageFileName = CString("..\\Output\\PNGMap")  + CString(".png");
			canvas.setTransPNG( hasPNGOutput );
			//canvas.SaveToPNGStream();
			canvas.SaveToPNGFile( sImageFileName );
		}
		else if( hasGIFOutput )
		{
			CString sImageFileName = CString("..\\Output\\GIFMap")  + CString(".gif");
			canvas.setTransGIF( hasTransGIFOutput );
			canvas.SaveToGIFFile( sImageFileName );
		}
		else if( hasXMLOutput && (!hasTxt) )
		{
			CString sImageFileName = CString("..\\Output\\XMLMap")  + CString(".xml");
			canvas.SaveToXMLFile( sImageFileName );
		}
		else if( hasTxt && hasXMLOutput )
		{
			CString sImageFileName = CString("G:\\Output\\TxtMap")  + CString(".txt");
			canvas.SaveToTxtFile( sImageFileName );
		}
		else
		{
			CString sImageFileName = CString("..\\Output\\JPGMap")  + CString(".jpg");
			//CString sImageFileName = CString("..\\Output\\JPGMap")  + CString(".png");
			LOG("Information: begin save image \n");
			canvas.SaveToFile( sImageFileName );
			LOG("Information: end save image \n");
			logObj.writeToLog(_INFO, "End processing.");
		}
		//_CrtDumpMemoryLeaks();

#endif

	}

	return nRetCode;
}