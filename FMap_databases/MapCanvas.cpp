#include "StdAfx.h"
#include "FormatPaintTool.h"
#include "ShapeObject.h"
#include "MapCanvas.h"
#include "fmap.h"
#include "ImageTransformer.h"
#include "configure.h"
#include <string>
#include <io.h>
#include <fcntl.h>

#include "atlimage.h"
#include <gdiplus.h>
#include <windows.h>
#include <stdio.h>
#include <gdiplusbitmap.h>
#include <Gdipluspixelformats.h>

#include "TransInfoManager.h"
 
using namespace std;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) 
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

   Gdiplus::GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}


CMapCanvas::CMapCanvas(void)
{
	m_pDCCanvas = NULL;
	hasTransGIF = false;
	nTotalPoint = 0;
	nTotalArc   = 0;
	nTotalArea  = 0;
	nTotalIcon  = 0;

}

CMapCanvas::~CMapCanvas(void)
{
	ReleaseCanvas();
}

bool CMapCanvas::InitCanvas(int nWidth, int nHeight, COLORREF crBKColor, bool bTransparence , int nBPP, bool hasGIFOutput, TransInfo* seastrans)
{

//	ReleaseCanvas();

	 // could be CreateEx for powerful features

	nTotalPoint = 0;
	nTotalArc   = 0;
	nTotalArea  = 0;
	nTotalIcon  = 0;

	//DWORD adwBitmasks[3] = { 0x0000000f, 0x000000f0, 0x00000f00 };
    //image.CreateEx( 100, 100, 16, BI_BITFIELDS, adwBitmasks, 0 );


	//if (m_Image.Create(nWidth, nHeight, nBPP, BI_RGB) == false)
	if (m_Image.Create(nWidth, nWidth, nBPP) == false)
	//if (m_Image.CreateEx( nWidth, nWidth, nBPP, BI_RGB, adwBitmasks, 1 ) == false)
	{
		ASSERT(false);
		LOG("could not create Image");
		return false;
	}
	m_pDCCanvas = CDC::FromHandle(m_Image.GetDC());
	ASSERT( m_pDCCanvas != NULL );

	/*LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbHatch = HS_CROSS;
	logBrush.lbColor = RGB(3,3,3);
	CBrush brush;
	VERIFY(brush.CreateBrushIndirect(&logBrush));
	CRect rc(0, 0, nWidth, nHeight);
	m_pDCCanvas->FillRect( &rc, &brush); */

	//COLORREF test;
	//if (bTransparence == false)
	//{
	//	LOGBRUSH logBrush;
	//	logBrush.lbStyle = BS_SOLID;
	//	CTransInfoManager tranManager;
	//	int trans = seastrans->values[1];
	//	if( hasGIFOutput )
	//	{
	//		//for gif
	//		logBrush.lbColor = RGB(3,3,255);
	//	}
	//	else
	//	{
	//		//add transinfo
	//		tranManager.SetTransInfo( trans, RGB(3,3,255) );
	//		//for jpg and ppm, the color is reversed
	//		logBrush.lbColor = tranManager.GetTransInfo();
	//	}
	//	logBrush.lbHatch = HS_CROSS;
	//	CBrush brush;
	//	VERIFY(brush.CreateBrushIndirect(&logBrush));
	//	CRect rc(0, 0, nWidth, nHeight);
	//	m_pDCCanvas->FillRect( &rc, &brush); 
	//}
	
	return true;
}


bool CMapCanvas::setTransGIF( bool hasTransGIFOutput )
{
	hasTransGIF = hasTransGIFOutput;
	return hasTransGIF;
}

bool CMapCanvas::setTransPNG( bool hasTransPNGOutput )
{
	hasTransPNG = hasTransPNGOutput;
	return hasTransPNG;
}

int CMapCanvas::ConstructGif(char* bufout)
{
	ASSERT( m_Image != NULL );
	if(bufout == NULL)
	{
		exit(1);
	}

    LARGE_INTEGER ui = {0, 0};
	ULONG read = 0;
	IStream* pStream;
	CreateStreamOnHGlobal(NULL, TRUE, &pStream);  

	//save to GIF
	HRESULT gifrs = m_Image.Save( pStream, Gdiplus::ImageFormatGIF );
	pStream->Seek(ui, STREAM_SEEK_SET, NULL);
	if (gifrs == E_FAIL)
	{
		ASSERT(false);
		return false;
	}

	int width  = m_Image.GetWidth();
	int height = m_Image.GetHeight();

	STATSTG ps;
	gifrs = pStream->Stat(&ps, STATFLAG_DEFAULT);

	//trans or opaque
    if( hasTransGIF )
	{
		pStream = CMapCanvas::MakeGIFTrans( pStream );
	}	

	/*if( hasTransPNG )
	{
		pStream = CMapCanvas::MakePNGTrans( pStream );
	}*/	   

	HRESULT rsr = pStream->Read( bufout, ps.cbSize.LowPart, &read );

	pStream->Stat(&ps, STATFLAG_DEFAULT);

	if( (rsr == STG_E_INVALIDPOINTER) || (rsr == STG_E_ACCESSDENIED) ||
		(rsr == STG_E_CANTSAVE) || (rsr == STG_E_REVERTED) )
	{
		ASSERT(false);
		return false;
	}

	pStream->Release();

	return ps.cbSize.LowPart;
}

int CMapCanvas::ConstructPNG(char* bufout)
{
	ASSERT( m_Image != NULL );
	if(bufout == NULL)
	{
		exit(1);
	}

    LARGE_INTEGER ui = {0, 0};
	ULONG read = 0;
	IStream* pStream;
	CreateStreamOnHGlobal(NULL, TRUE, &pStream);  

	//save to PNG
	HRESULT gifrs = m_Image.Save( pStream, Gdiplus::ImageFormatPNG );
	pStream->Seek(ui, STREAM_SEEK_SET, NULL);
	if (gifrs == E_FAIL)
	{
		ASSERT(false);
		return false;
	}

	int width  = m_Image.GetWidth();
	int height = m_Image.GetHeight();

	STATSTG ps;
	gifrs = pStream->Stat(&ps, STATFLAG_DEFAULT);
	//gifrs = pStream->Stat(&ps, STATFLAG_NONAME);

	if( hasTransPNG )
	{
		pStream = CMapCanvas::MakePNGTrans( pStream );
	}	   

	//HRESULT rsr = pStream->Read( bufout, ps.cbSize.LowPart, &read );
	HRESULT rsr = pStream->Read( bufout, ps.cbSize.LowPart*2, &read );

	pStream->Stat(&ps, STATFLAG_DEFAULT);

	if( (rsr == STG_E_INVALIDPOINTER) || (rsr == STG_E_ACCESSDENIED) ||
		(rsr == STG_E_CANTSAVE) || (rsr == STG_E_REVERTED) )
	{
		ASSERT(false);
		return false;
	}

	pStream->Release();

	return ps.cbSize.LowPart;
}



IStream* CMapCanvas::MakeGIFTrans(IStream* pStream)
{
	IStream* pIStream1 = NULL;
    //pStream to Image
	Gdiplus::Image* image1 = Gdiplus::Image::FromStream(pStream);
	UINT size = image1->GetPaletteSize();
	//get palette
	Gdiplus::ColorPalette* palette = (Gdiplus::ColorPalette*)malloc(size);
	image1->GetPalette(palette, size);
	//palette->Flags = PaletteFlagsHasAlpha;
	//palette->Count = nColors;
	int Alpha = 0x00; 
	//important  set the (0, 0, 0) transparent
	palette->Entries[0] = Gdiplus::Color::MakeARGB( 0x00, 
		0x00, 
		0x00, 
		0x00);
    //get the total number of the new palette
	int nTotalIndex = palette->Count;

	DWORD nColors;
	//nColors = palette->Count;
	Gdiplus::Status stat = Gdiplus::GenericError;
	DWORD   dwWidth  = image1->GetWidth();
	DWORD   dwHeight = image1->GetHeight();
	Gdiplus::Bitmap  bitmap(dwWidth, dwHeight, (3 | ( 8 << 8) | PixelFormatIndexed | PixelFormatGDI)); 
	stat = bitmap.GetLastStatus();
	if (stat != Gdiplus::Ok)
	{
		return NULL;        // No point in continuing.
	}
	palette->Flags = Gdiplus::PaletteFlagsHasAlpha;
	palette->Count = nTotalIndex;

	//set the palette to a new image
	bitmap.SetPalette(palette);

	//bitmap copy from image1
	Gdiplus::Bitmap BmpCopy(dwWidth, dwHeight, (10 | (32 << 8) | PixelFormatAlpha | PixelFormatGDI | PixelFormatCanonical)); 
	//Bitmap BmpCopy(dwWidth, dwHeight, (3 | ( 8 << 8) | PixelFormatIndexed | PixelFormatGDI)); 
	stat = BmpCopy.GetLastStatus();
	if (stat == Gdiplus::Ok)
	{
		Gdiplus::Graphics g(&BmpCopy);
		// Transfer the Image to the Bitmap.
		stat = g.DrawImage(image1, 0, 0, dwWidth, dwHeight);
		// g goes out of scope here and cleans up.
	}
	if (stat != Gdiplus::Ok)
	{
		delete [] (LPBYTE) palette;
		palette = NULL;
		return NULL;
	}

	// Lock the whole of the bitmap for writing.
	Gdiplus::BitmapData  bitmapData;
	Gdiplus::Rect rect(0, 0, dwWidth, dwHeight);

	stat = bitmap.LockBits(
		&rect,
		Gdiplus::ImageLockModeWrite,
		(3 | ( 8 << 8) | PixelFormatIndexed | PixelFormatGDI),
		&bitmapData);


	if (stat == Gdiplus::Ok)
	{
		// Write to the temporary buffer provided by LockBits.
		// Copy the pixels from the source image in this loop.
		// Because you want an index, convert RGB to the appropriate
		// palette index here.
		BYTE *pixels;
		if (bitmapData.Stride > 0)
			pixels = (BYTE*) bitmapData.Scan0;
		else
			// If the Stride is negative, Scan0 points to the last             // scanline in the buffer.
			// To normalize the loop, obtain the start of the buffer,
			// which is located (Height-1) scanlines previous.
			pixels = (BYTE*) bitmapData.Scan0 + bitmapData.Stride*(dwHeight-1);
		UINT stride = abs(bitmapData.Stride);

		// Top-down to set the pixels of image1 into the new transparent gif
		for(UINT row = 0; row < dwHeight; ++row)
		{
			for(UINT col = 0; col < dwWidth; ++col)
			{
				Gdiplus::Color     pixel;
				UINT      i8BppPixel = row * stride + col;

				BmpCopy.GetPixel(col, row, &pixel);

				if( pixel.GetValue() != 0xff000000 )
				{
					int n = 1;
				}
				/*if( (pixel.GetValue() <= 0x333333) && (pixel.GetValue() != 0xff000000) )
				{
					pixel.MakeARGB( 0x00, 
						0x00, 
						0x00, 
						0x00);
				}*/

				int newIndex = 0;
				if( (pixel.GetValue() == 0xff000000) || (pixel.GetValue() == 0x00000000) )
				{
					newIndex = 0;
				}
				else
				{
					for( int i=0; i<nTotalIndex; i++ )
					{
						Gdiplus::Color palPixel = palette->Entries[i];
						if( pixel.GetValue() == palPixel.GetValue() )
						{
							newIndex = i;
							break;
						}
					}
				}

				pixels[i8BppPixel] = newIndex;

			}
		}
		// To commit the changes, unlock the portion of the bitmap.  
		stat = bitmap.UnlockBits(&bitmapData);
	}
	// If destination work was successful, see whether the source was successful.
	if (stat == Gdiplus::Ok) stat = BmpCopy.GetLastStatus();
	// See whether the code has been successful to this point.
	if (stat == Gdiplus::Ok)
	{
		// Write it out to the stream or disk.
		CLSID gifClsid;
		GetEncoderClsid(L"image/gif", &gifClsid);
		LARGE_INTEGER ui = {0, 0};
		CreateStreamOnHGlobal(NULL, TRUE, &pIStream1); 
		stat =  bitmap.Save(pIStream1, &gifClsid, NULL);
		pIStream1->Seek(ui, STREAM_SEEK_SET, NULL);

		//test for file
		//stat =  bitmap.Save(L"E:\\FIUWork\\Integration\\Oct10\\Sep30\\FMap2006\\output\\11.gif", &gifClsid, NULL);
		
		if(stat != Gdiplus::Ok)
		{
			return NULL;
		}

	}
	free(palette);
	delete(image1);

	return pIStream1;
}


//bool CMapCanvas::SetColorTable( CMapStringToPtr* colortable )
bool CMapCanvas::SetColorTable( CMapColorTable* colortable )
{
	this->pColorTable = colortable;
	return true;
}

std::string CMapCanvas::IntDec2StrHex(int dec)
{
	int i = 0;
	std::stack<int> remainder;
	std::string hex, temp;
	char hexDigits[] = { "0123456789ABCDEF" };

	while (dec != 0)
	{
		remainder.push(dec % 16);
		dec /= 16;
		++i;
	}

	while (i != 0)
	{
		if (remainder.top() > 15)
		{
			temp = IntDec2StrHex(remainder.top());
			hex += temp;
		}
		hex.push_back(hexDigits[remainder.top()]);
		remainder.pop();
		--i;
	}
	return hex;
}


CString CMapCanvas::ColorToString(Gdiplus::Color pixel)
{
     /*CString s = c.ToString();
     s = s.Split(new char[]{'[',']'})[1];
     string[] strings = s.Split(new char[]{'=',','});
     if(strings.GetLength(0) > 7)
     {
          s = strings[1] + "," + strings[3] + "," + strings[5] + "," + strings[7];
     }*/

    CString str;
	//str = this->IntDec2StrHex(pixel.GetAlpha()).c_str();
	//str = "0xff";
	str = this->IntDec2StrHex(pixel.GetRed()).c_str();
	str.Append(this->IntDec2StrHex(pixel.GetGreen()).c_str());
	str.Append(this->IntDec2StrHex(pixel.GetBlue()).c_str());
	

    return str;
} 


IStream* CMapCanvas::MakePNGTrans(IStream* pStream)
{

	IStream* pIStream1 = NULL;
    //pStream to Image
	Gdiplus::Image* image1 = Gdiplus::Image::FromStream(pStream);
	//UINT size = image1->GetPaletteSize();
	////get palette
	//Gdiplus::ColorPalette* palette = (Gdiplus::ColorPalette*)malloc(size);
	//image1->GetPalette(palette, size);
	//palette->Flags = PaletteFlagsHasAlpha;
	//palette->Count = nColors;
	int Alpha = 0x80; 
	Gdiplus::ARGB BLACK      = 0xff000000;
	Gdiplus::ARGB Tran_BLACK = 0x00030303;
	Gdiplus::ARGB WHITE      = 0xffffffff;
	Gdiplus::ARGB Tran_WHITE = 0x80ffffff;
	Gdiplus::ARGB BLUE       = 0xff0000ff;
	Gdiplus::ARGB Tran_BLUE  = 0x800000ff;
	Gdiplus::ARGB RED        = 0xffff0000;
	Gdiplus::ARGB Tran_RED   = 0x80ff0000;
	/*Gdiplus::ARGB YELLOW     = 0xffffff00;
	Gdiplus::ARGB Tran_YELLOW  = 0x80ffff00;*/

	Gdiplus::Color pixel;
	Gdiplus::Color alpha_pixel;
	Gdiplus::ARGB  pixelvalue;
	CColorManager colormanager;
	//important  set the (0, 0, 0) transparent
	/*palette->Entries[0] = Gdiplus::Color::MakeARGB( 0x00, 
		0x00, 
		0x00, 
		0x00);*/
    //get the total number of the new palette
	//int nTotalIndex = palette->Count;

	DWORD nColors;
	//nColors = palette->Count;
	Gdiplus::Status stat = Gdiplus::GenericError;
	DWORD   dwWidth  = image1->GetWidth();
	DWORD   dwHeight = image1->GetHeight();
	Gdiplus::Bitmap  bitmap(dwWidth, dwHeight, PixelFormat32bppARGB); 
	stat = bitmap.GetLastStatus();
	if (stat != Gdiplus::Ok)
	{
		return NULL;        // No point in continuing.
	}
	/*palette->Flags = Gdiplus::PaletteFlagsHasAlpha;
	palette->Count = nTotalIndex;*/

	//set the palette to a new image
	//bitmap.SetPalette(palette);

	//bitmap copy from image1
	Gdiplus::Bitmap BmpCopy(dwWidth, dwHeight, PixelFormat32bppARGB); 
	//Bitmap BmpCopy(dwWidth, dwHeight, (3 | ( 8 << 8) | PixelFormatIndexed | PixelFormatGDI)); 
	stat = BmpCopy.GetLastStatus();
	if (stat == Gdiplus::Ok)
	{
		Gdiplus::Graphics g(&BmpCopy);
		// Transfer the Image to the Bitmap.
		stat = g.DrawImage(image1, 0, 0, dwWidth, dwHeight);
		// g goes out of scope here and cleans up.
	}
	if (stat != Gdiplus::Ok)
	{
		/*delete [] (LPBYTE) palette;
		palette = NULL;*/
		return NULL;
	}

	// Lock the whole of the bitmap for writing.
	/*Gdiplus::BitmapData  bitmapData;
	Gdiplus::Rect rect(0, 0, dwWidth, dwHeight);

	stat = bitmap.LockBits(
		&rect,
		Gdiplus::ImageLockModeWrite,
		PixelFormat32bppARGB,
		&bitmapData);*/






	//CImagePixelAccessOptimizer for optimizing CImage
	//CImagePixelAccessOptimizer imagePixelOptimizer_BmpCopy(&BmpCopy);
	//CImagePixelAccessOptimizer imagePixelOptimizer_bitmap(&bitmap);








	if (stat == Gdiplus::Ok)
	{
		// Write to the temporary buffer provided by LockBits.
		// Copy the pixels from the source image in this loop.
		// Because you want an index, convert RGB to the appropriate
		// palette index here.
		//BYTE *pixels;
		//if (bitmapData.Stride > 0)
		//pixels = (BYTE*) bitmapData.Scan0;
		//else
			// If the Stride is negative, Scan0 points to the last             // scanline in the buffer.
			// To normalize the loop, obtain the start of the buffer,
			// which is located (Height-1) scanlines previous.
			//pixels = (BYTE*) bitmapData.Scan0 + bitmapData.Stride*(dwHeight-1);
		//UINT stride = abs(bitmapData.Stride);

		// Top-down to set the pixels of image1 into the new transparent gif
		//for(UINT row = 0; row < dwHeight; ++row)
		for(long row = 0; row < dwHeight; row++)
		{
			for(long col = 0; col < dwWidth; col++)
			{
				//Gdiplus::Color pixel;
				//Gdiplus::Color alpha_pixel;
				//Gdiplus::ARGB  pixelvalue;
				//UINT      i8BppPixel = row * stride + col;

				BmpCopy.GetPixel(col, row, &pixel);
				pixelvalue = pixel.GetValue();
				
				if( (pixelvalue == BLACK) )
				{
					pixel = Tran_BLACK;
					//pixel = Tran_BLUE;
					//pixel = Gdiplus::Color(80, 0, 0, 0);
					//pixel = Gdiplus::Color::Black;
				}
				else if( pixelvalue == WHITE )
				{
					pixel = Tran_WHITE;
					//pixel = Gdiplus::Color(208, 255, 255, 255);
					//pixel = Gdiplus::Color(80, 255, 255, 255);
				}
				else if( pixelvalue == BLUE )
				{
					//pixel = Tran_BLUE;
					//pixel = Gdiplus::Color(128, 0, 0, 255);
					//pixel = Gdiplus::Color(255, 0, 0, 255);
				}
				else if( pixelvalue == RED )
				{
					//pixel = Tran_RED;
					//pixel = Gdiplus::Color(128, 0, 0, 255);
					//pixel = Gdiplus::Color(255, 0, 0, 255);
				}
				//else if( pixelvalue == YELLOW )
				//{
				//	pixel = Tran_YELLOW;
				//	//pixel = Gdiplus::Color(128, 0, 0, 255);
				//	//pixel = Gdiplus::Color(255, 0, 0, 255);
				//}
				///*else if( pixel.GetValue() == 0xffffff00 )
				//{
				//	pixel = pixel.MakeARGB( 0x50, 
				//		pixel.GetRed(), 
				//		pixel.GetGreen(), 
				//		pixel.GetBlue());
				//}*/
				///*else if( (this->pColorTable->Lookup(this->ColorToString(pixelvalue), 
				//		alpha_pixel) == true) )*/
				//else 
				if( (this->pColorTable->Lookup(colormanager.GetPNGColorTableIndex(pixelvalue), 
					alpha_pixel) == true) )
				{
					pixel = alpha_pixel;


					//pixels[row * bitmapData.Stride / 4 + col] = 0xff00ff00;
					/*pixel = Gdiplus::Color(255, 
						alpha_pixel.GetRed(), 
						alpha_pixel.GetGreen(), 
						alpha_pixel.GetBlue());*/
				}
				//not specified to Alpha, using default Alpha 0x80
				/*else if( (pixel.GetAlpha() == 0xff)&&(pixel.GetValue() != 0xff000000) )
				{					
					pixel = pixel.MakeARGB( Alpha, 
						pixel.GetRed(), 
						pixel.GetGreen(), 
						pixel.GetBlue());
				}*/

				//else if((pixel.GetValue() == 0x30ffff00))
				//{
				//	bitmap.SetPixel(col, row, pixel);
				//}

				else
				{
					//continue;
				}

				//pixels[row * bitmapData.Stride / 4 + col] = 0xff00ff00;

				bitmap.SetPixel(col, row, pixel);

				//pixels[row + col] = pixel;
				

				/*pixels[row + col] = 0xff;
				pixels[row + col + 8] = 0xff;
				pixels[row + col + 16] = 0x00;
				pixels[row + col + 24] = 0x00;*/


				/*if( pixel.GetValue() != 0xff000000 )
				{
					int n = 1;
				}*/
				/*if( (pixel.GetValue() <= 0x333333) && (pixel.GetValue() != 0xff000000) )
				{
					pixel.MakeARGB( 0x00, 
						0x00, 
						0x00, 
						0x00);
				}*/

				/*int newIndex = 0;
				if( (pixel.GetValue() == 0xff000000) || (pixel.GetValue() == 0x00000000) )
				{
					newIndex = 0;
				}
				else
				{
					for( int i=0; i<nTotalIndex; i++ )
					{
						Gdiplus::Color palPixel = palette->Entries[i];
						if( pixel.GetValue() == palPixel.GetValue() )
						{
							newIndex = i;
							break;
						}
					}
				}*/

				//pixels[i8BppPixel] = newIndex;

			}
		}




		// To commit the changes, unlock the portion of the bitmap.  
		//stat = bitmap.UnlockBits(&bitmapData);
	}


	/*Gdiplus::Color pixel_end1, pixel_end2;
	pixel_end1 = Gdiplus::Color(73, 69, 78, 68);
	pixel_end2 = Gdiplus::Color(174, 66, 96, 130);

	bitmap.SetPixel(dwWidth-1, dwHeight, pixel_end1);
	bitmap.SetPixel(dwWidth, dwHeight, pixel_end2);*/


	// If destination work was successful, see whether the source was successful.
	if (stat == Gdiplus::Ok) stat = BmpCopy.GetLastStatus();
	// See whether the code has been successful to this point.
	if (stat == Gdiplus::Ok)
	{
		// Write it out to the stream or disk.
		CLSID pngClsid;
		GetEncoderClsid(L"image/png", &pngClsid);
		LARGE_INTEGER ui = {0, 0};
		CreateStreamOnHGlobal(NULL, TRUE, &pIStream1); 
		
		stat =  bitmap.Save(pIStream1, &pngClsid, NULL);
		pIStream1->Seek(ui, STREAM_SEEK_SET, NULL);

		//test for file
		//stat =  bitmap.Save(L"d:\\111.png", &pngClsid, NULL);
		
		if(stat != Gdiplus::Ok)
		{
			return NULL;
		}

	}
	//free(palette);
	delete(image1);

	return pIStream1;
}











//int CMapCanvas::ConstructTransGif(char* bufout)
//{
//	ASSERT( m_Image != NULL );
//	if(bufout == NULL)
//	{
//		exit(1);
//	}
//
//	LARGE_INTEGER ui = {0, 0};
//	ULONG read = 0;
//	IStream* pStream;
//	CreateStreamOnHGlobal(NULL, TRUE, &pStream);  
//
//
//	/*bool bTrans = m_Image.IsTransparencySupported();
//	m_Image.SetTransparentColor( 0 );*/
//
//	//save to GIF
//	HRESULT gifrs = m_Image.Save( pStream, Gdiplus::ImageFormatGIF );
//	pStream->Seek(ui, STREAM_SEEK_SET, NULL);
//	if (gifrs == E_FAIL)
//	{
//		ASSERT(false);
//		return false;
//	}
//
//	int width  = m_Image.GetWidth();
//	int height = m_Image.GetHeight();
//
//	STATSTG ps;
//	gifrs = pStream->Stat(&ps, STATFLAG_DEFAULT);
//
//
//	////////////////////////////////////////////////////////////////
//	//to do: input pStream, output transparent pStream
//	//if(hasTransparentGIF)
//	//{ 
//	//   //IStream* pTransStream;
//	//   output transparet pStream
//	pStream = CMapCanvas::MakeTrans( pStream );
//	//   
//	//}
//
//	////////////////////////////////////////////////////////////////
//
//
//
//	HRESULT rsr = pStream->Read( bufout, ps.cbSize.LowPart, &read );
//
//	pStream->Stat(&ps, STATFLAG_DEFAULT);
//
//	if( (rsr == STG_E_INVALIDPOINTER) || (rsr == STG_E_ACCESSDENIED) ||
//		(rsr == STG_E_CANTSAVE) || (rsr == STG_E_REVERTED) )
//	{
//		ASSERT(false);
//		return false;
//	}
//
//	pStream->Release();
//
//	return ps.cbSize.LowPart;
//}




// save canvas to image
bool CMapCanvas::SaveToFile(CString& sFile)
{
	ASSERT( m_Image != NULL );


	//m_Image.SetTransparentColor(0);

	HRESULT rs = m_Image.Save(sFile);
	if (rs == E_FAIL)
	{
		ASSERT(false);
		LOG("ERROR: could not save the image to file %s", sFile);
		return false;
	}
	
	return true;
}


bool CMapCanvas::SaveToStream()
{
	ASSERT( m_Image != NULL );
	//CImage& img = m_Image;
	// out put to cgi
	ImageTransformer imgTrans(m_Image);

	int width  = m_Image.GetWidth();
	int height = m_Image.GetHeight();
	char head[32] = "P6 ";
	sprintf(head+3, "%d %d 255 ", width, height);
	int length = (int) (strlen(head));

	char* buf = new char[length+width*height*3];
	ASSERT( buf != NULL );
	if(buf == NULL)
	{
		// log error;
		exit(1);
	}

	strcpy(buf, head);
	imgTrans.img2ppm(buf+length);
	CGIReply::replyImage(buf, PPM, length+width*height*3);
	delete [] buf;

	return true;
}

bool CMapCanvas::SaveToGIFStream()
{
	ASSERT( m_Image != NULL );

	int width  = m_Image.GetWidth();
	int height = m_Image.GetHeight();

	char* bufout = new char[width*height*3*2];
	if(bufout == NULL)
	{
		exit(1);
	}

	int size = CMapCanvas::ConstructGif( bufout );

	printf("Content-type: image/gif\n\n");

	int ret = _setmode(_fileno(stdout), _O_BINARY);
	if(ret == -1)
	{
		exit(1);
	}

	fwrite(bufout, size, 1, stdout);

	return true;
}

bool CMapCanvas::SaveToPNGStream()
{
	ASSERT( m_Image != NULL );

	int width  = m_Image.GetWidth();
	int height = m_Image.GetHeight();

	char* bufout = new char[width*height*4*4];

	if(bufout == NULL)
	{
		exit(1);
	}

	memset(bufout, 0, width*height*4*4);

	int size = CMapCanvas::ConstructPNG( bufout );

	printf("Content-type: image/png\n\n");

	int ret = _setmode(_fileno(stdout), _O_BINARY);
	if(ret == -1)
	{
		exit(1);
	}

	fwrite(bufout, size, 1, stdout);

	return true;
}

bool CMapCanvas::SaveToGIFFile( CString& sFile )
{
	ASSERT( m_Image != NULL );

	int width  = m_Image.GetWidth();
	int height = m_Image.GetHeight();

	char* bufout = new char[width*height*3*2];
	if(bufout == NULL)
	{
		exit(1);
	}

	int size = CMapCanvas::ConstructGif( bufout );
    //int size = CMapCanvas::ConstructTransGif( bufout );

	FILE *fpGIF;
	fpGIF = fopen( sFile, "wb" );
	if( fpGIF != NULL )
	{
		fwrite( bufout, size, 1,fpGIF);
	}
	else
	{
		printf("output folder not created");
	}
	delete bufout;

	return true;
}

bool CMapCanvas::SaveToPNGFile( CString& sFile )
{
	ASSERT( m_Image != NULL );

	int width  = m_Image.GetWidth();
	int height = m_Image.GetHeight();

	char* bufout = new char[width*height*4*4];

	if(bufout == NULL)
	{
		exit(1);
	}

	//memset(bufout, 0, width*height*4*4);

	int size = CMapCanvas::ConstructPNG( bufout );
    //int size = CMapCanvas::ConstructTransGif( bufout );

	FILE *fpPNG;
    fpPNG = fopen( sFile, "wb" );
	if( fpPNG != NULL )
	{
		fwrite( bufout, size, 1,fpPNG);
	}
	else
	{
		printf("output folder not created");
	}

	delete bufout;

	return true;
}


bool CMapCanvas::SaveToXMLFile(CString& sXMLFile)  //for debugging
{
	ASSERT(sXMLFile != "");
	//KML standard tags
	string strSPM  = "<Placemark>";
	string strEPM  = "</Placemark>";
    string strSDOC = "<Document>";
	string strEDOC = "</Document>";

	string strSPT = "<Point>";
	string strEPT = "</Point>";
	string strSL  = "<LineString>";
	string strEL  = "</LineString>";
	string strSPN = "<Polygon>";
	string strEPN = "</Polygon>";
	string strSIC = "<Icon>";
	string strEIC = "</Icon>";

	string strSName = "<name>";
	string strEName = "</name>";
	string strSCOR  = "<coordinates>";
	string strECOR  = "</coordinates>";
	string strSHref = "<href>";
	string strEHref = "</href>";
	string strSColor = "<color>";        //might not follow standard KML
	string strEColor = "</color>";
	string strSRot  = "<rotation>";      //might not follow standard KML
	string strERot  = "</rotation>";

	FILE *fpXML;//output XML file
	fpXML = fopen( sXMLFile, "wt+" );

	fprintf( fpXML, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" );
	fprintf( fpXML, "<kml xmlns=\"http://www.terrafly.com/kml/2.0\">\n" );

	fprintf( fpXML, " %s\n", strSDOC.c_str() );
	fprintf( fpXML, "  %s\n", strSPT.c_str() );
	fprintf( fpXML, "    TotalPoints = %d\n", nTotalPoint );
	fprintf( fpXML, "  %s\n", strEPT.c_str() );
	fprintf( fpXML, "  %s\n", strSL.c_str() );
	fprintf( fpXML, "    TotalStreets = %d\n", nTotalArc );
	fprintf( fpXML, "  %s\n", strEL.c_str() );
	fprintf( fpXML, "  %s\n", strSPN.c_str() );
	fprintf( fpXML, "    TotalAreas = %d\n", nTotalArea );
	fprintf( fpXML, "  %s\n", strEPN.c_str() );
	fprintf( fpXML, "  %s\n", strSIC.c_str() );
    fprintf( fpXML, "    TotalIcons = %d\n", nTotalIcon );
	fprintf( fpXML, "  %s\n", strEIC.c_str() );
	fprintf( fpXML, " %s\n", strEDOC.c_str() );

	//there is one point-vector which has all of the points
	//then go through with the vector and retrieve from all points data 
	POSITION pos = m_XMLDataList.GetHeadPosition();
	CXMLData xmlData;
	while( pos != NULL )
	{
		xmlData = m_XMLDataList.GetNext( pos );
		if( POINT_TYPE == xmlData.nXMLDataType )
		{
			fprintf( fpXML, " %s\n", strSPM.c_str() );                                    //placemark
			fprintf( fpXML, "  %s\n", strSName.c_str() );                                  //name
			fprintf( fpXML, "     %s\n", xmlData.strSourceName.c_str() );
			fprintf( fpXML, "  %s\n", strEName.c_str() );
			fprintf( fpXML, "  %s\n", strSPT.c_str() );                                    //point
			fprintf( fpXML, "   %s\n", strSCOR.c_str() );                                   //coordinates
			fprintf( fpXML, "      %f,%f\n", xmlData.dX, xmlData.dY);
			fprintf( fpXML, "   %s\n", strECOR.c_str() );
			fprintf( fpXML, "  %s\n", strEPT.c_str() );
			fprintf( fpXML, " %s\n", strEPM.c_str() );
		}
		else if( ARC_TYPE == xmlData.nXMLDataType )
		{
			//fprintf( fpXML, " %s\n", strSPM.c_str() );                                    //placemark
			//fprintf( fpXML, "  %s\n", strSName.c_str() );                                  //name
			//fprintf( fpXML, "     %s\n", xmlData.strSourceName.c_str() );
			//fprintf( fpXML, "  %s\n", strEName.c_str() );
			//fprintf( fpXML, "  %s\n", strSL.c_str() );                                    //street
			//fprintf( fpXML, "   %s\n", strSCOR.c_str() );                                   //coordinates
			//fprintf( fpXML, "      %f,%f\n", xmlData.dX, xmlData.dY);
			//fprintf( fpXML, "   %s\n", strECOR.c_str() );
			//fprintf( fpXML, "  %s\n", strEL.c_str() );
            //fprintf( fpXML, "  %s\n", strSRot.c_str() );
			//fprintf( fpXML, "     %f\n", xmlData.dRotation );
			//fprintf( fpXML, "  %s\n", strERot.c_str() );
			//fprintf( fpXML, " %s\n", strEPM.c_str() );
			fprintf( fpXML, " %s\n", strSPM.c_str() );                    
			fprintf( fpXML, "  %s\n", strSName.c_str() );                 
			fprintf( fpXML, "     %s\n", xmlData.strSourceName.c_str() );
			fprintf( fpXML, "  %s\n", strEName.c_str() );
			fprintf( fpXML, "  %s\n", strSL.c_str() ); 

			fprintf( fpXML, "   %s\n", strSCOR.c_str() ); 
			POSITION pos = xmlData.m_arcPointList.GetHeadPosition();
			CXMLCoordinates tempXMLCoor;
			while( pos != NULL )
			{
				tempXMLCoor = xmlData.m_arcPointList.GetNext(pos);
				fprintf( fpXML, "      %f, %f\n", 
					tempXMLCoor.longitude, tempXMLCoor.latitude);
			}
			fprintf( fpXML, "   %s\n", strECOR.c_str() );
			fprintf( fpXML, "  %s\n", strEL.c_str() );
			//fprintf( fpXML, "  %s\n", strSRot.c_str() );
			//fprintf( fpXML, "     %f\n", xmlData.dRotation );
			//fprintf( fpXML, "  %s\n", strERot.c_str() );
			fprintf( fpXML, " %s\n", strEPM.c_str() );
		}
		else if( AREA_TYPE == xmlData.nXMLDataType )
		{
			fprintf( fpXML, " %s\n", strSPM.c_str() );                                    //placemark
			fprintf( fpXML, "  %s\n", strSName.c_str() );                                  //name
			fprintf( fpXML, "     %s\n", xmlData.strSourceName.c_str() );
			fprintf( fpXML, "  %s\n", strEName.c_str() );
			fprintf( fpXML, "  %s\n", strSPN.c_str() );                                    //area
			fprintf( fpXML, "   %s\n", strSCOR.c_str() );                                   //coordinates
			fprintf( fpXML, "      %f,%f\n", xmlData.dX, xmlData.dY);
			fprintf( fpXML, "   %s\n", strECOR.c_str() );
			fprintf( fpXML, "  %s\n", strEPN.c_str() );
			fprintf( fpXML, " %s\n", strEPM.c_str() );
		}
		else if( ICON_TYPE == xmlData.nXMLDataType )
		{
			fprintf( fpXML, " %s\n", strSPM.c_str() );                                    //placemark
			fprintf( fpXML, "  %s\n", strSIC.c_str() );                                    //icon
			fprintf( fpXML, "   %s\n", strSHref.c_str() );
            fprintf( fpXML, "      %s\n", xmlData.strSourceName.c_str() );
			fprintf( fpXML, "   %s\n", strEHref.c_str() );
			fprintf( fpXML, "   %s\n", strSCOR.c_str() );                                   //coordinates
			fprintf( fpXML, "      %f,%f\n", xmlData.dX, xmlData.dY);
			fprintf( fpXML, "   %s\n", strECOR.c_str() );
			fprintf( fpXML, "  %s\n", strEIC.c_str() );
			fprintf( fpXML, " %s\n", strEPM.c_str() );
		}
		else
		{
			fprintf( fpXML, " there is mal-type of XML data\n");
		}
	}

	fprintf( fpXML, "</kml>" );
	fclose( fpXML );

	m_XMLDataList.RemoveAll();
	xmlData.m_arcPointList.RemoveAll();


	return true;

}


bool CMapCanvas::SaveToTxtFile(CString& sXMLFile)
{
	ASSERT(sXMLFile != "");
	//KML standard tags
	//string strSPM  = "<Placemark>";
	//string strEPM  = "</Placemark>";
 //   string strSDOC = "<Document>";
	//string strEDOC = "</Document>";

	//string strSPT = "<Point>";
	//string strEPT = "</Point>";
	//string strSL  = "<LineString>";
	//string strEL  = "</LineString>";
	//string strSPN = "<Polygon>";
	//string strEPN = "</Polygon>";
	//string strSIC = "<Icon>";
	//string strEIC = "</Icon>";

	//string strSName = "<name>";
	//string strEName = "</name>";
	//string strSCOR  = "<coordinates>";
	//string strECOR  = "</coordinates>";
	//string strSHref = "<href>";
	//string strEHref = "</href>";
	//string strSColor = "<color>";        //might not follow standard KML
	//string strEColor = "</color>";
	//string strSRot  = "<rotation>";      //might not follow standard KML
	//string strERot  = "</rotation>";

	FILE *fpXML;//output XML file
	fpXML = fopen( sXMLFile, "wt+" );

	/*fprintf( fpXML, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" );
	fprintf( fpXML, "<kml xmlns=\"http://www.terrafly.com/kml/2.0\">\n" );

	fprintf( fpXML, " %s\n", strSDOC.c_str() );
	fprintf( fpXML, "  %s\n", strSPT.c_str() );
	fprintf( fpXML, "    TotalPoints = %d\n", nTotalPoint );
	fprintf( fpXML, "  %s\n", strEPT.c_str() );
	fprintf( fpXML, "  %s\n", strSL.c_str() );
	fprintf( fpXML, "    TotalStreets = %d\n", nTotalArc );
	fprintf( fpXML, "  %s\n", strEL.c_str() );
	fprintf( fpXML, "  %s\n", strSPN.c_str() );
	fprintf( fpXML, "    TotalAreas = %d\n", nTotalArea );
	fprintf( fpXML, "  %s\n", strEPN.c_str() );
	fprintf( fpXML, "  %s\n", strSIC.c_str() );
    fprintf( fpXML, "    TotalIcons = %d\n", nTotalIcon );
	fprintf( fpXML, "  %s\n", strEIC.c_str() );
	fprintf( fpXML, " %s\n", strEDOC.c_str() );*/

	//there is one point-vector which has all of the points
	//then go through with the vector and retrieve from all points data 
	POSITION pos = m_XMLDataList.GetHeadPosition();
	CXMLData xmlData;
	while( pos != NULL )
	{
		xmlData = m_XMLDataList.GetNext( pos );
		//if( POINT_TYPE == xmlData.nXMLDataType )
		//{
		//	fprintf( fpXML, " %s\n", strSPM.c_str() );                                    //placemark
		//	fprintf( fpXML, "  %s\n", strSName.c_str() );                                  //name
		//	fprintf( fpXML, "     %s\n", xmlData.strSourceName.c_str() );
		//	fprintf( fpXML, "  %s\n", strEName.c_str() );
		//	fprintf( fpXML, "  %s\n", strSPT.c_str() );                                    //point
		//	fprintf( fpXML, "   %s\n", strSCOR.c_str() );                                   //coordinates
		//	fprintf( fpXML, "      %f,%f\n", xmlData.dX, xmlData.dY);
		//	fprintf( fpXML, "   %s\n", strECOR.c_str() );
		//	fprintf( fpXML, "  %s\n", strEPT.c_str() );
		//	fprintf( fpXML, " %s\n", strEPM.c_str() );
		//}
		//else 
		if( ARC_TYPE == xmlData.nXMLDataType )
		{

			//fprintf( fpXML, " %s\n", strSPM.c_str() );                    
			//fprintf( fpXML, "	" );                 
			fprintf( fpXML, "%s", xmlData.strSourceName.c_str() );
			fprintf( fpXML, "	" );
			//fprintf( fpXML, "  %s\n", strSL.c_str() ); 

			//fprintf( fpXML, "   %s\n", strSCOR.c_str() ); 
			POSITION pos = xmlData.m_arcPointList.GetHeadPosition();
			CXMLCoordinates tempXMLCoor;
			while( pos != NULL )
			{
				tempXMLCoor = xmlData.m_arcPointList.GetNext(pos);
				fprintf( fpXML, "%f	%f	", 
					tempXMLCoor.longitude, tempXMLCoor.latitude);
			}

		}
		//else if( AREA_TYPE == xmlData.nXMLDataType )
		//{
		//	fprintf( fpXML, " %s\n", strSPM.c_str() );                                    //placemark
		//	fprintf( fpXML, "  %s\n", strSName.c_str() );                                  //name
		//	fprintf( fpXML, "     %s\n", xmlData.strSourceName.c_str() );
		//	fprintf( fpXML, "  %s\n", strEName.c_str() );
		//	fprintf( fpXML, "  %s\n", strSPN.c_str() );                                    //area
		//	fprintf( fpXML, "   %s\n", strSCOR.c_str() );                                   //coordinates
		//	fprintf( fpXML, "      %f,%f\n", xmlData.dX, xmlData.dY);
		//	fprintf( fpXML, "   %s\n", strECOR.c_str() );
		//	fprintf( fpXML, "  %s\n", strEPN.c_str() );
		//	fprintf( fpXML, " %s\n", strEPM.c_str() );
		//}
		//else if( ICON_TYPE == xmlData.nXMLDataType )
		//{
		//	fprintf( fpXML, " %s\n", strSPM.c_str() );                                    //placemark
		//	fprintf( fpXML, "  %s\n", strSIC.c_str() );                                    //icon
		//	fprintf( fpXML, "   %s\n", strSHref.c_str() );
  //          fprintf( fpXML, "      %s\n", xmlData.strSourceName.c_str() );
		//	fprintf( fpXML, "   %s\n", strEHref.c_str() );
		//	fprintf( fpXML, "   %s\n", strSCOR.c_str() );                                   //coordinates
		//	fprintf( fpXML, "      %f,%f\n", xmlData.dX, xmlData.dY);
		//	fprintf( fpXML, "   %s\n", strECOR.c_str() );
		//	fprintf( fpXML, "  %s\n", strEIC.c_str() );
		//	fprintf( fpXML, " %s\n", strEPM.c_str() );
		//}
		else
		{
			fprintf( fpXML, " there is mal-type of txt data\n");
		}
	}

	//fprintf( fpXML, "</kml>" );
	fclose( fpXML );

	m_XMLDataList.RemoveAll();
	xmlData.m_arcPointList.RemoveAll();


	return true;
}




bool CMapCanvas::SaveToXMLStream()
{
	//for standard output stream
	printf("Content-type: text/xml\n\n");
	//KML standard tags
	string strSPM  = "<Placemark>";
	string strEPM  = "</Placemark>";
    string strSDOC = "<Document>";
	string strEDOC = "</Document>";

	string strSPT = "<Point>";
	string strEPT = "</Point>";
	string strSL  = "<LineString>";
	string strEL  = "</LineString>";
	string strSPN = "<Polygon>";
	string strEPN = "</Polygon>";
	string strSIC = "<Icon>";
	string strEIC = "</Icon>";

	string strSName = "<name>";
	string strEName = "</name>";
	string strSCOR  = "<coordinates>";
	string strECOR  = "</coordinates>";
	string strSHref = "<href>";
	string strEHref = "</href>";
	string strSColor = "<color>";        //might not follow standard KML
	string strEColor = "</color>";
	string strSRot  = "<rotation>";      //might not follow standard KML
	string strERot  = "</rotation>";


    fprintf( stdout, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" );
	fprintf( stdout, "<kml xmlns=\"http://www.terrafly.com/kml/2.0\">\n" );

	fprintf( stdout, " %s\n", strSDOC.c_str() );
	fprintf( stdout, "  %s\n", strSPT.c_str() );
	fprintf( stdout, "    TotalPoints = %d\n", nTotalPoint );
	fprintf( stdout, "  %s\n", strEPT.c_str() );
	fprintf( stdout, "  %s\n", strSL.c_str() );
	fprintf( stdout, "    TotalStreets = %d\n", nTotalArc );
	fprintf( stdout, "  %s\n", strEL.c_str() );
	fprintf( stdout, "  %s\n", strSPN.c_str() );
	fprintf( stdout, "    TotalAreas = %d\n", nTotalArea );
	fprintf( stdout, "  %s\n", strEPN.c_str() );
	fprintf( stdout, "  %s\n", strSIC.c_str() );
    fprintf( stdout, "    TotalIcons = %d\n", nTotalIcon );
	fprintf( stdout, "  %s\n", strEIC.c_str() );
	fprintf( stdout, " %s\n", strEDOC.c_str() );

	//there is one point-vector which has all of the points
	//then go through with the vector and retrieve from all points data 
	POSITION pos = m_XMLDataList.GetHeadPosition();
	CXMLData xmlData;
	while( pos != NULL )
	{
		xmlData = m_XMLDataList.GetNext( pos );
		if( POINT_TYPE == xmlData.nXMLDataType )
		{
			fprintf( stdout, " %s\n", strSPM.c_str() );                                    //placemark
			fprintf( stdout, "  %s\n", strSName.c_str() );                                  //name
			fprintf( stdout, "     %s\n", xmlData.strSourceName.c_str() );
			fprintf( stdout, "  %s\n", strEName.c_str() );
			fprintf( stdout, "  %s\n", strSPT.c_str() );                                    //point
			fprintf( stdout, "   %s\n", strSCOR.c_str() );                                   //coordinates
			fprintf( stdout, "      %f,%f\n", xmlData.dX, xmlData.dY);
			fprintf( stdout, "   %s\n", strECOR.c_str() );
			fprintf( stdout, "  %s\n", strEPT.c_str() );
			fprintf( stdout, " %s\n", strEPM.c_str() );
		}
		else if( ARC_TYPE == xmlData.nXMLDataType )
		{
			//fprintf( stdout, " %s\n", strSPM.c_str() );                                    //placemark
			//fprintf( stdout, "  %s\n", strSName.c_str() );                                  //name
			//fprintf( stdout, "     %s\n", xmlData.strSourceName.c_str() );
			//fprintf( stdout, "  %s\n", strEName.c_str() );
			//fprintf( stdout, "  %s\n", strSL.c_str() );                                    //street
			//fprintf( stdout, "   %s\n", strSCOR.c_str() );                                   //coordinates
			//fprintf( stdout, "      %f,%f\n", xmlData.dX, xmlData.dY);
			//fprintf( stdout, "   %s\n", strECOR.c_str() );
			//fprintf( stdout, "  %s\n", strEL.c_str() );
            //fprintf( stdout, "  %s\n", strSRot.c_str() );
			//fprintf( stdout, "     %f\n", xmlData.dRotation );
			//fprintf( stdout, "  %s\n", strERot.c_str() );
			//fprintf( stdout, " %s\n", strEPM.c_str() );
			fprintf( stdout, " %s\n", strSPM.c_str() );                    
			fprintf( stdout, "  %s\n", strSName.c_str() );                 
			fprintf( stdout, "     %s\n", xmlData.strSourceName.c_str() );
			fprintf( stdout, "  %s\n", strEName.c_str() );
			fprintf( stdout, "  %s\n", strSL.c_str() ); 

			fprintf( stdout, "   %s\n", strSCOR.c_str() ); 
			POSITION pos = xmlData.m_arcPointList.GetHeadPosition();
			CXMLCoordinates tempXMLCoor;
			while( pos != NULL )
			{
				tempXMLCoor = xmlData.m_arcPointList.GetNext(pos);
				fprintf( stdout, "      %f, %f\n", 
					tempXMLCoor.longitude, tempXMLCoor.latitude);
			}
			fprintf( stdout, "   %s\n", strECOR.c_str() );
			fprintf( stdout, "  %s\n", strEL.c_str() );
			//fprintf( stdout, "  %s\n", strSRot.c_str() );
			//fprintf( stdout, "     %f\n", xmlData.dRotation );
			//fprintf( stdout, "  %s\n", strERot.c_str() );
			fprintf( stdout, " %s\n", strEPM.c_str() );
		}
		else if( AREA_TYPE == xmlData.nXMLDataType )
		{
			fprintf( stdout, " %s\n", strSPM.c_str() );                                    //placemark
			fprintf( stdout, "  %s\n", strSName.c_str() );                                  //name
			fprintf( stdout, "     %s\n", xmlData.strSourceName.c_str() );
			fprintf( stdout, "  %s\n", strEName.c_str() );
			fprintf( stdout, "  %s\n", strSPN.c_str() );                                    //area
			fprintf( stdout, "   %s\n", strSCOR.c_str() );                                   //coordinates
			fprintf( stdout, "      %f,%f\n", xmlData.dX, xmlData.dY);
			fprintf( stdout, "   %s\n", strECOR.c_str() );
			fprintf( stdout, "  %s\n", strEPN.c_str() );
			fprintf( stdout, " %s\n", strEPM.c_str() );
		}
		else if( ICON_TYPE == xmlData.nXMLDataType )
		{
			fprintf( stdout, " %s\n", strSPM.c_str() );                                    //placemark
			fprintf( stdout, "  %s\n", strSIC.c_str() );                                    //icon
			fprintf( stdout, "   %s\n", strSHref.c_str() );
            fprintf( stdout, "      %s\n", xmlData.strSourceName.c_str() );
			fprintf( stdout, "   %s\n", strEHref.c_str() );
			fprintf( stdout, "   %s\n", strSCOR.c_str() );                                   //coordinates
			fprintf( stdout, "      %f,%f\n", xmlData.dX, xmlData.dY);
			fprintf( stdout, "   %s\n", strECOR.c_str() );
			fprintf( stdout, "  %s\n", strEIC.c_str() );
			fprintf( stdout, " %s\n", strEPM.c_str() );
		}
		else
		{
			fprintf( stdout, " there is mal-type of XML data\n");
		}
	}

	fprintf( stdout, "</kml>" );

	m_XMLDataList.RemoveAll();
	xmlData.m_arcPointList.RemoveAll();

	return true;
}




void CMapCanvas::ReleaseCanvas(void)
{

	
	m_Image.ReleaseDC();
	
	if (m_pDCCanvas != NULL)
	{
		m_pDCCanvas = NULL;	
	}	

}

CDC* CMapCanvas::getCanvasDC(void)
{
	ASSERT( m_pDCCanvas != NULL );
	return m_pDCCanvas;
	
}
