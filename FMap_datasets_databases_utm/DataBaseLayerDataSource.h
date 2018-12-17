#pragma once

//#define CPPCONN_PUBLIC_FUNC

#include "layerdatasource.h"
#include "MySQL_Connector/mysql_connection.h"
#include "MySQL_Connector/mysql_driver.h"
#include "MySQL_Connector/resultset.h"
#include "MySQL_Connector/connection.h"
#include "MySQL_Connector/statement.h"
#include "configure.h"
#include "MySQL_Connector/exception.h"
#include "ShapeLayer.h"
//#include "shapeObject.h"
#include "fmap.h"
#include "WKTManager.h"
#include "MapUtility.h"

//#import "C:\Program files\Common Files\System\Ado\msado15.dll" rename("EOF", "ADOEOF")
#import "msado15.dll" rename("EOF", "ADOEOF")
//#import "msado15.dll" no_namespace rename("EOF", "EndOfFile")

//#include "PostGIS/libpq-fe.h"




extern Log logObj;


//class CShape;


class CDataBaseLayerDataSource :
	public CLayerDataSource
{
public:

	CDataBaseLayerDataSource(void);

	CDoubleRect rectExtend;
	std::string str_Table, str_Field, str_Quadkey, str_source, str_date;

	CStringList geometrylist;

	bool isLODData;
	bool hasFAOwner, hasFAADDR;
	void setFAOwner(bool faowner)
	{
		this->hasFAOwner = faowner;
	}
	void setFAADDR(bool faaddr)
	{
		this->hasFAADDR = faaddr;
	}


	//MySQL connection
	sql::mysql::MySQL_Driver *driver;
	sql::Connection	*con;
	sql::Statement	*stmt;
	sql::ResultSet  *res;
	sql::ResultSet  *countres;

	std::string str_LODLevel;
	int nlevel;
	int nDataBaseType, nLayerType;



	//SQL Server connection
	//smart pointer
	ADODB::_ConnectionPtr pConnection_sqlserver;
	_bstr_t strCnn_sqlserver;
	ADODB::_RecordsetPtr prs_sqlserver;


	//PostGIS connection
	/*const char *pconninfo_postgis;
	PGconn     *pconn_postgis;
	PGresult   *pres_postgis;*/

	
	



public:
	virtual ~CDataBaseLayerDataSource(void);


public:
	// basic interface of the datasource that should be implemented 
	virtual long GetRecoderCount(void) ;
	virtual ShapeUtility::ShapeType getShapeType(void) ;
	virtual CShape* getRecord(long lRecord, CStringArray& aPropertyFields);

		//sql::Connection	*con;
	//sql::Statement	*stmt;
	//sql::ResultSet  *res;
	bool GetRecordFromMySQL(CShapePtrArray& apShapes);
	bool GetRecordFromSQLServer(CShapePtrArray& apShapes);
	//bool GetRecordFromPostGIS(CShapePtrArray& apShapes)
	bool GetRecordFromDatabase(CShapePtrArray& apShapes);
	std::string GetRectExtWKTPolygon();






	bool SetTable(std::string strTable);
	bool SetField(std::string str_Field);
	bool SetLayerType(int ntype);
	bool SetQuadkey(std::string strQuadkey);
	bool SetSource(std::string source);
	bool SetDate(std::string date);
	std::string GetTable();
	virtual bool SetExtend(CDoubleRect rect);
	CDoubleRect GetExtend();




	sql::ResultSet* QueryMySQL(std::string table, std::string field);
	bool QuerySQLServer(std::string table, std::string field, bool isQueryFullyInsideTile);
	//bool QueryPostGIS(std::string table, std::string field);
    //bool GetRecordsFromWKT(CShapePtrArray& apShapesArray, CString str_res, CString str_name);
	bool GetRecordsFromWKT(CShapePtrArray& apShapesArray, CString str_res, 
		CString str_name, CString str_color);

	bool SetLODLevel(std::string strlevel);
	bool SetLODLevel(int level, std::string strlevel);

	bool SetDatabaseType(int nDBType);

	//for SQL Server
	inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};


    bool OpenDatabase();
	bool OpenMySQLDB();
	bool OpenSQLServerDB();
	//bool OpenPostGISDB();


	bool CloseDatabase();
	bool CloseMySQLDB();
	bool CloseSQLServerDB();
	//bool ClosePostGISDB();

	//void exit_nicely_PostGIS(PGconn * conn){PQfinish(conn);exit(1);}




};
