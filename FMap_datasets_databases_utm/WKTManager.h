#pragma once


#include "maputility.h"
//#include "shapeObject.h"
#include "ShapeLayer.h"


extern Log logObj;

class CWKTManager
{

public:

	int m_polygonParts;
	CIntegerArray m_polygonPartsStartingIndex;
	int nDBType;

public:
	CWKTManager(void);

	~CWKTManager(void);


public:
	CString RemoveWKTStringName(CString str, int ipos);

	CString RemoveWKTParentheses(CString str);

	CString RemoveWKTLeftDoubleParentheses(CString str);

	CString SplitCString(CString str, CString str_Token, int* icurpos, int *inextpos);

	int CountComma(CString str);

	int CountPolygonParts(CShape& pShpObj, CString str);

	bool GetWKTCoordinates(CString res, double& dX, double& dY);

	//CShape* GetWKTPoint(CString res, int* ipt, CString name);

	//CShape* GetWKTLineString(CString res, int* ipt, CString name);

	//CShape* GetWKTPolygon(CString res, int* ipt, CString name);

	////bool GetWKTMultiPolygon(CShapePtrArray& apShapesArray, CString res, int* ipt, CString name);
	//CShape* GetWKTMultiPolygon(CString res, int* ipt, CString name);

	//bool GetWKTMultiPolygon(CShapePtrArray& apShapesArray, CString res, int* ipt, CString name);

	//bool GetWKTMultiLineString(CShape& ashpobj, CString res, int* ipt, CString name);







	CShape* GetWKTPoint(CString res, CString name);

	CShape* GetWKTLineString(CString res, CString name);

	CShape* GetWKTPolygon(CString res, CString name);

	CShape* GetWKTPolygon(CString res, CString name, CString color);

	//bool GetWKTMultiPolygon(CShapePtrArray& apShapesArray, CString res, int* ipt, CString name);
	CShape* GetWKTMultiPolygon(CString res, CString name);

	CString GetGeometryCollectionDelimitor(int ntype);

	int GetTokenlength(CString str, int ntype);

	bool GetWKTGeometryCollection(CShapePtrArray& apShapesArray, CString name, CStringList* strlist, int ntype);

	bool GetWKTGeometryCollectionStringList(CString str_res, CStringList* strlist);

	//bool GetWKTMultiPolygon(CShapePtrArray& apShapesArray, CString res, CString name);

	//CShape* GetWKTMultiLineString(CString res, CString name);
	bool GetWKTMultiLineString(CShapePtrArray& apShapesArray, CString res, CString name);

	bool GetWKTMultiPoint(CShapePtrArray& apShapesArray, CString res, CString name);

	bool SetDatabaseType(int ntype);


	CString GetLCRDelimitor(int ntype);//get "),(" as delimitor





};
