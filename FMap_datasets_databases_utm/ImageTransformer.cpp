//#include "stdafx.h"
#include "configure.h"
#include "ImageTransformer.h"
using namespace std;

void ImageTransformer::img2ppm(char* buf)
{
	if(buf == NULL)
	{
		//log error
		exit(1);
	}

	int width = img.GetWidth();
	int height = img.GetHeight();

	/*for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			COLORREF color = img.GetPixel(j, i);
			buf[i*width*3+j*3] = GetRValue(color);
			buf[i*width*3+j*3+1] = GetGValue(color);
			buf[i*width*3+j*3+2] = GetBValue(color);
		}
	}*/

	//int pitch = img.GetPitch();
	for(int i = 0; i < height; i++)
		memcpy(buf+i*width*3, img.GetPixelAddress(0, i), width*3);

}