#pragma once
#include ".\\Utility\\Transformer.h"
//#include "..\utility\transformer.h"

class CShapeLayer;
class CShape;

class CCoordinateTransformer
{

public:
	CCoordinateTransformer(void);
public:
	virtual ~CCoordinateTransformer(void);




public:
	/*
	here we only hard code the Coordinate system parameter, meet our current
	system requirement and simplify the arithmetic.
	*/
		enum CoordinateSystem {
		UTM,
		GCS,//GCS_Assumed_Geographic_1 longitude, latitude system.
		LINEAR_VIEW//linear System. Should we put here or not?
	};
public:
	CoordinateSystem m_sourceCoordinate;
	CoordinateSystem m_destCoordinate;

	bool hasLatLong;

public:

	
//private:
	// linear transformer system object.
	CTransformer* m_pLinearTransformer;
	
public:
	
	bool Convert( CShape* pShape );
	//for MS coordinates convert
	bool Convert(CShape* pShape, int nlevel, int nStartX, int nStartY);
	bool Convert( CShape* pShapeSrc/*in*/,CShape* pShapeDest/*out*/ );


	// convet a shape layer' coordinate 	
	bool Convert( CShapeLayer* shapeLayer );
	//for MS coordinates layer convert
	bool Convert(CShapeLayer* shapeLayer, int nlevel, int nStartX, int nStartY);

	void SetLatLong(bool latlong)
	{
		this->hasLatLong = latlong;
	}


public:
	void SetSourceDestCoordinateSys(CoordinateSystem sourceCoordinate, CoordinateSystem destCoordinate);


	void SetLinearConvertParameters(
					CDoubleRect rectSourceExt,// generally should be Data View
					CDoubleRect rectDestExt,//	generally should be Map View	 
					bool bModeLeftToRight = true,   // = true by default
					bool bModeTopToBottom = false	// = false by default
					);

	void SetLinearConvertParameters( 
					double dX1MapView, 
					double dY1MapView, 
					double dX2MapView, 
					double dY2MapView,
					double dX1DataView,
					double dY1DataView, 
					double dX2DataView, 
					double dY2DataView,
					bool bModeLeftToRight  = true, 
					bool bModeTopToBottom = false );
	void ClearTransformer(void);
};


/*
Example

Coordinate system
name:GCS_Assumed_Geographic_1
Alias: 
Abbreviation: 
Remarks: 
Angular Unit: Degree (0.017453292519943295)
Prime Meridian: Greenwich (0.000000000000000000)
Datum: D_North_American_1927
Spheroid: Clarke_1866
Semimajor Axis: 6378206.400000000400000000
Semiminor Axis: 6356583.799998980900000000
Inverse Flattening: 294.978698200000000000





*/
