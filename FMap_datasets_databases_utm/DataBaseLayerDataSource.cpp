#include "DataBaseLayerDataSource.h"

CDataBaseLayerDataSource::CDataBaseLayerDataSource(void)
{


	/*if( !this->OpenDatabase(this->nDataBaseType) )
	{
		logObj.writeToLog(_ERROR, "openDatabase");
	}*/
	//this->OpenDatabase(this->nDataBaseType);

	//try
	//{
	//	//Connect to MySQL
	//	//sql::ResultSet  *res;
	//	driver = sql::mysql::get_mysql_driver_instance();
	//	//con = driver->connect("tcp://localhost:3306", "fmap", "iloveme");
	//	//con = driver->connect("tcp://localhost:3306", "root", "iloveme");
	//	con = driver->connect(MYSQL_SERVER, MYSQL_USERNAME, MYSQL_PASSWORD);
	//	//MySQL query
	//	stmt = con->createStatement();
	//}
	//catch(sql::SQLException &e)
	//{
	//	logObj.writeToLog(_ERROR, (char*)e.what());
	//	logObj.writeToLog(_ERROR, (char*)e.getSQLState());
	//}



}





CDataBaseLayerDataSource::~CDataBaseLayerDataSource(void)
{

	if( !this->CloseDatabase() )
	{
		logObj.writeToLog(_ERROR, "closeDatabase");
	}
	//this->CloseDatabase(this->nDataBaseType);
	//try
	//{
	//	//close the connection with MySQL
	//	delete res;
	//	//delete countres;
	//	delete stmt;
	//	delete con;
	//}
	//catch(sql::SQLException &e)
	//{
	//	logObj.writeToLog(_ERROR, (char*)e.what());
	//	logObj.writeToLog(_ERROR, (char*)e.getSQLState());
	//}


}

long  CDataBaseLayerDataSource::GetRecoderCount(void)
{


	if( -1 == this->nDataBaseType )
	{
		return false;
	}

	long ltotalrecords;
	ltotalrecords = 0;


	switch(this->nDataBaseType)
	{
	case MYSQL:

		try
		{
			//this->stmt->execute("CREATE TABLE testtesttest11222(id INT);");
			this->countres = this->stmt->executeQuery("SELECT count(*) from spatialtesttest." + this->str_Table +";");
			//std::string str = countres->getString(1);
			countres->next();
			ltotalrecords = countres->getInt(1);

		}
		catch(sql::SQLException &e)
		{
			logObj.writeToLog(_ERROR, (char*)e.what());
			logObj.writeToLog(_ERROR, (char*)e.getSQLState());
		}

		break;
	case SQLSERVER:
		break;
	case POSTGIS:
		break;
	default:
		return false;
	}






	return ltotalrecords;

}

ShapeUtility::ShapeType CDataBaseLayerDataSource::getShapeType(void) 
{
	ASSERT(false);// no implemneted	
	return ShapeUtility::arc;
}



bool CDataBaseLayerDataSource::SetExtend(CDoubleRect rect)
{
	//??? warning Be carefull with the operator = 
	rectExtend = rect;
	
	return true;
}

CDoubleRect CDataBaseLayerDataSource::GetExtend()
{
	return this->rectExtend;
}

bool CDataBaseLayerDataSource::SetTable(std::string strTable)
{
	this->str_Table = strTable;
	return true;
}

bool CDataBaseLayerDataSource::SetField(std::string str_Filed)
{
	this->str_Field = str_Filed;
	return true;
}

bool CDataBaseLayerDataSource::SetLayerType(int ntype)
{
	this->nLayerType = ntype;
	return true;
}

bool CDataBaseLayerDataSource::SetSource(std::string source)
{
	this->str_source = source;
	return true;
}

bool CDataBaseLayerDataSource::SetDate(std::string date)
{
	this->str_date = date;
	return true;
}

bool CDataBaseLayerDataSource::SetQuadkey(std::string strQuadkey)
{
	//current we have 1 to 9 data only
	//int test = strQuadkey.length();
	if( strQuadkey.length() > MAX_QUADKEY_LEVEL )
	{
		strQuadkey = strQuadkey.substr(0, MAX_QUADKEY_LEVEL);
	}

	this->str_Quadkey = strQuadkey;
	return true;
}


std::string CDataBaseLayerDataSource::GetTable()
{
	return this->str_Table;
}


//void CDataBaseLayerDataSource::GetRecordFromMySQL(sql::Connection& con, sql::Statement& stmt, sql::ResultSet& res, 
//		CStringArray& aPropertyFields, CShapePtrArray& apShapes)
//{
//	//MySQL connection
//	//sql::mysql::MySQL_Driver *driver;
//	//sql::Connection	*con;
//	//sql::Statement	*stmt;
//	//sql::ResultSet  *res;
//
//
//
//
//	//driver = sql::mysql::get_mysql_driver_instance();
//	//con = driver->connect("tcp://131.94.129.181:3306", "fmap", "iloveme");
//
//
//	//MySQL query
//
//	//stmt = con->createStatement();
//	//stmt->execute("");
//
//
//	res = stmt->executeQuery("SELECT id, label FROM " + this->GetTable() +" ORDER BY id ASC");
//	while (res->next())
//	{
//
//		
//
//		//composed to new shape object
//
//		CShape*  pShape = NULL;
//
//		//adding vertices, psObject needs to be MySQL object
//		for( int iVertex = 0; iVertex < psObject->nVertices; iVertex++ )
//		{
//			pShape->m_aPointArray[iVertex].dX = psObject->padfX[iVertex];
//			pShape->m_aPointArray[iVertex].dY = psObject->padfY[iVertex];
//		}
//
//		//adding parts, psObject needs to be MySQL object
//		pShape->m_nParts = psObject->nParts;
//		pShape->m_aPartsStartingIndex.SetSize( psObject->nParts );
//
//		for (int i = 0; i < (psObject->nParts); i++ )
//		{
//			pShape->m_aPartsStartingIndex[i] = *( psObject->panPartStart + i);
//		}
//
//		//adding fields, psObject needs to be MySQL object
//		const char* pszFieldValue = NULL;
//		long nFieldCount = (long)aPropertyFields.GetCount();
//		pShape->m_aProperties.SetSize(nFieldCount);
//
//		for(int i = 0; i < nFieldCount; i++ )
//		{
//
//			// get field index of DBF file
//			int nFieldIndexSrc = DBFGetFieldIndex(hDBFFile, (const char*)LPCTSTR(aPropertyFields[i]));
//
//
//			if ( nFieldIndexSrc == -1 ) 
//			{
//				//logObj.varWriteToLog(_WARNING, "sss", "there is no ", aPropertyFields[i]," field in the DBF File");
//				//...TODO
//				pszFieldValue = "";// or assign to NULL ???
//				//ASSERT(false);// should be deleted!!! only for temp using
//			}
//			else
//			{
//
//				pszFieldValue = DBFReadStringAttribute( hDBFFile, idx, nFieldIndexSrc);
//			}
//
//			pShape->m_aProperties[i] = CString(pszFieldValue);
//		}
//
//
//
//	}
//
//
//
//
//
//
//
//
//
//
//	//return pShape;
//
//}

bool CDataBaseLayerDataSource::SetLODLevel(std::string strlevel)
{
	if( "" == strlevel )
	{
		return false;
	}

	this->str_LODLevel = strlevel;
	return true;
}

bool CDataBaseLayerDataSource::SetLODLevel(int level, std::string strlevel)
{
	if( "" == strlevel )
	{
		return false;
	}
	if( 0 > level )
	{
		return false;
	}

	if(level <= 15)
	{
		this->str_LODLevel = strlevel;
	}
	else
	{
		/*level = 15;
		char str_level[10] = {0};
		sprintf_s(str_level, "%d", level);*/

		this->str_LODLevel = "15";
	}

	this->nlevel = level;

	return true;
}


sql::ResultSet* CDataBaseLayerDataSource::QueryMySQL(std::string table, std::string field)
{

	std::string str_ltLat1, str_ltLon1, str_rbLat2, str_rbLon2, polyString, str_sql;

	std::string str_rowlimit = "50";
	//std::string str_database = "adcitest.";
	std::string str_database = DATABASE;

	str_ltLon1 = this->rectExtend.str_ExtLong1;
	str_ltLat1 = this->rectExtend.str_ExtLat1;
	str_rbLon2 = this->rectExtend.str_ExtLong2;
	str_rbLat2 = this->rectExtend.str_ExtLat2;

	polyString = str_rbLon2 + " " + str_ltLat1 + " , " + str_ltLon1 + " " +
		str_ltLat1 + " , " + str_ltLon1 + " " + str_rbLat2 + ", " + str_rbLon2 +
		" " + str_rbLat2 + ", " + str_rbLon2 + " " + str_ltLat1;

	//str_sql =  "select " + field + ", astext(shape) from spatialtesttest." + table + " Where MBRIntersects(shape,GeomFromText('Polygon((" + polyString + "))'));";
	//str_sql =  "select " + field + ", astext(shape) from spatialtesttest." + table + " Where MBRIntersects(shape,GeomFromText('Polygon((" + polyString + "))')) limit 2;";


	//str_sql =  "select " + field + ", astext(shape) from adcitest." + table + " where cntry_name = 'Russia';";

	//str_sql = "select " + field + ", " + str_database + table + "_general_" + this->str_LODLevel + ".LonLat_Coordinate from " + str_database + table + " right join " + str_database + table + "_general_" + this->str_LODLevel + " on " + str_database + table + ".OGR_FID=" + str_database + table + "_general_" + this->str_LODLevel + ".OGR_FID where MBRIntersects(" + str_database + table + ".shape,GeomFromText('Polygon((" + polyString + "))')) limit " + str_rowlimit + ";";
	//str_sql = "select " + field + ", " + str_database + table + "_general_" + this->str_LODLevel + ".LonLat_Coordinate from " + str_database + table + " right join " + str_database + table + "_general_" + this->str_LODLevel + " on " + str_database + table + ".OGR_FID=" + str_database + table + "_general_" + this->str_LODLevel + ".OGR_FID where MBRIntersects(" + str_database + table + ".shape,GeomFromText('Polygon((" + polyString + "))'));";
	//str_sql = "select " + field + ", " + str_database + table + "_general_" + this->str_LODLevel + ".LonLat_Coordinate from " + str_database + table + " right join " + str_database + table + "_general_" + this->str_LODLevel + " on " + str_database + table + ".OGR_FID=" + str_database + table + "_general_" + this->str_LODLevel + ".OGR_FID;";
    //str_sql = "select " + field + ", " + str_database + table + "_general_" + this->str_LODLevel + ".LonLat_Coordinate from " + str_database + table + " left join " + str_database + table + "_general_" + this->str_LODLevel + " on " + str_database + table + ".OGR_FID=" + str_database + table + "_general_" + this->str_LODLevel + ".OGR_FID where MBRIntersects(" + str_database + table + ".shape,GeomFromText('Polygon((" + polyString + "))')) limit " + str_rowlimit + ";";

	//good one
	//str_sql = "select " + field + ", " + str_database + table + "_general_" + this->str_LODLevel + ".LonLat_Coordinate from " + str_database + table + " right join " + str_database + table + "_general_" + this->str_LODLevel + " on " + str_database + table + ".OGR_FID=" + str_database + table + "_general_" + this->str_LODLevel + ".OGR_FID where MBRIntersects(" + str_database + table + ".shape,GeomFromText('Polygon((" + polyString + "))'));";

	//LOD table
	//str_sql = "select " + field + ", astext(intersection(geomfromtext('Polygon(("+ polyString +"))'),shape)) from "+ str_database + table + "_general_" + this->str_LODLevel +" where MBRIntersects(geomfromtext('Polygon(("+ polyString +"))'), shape);";
	//non-LOD table
	//str_sql = "select " + field + ", astext(intersection(geomfromtext('Polygon(("+ polyString +"))'),shape)) from " + str_database + table + " where MBRIntersects(geomfromtext('Polygon(("+ polyString +"))'), shape);";
	//str_sql = "select " + field + ", astext(intersection(geomfromtext('Polygon(("+ polyString +"))'), shape)) from " + str_database + table + " where MBRIntersects(shape, geomfromtext('Polygon(("+ polyString +"))'));";
	


	try
	{
		this->res = this->stmt->executeQuery(str_sql);
	}
	catch(sql::SQLException &e)
	{
		logObj.writeToLog(_ERROR, (char*)e.what());
		logObj.writeToLog(_ERROR, (char*)e.getSQLState());
	}


	return this->res;

}

bool CDataBaseLayerDataSource::QuerySQLServer(std::string table, std::string field, bool isQueryFullyInsideTile)
{

	std::string str_ltLat1, str_ltLon1, str_rbLat2, str_rbLon2, polyString, str_sql;

	//std::string str_rowlimit = "50";
	//std::string str_database = "adcitest.";
	std::string str_database = DATABASE;

	/*str_ltLon1 = this->rectExtend.str_ExtLong1;
	str_ltLat1 = this->rectExtend.str_ExtLat1;
	str_rbLon2 = this->rectExtend.str_ExtLong2;
	str_rbLat2 = this->rectExtend.str_ExtLat2;*/


	str_ltLon1 = this->rectExtend.str_Long1;
	str_ltLat1 = this->rectExtend.str_Lat1;
	str_rbLon2 = this->rectExtend.str_Long2;
	str_rbLat2 = this->rectExtend.str_Lat2;


	polyString = str_rbLon2 + " " + str_ltLat1 + " , " + str_ltLon1 + " " +
		str_ltLat1 + " , " + str_ltLon1 + " " + str_rbLat2 + ", " + str_rbLon2 +
		" " + str_rbLat2 + ", " + str_rbLon2 + " " + str_ltLat1;

	double dQuadTileInside_long1, dQuadTileInside_lat1, 
		dQuadTileInside_long2, dQuadTileInside_lat2;
	std::string str_QuadTileInside_long1, str_QuadTileInside_lat1, 
		str_QuadTileInside_long2, str_QuadTileInside_lat2;
	CMapUtility mapUtility;

	//add 0.1/level lat/long offset to solve the precision
	/*dQuadTileInside_long1 = this->rectExtend.dX1 + (LONGLATOFFSET/this->nlevel);
	dQuadTileInside_lat1  = this->rectExtend.dY1 - (LONGLATOFFSET/this->nlevel);
	dQuadTileInside_long2 = this->rectExtend.dX2 - (LONGLATOFFSET/this->nlevel);
	dQuadTileInside_lat2  = this->rectExtend.dY2 + (LONGLATOFFSET/this->nlevel);*/

	dQuadTileInside_long1 = this->rectExtend.dX1 - (LONGLATOFFSET/this->nlevel);
	dQuadTileInside_lat1  = this->rectExtend.dY1 + (LONGLATOFFSET/this->nlevel);
	dQuadTileInside_long2 = this->rectExtend.dX2 + (LONGLATOFFSET/this->nlevel);
	dQuadTileInside_lat2  = this->rectExtend.dY2 - (LONGLATOFFSET/this->nlevel);

	str_QuadTileInside_long1 = mapUtility.DoubleToString(dQuadTileInside_long1);
	str_QuadTileInside_lat1  = mapUtility.DoubleToString(dQuadTileInside_lat1);
	str_QuadTileInside_long2 = mapUtility.DoubleToString(dQuadTileInside_long2);
	str_QuadTileInside_lat2  = mapUtility.DoubleToString(dQuadTileInside_lat2);

	//select geom.STIntersection(geography::STGeomFromText('Polygon((105 37, 105 35, 172 35, 172 37, 105 37))',4326)).ToString() from adcitest.dbo.wm_World_Nations where geom.STIntersects(geography::STGeomFromText('Polygon((105 37, 105 35, 172 35, 172 37, 105 37))',4326))=1;


	//good
	//str_sql = "select " + field + ", geom.STIntersection(geography::STGeomFromText('Polygon((" + polyString + "))',4326)).ToString() from " + str_database + "dbo." + table + " where geom.STIntersects(geography::STGeomFromText('Polygon((" + polyString + "))',4326))=1;";

	//str_sql = "select " + field + ", astext(intersection(geomfromtext('Polygon(("+ polyString +"))'),shape)) from " + str_database + table + " where MBRIntersects(geomfromtext('Polygon(("+ polyString +"))'), shape);";


	//SELECT Cntry_Name, geomNew.STIntersection(@h) from adcitest.dbo.wm_World_Nations where geomEnv.STIntersects(@h)=1;

	//non_LOD
	//str_sql = "select " + field + ", geomValid.STIntersection(geometry::STGeomFromText('Polygon((" + polyString + "))',0)).ToString() from " + str_database + "dbo." + table + " where geomEnv.STIntersects(geometry::STGeomFromText('Polygon((" + polyString + "))',0))=1;";

	//LOD
	//str_sql = "select " + field + ", geomValid.STIntersection(geometry::STGeomFromText('Polygon((" + polyString + "))',0)).ToString() from " + str_database + "dbo." + table + "_" + this->str_LODLevel + " where geomEnv.STIntersects(geometry::STGeomFromText('Polygon((" + polyString + "))',0))=1;";

	//multiple tables, good
	/*str_sql = "select " + field + ", " + table + "_" + this->str_LODLevel 
		+ ".geomValid.STIntersection(geometry::STGeomFromText('Polygon((" + polyString + "))',0)).ToString() from " 
		+ str_database + "dbo." + table + "_" + this->str_LODLevel + " left join " + str_database + "dbo." + table + " on " 
		+ str_database + "dbo." + table + "_" + this->str_LODLevel + ".id = " + str_database + "dbo." + table + ".ID" 
		+ " where " + table + "_" + this->str_LODLevel + ".geomEnv.STIntersects(geometry::STGeomFromText('Polygon((" + polyString 
		+ "))',0))=1;";*/


	//two tables, for polygon and linestring
	//if( ShapeUtility::polygon == this->nLayerType )
	//{
	//	str_sql = "select " + table + "." + field + ", " + table + "_LOD.geomValid.STIntersection(geometry::STGeomFromText('Polygon((" 
	//		+ polyString + "))',0)).ToString() from " + str_database + "dbo." + table 
	//		+ "_LOD left join " + str_database + "dbo." + table + " on " 
	//		+ str_database + "dbo." + table + "_LOD.id = " + str_database + "dbo." + table + ".ID" 
	//		+ " where " + str_database + "dbo." + table + "_LOD.LOD_LEVEL=" + this->str_LODLevel + " and " 
	//		+ table + "_LOD.geomEnv.STIntersects(geometry::STGeomFromText('Polygon((" + polyString 
	//		+ "))',0))=1;";
	//}
	//else if( ShapeUtility::point == this->nLayerType )//for points
	//{
	//	str_sql = "select " + field + ", " + table + ".geomValid.ToString() from " + str_database + "dbo." + table 
	//		+ " where " + table + ".geomValid.STWithin(geometry::STGeomFromText('Polygon((" + polyString 
	//		+ "))',0))=1;";
	//}
	//else if( ShapeUtility::arc == this->nLayerType )//for stringling
	//{
	//	str_sql = "select " + table + "." + field + ", " + table + "_LOD.geomValid.ToString() from " + str_database + "dbo." + table 
	//		+ "_LOD left join " + str_database + "dbo." + table + " on " 
	//		+ str_database + "dbo." + table + "_LOD.id = " + str_database + "dbo." + table + ".ID" 
	//		+ " where " + str_database + "dbo." + table + "_LOD.LOD_LEVEL=" + this->str_LODLevel + " and " 
	//		+ table + "_LOD.geomEnv.STIntersects(geometry::STGeomFromText('Polygon((" + polyString 
	//		+ "))',0))=1;";
	//}

	//good for one table
	//if( ShapeUtility::polygon == this->nLayerType )
	//{
	//	str_sql = "select labelname, geomValid.STIntersection(geometry::STGeomFromText('Polygon((" 
	//		+ polyString + "))',0)).ToString() from " + str_database + "dbo." + table 
	//		+ "_LOD where " + str_database + "dbo." + table + "_LOD.LOD_LEVEL=" + this->str_LODLevel + " and " 
	//		+ table + "_LOD.geomEnv.STIntersects(geometry::STGeomFromText('Polygon((" + polyString 
	//		+ "))',0))=1;";
	//}
	//else if( ShapeUtility::point == this->nLayerType )//for points
	//{
	//	str_sql = "select " + field + ", " + table + ".geomValid.ToString() from " + str_database + "dbo." + table 
	//		+ " where " + table + ".geomValid.STWithin(geometry::STGeomFromText('Polygon((" + polyString 
	//		+ "))',0))=1;";
	//}
	//else if( ShapeUtility::arc == this->nLayerType )//for stringling
	//{
	//	str_sql = "select labelname, " + table + "_LOD.geomValid.ToString() from " + str_database + "dbo." + table 
	//		+ "_LOD where " + str_database + "dbo." + table + "_LOD.LOD_LEVEL=" + this->str_LODLevel + " and " 
	//		+ table + "_LOD.geomEnv.STIntersects(geometry::STGeomFromText('Polygon((" + polyString 
	//		+ "))',0))=1;";
	//}
	//else
	//{
	//	logObj.writeToLog(_ERROR, "unknown format of SQL Query type");
	//}

	//SELECT cntry_name, wm_World_Nations_7.geomValid.STIntersection(@h) from adcitest.dbo.wm_World_Nations_7 left join adcitest.dbo.wm_World_Nations on adcitest.dbo.wm_World_Nations_7.id = adcitest.dbo.wm_World_Nations.ID where wm_World_Nations_7.geomEnv.STIntersects(@h)=1;


	if( "fapointsowner" == table )//for points
	{
		str_sql = "select FAParcel.dbo.points.OWNER as labelname, points.geom.ToString() as coordinates from FAParcel.dbo.points where LONGITUDE>=" + str_QuadTileInside_long1 
			+ " and LATITUDE<="  + str_QuadTileInside_lat1
			+ " and LONGITUDE<=" + str_QuadTileInside_long2
			+ " and LATITUDE>="  + str_QuadTileInside_lat2 + ";";

		/*str_sql = "select adcitest.dbo.wm_Cities_Unknownpop.NAME, points.geom.ToString() from adcitest.dbo.wm_Cities_Unknownpop where LONGITUDE>=" + str_QuadTileInside_long1 
			+ " and LATITUDE<="  + str_QuadTileInside_lat1
			+ " and LONGITUDE<=" + str_QuadTileInside_long2
			+ " and LATITUDE>="  + str_QuadTileInside_lat2 + ";";*/

		/*str_sql = "select ST_NAME as labelname, geomValid.ToString() from adcitest.dbo.Streets where X1<=" + str_QuadTileInside_long2 
				+ " and Y2<="  + str_QuadTileInside_lat1
				+ " and X2>=" + str_QuadTileInside_long1
				+ " and Y1>="  + str_QuadTileInside_lat2 + ";";*/


	}
	else if( "fapointsaddr" == table )//for points
	{
		str_sql = "select FAParcel.dbo.points.ADDR as labelname, points.geom.ToString() as coordinates from FAParcel.dbo.points where LONGITUDE>=" + str_QuadTileInside_long1 
			+ " and LATITUDE<="  + str_QuadTileInside_lat1
			+ " and LONGITUDE<=" + str_QuadTileInside_long2
			+ " and LATITUDE>="  + str_QuadTileInside_lat2 + ";";


	}
	else if( "parcel" == table )//for points
	{
		/*str_sql = "select " + field + ", " + table + ".geomValid.ToString() from " + str_database + ".dbo." + table 
			+ " where " + table + ".geomValid.STWithin(geometry::STGeomFromText('Polygon((" + polyString 
			+ "))',0))=1;";*/

		/*if(table != "nations_name")
		{*/

		str_sql = "select parcel.geomValid.ToString() as coordinates from FAParcel.dbo.parcel where central_lon>=" 
			+ str_QuadTileInside_long1 
			+ " and central_lat<="  + str_QuadTileInside_lat1
			+ " and central_lon<=" + str_QuadTileInside_long2
			+ " and central_lat>="  + str_QuadTileInside_lat2 + ";";


		/*str_sql = "select ST_NAME as labelname, geomValid.ToString() from adcitest.dbo.Streets where X1<=" + str_QuadTileInside_long2 
				+ " and Y2<="  + str_QuadTileInside_lat1
				+ " and X2>=" + str_QuadTileInside_long1
				+ " and Y1>="  + str_QuadTileInside_lat2 + ";";*/


	}


	else if( ShapeUtility::point == this->nLayerType )//for points
	{
		/*str_sql = "select " + field + ", " + table + ".geomValid.ToString() from " + str_database + ".dbo." + table 
			+ " where " + table + ".geomValid.STWithin(geometry::STGeomFromText('Polygon((" + polyString 
			+ "))',0))=1;";*/

		/*if(table != "nations_name")
		{*/

		str_sql = "select " + field + ", " + table + ".geomValid.ToString() from " + str_database + ".dbo." + table 
			+ " where LONGITUDE>=" + str_QuadTileInside_long1 
			+ " and LATITUDE<="  + str_QuadTileInside_lat1
			+ " and LONGITUDE<=" + str_QuadTileInside_long2
			+ " and LATITUDE>="  + str_QuadTileInside_lat2 + ";";
		/*}
		else
		{
			str_sql = "select " + field + ", " + table + ".geomValid.ToString() from " + str_database + ".dbo." + table 
			+ " where " + table + ".geomValid.STWithin(geometry::STGeomFromText('Polygon((" + polyString 
			+ "))',0))=1;";
		}*/


	}
	//else if(!isQueryFullyInsideTile)
	else if( "SOAR_test" == table )
	{
		/*str_sql = "select labelname, color, geomValid.ToString() from adcitest1.dbo." + table 
			+ " where LONGITUDE>=" + str_QuadTileInside_long1 
			+ " and LATITUDE<="  + str_QuadTileInside_lat1
			+ " and LONGITUDE<=" + str_QuadTileInside_long2
			+ " and LATITUDE>="  + str_QuadTileInside_lat2 + " order by id;";*/
		/*str_sql = "select labelname, color, geomValid.ToString() from adcitest1.dbo." + table 
			+ " where LONGITUDE>=" + str_QuadTileInside_long1 
			+ " and LATITUDE<="  + str_QuadTileInside_lat1
			+ " and LONGITUDE<=" + str_QuadTileInside_long2
			+ " and LATITUDE>="  + str_QuadTileInside_lat2 
			+ " and source='" + str_source + "'"
			+ " and date='" + str_date + "'"
			+ " order by id;";*/

		str_sql = "select labelname, color, geomValid.ToString() from adcitest1.dbo." + table 
			+ " where LONGITUDE>=" + this->rectExtend.str_ExtLong1
			+ " and LATITUDE<="  + this->rectExtend.str_ExtLat1
			+ " and LONGITUDE<=" + this->rectExtend.str_ExtLong2
			+ " and LATITUDE>="  + this->rectExtend.str_ExtLat2
			+ " and source='" + str_source + "'"
			+ " and date='" + str_date + "'"
			+ " order by id;";
	}
	else if( (!isQueryFullyInsideTile) && ("STREETS"!=table) )//testing for Navteq street only
	{
		str_sql = "select labelname, geom.ToString() from " + str_database + ".dbo." + table 
			+ "_QuadTile where quadkey=" + "'" + this->str_Quadkey + "'";
	}
	/////////////////////////
	//testing Navteq street only
	else if( "STREETS" == table /*||
		"wm_Minor_Routes" == table || 
		"wm_Major_Routes" == table*/ )
	{


		str_sql = "select ST_NAME as labelname, geomValid.ToString() from adcitest.dbo." + table 
				+ " where X1<=" + this->rectExtend.str_ExtLong2 
				+ " and Y2<="  + this->rectExtend.str_ExtLat1
				+ " and X2>=" + this->rectExtend.str_ExtLong1
				+ " and Y1>="  + this->rectExtend.str_ExtLat2 + ";";
		

	}
	/*else if( "wm_Minor_Routes" == table || "wm_Major_Routes" == table)
	{

		str_sql = "select ST_NAME as labelname, geomValid.ToString() from adcitest1.dbo." + table 
				+ " where X1<=" + this->rectExtend.str_ExtLong2 
				+ " and Y2<="  + this->rectExtend.str_ExtLat1
				+ " and X2>=" + this->rectExtend.str_ExtLong1
				+ " and Y1>="  + this->rectExtend.str_ExtLat2 + ";";
		
	}*/
	/////////////////////////
	else
	{


		str_sql = "select * from " + str_database + ".dbo." + table 
			+ "_QuadTile_Inside where long1<=" + str_QuadTileInside_long1 
			+ " and lat1>="  + str_QuadTileInside_lat1
			+ " and long2>=" + str_QuadTileInside_long2
			+ " and lat2<="  + str_QuadTileInside_lat2 + ";";

	}



	//testing for LOD data query
	//str_sql = "select labelname, geomValid.STIntersection(geometry::STGeomFromText('Polygon((" + polyString + "))',0)).ToString() from adcitest.dbo." + table + "_LOD where LOD_LEVEL=1 and geomEnv.STIntersects(geometry::STGeomFromText('Polygon((" + polyString + "))',0))=1;";


	try
	{
		//this->prs_sqlserver->Close();
		logObj.writeToLog(_INFO, (char*)str_sql.c_str());

		this->prs_sqlserver->Open(str_sql.c_str(), 
			_variant_t((IDispatch*)pConnection_sqlserver, true), 
			ADODB::adOpenForwardOnly, ADODB::adLockReadOnly, 
			ADODB::adCmdText/*ADODB::adCmdTable*/);
	}
	catch(_com_error &e)
	{
		logObj.writeToLog(_ERROR, (char*)e.Description());
		return false;
	}

	return true;
	


}

//bool CDataBaseLayerDataSource::QueryPostGIS(std::string table, std::string field)
//{
//
//	std::string str_ltLat1, str_ltLon1, str_rbLat2, str_rbLon2, polyString, str_sql;
//
//	std::string str_rowlimit = "50";
//	//std::string str_database = "adcitest.";
//	std::string str_database = DATABASE;
//
//	str_ltLon1 = this->rectExtend.str_Long1;
//	str_ltLat1 = this->rectExtend.str_Lat1;
//	str_rbLon2 = this->rectExtend.str_Long2;
//	str_rbLat2 = this->rectExtend.str_Lat2;
//
//	polyString = str_rbLon2 + " " + str_ltLat1 + " , " + str_ltLon1 + " " +
//		str_ltLat1 + " , " + str_ltLon1 + " " + str_rbLat2 + ", " + str_rbLon2 +
//		" " + str_rbLat2 + ", " + str_rbLon2 + " " + str_ltLat1;
//
//
//	//select geom.STIntersection(geography::STGeomFromText('Polygon((105 37, 105 35, 172 35, 172 37, 105 37))',4326)).ToString() from adcitest.dbo.wm_World_Nations where geom.STIntersects(geography::STGeomFromText('Polygon((105 37, 105 35, 172 35, 172 37, 105 37))',4326))=1;
//
//
//	str_sql = "select " + field + ", geom.STIntersection(geography::STGeomFromText('Polygon((" + polyString + "))',4326)).ToString() from " + str_database + "dbo." + table + " where geom.STIntersects(geography::STGeomFromText('Polygon((" + polyString + "))',4326))=1;";
//
//	//str_sql = "select " + field + ", astext(intersection(geomfromtext('Polygon(("+ polyString +"))'),shape)) from " + str_database + table + " where MBRIntersects(geomfromtext('Polygon(("+ polyString +"))'), shape);";
//
//
//	//try
//	//{
//	//	//this->prs_sqlserver->Open(str_sql.c_str(), _variant_t((IDispatch*)pConnection_sqlserver, true), ADODB::adOpenForwardOnly, ADODB::adLockReadOnly, ADODB::adCmdText/*ADODB::adCmdTable*/);
//	//}
//	//catch(_com_error &e)
//	//{
//	//	logObj.writeToLog(_ERROR, (char*)e.Description());
//	//}
//
//
//	/* Should PQclear PGresult whenever it is no longer needed to avoid memory leaks */
//	PQclear(this->pres_postgis);
//
//	/* Fetch rows from pg_database, the system catalog of databases */
//	this->pres_postgis = PQexec(this->pconn_postgis, str_sql.c_str());
//	if (PQresultStatus(pres_postgis) != PGRES_COMMAND_OK) 
//	{
//		/*fprintf(stderr, "DECLARE CURSOR failed: %s",
//			PQerrorMessage(conn));*/
//		PQclear(this->pres_postgis);
//		exit_nicely(this->pconn_postgis);
//	}
//
//
//
//
//	return true;
//	
//
//
//}










CShape*  CDataBaseLayerDataSource::getRecord(long lRecord, CStringArray& aPropertyFields)
{
	ASSERT(false);// no implemneted
	return NULL;
}







bool CDataBaseLayerDataSource::GetRecordsFromWKT(CShapePtrArray& apShapesArray, CString str_res, 
	CString str_name, CString str_color)
{
	//CShape* ashapeobject = NULL;
	//int ipos = str_res.Find("(", 0);
	//str_res = str_res.Trim();
	//str_res.Replace(" ","");
	int ipos = str_res.Find("(", 0);
	if( -1 == ipos )
	{
		return false;
	}

	if( SQLSERVER == this->nDataBaseType )
	{
		ipos = ipos - 1;//SQL Server adds one space before "("
	}

	//CString resToken;
	//int /*curPos= 0, */ipoint = 0;
	CWKTManager wktmanager;
	wktmanager.SetDatabaseType(this->nDataBaseType);
	CString listitem;

	switch(ipos)
	{
	case WKT_POINT:

		str_res = wktmanager.RemoveWKTParentheses(wktmanager.RemoveWKTStringName(str_res, WKT_POINT));
		apShapesArray.Add(wktmanager.GetWKTPoint(str_res, /*&ipoint,*/ str_name));
		break;

	case WKT_POLYGON:

		str_res = wktmanager.RemoveWKTParentheses(wktmanager.RemoveWKTStringName(str_res, WKT_POLYGON));
		if( str_color == "" )
		{
			apShapesArray.Add(wktmanager.GetWKTPolygon(str_res, str_name));
		}
		//this color info. is in database column
		else
		{
			apShapesArray.Add(wktmanager.GetWKTPolygon(str_res, str_name, str_color));
		}
		
		break;

	case WKT_LINESTRING_MULTIPOINT:
		if( -1 != str_res.Find("LINESTRING", 0))
		{
			//WKT_LINESTRING
			str_res = wktmanager.RemoveWKTStringName(str_res, WKT_LINESTRING_MULTIPOINT);
			//wktmanager.GetWKTLineString(*ashapeobject, str_res, &ipoint);
			apShapesArray.Add(wktmanager.GetWKTLineString(str_res, /*&ipoint,*/ str_name));

		}
		else if(-1 != str_res.Find("MULTIPOINT", 0))
		{
			//WKT_MULTIPOINT
			//need investigation for Multipoint format
			str_res = wktmanager.RemoveWKTParentheses(wktmanager.RemoveWKTStringName(str_res, WKT_LINESTRING_MULTIPOINT));
			wktmanager.GetWKTMultiPoint(apShapesArray, str_res, str_name);
		}
		else
		{
			logObj.writeToLog(_ERROR, "unknown format of WKT_LINESTRING_MULTIPOINT");
			return false;
		}
		break;

	case WKT_MULTIPOLYGON:

		str_res = wktmanager.RemoveWKTParentheses(wktmanager.RemoveWKTStringName(str_res, WKT_MULTIPOLYGON));
		//wktmanager.GetWKTMultiPolygon(apShapesArray, str_res, &ipoint, str_name);
		apShapesArray.Add(wktmanager.GetWKTMultiPolygon(str_res, /*&ipoint,*/ str_name));
		break;

	case WKT_MULTILINESTRING:

		str_res = wktmanager.RemoveWKTParentheses(wktmanager.RemoveWKTStringName(str_res, WKT_MULTILINESTRING));
		//wktmanager.GetWKTLineString(*ashapeobject, str_res, &ipoint, str_name);
		//apShapesArray.Add(ashapeobject);
		//apShapesArray.Add(wktmanager.GetWKTMultiLineString(str_res, /*&ipoint,*/ str_name));
		wktmanager.GetWKTMultiLineString(apShapesArray, str_res, str_name);
		break;
	case WKT_GEOMETRYCOLLECTION:

		str_res = wktmanager.RemoveWKTParentheses(wktmanager.RemoveWKTStringName(str_res, WKT_GEOMETRYCOLLECTION));
		//CStringList geometrylist;
		wktmanager.GetWKTGeometryCollectionStringList(str_res, &this->geometrylist);

		//if( !this->geometrylist.IsEmpty() )
		//{
		//	POSITION pos = this->geometrylist.GetHeadPosition();
		//	while( pos != NULL )
		//	{
		//		//this->GetRecordsFromWKT(apShapesArray, this->geometrylist.GetNext(pos), str_name);
		//		listitem = this->geometrylist.GetNext(pos);
		//		if( (WKT_POLYGON + 1) == listitem.Find("(", 0) )
		//		{
		//			apShapesArray.Add(wktmanager.GetWKTPolygon(listitem, str_name));
		//		}				
		//	}

		//	geometrylist.RemoveAll();
		//}

		wktmanager.GetWKTGeometryCollection(apShapesArray, str_name, &geometrylist, this->nLayerType);

		geometrylist.RemoveAll();

		break;

	default:
		logObj.writeToLog(_ERROR, "unknown format of WKT "/*+(char*)this->str_Table*/);
		return false;
	}

	return true;

}




//bool CDataBaseLayerDataSource::GetRecordFromMySQL(CShapePtrArray& apShapes)
//{
//	try
//	{
//		//std::string str_result;
//		CString str_label_name, str_result_coordinates;
//
//		this->QueryMySQL(this->str_Table, this->str_Field);
//
//		//std::string name, cord;
//
//		//str_result = this->res->relative(lRecord);
//
//
//		while (this->res->next()) 
//		{
//
//			//CString str_label_name, str_result_coordinates;
//
//
//			str_label_name         = res->getString(1).c_str();
//			str_result_coordinates = res->getString(2).c_str();
//
//			/*name = res->getString(1);
//			cord = res->getString(2);*/
//
//			//str_label_name         = name.c_str();
//			//str_result_coordinates = cord.c_str();
//
//
//
//			/*if("Turkey" == str_label_name)
//			{
//				printf("here");
//				this->GetRecordsFromWKT(apShapes, str_result_coordinates, str_label_name);
//			}*/
//
//			//if( "" == str_result_coordinates)
//			if(str_result_coordinates.IsEmpty())
//			{
//				continue;
//			}
//
//
//			this->GetRecordsFromWKT(apShapes, str_result_coordinates, str_label_name);
//
//		}
//
//
//		return true;
//
//	}
//	catch(sql::SQLException &e)
//	{
//		logObj.writeToLog(_ERROR, (char*)e.what());
//		logObj.writeToLog(_ERROR, (char*)e.getSQLState());
//	}
//
//	return true;
//
//}


bool CDataBaseLayerDataSource::GetRecordFromMySQL(CShapePtrArray& apShapes)
{
	CString str_label_name, str_result_coordinates, str_color;
	//color should be from database color column
	str_color = "";
	try
	{
		//CString str_label_name, str_result_coordinates;
		this->QueryMySQL(this->str_Table, this->str_Field);
		while (this->res->next()) 
		{
			str_label_name         = res->getString(1).c_str();
			str_result_coordinates = res->getString(2).c_str();

			if(str_result_coordinates.IsEmpty())
			{
				continue;
			}
			this->GetRecordsFromWKT(apShapes, str_result_coordinates, str_label_name, str_color);
		}
		return true;

	}
	catch(sql::SQLException &e)
	{
		logObj.writeToLog(_ERROR, (char*)e.what());
		logObj.writeToLog(_ERROR, (char*)e.getSQLState());
	}
	return true;
}

std::string CDataBaseLayerDataSource::GetRectExtWKTPolygon()
{
	std::string str;

	/*str = "POLYGON ((" + this->rectExtend.str_Long1 +
		this->rectExtend.str_Lat1  + ", " +

		this->rectExtend.str_Long2 + " "  +
		this->rectExtend.str_Lat1  + ", " +

		this->rectExtend.str_Long2 + " "  +
		this->rectExtend.str_Lat2  + ", " +

		this->rectExtend.str_Long1 + " "  +
		this->rectExtend.str_Lat2  + ", " +

		this->rectExtend.str_Long1 + " "  +
		this->rectExtend.str_Lat1  +

		"))";*/


	str = "POLYGON ((" + this->rectExtend.str_ExtLong1 +
		this->rectExtend.str_ExtLat1  + ", " +

		this->rectExtend.str_ExtLong2 + " "  +
		this->rectExtend.str_ExtLat1  + ", " +

		this->rectExtend.str_ExtLong2 + " "  +
		this->rectExtend.str_ExtLat2  + ", " +

		this->rectExtend.str_ExtLong1 + " "  +
		this->rectExtend.str_ExtLat2  + ", " +

		this->rectExtend.str_ExtLong1 + " "  +
		this->rectExtend.str_ExtLat1  +

		"))";


	return str;
}

bool CDataBaseLayerDataSource::GetRecordFromSQLServer(CShapePtrArray& apShapes)
{


	CString str_label_name, str_result_coordinates, str_color;
	str_color = "";

	try
	{
		//logObj.writeToLog(_INFO, "begin query");
		this->QuerySQLServer(this->str_Table, this->str_Field, false);
		//if( (this->prs_sqlserver->ADOEOF)&&
		//	(ShapeUtility::polygon == this->nLayerType) )
		//	//if returned null, there will be two possibilities, 
		//	//one is Fully inside Tile, another is Fullyoutside Tile
		//	//so query again to check if this is fully inside tile
		//{
		//	this->prs_sqlserver->Close();//should close the first time query
		//	this->QuerySQLServer(this->str_Table, this->str_Field, true);
		//	if(!this->prs_sqlserver->ADOEOF)
		//	{
		//		str_label_name = "";
		//		//str_color      = "";
		//		str_result_coordinates = this->GetRectExtWKTPolygon().c_str();
		//		this->GetRecordsFromWKT(apShapes, str_result_coordinates, 
		//			str_label_name, str_color);
		//	}
		//	else
		//	{
		//		//this is fully outside tile, should be return null
		//		this->prs_sqlserver->Close();
		//		return false;
		//	}

		//}
		//else 
		if(!this->prs_sqlserver->ADOEOF)
		{
			this->prs_sqlserver->MoveFirst();
			while (!this->prs_sqlserver->ADOEOF) 
			{
				//str_label_name         = (LPCSTR)(_bstr_t)this->prs_sqlserver->Fields->GetItem("Cntry_Name")->Value;
				//str_label_name         = (LPCSTR)(_bstr_t)this->prs_sqlserver->Fields->GetItem(this->str_Field.c_str())->Value;
				//if( (ShapeUtility::polygon == this->nLayerType) ||
				//	(ShapeUtility::arc     == this->nLayerType) )
				//{
				if(this->str_Table != "parcel")
				{
					str_label_name = (LPCSTR)(_bstr_t)this->prs_sqlserver->Fields->GetItem("labelname")->Value;
				}
					//str_label_name = (LPCSTR)(_bstr_t)this->prs_sqlserver->Fields->GetItem(this->str_Field.c_str())->Value;
				//}
				//else
				//{
				//	str_label_name = (LPCSTR)(_bstr_t)this->prs_sqlserver->Fields->GetItem(this->str_Field.c_str())->Value;
				//}

				//str_label_name         = (LPCSTR)(_bstr_t)this->prs_sqlserver->Fields->GetItem("labelname")->Value;
				str_result_coordinates = (LPCSTR)(_bstr_t)this->prs_sqlserver->Fields->GetItem("coordinates")->Value;

				/*if( "SOAR_test" == this->str_Table )
				{
					str_color = (LPCSTR)(_bstr_t)this->prs_sqlserver->Fields->GetItem("color")->Value;
				}*/
				


				if( ("GEOMETRYCOLLECTION EMPTY" != str_result_coordinates) && 
					!str_result_coordinates.IsEmpty())
				{
					this->GetRecordsFromWKT(apShapes, str_result_coordinates, 
						str_label_name, str_color);
				}


				prs_sqlserver->MoveNext();
			}
		}
		//else //(this->prs_sqlserver->ADOEOF)&&(ShapeUtility::polygon != this->nLayerType)
		//{
		//	this->prs_sqlserver->Close();
		//	return false;
		//}
		//}
		//logObj.writeToLog(_INFO, "end WKT");

		///////////////////////////////
		this->prs_sqlserver->Close();
		///////////////////////////////


	}
	catch(_com_error &e)
	{
		logObj.writeToLog(_ERROR, (char*)e.Description());
	}

	return true;
}



//bool CDataBaseLayerDataSource::GetRecordFromPostGIS(CShapePtrArray& apShapes)
//{
//
//	CString str_label_name, str_result_coordinates;
//	try
//	{
//
//		this->QueryPostGIS(this->str_Table, this->str_Field);
//
//		this->prs_sqlserver->MoveFirst();
//		if(!this->prs_sqlserver->ADOEOF)
//		{
//			while (!this->prs_sqlserver->ADOEOF) 
//			{
//				/*index.iVal = 0;
//				printf("%s\t", (LPCSTR)(_bstr_t)prs_sqlserver->GetFields()->GetItem(& index)->Value);
//				index.iVal = 1;
//				printf("%s\n", (LPCSTR)(_bstr_t)prs_sqlserver->Fields->GetItem(& index)->Value);*/
//
//				//str_label_name         = (LPCSTR)(_bstr_t)this->prs_sqlserver->Fields->GetItem("Cntry_Name")->Value;
//				str_label_name         = (LPCSTR)(_bstr_t)this->prs_sqlserver->Fields->GetItem(this->str_Field.c_str())->Value;
//				str_result_coordinates = (LPCSTR)(_bstr_t)this->prs_sqlserver->Fields->GetItem("")->Value;
//				if(str_result_coordinates.IsEmpty())
//				{
//					continue;
//				}
//
//				this->GetRecordsFromWKT(apShapes, str_result_coordinates, str_label_name);
//
//				prs_sqlserver->MoveNext();
//
//			}
//		}
//
//	}
//	catch(_com_error &e)
//	{
//		logObj.writeToLog(_ERROR, (char*)e.Description());
//	}
//
//	return true;
//}






bool CDataBaseLayerDataSource::GetRecordFromDatabase(CShapePtrArray& apShapes)
{
	if( -1 == this->nDataBaseType )
	{
		return false;
	}

	//the this->nDataBaseType should be assigned

	CString str_label_name, str_result_coordinates;

	switch(this->nDataBaseType)
	{
		case MYSQL:
			if(!this->GetRecordFromMySQL(apShapes))
			{
				return false;
			}
			break;
		case SQLSERVER:
			if(!this->GetRecordFromSQLServer(apShapes))
			{
				return false;
			}

			break;
		case POSTGIS:
			break;
		default:
			return false;
	}

	return true;
}





bool CDataBaseLayerDataSource::SetDatabaseType(int nDBType)
{
	if( -1 == nDBType )
	{
		return false;
	}
	this->nDataBaseType = nDBType;
	return true;
}

bool CDataBaseLayerDataSource::OpenDatabase()
{
	if( -1 == this->nDataBaseType )
	{
		return false;
	}

	switch(this->nDataBaseType)
	{
		case MYSQL:

			if(!this->OpenMySQLDB())
			{
				return false;
			}
			break;

		case SQLSERVER:

			if(!this->OpenSQLServerDB())
			{
				return false;
			}
			break;
			
			break;
		case POSTGIS:

			/*if(!this->OpenPostGISDB())
			{
				return false;
			}*/

			break;
		default:
			return false;
	}

	return true;


}

bool CDataBaseLayerDataSource::OpenMySQLDB()
{

	if( MYSQL != this->nDataBaseType )
	{
		return false;
	}

	try
	{
		//Connect to MySQL
		driver = sql::mysql::get_mysql_driver_instance();
		con = driver->connect(MYSQL_SERVER, MYSQL_USERNAME, MYSQL_PASSWORD);
		//MySQL query
		stmt = con->createStatement();
	}
	catch(sql::SQLException &e)
	{
		logObj.writeToLog(_ERROR, (char*)e.what());
		logObj.writeToLog(_ERROR, (char*)e.getSQLState());
	}

	return true;


}

bool CDataBaseLayerDataSource::OpenSQLServerDB()
{

	if( SQLSERVER != this->nDataBaseType )
	{
		return false;
	}

	HRESULT hr = S_OK;
	if ( FAILED(::CoInitialize(NULL)) )
	{
		return false;
	}

	try
	{
		//ADODB::ConnectionPtr conn;
		TESTHR(this->pConnection_sqlserver.CreateInstance(__uuidof(ADODB::Connection)));
		TESTHR(this->prs_sqlserver.CreateInstance(__uuidof(ADODB::Recordset)));
		strCnn_sqlserver = SQLSERVER_CONNECTION;
		pConnection_sqlserver->Open(strCnn_sqlserver, "", "", ADODB::adConnectUnspecified);

	}
	catch(_com_error &e)
	{
		logObj.writeToLog(_ERROR, (char*)e.Description());
	}

	//CoUninitialize(); 

	return true;

}


//bool CDataBaseLayerDataSource::OpenPostGISDB()
//{
//	if( POSTGIS != this->nDataBaseType )
//	{
//		return false;
//	}
//
//	this->pconninfo_postgis = POSTGIS_CONNECTION;
//	/* Make a connection to the database */
//	this->pconn_postgis = PQconnectdb(this->pconninfo_postgis);
//
//	/* Check to see that the backend connection was
//	successfully made */
//	if (PQstatus(this->pconn_postgis) != CONNECTION_OK) 
//	{
//		/*fprintf(stderr, "Connection to database failed: %s",
//			PQerrorMessage(conn));*/
//		logObj.writeToLog(_ERROR, "Connection to PostGIS database failed");
//		exit_nicely_PostGIS(pconn_postgis);
//	}
//
//
//	/* Start a transaction block */
//	this->pres_postgis = PQexec(this->pconn_postgis, "BEGIN");
//	if (PQresultStatus(this->pres_postgis) != PGRES_COMMAND_OK) 
//	{
//		/*fprintf(stderr, "BEGIN command failed: %s",
//			PQerrorMessage(conn));*/
//		logObj.writeToLog(_ERROR, "PostGIS BEGIN command failed");
//		PQclear(this->pres_postgis);
//		exit_nicely(this->pconn_postgis);
//	}
//
//	/* Should PQclear PGresult whenever it is no longer needed to avoid memory leaks */
//	PQclear(this->pres_postgis);
//
//	return true;
//
//}



bool CDataBaseLayerDataSource::CloseDatabase()
{
	if( -1 == this->nDataBaseType )
	{
		return false;
	}

	switch(this->nDataBaseType)
	{
		case MYSQL:
			if(!this->CloseMySQLDB())
			{
				return false;
			}
			break;
		case SQLSERVER:
			if(!this->CloseSQLServerDB())
			{
				return false;
			}
			break;
		case POSTGIS:

			/*if(!this->ClosePostGISDB())
			{
				return false;
			}*/
			break;
		default:
			return false;
	}

	return true;
}

bool CDataBaseLayerDataSource::CloseMySQLDB()
{

	if( MYSQL != this->nDataBaseType )
	{
		return false;
	}

	try
	{
		//close the connection with MySQL
		delete res;
		//delete countres;
		delete stmt;
		delete con;
	}
	catch(sql::SQLException &e)
	{
		logObj.writeToLog(_ERROR, (char*)e.what());
		logObj.writeToLog(_ERROR, (char*)e.getSQLState());
	}

	return true;

}


bool CDataBaseLayerDataSource::CloseSQLServerDB()
{

	if( SQLSERVER != this->nDataBaseType )
	{
		return false;
	}

	//rs_sqlserver->Close();

	::CoUninitialize();


	return true;
}

//bool CDataBaseLayerDataSource::ClosePostGISDB()
//{
//	if( POSTGIS != this->nDataBaseType )
//	{
//		return false;
//	}
//
//
//	/* close the portal ... we don't bother to check for errors ... */
//	this->pres_postgis = PQexec(this->pconn_postgis, "CLOSE myportal");
//	PQclear(this->pres_postgis);
//
//	/* end the transaction */
//	this->pres_postgis = PQexec(this->pconn_postgis, "END");
//	PQclear(this->pres_postgis);
//
//	/* close the connection to the database and cleanup */
//	PQfinish(this->pconn_postgis);
//
//	return true;
//
//}
