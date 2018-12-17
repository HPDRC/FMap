#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <string>
#include <ctime>
#include <cstdarg>
#include <sys/timeb.h>
#include "fmap.h"

typedef enum { _INFO, _WARNING, _ERROR } LogType;


class Configure
{
	std::string configFileName;
	std::string logFileName;
	std::string levelOneDir, levelTwoDir;

	//std::string str_layername;
	//std::string str_field_name;
	std::string str_layer_field;

	//FILE* fp;
	//const char* fconfigname;

	int fontHeight, fontWidth;
	bool bold, italic;
public:
	//Configure(const char* fName="fmap_config.txt");
	Configure(const char* fName="fmap_database_config.txt");

	/*Configure(void);
	~Configure(void);*/

	

	//Configure(const char* fName="fmap_config.txt");
	//Configure(CString filename = "fmap_MySQL_config.txt");

	//std::string getFieldName(std::string layername);

	std::string getLogFileName() { return logFileName; }
	std::string getLevelOneDir() { return levelOneDir; }
	std::string getLevelTwoDir() { return levelTwoDir; }
	
	
	std::string getLayerField() { return str_layer_field;}


	/*bool setLayerName( std::string layername )
	{
		this->str_layername = layername;
		return true;
	}

	std::string getFieldName() {return str_field_name;}*/

	int getFontHeight() { return fontHeight; }
	int getFontWidth() { return fontWidth; }
	bool isBold() { return bold; }
	bool isItalic() { return italic; }





	void show();
};

extern Configure configure;


class Log
{
	FILE *fpLog;	// file pointer to the master log file	
	bool bFileOpen;
	Configure& config;
public:	

	//static int const INFO = 0;
	//static int const WARNING = 1;
	//static int const ERROR = 2;

	Log(Configure c):config(c){}

	~Log()
	{		
		if( fpLog )
			fclose( fpLog );
	}	

	inline void initLogObj(Configure obj)
	{		
		bFileOpen = false;
		struct tm *newtime;
		__int64 ltime;
		char tempFileName[80];
		if( obj.getLogFileName() == "")
			strcpy(tempFileName, "fmap.log");

		_time64( &ltime );
		newtime = _gmtime64( &ltime );	// Obtain coordinated universal time

		if( newtime->tm_mon+1 < 10 )
			sprintf( tempFileName, "c:\\logs\\%s%d0%d%d.log", obj.getLogFileName().c_str(), newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday ); 
            //sprintf( tempFileName, "c:\\FMaplogs\\%s%d0%d%d.log", obj.getLogFileName().c_str(), newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday );
		else
			sprintf( tempFileName, "c:\\logs\\%s%d%d%d.log", obj.getLogFileName().c_str(), newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday ); 
            //sprintf( tempFileName, "c:\\FMaplogs\\%s%d%d%d.log", obj.getLogFileName().c_str(), newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday ); 



		if( (fpLog = fopen( tempFileName, "at" )) != NULL )
			bFileOpen = true;

	}

	inline void close()
	{		
		if( fpLog )
			fclose( fpLog );
	}

	inline void reInit()
	{
		close();
		initLogObj(configure);
	}

	inline void writeToLog(LogType type, char *str )
	{
		if( !bFileOpen ) return;
		char *currTime = getCurrTime();
		if( fpLog )
		{
			switch(type){
			case _INFO:
				fprintf( fpLog, "[%s]\t%s [ %s ]\n", "INFO", currTime, str );
				break;
			case _WARNING:
				fprintf( fpLog, "[%s]\t%s [ %s ]\n", "WARNING", currTime, str );
				break;
			case _ERROR:
				fprintf( fpLog, "[%s]\t%s [ %s ]\n", "ERROR", currTime, str );
				break;
			}
		}
		free( currTime );
	}

	inline char* getCurrTime()
	{
		char *sRetVal;
		struct _timeb timebuf;
		_ftime( &timebuf );

		struct tm *timePtr;
		time_t timer;
		char currTime[10];

		timer = time( NULL );
		timePtr = localtime( &timer );
		strftime( currTime ,10,  "%H:%M:%S", timePtr);

		// 15 = 10 + 1 + 3 + 1
		sRetVal = (char*)malloc( 15 );
		
		sprintf( sRetVal, "%s.%3u", currTime, timebuf.millitm );
		return sRetVal;	
	}

	/// writes the list of variable arguments to the log file
	inline void varWriteToLog(LogType type, char *szTypes, ... )
	{
		va_list vl;
		va_start( vl, szTypes );

		if( !bFileOpen ) return;
		char *currTime = getCurrTime();

		if( !fpLog )
		{
			fpLog = fopen( config.getLogFileName().c_str(), "at" );
		}

		switch(type){
			case _INFO:
				fprintf( fpLog, "[%s] %s [", "INFO", currTime );
				break;
			case _WARNING:
				fprintf( fpLog, "[%s] %s [", "WARNING", currTime );
				break;
			case _ERROR:
				fprintf( fpLog, "[%s] %s [", "ERROR", currTime );
				break;
		}

		// Step through the list of variable args
		for( int i = 0; szTypes[i] != '\0'; ++i )
		{
			union Printable_t
			{
				int   i;
				double  f;
				char    c;
				char    *s;
			} Printable;

			switch( szTypes[i] )    // Type to expect
			{
				case 'i':
				Printable.i = va_arg( vl, int );				
				fprintf( fpLog, " %i", Printable.i );
				break;

				case 'f':
				Printable.f = va_arg( vl, double );				
				fprintf( fpLog, " %lf", Printable.f );
				break;

				case 'c':
				Printable.c = va_arg( vl, char );				
				fprintf( fpLog, " %c", Printable.c );
				break;

				case 's':
				Printable.s = va_arg( vl, char * );
				fprintf( fpLog, " %s", Printable.s );
				break;

				default:
				break;
			}
		}

		fprintf( fpLog, " ]\n" );
		va_end( vl );
		free( currTime );
	}

};

typedef enum { PPM, JPEG } ImgType;

class CGIReply
{
public:

	CGIReply(){}

	static void replyText(std::string text);
	static void replyImage(const char* image, ImgType type, int length);
	//static void replyXML();
};

#endif CONFIGURE_H