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
using namespace std;

extern Log logObj;

CLayerDataSource::CLayerDataSource(void)
{
}

CLayerDataSource::~CLayerDataSource(void)
{
	
}

/**************************************************

get file handle directly this is a reserve function.
*****************************************************/

/*

do not add extention parameter!!! here

*/
// set the extend 

bool CLayerDataSource::SetExtend(CDoubleRect rectExtend)
{
	//??? warning Be carefull with the operator = 
	rectExtend = rectExtend;
	
	return true;
}



/*
	int		nShapeType = 0, nEntities = 0, nFieldIndexSrc = 0;
	SHPObject*	psObject = NULL; // shape object
	double* padPointX = NULL; // to store the points' abscissa value
	double* padPointY = NULL; // to store the points' ordinate value

	double 	adfMinBound[4] = {0}, adfMaxBound[4] = {0}; // to store boundary box 



*/

