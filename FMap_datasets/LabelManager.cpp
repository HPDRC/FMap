#include "LabelManager.h"


CString ExtractPictoFile( CString file, CString dir )
{
	file.TrimRight();

	if(file.GetAt(file.GetLength()-1) == 'w')	// indicate that it's a raw file
	{
		dir += "rawIcon\\";

		// change 'raw' to 'bmp'
		int length = file.GetLength();
		file.SetAt(length-1, 'p');
		file.SetAt(length-2, 'm');
		file.SetAt(length-3, 'b');
	}

	file = dir+file; 

	return file;
}




CLabelManager::CLabelManager(void)
{
}

CLabelManager::~CLabelManager(void)
{
}


CString CLabelManager::ExtractPointLabel(CPointShape* pPointShape)
{
	ASSERT( pPointShape != NULL );

	CString sLable = pPointShape->m_aProperties[0];
	
	if(sLable.IsEmpty())
	{
		sLable = pPointShape->m_aProperties[3];	// name from column
	}
	
	if(sLable.IsEmpty())
	{
		sLable = pPointShape->m_aProperties[7];
	}


	if(sLable.IsEmpty())//for ptrim
	{
		sLable = pPointShape->m_aProperties[9];
	}



	//if(sLable.IsEmpty())//for wm_Time_Zones
	//{
	//	sLable = pPointShape->m_aProperties[12];
	//}

	if(sLable.IsEmpty())//for wm_Airports
	{
		sLable = pPointShape->m_aProperties[13];
	}

	if(sLable.IsEmpty())//for wm_Airports
	{
		sLable = pPointShape->m_aProperties[14];
	}

	if(sLable.IsEmpty())//for wm_Glacier
	{
		sLable = pPointShape->m_aProperties[15];
	}

	if(sLable.IsEmpty())//for wm_Glacier
	{
		sLable = pPointShape->m_aProperties[16];
	}

	if(sLable.IsEmpty())//for wm_Glacier
	{
		sLable = pPointShape->m_aProperties[17];
	}

	if(sLable.IsEmpty())//for wm_Glacier
	{
		sLable = pPointShape->m_aProperties[18];
	}

	if(sLable.IsEmpty())//for rtgauges
	{
		//sLable = pPointShape->m_aProperties[10] + "/" + pPointShape->m_aProperties[8] + "'";

		bool isflowempty = false;
		sLable = pPointShape->m_aProperties[10];
		
		double dflow = atof(sLable.GetString());

        sLable = _T(sLable);
		sLable.Format("%f", dflow);


		dflow = dflow*10000;
		int nflow = (int)dflow;
		if(nflow == 0)
		{
			isflowempty = true;
		}

		int idot = sLable.Find('.');
		if( -1 != idot )
		{
			int nlength = 0;
			nlength = idot + 4;
			sLable = sLable.Left(nlength);
			if('0' == sLable.GetAt(nlength - 1))
			{
				sLable = sLable.Left(nlength - 1);
				if('0' == sLable.GetAt(nlength - 2))
				{
					sLable = sLable.Left(nlength - 2);
					if('0' == sLable.GetAt(nlength - 3))
					{
						sLable = sLable.Left(nlength - 4);
					}
				}
			}
			
		}
		
		if( isflowempty )
		{
			sLable = "";
		}
		else
		{
			sLable = sLable + "cfs" + "/"; //5 length			
		}


		CString sLable_rtgauges_stage;
		sLable_rtgauges_stage = pPointShape->m_aProperties[8];

		double dstage = atof(sLable_rtgauges_stage.GetString());
		sLable_rtgauges_stage = _T(sLable_rtgauges_stage);
		sLable_rtgauges_stage.Format("%f", dstage);
		
		int idot1 = sLable_rtgauges_stage.Find('.');
		if( -1 != idot1 )
		{
			int nlength = 0;
			nlength = idot1 + 4;
			sLable_rtgauges_stage = sLable_rtgauges_stage.Left(nlength);

			if('0' == sLable_rtgauges_stage.GetAt(nlength - 1))
			{
				sLable_rtgauges_stage = sLable_rtgauges_stage.Left(nlength - 1);
				if('0' == sLable_rtgauges_stage.GetAt(nlength - 2))
				{
					sLable_rtgauges_stage = sLable_rtgauges_stage.Left(nlength - 2);
					if('0' == sLable_rtgauges_stage.GetAt(nlength - 3))
					{
						sLable_rtgauges_stage = sLable_rtgauges_stage.Left(nlength - 4);
					}
				}
			}
		}

		sLable_rtgauges_stage = sLable_rtgauges_stage + "'"; //7 length
		
		sLable = sLable + sLable_rtgauges_stage;


	}





	//remove "the dining point" & "the business point" label
	if("" != pPointShape->m_aProperties[4])
	{
		sLable = "";
	}

	//mulr point includes two parts for label
	if( "" != pPointShape->m_aProperties[6] )
	{
		//then the total length is "the first length and the second length"
		sLable = sLable + "/" + pPointShape->m_aProperties[6];
	}

	//remove C5DFCF on condex data
	CString stemplable = sLable;
	if( ("C5DFCF" == stemplable.MakeUpper()) ||
		("/C5DFCF" == stemplable.MakeUpper()) )
	{
		sLable = "";
	}

	if("NONAME" == sLable)
	{
		sLable = "";
	}

	if("No Data" == sLable)
	{
		sLable = "";
	}

	return sLable;
}



CString CLabelManager::GetSpecifiedLable(CPointShape* pPointShape, int ipos)
{
	ASSERT( pPointShape != NULL );

	CString sLable = pPointShape->m_aProperties[ipos];

	return sLable;
}


bool CLabelManager::HasIgnoredLableColumn(CPointShape* pPointShape, int ipos)
{
	if( FA_OWNER == ipos)
	{
		//olny show owner, not show addr shape
		//CLabelManager lm;
		if("" == this->GetSpecifiedLable(pPointShape, FA_OWNER))//if this is addr, the owner column should be blank
		{
			return true;
		}
	}
	if( FA_ADDR == ipos)
	{
		//only show addr, not show owner shape
		//CLabelManager lm;
		if("" != this->GetSpecifiedLable(pPointShape, FA_OWNER))//if this is owner, the owner column should not be blank
		{
			return true;
		}
	}

	return false;
}




CString CLabelManager::ExtractArcLabel( CLineShape* pLineShape )
{
	ASSERT( pLineShape != NULL );

	//CString sLabel = pLineShape->m_aProperties[0];
	CString sLabel = pLineShape->m_aProperties[0];
	//int nSize = pLineShape->m_aProperties.GetCount();

	if(sLabel.IsEmpty())
	{
		sLabel = pLineShape->m_aProperties[1]; //be careful the size
	}

	if(sLabel.IsEmpty())
	{
		sLabel = pLineShape->m_aProperties[2]; //be careful the size
	}

	if(sLabel.IsEmpty())
	{
		sLabel = pLineShape->m_aProperties[5]; //be careful the size
	}

	if(sLabel.IsEmpty())
	{
		sLabel = pLineShape->m_aProperties[6]; //be careful the size
	}



	if(sLabel.IsEmpty())
	{
		sLabel = pLineShape->m_aProperties[7]; //wm_Water_line
	}

	if(sLabel.IsEmpty())
	{
		sLabel = pLineShape->m_aProperties[8]; //wm_Minor_routes
	}


	if(sLabel.IsEmpty())
	{
		sLabel = pLineShape->m_aProperties[9]; //wm_Minor_routes
	}

	if(sLabel.IsEmpty())
	{
		sLabel = pLineShape->m_aProperties[10]; //wm_Minor_routes
	}

	if(sLabel.IsEmpty())
	{
		sLabel = pLineShape->m_aProperties[11]; //fiber_optics
	}

	//if(sLabel.IsEmpty())
	//{
	//	sLabel = pLineShape->m_aProperties[12]; //fiber_optics
	//}

	if("NONAME" == sLabel)
	{
		sLabel = "";

	}


    return sLabel;
}


CString CLabelManager::ExtractAreaLabel( CPolygonShape* pPolygonShape )
{
	ASSERT( pPolygonShape != NULL );


	CString sLable = pPolygonShape->m_aProperties[0];
	CString sBgLable = pPolygonShape->m_aProperties[5];//bg ,4/5 are diff
	CString sTractLabel = pPolygonShape->m_aProperties[4];//tract,4/5 are same

	if( !sTractLabel.IsEmpty()  && (sTractLabel.GetLength()>4) )
	{
		sTractLabel = sTractLabel.Left(4) + sTractLabel.Right(2);
	}
	bool IsDoubleName = false;//for tract
	if ( (sBgLable == sTractLabel) && (!sBgLable.IsEmpty()) &&
		(!sTractLabel.IsEmpty()) )
	{
		IsDoubleName = true;
	}

	if( sLable.IsEmpty() && sBgLable.IsEmpty() )
	{
		sLable = pPolygonShape->m_aProperties[4];	// name from column
	}
	else if( sLable.IsEmpty() && (!sBgLable.IsEmpty()) &&
		(sBgLable.GetLength()<=2) && (!IsDoubleName) )
	{
		sLable = pPolygonShape->m_aProperties[4];
		sLable = sBgLable + "." + sLable;
	}
	else if( sLable.IsEmpty() && sBgLable.GetLength()>2 && //bg
		sBgLable.GetLength()<=4 && (!IsDoubleName) )
	{
		sLable = pPolygonShape->m_aProperties[4];
		sBgLable.Insert(2,".");
		sLable = sBgLable + "." + sLable;
	}
	else if( sLable.IsEmpty() && sBgLable.GetLength()>4 && //bg
		(!IsDoubleName))
	{
		sLable = pPolygonShape->m_aProperties[4];
		sBgLable.Insert(2,".");
		sBgLable.Insert(5,".");
		sLable = sBgLable + "." + sLable;
	}
	else if(sLable.IsEmpty() && sBgLable.GetLength()>2 && //tract
		sBgLable.GetLength()<=4 && (IsDoubleName) )
	{
		sLable = pPolygonShape->m_aProperties[5];
		sLable.Insert(2,".");
	}
	else if(sLable.IsEmpty() && sBgLable.GetLength()>4 && //tract
		(IsDoubleName) )
	{
		sLable = pPolygonShape->m_aProperties[5];
		sLable.Insert(2,".");
		sLable.Insert(5,".");
	}
	else
	{}

	if( sLable.IsEmpty() )//for ZIP area
	{
		sLable = pPolygonShape->m_aProperties[6];
	}


	if( sLable.IsEmpty() )//wm_Admin
	{
		sLable = pPolygonShape->m_aProperties[7];
	}

	if( sLable.IsEmpty() )//wm_Admin
	{
		sLable = pPolygonShape->m_aProperties[9];
	}

	if( sLable.IsEmpty() )//wm_Admin
	{
		sLable = pPolygonShape->m_aProperties[10];
	}

	if( sLable.IsEmpty() )//wm_Admin
	{
		sLable = pPolygonShape->m_aProperties[11];
	}

	if("NONAME" == sLable)
	{
		sLable = "";

	}


	return sLable;

}


int CLabelManager::ExtractAntiAliasingDegree(int* nAADgree)
{
	int n = 150;
	int nTotal = 3600/300; // lfEscapement from 0 to 3600, each 300 as one degree

	/*if( (nAADgree >= 0) && (nAADgree <= n) )
	{
		return 0;
	}
	else if( (nAADgree > n) && (nAADgree <= 3*n) )
	{
		return 2*n;
	}
	else if( (nAADgree > 3*n) && (nAADgree <= 5*n) )
	{
		return 4*n;
	}
	else if( (nAADgree > 5*n) && (nAADgree <= 7*n) )
	{
		return 6*n;
	}
	else if( (nAADgree > 7*n) && (nAADgree <= 9*n) )
	{
		return 8*n;
	}	
	else if( (nAADgree > 9*n) && (nAADgree <= 11*n) )
	{
		return 10*n;
	}*/

	if( (*nAADgree >= 0) && (*nAADgree <= n) )
	{
		return 0;
	}

	for( int i=1; i<=nTotal; i++)
	{

		if ( (*nAADgree >= (2*i-1)*n ) && (*nAADgree <= (2*i+1)*n) )
		{
			return 2*i*n;
		}

	}

	return 0;

}


	
bool CLabelManager::HasHighlightLabel(CString strL1, CString strL2)
{
	if( (strL1.Trim().MakeUpper()) == (strL2.Trim().MakeUpper()) )
	{
		return true;
	}

	return false;
	
}


//CString CLabelManager::ExtractPictoFile( CString file )
//{
//	file.TrimRight();
//
//	if(file.GetAt(file.GetLength()-1) == 'w')	// indicate that it's a raw file
//	{
//		dir += "rawIcon\\";
//
//		// change 'raw' to 'bmp'
//		int length = file.GetLength();
//		file.SetAt(length-1, 'p');
//		file.SetAt(length-2, 'm');
//		file.SetAt(length-3, 'b');
//	}
//
//	file = dir+file; 
//
//	return file;
//}