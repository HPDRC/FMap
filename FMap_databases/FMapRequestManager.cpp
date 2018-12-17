#include "FMapRequestManager.h"
#include "fmap.h"



CFMapRequestManager::CFMapRequestManager(void)
{
	isXML    = false;
	isUTM    = false;
	isKMLReq = false;
	isGMLReq = false;
	isAtlasReq = false;

	isKMLUTMReq   = false; 
	isGMLUTMReq   = false;
    isAtlasUTMReq = false;

	isTransGIF    = false;
	isGIF         = false;
	isTransPNG    = false;
	isPNG         = false;

	isHighlight   = false;

	isTxt         = false;
	isSourceDateReq = false;
	//isNorthernHemisphere = true;

	this->InitLevelResTabel();
}

CFMapRequestManager::~CFMapRequestManager(void)
{

}


//CFMapRequestManager::CalReqRect()
//{
//
//}

std::string CFMapRequestManager::InitFilledAreaReq(std::string strAreaReq, bool* pHasFilledArea)
{
	ASSERT(strAreaReq != "");

	Overlays reqOverlays(strAreaReq);

	std::string strOverlay = reqOverlays.getNextOverlay();
	std::string strFillArea;
	std::string strBlankLayer = "blanklayer";

	while(strOverlay != "")
	{

		int iDot = (int) (strOverlay.find('.'));

		if(iDot != std::string::npos)
		{
			std::string strAreaSuffix = strOverlay.substr(iDot+1);
			int nLASuffix = (int)(strAreaSuffix.size());

			if( FILLEDAREAREQ_LENGTH == nLASuffix )
			{
				strFillArea = strOverlay;
				int nFillAreaPos = (int)(strAreaReq.find(strFillArea));
				int nFillAreaLen = (int)(strFillArea.size());

				strAreaReq.replace(nFillAreaPos, nFillAreaLen, strBlankLayer);
				strAreaReq.insert(0,"+");
				strAreaReq.insert(0,strFillArea);

				*pHasFilledArea = true;
			}

		}

		strOverlay = reqOverlays.getNextOverlay();

	}

	return strAreaReq;
}



//std::string CFMapRequestManager::DoubleToString(double d)
//{
//	/*std::ostringstream ostr;
//	if (!(ostr << d)) 
//	{
//		return "ERROR";
//	}
//	return ostr.str();*/
//
//	char temp[30] = {0};
//	sprintf_s(temp, "%f", d);
//
//	return temp;
//
//}


double CFMapRequestManager::GetLatLongExt(double d, double doffset, bool isBigger)
{

	if(isBigger)
	{
		if( d >= 0 )
		{
			d = d*(1 + doffset);
		}
		else
		{
			d = d*(1 - doffset);
		}

	}
	else
	{
		if( d >= 0 )
		{
			d = d*(1 - doffset);
		}
		else
		{
			d = d*(1 + doffset);
		}

	}

	return d;

}




bool CFMapRequestManager::GetLatLongExtByPixel(double* dlong, double* dlat, int nlevel, bool isUpLeft, int nPixelOffset)
{
	int nPX = 0, nPY = 0;

	this->transformer.LatLongToPixelXY( *dlat, *dlong, nlevel, &nPX, &nPY, 0, 0 );

	if(isUpLeft)
	{

		nPX = nPX - nPixelOffset;
		nPY = nPY - nPixelOffset;

		if(nPX <= 0) 
		{
			nPX = 0;
		}
		
		if(nPY <= 0) 
		{
			nPY = 0;
		}

	}
	else
	{
		nPX = nPX + nPixelOffset;
		nPY = nPY + nPixelOffset;
	}

	this->transformer.PixelXYToLatLong(nPX, nPY, nlevel, *dlat, *dlong);

	return true;
}



bool CFMapRequestManager::HasFilledAreaReq(std::string strAreaReq, bool hasFilledAreaReq)
{
	ASSERT(strAreaReq != "");

	Overlays reqOverlays(strAreaReq);

	std::string strOverlay = reqOverlays.getNextOverlay();
	//std::string strFillArea;
	//std::string strBlankLayer = "blanklayer";

	while(strOverlay != "")
	{

		int iDot = (int) (strOverlay.find('.'));

		if(iDot != std::string::npos)
		{
			std::string strAreaSuffix = strOverlay.substr(iDot+1);
			int nLASuffix = (int)(strAreaSuffix.size());

			if( (FILLEDAREAREQ_LENGTH == nLASuffix) || (HIGHLIGHTCENTER_LENGTH == nLASuffix) ||
				(AREA_COLOR_LENGTH == nLASuffix)) 
			{
				//strFillArea = strOverlay;
				//int nFillAreaPos = (int)(strAreaReq.find(strFillArea));
				//int nFillAreaLen = (int)(strFillArea.size());

				//strAreaReq.replace(nFillAreaPos, nFillAreaLen, strBlankLayer);
				//strAreaReq.insert(0,"+");
				//strAreaReq.insert(0,strFillArea);

				hasFilledAreaReq = true;
			}

		}

		strOverlay = reqOverlays.getNextOverlay();

	}

	return hasFilledAreaReq;
}



bool CFMapRequestManager::HasXML(CString strTarImaType)
{
	if( ("KML" == strTarImaType.MakeUpper()) || 
		("KML_GDC" == strTarImaType.MakeUpper()) ||
		("GML" == strTarImaType.MakeUpper()) || 
		("GML_GDC" == strTarImaType.MakeUpper()) ||
        ("XML" == strTarImaType.MakeUpper()) || 
		("XML_GDC" == strTarImaType.MakeUpper()) ||
		("KML_UTM" == strTarImaType.MakeUpper()) || 
		("GML_UTM" == strTarImaType.MakeUpper()) ||
		("XML_UTM" == strTarImaType.MakeUpper()) ||
		("TXT"     == strTarImaType.MakeUpper()) )
	{
		isXML = true;
	}

	return isXML;
}

bool CFMapRequestManager::HasUTM(CString strTarImaType)
{
	if( ("KML_UTM" == strTarImaType.MakeUpper()) || 
		("GML_UTM" == strTarImaType.MakeUpper()) ||
		("XML_UTM" == strTarImaType.MakeUpper()) )
	{
		isUTM = true;
	}

	return isUTM;
}


bool CFMapRequestManager::HasKMLReq(CString strTarImaType)
{

	if( ("KML" == strTarImaType.MakeUpper()) || 
		("KML_GDC" == strTarImaType.MakeUpper()) )
	{
		isKMLReq = true;
	}

	return isKMLReq;

}

bool CFMapRequestManager::HasKMLUTMReq(CString strTarImaType)
{
	if( "KML_UTM" == strTarImaType.MakeUpper() )
	{
		isKMLUTMReq = true;
	}

	return isKMLUTMReq;
}

bool CFMapRequestManager::HasGMLReq(CString strTarImaType)
{
	if( ("GML" == strTarImaType.MakeUpper()) || 
		("GML_GDC" == strTarImaType.MakeUpper()) )
	{
		isGMLReq = true;
	}


	return isGMLReq;
}

bool CFMapRequestManager::HasGMLUTMReq(CString strTarImaType)
{
	if( "GML_UTM" == strTarImaType.MakeUpper() )
	{
		isGMLUTMReq = true;
	}

	return isGMLUTMReq;
}

bool CFMapRequestManager::HasAtlasReq(CString strTarImaType)
{
	if( ("XML" == strTarImaType.MakeUpper()) || 
		("XML_GDC" == strTarImaType.MakeUpper()) )
	{
		isAtlasReq = true;
	}


	return isAtlasReq;
}

bool CFMapRequestManager::HasAtlasUTMReq(CString strTarImaType)
{
	if( "XML_UTM" == strTarImaType.MakeUpper() )
	{
		isAtlasUTMReq = true;
	}

	return isAtlasUTMReq;
}


std::string CFMapRequestManager::RefactLayer( bool hasGIF, bool hasPNG, std::string strReq, bool *pHasTransCanvas, TransInfo *ptransInfo )
{
	//std::basic_string <char>::size_type all  = strReq.find ( "all" );
	//std::basic_string <char>::size_type ramb  = strReq.find ( "ramb" );

	std::string newall  = "";
	std::string newramb = "";
	std::string newseas = "";
	std::string newpoints = "";
	std::string info;

	std::string newsamotrails = "";
	std::string strBlankLayer = "blanklayer";
	std::string strnewsamotrails = "";

	Overlays overlays(strReq);
	std::string strOverlay = overlays.getNextOverlay();
	while(strOverlay != "")
	{
		int iDot = (int) (strOverlay.find('.'));
		if(iDot != std::string::npos)
		{
			//here just for seas layer, need the transinfo info from request
			info = strOverlay.substr(iDot+1);
			strOverlay  = strOverlay.substr(0, iDot);	
		}

		if( "all" == strOverlay )
		{
			//newall = "+gnis2+fire+schools+llfr+mulr+bwpoints+dos+incorpp+metrop+resr";
			newall = "+gnis2+fire+schools";
			strReq.append( newall.c_str() );
		}
		
		if( "ramb" == strOverlay )
		{
			newramb = "+comn+re1n+re2n+rinn+rldn+rntn+busn+cldn";
			strReq.append( newramb.c_str() );
		}

		/*if( "points2" == strOverlay )
		{
			newpoints = "+points";
			strReq.append( newpoints.c_str() );
		}*/

		if( ( "seas" == strOverlay ) && (!hasGIF) && (!hasPNG) )
		{
			*pHasTransCanvas = false;
			//here just for seas layer, need the transinfo info from request
			ptransInfo->type = TransInfo::area;
			ptransInfo->values[0] = ptransInfo->char2int(info[0]);
			ptransInfo->values[1] = ptransInfo->char2int(info[1]);
			ptransInfo->values[2] = ptransInfo->char2int(info[2]);
			//always set as sea.000, then replace seas to sea, in order to discard seas
			newseas = "+newsea.000";
			strReq.append( newseas.c_str() );
		}


		if( "samotrails" == strOverlay )
		{
			strnewsamotrails = strOverlay;
			int nsamotrailsPos = (int)(strReq.find(strnewsamotrails));
			int nsamotrailsLen = (int)(strnewsamotrails.size()) + 3;//lenth(".ln") = 3
			strnewsamotrails = strReq.substr(nsamotrailsPos, nsamotrailsLen);
			strReq.replace(nsamotrailsPos, nsamotrailsLen, strBlankLayer);
			newsamotrails = strnewsamotrails + "+";
			strReq.insert(0, newsamotrails);
		}
		
		strOverlay = overlays.getNextOverlay();
	}

	//std::string newall  = "";
	//std::string newramb = "";

	//if( (all != std::string::npos)||(all1 != std::string::npos) )
	//{
	//	//newall = "+gnis2+fire+schools+llfr+mulr+bwpoints+dos+incorpp+metrop+resr";
	//	newall = "+gnis2+fire+schools";
	//}
	//strReq.append( newall.c_str() );

	//if( (ramb != std::string::npos)||( ramb1 != std::string::npos) )
	//{
	//	newramb = "+comn+re1n+re2n+rinn+rldn+rntn+busn+cldn";
	//}
	//strReq.append( newramb.c_str() );

	return strReq;
}

CString CFMapRequestManager::InitRequest(CString strReq)
{
	strReq.Replace("%2C", ",");
	strReq.Replace("%2c", ",");
	strReq.Replace("%24", "$");
	strReq.Replace("%20", " ");
	strReq.Replace("%40", "@");
	strReq.Replace("%2f", "|");
	strReq.Replace("/"  , "|");
	strReq.Replace("%7c", "|");
	strReq.Replace("%7C", "|");
	strReq.Replace("%3A", ":");
	strReq.Replace("%3a", ":");


	return strReq;
}


bool CFMapRequestManager::HasTransGIF( CString strTarImaType )
{
	if( "GIF" == strTarImaType.MakeUpper() )
	{
		isTransGIF = true;
	}

	return isTransGIF;
}

bool CFMapRequestManager::hasGIFOutput( CString strTarImaType )
{
	if( ("GIF_OP" == strTarImaType.MakeUpper()) || 
		("GIF"    == strTarImaType.MakeUpper()) )
	{
		isGIF = true;
	}

	return isGIF;
}

bool CFMapRequestManager::HasTransPNG( CString strTarImaType )
{
	if( ("PNG" == strTarImaType.MakeUpper()) )
	{
		isTransPNG = true;
	}

	return isTransPNG;
}

bool CFMapRequestManager::hasPNGOutput( CString strTarImaType )
{
	if( ("PNG_OP" == strTarImaType.MakeUpper()) || 
		("PNG"    == strTarImaType.MakeUpper()) )
	{
		isPNG = true;
	}

	return isPNG;
}


bool CFMapRequestManager::makeResVisible(std::string overlay, std::pair<double, double> range, 
										 Overlays& overlays, double res)
{
	Rule reqRule( overlay, make_pair(range.first, range.second) );
	CompositeRule comRule;
	comRule.add( reqRule );
	comRule.apply( overlays, res );

	return true;
}


bool CFMapRequestManager::HasHighlight( CString strHigh )
{
	if( "" != strHigh )
	{
		isHighlight = true;
	}

	return isHighlight;

}

bool CFMapRequestManager::HasHighlightCenter( CString strHL )
{
	if( "CENTER" == strHL.MakeUpper() )
	{
		return true;
	}

	return false;
}

	
bool CFMapRequestManager::HasTxt( CString strTxt )
{
	if( "TXT" == strTxt.MakeUpper() )
	{
		isTxt = true;
	}

	return isTxt;

}

bool CFMapRequestManager::HasLatLongReq(std::string lat1, std::string long1, std::string lat2, std::string long2, std::string level)
{
	/*if( ("" != lat1) || ("" != long1) || 
		("" != lat2) || ("" != long2) )
	{
		return true;
	}
	else
	{
		return false;
	}*/


	if( ("" != lat1) && ("" != long1) && 
		("" != lat2) && ("" != long2) && ("" != level))
	{
		return true;
	}
	else
	{
		return false;
	}


}

bool CFMapRequestManager::HasQuadKey(CString quadkey)
{
	if(quadkey!="")
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool CFMapRequestManager::HasLatLongReqinUTM(std::string lat1, std::string long1, std::string lat2, std::string long2, std::string prj, std::string reqwidth, std::string reqheight)
{
	if( ("" != lat1) && ("" != long1) && 
		("" != lat2) && ("" != long2) && 
		("" != prj ) && ("" != reqwidth) && ("" != reqheight) )
	{
		return true;
	}
	else
	{
		return false;
	}
}






std::string CFMapRequestManager::GetLatLongFiles(std::string str_filename)
{
	if( "" == str_filename )
	{
		return str_filename;
	}

	string str_file = "C:\\degree\\latlong\\" + str_filename + ".qix";
	
	return str_file;

}


int CFMapRequestManager::GetLayerType(CString layer)
{

	if( "" == layer)
	{
		return -1;
	}

	int idot = layer.Find('.');
	if(-1 == idot)
	{
		return -1;
	}

	int nlength = layer.GetLength() - idot - 1;
	switch(nlength)
	{
	case AREA_COLOR_LENGTH:
		return ShapeUtility::polygon;
	case LINE_COLOR_LENGTH:
		return ShapeUtility::arc;
	case POINT_ICON_LENGTH:
		return ShapeUtility::point;

	//this is for the case without color info. in URL parameter
	case AREA_SOUCE_DATE:
		//this->isSourceDateReq = true;
		return ShapeUtility::polygon;
	default:
		return -1;
	}


}

int CFMapRequestManager::GetLineWidth( CString layer )
{
	int nwidth = 0;

	if( "" == layer)
	{
		return -1;
	}

	int idot = layer.Find('.');
	if(-1 == idot)
	{
		return -1;
	}


	int nlinewidth_2  = 2,
		nlinewidth_6  = 6,
		nlinewidth_10 = 10;


	if( '1' == layer.GetAt(idot+1) )
	{
		nwidth = nlinewidth_2;
	}
	else if( '2' == layer.GetAt(idot+1) )
	{
		nwidth = nlinewidth_6;
	}
	else if( '3' == layer.GetAt(idot+1) )
	{
		nwidth = nlinewidth_10;
	}
	else
	{
		nwidth = nlinewidth_2;
	}

	return nwidth;
}

CString CFMapRequestManager::GetTableName(CString name)
{
	if( "" == name )
	{
		return "";
	}

	CString tablename;

	//for hyphen syntax
	int iHyphen = name.Find('-');
	if(-1 != iHyphen)
	{
		return tablename = name.Left(iHyphen);
	}

	int idot = name.Find('.');
	if(-1 == idot)
	{
		return name;
	}

	tablename = name.Left(idot);

	//for soar
	if( (tablename.MakeUpper() == SOURCE_AIRS) || 
		(tablename.MakeUpper() == SOURCE_MODIS)  )
	{
		tablename = "SOAR_test";
	}

	return tablename;
}

bool CFMapRequestManager::HasAreaSourceDateReq(CString layername)
{
	if( "" == layername )
	{
		return false;
	}

	int idot = layername.Find('.');
	if(-1 == idot)
	{
		return false;
	}

	int nlength = layername.GetLength() - idot - 1;
	if( nlength == AREA_SOUCE_DATE )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CFMapRequestManager::GetDateAndSource(CString layername, CString *source, CString *date)
{

	if( "" == layername )
	{
		return false;
	}

	int idot = layername.Find('.');
	if(-1 == idot)
	{
		return false;
	}

	int nlength = layername.GetLength() - idot - 1;
	if( nlength != AREA_SOUCE_DATE )
	{
		return false;
	}

	*source = layername.Left(idot);
	CString strdate = layername.Right(nlength);
	//composing 20070505 to 2007-05-05
	strdate.Insert(4, '-');
	strdate.Insert(7, '-');
	*date = strdate;

	return true;
}


CString CFMapRequestManager::GetFieldName(CString str, CString tablename)
{
	logObj.writeToLog(_INFO, (char*)str.GetString());

	int icurPos = 0, ifield = 0;
	CString resToken, str_fieldname;
	str_fieldname = "";

	tablename = tablename.MakeUpper();
	resToken = str.Tokenize(";",icurPos);


	while (resToken != "")
	{
		resToken = resToken.MakeUpper();
		ifield = resToken.Find(tablename);
		if( -1 != ifield )
		{
			str_fieldname = resToken.Right( resToken.GetLength() - tablename.GetLength() - 1);
			return str_fieldname;
		}

		resToken = str.Tokenize(";",icurPos);
	}

	logObj.writeToLog(_INFO, (char*)str_fieldname.GetString());

	if( "" == str_fieldname )
	{
		str_fieldname = "labelname";
	}

	return str_fieldname;

}

//bool CFMapRequestManager::GetPolygonColor(OverridedColor* color, CString str, CMapStringToPtr* color_table)
bool CFMapRequestManager::GetPolygonColor(OverridedColor* color, CString str, CMapColorTable* color_table)
{

	
	if( "" == str )
	{
		color->line = "";
		color->area = "";
		color->label = "";
		return false;
	}

	int idot = str.Find('.');
	if(-1 == idot)
	{
		color->line = "";
		color->area = "";
		color->label = "";
		return false;
	}

	str = str.Right(str.GetLength() - idot -1);
	/*if( AREA_COLOR_LENGTH != str.GetLength() )
	{
		color->line = "";
		color->area = "";
		color->label = "";
		return false;
	}*/

	if( AREA_COLOR_LENGTH == str.GetLength() ||
		(AREA_COLOR_LENGTH+1) == str.GetLength() )
	{
		Gdiplus::Color pixel;
		CColorManager colormanager;

		color->line  = str.Mid(3,8).GetString();
		pixel = pixel.MakeARGB(
			(BYTE) strtol(color->line.substr(0,2).c_str(), NULL, 16),
			(BYTE) strtol(color->line.substr(2,2).c_str(), NULL, 16), 
			(BYTE) strtol(color->line.substr(4,2).c_str(), NULL, 16), 
			(BYTE) strtol(color->line.substr(6,2).c_str(), NULL, 16)
			);

		color_table->SetAt(colormanager.GetPNGColorTableIndex(pixel), pixel);
		//save color only, ignore the alpha
		//color_table->SetAt(str.Mid(5,6).MakeUpper(), pixel);


		color->area  = str.Mid(11,8).GetString();
		pixel = pixel.MakeARGB(
			(BYTE) strtol(color->area.substr(0,2).c_str(), NULL, 16),
			(BYTE) strtol(color->area.substr(2,2).c_str(), NULL, 16), 
			(BYTE) strtol(color->area.substr(4,2).c_str(), NULL, 16), 
			(BYTE) strtol(color->area.substr(6,2).c_str(), NULL, 16)
			);
		
		color_table->SetAt(colormanager.GetPNGColorTableIndex(pixel), pixel);
		//save color only, ignore the alpha
		//color_table->SetAt(str.Mid(13,6).MakeUpper(), pixel);

		color->label = str.Mid(19,8).GetString();
		pixel = pixel.MakeARGB(
			(BYTE) strtol(color->label.substr(0,2).c_str(), NULL, 16),
			(BYTE) strtol(color->label.substr(2,2).c_str(), NULL, 16), 
			(BYTE) strtol(color->label.substr(4,2).c_str(), NULL, 16), 
			(BYTE) strtol(color->label.substr(6,2).c_str(), NULL, 16)
			);
		
		color_table->SetAt(colormanager.GetPNGColorTableIndex(pixel), pixel);
		//save color only, ignore the alpha
		//color_table->SetAt(str.Mid(21,6).MakeUpper(), pixel);

	}
	else
	{
		color->line  = "";
		color->area  = "";
		color->label = "";
		return false;
	}

	return true;

}


bool CFMapRequestManager::GetLineStringColor(OverridedColor* color, CString str, CMapColorTable* color_table)
{
	if( "" == str )
	{
		color->line = "";
		color->area = "";
		color->label = "";
		return false;
	}

	int idot = str.Find('.');
	if(-1 == idot)
	{
		color->line = "";
		color->area = "";
		color->label = "";
		return false;
	}

	str = str.Right(str.GetLength() - idot -1);
	/*if( AREA_COLOR_LENGTH != str.GetLength() )
	{
		color->line = "";
		color->area = "";
		color->label = "";
		return false;
	}*/

	if( LINE_COLOR_LENGTH == str.GetLength() )
	{
		Gdiplus::Color pixel;
		CColorManager colormanager;

		color->line  = str.Mid(2,8).GetString();

		pixel = pixel.MakeARGB(
			(BYTE) strtol(color->line.substr(0,2).c_str(), NULL, 16),
			(BYTE) strtol(color->line.substr(2,2).c_str(), NULL, 16), 
			(BYTE) strtol(color->line.substr(4,2).c_str(), NULL, 16), 
			(BYTE) strtol(color->line.substr(6,2).c_str(), NULL, 16)
			);

		color_table->SetAt(colormanager.GetPNGColorTableIndex(pixel), pixel);

		color->area  = "";
		color->label = "";
	}
	else
	{
		color->line  = "";
		color->area  = "";
		color->label = "";
		return false;
	}

	return true;
}


bool GetPolygonLabelSize(int* pnsize, CString str)
{
	int idot = str.Find('.');
	if(-1 == idot)
	{
		return false;
	}
	str = str.Right(str.GetLength() - idot -1);

	if( (AREA_COLOR_LENGTH+1) == str.GetLength() )
	{
		*pnsize = atoi(str.Right(1).GetString());	 
	}
	else
	{
		return false;
	}


	return true;
}


bool CFMapRequestManager::HasPointLable(CString str)
{
	if( "" == str )
	{
		return false;
	}

	int idot = str.Find('.');
	if(-1 == idot)
	{
		return false;
	}

	if( ('l' == str.GetAt(idot+1)) || ('L' == str.GetAt(idot+1)) || 
		('f' == str.GetAt(idot+1)) || ('F' == str.GetAt(idot+1)) )
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool CFMapRequestManager::GetPointFontSize(CString str, int* pnheight, int* pnwidth)
{
	if( "" == str )
	{
		return false;
	}

	int idot = str.Find('.');
	if(-1 == idot)
	{
		return false;
	}

	int nheight_14 = 14, 
		nheight_12 = 12,
		nheight_11 = 11,
		nheight_9  = 9,
		nwidth_6   = 6,
		nwidth_8   = 8;


	if( ('l' == str.GetAt(idot+1)) || ('L' == str.GetAt(idot+1)) || 
		('f' == str.GetAt(idot+1)) || ('F' == str.GetAt(idot+1)) )
	{
		char size = str.GetAt(idot+2);
		switch(size)
		{
			/*case '1':
			*pnheight = 14;
			*pnwidth  = 8;
			return true;
			case '2':
			*pnheight = 12;
			*pnwidth  = 8;
			return true;
			case '3':
			*pnheight = 11;
			*pnwidth  = 6;
			return true;
			case '4':
			*pnheight = 9;
			*pnwidth  = 6;
			return true;*/

		case '1':
			*pnheight = nheight_14;
			*pnwidth  = nwidth_8;
			return true;
		case '2':
			*pnheight = nheight_12;
			*pnwidth  = nwidth_8;
			return true;
		case '3':
			*pnheight = nheight_11;
			*pnwidth  = nwidth_6;
			return true;
		case '4':
			*pnheight = nheight_9;
			*pnwidth  = nwidth_6;
			return true;

		default:
			return true;
		}
	}
	else
	{
		*pnheight = nheight_12;
		*pnwidth  = nwidth_8;
		return false;
	}

}

bool CFMapRequestManager::GetPointIconSize(CString str, int* pniconsize)
{
	if( "" == str )
	{
		return false;
	}

	int idot = str.Find('.');
	if(-1 == idot)
	{
		return false;
	}

	int niconsize_6 = 6, 
		niconsize_5 = 5,
		niconsize_4 = 4,
		niconsize_default = 3;

	if( ('i' == str.GetAt(idot+1)) || ('I' == str.GetAt(idot+1)) ||
		('f' == str.GetAt(idot+1)) || ('F' == str.GetAt(idot+1)))
	{
		char size = str.GetAt(idot+2);
		switch(size)
		{
			case '1':
				*pniconsize = niconsize_6;
				return true;
			case '2':
				*pniconsize = niconsize_5;
				return true;
			case '3':
				*pniconsize = niconsize_4;
				return true;
			case '4':
				*pniconsize = niconsize_default;
				return true;
			default:
				return true;
		}
	}
	else
	{
		*pniconsize = niconsize_default;
		return false;
	}

}


bool CFMapRequestManager::HasPointIcon(CString str)
{
	if( "" == str )
	{
		return false;
	}

	int idot = str.Find('.');
	if(-1 == idot)
	{
		return false;
	}

	if( ('i' == str.GetAt(idot+1)) || ('I' == str.GetAt(idot+1)) ||
		('f' == str.GetAt(idot+1)) || ('F' == str.GetAt(idot+1)))
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool CFMapRequestManager::HasLineStringLable(CString str)
{
	if( "" == str )
	{
		return false;
	}

	int idot = str.Find('.');
	if(-1 == idot)
	{

		return false;
	}

	if( ('n' == str.GetAt(idot+2)) || ('N' == str.GetAt(idot+2)) )
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool CFMapRequestManager::HasLineStringShape(CString str)
{
	if( "" == str )
	{
		return false;
	}

	int idot = str.Find('.');
	if(-1 == idot)
	{

		return false;
	}

	if( ('l' == str.GetAt(idot+1)) || ('L' == str.GetAt(idot+1)) ||  
		('1' == str.GetAt(idot+1)) || ('2' == str.GetAt(idot+1)) ||
		('3' == str.GetAt(idot+1)) )
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool CFMapRequestManager::HasPolygonDraw(CString str)
{
	if( "" == str )
	{
		return false;
	}

	int idot = str.Find('.');
	if(-1 == idot)
	{

		return false;
	}

	if( SOURCE_AIRS  == str.Left(idot) || 
		SOURCE_MODIS == str.Left(idot) )
	{
		return true;
	}
	//else if( ('0'== str.GetAt(idot+2)) && ('0' != str.GetAt(idot+1)) )

    else if( ('0'==str.GetAt(idot+2))&&
		('0'==str.GetAt(idot+1)) )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CFMapRequestManager::HasPolygonFill(CString str)
{
	if( "" == str )
	{
		return false;
	}

	int idot = str.Find('.');
	if(-1 == idot)
	{

		return false;
	}

	if( SOURCE_AIRS  == str.Left(idot) || 
		SOURCE_MODIS == str.Left(idot) )
	{
		return true;
	}
	//else if( ('0'== str.GetAt(idot+2)) && ('0' != str.GetAt(idot+1)) )

    else if( '0'== str.GetAt(idot+2) )
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool CFMapRequestManager::HasPolygonBoundary(CString str)
{
	if( "" == str )
	{
		return false;
	}

	int idot = str.Find('.');
	if(-1 == idot)
	{

		return false;
	}

	if( '0' == str.GetAt(idot+1) )
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool CFMapRequestManager::HasPolygonLable(CString str)
{
	if( "" == str )
	{
		return false;
	}

	int idot = str.Find('.');
	if(-1 == idot)
	{

		return false;
	}

	if( '0' == str.GetAt(idot+3) )
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool CFMapRequestManager::HasLODData(int ndatatype)
{

	if( (ndatatype == ShapeUtility::polygon)||
		(ndatatype == ShapeUtility::arc) )
	{
		return true;
	}
	else if( ndatatype == ShapeUtility::point )
	{
		return false;
	}
	else
	{
		//should be return log error
		return false;
	}

}

bool CFMapRequestManager::HasNorthernHemisphere(double lat1)
{
	if( lat1 >= 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool CFMapRequestManager::HasNewResSyntax(std::string overlays, bool* isLevelResSyntax)
{
	char hyphen = '-';
	int nlength = overlays.length();
	std::string layername = overlays;

	int ifirstPos = overlays.find(hyphen);
	if(ifirstPos != string::npos)
	{
		//overlays = overlays.substr(ifirstPos+1, nlength-ifirstPos);
		overlays = overlays.substr(ifirstPos+1);
		int isecondPos = overlays.find(hyphen);
		if(isecondPos != string::npos)
		{
			overlays = overlays.substr(isecondPos+1);
			int ithirdPos = overlays.find(hyphen);
			if(ithirdPos != string::npos)
			{
				if( ('l'==layername.at(ifirstPos+1))||
					('L'==layername.at(ifirstPos+1)) )
				{
					*isLevelResSyntax = true;
				}

				return true;
			}
		}
	}

	return false;

}

bool CFMapRequestManager::IsResVisible(std::string *overlays, double curres)
{	
	//before doing this function, must know this is a new syntax
	//this function doesn't check if has 3 hyphens
	char hyphen = '-';
	int nlength = overlays->length();

	int ifirstPos = overlays->find(hyphen);
	int ithirdPos = overlays->find_last_of(hyphen);
	if((ifirstPos != string::npos)
		&&(ithirdPos != string::npos)
		&&(ithirdPos>=ifirstPos))
	{
		std::string visible = overlays->substr(ifirstPos+1, ithirdPos-ifirstPos-1);

		/*std::string str1 = overlays->substr(0, ifirstPos);
		std::string str2 = overlays->substr(ithirdPos+1, nlength - ithirdPos);
		*overlays = str1.append(str2);*/

		*overlays = overlays->substr(0, ifirstPos);

		int isecondPos = visible.find(hyphen);
		if(isecondPos != string::npos)
		{
			pair<double, double> range;
			std::string first  = visible.substr(0, isecondPos);
			std::string second = visible.substr(isecondPos+1, nlength-isecondPos);
			range.first = atof(first.c_str());
			range.second = atof(second.c_str());
			if( (curres >= range.first) && (curres <= range.second) )
			{
				return true;
			}

		}
	}

	return false;

}

bool CFMapRequestManager::IsResVisible(CString *layername, double curres)
{
	//before doing this function, must know this is a new syntax
	//this function doesn't check if has 3 hyphens
	std::string overlays = (*layername).GetString(); 
	char hyphen = '-';
	int nlength = overlays.length();

	int ifirstPos = overlays.find(hyphen);
	int ithirdPos = overlays.find_last_of(hyphen);
	if((ifirstPos != string::npos)
		&&(ithirdPos != string::npos)
		&&(ithirdPos>=ifirstPos))
	{
		std::string visible = overlays.substr(ifirstPos+1, ithirdPos-ifirstPos-1);

		/*std::string str1 = overlays->substr(0, ifirstPos);
		std::string str2 = overlays->substr(ithirdPos+1, nlength - ithirdPos);
		*overlays = str1.append(str2);*/

		//*layername = overlays.substr(0, ifirstPos).c_str();
		std::string str1 = overlays.substr(0, ifirstPos);
		std::string str2 = overlays.substr(ithirdPos+1, nlength - ithirdPos);
		*layername = str1.append(str2).c_str();

		int isecondPos = visible.find(hyphen);
		if(isecondPos != string::npos)
		{
			pair<double, double> range;
			std::string first  = visible.substr(0, isecondPos);
			std::string second = visible.substr(isecondPos+1, nlength-isecondPos);
			range.first = atof(first.c_str());
			range.second = atof(second.c_str());
			if( (curres >= range.first) && (curres <= range.second) )
			{
				return true;
			}

		}
	}

	return false;
}

bool CFMapRequestManager::IsResVisible(CString *layername, int level)
{
	//before doing this function, must know this is a new syntax
	//this function doesn't check if has 3 hyphens
	std::string overlays = (*layername).GetString(); 
	char hyphen = '-';
	int nlength = overlays.length();

	int ifirstPos = overlays.find(hyphen);
	int ithirdPos = overlays.find_last_of(hyphen);
	if((ifirstPos != string::npos)
		&&(ithirdPos != string::npos)
		&&(ithirdPos>=ifirstPos))
	{
		std::string visible = overlays.substr(ifirstPos+1, ithirdPos-ifirstPos-1);

		std::string str1 = overlays.substr(0, ifirstPos);
		std::string str2 = overlays.substr(ithirdPos+1, nlength - ithirdPos);
		*layername = str1.append(str2).c_str();

		//*layername = overlays.substr(0, ifirstPos).c_str();

		int isecondPos = visible.find(hyphen);
		if(isecondPos != string::npos)
		{
			pair<int, int> range;
			std::string first  = visible.substr(1, isecondPos - 1);
			std::string second = visible.substr(isecondPos+2, nlength-isecondPos-1);
			range.first = atoi(first.c_str());
			range.second = atoi(second.c_str());
			if( (level >= range.first) && (level <= range.second) )
			{
				return true;
			}

		}
	}

	return false;
}


void CFMapRequestManager::InitLevelResTabel(void)
{
	//considerting the precision and future modification for this number, 
	//not using "for" to loop this to initialize this
	this->levelToResTable[1]=78271.517;
	this->levelToResTable[2]=39135.758;
	this->levelToResTable[3]=19567.879;
	this->levelToResTable[4]=9783.939;
	this->levelToResTable[5]=4891.969;
	this->levelToResTable[6]=2445.984;
	this->levelToResTable[7]=1222.992;
	this->levelToResTable[8]=611.496;
	this->levelToResTable[9]=305.748;
	this->levelToResTable[10]=152.874;
	this->levelToResTable[11]=76.8;
	this->levelToResTable[12]=38.4;
	this->levelToResTable[13]=19.2;
	this->levelToResTable[14]=9.6;
	this->levelToResTable[15]=4.8;
	this->levelToResTable[16]=2.4;
	this->levelToResTable[17]=1.2;
	this->levelToResTable[18]=0.6;
	this->levelToResTable[19]=0.3;
	this->levelToResTable[20]=0.15;
	this->levelToResTable[21]=0.075;

	this->resToLevelTable[78271.517]=1;
	this->resToLevelTable[39135.758]=2;
	this->resToLevelTable[19567.879]=3;
	this->resToLevelTable[9783.939]=4;
	this->resToLevelTable[4891.969]=5;
	this->resToLevelTable[2445.984]=6;
	this->resToLevelTable[1222.992]=7;
	this->resToLevelTable[611.496]=8;
	this->resToLevelTable[305.748]=9;
	this->resToLevelTable[152.874]=10;
	this->resToLevelTable[76.8]=11;
	this->resToLevelTable[38.4]=12;
	this->resToLevelTable[19.2]=13;
	this->resToLevelTable[9.6]=14;
	this->resToLevelTable[4.8]=15;
	this->resToLevelTable[2.4]=16;
	this->resToLevelTable[1.2]=17;
	this->resToLevelTable[0.6]=18;
	this->resToLevelTable[0.3]=19;
	this->resToLevelTable[0.15]=20;
	this->resToLevelTable[0.075]=21;

}

int CFMapRequestManager::GetLevelFromRes(double& res)
{
	return this->resToLevelTable.find(res)->second;
}

double CFMapRequestManager::GetResFromLevel(int& level)
{
	return this->levelToResTable.find(level)->second;
}



bool CFMapRequestManager::GetLevelFromURL(CString& quadkey, int& nlevel, std::string& str_level )
{

	if(quadkey!="")
	{
		nlevel = quadkey.GetLength();
		char buffer[21];  
		itoa(nlevel, buffer, 10);
		str_level = *buffer;
	}
	else if(str_level!="")
	{
		//str_level = str_level;
		nlevel = atoi(str_level.c_str());
	}
	else
	{
		return false;
	}

	return true;
}

bool CFMapRequestManager::GetWidthHeight(const bool& hasLatLong, const bool& hasQuadKey, 
	const std::string& XN, const std::string& YN, int& nWidth, int& nHeight, 
	const double& x1, const double& x2, const double& y1, const double& y2, const double& res)
{
	if( hasLatLong || hasQuadKey )  //Lat, Long
	{
		nWidth  = 256;
		nHeight = 256;
		/*if(nReqWidth != 0)
		{
		nWidth  = nReqWidth;
		nHeight = nReqHeight;
		}*/
	}


	///////////////////////////////////////////////////////////////////////////
	//UTM

	else if( XN != "" || YN != "" )	// BT
	{
		nWidth  = 400;
		nHeight = 400;
	}
	//else if( hasLatLonginUTM )//lat long in UTM
	//{
	//	//int nReqWidth, nReqHeight;

	//	//sscanf(str_reqwidth.c_str(), "%d", &nReqWidth);
	//	//sscanf(str_reqheight.c_str(), "%d", &nReqHeight);
	//	nWidth  = nReqWidth;
	//    nHeight = nReqHeight;
	//}
	else // DT
	{	
		nWidth  = (int) ((x2-x1)/res);
		nHeight = (int) ((y1-y2)/res);
	}

	return true;
}

bool CFMapRequestManager::GetLatLongFromX1Y1X2Y2(const std::string& X1, const std::string& X2,
	const std::string& Y1, const std::string& Y2, 
	const std::string& UTMZ1, std::string& UTMZ2, 
	Gdc_Coord_3d& gdc1, Gdc_Coord_3d gdc2)
{
	 /*X1 = requestString.getValue("X1");
	 X2 = requestString.getValue("X2");
	 Y1 = requestString.getValue("Y1");
	 Y2 = requestString.getValue("Y2");*/

	if(X1 == "" || X2 == "" || Y1 == "" || Y2 == "")
	{
		CGIReply::replyText("Error: XN YN or X1 Y1 parameter not specified!");
		exit(1);
	}

	double x1,x2,y1,y2;
	int utmz, utmz1, utmz2;

	/*string UTMZ1 = requestString.getValue("UTMZ1");
	string UTMZ2 = requestString.getValue("UTMZ2");*/
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

	Utm_Coord_3d utmCoord1(x1, y1, 0, utmz1, true/*not sure*/);
	Utm_Coord_3d utmCoord2(x2, y2, 0, utmz2, true);

	Utm_To_Gdc_Converter::Init();
	Utm_To_Gdc_Converter::Convert(utmCoord1, gdc1);
	Utm_To_Gdc_Converter::Convert(utmCoord2, gdc2);
	return true;
}

bool CFMapRequestManager::GetLatLongFromXNYN(const std::string& XN, const std::string& YN, 
	const std::string& UTMZ, const double& res,  
	Gdc_Coord_3d& gdc1, Gdc_Coord_3d gdc2)
{
	if(UTMZ == "")
	{
		CGIReply::replyText("Error: UTMZ parameter not specified!");
		exit(1);
	}

	int xn, yn, utmz;
	double x1, y1, x2, y2;

	sscanf(XN.c_str(), "%d", &xn);
	sscanf(YN.c_str(), "%d", &yn);
	sscanf(UTMZ.c_str(), "%d", &utmz);

	x1 = xn*TILE_SIZE*res;	// 400 is the tile size
	x2 = (xn+1)*TILE_SIZE*res;
	y1 = yn*TILE_SIZE*res;
	y2 = (yn-1)*TILE_SIZE*res;

	Utm_Coord_3d utmCoord1(x1, y1, 0, utmz, true/*not sure*/);
	Utm_Coord_3d utmCoord2(x2, y2, 0, utmz, true);

	Utm_To_Gdc_Converter::Init();
	Utm_To_Gdc_Converter::Convert(utmCoord1, gdc1);
	Utm_To_Gdc_Converter::Convert(utmCoord2, gdc2);
}



bool CFMapRequestManager::GetLatLongFromLatLong(const std::string& str_lat1, const std::string& str_lat2,  
	const std::string& str_long1, const std::string& str_long2,  
	Gdc_Coord_3d& gdc1, Gdc_Coord_3d& gdc2)
{
	double	dLat1  = atof(str_lat1.c_str());
	double	dLat2  = atof(str_lat2.c_str());
	double	dLong1 = atof(str_long1.c_str());
	double	dLong2 = atof(str_long2.c_str());

	gdc1.longitude = dLong1;
	gdc2.latitude = dLat2;
	gdc2.longitude = dLong2;
	gdc1.latitude = dLat1;

	return true;
}

bool CFMapRequestManager::GetLatLongFromQuadKey(const CString& str_quadkey, /*int& nlevel,*/ 
	const CCoordinateTransformer& transformer,
		Gdc_Coord_3d& gdc1, Gdc_Coord_3d& gdc2)
{
	int nlevel=0, tileX=0, tileY=0, pixelX1=0, pixelY1=0, pixelX2=0, pixelY2=0;
		double dLong1=0.0, dLong2=0.0, dLat1=0.0, dLat2=0.0;

	transformer.m_pLinearTransformer->QuadKeyToTileXY(str_quadkey, tileX, tileY, nlevel);
	transformer.m_pLinearTransformer->TileXYToPixelXY(tileX, tileY, pixelX1, pixelY1);
	pixelX2 = pixelX1 + 256;
	pixelY2 = pixelY1 + 256;
	transformer.m_pLinearTransformer->PixelXYToLatLong(pixelX1, pixelY1, nlevel, dLat1, dLong1);
	transformer.m_pLinearTransformer->PixelXYToLatLong(pixelX2, pixelY2, nlevel, dLat2, dLong2);

	gdc1.longitude = dLong1;
	gdc2.latitude  = dLat2;
	gdc2.longitude = dLong2;
	gdc1.latitude  = dLat1;

	return true;
}


int CFMapRequestManager::GetLayersNum(std::string& overlays)
{
	if(overlays.length() == 0)
	{
		return 0;
	}

	int length = overlays.length(), start = 0, count = 1;
	start = overlays.find("+", 0);
	while((start<length)&&(-1!=start))
	{
		count++;
		//start = start+1;
		start = overlays.find("+", ++start);
	}

	return count;

}


bool CFMapRequestManager::GetQuadKeyFromLatLong(const CDoubleRect& rcSourceExt, 
	const CCoordinateTransformer& transformer, 
		CString& str_quadkey, const int& nlevel, int& nMSOffsetX, int& nMSOffsetY)
{
	int nPX1 = 0, nPX2 =0, nPY1 = 0, nPY2 = 0;
	int nTileX1 = 0, nTileY1 = 0, nTileX2 = 0, nTileY2 = 0;
	//int nMSOffsetX = 0, nMSOffsetY = 0;

	transformer.m_pLinearTransformer->LatLongToPixelXY( 
		rcSourceExt.dY1, rcSourceExt.dX1, nlevel, &nPX1, &nPY1, 0, 0 );
	transformer.m_pLinearTransformer->LatLongToPixelXY( 
		rcSourceExt.dY2, rcSourceExt.dX2, nlevel, &nPX2, &nPY2, 0, 0 );
	nMSOffsetX = nPX1;
	nMSOffsetY = nPY1;

	//in tile system, left-top is start, right-buttom is biggest one
	//so here should be left-top point, nPX1, nPY1
	//if( reqManager.HasNorthernHemisphere(rcSourceExt.dY2) )
	//{
	transformer.m_pLinearTransformer->PixelXYToTileXY(nPX1, nPY1, &nTileX1, &nTileY1);
	//}
	/*else
	{
	transformer.m_pLinearTransformer->PixelXYToTileXY(nPX1, nPY2, &nTileX1, &nTileY1);
	}*/

	transformer.m_pLinearTransformer->TileXYToQuadKey(&str_quadkey, nTileX1, nTileY1, nlevel);

	return true;
}

