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
	//isNorthernHemisphere = true;
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
	/*int nPX = 0, nPY = 0;

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

	this->transformer.PixelXYToLatLong(nPX, nPY, nlevel, dlat, dlong);*/

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

	//string str_file = "e:\\degree\\latlong\\" + str_filename + ".qix";
	//string str_file = DISK_LOCATION_LATLONG + str_filename + ".qix";

#ifdef TERRA_NODE
	string str_file = DISK_LOCATION_LATLONG + str_filename + ".qix";
#else
	string str_file = DISK_LOCATION_LATLONG_LOCAL + str_filename + ".qix";
#endif
	
	
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
	default:
		return -1;
	}


}

CString CFMapRequestManager::GetTableName(CString name)
{
	if( "" == name )
	{
		return "";
	}

	int idot = name.Find('.');
	if(-1 == idot)
	{
		return name;
	}

	return name.Left(idot);

}


CString CFMapRequestManager::GetFieldName(CString str, CString tablename)
{
	logObj.writeToLog(_INFO, (char*)str.GetString());

	int icurPos = 0, ifield = 0;
	CString resToken, str_fieldname;
	str_fieldname = "";

	resToken = str.Tokenize(";",icurPos);
	while (resToken != "")
	{
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

	if( ('L' == str.GetAt(idot+1)) || ('l' == str.GetAt(idot+1)) )
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

	if( ('n' == str.GetAt(idot+2)) || ('N' == str.GetAt(idot+2)) )
	{
		return true;
	}
	else
	{
		return false;
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

	if( '0' == str.GetAt(idot+2) )
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

bool CFMapRequestManager::HasNorthUTM(int utm)
{
	if( utm >= 0 )
	{
		return true;
	}
	else
	{
		return false;
	}

}