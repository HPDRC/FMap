;// FMapDraw.cpp : Defines the entry point for the console application.
//


#define _CRTDBG_MAP_ALLOC
//#include<stdlib.h>
#include "stdafx.h"
#include "shapeObject.h"
#include "LayerDataSource.h"
#include "QixShapeFileLayerDataSource.h"
#include "StringLayerDataSource.h"
#include "PolygonLayer.h"
#include "PointLayer.h"
#include "lineshape.h"
#include "lineLayer.h"
#include "MapCanvas.h"
#include "CoordinateTransformer.h"
#include "shaperecords.h"
#include "TransInfoManager.h"
#include "configure.h"
#include "two_convert.h"
#include "maptree.h"
#include "FMapRequest.h"
#include "fmap.h"
#include "ImageTransformer.h"
#include <iostream>
#include <string>
#include <queue>
#include "atlutil.h"
#include <atlbase.h>
#include <atlstr.h>
#include "FMapDraw.h"
#include "placehoder.h"
#include <crtdbg.h> 
#include <math.h>
#include "FMapRequestManager.h"
#include ".\\Utility\\Transformer.h"
#include "DataBaseLayerDataSource.h"


Configure configure;
Log logObj(configure);



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;


using namespace std;


//void DrawArea(double dX1, double dY1, double dX2, double dY2, char sShpSrcFile[], char sFieldName[], CDC* pDC);
//bool MergerStreet( ,double dX1, double dY1, double dX2, double dY2)

double CLineShape::m_dConnectAcceptOffsetX = 0.0001; // here is hard code
double CLineShape::m_dConnectAcceptOffsetY = 0.0001; // here is hard code
CPlacehoder g_placeHolder;	



int main(/*int argc, TCHAR* argv[], TCHAR* envp[]*/)
{
	/*void* ptr = calloc(480000, 1);
    printf("Content-type: image/ppm\n\n");
	fwrite(ptr,480000,1,stdout);
	free(ptr);
	exit(0);*/
//test environment above

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#ifdef UNICODE
	ASSERT(FALSE);
#else

#endif

	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		logObj.initLogObj(configure);

		logObj.varWriteToLog(_INFO, "ss", "Start a new record: ", getenv("QUERY_STRING"));

#ifdef DEBUG
		fprintf(stderr, "Request String: %s\n", getenv("QUERY_STRING"));
#endif

#ifdef TERRA_NODE
		RequestString requestString(getenv("QUERY_STRING"));
#else
		//RequestString requestString("XN=142&YN=914&RES=8.000000&UTMZ1=17&COMPOSITE=points2+street._n+hotel+all+dining+biz+re1n+re2n+rntn+rinn+rldn+cldn+comn+busn+realtor+wtown3+ptrim4+nfires4");
		//XN=367&YN=1783&RES=4.000000&UTMZ1=17&COMPOSITE=state2.vv0
		//RequestString requestString("XN=366&YN=1783&RES=8.000000&UTMZ1=17&COMPOSITE=re1n");
		//RequestString requestString("XN=367&YN=1783&RES=4.000000&UTMZ1=17&COMPOSITE=state2.vv0");
		//RequestString requestString("XN=366&YN=1783&RES=4.000000&UTMZ1=17&COMPOSITE=street._n");
		//RequestString requestString("X1=585600&X2=588200&Y1=2853800&Y2=2851200&RES=2.000000&UTMZ1=17&UTMZ2=17&COMPOSITE=street._n+hotel+dining");
		//terranode-171.cs.fiu.edu/scripts/fmap-2006.exe?XN=1175&YN=10995&RES=1.200000&UTMZ1=10&COMPOSITE=street._n
		//RequestString requestString("XN=589&YN=5495&RES=2.400000&UTMZ1=10&COMPOSITE=streetn.aaaaaaffff00");//has bugs need to fix
		//RequestString requestString("XN=366&YN=1783&RES=4.000000&UTMZ1=17&COMPOSITE=hotel+dining+biz+re1n+re2n+rntn+rinn+rldn+cldn+comn+busn+wtown3+ptrim4+nfires4");
		//XN=142&YN=914&RES=8.000000&UTMZ1=17&COMPOSITE=points2+street._n+hotel+all+dining+biz+re1n+re2n+rntn+rinn+rldn+cldn+comn+busn+realtor+wtown3+ptrim4+nfires4//for point test


		//RequestString requestString("X1=324153.919&X2=324869.464&Y1=4306411.98&Y2=4305859.981&RES=2.4&UTMZ1=18&UTMZ2=18&COMPOSITE=street.ln");
		//RequestString requestString("X1=322729.919&X2=323498.464&Y1=4305345.98&Y2=4304692.981&RES=2.400000&UTMZ1=18&UTMZ2=18&COMPOSITE=street.ln");
		//RequestString requestString("X1=316166.919&X2=324636.464&Y1=4307985.98&Y2=4295943.981&RES=2.400000&UTMZ1=18&UTMZ2=18&COMPOSITE=street.ln");
		//http://terranode-171.cs.fiu.edu/scripts/fmap.exe?X1=316166.919&X2=324636.464&Y1=4307985.98&Y2=4295943.981&RES=4.800000&UTMZ1=18&UTMZ2=18&COMPOSITE=street._n+hotel+dining+biz+re1n+re2n+rntn+rinn+rldn+cldn+comn+busn+realtor+wtown3+ptrim4+nfires4
		//RequestString requestString("XN=366&YN=1783&RES=4.000000&UTMZ1=17&COMPOSITE=street._n");
		//RequestString requestString("X1=316166.919&X2=334636.464&Y1=4317985.98&Y2=4295943.981&RES=4.800000&UTMZ1=18&UTMZ2=18&COMPOSITE=re1n+re2n+rntn+rinn+rldn+cldn+comn+busn+wtown3+ptrim4+nfires4");//Washington DC
		//RequestString requestString("X1=586171&X2=586808&Y1=2852694&Y2=2852206&RES=1.2192&UTMZ1=17&UTMZ2=17&COMPOSITE=dining");//dining picto test
		//RequestString requestString("X1=455755&X2=460406&Y1=3153048&Y2=3150000&RES=4.8&UTMZ1=17&UTMZ2=17&COMPOSITE=hotel");//hotel star picto
		//RequestString requestString("X1=585902.0&X2=586365.0&Y1=2852751.0&Y2=2852375.0&RES=1.2192&UTMZ1=17&UTMZ2=17&COMPOSITE=points2+hotel+all+dining+biz+re1n+re2n+rntn+rinn+rldn+cldn+comn+busn+realtor+wtown3+ptrim4+nfires4");//test unknown picto
		//RequestString requestString("X1=586131.0&X2=586321.0&Y1=2852634.0&Y2=2852501.0&RES=1.2192&UTMZ1=17&UTMZ2=17&COMPOSITE=biz"); //biz point test
		//RequestString requestString("X1=321583&X2=323412&Y1=4306182&Y2=4304655&RES=4.800000&UTMZ1=18&UTMZ2=18&COMPOSITE=street.ln");

		//RequestString requestString("X1=455755&X2=460406&Y1=3153048&Y2=3150200&RES=4.8&sm=2.4&UTMZ1=17&UTMZ2=17&COMPOSITE=street.ln");
//		RequestString requestString("X1=455755&X2=460406&Y1=3153048&Y2=3150200&RES=4.8&SM=14.4&UTMZ1=17&UTMZ2=17&COMPOSITE=street.ln");
		//RequestString requestString("XN=166&YN=2245&RES=4.800000&UTMZ1=18&COMPOSITE=blkgrpy.111+street.ln");

		//RequestString requestString("XN=366&YN=1783&RES=4.000000&UTMZ1=17&COMPOSITE=hotel");
		//RequestString requestString("X1=454855&X2=455974&Y1=3150429&Y2=3149665&RES=4.800000&UTMZ1=17&UTMZ2=17&COMPOSITE=Composite=points2+street._n+hotel+all+dining+biz+re1n+re2n+rntn+rinn+rldn+cldn+comn+busn+wtown3+ptrim4+nfires4");
		//XN=808&YN=10767&RES=1.000000&UTMZ1=18&COMPOSITE=points2+street._n+hotel+all+dining+biz+re1n+re2n+rntn+rinn+rldn+cldn+comn+busn+wtown3+ptrim4+nfires4;

		//RequestString requestString("XN=808&YN=10767&RES=1.000000&UTMZ1=18&COMPOSITE=points2+street._n+hotel+all+dining+biz+re1n+re2n+rntn+rinn+rldn+cldn+comn+busn+wtown3+ptrim4+nfires4");


		//RequestString requestString("X1=322219.0&X2=324680.0&Y1=4307913.0&Y2=4305910.0&RES=4.8&UTMZ1=18&UTMZ2=18&COMPOSITE=all+hotel");
		//RequestString requestString("x1=324042.0&x2=326517.0&y1=4306871.0&y2=4304820.0&ReS=4.8&UTmZ1=18&UTMZ2=18&COMPOSiTE=all");

		//RequestString requestString("X1=502779.279&X2=537914.932&Y1=3634148.576&Y2=3603114.243&RES=4.0&UTMZ1=17&UTMZ2=17&COMPOSITE=nfires4");
		//RequestString requestString("X1=587018.726&X2=594568.5&Y1=2916365.294&Y2=2909655.929&RES=4.0&sm=2.0&UTMZ1=17&UTMZ2=17&COMPOSITE=busn");

//		RequestString requestString("X1=584492.805&X2=586408.135&Y1=2878227.234&Y2=2876113.235&TargetImageType=XmL&RES=4.0&UTMZ1=17&UTMZ2=17&COMPOSITE=comn");

		//RequestString requestString("X1=588867.0&X2=590970.0&Y1=3941585.0&Y2=3940005.0&RES=4.0&UTMZ1=16&UTMZ2=16&COMPOSITE=llfr");

		//RequestString requestString("X1=572451.635&X2=573111.177&Y1=3914656.201&Y2=3913984.966&RES=4.0&UTMZ1=16&UTMZ2=16&COMPOSITE=mulr");

		//RequestString requestString("X1=576106.609&X2=578222.075&Y1=2857564.69&Y2=2855760.757&RES=4.8&UTMZ1=17&UTMZ2=17&COMPOSITE=cldn");

		//RequestString requestString("X1=573843.278&X2=575729.531&Y1=2846487&Y2=2844660.151&RES=4.8&UTMZ1=17&UTMZ2=17&COMPOSITE=rldn");
		//RequestString requestString("X1=574961.673&X2=579969.674&Y1=2857978.979&Y2=2853545.134&RES=4.8&UTMZ1=17&UTMZ2=17&COMPOSITE=rinn");

		//RequestString requestString("X1=573183.15&X2=577930.993&Y1=2899877.976&Y2=2896139.926&RES=4.0&UTMZ1=17&UTMZ2=17&COMPOSITE=rntn");

		//RequestString requestString("X1=265598.287&X2=269423.56&Y1=3143254.631&Y2=3138810.774&RES=4.0&UTMZ1=18&UTMZ2=18&COMPOSITE=ptrim4");

		//RequestString requestString("X1=425592.415&X2=432273.458&Y1=5371476.068&Y2=5367245.452&RES=4.0&UTMZ1=12&UTMZ2=12&COMPOSITE=nfires4");

		//RequestString requestString("X1=278870.0&Y1=3532875.0&X2=605804.0&Y2=3124530.0&RES=1024&UTMZ1=17&UTMZ2=17&COMPOSITE=street.ln");

		//RequestString requestString("X1=494362.013&X2=552146&Y1=2771891.624&Y2=2728672&RES=64.000000&UTMZ1=17&UTMZ2=17&COMPOSITE=blkgroup.v0v");

		//RequestString requestString("X1=584937.425156012&Y1=2852084.763145525&X2=588367.425156012&Y2=2849272.763145525&Res=1.5000&UTMZ1=17&UTMZ2=17&composite=re1n+re2n+rinn+rldn+busn+comn+all+dining+streetn._n&sm=1.5");

		//RequestString requestString("XN=589&YN=5495&Res=2.4&TargetImageType=jpg&UTMZ1=10&Composite=streetn.ffffffffffff");

//		RequestString requestString("XN=589&YN=5495&Res=2.4&TargetImageType=XmL_UTM&UTMZ1=10&Composite=street.ln");

		//RequestString requestString("X1=721592.138&X2=724133.393&Y1=4216883.708&Y2=4214187.753&RES=4.0&UTMZ1=16&UTMZ2=16&COMPOSITE=realtor");

		//RequestString requestString("X1=552683.987&X2=556879.138&Y1=3971361.791&Y2=3967483.496&RES=4.0&UTMZ1=16&UTMZ2=16&COMPOSITE=cndr");

//		RequestString requestString("X1=586320.262872262&Y1=2856586.171729526&X2=589520.262872262&Y2=2854586.171729526&Res=4.0000&UTMZ1=17&UTMZ2=17&composite=bg.vat");//+bg_rent+bg_mhinc+bg_hyear+bg_hsize

		//RequestString requestString("X1=182831.265874&Y1=3435606.337559&X2=746031.265874&Y2=2708566.337559&RES=1024&UTMZ1=17&UTMZ2=17&COMPOSITE=city");

////		RequestString requestString("X1=560111.820971&Y1=2871131.609866&X2=611311.820971&Y2=2839131.609866&RES=64.000000&UTMZ1=17&UTMZ2=17&composite=zip.t0v");

		//RequestString requestString("X1=559457&Y1=3058321&X2=560057&Y2=3057921&UTMZ1=17&Source=usgs_AP_CIR1&TargetImageType=jpg&Res=16&Composite=zip.0u0");//problem

		//RequestString requestString("X1=587200.244949&Y1=2854271.639573&X2=588000.244949&Y2=2853771.639573&RES=1.000000&UTMZ1=17&COMPOSITE=gnis");
		//RequestString requestString("X1=559457&Y1=3058321&X2=560057&Y2=3057921&UTMZ1=17&Res=16&Composite=zip.0u0");


		//RequestString requestString("X1=582355.0&Y1=2862406.0&X2=590444.0&Y2=2856925.0&Res=8.0000&UTMZ1=17&composite=zip.v5t");//problem

		//RequestString requestString("X1=718014.2&Y1=3540067.4&X2=575512.5&Y2=3202014.8&Res=1024&UTMZ1=16&UTMZ2=17&composite=street.ln");//problem

		//RequestString requestString("X1=586320.262872262&Y1=2856586.171729526&X2=589520.262872262&Y2=2854586.171729526&TargetImageType=jpg&Res=4.0000&UTMZ1=17&UTMZ2=17&composite=bg");


		//RequestString requestString("x1=542810.46&x2=544081.92&y1=4180972.8&y2=4179369.5&res=1.2&UTMZ1=10&source=usgs_30cm_raw&composite=bg-__.aaa");

		////RequestString requestString("x1=542810.46&x2=544081.92&y1=4180972.8&y2=4179369.5&res=1.2&UTMZ1=10&Points=-122.50,37.76555,point1+-122.50,37.761000,point2+-122.5025,37.768000,point3&composite=gnis2-01");//37.755555 37.751000 37.758000 -122.50000000000 -122.55 -122.5825

		//RequestString requestString("x1=542810.46&x2=544081.92&y1=4180972.8&y2=4179369.5&TargetImageType=gMl_uTM&res=1.2&UTMZ1=10&composite=_-122.50,37.76555,$560_-122.50,37.761000,$660_-122.5025,37.768000,$760/$770/$780/@/Huan Wang_");

		//RequestString requestString("x1=586409.473&x2=587897.552&y1=2905995.867&y2=2904499.554&res=4.8768&UTMZ1=17&TargetImageType=gif&composite=cndx");

        //RequestString requestString("x1=542810.46&x2=544081.92&y1=4180972.8&y2=4179369.5&res=1.2&UTMZ1=10&TargetImageType=jpg&composite=street.ln");

        //RequestString requestString("TargetImageType=gif&X1=542810.460000&Y1=4180972.800000&X2=544081.920000&Y2=4179369.500000&RES=1.200000&UTMZ1=10&UTMZ2=10&COMPOSITE=_-122.50%2C37.76555%2C%24560_-122.50%2C37.761000%2C%24660_-122.5025%2C37.768000%2C%24HUAN_");


		//RequestString requestString("x1=587500.0&x2=589564.0&y1=2887953.0&y2=2886399.0&res=4.0&UTMZ1=17&TargetImageType=jpg&composite=hotel");

		//RequestString requestString("X1=585979.2831954404&X2=589051.2833175107&Y1=2858020.9565608264&Y2=2853220.9563700915&RES=9.6&UTMZ1=17&source=best_available&Composite=blkgrpy.t3v");

        //RequestString requestString("TargetImageType=gif&x1=322258.0&x2=324731.0&y1=4307885.0&y2=4305762.0&res=4&UTMZ1=18&composite=gnis2-4-4");

		//RequestString requestString("Xn=-16&Yn=-6&Res=0.0128&UTMZ1=0&COMPOSITE=obfs4+megap3+state4.s0v+ecregion3.0ss+ptrim4+z3aqq&Res=1024.0");


		//RequestString requestString("TargetImageType=gif&X1=557251.238&Y1=1101704.062&X2=557167.918&Y2=1100087.707&RES=4&UTMZ1=-30&UTMZ2=-30&COMPOSITE=cndx");


        //RequestString requestString("TargetImageType=gif&X1=587487.543954172&Y1=2855548.713461421&X2=588287.543954172&Y2=2855048.713461421&Res=1.0000&UTMZ1=17&composite=_-80.122586,25.813863,IBM,rddot_-80.122589,25.813864,825K,1_-80.123154,25.813353,190K,1_-80.123833,25.812693,2,2_");

		//RequestString requestString("TargetImageType=gif&Composite=FA_Point-_1.o&X1=623040&Y1=3364800&UTMZ1=14&X2=624000&Y2=3363840&UTMZ2=14&res=2.4");

		//RequestString requestString("TargetImageType=gif&Composite=wtown3&X1=679996.8&Y1=8563200&UTMZ1=-35&X2=681600&Y2=8561596.8&UTMZ2=-35&res=4.8");
        //RequestString requestString("TargetImageType=gif&Composite=wm_Major_Routes.ww&style=BT&XN=18&YN=94&res=76.8&UTMZ1=17&source=best_available");
		//RequestString requestString("TargetImageType=gif&Composite=street.ln&style=BT&XN=18&YN=94&res=76.8&UTMZ1=17&source=best_available");
		//RequestString requestString("TargetImageType=JPG&X1=545261&Y1=4264369&X2=594423&Y2=4223554&RES=16.0000&UTMZ1=14&composite=streetn.ln");
		//RequestString requestString("TargetImageType=JPG&X1=545261&Y1=4264369&X2=594423&Y2=4223554&RES=16.0000&UTMZ1=14&composite=street.ln");
		//RequestString requestString("X1=559933.447&Y1=3022168.416&X2=578462.452&Y2=2990946.036&Res=4.0000&UTMZ1=17&UTMZ2=17&composite=bg0.vat");

		//RequestString requestString("TargetImageType=gif&X1=561798.551869&Y1=2868324.694584&X2=601998.551869&Y2=2841524.694584&RES=67.000000&UTMZ1=17&UTMZ2=17&COMPOSITE=_-80.132202,25.813900,Start_-80.13406,25.813,Miami-Beach_-80.22414,25.7877,Miami-hotel,hotel_-80.24022,25.8213,%20,rddot_+blkgrpy.d0v+gnis.__");

		//RequestString requestString("TargetImageType=gif&X1=561798.551869&Y1=2868324.694584&X2=601998.551869&Y2=2841524.694584&RES=67.000000&UTMZ1=17&UTMZ2=17&COMPOSITE=gnis-__");

        //RequestString requestString("XN=324&YN=1752&Res=4.8&UTMZ1=14&Composite=_-97.718498,30.399797,IBM,rddot_");
		//RequestString requestString("XN=324&YN=1753&Res=4.8&UTMZ1=14&Composite=_-97.718498,30.399797,IBM,rddot_");

        //RequestString requestString("TargetImageType=jpg&X1=619661.2150249454&X2=625805.2152690861&Y1=3367624.225424796&Y2=3359944.22511962&res=16&UTMZ1=14&Composite=");

		//RequestString requestString("TargetImageType=jpg&X1=543856.041&X2=548619.318&Y1=2817728.068&Y2=2812572.991&res=10&UTMZ1=17&Composite=cndx+flproplines.aaa");

        //RequestString requestString("X1=303086.129977103&X2=320486.129977103&Y1=4311128.551458912&Y2=4299528.551458912&UTMZ1=18&TargetImageType=JPG&composite=_-77.1491,38.8811,Start,rddot_-77.1444,38.8654,Seven-Corners,1_-77.17616,38.8845,Falls-Church,2_-77.15566,38.8514,Lake-Barcroft,3_-77.18545,38.8672,Jefferson-CDP,4_-77.10826,38.8803,Arlington-CDP,5_-77.12909,38.8495,Baileys-Crossroads,6_-77.20592,38.8897,Idylwood-CDP,7_-77.20132,38.9116,Pimmit-Hills,8_-77.17535,38.9283,McLean-CDP,9_-77.14999,38.825,Lincolnia-CDP,10_-77.23115,38.892,Dunn-Loring,11_&Res=29");
        //RequestString requestString("X1=303086.129977103&X2=320486.129977103&Y1=4311128.551458912&Y2=4299528.551458912&UTMZ1=18&TargetImageType=JPG&composite=_-77.17616,38.8845,Falls-Church,2_+all+ramb&Res=29");

//		RequestString requestString("TargetImageType=jpg&X1=584096.0&X2=588904.0&Y1=2853669.0&Y2=2851231.0&res=4.8&UTMZ1=17&Composite=state.aaa");
        //RequestString requestString("TargetImageType=jpg&X1=584096.0&X2=588904.0&Y1=2853669.0&Y2=2851231.0&res=4.8&UTMZ1=17&Composite=seas.aaa");

		//RequestString requestString("XN=153&YN=747&Res=9.6&UTMZ1=17&TargetImageType=GIF&map34c=0&map32=0&Composite=cndx");

//		RequestString requestString("XN=305&YN=1490&Res=4.8&UTMZ1=17&TargetImageType=jpg&map34c=0&map32=0&Composite=street.ln");//Majhwy

		//TargetImageType=gif&XN=1221&YN=5958&Res=1.2&UTMZ1=17&TargetImageType=GIF&map34c=0&map32=0&Composite=flproplines.t00RequestString requestString("TargetImageType=gif_op&XN=304&YN=1489&Res=4.8&UTMZ1=17&Composite=flproplines.t00");//has bug

        //RequestString requestString("TargetImageType=jpg&X1=543856.041&X2=548619.318&Y1=2817728.068&Y2=2812572.991&res=4.8&UTMZ1=17&HighlightID=1079180080850&Composite=flproplines.t00");


		//RequestString requestString("TargetImageType=gif&XN=1221&YN=5958&Res=1.2&UTMZ1=17&Composite=flproplines.t00");

		//RequestString requestString("TargetImageType=JPG&X1=442906.62903004413&X2=449050.62927418476&Y1=3182247.8335190485&Y2=3174567.8332138727&res=19.2&UTMZ1=17&Composite=flproplines-_1.t00");

		//RequestString requestString("TargetImageType=JPG&X1=444894.86618765735&X2=451038.866431798&Y1=3186014.7383092656&Y2=3178334.73800409&res=19.2&UTMZ1=17&source=best_available&Composite=flproplines-_1.t00");

		//RequestString requestString("TargetImageType=jpg&XN=242&YN=1644&Res=4.8&UTMZ1=17&TargetImageType=GIF&map34c=0&map32=0&Composite=flproplines-_1.t00+_-81.364457,28.564086,%20,rddot_");

		//RequestString requestString("TargetImageType=jpg&XN=605&YN=2982&Res=2.4&UTMZ1=17&Composite=flproplines.t00");

		//RequestString requestString("TargetImageType=gif&X1=577731.0&Y1=3010924.0&x2=578966.0&y2=3009790.0&Res=4.8&UTMZ1=17&Composite=flproplines.t00");

		//RequestString requestString("TargetImageType=gif&X1=587026.0&X2=587915.0&Y1=2860130.0&Y2=2859382.0&res=2.4&UTMZ1=17&source=best_available&Composite=cndx");

		//RequestString requestString("TargetImageType=gif_op&X1=587683.0&X2=588303.0&Y1=2856161.0&Y2=2855738.0&res=4.8&UTMZ1=17&source=best_available&Composite=cndx");

//		RequestString requestString("TargetImageType=jpg&X1=284096.0&X2=788904.0&Y1=2883669.0&Y2=2051231.0&res=1024&UTMZ1=17&Composite=seas.aaa");


//		RequestString requestString("TargetImageType=jpg&X1=436503.134&X2=581616.412&Y1=2969239.384&Y2=2846644.331&res=128&UTMZ1=17&Composite=zip.aaa");

//		RequestString requestString("TargetImageType=jpg&X1=584096.0&X2=588904.0&Y1=2853669.0&Y2=2851231.0&res=4.8&UTMZ1=17&Composite=seas.aaa");
		//RequestString requestString("TargetImageType=jpg&XN=6&YN=149&RES=64.000000&UTMZ1=18&COMPOSITE=street.l_+state.0d0+seas.aaa");
        //RequestString requestString("TargetImageType=jpg&XN=32&YN=147&RES=64.000000&UTMZ1=17&COMPOSITE=street.l_+state.0d0+seas.aaa"); 
		//RequestString requestString("TargetImageType=jpg&XN=118&YN=1523&RES=8.000000&UTMZ1=16&COMPOSITE=seas.aaa");//p
		//RequestString requestString("TargetImageType=jpg&XN=185&YN=1290&RES=8.000000&UTMZ1=17&COMPOSITE=street.l_+state.0d0+seas.aaa");//p
//		RequestString requestString("TargetImageType=jpg&XN=1238&YN=10636&RES=1.000000&UTMZ1=18&COMPOSITE=seas.aaa");//p
//		RequestString requestString("TargetImageType=jpg&XN=182&YN=3146&RES=4.000000&UTMZ1=33&COMPOSITE=seas.aaa");//p
//		RequestString requestString("TargetImageType=jpg&XN=80&YN=533&RES=16.000000&UTMZ1=16&COMPOSITE=seas.aaa");//p
//		RequestString requestString("TargetImageType=jpg&XN=79&YN=531&RES=16.000000&UTMZ1=16&COMPOSITE=street.l_+state.0d0+seas.aaa");//p
		//RequestString requestString("TargetImageType=jpg&XN=79&YN=531&RES=16.000000&UTMZ1=16&COMPOSITE=street.l_+state.0d0+seas.aaa");//p
		//RequestString requestString("TargetImageType=jpg&XN=54&YN=220&RES=32.000000&UTMZ1=16&COMPOSITE=street.l_+state.0d0+seas.aaa");
        //RequestString requestString("TargetImageType=jpg&X1=373357.737&X2=374964.494&Y1=4038617.931&Y2=4036963.441&res=4.0&UTMZ1=17&Composite=Apron.aaa");
        //RequestString requestString("TargetImageType=jpg&X1=574578.0&X2=575021.0&Y1=2852816.0&Y2=2852351.0&res=1.2&UTMZ1=17&Composite=flproplines.00a");
//        RequestString requestString("TargetImageType=jpg&X1=41341.804&X2=661741.2&Y1=8057588.643&Y2=4903936.776&res=512&UTMZ1=7&UTMZ2=19&Composite=country.aaa");
      //RequestString requestString("TargetImageType=jpg&X1=284096.0&X2=788904.0&Y1=6710693.65&Y2=4879016.852&res=512&UTMZ1=17&UTMZ2=20&Composite=state.aaa");
        
      //RequestString requestString("TargetImageType=jpg&X1=269366.0&X2=317863.0&Y1=5051720.0&Y2=5019700.0&res=64&UTMZ1=33&Composite=seas.aaa");

 //     RequestString requestString("TargetImageType=jpg&X1=239366.0&X2=357863.0&Y1=5066720.0&Y2=5005700.0&res=64&UTMZ1=33&Composite=seas.aaa");

//		RequestString requestString("TargetImageType=jpg&X1=283788.0&X2=299202.0&Y1=5039338.0&Y2=5030930.0&res=16&UTMZ1=33&Composite=seas.aaa");

//        RequestString requestString("TargetImageType=jpg&X1=283688.0&X2=299202.0&Y1=5039338.0&Y2=5030930.0&res=16&UTMZ1=33&Composite=seas.aaa");

		//RequestString requestString("TargetImageType=jpg&X1=283788.0&X2=299202.0&Y1=5039338.0&Y2=5030930.0&res=16&UTMZ1=33&Composite=seas.aaa");
		//RequestString requestString("TargetImageType=jpg&X1=276704.0&X2=304542.0&Y1=5043285.0&Y2=5028040.0&res=32&UTMZ1=33&Composite=seas.aaa");

		//RequestString requestString("TargetImageType=jpg&X1=576947&Y1=2880035&X2=584608&Y2=2875176&RES=9.6&UTMZ1=17&UTMZ2=17&COMPOSITE=street.ln");

		//RequestString requestString("TargetImageType=gif&X1=579737&Y1=2879690&X2=583290&Y2=2876099&RES=9.6&UTMZ1=17&UTMZ2=17&COMPOSITE=street.ln");

		//RequestString requestString("TargetImageType=jpg&X1=300443.4253163342&Y1=5043240.108089233&X2=307388.2653733493&Y2=5036041.869249626&res=16&UTMZ1=33&Composite=seas.aaa");//

//		RequestString requestString("TargetImageType=JPG&X1=270365.95817946515&Y1=3295109.6215739846&X2=275129.89089459134&Y2=3292588.6531175086&res=4.8&UTMZ1=15&COMPOSITE=street.ln");

		//RequestString requestString("X1=356355.659308175&X2=359091.659308175&Y1=2559337.100635627&Y2=2558881.100635627&UTMZ1=17&TargetImageType=JPG&composite=_-82.3896,23.1347,%20,rddot_&Res=1");


        //RequestString requestString("X1=592740.082266041&X2=594340.082266041&Y1=2950619.348551166&Y2=2949339.348551166&UTMZ1=17&TargetImageType=JPG&UTMZ1=17&composite=ZIPS+bg.0bv+street._n+city+all&Res=4&sm=7");

		//RequestString requestString("X1=351495.798024710&Y1=2562328.305012233&X2=357495.798024710&Y2=2553628.305012233&Res=1.0000&UTMZ1=17&composite=points2&sm=2");

        //RequestString requestString("X1=584645.765414&Y1=2856259.037351&X2=590645.765414&Y2=2852259.037351&RES=10.000000&UTMZ1=17&UTMZ2=17&COMPOSITE=_-80.129046,25.790471,.,rddot_-80.1232,25.8116,Mosaic,1_-80.1231,25.8118,Courtyard,2_-80.123,25.8125,Crown,3_-80.1231,25.8132,Fairfield,4_-80.1228,25.8136,King-Richards,5_-80.1229,25.8139,La-Tour,6_-80.1227,25.8145,Days-Inn,7_-80.1226,25.8145,Vendome,8_-80.1226,25.8149,Holiday-Resort,9_-80.1223,25.8151,Four-Points,10_-80.1224,25.8159,Sovereign,11_-80.1231,25.8172,Fontainebleau,12_&targetimagetype=jpg");

        //RequestString requestString("X1=587628.046806607&X2=588228.046806607&Y1=2855573.479399249&Y2=2855173.479399249&UTMZ1=17&TargetImageType=jpg&composite=_-80.1224,25.8135,,12_&Res=1");

        //RequestString requestString("TargetImageType=gif&X1=471243.424829237&X2=961443.424829237&Y1=3018489.404801404&Y2=2691689.404801404&UTMZ1=17&TargetImageType=JPG&Source=best_available&OverLay=_-80.302382,25.763282,.,rddot_-79.5,25.9167,Bimini-District,1_-79.5,25.9167,Biminis,2_-79.5,25.9167,Bimini,3_-78.75,26.6667,Freeport-District,4_-78.75,26.6667,Freeport,5_-78,25.3667,Nichollstown-and,6_-78.2,27,High-Rock,7_-78.2,27,.,8_-78,24.5833,Fresh-Creek,9_-78,24.5833,.,10_-77.36667,26.1167,Sandy-Point,11_-77.36667,26.1167,.,12_+street._n&Res=817");

        //RequestString requestString("TargetImageType=gif&X1=561798.551869&Y1=2868324.694584&X2=601998.551869&Y2=2841524.694584&RES=16.000000&UTMZ1=17&UTMZ2=17&COMPOSITE=blkgroup-12.0-17.0");
		//RequestString requestString("TargetImageType=gif&X1=561798.551869&Y1=2868324.694584&X2=601998.551869&Y2=2841524.694584&RES=16.000000&UTMZ1=17&UTMZ2=17&COMPOSITE=blkgroup-12.0-17.0");
		//RequestString requestString("TargetImageType=gif&X1=534489.600006151&Y1=2846630.4000207814&UTMZ1=17&X2=560486.4000332191&Y2=2838720.000020643&UTMZ2=17&res=38.4&composite=rtgauges-_7");
		//RequestString requestString("X1=320950.16&X2=327533.84&Y1=4309080.56&Y2=4304691.44&UTMZ1=18&TargetImageType=jpg&Res=0.6096&COMPOSITE=street._n&sm=1.2");
		//RequestString requestString("X1=322641.8&X2=325842.2&Y1=4307823.26&Y2=4305948.74&UTMZ1=18&TargetImageType=gif&Res=0.3048&Source=county_1ft&jcq=10&COMPOSITE=street._n&sm=1.2192");

		//RequestString requestString("TargetImageType=gif&X1=576947&Y1=2880035&X2=584608&Y2=2875176&RES=9.6&UTMZ1=17&UTMZ2=17&COMPOSITE=bg");

        //RequestString requestString("TargetImageType=gif&X1=570069.419&X2=602699.752&Y1=2986166.075&Y2=2958996.429&res=64&UTMZ1=17&UTMZ2=17&Composite=wtown3");
        //RequestString requestString("XN=153&YN=747&Res=9.6&UTMZ1=17&TargetImageType=GIF&map34c=0&map32=0&Composite=wtown3");
        //RequestString requestString("TargetImageType=txt&X1=470069.419&X2=602699.752&Y1=3086166.075&Y2=2958996.429&res=64&UTMZ1=17&UTMZ2=17&Composite=street.ln");
        //RequestString requestString("TargetImageType=gif&X1=570069.419&X2=602699.752&Y1=2986166.075&Y2=2958996.429&res=64&UTMZ1=17&UTMZ2=17&Composite=wtown3");

        //RequestString requestString("TargetImageType=jpg&X1=561798.551869&Y1=2868324.694584&X2=601998.551869&Y2=2841524.694584&RES=16.000000&UTMZ1=17&UTMZ2=17&COMPOSITE=street.ln");

        //RequestString requestString("TargetImageType=jpg&X1=564064.839327544&Y1=2850048.4246345&UTMZ1=17&X2=564280.839327544&Y2=2849931.4246345&UTMZ2=17&res=0.075&COMPOSITE=flpropertiessize+flpropertiesyear+flpropertiesowner+flpropertiesvalue");

        //RequestString requestString("TargetImageType=jpg&X1=564064.839327544&Y1=2850048.4246345&UTMZ1=17&X2=564280.839327544&Y2=2849931.4246345&UTMZ2=17&res=0.075&HighlightID=2540060280300&COMPOSITE=flproplines.t0t");

		//RequestString requestString("TargetImageType=gif&X1=564064.839327544&Y1=2850048.4246345&UTMZ1=17&X2=564280.839327544&Y2=2849931.4246345&UTMZ2=17&res=0.075&HighID=2540060280300&Highlight=center&COMPOSITE=flproplines.t0t");

        //RequestString requestString("TargetImageType=gif&X1=564064.839327544&Y1=2850048.4246345&UTMZ1=17&X2=565480.839327544&Y2=2848931.4246345&UTMZ2=17&res=0.6&HighID=2540060280300&Highlight=center&COMPOSITE=flproplines.t0t");

		//RequestString requestString("TargetImageType=gif&X1=428132.390599206&Y1=3405688.41038779&X2=530705.149639206&Y2=3343421.42798779&UTMZ1=16&UTMZ2=16&Res=78.0288&COMPOSITE=street-0.075-152.874.ln");

		//RequestString requestString("TargetImageType=jpg&X1=468251.708511415&Y1=3381402.34546014&UTMZ1=16&X2=485749.666911415&Y2=3361036.82866014&UTMZ2=16&Source=best_available&Res=39.0144&COMPOSITE=street.ln");

        //RequestString requestString("TargetImageType=gif&X1=564064.839327544&Y1=2850048.4246345&UTMZ1=17&X2=565480.839327544&Y2=2848931.4246345&UTMZ2=17&res=0.6&COMPOSITE=flproplines.c.t0t");

        //RequestString requestString("TargetImageType=gif&source=best_available&X1=564064.839327544&Y1=2850048.4246345&UTMZ1=17&X2=564280.839327544&Y2=2849931.4246345&UTMZ2=17&res=0.075&COMPOSITE=flproplines.c.t0t");


        //RequestString requestString("TargetImageType=jpg&lat1=75&lat2=-75&long1=-140&long2=10&level=1&COMPOSITE=wm_Time_Zones.tv0");
		//RequestString requestString("TargetImageType=jpg&lat2=29.5400087925255&lat1=31.9568230158972&long2=-84.3804931640625&long1=-87.1929931640625&level=7&COMPOSITE=wm_Water_Poly");
//RequestString requestString("TargetImageType=gif&lat2=29.5400087925255&lat1=31.9568230158972&long2=-87.1929931640625&long1=-90.0054931640625&level=7&COMPOSITE=wm_World_country_Nations.ttt6e6e6ee8e4da0000ff");

		//RequestString requestString("TargetImageType=jpg&lat1=21.9838014173847&lat2=0.0439453081913492&long1=-67.5439453125&long2=-45.0439453125&COMPOSITE=state.t0t");
		//RequestString requestString("TargetImageType=jpg&lat2=39.9118429847497&lat1=40.9819715408665&long2=-98.4402465820313&long1=-99.8464965820313&COMPOSITE=state.t0t");
        
		//RequestString requestString("TargetImageType=gif&Composite=wcity+wtown3+street._n+gnis-_3+hotels-_3+_-80.193573,25.773941,%20,rddot_&X1=34560&Y1=178560&UTMZ1=17&X2=36480&Y2=176640&UTMZ2=17&res=4.8");

		//RequestString requestString("TargetImageType=gif&X1=576694.623731&Y1=2872009.928894&X2=589494.623731&Y2=2864009.928894&RES=16.000000&UTMZ1=17&UTMZ2=17&COMPOSITE=incorp.t5v&HighID=North Miami Beach");

		//RequestString requestString("TargetImageType=gif&X1=536694.623731&Y1=2892009.928894&X2=589494.623731&Y2=2864009.928894&RES=16.000000&UTMZ1=17&UTMZ2=17&COMPOSITE=ptrim");

		//RequestString requestString("TargetImageType=gif&source=best_available&X1=575437.8086&Y1=2793084.355&X2=596859.429&Y2=2769089.1763&RES=32.000000&UTMZ1=17&UTMZ2=17&COMPOSITE=ptrim");

        //RequestString requestString("TargetImageType=gif&source=best_available&X1=561740.591387401&X2=563180.591387401&Y1=2849800.930486234&Y2=2848840.930486234&UTMZ1=17&COMPOSITE=_-80.375801,25.756701,.,rddot_-80.373,25.7621,$220K,1_-80.3782,25.7625,$280K,2_-80.38052,25.7616,$200K,3_-80.37698,25.7632,$280K,4_-80.37625,25.7633,$1100,5_-80.3813,25.7615,$160K,6_-80.37499,25.7638,$1600,7_-80.3788,25.7634,$330K,8_-80.3775,25.7645,$240K,9_-80.37113,25.7636,$120K,10_-80.37295,25.7644,$150K,11_-80.38313,25.7618,$440K,12_+street._n&Res=2.4");

		//RequestString requestString("TargetImageType=gif&source=best_available&X1=535158.87401202&Y1=2945412.39032261&UTMZ1=17&X2=539450.07401202&Y2=2944548.39032261&UTMZ2=17&Res=4.8&COMPOSITE=ptrim");
 
		//RequestString requestString("TargetImageType=gif&X1=584933.84159155&Y1=2867777.52673385&UTMZ1=17&X2=586827.92159155&Y2=2866903.92673385&UTMZ2=17&Res=2.4&COMPOSITE=gnis-_3");

        //RequestString requestString("TargetImageType=jpg&X1=361889.150925493&Y1=5874369.63164096&UTMZ1=33&X2=366660.350925493&Y2=5873937.63164096&UTMZ2=33&Source=best_available&Res=1.2&COMPOSITE=sonygps");

		//RequestString requestString("TargetImageType=jpg&X1=389054.805202752&Y1=5819725.72245172&UTMZ1=33&X2=391037.525202752&Y2=5818293.72245172&UTMZ2=33&Source=best_available&Res=2.4&composite=sonygps.ln");

		//RequestString requestString("TargetImageType=jpg&X1=368726.677&Y1=3778627.846&UTMZ1=11&X2=373094.899&Y2=3774286.712&UTMZ2=11&Source=best_available&Res=1.2&composite=samotrails.ln");

        //RequestString requestString("TargetImageType=gif&Composite=street._n+wtown3+wcity+samotrails.ln+street._n+city+incorp.00v+annomreal+gnis-_3+hotels-_3&X1=370560&Y1=3776640&UTMZ1=11&X2=371520&Y2=3775680&UTMZ2=11&res=2.4");

        //RequestString requestString("TargetImageType=jpg&Composite=street.ln+samotrails.ln+test&X1=370560&Y1=3776640&UTMZ1=11&X2=371520&Y2=3775680&UTMZ2=11&res=2.4");

        //RequestString requestString("TargetImageType=gif&X1=587108.508595653&Y1=2851746.13489932&UTMZ1=17&X2=587255.658595653&Y2=2851680.07489932&UTMZ2=17&res=0.15&Composite=sonygps.ln");


        //RequestString requestString("TargetImageType=jpg&X1=368722.677&Y1=3778627.846&UTMZ1=11&X2=373094.899&Y2=3774286.712&UTMZ2=11&Source=best_available&Res=2.4&composite=wm_Minor_Routes.ln");


		//RequestString requestString("TargetImageType=gif&X1=323858.409701569&Y1=4313771.63248256&UTMZ1=18&X2=324883.689701569&Y2=4313330.99248256&UTMZ2=18&res=1.2&Composite=FA_Point.a");
		//RequestString requestString("TargetImageType=gif&X1=586920&Y1=2851800&UTMZ1=17&X2=587040&Y2=2851680&UTMZ2=17&res=0.3&Composite=street.ln");
		//RequestString requestString("TargetImageType=gif&X1=586920&Y1=2851920&UTMZ1=17&X2=587040&Y2=2851680&UTMZ2=17&res=0.3&Composite=street.ln");


		//RequestString requestString("TargetImageType=gif&lat2=24.5321322103027&lat1=27.0640175466087&long2=-81.5679931640625&long1=-84.3804931640625&prj=UTM&COMPOSITE=street.l_&res=0.3&width=400&height=400");

		//RequestString requestString("TargetImageType=gif&lat2=25.780655625328325&lat1=25.782829743448751&long2=-80.131901941846095&long1=-80.133082907250213&prj=UTM&COMPOSITE=street.l_&res=0.3&width=400&height=800");

		//RequestString requestString("TargetImageType=gif&lat2=25.780655625328325&lat1=25.782829743448751&long2=-80.131901941846095&long1=-80.133082907250213&prj=UTM&COMPOSITE=wm_Water_Poly&res=0.3&width=400&height=800");



        //RequestString requestString("TargetImageType=gif&lat2=29.5400087925255&lat1=31.9568230158972&long2=-84.3804931640625&long1=-87.1929931640625&level=7&COMPOSITE=wm_Airports&width=256&height=256");

        //RequestString requestString("TargetImageType=gif&lat2=27.0689090954634&lat1=31.9614835572685&long2=-84.385986328125&long1=-90.010986328125&level=6&COMPOSITE=wm_Cities_Greater_900K.l+wm_Cities_75K_to_900K.l");
		//RequestString requestString("TargetImageType=gif&lat2=48.936934954094&lat1=55.7889289538926&long2=-56.27197265625&long1=-67.52197265625&level=5&COMPOSITE=wm_world_country_nations.ttt6e6e6ee8e8daffffff");
        //+wm_great_lakes.ttt6e6e6e99ccccffffff
        //RequestString requestString("TargetImageType=gif&lat2=41.0130657870063&lat1=55.8012809711804&long2=-67.5439453125&long1=-90.0439453125&level=4&COMPOSITE=wm_world_country_nations.ttt6e6e6ee8e8daffffff+wm_great_lakes.ttt6e6e6e99ccccffffff");


        //RequestString requestString("TargetImageType=txt&lat2=-89&lat1=89&long2=179&long1=-179&level=7&COMPOSITE=wm_World_country_Nations.t0t");
		//RequestString requestString("TargetImageType=gif&lat2=0.351560293992273&lat1=85.05112878&long2=179.64843751&long1=-0.3515625&level=1&COMPOSITE=wm_World_Nations.ttt");

        //RequestString requestString("TargetImageType=xml&lat2=-89&lat1=89&long2=179&long1=-179&prj=UTM&COMPOSITE=wm_World_country_Nations.t0t&res=19567&width=1800&height=1800");

		//RequestString requestString("TargetImageType=gif&Composite=nations.l+wm_Capitals&lat2=11.199956&lat1=21.963424&long2=-90.021973&long1=-101.271973&level=5");

		//RequestString requestString("TargetImageType=gif&Composite=street.ln&X1=537600&Y1=2808960&UTMZ1=17&X2=539520&Y2=2807040&UTMZ2=17&res=4.8");
		//RequestString requestString("TargetImageType=gif&Composite=wcity+wtown3+street._n+street.l_+hotels-_3+gnis-_3&X1=539040&Y1=2807520&UTMZ1=17&X2=539520&Y2=2807040&UTMZ2=17&res=1.2");
		//RequestString requestString("TargetImageType=gif&Composite=street.ln&X1=539040&Y1=2808000&UTMZ1=17&X2=539520&Y2=2807520&UTMZ2=17&res=1.2");

		//RequestString requestString("TargetImageType=gif&X1=780497.318422983&Y1=2053325.30415906&UTMZ1=18&X2=783662.438422983&Y2=2051024.18415906&UTMZ2=18&Source=best_available&Res=9.6&Composite=IDP");

		//RequestString requestString("TargetImageType=gif&Composite=FA_Point-0.0-2.4-.a+FA_Point-0.0-2.4-.o&X1=562560&Y1=2849760&UTMZ1=17&X2=563040&Y2=2849280&UTMZ2=17&res=1.2");

		//RequestString requestString("TargetImageType=gif&Composite=FA_Point-_1.a+FA_Parcel-0.0-2.4-.t00&X1=464160&Y1=3159840&UTMZ1=17&X2=464640&Y2=3159360&UTMZ2=17&res=1.2");

		//RequestString requestString("TargetImageType=gif&style=BT&Composite=FA_Point-_1.a+FA_Parcel-0.0-2.4-.t00&X1=577169&Y1=2916326&UTMZ1=17&X2=577413&Y2=2916082&UTMZ2=17&res=0.6");
		//RequestString requestString("TargetImageType=gif&style=BT&Composite=street.ln+FA_Point.o+FA_Parcel+FA_Point.a&X1=577169&Y1=2916326&UTMZ1=17&X2=577413&Y2=2916082&UTMZ2=17&res=0.6");
		RequestString requestString("TargetImageType=gif&style=BT&Composite=FA_Parcel-0-1-.t00&X1=577169&Y1=2916326&UTMZ1=17&X2=577413&Y2=2916082&UTMZ2=17&res=0.6");
		//RequestString requestString("TargetImageType=gif&style=BT&Composite=flpropertiesvalue-0-0.3-&X1=578998&Y1=2914497&UTMZ1=17&X2=579059&Y2=2914436&UTMZ2=17&res=0.15");
		//RequestString requestString("TargetImageType=gif&Composite=FA_Point-_1.a+FA_Point-_1.o+FA_Parcel-_1.t00&X1=375999.6&Y1=3773199.6&UTMZ1=11&X2=376399.2&Y2=3772800&UTMZ2=11&res=1.2");

		//RequestString requestString("TargetImageType=gif&Composite=FA_Parcel-_1.t00&X1=357600&Y1=3516799.2&UTMZ1=13&X2=358399.2&Y2=3516000&UTMZ2=13&res=2.4");

#endif

		string comp = requestString.getValue("COMPOSITE");

		CFMapRequestManager reqManager;

		//initialize the request, decode
		//comp = reqManager.InitRequest(comp.c_str());

		//set drawing the filled area first
		bool hasFilledAreaReq = false;

		hasFilledAreaReq = reqManager.HasFilledAreaReq( comp, hasFilledAreaReq/*, hasFALayer*/ );

		

		//comp = reqManager.InitFilledAreaReq( comp, &hasFilledAreaReq );

		//set XML
		CString strTarImaType  = requestString.getValue("TargetImageType").c_str();
		bool hasXMLOutput = reqManager.HasXML( strTarImaType );
		bool hasXMLUTM    = reqManager.HasUTM( strTarImaType );
		//set GIF
		bool hasTransGIFOutput  = reqManager.HasTransGIF( strTarImaType );
		bool hasGIFOutput       = reqManager.hasGIFOutput( strTarImaType );

		//for all, ramb and seas layers
		bool hasTransCanvas = true;
		TransInfo seastrans;
		comp = reqManager.RefactLayer( hasGIFOutput, comp, &hasTransCanvas, &seastrans);

		bool hasLatLongStreetRequest = false;

		//bool hasFAADDR = false;

		//for highlightArea
		CString strHighlightAreaID = requestString.getValue("HighID").c_str();
		strHighlightAreaID = reqManager.InitRequest(strHighlightAreaID);
		bool hasHighlightArea      = reqManager.HasHighlight( strHighlightAreaID );
		//bool hasHighlightCenter    = reqManager.HasHighlightCenter( requestString.getValue("Highlight").c_str() );

		//set txt
		bool hasTxt = reqManager.HasTxt( strTarImaType );


		string XN = requestString.getValue("XN");
		string YN = requestString.getValue("YN");


		//Lat, Long
		string str_lat1  = requestString.getValue("lat1");
		string str_long1 = requestString.getValue("long1");
		string str_lat2  = requestString.getValue("lat2");
		string str_long2 = requestString.getValue("long2");	
		string str_level = requestString.getValue("level");
		int nlevel       = atoi(str_level.c_str());
		//set Lat, Long
		bool hasLatLong = reqManager.HasLatLongReq(str_lat1, str_long1, str_lat2, str_long2, str_level);


		string str_prj = requestString.getValue("prj");
		string str_reqwidth = requestString.getValue("width");
		string str_reqheight = requestString.getValue("height");
		bool hasLatLonginUTM = reqManager.HasLatLongReqinUTM(str_lat1, str_long1, str_lat2, str_long2, str_prj, str_reqwidth, str_reqheight);


		int nlabeldensity = 0;
		string str_ldensity = requestString.getValue("labeldensity");
		if( "" != str_ldensity)
		{
			sscanf(str_ldensity.c_str(), "%d", &nlabeldensity);
		}
		


		int nReqWidth, nReqHeight;
		nReqWidth = 0;
		nReqHeight = 0;
		sscanf(str_reqwidth.c_str(), "%d", &nReqWidth);
		sscanf(str_reqheight.c_str(), "%d", &nReqHeight);

		rectObj rect;
		double x1,x2,y1,y2,res;
		int xn,yn;
		int utmz, utmz1, utmz2;
		Gdc_Coord_3d gdc1;
		Gdc_Coord_3d gdc2;
		
		string RES = requestString.getValue("RES");

		//process SM
		string SM = requestString.getValue("SM");
		double dLetterEnlargeRatio = 1, dSM = 0, dRES = 0;

		if("" != SM)
		{
			dRES = atof( RES.c_str() );
			dSM = atof( SM.c_str() );
			dLetterEnlargeRatio = dSM / dRES;
			//RES = SM;
		}


		if( (RES == "") && (!hasLatLong) )
		{
			CGIReply::replyText("Error: Resolution parameter not specified!");
			exit(1);
		}

		sscanf(RES.c_str(), "%lf", &res);

		if( hasLatLong || hasLatLonginUTM )
		{

			double dLat1  = atof(str_lat1.c_str());
			double dLat2  = atof(str_lat2.c_str());
			double dLong1 = atof(str_long1.c_str());
			double dLong2 = atof(str_long2.c_str());

			gdc1.longitude = dLong1;
			gdc2.latitude = dLat2;
			gdc2.longitude = dLong2;
			gdc1.latitude = dLat1;

		}
		else if( XN == "" || YN == "" )
		{
			string X1 = requestString.getValue("X1");
			string X2 = requestString.getValue("X2");
			string Y1 = requestString.getValue("Y1");
			string Y2 = requestString.getValue("Y2");

			if(X1 == "" || X2 == "" || Y1 == "" || Y2 == "")
			{
				CGIReply::replyText("Error: XN YN or X1 Y1 parameter not specified!");
				exit(1);
			}

			string UTMZ1 = requestString.getValue("UTMZ1");
			string UTMZ2 = requestString.getValue("UTMZ2");
			if("" == UTMZ2)
			{
				UTMZ2 = UTMZ1;
			}

			if(UTMZ1 == "")
			{
				CGIReply::replyText("Error: UTMZ1 parameter not specified!");
				exit(1);
			}

			sscanf(X1.c_str(), "%lf", &x1);
			sscanf(Y1.c_str(), "%lf", &y1);
			sscanf(X2.c_str(), "%lf", &x2);
			sscanf(Y2.c_str(), "%lf", &y2);

			sscanf(UTMZ1.c_str(), "%d", &utmz1);
			sscanf(UTMZ2.c_str(), "%d", &utmz2);

			bool isNorthUTM;
			isNorthUTM = reqManager.HasNorthUTM(utmz1);
			Utm_Coord_3d utmCoord1(x1, y1, 0, abs(utmz1), isNorthUTM/*not sure*/);
			isNorthUTM = reqManager.HasNorthUTM(utmz2);
			Utm_Coord_3d utmCoord2(x2, y2, 0, abs(utmz2), isNorthUTM);

			Utm_To_Gdc_Converter::Init();
			Utm_To_Gdc_Converter::Convert(utmCoord1, gdc1);
			Utm_To_Gdc_Converter::Convert(utmCoord2, gdc2);
		}
		else
		{
			string UTMZ = requestString.getValue("UTMZ1");
			if(UTMZ == "")
			{
				CGIReply::replyText("Error: UTMZ parameter not specified!");
				exit(1);
			}


			sscanf(XN.c_str(), "%d", &xn);
			sscanf(YN.c_str(), "%d", &yn);
			sscanf(UTMZ.c_str(), "%d", &utmz);

			x1 = xn*TILE_SIZE*res;	// 400 is the tile size
			x2 = (xn+1)*TILE_SIZE*res;
			y1 = yn*TILE_SIZE*res;
			y2 = (yn-1)*TILE_SIZE*res;

			Utm_Coord_3d utmCoord1(x1, y1, 0, utmz, true/*not sure*/);
			Utm_Coord_3d utmCoord2(x2, y2, 0, utmz, true);

			//Utm_Coord_3d utmCoord1(x1, y1, 0, utmz, false/*not sure*/);
			//Utm_Coord_3d utmCoord2(x2, y2, 0, utmz, false);

			Utm_To_Gdc_Converter::Init();
			Utm_To_Gdc_Converter::Convert(utmCoord1, gdc1);
			Utm_To_Gdc_Converter::Convert(utmCoord2, gdc2);
		}


		//the scope of longitude and latitude
		if(gdc1.longitude < -180 || gdc2.longitude >= 180
			|| gdc1.latitude < -90 || gdc2.latitude >= 90)
		{
			CGIReply::replyText("XN YN or X1 Y2 coordinates error, please check these parameters!");
			exit(1);
		}

		rect.minX = gdc1.longitude;
		rect.minY = gdc2.latitude;
		rect.maxX = gdc2.longitude;
		rect.maxY = gdc1.latitude;



		//for canvas and boundary
		CMapCanvas canvas;
		int nWidth = 0;
		int nHeight = 0;

		if( hasLatLong )  //Lat, Long
		{
			nWidth  = 256;
			nHeight = 256;
			if(nReqWidth != 0)
			{
				nWidth  = nReqWidth;
				nHeight = nReqHeight;
			}
		}
		else if( XN != "" || YN != "" )	// BT
		{
			nWidth  = 400;
			nHeight = 400;
		}
		else if( hasLatLonginUTM )//lat long in UTM
		{
			//int nReqWidth, nReqHeight;

			//sscanf(str_reqwidth.c_str(), "%d", &nReqWidth);
			//sscanf(str_reqheight.c_str(), "%d", &nReqHeight);
			nWidth  = nReqWidth;
		    nHeight = nReqHeight;
		}
		else // DT
		{	
			nWidth  = (int) ((x2-x1)/res);
			nHeight = (int) ((y1-y2)/res);
		}
		//else if(XN == "")	// DT
		//{	
		//	nWidth  = (int) ((x2-x1)/res);
		//	nHeight = (int) ((y1-y2)/res);
		//}
		//else			// BT
		//{
		//	nWidth = 400;
		//	nHeight = 400;
		//}

		//canvas.InitCanvas(nWidth, nHeight, RGB(255, 255, 255), true, 24 );
        
		canvas.InitCanvas( nWidth, nHeight, RGB(0, 0, 255), hasTransCanvas, 24, hasGIFOutput, &seastrans);

		CCoordinateTransformer transformer;

		//set Lat, Long to transformer
		transformer.SetLatLong( hasLatLong );

		/* from source to dest coordinate system transformer */
		transformer.SetSourceDestCoordinateSys(CCoordinateTransformer::GCS, 
			CCoordinateTransformer::LINEAR_VIEW);

		CDoubleRect rcSourceExt, rcDestExt;	// BUG
		rcSourceExt.dX1 = rect.minX;
		rcSourceExt.dX2 = rect.maxX;
		rcSourceExt.dY1 = rect.minY;
		rcSourceExt.dY2 = rect.maxY;

		rcDestExt.dX1 = 0;
		rcDestExt.dY1 = 0;
		rcDestExt.dX2 = nWidth;
		rcDestExt.dY2 = nHeight;

		int nMSOffsetX = 0, nMSOffsetY = 0;
		if( hasLatLong )
		{
			int nPX1 = 0, nPX2 =0, nPY1 = 0, nPY2 = 0;
			transformer.m_pLinearTransformer->LatLongToPixelXY( 
				rcSourceExt.dY1, rcSourceExt.dX1, nlevel, &nPX1, &nPY1, 0, 0 );
			transformer.m_pLinearTransformer->LatLongToPixelXY( 
				rcSourceExt.dY2, rcSourceExt.dX2, nlevel, &nPX2, &nPY2, 0, 0 );
            nMSOffsetX = nPX1;
			nMSOffsetY = nPY2;
		}


		transformer.SetLinearConvertParameters( rcSourceExt, rcDestExt ,  true, false);
		//set the boundary of the placeholder for detect boundary confliction.
		g_placeHolder.SetBoundary( rcDestExt );


#ifdef DEBUG
		fprintf(stderr, "Composite: %s\n", comp.c_str());
#endif
        //////////////////////////////////////////////////////////
		//customized points
		Overlays overlays(comp);
		string sStrOverlay = overlays.getNextOverlay();	
		CString strPoints;
		bool hasStrPoint = false;
		while(sStrOverlay != "")
		{
			strPoints = sStrOverlay.c_str();

			int nStrStartPoz = 0;
			int nStrEndPoz = strPoints.GetLength() - 1;

			if( ('_' == strPoints.GetAt(nStrStartPoz))&&('_' == strPoints.GetAt(nStrEndPoz)) )
			{
				hasStrPoint = true;
				break;
			}

			sStrOverlay = overlays.getNextOverlay();
		}
		///////////////////////////////////////////////////////////

		reqManager.makeResVisible( "dining",      make_pair(0,3),   overlays, res);
		reqManager.makeResVisible( "hotel",       make_pair(1,16),  overlays, res);
		reqManager.makeResVisible( "ramb",        make_pair(0,5),   overlays, res);
		reqManager.makeResVisible( "biz",         make_pair(0,2),   overlays, res);
		reqManager.makeResVisible( "realestate",  make_pair(0,5),   overlays, res);
		reqManager.makeResVisible( "blk_pop",     make_pair(0,41),  overlays, res);
		reqManager.makeResVisible( "flproplines", make_pair(0,4.9), overlays, res);

		string overlay = overlays.getNextOverlay();
		//vector<string> files;
		vector<ShapeRecords> shapeRecords;

		bool hasFALayer  = false;
		bool hasFAParcel = false;
		bool hasFAOwner  = false;
		bool hasFAADDR   = false;
		
		while(overlay != "")
		{

			/*if(!hasFALayer&& reqManager.hasFALayers(overlay))
			{
				hasFALayer = true;
			}*/

			vector<string> files;

			TransInfo transInfo;
			OverridedColor color;
			ShapeRecords shapeRecord;
			string preOverlay;
			//string postfixOverlay;
			int nPosLastDigit = 0;
			double dResRange  = 0.0;
			double dLastDigit = 0.0;
			////////////////////////////////////////////////////////
			bool hasHLInfo = false;
			hasLatLongStreetRequest = false;

			//trim overlay, first find .xxx, then -xxx
			int iDot = (int) (overlay.find_last_of('.'));
			//if( reqManager.HasNewResSyntax(overlay) 
			//	&& reqManager.IsResVisibleDot(overlay))
			//{
			//	//this new syntax
			//	int x = 0;
			//}
			////this is old syntax
			//else 
			if( (iDot != string::npos) && (!reqManager.IsResVisibleDot(overlay)) )
			{
				// trans or override info exists
				string info = overlay.substr(iDot+1);
				//overlay only has the part before dot
				overlay = overlay.substr(0, iDot);

				hasLatLongStreetRequest = reqManager.HasLatLongStreetRequest(info);

				if( (info.at(0) == 'c') && (info.at(1) == '.') && (info.size() == HIGHLIGHTCENTER_LENGTH) )
				{
					hasHLInfo = true;
					shapeRecord.SetHighlightCenterInfo( hasHLInfo );
					int iSecondDot = (int) (info.find('.'));
					if(iSecondDot != string::npos)
					{ 
						info = info.substr( iSecondDot + 1 );
					}
				}
				else if( ((info.at(0) == 'o') && (info.size() == 1))||
					((info.at(0) == 'O') && (info.size() == 1)) )
				{
					hasFALayer = true;
					hasFAOwner = true;
					reqManager.addFALayers("fapointsowner");
				}
				else if( ((info.at(0) == 'a') && (info.size() == 1))||
					((info.at(0) == 'A') && (info.size() == 1)) )
				{
					hasFALayer = true;
					hasFAADDR = true;
					reqManager.addFALayers("fapointsaddr");
				}
				else
				{
					hasHLInfo = false;
					shapeRecord.SetHighlightCenterInfo( hasHLInfo );
				}

				if(!hasFALayer)
				{
					switch(info.size()){
					case OVERRIDE_COLOR_LENGTH:	// line, label override color
						color.line = info.substr(0, 6);
						color.label = info.substr(6);
						shapeRecord.setColorInfo(color);
						break;
					case OVERRIDE_COLOR_LENGTH/2:		// line overrided color
						color.line = info.substr(0, 6);
						shapeRecord.setColorInfo(color);
						break;
					case AREA_TRANSPARENCY_LENGTH:		// area transparency info
						transInfo.type = TransInfo::area;
						transInfo.values[0] = transInfo.char2int(info[0]);
						transInfo.values[1] = transInfo.char2int(info[1]);
						transInfo.values[2] = transInfo.char2int(info[2]);
						shapeRecord.setTransInfo(transInfo);
						shapeRecord.setColorInfo(color);
						break;

					case AREA_COLOR_LENGTH:		// area color info
						transInfo.type = TransInfo::area;
						transInfo.values[0] = transInfo.char2int(info[0]);
						transInfo.values[1] = transInfo.char2int(info[1]);
						transInfo.values[2] = transInfo.char2int(info[2]);

						color.line = info.substr(3, 6);
						color.area = info.substr(9, 6);
						color.label = info.substr(15, 6);
						shapeRecord.setTransInfo(transInfo);
						shapeRecord.setColorInfo(color);

						break;

					case LINE_TRANSPARENCY_LENGTH:		// line transparency info
						transInfo.type = TransInfo::line;
						transInfo.values[0] = transInfo.char2int(info[0]);
						transInfo.values[1] = transInfo.char2int(info[1]);
						shapeRecord.setTransInfo(transInfo);

						break;

					case POINT_ICON_LENGTH:		// point info
						transInfo.type = TransInfo::point;
						transInfo.values[0] = transInfo.char2int(info[0]);
						//transInfo.values[1] = transInfo.char2int(info[1]);
						shapeRecord.setTransInfo(transInfo);
					}
				}
			}

			//// find -xxx
			int iHyphen = (int) (overlay.find('-'));
			//new syntax
			if( reqManager.HasNewResSyntax(overlay) )
			{
				if(!reqManager.IsResVisible(&overlay, res))
				{
					overlay = overlays.getNextOverlay();
					continue;
				}
			}
			//old syntax
			else if(iHyphen != string::npos)
			{
				// two digits which decide the visibility of the overlay
				string visib = overlay.substr(iHyphen+1);
				overlay = overlay.substr(0, iHyphen);

				pair<int, int> range;
				double dMiniRes = 0.0; 
				double dMaxRes  = 0.0;

				if( ('_' == visib[0]) && ('_' != visib[1]) )
				{
					range.first  = 0;
					dMaxRes  = TransInfo::char2int(visib[1]);
					range.second = (int) (pow( 2,dMaxRes+1 ));
				}
				else if( ( '_' == visib[1] ) && ('_' != visib[0]) )
				{
					range.second = 0;
					dMiniRes = TransInfo::char2int(visib[0]);
					range.first  = (int) (pow( 2,dMiniRes ));
				}
				else if( ( '_' == visib[1] ) && ('_' == visib[0]) )
				{
					range.first  = 0;
					range.second = 1;
				}
				else
				{
					dMiniRes = TransInfo::char2int(visib[0]);
					dMaxRes  = TransInfo::char2int(visib[1]);
					/*range.first = TransInfo::char2int(visib[0]);
					range.second = TransInfo::char2int(visib[1]);*/
					range.first  = (int) (pow( 2,dMiniRes ));
					range.second = (int) (pow( 2,dMaxRes+1));
				}

				if(res < range.first || res >= range.second)	// should not request this overlay, drop it
				{
					overlay = overlays.getNextOverlay();
					continue;
				}
			}
			else if(hasLatLong){}
			else //find the last digit of overlay to define resolution
			{
				//if( iDot != string::npos )
				//{
				//	preOverlay = overlay.substr(0, iDot);
				//}
				//else
				//{
                    preOverlay = overlay;
				//}

				nPosLastDigit  = ( (int)(preOverlay.length()) ) - 1;
				string strLast = preOverlay.substr( nPosLastDigit );
				dLastDigit     = TransInfo::char2int( strLast[0] );
				if( '_' == strLast[0] )
				{
					dResRange = 8;
					overlay   = overlay.substr(0, nPosLastDigit);
				}
				else if( '0'==strLast[0] || '1'==strLast[0] || 
					     '2'==strLast[0] || '3'==strLast[0] ||
					     '4'==strLast[0] || '5'==strLast[0] )
				{
					dResRange = pow( 2,dLastDigit+5 );
					overlay   = overlay.substr(0, nPosLastDigit);
				}
				else
				{
					dResRange = res + 1;
				}

				if( res >= dResRange )	// should not request this overlay, drop it
				{
					overlay = overlays.getNextOverlay();
					continue;
				}

			}

			
			if(reqManager.hasFALayers(overlay))
			{
				
				if(reqManager.hasFAParcel(overlay))
				{
					reqManager.addFALayers("parcel");
					hasFAParcel = true;
				}
				hasFALayer = true;
				overlay = overlays.getNextOverlay();
				continue;
			}

			/*Overlay2Qix o2q(overlay, rect.minX, rect.maxX, rect.minY, rect.maxY, res);

			files = o2q.getQixFile(configure.getLevelOneDir(), configure.getLevelTwoDir());*/
			if( hasLatLong || hasLatLongStreetRequest )
			{
				string str_Files = reqManager.GetLatLongFiles(overlay);
				//files = str_Files;
				files.push_back(str_Files);
			}
			/*else if(hasFALayer)
			{
			}*/
			else
			{
				Overlay2Qix o2q(overlay, rect.minX, rect.maxX, rect.minY, rect.maxY, res);
				files = o2q.getQixFile(configure.getLevelOneDir(), configure.getLevelTwoDir());
			}


			/*CString test = files.at(0).c_str();
			logObj.varWriteToLog(_INFO, "ss", "shp file name: ", test);*/



			for(int i = 0; i < (int)(files.size()); i++)
			{
				char* ids = NULL;
				ShapeRecords record = shapeRecord;

				string baseName = files[i].substr(0, files[i].length()-3);
				record.setShpBaseName(baseName);

				char name[MAX_PATH_LENGTH] = {0};
				strcpy(name, baseName.c_str());
				ids = msSearchDiskTree(name, rect, 0);

				int number, type;
				double min[4];
				double max[4];
				SHPHandle handle = SHPOpen(name, "rb");
				SHPGetInfo(handle, &number, &type, min, max);

				msFilterTreeSearch(handle, number, ids, rect, record);

				/*int nsize = record.size();
				char* test = new char[100];
				itoa(nsize,test,10);
				logObj.varWriteToLog(_INFO, "ss", "record size: ", test);
				delete test;*/

				SHPClose(handle);

				record.setShpType(type);
				free(ids);
				shapeRecords.push_back(record);
			}

			overlay = overlays.getNextOverlay();
		}


		logObj.reInit();

		//CShape* pShape = NULL;*/
		CQixShapeFileLayerDataSource dataSource, dataSourcePoints, dataSourceArea;
		//////////////////////////////////////
		dataSource.SetExtend( rcSourceExt );
        //////////////////////////////////////

		CLineLayer layerStreet, layerStreetMerged;
		CPointLayer layerPoints;
		CPolygonLayer layerArea;

		//Line		
		layerStreet.m_asPropertiesField.Add( CString("HIGHWAY_NM") );//name
		layerStreet.m_asPropertiesField.Add( CString("ST_NAME") );//name
		layerStreet.m_asPropertiesField.Add( CString("streername") );//name
		layerStreet.m_asPropertiesField.Add( CString("LabelCQ") );//color of the label
		layerStreet.m_asPropertiesField.Add( CString("LineCQ") );//color of the line
		layerStreet.m_asPropertiesField.Add( CString("NAME1") );//samotrails name
		layerStreet.m_asPropertiesField.Add( CString("labeltq") );//new name

		layerStreet.m_asPropertiesField.Add( CString("NAME") );//wm_Water_line
		layerStreet.m_asPropertiesField.Add( CString("LOCAL_NUM") );//wm_Minor_routes
		layerStreet.m_asPropertiesField.Add( CString("ID") );//wm_Railroad_track

        layerStreet.m_asPropertiesField.Add( CString("POLYGON_NM") );//polyline
		layerStreet.m_asPropertiesField.Add( CString("OBJECTID") );//fiber_optics

		//layerStreet.m_asPropertiesField.Add( CString("st_nm_base") );

		//Point
		layerPoints.m_asPropertiesField.Add( CString("labeltq") );//name
		layerPoints.m_asPropertiesField.Add( CString("LABELCQ") );
		layerPoints.m_asPropertiesField.Add( CString("PictoFQ") );//picto
		layerPoints.m_asPropertiesField.Add( CString("column1") );//another name
		layerPoints.m_asPropertiesField.Add( CString("column10") );//dining&business point picto 
		layerPoints.m_asPropertiesField.Add( CString("OWNER") );
		layerPoints.m_asPropertiesField.Add( CString("ADDR") );

		layerPoints.m_asPropertiesField.Add( CString("column6") );//hotel point's stars
		layerPoints.m_asPropertiesField.Add( CString("LABEL1TQ") );//mulr second label part
		layerPoints.m_asPropertiesField.Add( CString("CITYNAME") );//city name
		layerPoints.m_asPropertiesField.Add( CString("STAGE") );//rtgauges name
		layerPoints.m_asPropertiesField.Add( CString("SHORTNAME") );//ptrim name
		layerPoints.m_asPropertiesField.Add( CString("FLOW") );//rtgauges name
		layerPoints.m_asPropertiesField.Add( CString("NAME1") );//samotrails name

		layerPoints.m_asPropertiesField.Add( CString("Offset") );//wm_Time_Zones name
		layerPoints.m_asPropertiesField.Add( CString("NAME") );//wm_Airports name
		layerPoints.m_asPropertiesField.Add( CString("LOCAL_NAME") );//wm_Capitals name
		layerPoints.m_asPropertiesField.Add( CString("Site") );//IDP
		layerPoints.m_asPropertiesField.Add( CString("ID") );//wm_Glacier name
		layerPoints.m_asPropertiesField.Add( CString("Name") );//wm_Great_lake name



		//Area
		layerArea.m_asPropertiesField.Add( CString("labeltq") );//name
		layerArea.m_asPropertiesField.Add( CString("LABELCQ") );
		layerArea.m_asPropertiesField.Add( CString("BORDERCQ") );
		layerArea.m_asPropertiesField.Add( CString("AREACQ") );
		layerArea.m_asPropertiesField.Add( CString("NAME") );//for new area name
		layerArea.m_asPropertiesField.Add( CString("TRACT") );//for bg name
		layerArea.m_asPropertiesField.Add( CString("ZIP") );//for zip area

		layerArea.m_asPropertiesField.Add( CString("Admin_Name") );//wm_Admin
		layerArea.m_asPropertiesField.Add( CString("ID") );//wm_Glacier
		layerArea.m_asPropertiesField.Add( CString("POLYGON_NM") );//polygon
		layerArea.m_asPropertiesField.Add( CString("Cntry_Name") );//wm_nations
		layerArea.m_asPropertiesField.Add( CString("PARCEL_ID") );//wm_Great_lake name




		CDataBaseLayerDataSource dataSource_db;
		dataSource_db.SetDatabaseType(SQLSERVER);
		/*dataSource_db.setFAOwner(hasFAOwner);
		dataSource_db.setFAADDR(hasFAADDR);*/
		dataSource_db.SetLODLevel(16, "16");

		//faparcel
		if(hasFALayer)
		{
			if( !dataSource_db.OpenDatabase() )
			{
				logObj.writeToLog(_ERROR, "openDatabase");
				return nRetCode;
			}
			dataSource_db.SetExtend( rcSourceExt );
		}
		if(hasFAParcel)
		{
			CPolygonLayer faparcel;
			dataSource_db.SetTable("parcel");
			if(!faparcel.LoadDataFromDatabase(dataSource_db))
			{
				logObj.writeToLog(_ERROR, "LoadDataFromDatabase_parcel");
			}
			//GIF output
			faparcel.m_formatPaintTool.setGIFOutput( hasGIFOutput );
			//transformer.Convert( &faparcel );

			transformer.Convert( &faparcel, nlevel, nMSOffsetX, nMSOffsetY );
			faparcel.m_formatPaintTool.hasTransInfo = true;
			faparcel.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
			faparcel.m_formatPaintTool.setGIFOutput( hasGIFOutput );
			faparcel.m_formatPaintTool.setLatLong( hasLatLong );
			faparcel.m_formatPaintTool.setLabelDensity(nlabeldensity);

			//set holder
			//faparcel.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
			//set transformer
			faparcel.m_formatPaintTool.SetTransformer( &transformer );

			faparcel.DrawShape(&canvas);
		}


		if( hasFilledAreaReq )
		{
			for(int i = 0; i < (int) (shapeRecords.size()); i++)
			{
				ShapeRecords record = shapeRecords[i];

				CString sFile(record.getShpBaseName().c_str());
				dataSource.FromFile(sFile);
				dataSource.setShapeRecords(record);
				int type = record.getShpType();

				if( ShapeUtility::polygon == type )
				{
					if(record.hasTransInfo())
						layerArea.m_formatPaintTool.setTransInfo(record.getTransInfo());
					// set dbf attributes
					layerArea.LoadData(dataSource);
					//for MS coordinates convert
					transformer.Convert( &layerArea, nlevel, nMSOffsetX, nMSOffsetY );
					//holder
					//layerArea.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
					//setSM
					layerArea.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
					layerArea.m_formatPaintTool.setGIFOutput( hasGIFOutput );
					layerArea.m_formatPaintTool.setLatLong( hasLatLong );
					layerArea.m_formatPaintTool.setLabelDensity(nlabeldensity);

					if(record.hasColorInfo())
					{
						layerArea.m_formatPaintTool.colorInfo = record.getColorInfo();
					}
					//set transformer
					//layerArea.m_formatPaintTool.SetTransformer( &transformer );

					//set highlightArea
					if( hasHighlightArea )
					{
						//layerArea.m_formatPaintTool.setHighlightAreaID( strHighlightAreaID, hasHighlightArea);
						layerArea.SetHighlightAreaValues( strHighlightAreaID, hasHighlightArea );
					}
					//set highlightCenter
					if( record.GetHighlightCenterInfo() )
					{
						layerArea.setHighlightBoundary( rcDestExt, record.GetHighlightCenterInfo() );
					}
					else
					{
						layerArea.HasHLCenter = false;
					}

					if( !layerArea.DrawShape(&canvas) )
					{
						continue;
					}
					//layerArea.DrawShape( &canvas );
				}
			}
		}

		//fapoints
		if(hasFAOwner)
		{
			CPointLayer faownerpoints;
			dataSource_db.SetTable("fapointsowner");

			if(!faownerpoints.LoadDataFromDatabase(dataSource_db))
			{
				logObj.writeToLog(_ERROR, "LoadDataFromDatabase_parcel_owner");
			}
			//GIF output
			faownerpoints.m_formatPaintTool.setGIFOutput( hasGIFOutput );
			transformer.Convert( &faownerpoints );
			//set holder
			faownerpoints.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
			//set transformer
			faownerpoints.m_formatPaintTool.SetTransformer( &transformer );
			faownerpoints.m_formatPaintTool.hasTransInfo = true;
			faownerpoints.m_formatPaintTool.hasFAPoint   = true;
			faownerpoints.DrawShape(&canvas);
			faownerpoints.DrawLabel(&canvas);




			//CLineLayer layerStreettest;
			//dataSource_db.SetTable("fapointsowner");

			//if(!layerStreettest.LoadDataFromDatabase(dataSource_db))
			//{
			//	logObj.writeToLog(_ERROR, "LoadDataFromDatabase_parcel_owner");
			//}
			////GIF output
			//layerStreettest.m_formatPaintTool.setGIFOutput( hasGIFOutput );
			//transformer.Convert( &layerStreettest );
			////set holder
			//layerStreettest.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
			////set transformer
			//layerStreettest.m_formatPaintTool.SetTransformer( &transformer );
			//faparcel.m_formatPaintTool.hasTransInfo = true;
			//layerStreettest.DrawShape(&canvas);
			//layerStreettest.DrawLabel(&canvas);
			
		}
		if(hasFAADDR)
		{
			//CPointLayer faaddrpoints;
			//dataSource_db.SetTable("fapointsaddr");

			//if(!layerPoints.LoadDataFromDatabase(dataSource_db))
			//{
			//	logObj.writeToLog(_ERROR, "LoadDataFromDatabase_parcel_addr");
			//}
			////GIF output
			//faaddrpoints.m_formatPaintTool.setGIFOutput( hasGIFOutput );
			//transformer.Convert( &faaddrpoints );
			////set holder
			//faaddrpoints.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
			////set transformer
			//faaddrpoints.m_formatPaintTool.SetTransformer( &transformer );
			//faaddrpoints.m_formatPaintTool.hasTransInfo = true;
			//faaddrpoints.DrawShape(&canvas);
			//faaddrpoints.DrawLabel(&canvas);


			CPointLayer faaddrpoints;
			dataSource_db.SetTable("fapointsaddr");

			if(!faaddrpoints.LoadDataFromDatabase(dataSource_db))
			{
				logObj.writeToLog(_ERROR, "LoadDataFromDatabase_parcel_owner");
			}
			//GIF output
			faaddrpoints.m_formatPaintTool.setGIFOutput( hasGIFOutput );
			transformer.Convert( &faaddrpoints );
			//set holder
			faaddrpoints.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
			//set transformer
			faaddrpoints.m_formatPaintTool.SetTransformer( &transformer );
			faaddrpoints.m_formatPaintTool.hasTransInfo = true;
			faaddrpoints.m_formatPaintTool.hasFAPoint   = true;
			faaddrpoints.DrawShape(&canvas);
			faaddrpoints.DrawLabel(&canvas);
			
		}

		////////////////////////////////////////////////////////////
		//customized layer should be here
		if( hasStrPoint )  //process the customised points
		{
			CString decodedStrPt = reqManager.InitRequest(strPoints);
			strPoints = decodedStrPt;

			CStringLayerDataSource strDataSource;
			CPointLayer strLayerPoints;
			strLayerPoints.m_asPropertiesField.Add( CString("name") );//name,do not use it

			strDataSource.FromString( strPoints );

			strLayerPoints.LoadData( strDataSource );

			//XML output
			//set XML output into formatPaintTool
			strLayerPoints.m_formatPaintTool.setXMLOutput( hasXMLOutput );
			strLayerPoints.m_formatPaintTool.setXMLUTM( hasXMLUTM );
			strLayerPoints.m_formatPaintTool.setLabelDensity(nlabeldensity);

			//GIF output
			strLayerPoints.m_formatPaintTool.setGIFOutput( hasGIFOutput );

			transformer.Convert( &strLayerPoints );

			//set holder
			strLayerPoints.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
			//set transformer
			strLayerPoints.m_formatPaintTool.SetTransformer( &transformer );

			strLayerPoints.DrawShape( &canvas );
			strLayerPoints.DrawLabel( &canvas );	
		}
		/////////////////////////////////////////////////////////////

		//queue<CLineLayer> mergedStreets;
		// traverse all the layers in the vector
		for(int i = 0; i < (int) (shapeRecords.size()); i++)
		{
			ShapeRecords record = shapeRecords[i];

			CString sFile(record.getShpBaseName().c_str());
			dataSource.FromFile(sFile);
			dataSource.setShapeRecords(record);
			int type = record.getShpType();

			switch(type){
				case ShapeUtility::arc:	//line
					{
						//shapeLayer  = new CLineLayer();
						bool hasTransInfo = false;
						bool hasColorInfo = false;
						TransInfo info;

						if(record.hasTransInfo())
						{
							hasTransInfo = true;

							info = record.getTransInfo();
							layerStreet.m_formatPaintTool.transInfo = info;	

						}
						if(record.hasColorInfo())
						{
							hasColorInfo = true;
							//layerStreet.m_formatPaintTool.colorInfo = record.getColorInfo();
						}

						layerStreet.LoadData(dataSource);
						layerStreet.SetLabelFieldIndex(0); // set street name						
						logObj.writeToLog(_INFO, "Beging merge.");
						layerStreet.MergePartsTo(&layerStreetMerged);
						logObj.writeToLog(_INFO, "End merge.");
						//GIF output
						layerStreetMerged.m_formatPaintTool.setGIFOutput( hasGIFOutput );
						//Arc XML output
						//set XML output into formatPaintTool
						layerStreetMerged.m_formatPaintTool.setXMLOutput( hasXMLOutput );
						layerStreetMerged.m_formatPaintTool.setXMLUTM( hasXMLUTM );

						transformer.Convert( &layerStreetMerged );

						//set holder
						layerStreetMerged.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
						//set transformer
					    layerStreetMerged.m_formatPaintTool.SetTransformer( &transformer );
						logObj.writeToLog(_INFO, "bEGINGing draw label.");
						//setSM
						layerStreetMerged.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );

						//layerStreetMerged.m_formatPaintTool.setLabelDensity(nlabeldensity);

						if(hasTransInfo)
						{
							if(info.values[0] == 'l' || info.values[0] == 'L' ||
								info.values[0] == 'w' || info.values[0] == 'W')
								layerStreetMerged.DrawShape( &canvas );

							//if(info.values[1] == 'n' || info.values[1] == 'N')
							//		layerStreetMerged.DrawLabel( &canvas );
						}
						else if(hasColorInfo)
						{
							layerStreetMerged.m_formatPaintTool.colorInfo = record.getColorInfo();
							layerStreetMerged.DrawShape( &canvas );
							//layerStreetMerged.DrawLabel( &canvas );
						}
						else if( hasXMLOutput )  //XML Output
						{
							layerStreetMerged.DrawShape( &canvas );
						}

						logObj.writeToLog(_INFO, "Ending draw label.");
						//layerStreetMerged.m_formatPaintTool.crLalelColor = ;
						break;

					}
				case ShapeUtility::point:	// point
					if( (record.getTransInfo().values[0] == 'l') || 
						(record.getTransInfo().values[0] == 'L') )
					{
						break;
					}

					char c = record.getTransInfo().values[0];

					//for FA_Owner
					if( (record.getTransInfo().values[0] == 'o') || 
						(record.getTransInfo().values[0] == 'O'))
					{
						layerPoints.m_formatPaintTool.iSpecifiedLableColumn = FA_OWNER;
					}
					//for FA_ADDR
					if((record.getTransInfo().values[0] == 'a') ||
						(record.getTransInfo().values[0] == 'A'))
					{
						layerPoints.m_formatPaintTool.iSpecifiedLableColumn = FA_ADDR;
					}

					// set dbf attributes
					layerPoints.LoadData(dataSource);
					//GIF output
					layerPoints.m_formatPaintTool.setGIFOutput( hasGIFOutput );
					//picto XML output
					//set XML output into formatPaintTool
					layerPoints.m_formatPaintTool.setXMLOutput( hasXMLOutput );
					layerPoints.m_formatPaintTool.setXMLUTM( hasXMLUTM );

					layerPoints.m_formatPaintTool.setLabelDensity(nlabeldensity);

					//transformer.Convert( &layerPoints );
					//for MS coordinates convert
					transformer.Convert( &layerPoints, nlevel, nMSOffsetX, nMSOffsetY );
					//set holder
					layerPoints.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
					//set SM
					layerPoints.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
					//set transformer
					layerPoints.m_formatPaintTool.SetTransformer( &transformer );


					/*if((!record.hasTransInfo()) || (record.getTransInfo().values[0] == 'i') || 
						(record.getTransInfo().values[0] == 'I'))*/
					//{
					layerPoints.DrawShape( &canvas );
					//}
					//layerPoints.DrawShape( &canvas );
					break;
				//case ShapeUtility::polygon:	// area
				//	//shapeLayer  = new CPolygonLayer();
				//	if(record.hasTransInfo())
				//		layerArea.m_formatPaintTool.setTransInfo(record.getTransInfo());
				//	// set dbf attributes
				//	layerArea.LoadData(dataSource);
				//	transformer.Convert( &layerArea);
				//	//holder
				//	//layerArea.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
				//	//setSM
				//	layerArea.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
				//	//set transformer
				//	//layerArea.m_formatPaintTool.SetTransformer( &transformer );
				//	layerArea.DrawShape( &canvas );
			}		
		}

		CQixShapeFileLayerDataSource dataSource1;
		////////////////////////////////////
		dataSource1.SetExtend(rcSourceExt);
		////////////////////////////////////

		CLineLayer layerStreet1, layerStreetMerged1;
		CPointLayer layerPoints1;
		CPolygonLayer layerArea1;

		//Line
		//layerStreet1.m_asPropertiesField.Add( CString("streername") );//name
		////layerStreet1.m_asPropertiesField.Add( CString("ST_NAME") );//name
		//layerStreet1.m_asPropertiesField.Add( CString("LabelCQ") );//color of the label
		//layerStreet1.m_asPropertiesField.Add( CString("LineCQ") );//color of the line

		layerStreet1.m_asPropertiesField.Add( CString("HIGHWAY_NM") );//name
		layerStreet1.m_asPropertiesField.Add( CString("ST_NAME") );//name
		layerStreet1.m_asPropertiesField.Add( CString("streername") );//name
		layerStreet1.m_asPropertiesField.Add( CString("LabelCQ") );//color of the label
		layerStreet1.m_asPropertiesField.Add( CString("LineCQ") );//color of the line
		layerStreet1.m_asPropertiesField.Add( CString("NAME1") );//samotrails name
		layerStreet1.m_asPropertiesField.Add( CString("labeltq") );//new name

		layerStreet1.m_asPropertiesField.Add( CString("NAME") );//wm_Water_line
		layerStreet1.m_asPropertiesField.Add( CString("LOCAL_NUM") );//wm_Minor_routes
		layerStreet1.m_asPropertiesField.Add( CString("ID") );//wm_Railroad_track

		layerStreet1.m_asPropertiesField.Add( CString("POLYGON_NM") );
		layerStreet1.m_asPropertiesField.Add( CString("OBJECTID") );//fiber_optics
		//layerStreet.m_asPropertiesField.Add( CString("st_nm_base") );

		//Point
		layerPoints1.m_asPropertiesField.Add( CString("labeltq") );//name
		layerPoints1.m_asPropertiesField.Add( CString("LABELCQ") );
		layerPoints1.m_asPropertiesField.Add( CString("PictoFQ") );//picto
		layerPoints1.m_asPropertiesField.Add( CString("column1") );//another name
		layerPoints1.m_asPropertiesField.Add( CString("column10") );//dining&business point picto 
		layerPoints1.m_asPropertiesField.Add( CString("OWNER") );//wm_Great_lake name
		layerPoints1.m_asPropertiesField.Add( CString("ADDR") );//wm_Great_lake name
		layerPoints1.m_asPropertiesField.Add( CString("column6") );//hotel point stars picto
		layerPoints1.m_asPropertiesField.Add( CString("LABEL1TQ") );//mulr second label part
		layerPoints1.m_asPropertiesField.Add( CString("CITYNAME") );//city name
		layerPoints1.m_asPropertiesField.Add( CString("STAGE") );//rtgauges name
		layerPoints1.m_asPropertiesField.Add( CString("SHORTNAME") );//ptrim name
		layerPoints1.m_asPropertiesField.Add( CString("FLOW") );//rtgauges name
		layerPoints1.m_asPropertiesField.Add( CString("NAME1") );//samotrails name

		layerPoints1.m_asPropertiesField.Add( CString("Offset") );//wm_Time_Zones name
		layerPoints1.m_asPropertiesField.Add( CString("NAME") );//wm_Airports name
		layerPoints1.m_asPropertiesField.Add( CString("LOCAL_NAME") );//wm_Capitals name
		layerPoints1.m_asPropertiesField.Add( CString("Site") );//IDP
		layerPoints1.m_asPropertiesField.Add( CString("ID") );//wm_Glacier name
		layerPoints1.m_asPropertiesField.Add( CString("Name") );//wm_Great_lake name


		//Area
		layerArea1.m_asPropertiesField.Add( CString("labeltq") );//name
		layerArea1.m_asPropertiesField.Add( CString("LABELCQ") );
		layerArea1.m_asPropertiesField.Add( CString("BORDERCQ") );
		layerArea1.m_asPropertiesField.Add( CString("AREACQ") );
		layerArea1.m_asPropertiesField.Add( CString("NAME") );//for area name
		layerArea1.m_asPropertiesField.Add( CString("TRACT") );//for bg name
		layerArea1.m_asPropertiesField.Add( CString("ZIP") );//for zip area

		layerArea1.m_asPropertiesField.Add( CString("Admin_Name") );//wm_Admin
		layerArea1.m_asPropertiesField.Add( CString("ID") );//wm_Glacier

		layerArea1.m_asPropertiesField.Add( CString("POLYGON_NM") );//polygon
		layerArea1.m_asPropertiesField.Add( CString("Cntry_Name") );//wm_nations
		layerArea1.m_asPropertiesField.Add( CString("PARCEL_ID") );//wm_Great_lake name

		// traverse all the layers in the vector, draw labels
		for(int i = 0; i < (int) (shapeRecords.size()); i++)
		{
			ShapeRecords record = shapeRecords[i];
			CString sFile(record.getShpBaseName().c_str());	
			dataSource1.FromFile(sFile);
			dataSource1.setShapeRecords(record);
			int type = record.getShpType();

			switch(type){
				case ShapeUtility::arc:	//line
					{
						bool hasTransInfo = false;
						bool hasColorInfo = false;
						layerStreetMerged1.m_formatPaintTool.hasStreets = false;
						layerStreetMerged1.m_formatPaintTool.hasSechwys = false;
						layerStreetMerged1.m_formatPaintTool.hasMajHwys = false;
						layerStreetMerged1.m_formatPaintTool.hasMainroads = false;
						layerStreetMerged1.m_formatPaintTool.hasDetailroads = false;
						TransInfo info;
						if(record.hasTransInfo())
						{
							hasTransInfo = true;

							info = record.getTransInfo();
							layerStreet1.m_formatPaintTool.transInfo = info;	

						}
						if(record.hasColorInfo())
						{
							hasColorInfo = true;
							//layerStreet.m_formatPaintTool.colorInfo = record.getColorInfo();
						}
						layerStreet1.LoadData(dataSource1);
						layerStreet1.SetLabelFieldIndex(0); // set street name						
						//logObj.writeToLog(_INFO, "Beging merge.");
						layerStreet1.MergePartsTo(&layerStreetMerged1);
						//logObj.writeToLog(_INFO, "End merge.");
						transformer.Convert( &layerStreetMerged1 );
						//set holder
						layerStreetMerged1.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
						//setSM
						layerStreetMerged1.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
						//GIF output
						layerStreetMerged1.m_formatPaintTool.setGIFOutput( hasGIFOutput );

						layerStreetMerged1.m_formatPaintTool.setLabelDensity(nlabeldensity);

						//set street type
						if( sFile.Find("streets") > 0 )
						{
							layerStreetMerged1.m_formatPaintTool.hasStreets = true;
						}
						else if( sFile.Find("Sechwys") > 0 )
						{
							layerStreetMerged1.m_formatPaintTool.hasSechwys = true;
						}
						else if( sFile.Find("MajHwys") > 0 )
						{
							layerStreetMerged1.m_formatPaintTool.hasMajHwys = true;
						}
						else if( sFile.Find("Mainroads") > 0)
						{
							layerStreetMerged1.m_formatPaintTool.hasMainroads = true;
						}
						else if( sFile.Find("detailroads") > 0)
						{
							layerStreetMerged1.m_formatPaintTool.hasDetailroads = true;
						}

						logObj.writeToLog(_INFO, "bEGINGing draw label.");

						layerStreetMerged1.m_formatPaintTool.SetTransformer( &transformer );

						if(hasTransInfo)
						{
							//if(info.values[0] == 'l' || info.values[0] == 'L')
							//		layerStreetMerged.DrawShape( &canvas );

							if(info.values[1] == 'n' || info.values[1] == 'N' ||
								info.values[1] == 'w' || info.values[1] == 'W')
								layerStreetMerged1.DrawLabel( &canvas );
						}
						else if(hasColorInfo)
						{
							layerStreetMerged1.m_formatPaintTool.colorInfo = record.getColorInfo();
							//layerStreetMerged.DrawShape( &canvas );
							layerStreetMerged1.DrawLabel( &canvas );
						}
						//else if( hasXMLOutput )  //XML Output
						//{
						//	//layerStreetMerged1.m_formatPaintTool.SetTransformer( &transformer );
						//	layerStreetMerged1.DrawShape( &canvas );
						//}
						logObj.writeToLog(_INFO, "Ending draw label.");
						//layerStreetMerged.m_formatPaintTool.crLalelColor = ;
						break;

					}
				case ShapeUtility::point:	// point

					if( (record.getTransInfo().values[0] == 'i') || 
						(record.getTransInfo().values[0] == 'I'))
					{
						break;
					}

					//for FA_Owner
					if( (record.getTransInfo().values[0] == 'o') || 
						(record.getTransInfo().values[0] == 'O'))
					{
						layerPoints1.m_formatPaintTool.iSpecifiedLableColumn = FA_OWNER;
					}
					//for FA_ADDR
					if( (record.getTransInfo().values[0] == 'a') || 
						(record.getTransInfo().values[0] == 'A'))
					{
						layerPoints1.m_formatPaintTool.iSpecifiedLableColumn = FA_ADDR;
					}

					// set dbf attributes
					layerPoints1.LoadData(dataSource1);
					//point XML output
					//set XML output into formatPaintTool
					layerPoints1.m_formatPaintTool.setXMLOutput( hasXMLOutput );
					layerPoints1.m_formatPaintTool.setXMLUTM( hasXMLUTM );
					//set GIF output
					layerPoints1.m_formatPaintTool.setGIFOutput( hasGIFOutput );
					//transformer.Convert( &layerPoints1 );
					//for MS coordinates convert
					transformer.Convert( &layerPoints1, nlevel, nMSOffsetX, nMSOffsetY );
					//set holder
					layerPoints1.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
					//set SM
					layerPoints1.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
                    //set transformer
					layerPoints1.m_formatPaintTool.SetTransformer( &transformer );

					layerPoints1.m_formatPaintTool.setLabelDensity(nlabeldensity);

					/*if((!record.hasTransInfo()) || (record.getTransInfo().values[0] == 'l') || 
						(record.getTransInfo().values[0] == 'L'))*/
					//{
					layerPoints1.DrawLabel( &canvas );
					//}
					//layerPoints1.DrawLabel( &canvas );
					break;
				case ShapeUtility::polygon:	// area
					//shapeLayer  = new CPolygonLayer();
					if(record.hasTransInfo())
						layerArea1.m_formatPaintTool.setTransInfo(record.getTransInfo());
					//set dbf attributes
					layerArea1.LoadData( dataSource1 );
					//set GIF output
					layerArea1.m_formatPaintTool.setGIFOutput( hasGIFOutput );
                    //XML output
					layerArea1.m_formatPaintTool.setXMLOutput( hasXMLOutput );
					layerArea1.m_formatPaintTool.setXMLUTM( hasXMLUTM );

					layerArea1.m_formatPaintTool.setLabelDensity(nlabeldensity);

					//for MS coordinates convert
					transformer.Convert( &layerArea1, nlevel, nMSOffsetX, nMSOffsetY );
					//set holder
					layerArea1.m_formatPaintTool.m_pPlaceHolder = &g_placeHolder;
					//setSM
					layerArea1.m_formatPaintTool.setLetterEnlargeRatio( dLetterEnlargeRatio );
					layerArea1.m_formatPaintTool.SetTransformer( &transformer );
					layerArea1.DrawLabel( &canvas );
			}		
		}
		// destroy shape objects here, otherwise memory leak
		for(int i = 0; i < (int) (shapeRecords.size()); i++)
			shapeRecords[i].destroyRecords();

		logObj.writeToLog(_INFO, "begining transforming image.");

#ifdef TERRA_NODE

		if( hasXMLOutput )
		{
			canvas.SaveToXMLStream();
		}
		else if( hasGIFOutput )
		{
			canvas.setTransGIF( hasTransGIFOutput );
			canvas.SaveToGIFStream();
		}
		else
		{
			canvas.SaveToStream();
		}

#else

		if( hasXMLOutput && (!hasTxt) )
		{
			CString sImageFileName = CString("..\\Output\\XMLMap")  + CString(".xml");
			canvas.SaveToXMLFile( sImageFileName );
		}
		else if( hasGIFOutput )
		{
			CString sImageFileName = CString("..\\Output\\GIFMap")  + CString(".gif");
			canvas.setTransGIF( hasTransGIFOutput );
			canvas.SaveToGIFFile( sImageFileName );
		}
		else if( hasTxt && hasXMLOutput )
		{
			CString sImageFileName = CString("G:\\Output\\TxtMap")  + CString(".txt");
			canvas.SaveToTxtFile( sImageFileName );
		}
		else
		{
			CString sImageFileName = CString("..\\Output\\JPGMap")  + CString(".jpg");
			//CString sImageFileName = CString("..\\Output\\JPGMap")  + CString(".png");
			LOG("Information: begin save image \n");
			canvas.SaveToFile( sImageFileName );
			LOG("Information: end save image \n");
			logObj.writeToLog(_INFO, "End processing.");
		}
		//_CrtDumpMemoryLeaks();

#endif

	}

	return nRetCode;
}