#ifndef FMAP_H
#define FMAP_H

#define AREA_TRANSPARENCY_LENGTH	3

#define AREA_COLOR_LENGTH	27
#define LINE_COLOR_LENGTH	10
#define AREA_SOUCE_DATE 8

#define LINE_TRANSPARENCY_LENGTH	2
#define POINT_ICON_LENGTH   2
#define OVERRIDE_COLOR_LENGTH		12
#define MAX_PATH_LENGTH		256
#define TILE_SIZE	400

#define POINT_TYPE   1
#define ARC_TYPE     3
#define AREA_TYPE    5
#define ICON_TYPE    7

#define FILLEDAREAREQ_LENGTH  3
#define HIGHLIGHTCENTER_LENGTH 5

#define WKT_POINT 5
#define WKT_LINESTRING_MULTIPOINT 10
#define WKT_POLYGON 7
#define WKT_MULTILINESTRING 15
#define WKT_MULTIPOLYGON 12
#define WKT_GEOMETRYCOLLECTION 18

#define MYSQL_SERVER "tcp://localhost:3306"
#define MYSQL_USERNAME "root"
#define MYSQL_PASSWORD ""

#define MYSQL 1
#define SQLSERVER 2
#define POSTGIS 3

#define SQLSERVER_CONNECTION "Provider='sqloledb';Data Source='(local)';Initial Catalog='adcitest';Integrated Security='SSPI';User Id='SA';Password='';"

#define DATABASE "adcitest1"

#define POSTGIS_CONNECTION "dbname = postgis"

#define PIXELOFFSET 5
#define LONGLATOFFSET 0.1

#define CONFIGFILENAME "fmap_database_config.txt"
#define CONFIGFILELINESIZE 1024

#define NONPRIORITY 100

#define SOURCE_AIRS "AIRS"
#define SOURCE_MODIS "MODIS"

#define MAX_QUADKEY_LEVEL 9

#define WKB_WORDS_BIGENDIAN


#define TERRA_NODE

#endif