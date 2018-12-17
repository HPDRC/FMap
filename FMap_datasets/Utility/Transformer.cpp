//#include "StdAfx.h"

#include "Transformer.h"

CTransformer::CTransformer(void)
{
	m_dX1MapExt = m_dY1MapExt = m_dX2MapExt = m_dY2MapExt = m_dX1DataExt= m_dY1DataExt=m_dX2DataExt=m_dY2DataExt =0;
	bModeLeftToRight = true;
	bModeTopToBottom = false;
	double dRatioX = 1;
	double dRatioY = 1;

}

CTransformer::~CTransformer(void)
{
}


 double CTransformer::MapToDate_X(double dXMap)//map to latitude 
{
	return ( ((dXMap - m_dX1MapExt) / dRatioX) + m_dX1DataExt );
}

 double CTransformer::MapToDate_Y(double dYMap)//map to Longitude
{
	return ( ((dYMap - m_dY1MapExt) / dRatioY) + m_dY1DataExt );
}

 void CTransformer::GDCToUTM(double &dGdcX, double &dGdcY)//GDC coordinates to UTM coordinates
 {	
	 Gdc_Coord_3d gdc(dGdcX, dGdcY, 0);
	 Utm_Coord_3d utm(0, 0, 0, 0, true);

	 Gdc_To_Utm_Converter::Init();
	 Gdc_To_Utm_Converter::Convert(gdc,utm);

	 dGdcX = utm.x;
	 dGdcY = utm.y;
 }

bool CTransformer::SetMappingExtMode(double dX1MapExt,
									 double dY1MapExt, 
									 double dX2MapExt, 
									 double dY2MapExt,
									 double dX1DataExt, 
									 double dY1DataExt, 
									 double dX2DataExt, 
									 double dY2DataExt,
									 bool bModeLeftToRight = true, 
									 bool bModeTopToBottom = false)
{

		this->m_dX1MapExt 	=	dX1MapExt; 
		this->m_dY1MapExt 	=	dY1MapExt; 
		this->m_dX2MapExt 	=	dX2MapExt; 
		this->m_dY2MapExt 	=	dY2MapExt; 
		this->m_dX1DataExt	=	dX1DataExt;
		this->m_dY1DataExt	=	dY1DataExt;
		this->m_dX2DataExt	=	dX2DataExt;
		this->m_dY2DataExt	=	dY2DataExt;

		VERIFY( (m_dX2DataExt - m_dX1DataExt) != 0 );
		VERIFY( (m_dY2DataExt - m_dY1DataExt) != 0 );

		dRatioX = ( m_dX2MapExt - m_dX1MapExt)/(m_dX2DataExt - m_dX1DataExt);
		dRatioY = ( m_dY2MapExt - m_dY1MapExt)/(m_dY2DataExt - m_dY1DataExt);


	return true;
}

void CTransformer::LatLongToPixelXY(double latitude, double longitude, int levelOfDetail, int* pPixelX, int* pPixelY , int nStartX, int nStartY)
{

	double	MinLatitude = -85.05112878;
	double  MaxLatitude = 85.05112878;
	double  MinLongitude = -180;
	double  MaxLongitude = 180;
	/*double	MinLatitude = drect.dY1;
	double  MaxLatitude = drect.dY2;
	double  MinLongitude = drect.dX1;
	double  MaxLongitude = drect.dX2;*/

	latitude = Clip(latitude, MinLatitude, MaxLatitude);
	longitude = Clip(longitude, MinLongitude, MaxLongitude);

	double x = (longitude + 180) / 360; 
	double sinLatitude = sin(latitude * PI / 180);
	double y = 0.5 - log((1 + sinLatitude) / (1 - sinLatitude)) / (4 * PI);

	int mapSize = MapSize(levelOfDetail);
	//int mapSize = 512;
	*pPixelX = (int) Clip(x * mapSize + 0.5, 0, mapSize - 1) - nStartX;
	*pPixelY = (int) Clip(y * mapSize + 0.5, 0, mapSize - 1) - nStartY;
}
