#include "StringLayerDataSource.h"

#include "PointShape.h"
#include "LineShape.h"
#include "PolygonShape.h"


CStringLayerDataSource::CStringLayerDataSource(void)
{
	nShapeOjbectCount = 0;
}

CStringLayerDataSource::~CStringLayerDataSource(void)
{

}


bool CStringLayerDataSource::FromString(CString strData)
{
 // parse the string and split it into an array
// be carefull with the triming of the string and the lenght > 0


	ASSERT( strData.GetLength() > 0 );
	
	CString resToken;
	int curPos= 0;

	resToken= strData.Tokenize(strObjectDelimiter, curPos);


	while (curPos != -1)
	{
		
		resToken = resToken.Trim();
		if (!resToken.IsEmpty())
		{
			m_aStrShapeObject.Add(resToken);
		}
		resToken= strData.Tokenize(strObjectDelimiter, curPos);
	
	};


	//int nIndex1 = 0;
	//int nIndex2 = 0;
	//strData += ',';	
	//nIndex1 = strData.Find(',',nIndex1);
	//CString strTemp;

	//while( -1 != nIndex1 )
	//{
	//	// trans or override info exists
	//	//string info = overlay.substr(idx+1);
	//	//overlay = overlay.substr(0, idx);
	//	strTemp = strData.Mid( nIndex2,nIndex1-nIndex2 );
	//	aStrShapeObject.Add(strTemp);
	//	nIndex2 = nIndex1+1;
	//	nIndex1 = strData.Find(',', nIndex2);
	//}

	return true;

}

long  CStringLayerDataSource::GetRecoderCount(void)
{	
	return  (long) (m_aStrShapeObject.GetCount());
}

ShapeUtility::ShapeType CStringLayerDataSource::getShapeType(void) 
{

	return ShapeUtility::point;

}

CShape*  CStringLayerDataSource::getRecord(long lRecord, CStringArray& aPropertyFields)
{
	//ASSERT(false);// no implemneted

   // parse each substing according to the index of LRecord.
	ASSERT(lRecord >= 0 && lRecord <  m_aStrShapeObject.GetCount());
	double dPointX = 0.0;
	double dPointY = 0.0;
	CString strName = " ";
	CString sPicto  = "other"; 
	CString strPicto = "other.raw";
	

  // convert string to shape object , convert string to float for coordinate 
 

	CString strObject = m_aStrShapeObject[ lRecord ];
	//CString resToken;

	int i = 0;
	//int curPos= 0;

	//resToken= strObject.Tokenize(strPropertyDelimiter, curPos);


	/*while (curPos != -1)
	{
		resToken = resToken.Trim();

		if (i  == 0)
		{
			if (resToken.IsEmpty())
			{
				dPointX = 0.0;
			}
			else
			{
				CString strX   = resToken;
				dPointX = atof(strX);		
			}

		}
		else if( i == 1)
		{
			if (resToken.IsEmpty())
			{
				dPointY = 0.0;
			}
			else
			{
				CString strY   = resToken;
				dPointY = atof(strY);
			}

		}
		else if( i == 2)
		{	
			if("." == resToken)
			{
				strName = " ";
			}
			else
			{
				strName = resToken;
			}
		}
		else if( i == 3 )
		{
			strPicto = resToken + ".raw";
		}
		else
		{
			break; /// more than 3 properties provided.ignore the extra ones
			ASSERT(false);
		}

		resToken= strObject.Tokenize(strPropertyDelimiter, curPos);

		i++;
	};*/


	//Cstring str = (char*)(LPCTSTR)strObject;

	char * pch,*prevpch=(char*)(LPCTSTR)strObject,*endpch=(char*)(LPCTSTR)strObject+strlen((char*)(LPCTSTR)strObject);
	//printf ("Splitting string \"%s\" into tokens:\n",str);
	pch = strchr ((char*)(LPCTSTR)strObject,',');
	do
	{
		if (pch!=NULL)
			*pch='\00';

		if (i  == 0)
		{
			if (*prevpch)
			{
			    dPointX = atof(prevpch);
			}
			else
			{
				dPointX = 0.0;
			}
		}
		else if( i == 1)
		{
			if (*prevpch)
			{
			    dPointY = atof(prevpch);
			}
			else
			{
				dPointY = 0.0;
			}
		}
		else if( i == 2)
		{	
			if('.' == *prevpch)
			{
				strName = " ";
			}
			else if(*prevpch)
			{
			    strName = prevpch;
			}
			else
			{
				strName = " ";
			}

		}
		else if( i == 3 )
		{
			if(*prevpch)
			{
				sPicto   = prevpch;
				strPicto = sPicto + ".raw";
			}
		}
		else
		{
			break; /// more than 3 properties provided.ignore the extra ones
			ASSERT(false);
		}

		prevpch=pch+1;
		if (pch>=endpch || pch==NULL)
			break;
		pch = strchr (pch+1, ',');
		i++;

	} while (true);



	// be carefull with the triming of the string and the lenght > 0
    //if(str.length > 0 )....


	////////////////////////
	/*
	;//here is very very dangrous!!! 
	flexibility here is very important, here here we have a hard code of class.

	should be a "class factory" mechanism
	or a swith case statement.
	*/
	CShape*  pShape = new CPointShape; 
	if (pShape == NULL)// if failed to new objcet.
	{
		LOG("ERROR: %s \n", "insufficent memery occured, when allocate memery for shape object");
		//delete pShape; ///!!!!!!!!!!!!!
		return pShape = NULL;
	}


	pShape->m_aPointArray.SetSize(1);

	pShape->m_aPointArray[0].dX = dPointX;
	pShape->m_aPointArray[0].dY = dPointY;

	//this one should be set
	pShape->m_aProperties.SetSize(17);

	pShape->m_aProperties[0] = strName;		//("labeltq") );//name		
	pShape->m_aProperties[1] = "ffffff";	//("LABELCQ") );		
	pShape->m_aProperties[2] = strPicto;	    //("PictoFQ") );//picto	
	pShape->m_aProperties[3] = "";			//("column1") );//another na	
	pShape->m_aProperties[4] = "";			//("column10") );//dining&bu	
	pShape->m_aProperties[5] = "";			//("column6") );//hotel poin
	pShape->m_aProperties[6] = "";			//("LABEL1TQ") );//mulr seco
	pShape->m_aProperties[7] = "";			//("CITYNAME") );//city name
	pShape->m_aProperties[8] = "";			//("STAGE") );//city name
	pShape->m_aProperties[9] = "";			//("SHORTNAME") );//city name
	pShape->m_aProperties[10] = "";			//("FLOW") );//city name
	pShape->m_aProperties[11] = "";			//("CITYNAME") );//city name

	pShape->m_aProperties[12] = "";			//("CITYNAME") );//city name
	pShape->m_aProperties[13] = "";			//("CITYNAME") );//city name
	pShape->m_aProperties[14] = "";			//("CITYNAME") );//city name
	pShape->m_aProperties[15] = "";			//("CITYNAME") );//city name
	pShape->m_aProperties[16] = "";			//("CITYNAME") );//city name




	return pShape;
}

//bool CStringLayerDataSource::GetRecordFromMySQL(CShapePtrArray& apShapes)
//{
//
//	return true;
//}