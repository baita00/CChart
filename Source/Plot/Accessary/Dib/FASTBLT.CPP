#include <windows.h>
#include "FastBlt.h"
#include "palette.h"
#include <tchar.h>
#include <stdio.h>

BOOL bFastBltEnabled = TRUE;

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :BitsInDWORD(DWORD dw)                                         *
 *                                                                            *
 *  PURPOSE    :Count the number of set bits in a given DWORD                 * 
 *                                                                            *
 *  *NOTE:     :Used only in CreateBIPalette() for extracting RGB values      *
 *              from 16 and 32bpp DIBS                                        *
 *                                                                            *
 *****************************************************************************/
BYTE BitsInDWORD(DWORD dw)
{
  int i, Count = 0;

  for (i=0; i<sizeof(DWORD) * 8; i++)
    Count += (((1 << i) & dw) != 0);

  return Count;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :RightBit(DWORD dw)                                            *
 *                                                                            *
 *  PURPOSE    :Returns the position of the rightmost set bit in a DWORD      * 
 *                                                                            *
 *  *NOTE:     :Used only in CreateBIPalette() for extracting RGB values      *
 *              from 16 and 32bpp DIBS                                        *
 *                                                                            *
 *****************************************************************************/
BYTE RightBit(DWORD dw)
{
  int i;

  for (i=0; i<sizeof(DWORD) * 8; i++)
    if (((1 << i) & dw) != 0)
      return i;

  return (BYTE)-1; // No bits are set
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :LeftBit(DWORD dw)                                             *
 *                                                                            *
 *  PURPOSE    :Returns the position of the leftmost set bit in a DWORD       * 
 *                                                                            *
 *  *NOTE:     :Used only in CreateBIPalette() for extracting RGB values      *
 *              from 16 and 32bpp DIBS                                        *
 *                                                                            *
 *****************************************************************************/
BYTE LeftBit(DWORD dw)
{
  int i;

  for (i=(sizeof(DWORD) * 8)-1; i >= 0; i--)
    if (((1 << i) & dw) != 0)
      return i;

  return (BYTE)-1; // No bits are set
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :IsValidMask(DWORD dw)                                         *
 *                                                                            *
 *  PURPOSE    :Determines if a given DWORD is valid as a color mask for      * 
 *              16 or 32bpp DIBS (are all the set bits contiguous)            *
 *                                                                            *
 *  *NOTE:     :Used only in CreateBIPalette()                                *
 *                                                                            *
 *****************************************************************************/
BOOL IsValidMask(DWORD dw)
{
  int iStart, iStop, i;
  
  iStart = LeftBit(dw);
  iStop  = RightBit(dw);

  if (iStart == iStop)
    return TRUE;

  for (i=iStart; i>=iStop; i--) 
     if (((1 << i) & dw) == 0)
       return FALSE;

  return TRUE;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :CreateRGBToPalMap(HPALETTE, LPBYTE)                           *
 *                                                                            *
 *  PURPOSE    :Create a table that maps 15 bit RGB values to palette indices * 
 *              by blting from a 16 bpp dibsection into an 8 bpp dibsection.  *
 *                                                                            *
 *****************************************************************************/
HBITMAP CreateRGBToPalMap(HPALETTE hPal, LPBYTE *pColorToPaletteIndex)
{
   int i;
   LPWORD lpw;
   RGBQUAD rgbq[256];
   HBITMAP hbmSrc, hbmDst = NULL;
   LPDWORD lpMasks;
   LPBITMAPINFOHEADER lpbih;
   BITMAPINFOHEADER bih;
   HDC hdcSrc = CreateCompatibleDC(NULL);
   HDC hdcDst = CreateCompatibleDC(NULL);
      
   // Allocate room for our bitmap header
   lpbih = (LPBITMAPINFOHEADER)GlobalAlloc(GPTR, sizeof(BITMAPINFOHEADER) + sizeof(DWORD)*3);
   
   // Initialize the fields for a 16 bpp DIB section
   lpbih->biSize = sizeof(BITMAPINFOHEADER);
   lpbih->biHeight = 128;
   lpbih->biWidth  = 256;
   lpbih->biPlanes = 1;
   lpbih->biBitCount = 16;
   lpbih->biCompression = BI_BITFIELDS;
   lpbih->biSizeImage = 256L * 128L  * 2L;  
   
   bih = *lpbih;
   
   // Get a dword pointer to the masks
   lpMasks = (LPDWORD)((LPSTR)lpbih + (WORD)lpbih->biSize);

   // Initialize the masks to RGB 5,5,5
   lpMasks[0] = MAKE555WORD(0xff, 0, 0);
   lpMasks[1] = MAKE555WORD(0, 0xff, 0);
   lpMasks[2] = MAKE555WORD(0, 0, 0xff);
      
   hbmSrc = CreateDIBSection(hdcSrc, (LPBITMAPINFO)lpbih, DIB_RGB_COLORS,  (void **)&lpw, NULL, 0L);

   // Initialize the bits of the DIB section to cover all 15 bit values
   for (i=0; i<128*256; i++) 
      lpw[i] = i;

   // Prepare the target 8 bpp DIB section
   bih.biBitCount = 8;
   bih.biCompression = BI_RGB;
   bih.biSizeImage = 256L * 128L;

   hbmDst = CreateDIBSection(hdcDst, (LPBITMAPINFO)&bih, DIB_RGB_COLORS, (void **)pColorToPaletteIndex, NULL, 0L);
   GlobalFree(lpbih);
   
   // Prepare to blt by selecting both DIB sections into memory DCs
   SelectObject(hdcSrc, hbmSrc);
   SelectObject(hdcDst, hbmDst);
   
   // Initialize the color table of the target 8 bpp DIB section using the specified palette
   ZeroMemory(rgbq, sizeof(rgbq));
   CreateRGBQUADFromPalette(rgbq, hPal);
   SetDIBColorTable(hdcDst, 0, 256, rgbq);

   // Get GDI to build the index translate table
   BitBlt(hdcDst, 0, 0, 256, 128, hdcSrc, 0, 0, SRCCOPY);
   
   // Clean up the mess we made
   DeleteDC(hdcSrc);
   DeleteDC(hdcDst);
   DeleteObject(hbmSrc);

   // return an HBITMAP so that we can delete object on it when we're done
   return hbmDst;
}  

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :FreeRGBToPalMap(HBITMAP hbm)                                  *
 *                                                                            *
 *  PURPOSE    :This function just wraps DeleteObject() so that we can call   * 
 *              on it to delete the HBITMAP returned by CreateRGBToPalMap.    *
 *                                                                            *
 *****************************************************************************/
void FreeRGBToPalMap(HBITMAP hbm)
{
   if (hbm)
     DeleteObject(hbm);
}       

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :Create8BPPDIBSec(HDC, int, int, int, int, HDC);               *
 *                                                                            *
 *  PURPOSE    :Converts a section of a memory bitmap with more than 8 bpp    *
 *              into a DIB section with 8 bpp                                 *
 *                                                                            *
 *****************************************************************************/
HBITMAP Create8BPPDIBSec(HDC hdcSrc, int nXOrigin, int nYOrigin, int nWidth, int nHeight, HDC hdcDest)
{
  DIBSECTION ds;
  HBITMAP hbmTemp, hbmSrc, hbmPalMap, hbmNew;
  HDC hdcNew;
  LPBYTE lpbPalMap;
  HPALETTE hPal;
  BITMAPINFO bi;
  LPBYTE lpb;
  RGBQUAD   rgbq[256];
  WORD      dwX, dwY;
  RGBTRIPLE *rgbt;
  DWORD     dwRGBAdd;
  DWORD     dwByteAdd;
  DWORD     dwWidth;
  LPDWORD   lpBits32; 
  LPWORD    lpBits16; 
  BYTE      bLRed, bLGreen, bLBlue; 
  BYTE      bRRed, bRGreen, bRBlue; 
  DWORD     dwRedMask, dwGreenMask, dwBlueMask;
  int i;

  SetLastError(0);
  
  hbmTemp = CreateCompatibleBitmap(hdcDest,1,1);
  
  // Lets find out what kind of bitmap we have in our source
  hbmSrc  = (HBITMAP)SelectObject(hdcSrc, hbmTemp);
  if (!hbmSrc) {  // If this hdcSrc is a device DC then bail
    DeleteObject(hbmTemp);  
    return NULL;
  }
  SelectObject(hdcSrc, hbmSrc);
  DeleteObject(hbmTemp);
    
  GetObject(hbmSrc, sizeof(DIBSECTION), &ds);

  if ((GetDeviceCaps(hdcDest, BITSPIXEL) != ds.dsBmih.biBitCount) && (ds.dsBmih.biBitCount > 8)) {
    // Get our destination palette
    hPal = (HPALETTE)SelectPalette(hdcDest, (HPALETTE)GetStockObject(DEFAULT_PALETTE), FALSE);
    SelectPalette(hdcDest, hPal, FALSE);
	//RealizePalette(hdcDest);

    // Create our palette mapping table
    hbmPalMap = CreateRGBToPalMap(hPal, &lpbPalMap);

    // Set up our dib sec info
    bi.bmiHeader = ds.dsBmih;
	bi.bmiHeader.biWidth = nWidth;
	bi.bmiHeader.biHeight = nHeight;
    bi.bmiHeader.biBitCount = 8;
    bi.bmiHeader.biCompression = BI_RGB;    
    bi.bmiHeader.biSizeImage = ((DWORD)BYTESPERLINE(nWidth, 8) * nHeight); 
    bi.bmiHeader.biClrUsed = 1;
	dwByteAdd = (DWORD)BYTESPERLINE(nWidth, 8);
          
    // Create and select the dib section
    hdcNew = CreateCompatibleDC(NULL);
	hbmNew = CreateDIBSection(hdcNew, &bi, DIB_RGB_COLORS, (void **)&lpb, NULL, 0);
    SelectObject(hdcNew, hbmNew);

    ZeroMemory(rgbq, sizeof(rgbq));
    GetPaletteEntries(hPal, 0, 256, (LPPALETTEENTRY)rgbq);
    for (i=0; i<256; i++) 
      SWAP(rgbq[i].rgbBlue, rgbq[i].rgbRed);
    SetDIBColorTable(hdcNew, 0, 256, rgbq);

    switch (ds.dsBmih.biBitCount) {
      case 24:  // Traverse a 24 bpp DIB section
          rgbt = (RGBTRIPLE *)ds.dsBm.bmBits;
          dwRGBAdd = (DWORD)BYTESPERLINE(ds.dsBmih.biWidth, 24);
          
          // Get starting position in source
          rgbt = (RGBTRIPLE*)((LPBYTE)rgbt + (dwRGBAdd * (ds.dsBmih.biHeight - (nYOrigin+1))));
    	  lpb  = (LPBYTE)((LPBYTE)lpb + (dwByteAdd * (nHeight-1)));

    	  // Walk the bits 
    	  for (dwY = 0; dwY < nHeight; dwY++) {
            for (i = 0, dwX = nXOrigin; i < nWidth; dwX++, i++)  
               lpb[i] = lpbPalMap[MAKE555WORD(rgbt[dwX].rgbtRed, rgbt[dwX].rgbtGreen, rgbt[dwX].rgbtBlue)];
            
            lpb  = (LPBYTE)((LPBYTE)lpb - dwByteAdd);
            rgbt = (RGBTRIPLE*)((LPSTR)rgbt - dwRGBAdd);
          }
        break;

      case 16:  // Traverse a 16 bpp DIB section
      case 32:  // Traverse a 32 bpp DIB section
        { 
    	  lpBits32 = (LPDWORD)ds.dsBm.bmBits;
          lpBits16 = (LPWORD)ds.dsBm.bmBits;
          
          // If not using BI_BITFIELDS then set masks to default values
          if (ds.dsBmih.biCompression == BI_RGB) {
            if (ds.dsBmih.biBitCount == 16) {
              dwRedMask   = MAKE555WORD(0xFF, 0, 0);   // Make 5, 5, 5 mask
              dwGreenMask = MAKE555WORD(0, 0xFF, 0);
              dwBlueMask  = MAKE555WORD(0, 0, 0xFF);
            } else {
              dwRedMask   = 0xFF0000;                  // Make 8, 8, 8 mask
              dwGreenMask = 0x00FF00;
              dwBlueMask  = 0x0000FF;
            } 
          } else {
            dwRedMask   = ds.dsBitfields[0];           // Use the specified mask
            dwGreenMask = ds.dsBitfields[1];
            dwBlueMask  = ds.dsBitfields[2];
          }
                                                                                           
          dwWidth = ds.dsBmih.biWidth;
    	  if (ds.dsBmih.biBitCount == 16)  // DWORD align for 16bpp
            dwWidth += dwWidth & 1;

    	  // While more that 8 bits per mask, turn off the righmost bit 
          while (BitsInDWORD(dwRedMask) > 8) dwRedMask -= (1 << RightBit(dwRedMask));
          while (BitsInDWORD(dwGreenMask) > 8) dwGreenMask -= (1 << RightBit(dwGreenMask));
          while (BitsInDWORD(dwBlueMask) > 8) dwBlueMask -= (1 << RightBit(dwBlueMask));

          // How much will we need to shift right in order to "right justify" the bits
          bRRed   = RightBit(dwRedMask);
          bRGreen = RightBit(dwGreenMask);
          bRBlue  = RightBit(dwBlueMask);

          // How much will we need to shift left in order to pad to 8 bits per mask
          bLRed   = 8 - BitsInDWORD(dwRedMask);  
          bLGreen = 8 - BitsInDWORD(dwGreenMask);
          bLBlue  = 8 - BitsInDWORD(dwBlueMask); 

    	  lpb  = (LPBYTE)((LPBYTE)lpb + (dwByteAdd * (nHeight-1)));

    	  // Walk the bits and add the extracted RGB values to our color histogram
    	  if (ds.dsBmih.biBitCount == 16) {
              dwRGBAdd = (DWORD)BYTESPERLINE(ds.dsBmih.biWidth, 16);

              // Get starting position in source
              lpBits16 = (LPWORD)((LPBYTE)lpBits16 + (dwRGBAdd * (ds.dsBmih.biHeight - (nYOrigin+1))));
        	  
        	  // Walk the bits 
        	  for (dwY = 0; dwY < nHeight; dwY++) {
                for (i = 0, dwX = nXOrigin; i < nWidth; dwX++, i++)  
                   lpb[i] = lpbPalMap[MAKE555WORD((BYTE)((lpBits16[dwX] & dwRedMask)   >> bRRed)   << bLRed, 
                                                  (BYTE)((lpBits16[dwX] & dwGreenMask) >> bRGreen) << bLGreen,
                                                  (BYTE)((lpBits16[dwX] & dwBlueMask)  >> bRBlue)  << bLBlue)];
                
                lpb  = (LPBYTE)((LPBYTE)lpb - dwByteAdd);
                lpBits16 = (LPWORD)((LPSTR)lpBits16 - dwRGBAdd);
            }
          } else {
              dwRGBAdd = (DWORD)BYTESPERLINE(ds.dsBmih.biWidth, 32);

              // Get starting position in source
              lpBits32 = (LPDWORD)((LPBYTE)lpBits32 + (dwRGBAdd * (ds.dsBmih.biHeight - (nYOrigin+1))));
        	  
        	  // Walk the bits 
        	  for (dwY = 0; dwY < nHeight; dwY++) {
                for (i = 0, dwX = nXOrigin; i < nWidth; dwX++, i++)  
                   lpb[i] = lpbPalMap[MAKE555WORD((BYTE)((lpBits32[dwX] & dwRedMask)   >> bRRed)   << bLRed, 
                                                  (BYTE)((lpBits32[dwX] & dwGreenMask) >> bRGreen) << bLGreen,
                                                  (BYTE)((lpBits32[dwX] & dwBlueMask)  >> bRBlue)  << bLBlue)];
                
                lpb  = (LPBYTE)((LPBYTE)lpb - dwByteAdd);
                lpBits32 = (LPDWORD)((LPSTR)lpBits32 - dwRGBAdd);
            }
          }
        };               
      };

    // Clean up!
    FreeRGBToPalMap(hbmPalMap);

	DeleteObject(hdcNew);
  }	else
    return NULL;

  return hbmNew;
}       


/******************************************************************************
 *                                                                            *
 *  FUNCTION   :DIBSecBitBlt(HDC, int, int, int, int, HDC, int, int, DWORD)   *
 *                                                                            *
 *  PURPOSE    :Replaces BitBlt for when the target device is 8 bpp and the   *
 *              source has more than 8 bpp.                                   *
 *                                                                            *
 *****************************************************************************/
BOOL DIBSecBitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, 
                  HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop)
{
    HBITMAP hbm = NULL;
    HDC hdc;
	BOOL bResult;
       
    if ((nWidth * nHeight > 128000L) && bFastBltEnabled)
      hbm = Create8BPPDIBSec(hdcSrc, nXSrc, nYSrc, nWidth, nHeight, hdcDest);

	if (hbm) {
	  hdc = CreateCompatibleDC(hdcDest);
      SelectObject(hdc, hbm);
	  
	  bResult = BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, hdc, 0, 0, dwRop);
	  
	  DeleteDC(hdc);
	  DeleteObject(hbm);

	  return bResult;
	} else
	  return BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, dwRop);
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :DIBSecStretchBlt(HDC, int, int, int, int,                     *
 *                               HDC, int, int, int, int, DWORD)              *
 *                                                                            *
 *  PURPOSE    :Replaces StretchBlt for when the target device is 8 bpp and   *
 *              the source has more than 8 bpp.                               *
 *                                                                            *
 *****************************************************************************/
BOOL DIBSecStretchBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, 
                      HDC hdcSrc,  int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, DWORD dwRop)
{
    HBITMAP hbm = NULL;
    HDC hdc;
	BOOL bResult;

    {
      TCHAR szBuffer[128];

#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_stprintf_s(szBuffer, 128, _TEXT("X %d   Y %d     Width %d   Height %d\n"), nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc);
#else
		_stprintf(szBuffer, _TEXT("X %d   Y %d     Width %d   Height %d\n"), nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc);
#endif
      
      OutputDebugString(szBuffer);
    }

    if ((nWidthSrc * nHeightSrc > 128000L) && bFastBltEnabled)
      hbm = Create8BPPDIBSec(hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, hdcDest);

	if (hbm) {
	  hdc = CreateCompatibleDC(hdcDest);
      SelectObject(hdc, hbm);
	  
	  bResult = StretchBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, 
	                       hdc, 0, 0, nWidthSrc, nHeightSrc, dwRop);
	  
	  DeleteDC(hdc);
	  DeleteObject(hbm);

	  return bResult;
	} else
	  return StretchBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, 
                        hdcSrc,  nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, dwRop);
}
