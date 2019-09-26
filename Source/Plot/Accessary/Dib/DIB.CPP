/*******************************************************************************
 *                                                                             *
 *  MODULE      : DIB.C                                                        *
 *                                                                             *
 *  DESCRIPTION : Routines for dealing with both Device Independent Bitmaps    *
 *                and Device Dependant Bitmaps                                 *
 *                                                                             *
 *  FUNCTIONS:                                                                 * 
 *                                                                             *
 *    OpenDIB()           - Opens DIB file and creates a memory DIB            *
 *                                                                             *
 *    WriteDIB()          - Writes a global handle in CF_DIB format to a file. *                           
 *                                                                             *
 *    DIBInfo()           - Retrieves the info. block associated with a CF_DIB *
 *                          format memory block.                               *
 *                                                                             *
 *    CreateBIPalette()   - Creates a GDI palette given a pointer to a         *
 *                          BITMAPINFO structure.                              *
 *                                                                             *
 *    CreateDIBPalette()  - Creates a GDI palette given a HANDLE to a          *
 *                          BITMAPINFO structure.                              *
 *                                                                             *
 *    ReadDIBBitmapInfo() - Reads a file in DIB format and returns a global    *
 *                          handle to it's BITMAPINFO                          *
 *                                                                             *
 *    ColorTableSize()    - Calculates the size in bytes of the color table    *
 *                          for the given DIB                                  *
 *                                                                             *
 *    DIBNumColors()      - Determines the number of colors in DIB.            *
 *                                                                             *
 *    BitmapFromDIB()     - Creates a DDB given a global handle to a block in  *
 *                          CF_DIB format.                                     *
 *                                                                             *
 *    DIBFromBitmap()     - Creates a DIB out of a DDB                         *
 *                                                                             *
 *    DrawBitmap()        - Draws a bitmap at a specified position in a DC.    *
 *                                                                             *
 *    DIBBlt()            - Draws a DIB at a specified position in a DC.       *
 *                                                                             *
 *    StretchDIBBlt()     - Draws DIBbitmap in CIF_DIB format using            *
 *                          StretchDIBits()                                    *
 *                                                                             *
 *    ChangeDIBFormat()   - Convert a DIB to another compression and bits per  *
 *                          pixel format.                                      *
 *                                                                             *
 *    ChangeDIBPalette()  - Map the colors in the given DIB to a new palette   *
 *                                                                             *
 *    CopyDIB()           - Make a copy of a DIB.                              *  
 *                                                                             *
 *    CopyBitmap()        - Make a copy of a bitmap.                           *
 *                                                                             *
 *    CropBitmap()        - Crops a bitmap to a new size specified by the      *
 *                          lpRect parameter.                                  *
 *                                                                             *
 *    CopyDIBData()       - Copy the data from one DIB to another.             *
 *                                                                             *
 *    GetDIBResolution()  - Returns the width and the height of a DIB as       *
 *                          specified in the BITMAPINFOHEADER                  *
 *                                                                             *
 *    GetDIBColorUsed()   - Returns the number of colors used by a DIB as      *
 *                          specified in the BITMAPINFOHEADER                  *
 *                                                                             *
 *    GetDIBCopmression() - Returns the compression used by a DIB as specified *
 *                          in the BITMAPINFOHEADER                            *
 *                                                                             *
 *    GetDIBBitcount()    - Returns the number of bits per pixel as specified  *
 *                          in the BITMAPINFOHEADER                            * 
 *                                                                             *
 *    CreateDIBPaletteEx()- Create a palette for a DIB and set the palette     *
 *                          entry flags to a specified value.                  *
 *                                                                             *
 *    SetOptimizedPaletteState() - Use an optimized palette (rather than a     *
 *                          spectrum palette) when loading (16/24/32bpp) DIBs  *
 *                                                                             *
 *    GetOptimizedPaletteState() - Returns true if bitmaps will be loaded      *
 *                          using an optimized palette.                        *
 *                                                                             *
 *******************************************************************************
 *                                                                             *
 *  Last modified: 1/20/95 by Mike Irvine                                      *
 *                                                                             *
 ******************************************************************************/  

#define STRICT

#include <windows.h>
#include "dib.h"
#include "palette.h"

#pragma warning(disable:4244)
#pragma warning(disable:4309)

static   HCURSOR hcurSave;

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :CountBits(DWORD dw)                                           *
 *                                                                            *
 *  PURPOSE    :Count the number of set bits in a given DWORD                 * 
 *                                                                            *
 *  *NOTE:     :Used only in CreateBIPalette() for extracting RGB values      *
 *              from 16 and 32bpp DIBS                                        *
 *                                                                            *
 *****************************************************************************/
BYTE CountBits(DWORD dw)
{
  int i, Count = 0;

  for (i=0; i<sizeof(DWORD) * 8; i++)
    Count += (((1 << i) & dw) != 0);

  return Count;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :RightmostBit(DWORD dw)                                        *
 *                                                                            *
 *  PURPOSE    :Returns the position of the rightmost set bit in a DWORD      * 
 *                                                                            *
 *  *NOTE:     :Used only in CreateBIPalette() for extracting RGB values      *
 *              from 16 and 32bpp DIBS                                        *
 *                                                                            *
 *****************************************************************************/
BYTE RightmostBit(DWORD dw)
{
  int i;

  for (i=0; i<sizeof(DWORD) * 8; i++)
    if (((1 << i) & dw) != 0)
      return i;

  return (BYTE)-1; // No bits are set
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :LeftmostBit(DWORD dw)                                         *
 *                                                                            *
 *  PURPOSE    :Returns the position of the leftmost set bit in a DWORD       * 
 *                                                                            *
 *  *NOTE:     :Used only in CreateBIPalette() for extracting RGB values      *
 *              from 16 and 32bpp DIBS                                        *
 *                                                                            *
 *****************************************************************************/
BYTE LeftmostBit(DWORD dw)
{
  int i;

  for (i=(sizeof(DWORD) * 8)-1; i >= 0; i--)
    if (((1 << i) & dw) != 0)
      return i;

  return (BYTE)-1; // No bits are set
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :ValidMask(DWORD dw)                                           *
 *                                                                            *
 *  PURPOSE    :Determines if a given DWORD is valid as a color mask for      * 
 *              16 or 32bpp DIBS (are all the set bits contiguous)            *
 *                                                                            *
 *  *NOTE:     :Used only in CreateBIPalette()                                *
 *                                                                            *
 *****************************************************************************/
BOOL ValidMask(DWORD dw)
{
  int iStart, iStop, i;
  
  iStart = LeftmostBit(dw);
  iStop  = RightmostBit(dw);

  if (iStart == iStop)
    return TRUE;

  for (i=iStart; i>=iStop; i--) 
     if (((1 << i) & dw) == 0)
       return FALSE;

  return TRUE;
}
 
HGLOBAL GlobalFreeDIB(HGLOBAL hDIB)
{
   LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)hDIB;

   if (!lpbi->biClrImportant)
     return GlobalFree(hDIB);

   if (GlobalFlags((HGLOBAL)lpbi->biClrImportant) == GMEM_INVALID_HANDLE) {
    SetLastError(0);
    return GlobalFree(hDIB);
  } else
    return GlobalFree((HANDLE)lpbi->biClrImportant); 
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :OpenDIB(LPSTR szFilename)                                     *
 *                                                                            *
 *  PURPOSE    :Open a DIB file and create a memory DIB -- a memory handle    *
 *              containing BITMAPINFO, palette data and the bits.             * 
 *                                                                            *
 *  RETURNS    :A handle to the DIB.                                          *
 *                                                                            *
 *****************************************************************************/
HANDLE OpenDIB (LPCSTR szFilename)
{
    HFILE               hFile;
    BITMAPINFOHEADER    bih;
    LPBITMAPINFOHEADER  lpbih;
    DWORD               dwLen = 0;
    DWORD               dwBits;
    HANDLE              hDIB;
    HANDLE              hMem;
    OFSTRUCT            of;

    /* Open the file and read the DIB information */
    hFile = OpenFile(szFilename, &of, (UINT)OF_READ);
    if (hFile == HFILE_ERROR)
        return NULL;

    hDIB = ReadDIBBitmapInfo(hFile);
    if (!hDIB)
        return NULL;
    DIBInfo(hDIB, &bih);

    /* Calculate the memory needed to hold the DIB */
    dwBits = bih.biSizeImage;
    dwLen  = bih.biSize + (DWORD)ColorTableSize (&bih) + dwBits;

    /* Try to increase the size of the bitmap info. buffer to hold the DIB */
    hMem = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE);

    if (!hMem){
        GlobalFreeDIB(hDIB);
        hDIB = NULL;
    }
    else
        hDIB = hMem;

    /* Read in the bits */
    if (hDIB){
        lpbih = (LPBITMAPINFOHEADER)hDIB;
        _hread(hFile, (LPSTR)lpbih + (WORD)lpbih->biSize + ColorTableSize(lpbih), dwBits);
    }
    _lclose(hFile);

    return hDIB;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : WriteDIB(LPSTR szFilename, HANDLE hDIB)                      *
 *                                                                            *
 *  PURPOSE    : Write a global handle in CF_DIB format to a file.            *
 *                                                                            *
 *  RETURNS    : TRUE  - if successful.                                       *
 *               FALSE - otherwise                                            *
 *                                                                            *
 *****************************************************************************/
BOOL WriteDIB (LPCSTR szFilename, HANDLE hDIB)
{
    BITMAPFILEHEADER    bmfhdr;
    LPBITMAPINFOHEADER  lpbih;
    HFILE               hFile;
    OFSTRUCT            of;

    if (!hDIB)
        return FALSE;

    hFile = OpenFile(szFilename, &of, (UINT)OF_CREATE|OF_READWRITE);
    if (hFile == HFILE_ERROR)
        return FALSE;

    lpbih = (LPBITMAPINFOHEADER)hDIB;

    /* Fill in the fields of the file header */
    bmfhdr.bfType          = BFT_BITMAP;
    bmfhdr.bfSize          = GlobalSize (hDIB) + SIZEOF_BITMAPFILEHEADER_PACKED;
    bmfhdr.bfReserved1     = 0;
    bmfhdr.bfReserved2     = 0;
    bmfhdr.bfOffBits       = (DWORD)(SIZEOF_BITMAPFILEHEADER_PACKED + 
                                  lpbih->biSize + ColorTableSize(lpbih));

    WritePackedFileHeader(hFile, &bmfhdr);

    /* Write the DIB header and the bits */
    _hwrite (hFile, (LPSTR)lpbih, GlobalSize (hDIB));

    _lclose(hFile);
 
    return TRUE;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION  : WriteDIBEx(LPSTR szFilename, HANDLE hDIB,                     *
 *                         WORD wBPP, DWORD dwComp)                           *                           
 *                                                                            *
 *  PURPOSE   : Write a global handle in CF_DIB format to a file with a       *
 *              specified pixel depth and compression format                  *
 *                                                                            *
 *  RETURNS   : TRUE  - if successful.                                        *
 *              FALSE - otherwise                                             *
 *                                                                            *
 *****************************************************************************/
 BOOL WriteDIBEx(LPCSTR szFilename, HANDLE hDIB, WORD wBPP, DWORD dwComp)
 {
    BITMAPINFOHEADER bih;
    HANDLE hNewDIB;
    BOOL bReturn;

    DIBInfo(hDIB, &bih);

    if (((bih.biBitCount == wBPP) && (dwComp == bih.biCompression)) || ((wBPP == 0) && (dwComp == 0)))
       return WriteDIB(szFilename, hDIB);

    hNewDIB = CopyDIB(hDIB);

    StartWait();
    
    hDIB = ChangeDIBFormat(hNewDIB, wBPP, dwComp);
    if (hDIB) {
      bReturn = WriteDIB(szFilename, hDIB);
      GlobalFreeDIB(hDIB);
    } else {
      bReturn = FALSE;
      GlobalFreeDIB(hNewDIB);
    }

    EndWait();
     
    return bReturn;
 }

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : DIBInfo(HANDLE hbi, LPBITMAPINFOHEADER lpbih)                *
 *                                                                            *
 *  PURPOSE    : Retrieves the DIB info associated with a CF_DIB              *
 *               format memory block.                                         *
 *                                                                            *
 *  RETURNS    : TRUE  - if successful.                                       *
 *               FALSE - otherwise                                            *
 *                                                                            *
 *****************************************************************************/
BOOL DIBInfo (HANDLE hbi, LPBITMAPINFOHEADER lpbih)
{
    if (hbi){
      *lpbih = *(LPBITMAPINFOHEADER)hbi;

      /* fill in the default fields */
      if (NEW_DIB_FORMAT(lpbih)) {
        if (lpbih->biSizeImage == 0L)
          lpbih->biSizeImage = WIDTHBYTES(lpbih->biWidth*lpbih->biBitCount) * lpbih->biHeight;

        if (lpbih->biClrUsed == 0L)
          lpbih->biClrUsed = DIBNumColors (lpbih);
      }

      return TRUE;
    }
    return FALSE;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : CreateBIPalette(LPBITMAPINFOHEADER lpbih)                    *
 *                                                                            *
 *  PURPOSE    : Given a Pointer to a BITMAPINFO struct will create a         *
 *               a GDI palette object from the color table.                   *
 *                                                                            *
 *  RETURNS    : A handle to the palette.                                     *
 *                                                                            *
 *****************************************************************************/
HPALETTE CreateBIPalette (LPBITMAPINFOHEADER lpbih)
{
    HPALETTE       hPal = NULL;
    WORD           nNumColors;
    LPRGBQUAD      lprgbq;

    if (!lpbih)
        return NULL;

    if (!NEW_DIB_FORMAT(lpbih))  // Return if we dont have a BITMAPINFOHEADER
        return NULL;

    /* Get a pointer to the color table and the number of colors in it */
    lprgbq = (LPRGBQUAD)((LPSTR)lpbih + (WORD)lpbih->biSize);
    /* Adjust pointer for cases where BITFIELDS bitmaps might contain an optimal palette */
    if (lpbih->biCompression == BI_BITFIELDS)
      lprgbq = (LPRGBQUAD)((LPDWORD)lprgbq + 3); 
    nNumColors = DIBNumColors(lpbih);
	lpbih->biClrUsed = nNumColors;

    if (nNumColors)   /* Create a palette from the entries in the DIBs color table */
        hPal = CreatePaletteFromRGBQUAD(lprgbq, nNumColors);
    else 
        {
          HDC hDC = GetDC(NULL);
          hPal = CreateHalftonePalette(hDC);
          ReleaseDC(NULL, hDC);
        } ;
  
    return hPal;
}
              

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : CreateDIBPalette(HANDLE hbi)                                 *
 *                                                                            *
 *  PURPOSE    : Given a Global HANDLE to a BITMAPINFO Struct will create a   *
 *               GDI palette object from the color table. (BITMAPINFOHEADER   *
 *               format DIBs only)                                            *
 *                                                                            *
 *  RETURNS    : A handle to the palette.                                     *
 *                                                                            *
 *****************************************************************************/
HPALETTE CreateDIBPalette (HANDLE hbi)
{
    HPALETTE hPal;

    if (!hbi)
        return NULL;

    hPal = CreateBIPalette((LPBITMAPINFOHEADER)hbi);

    return hPal;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : ReadDIBBitmapInfo(int hFile)                                 *
 *                                                                            *
 *  PURPOSE    : Will read a file in DIB format and return a global HANDLE    *
 *               to it's BITMAPINFO.  This function will work with both       *
 *               "old" (BITMAPCOREHEADER) and "new" (BITMAPINFOHEADER)        *
 *               bitmap formats, but will always return a "new" BITMAPINFO    *
 *                                                                            *
 *  RETURNS    : A handle to the BITMAPINFO of the DIB in the file.           *
 *                                                                            *
 *****************************************************************************/
HANDLE ReadDIBBitmapInfo (INT hFile)
{
    DWORD              dwOffset;
    HANDLE             hbi = NULL;
    INT                size;
    INT                i;
    WORD               nNumColors;
    LPRGBQUAD          lprgbq;
    BITMAPINFOHEADER   bih;
    BITMAPCOREHEADER   bch;
    LPBITMAPINFOHEADER lpbih;
    BITMAPFILEHEADER   bf;
    DWORD              dwDWMasks= 0;
    DWORD              dwWidth = 0;
    DWORD              dwHeight = 0;
    WORD               wPlanes, wBitCount;

    if (hFile == HFILE_ERROR)
        return NULL;

    /* Read the bitmap file header */
    ReadPackedFileHeader(hFile, &bf, &dwOffset);

    /* Do we have a RC HEADER? */
    if (!ISDIB (bf.bfType)) {    
      bf.bfOffBits = 0L;               
        _llseek(hFile, dwOffset, (UINT)SEEK_SET); /* seek back to beginning of file */
    }

    if (sizeof(bih) != _hread(hFile, (LPSTR)&bih, (UINT)sizeof(bih)))
      return FALSE;

    nNumColors = DIBNumColors (&bih);

    /* Check the nature (BITMAPINFO or BITMAPCORE) of the info. block
     * and extract the field information accordingly. If a BITMAPCOREHEADER, 
     * transfer it's field information to a BITMAPINFOHEADER-style block
     */
    switch (size = (INT)bih.biSize){
        case sizeof (BITMAPINFOHEADER):
            break;

        case sizeof (BITMAPCOREHEADER):

            bch = *(LPBITMAPCOREHEADER)&bih;

            dwWidth   = (DWORD)bch.bcWidth;
            dwHeight  = (DWORD)bch.bcHeight;
            wPlanes   = bch.bcPlanes;
            wBitCount = bch.bcBitCount;

            bih.biSize           = sizeof(BITMAPINFOHEADER);
            bih.biWidth          = dwWidth;
            bih.biHeight         = dwHeight;
            bih.biPlanes         = wPlanes;
            bih.biBitCount       = wBitCount;
            bih.biCompression    = BI_RGB;
            bih.biSizeImage      = 0;
            bih.biXPelsPerMeter  = 0;
            bih.biYPelsPerMeter  = 0;
            bih.biClrUsed        = nNumColors;
            bih.biClrImportant   = nNumColors;

            _llseek(hFile, (LONG)sizeof (BITMAPCOREHEADER) - sizeof (BITMAPINFOHEADER), (UINT)SEEK_CUR);
            break;

        default:
            /* Not a DIB! */
            return NULL;
    }

    /*  Fill in some default values if they are zero */
    if (bih.biSizeImage == 0){
        bih.biSizeImage = WIDTHBYTES((DWORD)bih.biWidth * bih.biBitCount) * bih.biHeight;
    }
    if (bih.biClrUsed == 0)
        bih.biClrUsed = DIBNumColors(&bih);

    /* Allocate for the BITMAPINFO structure and the color table. */
    if ((bih.biBitCount == 16) || (bih.biBitCount == 32))
      dwDWMasks = sizeof(DWORD) * 3;
    hbi = GlobalAlloc (GPTR, (LONG)bih.biSize + nNumColors * sizeof(RGBQUAD) + dwDWMasks);
    if (!hbi)
        return NULL;
    lpbih = (LPBITMAPINFOHEADER)hbi;
    *lpbih = bih;

    /* Get a pointer to the color table */
    lprgbq = (LPRGBQUAD)((LPSTR)lpbih + bih.biSize);
    if (nNumColors){
        if (size == sizeof(BITMAPCOREHEADER)){
            /* Convert a old color table (3 byte RGBTRIPLEs) to a new
             * color table (4 byte RGBQUADs)
             */
            _hread(hFile, (LPSTR)lprgbq, (UINT)nNumColors * sizeof(RGBTRIPLE));

            for (i = nNumColors - 1; i >= 0; i--){
                RGBQUAD rgbq;

                rgbq.rgbRed      = ((RGBTRIPLE*)lprgbq)[i].rgbtRed;
                rgbq.rgbBlue     = ((RGBTRIPLE*)lprgbq)[i].rgbtBlue;
                rgbq.rgbGreen    = ((RGBTRIPLE*)lprgbq)[i].rgbtGreen;
                rgbq.rgbReserved = (BYTE)0;

                lprgbq[i] = rgbq;
            }
        }
        else
            _hread(hFile, (LPSTR)lprgbq, (UINT)nNumColors * sizeof(RGBQUAD));
    } else
        if (dwDWMasks)
           _hread(hFile, (LPSTR)lprgbq, dwDWMasks);

    if (bf.bfOffBits != 0L){
        _llseek(hFile, dwOffset + bf.bfOffBits, (UINT)SEEK_SET);
        }
    
    return hbi;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :  ColorTableSize(LPVOID lpv)                                  *
 *                                                                            *
 *  PURPOSE    :  Calculates the palette size in bytes. If the info. block    *
 *                is of the BITMAPCOREHEADER type, the number of colors is    *
 *                multiplied by 3 to give the palette size, otherwise the     *
 *                number of colors is multiplied by 4.                        *
 *                                                                            *
 *  RETURNS    :  Color table size in number of bytes.                        *
 *                                                                            *
 *****************************************************************************/
WORD ColorTableSize (LPVOID lpv)
{
    LPBITMAPINFOHEADER lpbih = (LPBITMAPINFOHEADER)lpv;
    
    if (NEW_DIB_FORMAT(lpbih))
    {
      if (((LPBITMAPINFOHEADER)(lpbih))->biCompression == BI_BITFIELDS)
         /* Remember that 16/32bpp dibs can still have a color table */
         return (sizeof(DWORD) * 3) + (DIBNumColors (lpbih) * sizeof (RGBQUAD));
      else
         return (DIBNumColors (lpbih) * sizeof (RGBQUAD));
    }
    else
      return (DIBNumColors (lpbih) * sizeof (RGBTRIPLE));
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : DIBNumColors(LPVOID lpv)                                     *
 *                                                                            *
 *  PURPOSE    : Determines the number of colors in the DIB by looking at     *
 *               the BitCount and ClrUsed fields in the info block.           *
 *                                                                            *
 *  RETURNS    : The number of colors in the DIB. With DIBS with more than    *
 *               8-bits-per-pixel that have a color table table included,     *
 *               then the return value will be the number of colors in the    *
 *               color table rather than the number of colors in the DIB.     *
 *                                                                            *
 *                                                                            *
 *****************************************************************************/
WORD DIBNumColors (LPVOID lpv)
{
    INT                 bits;
    LPBITMAPINFOHEADER  lpbih = (LPBITMAPINFOHEADER)lpv;
    LPBITMAPCOREHEADER  lpbch = (LPBITMAPCOREHEADER)lpv;

    /*  With the BITMAPINFO format headers, the size of the palette
     *  is in biClrUsed, whereas in the BITMAPCORE - style headers, it
     *  is dependent on the bits per pixel ( = 2 raised to the power of
     *  bits/pixel).
     */
    if (NEW_DIB_FORMAT(lpbih)) {
      if (lpbih->biClrUsed != 0)
        return (WORD)lpbih->biClrUsed;
      bits = lpbih->biBitCount;
    }
    else
      bits = lpbch->bcBitCount;

    if (bits > 8) 
      return 0; /* Since biClrUsed is 0, we dont have a an optimal palette */
    else
      return (1 << bits); 
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : DIBFromBitmap()                                              *
 *                                                                            *
 *  PURPOSE    : Will create a global memory block in DIB format that         *
 *               represents the Device-dependent bitmap (DDB) passed in.      *
 *                                                                            *
 *  RETURNS    : A handle to the DIB                                          *
 *                                                                            *
 *****************************************************************************/
HANDLE DIBFromBitmap (HBITMAP hBitmap, DWORD biStyle, WORD biBits, HPALETTE hPal)
{
    BITMAP               Bitmap;
    BITMAPINFOHEADER     bih;
    LPBITMAPINFOHEADER   lpbih;
    DWORD                dwLen;
    HANDLE               hDIB;
    HANDLE               hMem;
    HDC                  hDC;

    if (!hBitmap)
        return NULL;

    if (hPal == NULL)
        hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

    GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);

    if (biBits == 0)
        biBits =  Bitmap.bmPlanes * Bitmap.bmBitsPixel;

    bih.biSize            = sizeof(BITMAPINFOHEADER);
    bih.biWidth           = Bitmap.bmWidth;
    bih.biHeight          = Bitmap.bmHeight;
    bih.biPlanes          = 1;
    bih.biBitCount        = biBits;
    bih.biCompression     = biStyle;
    bih.biSizeImage       = 0;
    bih.biXPelsPerMeter   = 0;
    bih.biYPelsPerMeter   = 0;
    bih.biClrUsed         = 0;
    bih.biClrImportant    = 0;

    dwLen  = bih.biSize + ColorTableSize(&bih);

    hDC = GetDC(NULL);
    hPal = SelectPalette(hDC, hPal, FALSE);
    RealizePalette(hDC);

    hDIB = GlobalAlloc(GPTR, dwLen);

    if (!hDIB) {
        SelectPalette(hDC, hPal, FALSE);
        ReleaseDC(NULL, hDC);
        return NULL;
    }

    lpbih = (LPBITMAPINFOHEADER)hDIB;

    *lpbih = bih;

    /*  call GetDIBits with a NULL lpBits param, so it will calculate the
     *  biSizeImage field for us
     */
    GetDIBits(hDC, 
              hBitmap, 
              (UINT)0, 
              (UINT)bih.biHeight, 
              (LPVOID)NULL, 
              (LPBITMAPINFO)lpbih, 
              DIB_RGB_COLORS);

    bih = *lpbih;

    /* If the driver did not fill in the biSizeImage field, make one up */
    if (bih.biSizeImage == 0){
        bih.biSizeImage = WIDTHBYTES((DWORD)Bitmap.bmWidth * biBits) * Bitmap.bmHeight;

        if (biStyle != BI_RGB)
            bih.biSizeImage = (bih.biSizeImage * 3) / 2;
    }

    /*  realloc the buffer big enough to hold all the bits */
    dwLen = bih.biSize + ColorTableSize(&bih) + bih.biSizeImage;
    if (hMem = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
        hDIB = hMem;
    else {
        GlobalFreeDIB(hDIB);
        hDIB = NULL;

        SelectPalette(hDC, hPal, FALSE);
        ReleaseDC(NULL, hDC);
        return hDIB;
    }

    /*  call GetDIBits with a NON-NULL lpBits param, and actualy get the
     *  bits this time
     */
    lpbih = (LPBITMAPINFOHEADER)hDIB;

    if (!GetDIBits(hDC, 
                   hBitmap, 
                   (UINT)0, 
                   (UINT)bih.biHeight, 
                   (LPBYTE)lpbih + (WORD)lpbih->biSize + ColorTableSize(lpbih), 
                   (LPBITMAPINFO)lpbih, 
                   DIB_RGB_COLORS)) {
         hDIB = NULL;
         SelectPalette(hDC, hPal, FALSE);
         ReleaseDC(NULL, hDC);
         return NULL;
    }

    bih = *lpbih;

    SelectPalette(hDC, hPal, FALSE);
    ReleaseDC(NULL, hDC);

    return hDIB;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : BitmapFromDIB(HANDLE hDIB, HPALETTE hPal)                    *
 *                                                                            *
 *  PURPOSE    : Will create a DDB (Device Dependent Bitmap) given a global   *
 *               handle to a memory block in CF_DIB format                    *
 *                                                                            *
 *  RETURNS    : A handle to the DDB.                                         *
 *                                                                            *
 *****************************************************************************/
HBITMAP BitmapFromDIB (HANDLE hDIB, HPALETTE  hPal)
{
    LPBITMAPINFOHEADER  lpbih;
    HPALETTE            hPalOld;
    HDC                 hDC;
    HBITMAP             hBitmap;  

    StartWait();

    if (!hDIB)
        return NULL;

    lpbih = (LPBITMAPINFOHEADER)hDIB;

    if (!lpbih)
        return NULL;

    hDC = GetDC(NULL);

    if (hPal){
        hPalOld = SelectPalette(hDC, hPal, FALSE);
        RealizePalette(hDC);     // GDI Bug...????
    }                              
   
    hBitmap = CreateDIBitmap(hDC, 
                lpbih, 
                CBM_INIT, 
                (LPSTR)lpbih + lpbih->biSize + ColorTableSize(lpbih), 
                (LPBITMAPINFO)lpbih, 
                DIB_RGB_COLORS ); 

    if (hPal)
        SelectPalette(hDC, hPalOld, FALSE);

    ReleaseDC(NULL, hDC);

    EndWait();

    return hBitmap;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : DrawBitmap(HDC hDC, int x, int y,                            *
 *                          HBITMAP hBitmap, DWORD dwROP)                     * 
 *                                                                            *
 *  PURPOSE    : Draws bitmap <hBitmap> at the specifed position in DC <hDC>  *
 *                                                                            *
 *  RETURNS    : Return value of BitBlt()                                     *
 *                                                                            *
 *****************************************************************************/
BOOL DrawBitmap (HDC hDC, INT x, INT y, HBITMAP hBitmap, DWORD dwROP)
{
    HDC       hDCBits;
    BITMAP    Bitmap;
    BOOL      bResult;

    if (!hDC || !hBitmap)
        return FALSE;

    hDCBits = CreateCompatibleDC(hDC);
    GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
    SelectObject(hDCBits, hBitmap);
    bResult = BitBlt(hDC, x, y, Bitmap.bmWidth, Bitmap.bmHeight, hDCBits, 0, 0, dwROP);
    DeleteDC(hDCBits);

    return bResult;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : DIBBlt( HDC hDC,                                             *
 *                       int x0, int y0,                                      *
 *                       int dx, int dy,                                      *
 *                       HANDLE hDIB,                                         *
 *                       int x1, int y1,                                      *
 *                       LONG dwROP)                                          *
 *                                                                            *
 *  PURPOSE    : Draws a bitmap in CF_DIB format, using SetDIBits to device.  *
 *               taking the same parameters as BitBlt().                      *
 *                                                                            *
 *  RETURNS    : TRUE  - if function succeeds.                                *
 *               FALSE - otherwise.                                           *
 *                                                                            *
 ******************************************************************************/
BOOL DIBBlt (HDC hDC,     INT x0, INT y0, INT dx, INT dy, 
             HANDLE hDIB, INT x1, INT y1, LONG  dwROP)
{
    LPBITMAPINFOHEADER   lpbih;
    LPSTR                pBuf;

    if (!hDIB)
        return PatBlt(hDC, x0, y0, dx, dy, dwROP);

    lpbih = (LPBITMAPINFOHEADER)hDIB;

    if (!lpbih)
        return FALSE;

    pBuf = (LPSTR)lpbih + (WORD)lpbih->biSize + ColorTableSize(lpbih);
    SetDIBitsToDevice (hDC, x0, y0, dx, dy, 
                       x1, y1, 
                       x1, 
                       dy, 
                       pBuf, (LPBITMAPINFO)lpbih, 
                       DIB_RGB_COLORS );

    return TRUE;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : StretchDIBBlt( HDC hDC,                                      *
 *                              int x, int y,                                 *
 *                              int dx, int dy,                               *
 *                              HANDLE hDIB,                                  *
 *                              int x0, int y0,                               *
 *                              int dx0, int dy0,                             *
 *                              LONG dwROP)                                   *
 *                                                                            *
 *  PURPOSE    : Draws a bitmap in CF_DIB format, using StretchDIBits()       *
 *               taking the same parameters as StretchBlt().                  *
 *                                                                            *
 *  RETURNS    : TRUE  - if function succeeds.                                *
 *               FALSE - otherwise.                                           *
 *                                                                            *
 ******************************************************************************/
BOOL StretchDIBBlt (HDC hDC,     INT x,  INT y,  INT dx,  INT dy, 
                    HANDLE hDIB, INT x0, INT y0, INT dx0, INT dy0, LONG dwROP)

{
    LPBITMAPINFOHEADER lpbih;
    LPSTR        pBuf;
    BOOL         bResult;

    if (!hDIB)
        return PatBlt(hDC, x, y, dx, dy, dwROP);

    lpbih = (LPBITMAPINFOHEADER)hDIB;

    if (!lpbih)
        return FALSE;

    pBuf = (LPSTR)lpbih + (WORD)lpbih->biSize + ColorTableSize(lpbih);
    
    bResult = StretchDIBits (hDC, x,  y,  dx,  dy, 
                                  x0, y0, dx0, dy0, 
                             pBuf, (LPBITMAPINFO)lpbih, 
                             DIB_RGB_COLORS, dwROP);

    return bResult;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : ReadPackedFileHeader(HFILE hFile, LPBITMAPFILEHEADER pbf)    *
 *                                                                            *
 *  PURPOSE    : read file header (which is packed) and convert into          *
 *               unpacked BITMAPFILEHEADER strucutre                          *
 *                                                                            *
 *  RETURNS    : VOID                                                         *
 *                                                                            *
 *****************************************************************************/
VOID ReadPackedFileHeader(HFILE hFile, LPBITMAPFILEHEADER lpbmfhdr, LPDWORD lpdwOffset)
{
    *lpdwOffset = _llseek(hFile, 0L, (UINT) SEEK_CUR);
    _hread(hFile, (LPSTR) &lpbmfhdr->bfType, sizeof(WORD)); /* read in bfType*/            
    _hread(hFile, (LPSTR) &lpbmfhdr->bfSize, sizeof(DWORD) * 3); /* read in last 3 dwords*/
}
  
/******************************************************************************
 *                                                                            *
 *  FUNCTION   : WritePackedFileHeader(HFILE hFile, LPBITMAPFILEHEADER pbf)   *
 *                                                                            *
 *  PURPOSE    : write header structure (which NOT packed) and write          *
 *               it out in PACKED format                                      *
 *                                                                            *
 *  RETURNS    : VOID                                                         *
 *                                                                            *
 *****************************************************************************/
VOID WritePackedFileHeader(HFILE hFile, LPBITMAPFILEHEADER lpbmfhdr)
{                                                       
    _hwrite(hFile, (LPSTR)&lpbmfhdr->bfType, (UINT)sizeof (WORD)); 

    /* pass over extra word and write next 3 DWORDS! */
    _hwrite(hFile, (LPSTR)&lpbmfhdr->bfSize, sizeof(DWORD) * 3);   
}

/******************************************************************************
 *                                                                            *
 * HANDLE ConvertRGBDIB(HANDLE hSrcDIB, DWORD dwDstComp, WORD wDstBPP)        *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HDIB             - handle to packed-DIB in memory                          *
 * WORD             - desired bits per pixel                                  *
 * DWORD            - desired compression format                              *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HDIB             - handle to the new DIB if successful, else NULL          *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function will create a new DIB with the specified BPP format using an *
 * existing DIB as the source of the bits.  The conversion method will keep   *
 * as much of the color information as possible. Both source and destination  *
 * must be at least 16 bits-per-pixel.                                        *
 *                                                                            *
 * *Notes:                                                                    *
 *    The source DIB is never destroyed                                       *
 *    This function is not exposed and it is only called by ChangeDIBFormat() *
 *    The function will return NULL if it fails                               *
 *                                                                            *
 *****************************************************************************/
HANDLE ConvertRGBDIB(HANDLE hSrcDIB, DWORD dwDstComp, WORD wDstBPP)
{
    HDC hDC = GetDC(NULL);
    HDC hSrcMemDC = CreateCompatibleDC(NULL);
    HDC hDstMemDC = CreateCompatibleDC(NULL);
    HBITMAP hSrcBmp = NULL, hDstBmp = NULL;
    HANDLE hDstDIB = NULL;
 	LPBITMAPINFOHEADER biSrcPtr, biDstPtr;
    LPRGBQUAD rgbqSrcPtr, rgbqDstPtr; 
    RGBTRIPLE *bmSrcPtr, *bmDstPtr;
    LPVOID pSrcDisp, pDstDisp;                                

    __try {
        /* This method of conversion only works with > 8bpp format DIBs */
        if ((wDstBPP < 16) || (GetDIBBitCount(hSrcDIB) < 16))
          RAISE_AN_EXCEPTION();

        /* Initialize pointers for the source DIB */
        if (!GetDIBPointers(hSrcDIB, (void **)&biSrcPtr, (void **)&rgbqSrcPtr, (void **)&bmSrcPtr))
          RAISE_AN_EXCEPTION();

        /* Create destination DIB and initialize pointers for it */
        hDstDIB = CreateRGBDIB(biSrcPtr->biWidth, biSrcPtr->biHeight, wDstBPP, dwDstComp);
        if (!GetDIBPointers(hDstDIB, (void **)&biDstPtr, (void **)&rgbqDstPtr, (void **)&bmDstPtr))
          RAISE_AN_EXCEPTION();
    
    	/* Create and load source DIB into source bitmap */
    	hSrcBmp = CreateDIBSection(hDC, (LPBITMAPINFO)biSrcPtr, 0, &pSrcDisp, NULL, 0);
        if (!SelectObject(hSrcMemDC, hSrcBmp))
          RAISE_AN_EXCEPTION();
    	CopyMemory(pSrcDisp, bmSrcPtr, biSrcPtr->biSizeImage);

        /* Create destination bitmap */
        hDstBmp = CreateDIBSection(hDC, (LPBITMAPINFO)biDstPtr, 0, &pDstDisp, NULL, 0);
        if (!SelectObject(hDstMemDC, hDstBmp))
          RAISE_AN_EXCEPTION();

        /* Make GDI do the conversion between the different BPP formats */
        BitBlt(hDstMemDC, 0, 0, biSrcPtr->biWidth, biSrcPtr->biHeight,
               hSrcMemDC, 0, 0, SRCCOPY);
        GdiFlush();
          
        /* Copy the converted data to our destination DIB */
        CopyMemory(bmDstPtr, pDstDisp, biDstPtr->biSizeImage);
    } __except (EXCEPTION_EXECUTE_HANDLER) {

        /* Something got hosed, make sure we deallocate our destination DIB */
        if (hDstDIB) {
          GlobalFreeDIB(hDstDIB);
          hDstDIB = NULL;
        }                
    }

    /* Clean up the mess we made */
    if (hDC) ReleaseDC(NULL, hDC);
    if (hSrcMemDC) DeleteDC(hSrcMemDC);
    if (hDstMemDC) DeleteDC(hDstMemDC);
    if (hSrcBmp) DeleteObject(hSrcBmp);
    if (hDstBmp) DeleteObject(hDstBmp);

    return hDstDIB;          
}

/******************************************************************************
 *                                                                            *
 * HANDLE ChangeDIBFormat(HANDLE hDIB, WORD wBPP, DWORD dwComp)               *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HDIB             - handle to packed-DIB in memory                          *
 * WORD             - desired bits per pixel                                  *
 * DWORD            - desired compression format                              *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HDIB             - handle to the new DIB if successful, else NULL          *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function will convert the bits per pixel and/or the compression       *
 * format of the specified DIB. Note: If the conversion was unsuccessful,     *
 * we return NULL. The original DIB is left alone. Don't use code like the    *
 * following:                                                                 *
 *                                                                            *
 *    hMyDIB = ChangeDIBFormat(hMyDIB, 8, BI_RLE4);                           *
 *                                                                            *
 * The conversion will fail, but hMyDIB will now be NULL and the original     *
 * DIB will now hang around in memory. We could have returned the old         *
 * DIB, but we wanted to allow the programmer to check whether this           *
 * conversion succeeded or failed.                                            *
 *                                                                            *
 *****************************************************************************/
HANDLE ChangeDIBFormat(HANDLE hDIB, WORD wBPP, DWORD dwComp)
{
   HBITMAP            hBitmap;              // Handle to bitmap
   LPBITMAPINFOHEADER lpbih;                // Pointer to bitmap info
   HANDLE             hNewDIB = NULL;       // Handle to new DIB
   HPALETTE           hPal;                 // Handle to palette, prev pal
   WORD               wOldBPP, wNewBPP;     // DIB bits per pixel, new bpp
   DWORD              dwOldComp, dwNewComp; // DIB compression, new compression
    
   /* Check for a valid DIB handle */
   if (!hDIB)
      return NULL;
   
   /* Get the old DIB's bits per pixel and compression format */
   lpbih = (LPBITMAPINFOHEADER)hDIB;
   wOldBPP = lpbih->biBitCount;
   dwOldComp = lpbih->biCompression;

   /* Validate wBPP and dwComp
    * They must match correctly (i.e., BI_RLE4 and 4 BPP or
    * BI_RLE8 and 8BPP, etc.) or we return failure */
   if (wBPP == 0) {
      wNewBPP = wOldBPP;
      if ((dwComp == BI_RLE4 && wNewBPP == 4) ||
          (dwComp == BI_RLE8 && wNewBPP == 8) ||
          (dwComp == BI_RGB))
        dwNewComp = dwComp;
      else
        return NULL;
      }
   else if (wBPP == 1 && dwComp == BI_RGB) {
      wNewBPP = wBPP;
      dwNewComp = BI_RGB;
      }
   else if (wBPP == 4) {
      wNewBPP = wBPP;
      if (dwComp == BI_RGB || dwComp == BI_RLE4)
        dwNewComp = dwComp;
      else
        return NULL;
      }
   else if (wBPP == 8) {
      wNewBPP = wBPP;
      if (dwComp == BI_RGB || dwComp == BI_RLE8)
        dwNewComp = dwComp;
      else
        return NULL;
      }
   else if (wBPP == 24 && dwComp == BI_RGB) {
      wNewBPP = wBPP;
      dwNewComp = BI_RGB;
      }
   else if (((wBPP == 16) || (wBPP == 32)) && 
            ((dwComp == BI_BITFIELDS) || (dwComp == BI_RGB))) {
        wNewBPP = wBPP;
        dwNewComp = dwComp;
      }
   else
        return NULL;
   
   if ((wOldBPP > 8) && (wNewBPP > 8)) 
     /* Keep as much color info as possible */
     hNewDIB = ConvertRGBDIB(hDIB, dwNewComp, wNewBPP);

   if (!hNewDIB) {
       /* Save the old DIB's palette */
       hPal = CreateDIBPalette(hDIB);  
       if (!hPal)
          return NULL;
   
       /* Convert old DIB to a bitmap */
       hBitmap = BitmapFromDIB(hDIB, hPal);

       /* Convert Bitmap into a DIB with the new compression and BPP  */
       hNewDIB = DIBFromBitmap(hBitmap, dwNewComp, wNewBPP, hPal);
       
       DeleteObject(hBitmap);
       DeleteObject(hPal);
   }

   if (hNewDIB)
         GlobalFreeDIB(hDIB);

   return hNewDIB;
}

/******************************************************************************
 *                                                                            *
 * BOOL ChangeDIBPalette(HANDLE hDIB, HPALETTE hPal)                          *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HDIB             - handle to packed-DIB in memory                          *
 * HPALETTE         - the new palette for the DIB                             *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * BOOL             - returns success or failure status                       *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function will remap the colors in the given DIB so that they          *
 * reference the colors in the new palette - note that this is not the same   *
 * changing the DIBs color table since we are forcing the bits to be          *
 * remapped to new values in addition to changing the color table.            *
 *                                                                            *
 *****************************************************************************/
BOOL ChangeDIBPalette(HANDLE hDIB, HPALETTE hPal)
{
   HDC                hDC;      
   HBITMAP            hBitmap;  
   LPBITMAPINFOHEADER lpbih;      // Pointer to bitmap info
   HPALETTE           hOldPal;   // Handle to palette, prev pal
      
   /* Check for a valid DIB/Palette handle */
   if (!hDIB || !hPal)
      return FALSE;

   /* Convert old DIB to a bitmap */
   hBitmap = BitmapFromDIB(hDIB, hPal);

   /* Make sure it worked */
   if (!hBitmap) 
      return FALSE;

   /* Get a pointer to the DIB header */
   lpbih = (LPBITMAPINFOHEADER)hDIB;

   /* Get a DC and select/realize our palette in it */
   hDC  = GetDC(NULL);
   hOldPal = SelectPalette(hDC, hPal, FALSE);
   RealizePalette(hDC);

   /* Call GetDIBits and get the new DIB bits */
   if (!GetDIBits(hDC, 
                  hBitmap, 
                  (UINT)0, 
                  (UINT)lpbih->biHeight, 
                  (LPBYTE)lpbih + (WORD)lpbih->biSize + ColorTableSize((LPSTR)lpbih), 
                  (LPBITMAPINFO)lpbih, 
                  DIB_RGB_COLORS)) 
      return FALSE;

   /* Clean up and return */
   SelectPalette(hDC, hOldPal, TRUE);
   RealizePalette(hDC);
   ReleaseDC(NULL, hDC);

   DeleteObject(hBitmap);

   return TRUE;
}

/******************************************************************************
 *                                                                            *
 * HANDLE CopyDIB(HANDLE hDIBSrc)                                             *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HANDLE           - The handle to the DIB you want to make a copy of.       *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HANDLE           - A copy of the DIB passed as a parameter                 *  
 *                                                                            *
 *****************************************************************************/
HANDLE CopyDIB(HANDLE hDIBSrc)
{
  HANDLE hDIBDst = NULL;
  DWORD dwSize = GlobalSize(hDIBSrc);
    
  if (!hDIBSrc) 
    return NULL;	
  
  hDIBDst = GlobalAlloc(GPTR, dwSize);
  if (!hDIBDst) 
    return NULL;
    
  CopyMemory(hDIBDst, hDIBSrc, dwSize); 

  return (hDIBDst);
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : CopyBitmap (HBITMAP hBitmap)                               *
 *                                                                          *
 *  PURPOSE    : Copies the given bitmap to another.                        *
 *                                                                          *
 *  RETURNS    : A handle to the new bitmap.                                *
 *                                                                          *
 ****************************************************************************/
HBITMAP CopyBitmap (HBITMAP hBitmap)
{
    BITMAP  Bitmap;
    RECT    rect;

    if (!hBitmap)
         return NULL;

    GetObject (hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = Bitmap.bmWidth;
    rect.bottom = Bitmap.bmHeight;

    return CropBitmap (hBitmap, &rect);
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   :  CropBitmap (HBITMAP hBitmap, LPRECT lpRect)               *
 *                                                                          *
 *  PURPOSE    :  Crops a bitmap to a new size specified by the lpRect      *
 *                parameter.                                                *
 *                                                                          *
 *  RETURNS    :  A handle to the new bitmap.                               *
 *                                                                          *
 ****************************************************************************/
HBITMAP CropBitmap (HBITMAP hBitmap, LPRECT lpRect)
{
    HDC     hMemDCsrc;
    HDC     hMemDCdst;
    HDC     hDC;
    HBITMAP hNewBm;
    BITMAP  Bitmap;
    INT     dx, dy;

    if (!hBitmap)
         return NULL;

    hDC = GetDC (NULL);
    hMemDCsrc = CreateCompatibleDC (hDC);
    hMemDCdst = CreateCompatibleDC (hDC);

    GetObject (hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
    dx = lpRect->right  - lpRect->left;
    dy = lpRect->bottom - lpRect->top;

    /*hNewBm = +++CreateBitmap - Not Recommended(use CreateDIBitmap)+++ (dx, dy, Bitmap.BitmapPlanes, Bitmap.BitmapBitsPixel, NULL);*/
    hNewBm = CreateBitmap(dx, dy, Bitmap.bmPlanes, Bitmap.bmBitsPixel, NULL);
    if (hNewBm) {
        SelectObject (hMemDCsrc, hBitmap);
        SelectObject (hMemDCdst, hNewBm);

        BitBlt (hMemDCdst, 
                0, 
                0, 
                dx, 
                dy, 
                hMemDCsrc, 
                lpRect->left, 
                lpRect->top, 
                SRCCOPY);
    }

    ReleaseDC (NULL, hDC);
    DeleteDC (hMemDCsrc);
    DeleteDC (hMemDCdst);

    return hNewBm;
}

/******************************************************************************
 *                                                                            *
 * BOOL CopyDIBData(HANDLE hDIBDst, HANDLE hDIBSrc)                           *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HANDLE           - The handle to the DIB you want to copy to.              *
 * HANDLE           - The handle to the DIB you want to copy from.            *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HANDLE           - Success or failure of the function                      *  
 *                                                                            *
 *  Replaces the contents of the destination DIB with the contents of the     *
 *  source DIB.  This will fail if the initial sizes of the DIBS are          * 
 *  not equal.                                                                *
 *                                                                            *
 *****************************************************************************/
BOOL CopyDIBData(HANDLE hDIBDst, HANDLE hDIBSrc)
{
  DWORD dwDstSize = GlobalSize(hDIBDst);
  DWORD dwSrcSize = GlobalSize(hDIBSrc);
  
  if (dwDstSize != dwDstSize)
    return FALSE;

  CopyMemory(hDIBDst, hDIBSrc, dwDstSize); 

  return (TRUE);
}

/******************************************************************************
 *                                                                            *
 * HANDLE GetDIBFromClipboard(HWND hWnd)                                      *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HWND             - The handle of the window that will own the clipboard    *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HANDLE           - Handle to the DIB that was on the Clipboard             *  
 *                                                                            *
 *****************************************************************************/
HANDLE GetDIBFromClipboard(HWND hWnd)
{				  
  HANDLE hData;
  HANDLE hDIB;

  if (OpenClipboard(hWnd)) {
    hData = GetClipboardData(CF_DIB);
	if (hData == NULL) {
	  	hData = GetClipboardData(CF_BITMAP);
		if (hData != NULL) {
		  HPALETTE hPal, hClipPal = (HPALETTE)GetClipboardData(CF_PALETTE);
		  
		  hPal = CopyPalette(hClipPal);
		  hDIB = DIBFromBitmap((HBITMAP)hData, BI_RGB, 8, hPal);
          DeleteObject(hPal);
		} else
		  return NULL;
	} else 
		hDIB = CopyDIB(hData);
 	
	CloseClipboard();
  }	else
	return NULL;

  return hDIB;
}

/******************************************************************************
 *                                                                            *
 * BOOL GetDIBResolution(HANDLE hDIB, LONG *iXRes, LONG *iYRes)               *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HANDLE           - The handle to the DIB you want get the resolution of    *
 * LONG*            - Pointer to x resolution                                 *
 * LONG*            - Pointer to y resolution                                 *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HANDLE           - Success or failure of the function                      *  
 *                                                                            *
 *  Reads the width and the height of a DIB from the BITMAPINFOHEADER and     *
 *  returns the values.                                                       *
 *                                                                            *
 *****************************************************************************/
BOOL GetDIBResolution(HANDLE hDIB, LONG *iXRes, LONG *iYRes)
{
  if (!hDIB) return FALSE;
  
  *iXRes = ((LPBITMAPINFOHEADER)hDIB)->biWidth;
  *iYRes = ((LPBITMAPINFOHEADER)hDIB)->biHeight;

  return TRUE;
}

/******************************************************************************
 *                                                                            *
 * DWORD GetDIBColorUsed(HANDLE hDIB)                                         *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HANDLE           - The handle to the DIB you want to check                 *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * DWORD            - Number of colors used by the DIB                        *  
 *                                                                            *
 *  Reads and returns the number of colors used from the BITMAPINFOHEADER     *
 *                                                                            *
 *****************************************************************************/
DWORD GetDIBColorUsed(HANDLE hDIB)
{
  DWORD dwClrUsed;

  if (!hDIB) 
    return 0;
  
  dwClrUsed = ((LPBITMAPINFOHEADER)hDIB)->biClrUsed;

  return dwClrUsed;
}

/******************************************************************************
 *                                                                            *
 * DWORD GetDIBCopmression(HANDLE hDIB)                                       *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HANDLE           - The handle to the DIB you want to check                 *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * DWORD            - The compression type for the given DIB                  *  
 *                                                                            *
 *****************************************************************************/
DWORD GetDIBCompression(HANDLE hDIB)
{
  DWORD dwComp;

  if (!hDIB) 
    return 0;
  
  dwComp = ((LPBITMAPINFOHEADER)hDIB)->biCompression;

  return dwComp;
}

/******************************************************************************
 *                                                                            *
 * DWORD GetDIBBitcount(HANDLE hDIB)                                          *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HANDLE           - The handle to the DIB you want to check                 *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * DWORD            - The number of bits per pixel for given DIB              *  
 *                                                                            *
 *****************************************************************************/
WORD GetDIBBitCount(HANDLE hDIB)
{
  WORD wBitCount;

  if (!hDIB) 
    return 0;
  
  wBitCount = ((LPBITMAPINFOHEADER)hDIB)->biBitCount;

  return wBitCount;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : CreateDIBPaletteEx(HANDLE hbi, BYTE bFlags)                  *
 *                                                                            *
 *  PURPOSE    : Given a Global HANDLE to a BITMAPINFO Struct will create a   *
 *               GDI palette object from the color table. (BITMAPINFOHEADER   *
 *               format DIBs only)  In addition, you can specify the palette  *
 *               palette flags for the palette entries.                       *
 *                                                                            *
 *  RETURNS    : A handle to the palette.                                     *
 *                                                                            *
 *****************************************************************************/
HPALETTE CreateDIBPaletteEx(HANDLE hDIB, BYTE bFlags)
{
    HPALETTE hPal, hNewPal;

    if (!hDIB)
        return NULL;

    hPal = CreateDIBPalette(hDIB);
    hNewPal = CopyPaletteEx(hPal, bFlags);
    DeleteObject(hPal);
       
    return hNewPal;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : GetDIBPointers(HANDLE hDIB, LPVOID *biPtr,                   *
 *                                     LPVOID *rgbqPtr, LPVOID *bmPtr)        *
 *                                                                            *
 *  PURPOSE    : Given a handle to a DIB in CF_DIB format, this function will *
 *               retrieve pointers for the bitmap info, color table, and bits *
 *                                                                            *
 *****************************************************************************/
BOOL GetDIBPointers(HANDLE hDIB, LPVOID *biPtr, LPVOID *rgbqPtr, LPVOID *bmPtr)
{  
   LPBITMAPINFO pbi;
   
   pbi = (LPBITMAPINFO)hDIB;  
   
   if (!pbi)
     return FALSE;

   *biPtr   = (LPVOID)pbi;
   
   *rgbqPtr = (LPVOID)((LPSTR)pbi + (WORD)((LPBITMAPINFOHEADER)pbi)->biSize);
   if (((LPBITMAPINFOHEADER)pbi)->biCompression == BI_BITFIELDS)
      //rgbqPtr = (LPVOID)((LPDWORD)rgbqPtr + 3);
	  rgbqPtr = (LPVOID *)((LPDWORD)rgbqPtr + 3);

   *bmPtr   = (LPBYTE)pbi + (WORD)pbi->bmiHeader.biSize + ColorTableSize(pbi); 

   return TRUE;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : HANDLE CreateRGBDIB(DWORD dwWidth, DWORD dwHeight,           *
 *                                    WORD wBPP, DWORD dwComp);               *
 *                                                                            *
 *  PURPOSE    : Returns a handle to a RGB DIB (no color table) with the      *
 *               specified width, height, bits per pixel, and compression.    *
 *                                                                            *
 *****************************************************************************/
HANDLE CreateRGBDIB(DWORD dwWidth, DWORD dwHeight, WORD wBPP, DWORD dwComp)
{
   HANDLE hDIB;
   LPBITMAPINFOHEADER lpbih;
   DWORD dwSize;
   LPDWORD lpMasks;
   
   /* We cant support compression so check to see if that's what is being asked for */
   if ((dwComp == BI_RLE8) || (dwComp == BI_RLE4)) return NULL;

   /* Allocate enough memory to hold the DIB */
   switch (wBPP) {
     case  8: dwSize = ((DWORD)BYTESPERLINE(dwWidth, 8) * dwHeight) + sizeof(RGBQUAD) * 256; break;
     case 16: dwSize = ((dwWidth + (dwWidth & 1)) << 1) * dwHeight; break;
     case 24: dwSize = (DWORD)BYTESPERLINE(dwWidth, 24) * dwHeight; break;
     case 32: dwSize = ((dwWidth * dwHeight) << 2); break;
     default: return NULL;
   }                                               
   
   if (dwComp == BI_BITFIELDS) /* Add in space for DWORD masks */
     hDIB = GlobalAlloc(GPTR, (DWORD)sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * 3) + dwSize);
   else
     hDIB = GlobalAlloc(GPTR, (DWORD)sizeof(BITMAPINFOHEADER) + dwSize);

   if (!hDIB)
     return NULL;
   
   lpbih = (LPBITMAPINFOHEADER)hDIB;
   lpbih->biSize         = sizeof(BITMAPINFOHEADER);
   lpbih->biWidth        = dwWidth;
   lpbih->biHeight       = dwHeight;
   lpbih->biPlanes       = 1;
   lpbih->biBitCount     = wBPP;
   lpbih->biCompression  = dwComp;    
   lpbih->biSizeImage    = ((DWORD)BYTESPERLINE(dwWidth, wBPP) * dwHeight); 

   lpMasks = (LPDWORD)((LPSTR)lpbih + (WORD)lpbih->biSize);
   
   if (dwComp == BI_BITFIELDS) 
       if (wBPP == 16) {
         lpMasks[0] = MAKE565WORD(0xff, 0, 0);
         lpMasks[1] = MAKE565WORD(0, 0xff, 0);
         lpMasks[2] = MAKE565WORD(0, 0, 0xff);
       } else if (wBPP == 32) {
           lpMasks[0] = 0xff0000;
           lpMasks[1] = 0x00ff00;
           lpMasks[2] = 0x0000ff;
         }
      
   return hDIB;
}


