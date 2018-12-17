////#include "stdafx.h"
//#include "configure.h"
//#include "shaperecords.h"
//#include "two_convert.h"
//#include "maptree.h"
//#include "requeststring.h"
//#include "fmap.h"
//#include "ImageTransformer.h"
//#include <iostream>
////#include <cstring>
//using namespace std;
//
//Configure configure;
//Log logObj(configure);
//
///*
//int main()
//{
//	logObj.initLogObj(configure);
//
//	logObj.varWriteToLog(_INFO, "ss", "Start a new record: ", getenv("QUERY_STRING"));
//
//#ifdef DEBUG
//	fprintf(stderr, "Request String: %s\n", getenv("QUERY_STRING"));
//#endif
//
//	//RequestString requestString(getenv("QUERY_STRING"));
//
//	RequestString requestString("XN=142&YN=914&RES=8.000000&UTMZ1=17&COMPOSITE=points2+street._n+hotel+all+dining+biz+re1n+re2n+rntn+rinn+rldn+cldn+comn+busn+realtor+wtown3+ptrim4+nfires4");
//
//	/*Overlay2Qix o2q("street", -67.4, -67.1, 17.6, 18.3, 3.0);
//	vector<string> files;
//	files = o2q.getQixFile(configure.getLevelOneDir(), configure.getLevelTwoDir());
//	cout<<files[0]<<endl
//		<<files[1]<<endl;
//	cout<<"size "<<files.size()<<endl;*/
//
///*	string comp = requestString.getValue("COMPOSITE");
//	string XN = requestString.getValue("XN");
//	string YN = requestString.getValue("YN");
//
//	rectObj rect;
//	if(XN == "" || YN == "")
//	{
//		string X1 = requestString.getValue("X1");
//		string X2 = requestString.getValue("X2");
//		string Y1 = requestString.getValue("Y2");
//		string Y2 = requestString.getValue("Y2");
//
//		if(X1 == "" || X2 == "" || Y1 == "" || Y2 == "")
//		{
//			CGIReply::replyText("Error: XN YN or X1 Y1 parameter not specified!");
//			exit(1);
//		}
//
//		string UTMZ1 = requestString.getValue("UTMZ1");
//		string UTMZ2 = requestString.getValue("UTMZ2");
//
//		if(UTMZ1 == "" || UTMZ2 == "")
//		{
//			CGIReply::replyText("Error: UTMZ1 or UTMZ2 parameter not specified!");
//			exit(1);
//		}
//
//		double x1,x2,y1,y2;
//		int utmz1, utmz2;
//
//		sscanf(X1.c_str(), "%lf", &x1);
//		sscanf(Y1.c_str(), "%lf", &y1);
//		sscanf(X2.c_str(), "%lf", &x2);
//		sscanf(Y1.c_str(), "%lf", &y2);
//
//		sscanf(UTMZ1.c_str(), "%d", &utmz1);
//		sscanf(UTMZ2.c_str(), "%d", &utmz2);
//
//		Utm_Coord_3d utmCoord1(x1, y1, 0, utmz1, false/*not sure*/);
//		Utm_Coord_3d utmCoord2(x2, y2, 0, utmz2, false);
//
//		Gdc_Coord_3d gdc1;
//		Gdc_Coord_3d gdc2;
//
//		Utm_To_Gdc_Converter::Init();
//		Utm_To_Gdc_Converter::Convert(utmCoord1, gdc1);
//		Utm_To_Gdc_Converter::Convert(utmCoord2, gdc2);
//
//		rect.minX = gdc1.longitude;
//		rect.minY = gdc2.latitude;
//		rect.maxX = gdc2.longitude;
//		rect.maxY = gdc1.latitude;
//	}
//
//	string UTMZ = requestString.getValue("UTMZ1");
//	if(UTMZ == "")
//	{
//		CGIReply::replyText("Error: UTMZ parameter not specified!");
//		exit(1);
//	}
//
//	int xn,yn;
//	int utmz;
//
//	sscanf(XN.c_str(), "%d", &xn);
//	sscanf(YN.c_str(), "%d", &yn);
//	sscanf(UTMZ.c_str(), "%d", &utmz);
//
//	string RES = requestString.getValue("RES");
//	if(RES == "")
//	{
//		CGIReply::replyText("Error: Resolution parameter not specified!");
//		exit(1);
//	}
//
//	double x1,x2,y1,y2,res;
//	sscanf(RES.c_str(), "%lf", &res);
//	
//	x1 = xn*TILE_SIZE*res;	// 400 is the tile size
//	x2 = (xn+1)*TILE_SIZE*res;
//	y1 = yn*TILE_SIZE*res;
//	y2 = (yn-1)*TILE_SIZE*res;
//
//	Utm_Coord_3d utmCoord1(x1, y1, 0, utmz, true/*not sure*/);
//	Utm_Coord_3d utmCoord2(x2, y2, 0, utmz, true);
//
//	Gdc_Coord_3d gdc1;
//	Gdc_Coord_3d gdc2;
//
//	Utm_To_Gdc_Converter::Init();
//	Utm_To_Gdc_Converter::Convert(utmCoord1, gdc1);
//	Utm_To_Gdc_Converter::Convert(utmCoord2, gdc2);
//
//	rect.minX = gdc1.longitude;
//	rect.minY = gdc2.latitude;
//	rect.maxX = gdc2.longitude;
//	rect.maxY = gdc1.latitude;
//
//#ifdef DEBUG
//	fprintf(stderr, "Composite: %s\n", comp.c_str());
//#endif
//
//	Overlays overlays(comp);
//
//	string overlay = overlays.getNextOverlay();
//	vector<string> files;
//	vector<ShapeRecords> shapeRecords;
//	while(overlay != "")
//	{
//		TransInfo transInfo;
//		OverridedColor color;
//		ShapeRecords shapeRecord;
//
//		// trim overlay, first find .xxx, then -xxx
//		int idx = overlay.find('.');
//		if(idx != string::npos)
//		{
//			// trans or override info exists
//			string info = overlay.substr(idx+1);
//			overlay = overlay.substr(0, idx);
//			switch(info.size()){
//				case OVERRIDE_COLOR_LENGTH:	// line, label override color
//					color.line = info.substr(0, 6);
//					color.label = info.substr(7);
//					shapeRecord.setColorInfo(color);
//					break;
//				case OVERRIDE_COLOR_LENGTH/2:		// line overrided color
//					color.line = info.substr(0, 6);
//					shapeRecord.setColorInfo(color);
//					break;
//				case AREA_TRANSPARENCY_LENGTH:		// area transparency info
//					transInfo.type = TransInfo::area;
//					transInfo.values[0] = transInfo.char2int(info[0]);
//					transInfo.values[1] = transInfo.char2int(info[1]);
//					transInfo.values[2] = transInfo.char2int(info[2]);
//					shapeRecord.setTransInfo(transInfo);
//			
//					break;
//				case LINE_TRANSPARENCY_LENGTH:		// line transparency info
//					transInfo.type = TransInfo::line;
//					transInfo.values[0] = transInfo.char2int(info[0]);
//					transInfo.values[1] = transInfo.char2int(info[1]);
//					shapeRecord.setTransInfo(transInfo);
//			}
//		}
//
//		// find -xxx
//		idx = overlay.find('-');
//		if(idx != string::npos)
//		{
//			// two digits which decide the visibility of the overlay
//			string visib = overlay.substr(idx+1);
//			overlay = overlay.substr(0, idx);
//
//			pair<int, int> range;
//			range.first = TransInfo::char2int(visib[0]);
//			range.second = TransInfo::char2int(visib[1]);
//
//			if(res < range.first || res >= range.second)	// should not request this overlay, drop it
//			{
//				overlay = overlays.getNextOverlay();
//				continue;
//			}
//		}
//
//		Overlay2Qix o2q(overlay, rect.minX, rect.maxX, rect.minY, rect.maxY, res);
//		
//		files = o2q.getQixFile(configure.getLevelOneDir(), configure.getLevelTwoDir());
//
//		for(int i = 0; i < files.size(); i++)
//		{
//			char* ids = NULL;
//			ShapeRecords record = shapeRecord;
//
//			string baseName = files[i].substr(0, files[i].length()-3);
//			record.setShpBaseName(baseName);
//			
//			char name[MAX_PATH_LENGTH] = {0};
//			strcpy(name, baseName.c_str());
//			ids = msSearchDiskTree(name, rect, 1);
//
//			int number, type;
//			double min[4];
//			double max[4];
//			SHPHandle handle = SHPOpen(name, "rb");
//			SHPGetInfo(handle, &number, &type, min, max);
//
//			msFilterTreeSearch(handle, number, ids, rect, record);
//
//			SHPClose(handle);
//
//			record.setShpType(type);
//			shapeRecords.push_back(record);
//		}
//
//		overlay = overlays.getNextOverlay();
//	}
//
//	// draw map
//	
//
//
//	// out put to cgi
///*	ImageTransformer imgTrans(img);
//
//	int width = img.GetWidth();
//	int height = img.GetLength();
//	char head[32] = "P6 ";
//	sprintf(head, "%d %d 255\n", width, height);
//	int length = strlen(head);
//
//	char* buf = new char[length+width*height*3];
//	if(buf == NULL)
//	{
//		// log error;
//		exit(1);
//	}
//
//	strcpy(buf, head);
//	imgTrans.img2ppm(buf+length);
//	CGIReply::replyImage(buf, PPM, length+width*height*3);*/
//
//	return 0;
//}