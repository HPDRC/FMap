#include "configure.h"
//#include "stdafx.h"
using namespace std;

Configure::Configure(const char* fName/*="fmap_config.txt"*/)
{
	FILE* fp = fopen(fName, "r");


	if(fp != NULL)
	{
		char line[128] = {0};
		while(fgets(line, 128, fp) != NULL)
		{
			if(line[0] == '#' || line[0] == '\n')
				continue;

			char name[32] = {0};
			sscanf(line, "%s", name);
			if(strcmp(name, "LOGFILE") == 0)
			{
				char log[32] = {0};
				sscanf(line+strlen(name)+1, "%s", log);
				logFileName = string(log);
			}
			else if(strcmp(name, "LEVELONE") == 0)
			{
				char levelOne[256];
				sscanf(line+strlen(name)+1, "%s", levelOne);
				levelOneDir = string(levelOne);
			}
			else if(strcmp(name, "LEVELTWO") == 0)
			{
				char levelTwo[256];
				sscanf(line+strlen(name)+1, "%s", levelTwo);
				levelTwoDir = string(levelTwo);
			}
			//else if(strcmp(name, "FONTHEIGHT") == 0)
			//{
			//	int height = 0;
			//	sscanf(line+strlen(name)+1, "%d", &height);
			//	fontHeight = height;
			//}
			//else if(strcmp(name, "FONTWIDTH") == 0)
			//{
			//	sscanf(line+strlen(name)+1, "%d", &fontWidth);
			//}
			//else if(strcmp(name, "BOLD") == 0)
			//{
			//	int ibold = 0;
			//	sscanf(line+strlen(name)+1, "%d", &ibold);
			//	if(0 == ibold)
			//	{
			//		bold = false;
			//	}
			//	else
			//	{
			//		bold = true;
			//	}
			//}
			//else if(strcmp(name, "ITALIC") == 0)
			//{
			//	int iitalic = 0;
			//	sscanf(line+strlen(name)+1, "%d", &iitalic);
			//	if(0 == iitalic)
			//	{
			//		italic = false;
			//	}
			//	else
			//	{
			//		italic = true;
			//	}
			//}
			else
			{}
		}

		fclose(fp);

	}
	else
	{
		// log error
		logFileName = "fmap_log.txt";
		levelOneDir = "business;dining;hotel;zip;ramb_auto;real";
		levelTwoDir = "line;new_area;point";
		fontHeight = 10;
		fontWidth = 10;
		bold = true;
		italic = false;
	}

}

void Configure::show()
{
	fprintf(stdout, "Fmap's configuration is as follow:\n");
	fprintf(stdout, "logFile: %s\n", logFileName.c_str());
	fprintf(stdout, "level one dir: %s\tlevel two dir: %s\n", levelOneDir.c_str(), levelTwoDir.c_str());
	fprintf(stdout, "font height: %d\tfont width: %d\tbold: %d\titalic: %d\n",
		fontHeight, fontWidth, bold, italic);
}

void CGIReply::replyText(std::string text)
{
	printf("Content-type: text/plain\n\n");
	printf(text.c_str());
}


void CGIReply::replyImage(const char* image, ImgType type, int length)
{
	if(type == PPM)
		printf("Content-type: image/ppm\n\n");
	if(type == JPEG)
		printf("Content-type: image/jpeg\n\n");

	fwrite(image, length, 1, stdout);
}

//void CGIReply::replyXML()
//{
//	printf("Content-type: text/xml\n\n");
//
//
//}

/*
int main()
{
	Configure c;
	c.show();

	Log l(c);
	l.initLogObj(c);
	l.writeToLog(INFO, "TEST ONLY");
	l.varWriteToLog(WARNING, "sfi", "test", 3.98, 98);
}*/