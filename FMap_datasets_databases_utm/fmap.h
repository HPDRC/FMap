#ifndef FMAP_H
#define FMAP_H

#define AREA_TRANSPARENCY_LENGTH	3
#define AREA_COLOR_LENGTH	21
#define LINE_TRANSPARENCY_LENGTH	2
#define POINT_ICON_LENGTH   1
#define OVERRIDE_COLOR_LENGTH		12
#define MAX_PATH_LENGTH		256
#define TILE_SIZE	400


#define POINT_TYPE   1
#define ARC_TYPE     3
#define AREA_TYPE    5
#define ICON_TYPE    7

#define FILLEDAREAREQ_LENGTH  3
#define HIGHLIGHTCENTER_LENGTH 5

#define FA_OWNER 5
#define FA_ADDR 6

#define DISK_LOCATION_DEGREE_LOCAL "C:\\degree\\"
#define DISK_LOCATION_STREET_LOCAL "C:\\streetdata\\"
#define DISK_LOCATION_ICON_LOCAL "C:\\degree\\bitmap-opc\\gifIcon\\"
#define DISK_LOCATION_ICON_LOCAL_JPEG "C:\\degree\\bitmap-opc\\gifIcon\\"
#define DISK_LOCATION_LATLONG_LOCAL "C:\\degree\\latlong\\"

//#define DISK_LOCATION_DEGREE "C:\\degree\\"
//#define DISK_LOCATION_STREET "C:\\streetdata\\"
//#define DISK_LOCATION_ICON "C:\\degree\\bitmap-opc\\gifIcon\\"
//#define DISK_LOCATION_ICON_JPEG "C:\\degree\\bitmap-opc\\"
//#define DISK_LOCATION_LATLONG "C:\\degree\\latlong\\"

#define DISK_LOCATION_DEGREE "E:\\degree\\"
#define DISK_LOCATION_STREET "E:\\streetdata\\"
#define DISK_LOCATION_ICON "E:\\degree\\bitmap-opc\\gifIcon\\"
#define DISK_LOCATION_ICON_JPEG "E:\\degree\\bitmap-opc\\"
#define DISK_LOCATION_LATLONG "E:\\degree\\latlong\\"


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
#define MAX_QUADKEY_LEVEL 9

#define TERRA_NODE

#endif