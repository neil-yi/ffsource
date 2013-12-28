#ifndef EIVA_SET_RAW_PIXELS_H
#define EIVA_SET_RAW_PIXELS_H

//------------------------------------------------------------------------
//
// this uses standart StretchDIBits() to copy a memory
// pixel buffer to a dc, you should use this because all
// the flippnig and the stretching and the minus one issue
// is handled here..
//
// The pSourceBuffer is a buffer of RGB pixels (the image, or images)
//
// note1: if source w and h parsed to these functions are both -1, the
//       image dimensions will be extracted from the two first integers
//       in the source buffer.
//
//note2: if the destination rect is NULL, the image will be drawn 1:1
//
//------------------------------------------------------------------------

void SetRawPixels( CDC* pDC , int* pSourceBuffer , const CRect* pDestinationRect=NULL , int nSourceWidth=-1 , int nSourceHeight=-1 );
void SetRawPixels( CDC* pDC , int* pSourceBuffer , const CRect* pDestinationRect , const CRect* pSourceRect , int nPixelMapWidth=-1 , int nPixelMapHeight=-1 );
void SetRawPixels( CDC* pDC , int* pSourceBuffer , int nSourceWidth , int nSourceHeight , bool bFirstPixelAtTop );	//move viewportorg if you want to move the image in the target dc

#endif