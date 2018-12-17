#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <atlimage.h>

class ImageTransformer
{
	CImage& img;
public:
	ImageTransformer(CImage& image):img(image){}

	void img2ppm(char*);
	//void img2xml();
};

#endif