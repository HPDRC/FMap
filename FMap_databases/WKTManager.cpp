#include "WKTManager.h"

CWKTManager::CWKTManager(void)
{
	m_polygonParts = 0;
	nDBType = 0;
}

CWKTManager::~CWKTManager(void)
{
}

CString CWKTManager::RemoveWKTStringName(CString str, int ipos)
{
	ASSERT( (str.GetLength()-ipos-1) > 0 );

	//remove "POINT"
	//substring??

	if(SQLSERVER == this->nDBType)
	{
		ipos = ipos + 1;
	}



	str.Delete(0, ipos);
	//remove right ")"
	//str.Delete(str.GetLength()-1, 1);
	return str;
}

CString CWKTManager::RemoveWKTParentheses(CString str)
{
	if(str.GetLength() > 1)
	{
		str.Delete(0,1);
		str.Delete(str.GetLength()-1, 1);
	}

	return str;

}

CString CWKTManager::RemoveWKTLeftDoubleParentheses(CString str)
{
	if(str.GetLength() > 1)
	{
		str.Delete(0, 2);
	}

	return str;
}


CString CWKTManager::SplitCString(CString str, CString str_Token, int* icurpos, int *inextpos)
{
	//int inextpos = 0;

	//int test = str.GetLength() - 1;

	*inextpos = str.Find(str_Token, *icurpos);

	if( (-1 == *inextpos) && (0 == *icurpos) ) //not found delimitor and curpos at start index
	{
		//move curpos to end of str
		*icurpos = str.GetLength() - 1;
		return str;
	}
	else if( (-1 != *inextpos)&&(0 == *icurpos) )
	{
		str = str.Left(*inextpos);

		//if this is GeometryCollectionDelimitor, keep all string, 
		//if not, get the rest string
		if(str_Token != this->GetGeometryCollectionDelimitor(this->nDBType))
		{
			str.Delete(*icurpos, (int)(str_Token.GetLength()/2));
		}

		*icurpos = *inextpos + str_Token.GetLength();
		return str;
	}
	else if(-1 != *inextpos)//found the delimitor
	{
		str = str.Left(*inextpos);
		str = str.Right(str.GetLength() - *icurpos);
		*icurpos = *inextpos + str_Token.GetLength();
		return str;
	}
	else if( (-1 == *inextpos) && ((str.GetLength()-1) != *icurpos) )//not found delimitor and curpos is not at end index, this means the last one
	{
		int nend = str.GetLength() - 1;//move icurpos to end
		str = str.Right(str.GetLength() - *icurpos);//the last right side of )),((

		if(str_Token != this->GetGeometryCollectionDelimitor(this->nDBType))
		{
			//str = str.Left(str.GetLength() - (int)str_Token.GetLength()/2/*right side parentheses*/);//remove the )) or ) 
			str = str.Left(str.GetLength() - (int)this->GetTokenlength(str_Token, this->nDBType)/2/*right side parentheses*/);//remove the )) or ) 
		}

		*icurpos = nend;//move icurpos to end
		return str;
	}
	else//not found the delimitor and icurpos at the end of str , return null
	{
		str = "";
	}

	return str;
}


//CString CWKTManager::SplitCString(CString str, CString str_Token, int* ipos)
//{
//	//int n =  str_Token.GetLength();
//	if(-1 != *ipos)
//	{
//		//keep the last position
//		int ilastpos = *ipos;
//
//		*ipos = str.Find(str_Token, *ipos);
//		if(*ipos > 0)//this case: ((dx,dy),(dx,dy))
//		{
//			str   = str.Left(*ipos);
//			str   = str.Right(str.GetLength() - ilastpos);
//			*ipos = *ipos + str_Token.GetLength();
//		}
//		else if( (ilastpos == 0) && (*ipos == -1) )//this case: ((dx,dy))
//		{
//			str = str; //return same one
//		}
//		else
//		{
//			str = ""; //return null
//		}
//
//	}
//	else
//	{ 
//		str = ""; //return null
//	}
// 
//	return str;
//}

int CWKTManager::CountComma(CString str)
{
	int ncount = 0, ipos = 0;

	ipos = str.Find(",", ipos);
	while(-1 != ipos)
	{
		ipos = str.Find(",", ipos+1);// ipos+1 ?????
		ncount++;
	}
	
	return ncount;

}

int CWKTManager::CountPolygonParts(CShape& pShpObj, CString str)
{
	int npartscount = 0, ncommacount = 0, ipos_comma = 0, ipos_parts = 0;
	//bool IsPartsStartingIndex = false;

	ipos_comma = str.Find(",", ipos_comma);
	//ipos_parts = str.Find("),(", ipos_parts);

	//if( MYSQL == this->nDBType )
	//{
	//	ipos_parts = str.Find("),(", ipos_parts);
	//}
	//else if(SQLSERVER == this->nDBType)//SQL Server has space before "("
	//{
	//	ipos_parts = str.Find("), (", ipos_parts);
	//}
	//else
	//{
	//	ipos_parts = str.Find("),(", ipos_parts);
	//}

	ipos_parts = str.Find(this->GetLCRDelimitor(this->nDBType), ipos_parts);




	//if(-1 == ipos_comma)//this is not valid polygon
	//{
	//	return -1;
	//}

	if(-1 == ipos_parts)//only one part
	{
		npartscount = 1;
		ncommacount = 1;
		pShpObj.m_aPartsStartingIndex.Add(1);
		return npartscount;
	}

	//for first part
	npartscount = 1;
	ncommacount = 1;
	pShpObj.m_aPartsStartingIndex.Add(1);
	
	while(-1 != ipos_comma)
	{
		//check all ",", and then check left side if == ')' , check right side if '('
		//check if "),(" in MySQL and "), ("
		if( (')' == str.GetAt(ipos_comma-1))&&('(' == str.GetAt(ipos_comma+1)) ||
			(')' == str.GetAt(ipos_comma-1))&&(' ' == str.GetAt(ipos_comma+1))&&('(' == str.GetAt(ipos_comma+2)))
		{
			npartscount++;
			pShpObj.m_aPartsStartingIndex.Add(ncommacount);
		}

		ipos_comma = str.Find(",", ipos_comma+1);
		//ipos_parts = str.Find("),(", ipos_parts);
		ncommacount++;
	}
	
	return npartscount;
}


bool CWKTManager::GetWKTCoordinates(CString res, double& dX, double& dY)
{

	CString rT;
	int icurPos = 0;

	rT = res.Tokenize(" ",icurPos);
	dX = atof(rT);
	rT = res.Tokenize(" ",icurPos);
	dY = atof(rT);

	return true;

}


CShape* CWKTManager::GetWKTPoint(CString res, CString name)
{

	if( "" == res)
	{
		return NULL;
	}

	CShape* pshpobj = new CPointShape;
	double dlong = 0.0, dlat = 0.0;

	pshpobj->m_aPointArray.SetSize(1);
	this->GetWKTCoordinates(res, dlong, dlat);
	pshpobj->m_aPointArray[0].dX = dlong;
	pshpobj->m_aPointArray[0].dY = dlat;

	//name
	pshpobj->m_aProperties.SetSize(10);
	pshpobj->m_aProperties[0]    = name;

	return pshpobj;

}


CShape* CWKTManager::GetWKTLineString(CString res, CString name)
{

	if( "" == res)
	{
		return NULL;
	}

	CShape* pshpobj = new CLineShape;
	double dlong = 0.0, dlat = 0.0;
	//for segment
	//int ipoint = 0;
	if( (-1 != res.Find("(", 0)) && (-1 != res.Find(")", 0)))
	{
		res = this->RemoveWKTParentheses(res);
	}

	CString resToken_LineString;
	int icurPos = 0, ipointarray = 0;

	//set the size of m_aPointArray
	/*if( ashpobj.m_aPointArray.GetSize() <= 0 )
	{*/
	pshpobj->m_aPointArray.SetSize(this->CountComma(res) + 1);
	//}

	resToken_LineString = res.Tokenize(",",icurPos);
	while (resToken_LineString != "")
	{

		if( (-1 != resToken_LineString.Find("(", 0)) && (-1 != resToken_LineString.Find(")", 0)))
		{
			resToken_LineString = this->RemoveWKTParentheses(resToken_LineString);
		}

		//this->GetWKTCoordinates(ashpobj, resToken_LineString, *ipt);
		this->GetWKTCoordinates(resToken_LineString, dlong, dlat);

		pshpobj->m_aPointArray[ipointarray].dX = dlong;
		pshpobj->m_aPointArray[ipointarray].dY = dlat;

		resToken_LineString = res.Tokenize(",",icurPos);
        ipointarray++;
		//ipt++;
	};

	//name
	pshpobj->m_aProperties.SetSize(20);
	pshpobj->m_aProperties[0] = name;

	return pshpobj;

}


CShape* CWKTManager::GetWKTPolygon(CString res, /*int* ipt,*/ CString name)
{
	//for part

	if( "" == res)
	{
		return NULL;
	}

	/*if( (WKT_POLYGON + 1) != res.Find("(", 0) )
	{
		return NULL;
	}*/

	CShape* pshpobj = new CPolygonShape;
	double dlong = 0.0, dlat = 0.0;

	CString resToken_Polygon, str_PolygonDelimitor;
	int icurPos = 0, inextPos = 0;
	/*if( MYSQL == this->nDBType )
	{
		str_PolygonDelimitor = "),(";
	}
	else if(SQLSERVER == this->nDBType)
	{
		str_PolygonDelimitor = "), (";
	}
	else
	{
		str_PolygonDelimitor = "),(";
	}*/
	//str_PolygonDelimitor = "),(";

	str_PolygonDelimitor = this->GetLCRDelimitor(this->nDBType);


	CString resToken_LineString;
	int icurPos_LineString = 0, ipointarray = 0;

	//the size of PointArray == CountComma(res) + 1
	//ashpobj = new CPolygonShape;
	pshpobj->m_aPointArray.SetSize(this->CountComma(res) + 1);

	/////////////////////////////////////
	//count parts and setup the m_aPartsStartingIndex
	pshpobj->m_nParts = this->CountPolygonParts(*pshpobj, res);

	/////////////////////////////////////




	//resToken_Polygon = res.Tokenize(",",icurPos);
	resToken_Polygon = this->SplitCString(res, str_PolygonDelimitor, &icurPos, &inextPos);
	while (resToken_Polygon != "")
	{
		/*if(-1 != inextPos)
		{
			resToken_Polygon = resToken_Polygon + ")";
		}
		else
		{
			resToken_Polygon = this->RemoveWKTParentheses(resToken_Polygon);
		}*/

		/*if(-1 == inextPos)
		{
			resToken_Polygon = this->RemoveWKTParentheses(resToken_Polygon);
		}*/

		
		
		//////////////////////////////////////////////////////////
		//this->GetWKTLineString(ashpobj, resToken_Polygon, ipt);
		//double dlong = 0.0, dlat = 0.0;
		//for segment
		//int ipoint = 0;

		//make sure there's no "()"
		if( (-1 != resToken_Polygon.Find("(", 0)) && (-1 != resToken_Polygon.Find(")", 0)))
		{
			resToken_Polygon = this->RemoveWKTParentheses(resToken_Polygon);
		}


		int icurPos_LineString = 0;
		resToken_LineString = resToken_Polygon.Tokenize(",",icurPos_LineString);
		while (resToken_LineString != "")
		{
			//this->GetWKTCoordinates(ashpobj, resToken_LineString, *ipt);
			this->GetWKTCoordinates(resToken_LineString, dlong, dlat);

			pshpobj->m_aPointArray[ipointarray].dX = dlong;
			pshpobj->m_aPointArray[ipointarray].dY = dlat;

			resToken_LineString = resToken_Polygon.Tokenize(",",icurPos_LineString);
			ipointarray++;
			//ipt++;
		};       

		//////////////////////////////////////////////////////////
		//pshpobj->m_nParts++;
		//pshpobj->m_aPartsStartingIndex.Add(*ipt);
		//pshpobj->m_aPartsStartingIndex.Add(*ipt);

		
		resToken_Polygon = this->SplitCString(res, str_PolygonDelimitor, &icurPos, &inextPos);

		
		//this->m_polygonParts++; //default: m_polygonParts = 0
		//this->m_polygonPartsStartingIndex.Add(*ipt);

	};

	//name
	pshpobj->m_aProperties.SetSize(20);
	pshpobj->m_aProperties[0] = name;

	return pshpobj;


}


CShape* CWKTManager::GetWKTPolygon(CString res, CString name, CString color)
{
	//for part

	if( "" == res)
	{
		return NULL;
	}

	CShape* pshpobj = new CPolygonShape;
	double dlong = 0.0, dlat = 0.0;

	CString resToken_Polygon, str_PolygonDelimitor;
	int icurPos = 0, inextPos = 0;

	str_PolygonDelimitor = this->GetLCRDelimitor(this->nDBType);


	CString resToken_LineString;
	int icurPos_LineString = 0, ipointarray = 0;
	pshpobj->m_aPointArray.SetSize(this->CountComma(res) + 1);
	pshpobj->m_nParts = this->CountPolygonParts(*pshpobj, res);


	resToken_Polygon = this->SplitCString(res, str_PolygonDelimitor, &icurPos, &inextPos);
	while (resToken_Polygon != "")
	{


		//make sure there's no "()"
		if( (-1 != resToken_Polygon.Find("(", 0)) && (-1 != resToken_Polygon.Find(")", 0)))
		{
			resToken_Polygon = this->RemoveWKTParentheses(resToken_Polygon);
		}


		int icurPos_LineString = 0;
		resToken_LineString = resToken_Polygon.Tokenize(",",icurPos_LineString);
		while (resToken_LineString != "")
		{
			//this->GetWKTCoordinates(ashpobj, resToken_LineString, *ipt);
			this->GetWKTCoordinates(resToken_LineString, dlong, dlat);

			pshpobj->m_aPointArray[ipointarray].dX = dlong;
			pshpobj->m_aPointArray[ipointarray].dY = dlat;

			resToken_LineString = resToken_Polygon.Tokenize(",",icurPos_LineString);
			ipointarray++;
			//ipt++;
		};       
	
		resToken_Polygon = this->SplitCString(res, str_PolygonDelimitor, &icurPos, &inextPos);


	};

	//name
	pshpobj->m_aProperties.SetSize(20);
	pshpobj->m_aProperties[0] = name;

	//set color info. in attributes which from database
	if( color != "" )
	{
		pshpobj->m_aProperties[3] = color;
	}

	return pshpobj;


}


CShape* CWKTManager::GetWKTMultiPolygon(CString res, /*int* ipt,*/ CString name)
{

	if( "" == res)
	{
		return NULL;
	}

	res.Replace("((", "(");
	res.Replace("))", ")");

	/*CShape* pshpobj = new CPolygonShape;
	pshpobj = this->GetWKTPolygon(res, ipt, name);

	return pshpobj;*/

	return this->GetWKTPolygon(res, /*ipt,*/ name);

}

//int GetWKTGeometryCollectionTotalCount(CString str_res)
//{
//
//}

bool CWKTManager::GetWKTGeometryCollectionStringList(CString str_res, CStringList* strlist)
{

	if( "" == str_res)
	{
		return false;
	}

	CString resToken_GeometryCollection, str_GeometryCollectionDelimitor;

	str_GeometryCollectionDelimitor = this->GetGeometryCollectionDelimitor(this->nDBType);

	int icurPos = 0, inextPos = 0;

	resToken_GeometryCollection = this->SplitCString(str_res, str_GeometryCollectionDelimitor, &icurPos, &inextPos);
	while (resToken_GeometryCollection != "")
	{
		if( -1 != inextPos )
		{
			resToken_GeometryCollection = resToken_GeometryCollection + ")";
		}


		strlist->AddTail(resToken_GeometryCollection);

		resToken_GeometryCollection = this->SplitCString(str_res, str_GeometryCollectionDelimitor, &icurPos, &inextPos);

	}

	return true;

}


bool CWKTManager::GetWKTGeometryCollection(CShapePtrArray& apShapesArray, CString name, CStringList* strlist, int ntype)
{

	CString listitem;
	if( !strlist->IsEmpty() )
	{
		POSITION pos = strlist->GetHeadPosition();
		while( pos != NULL )
		{
			//this->GetRecordsFromWKT(apShapesArray, this->geometrylist.GetNext(pos), str_name);
			listitem = strlist->GetNext(pos);
			switch(ntype)
			{
			case ShapeUtility::polygon:
				if( (WKT_POLYGON + 1) == listitem.Find("(", 0) )
				{
					apShapesArray.Add(this->GetWKTPolygon(listitem, name));
				}	
				break;
			case ShapeUtility::point:
				if( (WKT_POINT + 1) == listitem.Find("(", 0) )
				{
					apShapesArray.Add(this->GetWKTPoint(listitem, name));
				}
				break;
			case ShapeUtility::arc:
				if( (WKT_LINESTRING_MULTIPOINT + 1) == listitem.Find("(", 0) )
				{
					apShapesArray.Add(this->GetWKTLineString(listitem, name));
				}
				break;
			default:
				logObj.writeToLog(_INFO, "unknown shp type in WKTManager");
				return false;
			}
		}
		//geometrylist.RemoveAll();
	}
	return true;
}


//bool CWKTManager::GetWKTMultiPolygon(CShapePtrArray& apShapesArray, CString res, int* ipt, CString name)
//{
//	CString resToken_MultiPolygon, str_MultiPolygonDelimitor;
//	int icurPos = 0, inextPos = 0;
//	str_MultiPolygonDelimitor = ")),((";
//
//	resToken_MultiPolygon = this->SplitCString(res, str_MultiPolygonDelimitor, &icurPos, &inextPos);
//	resToken_MultiPolygon = this->RemoveWKTLeftDoubleParentheses(resToken_MultiPolygon);
//
//	while (resToken_MultiPolygon != "")
//	{
//		//add the "))"
//		if(-1 != inextPos)
//		{
//			resToken_MultiPolygon = "((" + resToken_MultiPolygon + "))";
//		}
//		else
//		{
//			resToken_MultiPolygon = this->RemoveWKTParentheses(resToken_MultiPolygon);
//		}
//		//resToken_MultiPolygon = resToken_MultiPolygon + "))";
//
//		//CShape* pshape = NULL;
//		//this->GetWKTPolygon(resToken_MultiPolygon, ipt);
//		apShapesArray.Add(this->GetWKTPolygon(resToken_MultiPolygon, ipt, name));
//		resToken_MultiPolygon = this->SplitCString(res, str_MultiPolygonDelimitor, &icurPos, &inextPos);
//	}
//
//	//resToken_MultiPolygon = res.Tokenize(")),((",icurPos);
//	//while (resToken_MultiPolygon != "")
//	//{
//	//	//add the "))"
//	//	resToken_MultiPolygon = resToken_MultiPolygon + "))";
//
//	//	CShape* pshape = NULL;
//	//	this->GetWKTPolygon(*pshape, resToken_MultiPolygon, ipt);
//	//	apShapesArray.Add(pshape);
//	//	resToken_MultiPolygon = res.Tokenize(")),((",icurPos);
//	//}
//
//	return true;
//
//}


bool CWKTManager::GetWKTMultiLineString(CShapePtrArray& apShapesArray, CString res, CString name)
{
	//res.Replace("((", "(");
	//res.Replace("))", ")");
	if( "" == res)
	{
		return false;
	}

	CString resToken, str_MultiLineStringDelimitor;
	//str_MultiLineStringDelimitor = "),(";

	/*if( MYSQL == this->nDBType )
	{
		str_MultiLineStringDelimitor = "),(";
	}
	else if(SQLSERVER == this->nDBType)
	{
		str_MultiLineStringDelimitor = "), (";
	}
	else
	{
		str_MultiLineStringDelimitor = "),(";
	}*/

	str_MultiLineStringDelimitor = this->GetLCRDelimitor(this->nDBType);


	int icurPos = 0, inextPos = 0;

	resToken = this->SplitCString(res, str_MultiLineStringDelimitor, &icurPos, &inextPos);
	while (resToken != "")
	{

		//here might need to check the format of resToken
		apShapesArray.Add(this->GetWKTLineString(resToken, name));

		resToken = this->SplitCString(res, str_MultiLineStringDelimitor, &icurPos, &inextPos);

	}

	return true;

}


bool CWKTManager::GetWKTMultiPoint(CShapePtrArray& apShapesArray, CString res, CString name)
{
	//res.Replace("((", "(");
	//res.Replace("))", ")");
	if( "" == res)
	{
		return false;
	}

	CString resToken, str_MultiPointDelimitor;
	//str_MultiPointDelimitor = "),(";


	/*if( MYSQL == this->nDBType )
	{
		str_MultiPointDelimitor = "),(";
	}
	else if(SQLSERVER == this->nDBType)
	{
		str_MultiPointDelimitor = "), (";
	}
	else
	{
		str_MultiPointDelimitor = "),(";
	}*/


	str_MultiPointDelimitor = this->GetLCRDelimitor(this->nDBType);



	int icurPos = 0, inextPos = 0;

	resToken = this->SplitCString(res, str_MultiPointDelimitor, &icurPos, &inextPos);
	while (resToken != "")
	{

		//here might need to check the format of resToken
		apShapesArray.Add(this->GetWKTPoint(resToken, name));

		resToken = this->SplitCString(res, str_MultiPointDelimitor, &icurPos, &inextPos);

	}

	return true;
}


bool CWKTManager::SetDatabaseType(int ntype)
{
	if( -1 == ntype)
	{
		return false;
	}

	this->nDBType = ntype;
	return true;
}


CString CWKTManager::GetLCRDelimitor(int ntype)//get "),("
{
	if( -1 == ntype )
	{
		return "";
	}

	if( MYSQL == this->nDBType )
	{
		return "),(";
	}
	else if(SQLSERVER == this->nDBType)
	{
		return "), (";
	}
	else
	{
		return "),(";
	}
}

CString CWKTManager::GetGeometryCollectionDelimitor(int ntype)//get "), "
{
	if( -1 == ntype )
	{
		return "";
	}

	if( MYSQL == this->nDBType )
	{
		return "),";
	}
	else if(SQLSERVER == this->nDBType)
	{
		return "), ";
	}
	else
	{
		return "),";
	}
}

int CWKTManager::GetTokenlength(CString str, int ntype)
{
	if( "" == str )
	{
		return 1;
	}

	if( MYSQL == this->nDBType )
	{
		return str.GetLength();
	}
	else if( SQLSERVER == this->nDBType )
	{
		return str.GetLength() - 1;
	}
	else
	{
		return str.GetLength();
	}

}