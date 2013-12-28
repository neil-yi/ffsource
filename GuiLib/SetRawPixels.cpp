

#include "stdafx.h"
#include "SetRawPixels.h"

void SetRawPixels( CDC* pDC , int* pSourceBuffer , const CRect* pDestinationRect , const CRect* pSourceRect , int nPixelMapWidth , int nPixelMapHeight )
{
	if( nPixelMapWidth==-1 && nPixelMapHeight==-1 )
	{
		nPixelMapWidth  = *pSourceBuffer++;
		nPixelMapHeight = *pSourceBuffer++;
	}

	BITMAPINFO bmi;
	bmi.bmiHeader.biBitCount      = 32;
	bmi.bmiHeader.biClrImportant  = 0;
	bmi.bmiHeader.biClrUsed       = 0;
	bmi.bmiHeader.biCompression   = BI_RGB;
	bmi.bmiHeader.biHeight        = nPixelMapHeight;
	bmi.bmiHeader.biPlanes        = 1;
	bmi.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biSizeImage     = 0;			//0 is ok for BI_RGB (uncompressed)
	bmi.bmiHeader.biWidth         = nPixelMapWidth;
	bmi.bmiHeader.biXPelsPerMeter = 1;
	bmi.bmiHeader.biYPelsPerMeter = 1;

	StretchDIBits(
		*pDC ,
		pDestinationRect->left     ,	//destination x0
		pDestinationRect->top      ,	//destination y0
		pDestinationRect->Width()  ,	//destination width
		pDestinationRect->Height() ,	//destination height
		pSourceRect->left          ,	//source x0
		pSourceRect->bottom+1      ,	//source y0
		pSourceRect->Width()       ,	//source width
		-pSourceRect->Height()     ,	//source height
		pSourceBuffer , &bmi , DIB_RGB_COLORS , SRCCOPY );
}

void SetRawPixels( CDC* pDC , int* pSourceBuffer , const CRect* pDestinationRect , int nSourceWidth , int nSourceHeight )
{
	if( nSourceWidth==-1 && nSourceHeight==-1 )
	{
		nSourceWidth  = *pSourceBuffer++;
		nSourceHeight = *pSourceBuffer++;
	}

	BITMAPINFO bmi;
	bmi.bmiHeader.biBitCount      = 32;
	bmi.bmiHeader.biClrImportant  = 0;
	bmi.bmiHeader.biClrUsed       = 0;
	bmi.bmiHeader.biCompression   = BI_RGB;
	bmi.bmiHeader.biHeight        = nSourceHeight;
	bmi.bmiHeader.biPlanes        = 1;
	bmi.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biSizeImage     = 0;			//0 is ok for BI_RGB (uncompressed)
	bmi.bmiHeader.biWidth         = nSourceWidth;
	bmi.bmiHeader.biXPelsPerMeter = 1;
	bmi.bmiHeader.biYPelsPerMeter = 1;

	int dx = 0;
	int dy = 0;
	int sw = nSourceWidth;
	int sh = nSourceHeight;
	int dw = nSourceWidth;
	int dh = nSourceHeight;

	if( pDestinationRect != NULL )
	{
		dw = pDestinationRect->Width();
		dh = pDestinationRect->Height();
		dx = pDestinationRect->left;
		dy = pDestinationRect->top;
	}

	StretchDIBits(
		*pDC ,
		dx ,	//destination x0
		dy ,	//destination y0
		dw ,	//destination width
		dh ,	//destination height
		0 ,		//source x0
		sh+1 ,	//source y0
		sw ,	//source width
		-sh ,	//source height
		pSourceBuffer , &bmi , DIB_RGB_COLORS , SRCCOPY );
}


void SetRawPixels( CDC* pDC , int* pSourceBuffer , int nSourceWidth , int nSourceHeight , bool bFirstPixelAtTop )
{
	BITMAPINFO bmi;
	bmi.bmiHeader.biBitCount      = 32;
	bmi.bmiHeader.biClrImportant  = 0;
	bmi.bmiHeader.biClrUsed       = 0;
	bmi.bmiHeader.biCompression   = BI_RGB;
	bmi.bmiHeader.biHeight        = nSourceHeight;
	bmi.bmiHeader.biPlanes        = 1;
	bmi.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biSizeImage     = 0;			//0 is ok for BI_RGB (uncompressed)
	bmi.bmiHeader.biWidth         = nSourceWidth;
	bmi.bmiHeader.biXPelsPerMeter = 1;
	bmi.bmiHeader.biYPelsPerMeter = 1;

	if( bFirstPixelAtTop )
	{
		StretchDIBits(
			*pDC ,
			0 ,				//destination x0
			0 ,				//destination y0
			nSourceWidth ,	//destination width
			nSourceHeight ,	//destination height
			0 ,				//source x0
			nSourceHeight+1 ,	//source y0
			nSourceWidth ,		//source width
			-nSourceHeight ,	//source height
			pSourceBuffer , &bmi , DIB_RGB_COLORS , SRCCOPY );
	}
	else
	{
		StretchDIBits(
			*pDC ,
			0 ,					//destination x0
			0 ,					//destination y0
			nSourceWidth ,		//destination width
			nSourceHeight ,		//destination height
			0 ,					//source x0
			0 ,					//source y0
			nSourceWidth ,		//source width
			nSourceHeight ,		//source height
			pSourceBuffer , &bmi , DIB_RGB_COLORS , SRCCOPY );
	}

}