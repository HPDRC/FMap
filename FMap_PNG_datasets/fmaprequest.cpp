#include "fmap.h"
#include "FMapRequest.h"
#include <algorithm>
#include <cstring>
//#include <cstringt.h>
#include <io.h>
#include <cmath>
#include <iostream>
//#include <list>
#include "stdafx.h"
using namespace std;

Pair::Pair(string rPair)
{
	int idx = (int)(rPair.find('='));
	if(idx != string::npos)
	{
		requestPair = make_pair(rPair.substr(0, idx), rPair.substr(idx+1));
	}
}

string Pair::getValue()
{
	return requestPair.second;
}

string Pair::getName()
{
	return requestPair.first;
}

RequestString::RequestString(const char* requestString)
{
	string rString(requestString);
	rString += '&';	// sentinel
	int idx1 = 0;
	int idx2 = 0;
	idx1 = (int)(rString.find('&', idx1));
	while(idx1 != string::npos)
	{
		request.push_back(Pair(rString.substr(idx2, idx1-idx2)));
		idx2 = idx1+1;
		idx1 = (int)(rString.find('&', idx2));
	}		

}

bool binPred(const char a, const char b)
{
	return toupper(a) == toupper(b);
}

string RequestString::getValue(string name)
{
	for(int i = 0; i < (int)(request.size()); i++)
	{
		//if( request[i].getName() == name )
		//	return request[i].getValue();
		string req = request[i].getName();
		if(req.size() < name.size())
		{
			if(equal(req.begin(), req.end(),
				name.begin(), binPred))
				return request[i].getValue();
		}
		else
		{
			if(equal(name.begin(), name.end(),
				req.begin(), binPred))
				return request[i].getValue();
		}
		
	}

	return "";
}

Overlays::Overlays(string overlay)
{
	overlay += '+';	// sentinel
	int idx1 = 0;
	int idx2 = 0;
	idx1 = (int)(overlay.find('+'));
	while(idx1 != string::npos)
	{
		overlays.push_back(overlay.substr(idx2, idx1-idx2));
		idx2 = idx1 + 1;
		idx1 = (int)(overlay.find('+', idx2));
	}

	cur = overlays.begin();
}

string Overlays::getNextOverlay()
{
	if(cur != overlays.end())
		return *cur++;
	else
	{
		cur = overlays.begin();
		return "";
	}
}

void Rule::apply(Overlays& overlays, double res)
{
	string reqOverlay = overlays.getNextOverlay();//keep reqOverlay == original request
	string aOverlay = reqOverlay;

	while(aOverlay != "")
	{
		int iDot = (int) (aOverlay.find('.'));
		if(iDot != string::npos)
		{
			aOverlay = aOverlay.substr(0, iDot);
		}

		if(aOverlay == overlay && (res < range.first || res >= range.second))
		{
			overlays.remove(reqOverlay);//remove original request from overlays
		}

		reqOverlay = overlays.getNextOverlay();//keep reqOverlay == original request
		aOverlay = reqOverlay;
	}
}

void CompositeRule::apply(Overlays& overlays, double res)
{
	list<Rule>::iterator it = rules.begin();
	while(it != rules.end())
	{
		it->apply(overlays, res);
		++it;
	}
}

vector<string> Overlay2Qix::getQixFile(string levelOne, string levelTwo)
{
	vector<string> ret;
#ifdef TERRA_NODE
	//char path[256] = "C:\\degree\\";
	//char path[256] = "E:\\degree\\";
	char path[256] = DISK_LOCATION_DEGREE;
#else
	//char path[256] = "C:\\degree\\";
	char path[256] = DISK_LOCATION_DEGREE_LOCAL;
#endif

	if( "biz" == overlay  )
	{
		overlay = "business";
	}
	if( "realestate" == overlay )
	{
		overlay = "real";
	}
	/*if( "city" == overlay )
	{
		overlay = "megap";
	}*/
	if( "obfsn" == overlay )
	{
        overlay = "obfsz";
	}
	if( "ptrim" == overlay )
	{
        overlay = "ptrim4";
	}
	/*if( "ptrim" == overlay )
	{
        overlay = "ptrim2";
	}*/
	if( "cndr" == overlay )
	{
        overlay = "cnd2r";
	}
	if( "gnis"  == overlay )
	{
		overlay = "gnis2";
	}
	if( "points"  == overlay )
	{
		overlay = "points2";
	}
	if( "wtown" == overlay )
	{
		overlay = "wtown3";
	}
	if( "ypage" == overlay )
	{
		overlay = "ypage2";
	}
	//if( ("seas" == overlay.substr(0,4)) && (overlay.at(5) != NULL) )
	//{
	//	return ret;
	//}
	/*if( "wtown" == overlay )
	{
		overlay = "rwtown3";
	}*/
	if( "blanklayer" == overlay || 
		"all"        == overlay || 
		"ramb"       == overlay ||
		"seas"       == overlay )
	{
		return ret;
	}

	/*if( "newsea" == overlay )
	{
		char newsea[256] = {0};
		strcpy(newsea, path);
		strcat(newsea, "new_area\\newsea\\newsea.qix");
		ret.push_back(string(newsea));
		return ret;
	}*/

	if(levelOne.find(overlay) != string::npos)	// under level one dir
	{
		pair<double, double> rangeLong = getDirName(longitude1, longitude2, 0.5);
		pair<double, double> rangeLat = getDirName(latitude1, latitude2, 0.5);

		while(rangeLat.first <= rangeLat.second)
		{
			char lat[16] = {0};
			sprintf(lat, "%3.1lf", rangeLat.first);
			double start = rangeLong.first;
			double end = rangeLong.second;
			while(start <= end)
			{
				char longi[16] = {0};
				char temp[256] = {0};
				strcpy(temp, path);
				sprintf(longi, "%3.1lf", start);

				///////////////////////////////////////////
				//if( overlay == "biz" )
				//{
				//	strcat(temp, "business");
				//}
				//else if( overlay == "realestate" )
				//{
				//	strcat(temp, "real");
				//}
				//else
				{
					strcat(temp, overlay.c_str());
				}
				///////////////////////////////////////////

				//strcat(temp, overlay.c_str()); //before update


				strcat(temp, "\\");
				if(overlay == "ramb_auto" || overlay == "real")
					strcat(temp, "0\\");
				strcat(temp, lat); strcat(temp, "\\");
				strcat(temp, longi); strcat(temp, ".qix");

				// check file existence
				if(_access(temp, 4) == 0)
				{
					// exists and has read permission
					ret.push_back(string(temp));	   	   	  
				}
				else
				{
					// log error!
				}

				start += 0.5;
			}

			rangeLat.first += 0.5;
		}
	}
	else if(overlay.find("street") != string::npos)	// goto street dir
	{
#ifdef TERRA_NODE
		//char dir[256] = "C:\\streetdata\\";
		//char dir[256] = "E:\\streetdata\\";
		char dir[256] = DISK_LOCATION_STREET;
#else
		//char dir[256] = "C:\\streetdata\\";
		char dir[256] = DISK_LOCATION_STREET_LOCAL;
#endif


		int streets = 0;
		int sechwys = 0;
		int majhwys = 0;
		int tigerstreetdata = 0;

		if(overlay == "street" || overlay == "streetn")
		{
			if(resolution < 4.0)	// streets
			{
				char temp[256] = {0};
				strcpy(temp, dir);
				strcat(temp, "streets\\");

				streets = checkAndAddFile(ret, temp);
			}

			if(resolution < 64.0)
			{
				char temp[256] = {0};
				strcpy(temp, dir);
				strcat(temp, "Sechwys\\");

				sechwys = checkAndAddFile(ret, temp);
			}

			char temp[256] = {0};
			strcpy(temp, dir);
			strcat(temp, "MajHwys\\");
			majhwys = checkAndAddFile(ret, temp);
		}


		////0 means there are no Nachtech data, will check tiger data
		tigerstreetdata = streets + sechwys + majhwys;
        //tigerstreetdata = 0;

		if( (overlay == "street" || overlay == "streett") && (0 == tigerstreetdata) )
		{
			if( (resolution < 4.0) && (resolution > 0.6) )
			{
				char temp[256] = {0};
				strcpy(temp, dir);
				strcat(temp, "detailroads\\");

				checkAndAddFile(ret, temp);
			}

			if( resolution > 1.0 )
			{
				strcat(dir, "Mainroads\\");
				checkAndAddFile(ret, dir);
			}
			/*strcat(dir, "Mainroads\\");
			checkAndAddFile(ret, dir);*/
		}

	}
	else 
	{
		// assume under level two directory
		char dir[256] = {0};
		strcpy(dir, path);

		int idx1 = 0;
		int idx2 = 0;
		levelTwo += ';'; // sentinel
		idx2 = (int)(levelTwo.find(';'));
		while(idx2 != string::npos)
		{
			char temp[256] = {0};
			strcpy(temp, dir);
			// go through the level two dir one by one, check if match
			strcat(temp, levelTwo.substr(idx1, idx2-idx1).c_str());
			strcat(temp, "\\");
			strcat(temp, overlay.c_str());
			strcat(temp, "\\");
			if(_access(temp, 0) == 0)
			{
				checkAndAddFile(ret, temp);
				break;
			}

			idx1 = idx2 + 1;
			idx2 = (int)(levelTwo.find(';', idx1));
		}
	}

	return ret;
}

pair<double, double> Overlay2Qix::getDirName(double head, double tail, double dTokenizer)
{
	if(head > tail)
		swap(head, tail);
	
	double start = 0;
	double end = 0;
	double temp = floor(head);
	if(head >= temp + dTokenizer)
		start = temp + dTokenizer;
	else
	{
		//start = temp;
		if( temp < 0 )
		{
			double nnn = temp/dTokenizer;
			temp = floor(temp/dTokenizer);
			start = temp * dTokenizer;
		}
		else
		{
			start = ((int) (temp/dTokenizer))*dTokenizer;
		}
	}
		

	temp = floor(tail);
	if(tail >= temp + dTokenizer)
		end = temp + dTokenizer;
	else
	{
		//end = temp;
		//end = ((int) (temp/nTokenizer))*nTokenizer;
		if( temp < 0 )
		{
			temp = floor(temp/dTokenizer);
			end = temp * dTokenizer;
		}
		else
		{
			end = ((int) (temp/dTokenizer))*dTokenizer;
		}
	}
		

	return make_pair(start, end);
}

int Overlay2Qix::checkAndAddFile(vector<string>& files, char* path)
{
	char temp[256] = {0};
	strcpy(temp, path);
	//pair<double, double> rangeLong = getDirName(longitude1, longitude2);
	//pair<double, double> rangeLat = getDirName(latitude1, latitude2);

	//bool isFileExist = false;

	//while(rangeLat.first <= rangeLat.second)
	//{
	//	char lat[16] = {0};
	//	sprintf(lat, "%3.1lf", rangeLat.first);
	//	double start = rangeLong.first;
	//	double end = rangeLong.second;
	//	while(start <= end)
	//	{
	//		char longi[16] = {0};
	//		char dir[256] = {0};
	//		strcpy(dir, temp);
	//		sprintf(longi, "%3.1lf", start);
	//		strcat(dir, lat); strcat(dir, "\\");
	//		strcat(dir, longi); strcat(dir, ".qix");

	//		// check file existence
	//		if(_access(dir, 4) == 0)
	//		{
	//			// exists and has read permission
	//			files.push_back(string(dir));	
	//			isFileExist = true;
	//		}
	//		else
	//		{
	//			// log error!
	//		}

	//		start += 0.5;
	//	}

	//	rangeLat.first += 0.5;
	//}


	double dTokenizer1 = 0.5;
	double dTokenizer2 = 2.0;
	double dTokenizer3 = 4.0;
    double dTokenizer4 = 6.0;

	double dTokenizer  = 0.0;

	double dTokenizer8 = 8.0;

	double dTokenizer16 = 16;



	for(int i=0; i<6; i++)
	{
		if( 0 == i )
		{
			dTokenizer = dTokenizer1;
		}
		else if( 1 ==i )
		{
			dTokenizer = dTokenizer2;
		}
		else if( 2 == i )
		{
			dTokenizer = dTokenizer3;
		}
		else if( 3 == i )
		{
			dTokenizer = dTokenizer4;
		}
		else if( 4 == i )
		{
			dTokenizer = dTokenizer8;
		}
		else if( 5 ==i )
		{
			dTokenizer = dTokenizer16;
		}
		else
		{
			return 0;
		}

		pair<double, double> rangeLong = getDirName(longitude1, longitude2, dTokenizer);
		pair<double, double> rangeLat  = getDirName(latitude1,  latitude2,  dTokenizer);

		bool isFileExist = false;

		while(rangeLat.first <= rangeLat.second)
		{
			char lat[16] = {0};
			sprintf(lat, "%3.1lf", rangeLat.first);
			double start = rangeLong.first;
			double end = rangeLong.second;
			while(start <= end)
			{
				char longi[16] = {0};
				char dir[256] = {0};
				strcpy(dir, temp);
				sprintf(longi, "%3.1lf", start);
				strcat(dir, lat); strcat(dir, "\\");
				strcat(dir, longi); strcat(dir, ".qix");

				// check file existence
				if(_access(dir, 4) == 0)
				{
					// exists and has read permission
					files.push_back(string(dir));	
					isFileExist = true;
					return 1;
				}
				else
				{
					// log error!
				}

				if(dTokenizer == dTokenizer1)
				{
					start += 0.5;
				}
				else if(dTokenizer == dTokenizer2)
				{
					start += 4.0;
				}
				else if(dTokenizer == dTokenizer3)
				{
					start += 2.0;
				}
				else if(dTokenizer == dTokenizer4)
				{
					start += 6.0;
				}
				else if(dTokenizer == dTokenizer8)
				{
					start += 8.0;
				}
				else if(dTokenizer == dTokenizer16)
				{
					start += 16.0;
				}

				//start += 0.5;
			}

			//rangeLat.first += 0.5;
			if(dTokenizer == dTokenizer1)
			{
				rangeLat.first += 0.5;
			}
			else if(dTokenizer == dTokenizer2)
			{
				rangeLat.first += 4.0;
			}
			else if(dTokenizer == dTokenizer3)
			{
				rangeLat.first += 2.0;
			}
			else if(dTokenizer == dTokenizer4)
			{
				rangeLat.first += 6.0;
			}
			else if(dTokenizer == dTokenizer8)
			{
				rangeLat.first += 8.0;
			}
			else if(dTokenizer == dTokenizer16)
			{
				rangeLat.first += 16.0;
			}
		}

	}


	return 0;
	/*if( isFileExist )
	{
		return 1;
	}
	else
	{
		return 0;
	}*/
}

bool operator==(const Rule& r1, const Rule& r2)
{
	return r1.overlay==r2.overlay && r1.range==r2.range;
}

/*
int main()
{
	RequestString s("XN=142&YN=914&RES=8.000000&UTMZ1=17&COMPOSITE=points2+street._n+hotel+all+dining+biz+re1n+re2n+rntn+rinn+rldn+cldn+comn+busn+realtor+wtown3+ptrim4+nfires4");
	cout<<"XN "<<s.getValue("XN")
		<<"yn "<<s.getValue("YN")
		<<"res "<<s.getValue("RES")
		<<"UTMZ "<<s.getValue("UTMZ1")
		<<"COMPOSITE"<<s.getValue("COMPOSITE");

	Overlays o(s.getValue("COMPOSITE"));
	string overlay;
	while((overlay = o.getNextOverlay()) != "")
		cout<<overlay<<endl;
	
}*/