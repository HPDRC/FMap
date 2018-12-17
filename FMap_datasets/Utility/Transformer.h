#pragma once

#include "..\maputility.h"
#include "..\two_convert.h"
#include "Utility.h"
#include "..\DoubleRect.h"

class CTransformer
{
public:
	CTransformer(void);
	~CTransformer(void);

public:
	// for points convertion
	double  MapToDate_X(double dXData);//latitude to map
	double  MapToDate_Y(double dYData);//Longitude to map
	void GDCToUTM(double &dGdcX, double &dGdcY);

	double DateToMap_X(double dXData)//latitude to map
	{

		if (bModeLeftToRight) 
		{
			return ( dXData - m_dX1DataExt )* dRatioX + m_dX1MapExt;	
		}
		else
		{
			return m_dX2MapExt - ( dXData - m_dX1DataExt )* dRatioX;	
		}

	}

	double DateToMap_Y(double dYData)//Longitude to map
	{
		if (bModeTopToBottom) 
		{
			return ( dYData - m_dY1DataExt )* dRatioY + m_dY1MapExt;	
		}
		else
		{
			return m_dY2MapExt - ( dYData - m_dY1DataExt )* dRatioY;	
		}

	}


public:
	 // convert a series of Vertices
	 void DateToMap(int nVertices, double* pdXData, double* pdYData,double* pdXMap, double* pdYMap )//
	 {

		 if (nVertices == 0)
		 {
			 pdXMap = pdYMap = NULL;
			 return;
		 }

		 VERIFY( pdXData != NULL ); 
		 VERIFY( pdYData != NULL );


		 for(int i = 0; i < nVertices; i++)
		 {
			*(pdXData + i) = DateToMap_X( *(pdXData + i));
			*(pdYData + i) = DateToMap_Y( *(pdYData + i));

		 }


	 };
	 void  MapToDate(int nVertices,double* pdXMap,double* pdYMap,double* pdXData,double* pdYData )//
	 {
		 ASSERT(false);;
	 };

	 void  DataToMap(CPointArray* paPointArray)//
	 {
		 
		 int nCount = (int) paPointArray->GetCount();//!!!!??????
		 

		 for( int i = 0; i < nCount; i++ )
		 {
				//Convert X
			 	if (bModeLeftToRight) 
				{
					(*paPointArray)[i].dX = ( (*paPointArray)[i].dX - m_dX1DataExt )* dRatioX + m_dX1MapExt;	
				}
				else
				{
					(*paPointArray)[i].dX =  m_dX2MapExt - ( (*paPointArray)[i].dX - m_dX1DataExt )* dRatioX;	
				}

				// convert Y
				if (bModeTopToBottom) 
				{
					(*paPointArray)[i].dY = ( (*paPointArray)[i].dY- m_dY1DataExt )* dRatioY + m_dY1MapExt;	
				}
				else
				{
					(*paPointArray)[i].dY = m_dY2MapExt - ( (*paPointArray)[i].dY - m_dY1DataExt )* dRatioY;	
				}
		 }
		 
	 };



	 void  MapToData(CPointArray* paPointArray)//
	 {
		 //aPointArray->
		 ASSERT(false); // not implemented!!
		 
		 //int nCount = paPointArray->;
		 
	 };
	 //double  DateToMap_Y(double dYData);//Longitude to map




	bool SetMappingExtMode(double dX1MapExt, double dY1MapExt, double dX2MapExt, double dY2MapExt,
		double dX1DataExt, double dY1DataExt, double dX2DataExt, double dY2DataExt,
		bool bModeLeftToRight , bool bModeTopToBottom );


private:
	double m_dX1MapExt, m_dY1MapExt,  m_dX2MapExt,  m_dY2MapExt;
	double m_dX1DataExt,  m_dY1DataExt,  m_dX2DataExt, m_dY2DataExt;
	bool bModeLeftToRight, bModeTopToBottom;
	double dRatioX; 
	double dRatioY;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//Lat/Long System for MS Tail System
	 double Clip(double n, double minValue, double maxValue)
	 {
		 return min(max(n, minValue), maxValue);
	 }

	 int MapSize(int levelOfDetail)
	 {
		 return (int) 256 << levelOfDetail;
	 }

	 void LatLongToPixelXY(double latitude, double longitude, int levelOfDetail, int* pPixelX, int* pPixelY, int nStartX, int nStartY);

	 //lat/long to MS tail system
	 void LatLongDataToMap( CPointArray* paPointArray, int nlevel/*, CDoubleRect rect*/ , int nStartX, int nStartY)
	 {
		 int nCount = (int) paPointArray->GetCount();

		 for( int i = 0; i < nCount; i++ )
		 {

			 int nPixelX = 0;
			 int nPixelY = 0;
			 LatLongToPixelXY( (*paPointArray)[i].dY, (*paPointArray)[i].dX, nlevel, &nPixelX, &nPixelY/*, rect*/, nStartX, nStartY);
			 //Convert X of lat/long to MS Tail System
			 (*paPointArray)[i].dX = nPixelX;
			 //Convert Y of lat/long to MS Tail System
			 (*paPointArray)[i].dY = nPixelY;

		 }


	 }
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
};
