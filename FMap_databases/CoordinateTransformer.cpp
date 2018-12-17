#include "StdAfx.h"
#include "shapeObject.h"
#include "shapeLayer.h"
#include "CoordinateTransformer.h"

CCoordinateTransformer::CCoordinateTransformer(void)
{
	m_pLinearTransformer = NULL;
	hasLatLong = false;
	hasQuadKey = false;
}

CCoordinateTransformer::~CCoordinateTransformer(void)
{
	ClearTransformer();	
}


void CCoordinateTransformer::ClearTransformer(void)
{
	if (m_pLinearTransformer != NULL)
	{
		delete m_pLinearTransformer;
		m_pLinearTransformer = NULL;
	}

}



//// convet a shape layer' coordinate 
//bool CCoordinateTransformer::Convert(CShape* pShape)
//{
//	m_pLinearTransformer->DataToMap(&pShape->m_aPointArray);
//	m_pLinearTransformer->DataToMap(&pShape->m_aRectPointArray);
//
//	return true;
//}

// convet a shape layer' coordinate 
bool CCoordinateTransformer::Convert(CShape* pShape)
{
	//if( !hasLatLong ) //UTM system
	//{
		m_pLinearTransformer->DataToMap(&pShape->m_aPointArray);
		m_pLinearTransformer->DataToMap(&pShape->m_aRectPointArray);
	//}
	//else //Lat, Long system to MS tail system
	//{
	//	m_pLinearTransformer->LatLongDataToMap(&pShape->m_aPointArray);
	//	m_pLinearTransformer->LatLongDataToMap(&pShape->m_aRectPointArray);
	//}

	return true;
}

//for MS coordinates convert
bool CCoordinateTransformer::Convert(CShape* pShape, int nlevel, int nStartX, int nStartY)
{
	if( !hasLatLong ) //UTM system
	{
		m_pLinearTransformer->DataToMap(&pShape->m_aPointArray);
		m_pLinearTransformer->DataToMap(&pShape->m_aRectPointArray);
	}
	else //Lat, Long system to MS tail system
	{
		m_pLinearTransformer->LatLongDataToMap(&pShape->m_aPointArray, nlevel, nStartX, nStartY);
		m_pLinearTransformer->LatLongDataToMap(&pShape->m_aRectPointArray, nlevel, nStartX, nStartY);
	}

	return true;
}

bool CCoordinateTransformer::Convert(CShape* pShapeSrc/*in*/,CShape* pShapeDest/*out*/)
{

	ASSERT(false);// not implemented!!
	return false;
}



void CCoordinateTransformer::SetSourceDestCoordinateSys(CoordinateSystem sourceCoordinate, CoordinateSystem destCoordinate)
{
	 m_sourceCoordinate = sourceCoordinate;
	 m_destCoordinate = destCoordinate;
	
	ClearTransformer();
	
	// if linear convertion required new a transfer.
	if(m_sourceCoordinate == LINEAR_VIEW || m_destCoordinate == LINEAR_VIEW)
	{
		m_pLinearTransformer = new CTransformer();
		if (m_pLinearTransformer == NULL)
		{
			ASSERT(false);
			LOG("FATAL ERROR: Memory insufficent!! while allocating for linear transformer");
			exit(10);
		}
	}
}



//SetMappingExtMode
/*
bool SetMappingExtMode(double dX1MapView, double dY1MapView, double dX2MapView, double dY2MapView,
		double dX1DataView, double dY1DataView, double dX2DataView, double dY2DataView,
		bool bModeLeftToRight , bool bModeTopToBottom );

*/


void CCoordinateTransformer::SetLinearConvertParameters(
										double dX1SourceExt, // generally should be Data View
										double dY1SourceExt, 
										double dX2SourceExt, 
										double dY2SourceExt,
										double dX1DestExt, // generally should be Map View
										double dY1DestExt, 
										double dX2DestExt, 
										double dY2DestExt,
										bool bModeLeftToRight,// = true by default
										bool bModeTopToBottom// = false by default
										)
{
	
	VERIFY(m_pLinearTransformer != NULL); 
	if (m_pLinearTransformer == NULL)
	{
		LOG("ERROR: [SetLinearConvertParameters] was called before [SetSourceDestCoordinate]");//should not
		VERIFY(false);
	}
	
	
	m_pLinearTransformer->SetMappingExtMode(
										 dX1DestExt, //  should be Map View
										 dY1DestExt, 
										 dX2DestExt, 
										 dY2DestExt,
										 dX1SourceExt, //  should be Data View
										 dY1SourceExt, 
										 dX2SourceExt, 
										 dY2SourceExt,
										 bModeLeftToRight,// = true by default
										 bModeTopToBottom// = false by default
										);
	
}
void CCoordinateTransformer::SetLinearConvertParameters(
										CDoubleRect rectSourceExt,// generally should be Data View
										CDoubleRect rectDestExt,//	generally should be Map View	 
										bool bModeLeftToRight,// = true by default
										bool bModeTopToBottom// = false by default
										)
{


	SetLinearConvertParameters(	rectSourceExt.dX1,
								rectSourceExt.dY1,
								rectSourceExt.dX2,
								rectSourceExt.dY2,
								rectDestExt.dX1,
								rectDestExt.dY1,
								rectDestExt.dX2,
								rectDestExt.dY2,
								bModeLeftToRight,// = true by default
								bModeTopToBottom // = false by default);
								);
}



bool CCoordinateTransformer::Convert(CShapeLayer* shapeLayer)
{
	CShape* pShape = NULL;

	int nCount = shapeLayer->getShapeObjectCount();

	for(int i = 0; i <nCount; i++)
	{
		pShape = shapeLayer->getShapeObject(i);

		if (pShape == NULL)
		{
			ASSERT("false");

		}	
		
		Convert(pShape);		
	}

	return true;
}

//for MS coordinates layer convert
bool CCoordinateTransformer::Convert(CShapeLayer* shapeLayer, int nlevel, int nStartX, int nStartY)
{
	CShape* pShape = NULL;

	int nCount = shapeLayer->getShapeObjectCount();

	for(int i = 0; i <nCount; i++)
	{
		pShape = shapeLayer->getShapeObject(i);

		if (pShape == NULL)
		{
			ASSERT("false");

		}	
		
		Convert(pShape, nlevel, nStartX, nStartY);		
	}

	return true;
}
