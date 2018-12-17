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
	isPNG         = false;

	isHighlight   = false;

	isTxt         = false;

	isNorthHemisphere = true;
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

		int iDot = (int) (strOverlay.find_last_of('.'));

		if(iDot != std::string::npos)
		{
			std::string strAreaSuffix = strOverlay.substr(iDot+1);
			if( this->IsResVisibleDot(strAreaSuffix) )
			{
				strOverlay = reqOverlays.getNextOverlay();
				continue;
			}
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


bool CFMapRequestManager::HasFilledAreaReq(std::string strAreaReq, bool hasFilledAreaReq)
{
	ASSERT(strAreaReq != "");

	Overlays reqOverlays(strAreaReq);

	std::string strOverlay = reqOverlays.getNextOverlay();
	//std::string strFillArea;
	//std::string strBlankLayer = "blanklayer";

	while(strOverlay != "")
	{

		//int iDot = (int) (strOverlay.find('.'));
		int iDot = (int) (strOverlay.find_last_of('.'));

		if(iDot != std::string::npos)
		{
			std::string strAreaSuffix = strOverlay.substr(iDot+1);

			/*if( this->IsResVisibleDot(strAreaSuffix) )
			{
				strOverlay = reqOverlays.getNextOverlay();
				continue;
			}*/

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


std::string CFMapRequestManager::RefactLayer( bool hasGIF, std::string strReq, bool *pHasTransCanvas, TransInfo *ptransInfo)
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


		//if( "FA_Point_O" == strOverlay )//for owner field
		//{
		//	int ipos = strReq.find("FA_Point_O");
		//	strReq.replace(ipos, 9, "FA_Point");
		//	*pFAOwner = true;
		//}

		//if( "FA_Point_A" == strOverlay )//for address field
		//{
		//	int ipos = strReq.find("FA_Point_A");
		//	strReq.replace(ipos, 9, "FA_Point");
		//	//strReq.replace("FA_Point_A", "FA_Point");
		//	*pFAADDR = true;
		//}



		/*if( "points2" == strOverlay )
		{
			newpoints = "+points";
			strReq.append( newpoints.c_str() );
		}*/

		if( ( "seas" == strOverlay ) && (!hasGIF) )
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

	/*strReq.Replace("FA_Point_O", "FA_Point");
	strReq.Replace("FA_Point_A", "FA_Point");*/


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


bool CFMapRequestManager::HasLatLongStreetRequest(std::string str_layer_postfix)
{
	if( (str_layer_postfix.at(0)=='w') || 
		(str_layer_postfix.at(0)=='W') ||
		(str_layer_postfix.at(0)=='w') ||
		(str_layer_postfix.at(0)=='W') )
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool CFMapRequestManager::HasNewResSyntax(std::string overlays)
{
	char hyphen = '-';
	int nlength = overlays.length();

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
				return true;
			}
		}
	}

	return false;

}

int CFMapRequestManager::GetLineWidth( int& lineWidth )
{
	int nwidth = 0;

	int nlinewidth_2  = 2,
		nlinewidth_6  = 6,
		nlinewidth_10 = 10;


	if( 1 == lineWidth )
	{
		nwidth = nlinewidth_2;
	}
	else if( 2 == lineWidth )
	{
		nwidth = nlinewidth_6;
	}
	else if( 3 == lineWidth )
	{
		nwidth = nlinewidth_10;
	}
	else
	{
		nwidth = nlinewidth_2;
	}

	return nwidth;
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

bool CFMapRequestManager::IsResVisibleDot(std::string overlays)
{
	int iDot = (int) (overlays.find_last_of('.'));
	string info = overlays.substr(iDot+1);

	/*if (("0"==info)
		||("075"==info) 
		|| ("15"==info)
		|| ("3"==info)
		|| ("6"==info)
		|| ("2"==info)
		|| ("4"==info)
		|| ("8"==info)
		|| ("874"==info)
		|| ("748"==info)
		|| ("496"==info)
		|| ("992"==info)
		|| ("984"==info)
		|| ("969"==info)
		|| ("939"==info)
		|| ("879"==info)
		|| ("758"==info)
		|| ("517"==info) )
	{
		return true;
	}*/
	int ihyphen = info.find('-');
	if(-1 == ihyphen)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

std::string CFMapRequestManager::GetLatLongFiles(std::string str_filename)
{
	if( "" == str_filename )
	{
		return str_filename;
	}

	//string str_file = "C:\\degree\\latlong\\" + str_filename + ".qix";

#ifdef TERRA_NODE
	string str_file = DISK_LOCATION_LATLONG + str_filename + ".qix";
#else
	string str_file = DISK_LOCATION_LATLONG_LOCAL + str_filename + ".qix";
#endif
	
	return str_file;

}


bool CFMapRequestManager::HasFAADDRReq(CString* pstr)
{
	if( -1 != pstr->Find("FA_Point_A"))
	{
		pstr->Replace("FA_Point_A", "FA_Point");
		return true;
	}
	else if(-1 != pstr->Find("FA_Point_O"))
	{
		pstr->Replace("FA_Point_O", "FA_Point");
        return false;
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