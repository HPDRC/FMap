#pragma once

#ifndef _CIMAGE_PIXEL_ACCESS_OPTIMIZER_H_
#define _CIMAGE_PIXEL_ACCESS_OPTIMIZER_H_

#include <afx.h>
#include <atlimage.h>

//#define ENABLE_GET_SET_PIXEL_VERIFICATION
#ifdef ENABLE_GET_SET_PIXEL_VERIFICATION
#	define FOR_GET_SET_PIXEL_ASSERT( what ) what
#	define GET_SET_PIXEL_ASSERT ASSERT
#else
#	define FOR_GET_SET_PIXEL_ASSERT( what )
#	define GET_SET_PIXEL_ASSERT( whate )
#endif

class CImagePixelAccessOptimizer
{
public:
	CImagePixelAccessOptimizer( CImage* _image );
	CImagePixelAccessOptimizer( const CImage* _image );
	~CImagePixelAccessOptimizer();

	COLORREF GetPixel( int _x, int _y ) const;
	void SetPixel( int _x, int _y, const COLORREF _color );

protected:
	bool PositionOK( int _x, int _y ) const;

private:
	void Init();

	CImage* m_image;
	const bool m_const;
	
	int m_width;
	int m_height;
	int m_bitCnt;
	int m_rowBytes;

	RGBQUAD* m_colors;
	BYTE* m_bits;
};

inline CImagePixelAccessOptimizer::CImagePixelAccessOptimizer( CImage* _image )
: m_image( _image )
, m_const( false )
{
	Init();
}

inline CImagePixelAccessOptimizer::CImagePixelAccessOptimizer( const CImage* _image )
: m_image( const_cast< CImage* >( _image ) )
, m_const( true )
{	
	Init();
}

inline void CImagePixelAccessOptimizer::Init()
{
	ASSERT( m_image );
	m_colors = NULL;
	m_bits = NULL;

	m_width = m_image->GetWidth();
	m_height = m_image->GetHeight();

	m_bitCnt = m_image->GetBPP();
	m_rowBytes = ( ( m_width * m_bitCnt + 31) & (~31) ) / 8;
	if( m_image->GetPitch() < 0 )
	{
		m_rowBytes = -m_rowBytes;
	}

	switch( m_bitCnt )
	{
	case 1:
	case 4:
	case 8:
		{
			const int paletteSize = 1 << m_bitCnt;
			m_colors = new RGBQUAD[ paletteSize ];
			m_image->GetColorTable( 0, paletteSize, m_colors );
		}
		break;
	default:
		break;
	}

	m_bits = reinterpret_cast< BYTE* >( m_image->GetBits() );

	//this is for DC
	//m_image->GetDC();
}

inline CImagePixelAccessOptimizer::~CImagePixelAccessOptimizer()
{
	delete m_colors;

	//this is for DC
	//m_image->ReleaseDC();
}

inline COLORREF CImagePixelAccessOptimizer::GetPixel( int _x, int _y ) const
{
	ASSERT( PositionOK( _x, _y ) );

	FOR_GET_SET_PIXEL_ASSERT( const COLORREF color = m_image->GetPixel( _x, _y ) );

	const RGBQUAD* rgbResult = NULL;
	RGBQUAD tempRgbResult;
	switch( m_bitCnt )
	{
	case 1:		//Monochrome
		rgbResult = &m_colors[ *(m_bits + m_rowBytes*_y + _x/8) & (0x80 >> _x%8) ];
		break;
	case 4:
		rgbResult = &m_colors[ *(m_bits + m_rowBytes*_y + _x/2) & ((_x&1) ? 0x0f : 0xf0) ];
		break;
	case 8:
		rgbResult = &m_colors[ *(m_bits + m_rowBytes*_y + _x) ];
		break;
	case 16:
		{
			WORD dummy = *(LPWORD)(m_bits + m_rowBytes*_y + _x*2);

			tempRgbResult.rgbBlue = (BYTE)(0x001F & dummy);
			tempRgbResult.rgbGreen = (BYTE)(0x001F & (dummy >> 5));
			tempRgbResult.rgbRed = (BYTE)(0x001F & dummy >> 10 );
			rgbResult = &tempRgbResult;
		}
		break;
	case 24:
		rgbResult = (LPRGBQUAD)(m_bits + m_rowBytes*_y + _x*3);
		break;
	case 32:
		rgbResult = (LPRGBQUAD)(m_bits + m_rowBytes*_y + _x*4);
		break;
	default:
		//error
		ASSERT( false );
		break;
	}

	const COLORREF rgbResultColorRef = RGB( rgbResult->rgbRed, rgbResult->rgbGreen, rgbResult->rgbBlue );
	GET_SET_PIXEL_ASSERT( rgbResultColorRef == color );

	return rgbResultColorRef;
}

inline void CImagePixelAccessOptimizer::SetPixel( int _x, int _y, const COLORREF _color )
{
	ASSERT( !m_const );
	ASSERT( PositionOK( _x, _y ) );

	switch( m_bitCnt )
	{
	case 1:		
	case 4:
	case 8:
		{
			m_image->SetPixel( _x, _y, _color );
		}
		break;
	case 16:
		{
			WORD wDummy = 0;
			wDummy = GetRValue( _color );
			wDummy = wDummy << 5;
			wDummy |= GetGValue( _color );
			wDummy = wDummy << 5;
			wDummy |= GetBValue( _color );

			*(LPWORD)(m_bits + m_rowBytes*_y + _x*2) = wDummy;
		}
		break;
	case 24:
		{
			RGBQUAD* rgbValue = reinterpret_cast< RGBQUAD* >(m_bits + m_rowBytes*_y + _x*3);
			rgbValue->rgbRed = GetRValue( _color );
			rgbValue->rgbGreen = GetGValue( _color );
			rgbValue->rgbBlue = GetBValue( _color );
		}
		break;
	case 32:
		{
			RGBQUAD* rgbValue = reinterpret_cast< RGBQUAD* >(m_bits + m_rowBytes*_y + _x*4);
			rgbValue->rgbRed = GetRValue( _color );
			rgbValue->rgbGreen = GetGValue( _color );
			rgbValue->rgbBlue = GetBValue( _color );
		}
		break;
	default:
		ASSERT( false );
	}

	GET_SET_PIXEL_ASSERT( m_image->GetPixel( _x, _y ) == _color );
}

inline bool CImagePixelAccessOptimizer::PositionOK( int _x, int _y ) const
{
	return ( _x >= 0) && ( _x < m_width ) && ( _y >= 0) && ( _y < m_height );
}

#endif
