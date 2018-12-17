

#include "StdAfx.h"
#include "doubleRect.h"
#include "doublePoint.h"
#include "ShapeObject.h"
#include "polygonShape.h"
#include "lineShape.h"
#include "PointShape.h"
#include "configure.h"
//#include "ShapeLayer.h"
//ShapeUtility

#include "LayerDataSource.h"
#include <vector>
#include "QixShapeFileLayerDataSource.h"

using namespace std;

extern Log logObj;


CQixShapeFileLayerDataSource::CQixShapeFileLayerDataSource(void)
{
	hSHPFile = NULL;
	hDBFFile = NULL;
	
}


CQixShapeFileLayerDataSource::CQixShapeFileLayerDataSource(CString sfileName, CString& sAccessMode)
{
	FromFile(sfileName,sAccessMode);
}



CQixShapeFileLayerDataSource::~CQixShapeFileLayerDataSource(void)
{
	if(hSHPFile != NULL)
	{
		SHPClose( hSHPFile );
		hSHPFile = NULL;
	}

	if(hDBFFile != NULL)
	{
		DBFClose( hDBFFile );
		hDBFFile = NULL;
	}
	
	//for(int i = 0; i < records.size(); i++)
	//	::SHPDestroyObject(records.getRecord(i).second);
	
}


/**************************************************

get file handle directly this is a reserve function.
*****************************************************/
SHPHandle CQixShapeFileLayerDataSource::GetSHPHandle(void)
{
	ASSERT(hSHPFile != NULL);
	return hSHPFile;
}

DBFHandle CQixShapeFileLayerDataSource::GetDBFHandle(void)
{
	ASSERT(hDBFFile != NULL);
	return hDBFFile;
}

/*

do not add extention parameter!!! here

*/
bool CQixShapeFileLayerDataSource::FromFile(CString sFileName, CString& sAccessMode )
{
	// close old file first
	if (hSHPFile != NULL)
	{
		SHPClose(hSHPFile);
		hSHPFile = NULL;
	}

	if (hDBFFile != NULL)
	{
		DBFClose( hDBFFile );
		hDBFFile = NULL;
	}

	//open shape file 
	
	/*************************************
	The fopen() style access string.
	At this time only "rb" (read-only binary) and "rb+" (read/write binary) 
	should be used.
	**************************************/
	if ( (sAccessMode.CompareNoCase(_T("rb")) != 0 ) 
		&&  (sAccessMode.CompareNoCase(_T("rb+")) != 0) 
		)
	{
		VERIFY(false);
		return false;
	}
	
	// open the shape file 
	//CSTringA ANSI unicode !!!!!!!!!
	hSHPFile = ::SHPOpen((const char*)LPCTSTR(sFileName), (const char*)LPCTSTR(sAccessMode) );


	if( hSHPFile == NULL ) // if failed
	{
		VERIFY(false);
		LOG( "ERROR: Unable to open SHP :%s \n ", sFileName );
		return false;
	}

	//logObj.writeToLog(_INFO, "Opening dbf file!");

	// open the DBF file 
	hDBFFile = ::DBFOpen((const char*)LPCTSTR(sFileName), (const char*)LPCTSTR(sAccessMode) ) ;
	if( hDBFFile == NULL ) // if failed
	{
		VERIFY(false);
		LOG( "ERROR: Unable to open DBF :%s \n ", sFileName );
		return false;
	}

	//logObj.writeToLog(_INFO, "after open dbf");
	
	return true;
}


// set the extend 

bool CQixShapeFileLayerDataSource::SetExtend(CDoubleRect rect)
{
	//??? warning Be carefull with the operator = 
	rectExtend = rect;
	
	return true;
}

CDoubleRect CQixShapeFileLayerDataSource::GetExtend()
{
	return this->rectExtend;
}



long CQixShapeFileLayerDataSource::GetRecoderCount(void)
{
	// TODO: add QIX Search here.

	//int		nShapeType = 0, nSHPEntities = 0;
	//double 	adfMinBound[4] = {0}, adfMaxBound[4] = {0}; // to store boundary box 

	/////*  get information of shape file.*/
	//SHPGetInfo( hSHPFile, &nSHPEntities, &nShapeType, adfMinBound, adfMaxBound );
	////
	//long nDBFEntities = DBFGetRecordCount(hDBFFile);

	//if (nDBFEntities < nSHPEntities )
	//{
	//	ASSERT(false); // should be deleted, only for temp use
	//	LOG("ERROR: %s \n", "the record count of SHP file lager than the record count of DBF file! ");

	//}
	//ASSERT( nSHPEntities ==  (getShapeRecords()).size());// from mike 
	return (long)(getShapeRecords().size()); // only return the record of shape!
}

/*
	int		nShapeType = 0, nEntities = 0, nFieldIndexSrc = 0;
	SHPObject*	psObject = NULL; // shape object
	double* padPointX = NULL; // to store the points' abscissa value
	double* padPointY = NULL; // to store the points' ordinate value

	double 	adfMinBound[4] = {0}, adfMaxBound[4] = {0}; // to store boundary box 



*/
ShapeUtility::ShapeType CQixShapeFileLayerDataSource::getShapeType(void)
{

	int		nShapeType = 0, nEntities = 0;
	double 	adfMinBound[4] = {0}, adfMaxBound[4] = {0}; // to store boundary box 


	/*  get information of shape file.*/
	//SHPGetInfo( hSHPFile, &nEntities, &nShapeType, adfMinBound, adfMaxBound );
	nShapeType = records.getShpType();
	   switch ( nShapeType )
      {
         case SHPT_NULL:
			return ShapeUtility::unknown;

         case SHPT_POINT:
			 return ShapeUtility::point;			

         case SHPT_ARC:
			 return ShapeUtility::arc;
            
		case SHPT_POLYGON:
			return ShapeUtility::polygon;
            
         default:
			 return ShapeUtility::unknown;
      }

	   return ShapeUtility::unknown;
}


	//bool CQixShapeFileLayerDataSource::MergeVertices( int i, int nCount,
	//	double dX, double dY, 
	//	double* pdFirstX, double* pdFirstY, 
	//	double* pdLastX,  double* pdLastY, 
	//	CDoubleRect rect, CShape*  pShape)
	//{	
	//	if( (*pdFirstX == 0.0) && (*pdFirstY == 0.0) )
	//	{
	//		*pdFirstX = dX;
	//		*pdFirstY = dY;

	//		pShape->m_aRectPointArray[i].dX = dX;
	//		pShape->m_aRectPointArray[i].dY = dY;
	//	}
	//	else
	//	{
	//		*pdLastX = dX;
	//		*pdLastY = dY;
	//		if(i + 1 <= nCount)
	//		{
	//			if(pShape->m_aPointArray[i+1].dX < rect.dX1)
	//			{
	//				//continue;
	//			}
	//			else 
	//			{
	//				pShape->m_aRectPointArray[i].dX = *pdLastX;
	//				pShape->m_aRectPointArray[i].dY = *pdLastY;
	//				*pdFirstX = 0.0;
	//				*pdFirstY = 0.0;
	//			}
	//		}
	//	}

	//	return true;

	//}

	//bool CQixShapeFileLayerDataSource::ReduceOuterPoints(CDoubleRect rect, CShape*  pShape)
	//{
	//	//CShape* pShape = Shape;

	//	int nCount   = (int) pShape->m_aPointArray.GetCount();

	//	double dX    = 0.0;
	//	double dY    = 0.0;

	//	double dFirstLeftX   = 0.0;
	//	double dFirstLeftY   = 0.0;
	//	double dLastLeftX    = 0.0;
	//	double dLastLeftY    = 0.0;

	//	double dFirstRightX  = 0.0;
	//	double dFirstRightY  = 0.0;
	//	double dLastRightX   = 0.0;
	//	double dLastRightY   = 0.0;


	//	double dFirstUpperX  = 0.0;
	//	double dFirstUpperY  = 0.0;
	//	double dLastUpperX   = 0.0;
	//	double dLastUpperY   = 0.0;

	//	double dFirstBottomX = 0.0;
	//	double dFirstBottomY = 0.0;
	//	double dLastBottomX  = 0.0;
	//	double dLastBottomY  = 0.0;

	//	for(int i=0; i < nCount; i++)
	//	{
	//		dX = pShape->m_aPointArray[i].dX;
	//	    dY = pShape->m_aPointArray[i].dY;

	//		if( dX < rect.dX1 )
	//		{

	//			MergeVertices( i, nCount, dX, dY, 
	//				           &dFirstLeftX, &dFirstLeftY, 
	//				           &dLastLeftX,  &dLastLeftY, rect, pShape );

	//		}
	//		else if( dX > rect.dX2)
	//		{
	//			MergeVertices( i, nCount, dX, dY, 
	//				&dFirstRightX, &dFirstRightY, 
	//				&dLastRightX,  &dLastRightY, rect, pShape );
	//		}
	//		else
	//		{
	//			if( dY < rect.dY1 )
	//			{				
	//				MergeVertices( i, nCount, dX, dY, 
	//					&dFirstUpperX, &dFirstUpperY, 
	//					&dLastUpperX,  &dLastUpperY, rect, pShape );

	//			}
	//			else if( dY > rect.dY2)
	//			{
	//				MergeVertices( i, nCount, dX, dY, 
	//					&dFirstBottomX, &dFirstBottomY, 
	//					&dLastBottomX,  &dLastBottomY, rect, pShape );
	//			}
	//			else//quad
	//			{
	//				pShape->m_aRectPointArray[i].dX = dX;
	//				pShape->m_aRectPointArray[i].dY = dY;
	//			}
	//		}
	//		//return true;
	//	}
	//	return true;
	//}



/*
this is a Wrap for
SHPObject *SHPReadObject( SHPHandle hSHP, int iShape );
be carefull with the new object !!!!!!!!!!!
*/
 CShape* CQixShapeFileLayerDataSource::getRecord(long lRecord, CStringArray& aPropertyFields)
{
	// TODO: add QIX Search here.
	
	SHPObject*	psObject = NULL; // shape object
	double* padPointX = NULL; // to store the points' abscissa value
	double* padPointY = NULL; // to store the points' ordinate value




	//psObject = SHPReadObject( hSHPFile, lRecord );
	pair<int, SHPObject*> shpPair;
	shpPair = records.getRecord(lRecord);
	psObject = shpPair.second;
	int idx = shpPair.first;

	if (psObject == NULL)
	{
		return NULL;
	}

	//TODO: verify the type part is ???

	////////////////////////
	/*
	;//here is very very dangrous!!! 
	flexibility here is very important, here here we have a hard code of class.

	should be a "class factory" mechanism
	or a swith case statement.
	*/
	CShape*  pShape = NULL;

	switch ( getShapeType() )
      {
         case ShapeUtility::unknown:
			VERIFY(false);
			pShape = NULL;
			//LOG ..... impossible!!!
			break ;

		 case ShapeUtility::point:
			 pShape = new CPointShape; 
			break;

		 case ShapeUtility::arc:
			pShape = new CLineShape;
			break;
            
		 case ShapeUtility::polygon:
			 pShape = new CPolygonShape;
			break;
            
         default:
			VERIFY(false);
			pShape = NULL;
			//LOG ..... impossible!!!
			break;
      }

	

	if (pShape == NULL)// if failed to new objcet.
	{
		LOG("ERROR: %s \n", "insufficent memery occured, when allocate memery for shape object");
	
		::SHPDestroyObject(psObject);
		//delete pShape; ///!!!!!!!!!!!!!
		return pShape = NULL;
	}

	////////////////////////////////////////////////////////////////////
	// read Shape file
	/////////////////////////////////////////////////////////////////////

	pShape->m_aPointArray.SetSize(psObject->nVertices); // this is important for perfomance if set size first
	




	//TODO: add field to properties!!
	for( int iVertex = 0; iVertex < psObject->nVertices; iVertex++ )
	{
		pShape->m_aPointArray[iVertex].dX = psObject->padfX[iVertex];
		pShape->m_aPointArray[iVertex].dY = psObject->padfY[iVertex];
	}

	/*if( psObject->nParts < 1 )
	{
		VERIFY(false);
		return NULL;
	}*/

	/*if ( *( psObject->panPartStart ) != 0)
	{
		VERIFY(false);
		return false;
	}*/



	///////////////////////////////////////////////
	//pShape->m_aRectPointArray.SetSize(pShape->m_aPointArray.GetCount());
	//pShape->ReduceOuterPoints(this->GetExtend());
	///////////////////////////////////////////////



	pShape->m_nParts = psObject->nParts;
	pShape->m_aPartsStartingIndex.SetSize( psObject->nParts );


	for (int i = 0; i < (psObject->nParts); i++ )
	{
		pShape->m_aPartsStartingIndex[i] = *( psObject->panPartStart + i);
	}
	//pShape->nStartNum = psObject->panPartStart;
	
	/*int a = *pShape->nStartNum;
	int b = *(pShape->nStartNum+1); */

	//::SHPDestroyObject(psObject);//cause memory leak

	////////////////////////////////////////////////////////////////////
	// reand DBF file
	/////////////////////////////////////////////////////////////////////
	const char* pszFieldValue = NULL;

	long nFieldCount = (long)aPropertyFields.GetCount();

	pShape->m_aProperties.SetSize(nFieldCount);
	
	//get all of the columns from one DBF file
	for(int i = 0; i < nFieldCount; i++ )
	{

		// get field index of DBF file
		int nFieldIndexSrc = DBFGetFieldIndex(hDBFFile, (const char*)LPCTSTR(aPropertyFields[i]));
		

		if ( nFieldIndexSrc == -1 ) 
		{
			//logObj.varWriteToLog(_WARNING, "sss", "there is no ", aPropertyFields[i]," field in the DBF File");
			//...TODO
			pszFieldValue = "";// or assign to NULL ???
			//ASSERT(false);// should be deleted!!! only for temp using
		}
		else
		{

			pszFieldValue = DBFReadStringAttribute( hDBFFile, idx, nFieldIndexSrc);
		}

		pShape->m_aProperties[i] = CString(pszFieldValue);
	}


	return pShape;
}

//bool CQixShapeFileLayerDataSource::GetRecordFromMySQL(CShapePtrArray& apShapes)
//{
//
//	return true;
//}