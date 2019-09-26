/*******************************************************************************
 *                                                                             *
 *  MODULE      : PALETTE.C                                                    *
 *                                                                             *
 *  DESCRIPTION : Routines for dealing with palettes                           *
 *                                                                             *
 *    PalEntriesOnDevice()   - number of palette entries on device             *
 *																			   *
 *    CreatePaletteFromRGBQUAD() - Build a valid HPALETTE when given an array  *
 *                          of RGBQUADs                                        *
 *                                                                             *
 *    GetSystemPalette()  - returns a handle to a copy of the current system   *
 *                          palette                                            *  
 *                                                                             *
 *    CreateSpectrumPalette() - Build a palette with a spectrum of colors.     *
 *                                                                             *
 *    CopyPalette()       - Make a copy of a palette.                          *
 *																			   *
 *    CopyPaletteEx()     - Copy a palette and alter the flags                 *
 *                                                                             *
 *    CreateIdentityPalette() - Creates an identity palette from an array of   *
 *                          RGBQUADS.                                          *
 *                                                                             *
 *    ClearSystemPalette()- Clears the system palette.                         *
 *                                                                             *
 *******************************************************************************
 *                                                                             *
 *  Last modified: 1/20/95 by Mike Irvine                                      *
 *                                                                             *
 ******************************************************************************/  

#define STRICT
 
#include <windows.h>
#include "palette.h"

/******************************************************************************
 *                                                                            *
 * PalEntriesOnDevice()                                                       *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HDC hDC          - device context                                          *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * int              - number of palette entries on device                     *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function gets the number of palette entries on the specified device   *
 *                                                                            *
 * History:   Date      Author               Reason                           *
 *            6/01/91   Garrett McAuliffe    Created                          *
 *            9/15/91   Patrick Schreiber    Added header and comments        *
 *                                                                            *
 *****************************************************************************/   
int PalEntriesOnDevice(HDC hDC)                                      
{                                                                        
   int nColors;  // number of colors                                     

   /*  Find out the number of palette entries on this
    *  device.
    */

   nColors = GetDeviceCaps(hDC, SIZEPALETTE);

   /*  For non-palette devices, we'll use the # of system
    *  colors for our palette size.
    */
   if (!nColors)
      nColors = GetDeviceCaps(hDC, NUMCOLORS);   

   return nColors;
}

/******************************************************************************
 *                                                                            *
 * HPALETTE CreatePaletteFromRGBQUAD(LPRGBQUAD rgbqPalette, WORD wEntries)    *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * LPRGBQUAD        - pointer to RGBQUADs                                     *
 * WORD             - the number of RGBQUADs we are pointing to               *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HPALETTE         - returns a handle to a palette or NULL if it failes      *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function will build a valid HPALETTE when given an array of RGBQUADs  *
 *                                                                            *
 *****************************************************************************/
HPALETTE CreatePaletteFromRGBQUAD(LPRGBQUAD rgbqPalette, WORD wEntries)
{ 
    HPALETTE hPal;
    LPLOGPALETTE lplgPal;
    PALETTEENTRY pe[256];
    int i;

    lplgPal = (LPLOGPALETTE)GlobalAlloc(GPTR, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * wEntries); 
    if (!lplgPal)
      return NULL;

    lplgPal->palVersion = PALVERSION;
    lplgPal->palNumEntries = wEntries;

    for (i=0; i<wEntries; i++) {
      lplgPal->palPalEntry[i].peRed   = rgbqPalette[i].rgbRed;
      lplgPal->palPalEntry[i].peGreen = rgbqPalette[i].rgbGreen;
      lplgPal->palPalEntry[i].peBlue  = rgbqPalette[i].rgbBlue;
      lplgPal->palPalEntry[i].peFlags = 0;
    }

    CopyMemory(pe, lplgPal->palPalEntry, wEntries * sizeof(PALETTEENTRY));

    hPal = CreatePalette(lplgPal);
    GlobalFree(lplgPal);

    return hPal;
}

/******************************************************************************
 *                                                                            *
 * WORD CreateRGBQUADFromPalette(LPRGBQUAD rgbqPalette, HPALETTE hPal);       *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * LPRGBQUAD        - pointer to RGBQUADs                                     *
 * HPALETTE         - HANDLE of Palette                                       *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * WORD             - returns the number of palette entries copied into the   *
 *                    LPRGBQUAD array                                         *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function will fill an array of RGBQUAD from given a palette handle    *
 *                                                                            *
 *****************************************************************************/
WORD CreateRGBQUADFromPalette(LPRGBQUAD rgbqPalette, HPALETTE hPal)
{ 
    int i;
    WORD wEntries;
  
    GetObject(hPal, sizeof(wEntries), &wEntries);

    GetPaletteEntries(hPal, 0, wEntries, (LPPALETTEENTRY)rgbqPalette);

    for (i=0; i<wEntries; i++) {
      rgbqPalette[i].rgbReserved = rgbqPalette[i].rgbRed;
      rgbqPalette[i].rgbRed   = rgbqPalette[i].rgbBlue;
      rgbqPalette[i].rgbBlue  = rgbqPalette[i].rgbReserved;
      rgbqPalette[i].rgbReserved = 0;
    }

    return wEntries;
}

/******************************************************************************  
 *                                                                            *  
 * GetSystemPalette()                                                         *  
 *                                                                            *  
 * Parameters:                                                                *  
 *                                                                            *  
 * None                                                                       *  
 *                                                                            *  
 * Return Value:                                                              *  
 *                                                                            *  
 * HPALETTE         - handle to a copy of the current system palette          *  
 *                                                                            *  
 * Description:                                                               *  
 *                                                                            *  
 * This function returns a handle to a palette which represents the system    *  
 * palette.  The system RGB values are copied into our logical palette        *  
 * using the GetSystemPaletteEntries function.                                *  
 *                                                                            *  
 * History:                                                                   *  
 *                                                                            *  
 *    Date      Author               Reason                                   *
 *    6/01/91   Garrett McAuliffe    Created                                  *
 *    9/15/91   Patrick Schreiber    Added header and comments                *
 *    12/20/91  Mark Bader           Added GetSystemPaletteEntries call       *
 *                                                                            *
 *****************************************************************************/  
HPALETTE GetSystemPalette()
{
   HDC hDC;                // handle to a DC
   static HPALETTE hPal = NULL;   // handle to a palette
   HANDLE hLogPal;         // handle to a logical palette
   LPLOGPALETTE lpLogPal;  // pointer to a logical palette
   int nColors;            // number of colors

   /* Find out how many palette entries we want. */

   hDC = GetDC(NULL);
   if (!hDC)
      return NULL;
  
   if (!(GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE)) {
     ReleaseDC(NULL, hDC);
     return NULL;
   }

   nColors = PalEntriesOnDevice(hDC);   // Number of palette entries

   /* Allocate room for the palette */
   hLogPal = GlobalAlloc(GPTR, sizeof(LOGPALETTE) + nColors * sizeof(PALETTEENTRY));

   /* if we didn't get a logical palette, return NULL */
   if (!hLogPal)
      return NULL;

   /* get a pointer to the logical palette */
   lpLogPal = (LPLOGPALETTE)hLogPal;

   /* set some important fields */
   lpLogPal->palVersion = PALVERSION;
   lpLogPal->palNumEntries = nColors;

   /* Copy the current system palette into our logical palette */

   GetSystemPaletteEntries(hDC, 0, nColors, 
                           (LPPALETTEENTRY)(lpLogPal->palPalEntry));

   /*  Go ahead and create the palette.  Once it's created, 
    *  we no longer need the LOGPALETTE, so free it.
    */

   hPal = CreatePalette(lpLogPal);

   /* clean up */
   GlobalFree(hLogPal);
   ReleaseDC(NULL, hDC);

   return hPal;
}

/******************************************************************************
 *                                                                            *
 * HPALETTE CreateSpectrumPalette()                                           *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * (none)                                                                     *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HPALETTE         - Returns a handle to a spectrum palette or NULL if it    *
 *                    fails.                                                  *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function will build a palette with a spectrum of colors.  It is       *
 * useful when you want to display a number of DIBs each with a different     *
 * palette yet still have an a good selection of colors for the DIBs to map   *
 * to.                                                                        *
 *                                                                            *
 *****************************************************************************/
HPALETTE CreateSpectrumPalette() 
{                 
    HPALETTE hPal;
    LPLOGPALETTE lplgPal;
    BYTE red, green, blue;
    int i; 
    
    lplgPal = (LPLOGPALETTE)GlobalAlloc(GPTR, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * MAXPALETTE); 
    if (!lplgPal)
      return NULL;
    
    lplgPal->palVersion = PALVERSION;
    lplgPal->palNumEntries = MAXPALETTE;
     
    red = green = blue = 0;
    for (i = 0; i < MAXPALETTE; i++) {
        lplgPal->palPalEntry[i].peRed   = red;
        lplgPal->palPalEntry[i].peGreen = green;
        lplgPal->palPalEntry[i].peBlue  = blue;
        lplgPal->palPalEntry[i].peFlags = (BYTE)0;

        if (!(red += 32))
            if (!(green += 32))
                blue += 64;
    }

    hPal = CreatePalette(lplgPal);
    GlobalFree(lplgPal);

    return hPal;
}

/******************************************************************************
 *                                                                            *
 * HANDLE CopyPalette(HPALETTE hPal)                                          *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HPALETTE         - The handle to the palette you want to make a copy of.   *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HPALETTE         - A copy of the palette passed as a parameter             *  
 *                                                                            *
 *****************************************************************************/
HPALETTE CopyPalette(HPALETTE hPal)
{
  HPALETTE hPalNew; 
  LPLOGPALETTE lplgPal;
  WORD iNumEntries;
  
  GetObject(hPal, sizeof(iNumEntries), &iNumEntries);
    
  lplgPal = (LPLOGPALETTE)GlobalAlloc(GPTR, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * iNumEntries); 
  if (!lplgPal)
    return NULL;

  lplgPal->palVersion = PALVERSION;
  lplgPal->palNumEntries = GetPaletteEntries(hPal, 0, iNumEntries, lplgPal->palPalEntry);
  
  hPalNew = CreatePalette(lplgPal);
  GlobalFree(lplgPal);

  return (hPalNew);
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : CopyPaletteEx(HPALETTE hPal, BYTE bFlags)                    *
 *                                                                            *
 *  PURPOSE    : Given a handle to a palette, this function return a copy of  *
 *               the palette with all of the flags set to a the specified     *
 *               value.											              *
 *                                                                            *
 *  RETURNS    : A handle to the palette.                                     *
 *                                                                            *
 *****************************************************************************/
HPALETTE CopyPaletteEx(HPALETTE hPal, BYTE bFlag)
{
   int i;
   WORD wNumColors;
   PALETTEENTRY pe[MAXPALETTE];
   HPALETTE hNewPal;

   if (!hPal || !GetObject(hPal, sizeof(wNumColors), &wNumColors))
     return NULL;
   
   hNewPal = CopyPalette(hPal);
   GetPaletteEntries(hNewPal, 0, wNumColors, (LPPALETTEENTRY)&pe);

   for (i=0; i<(int)wNumColors; i++) 
      pe[i].peFlags = bFlag;
   
   SetPaletteEntries(hNewPal, 0, wNumColors, (LPPALETTEENTRY)&pe);

   return hNewPal;
}


/******************************************************************************
 *                                                                            *
 *  HPALETTE CreateIdentityPalette(LPRGBQUAD lprgbq, int nColors)             *
 *                                                                            *
 * LPRGBQUAD        - pointer to RGBQUADs                                     *
 * int              - the number of RGBQUADs we are pointing to               *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HPALETTE         - returns a handle to an identity palette or NULL if it   *
 *                    fails                                                   *
 *                                                                            *
 * *Note: This code was originally taken from the WinG online docs            *
 *****************************************************************************/
HPALETTE CreateIdentityPalette(LPRGBQUAD lprgbq, int nColors)
{
	int i;
	struct {
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[MAXPALETTE];
	} Palette =
	{
		0x300, 
		MAXPALETTE
	};

	//*** Just use the screen DC where we need it
	HDC hDC = GetDC(NULL);

	//*** For SYSPAL_NOSTATIC, just copy the color table into
	//*** a PALETTEENTRY array and replace the first and last entries
	//*** with black and white
	if (GetSystemPaletteUse(hDC) == SYSPAL_NOSTATIC)

	{
		//*** Fill in the palette with the given values, marking each
		//*** as PC_NOCOLLAPSE
		for(i = 0; i < nColors; i++)
		{
			Palette.aEntries[i].peRed = lprgbq[i].rgbRed;
			Palette.aEntries[i].peGreen = lprgbq[i].rgbGreen;
			Palette.aEntries[i].peBlue = lprgbq[i].rgbBlue;
			Palette.aEntries[i].peFlags = PC_NOCOLLAPSE;
		}

		//*** Mark any unused entries PC_NOCOLLAPSE
		for (; i < MAXPALETTE; ++i)
			Palette.aEntries[i].peFlags = PC_NOCOLLAPSE;

		//*** Make sure the last entry is white
		//*** This may replace an entry in the array!
		Palette.aEntries[255].peRed = 255;
		Palette.aEntries[255].peGreen = 255;
		Palette.aEntries[255].peBlue = 255;
		Palette.aEntries[255].peFlags = 0;

		//*** And the first is black
		//*** This may replace an entry in the array!
		Palette.aEntries[0].peRed = 0;
		Palette.aEntries[0].peGreen = 0;
		Palette.aEntries[0].peBlue = 0;
		Palette.aEntries[0].peFlags = 0;

	}
	else
	//*** For SYSPAL_STATIC, get the twenty static colors into
	//*** the array, then fill in the empty spaces with the
	//*** given color table
	{
		int nStaticColors;
		int nUsableColors;

		//*** Get the static colors from the system palette
		nStaticColors = GetDeviceCaps(hDC, NUMCOLORS);
		GetSystemPaletteEntries(hDC, 0, MAXPALETTE, Palette.aEntries);

		//*** Set the peFlags of the lower static colors to zero
		nStaticColors = nStaticColors / 2;

		for (i=0; i<nStaticColors; i++)
			Palette.aEntries[i].peFlags = 0;

		//*** Fill in the entries from the given color table
		nUsableColors = nColors - nStaticColors;   
		for (; i<nUsableColors; i++)
		{
			Palette.aEntries[i].peRed = lprgbq[i].rgbRed;
			Palette.aEntries[i].peGreen = lprgbq[i].rgbGreen;
			Palette.aEntries[i].peBlue = lprgbq[i].rgbBlue;
			Palette.aEntries[i].peFlags = PC_NOCOLLAPSE;
		}

		//*** Mark any empty entries as PC_NOCOLLAPSE
        for (; i<MAXPALETTE - nStaticColors; i++)
			Palette.aEntries[i].peFlags = PC_NOCOLLAPSE;

		//*** Set the peFlags of the upper static colors to zero
		for (i = MAXPALETTE - nStaticColors; i<MAXPALETTE; i++)
			Palette.aEntries[i].peFlags = 0;
	}

	//*** Remember to release the DC!
	ReleaseDC(NULL, hDC);

	//*** Return the palette
	return CreatePalette((LPLOGPALETTE)&Palette);
}

/******************************************************************************
 *                                                                            *
 *  void ClearSystemPalette(void)                                             *
 *                                                                            *
 * Purpose:  Clears the system palette by realizing an all black palette.     *
 *           This will ensure that palette-managed applications executed      *
 *           before your application will not affect the identity mapping of  *
 *           your palette.                                                    * 
 *                                                                            *
 *                                                                            *
 * *Note: This code was originally taken from the WinG online docs            *
 *****************************************************************************/
void ClearSystemPalette(void)
{
	//*** A dummy palette setup
	struct
	{
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[MAXPALETTE];
	} Palette =
	{
		0x300, 
		MAXPALETTE
	};

	HPALETTE ScreenPalette = 0;
	HDC ScreenDC;
	int Counter;

	//*** Reset everything in the system palette to black
	for (Counter = 0; Counter < MAXPALETTE; Counter++)
	{
		Palette.aEntries[Counter].peRed = 0;
		Palette.aEntries[Counter].peGreen = 0;
		Palette.aEntries[Counter].peBlue = 0;
		Palette.aEntries[Counter].peFlags = PC_NOCOLLAPSE;
	}

	//*** Create, select, realize, deselect, and delete the palette
	ScreenDC = GetDC(NULL);
	ScreenPalette = CreatePalette((LPLOGPALETTE)&Palette);
	if (ScreenPalette)
	{
		ScreenPalette = SelectPalette(ScreenDC, ScreenPalette, FALSE);
		RealizePalette(ScreenDC);
		ScreenPalette = SelectPalette(ScreenDC, ScreenPalette, FALSE);
		DeleteObject(ScreenPalette);
	}
	ReleaseDC(NULL, ScreenDC);
}

