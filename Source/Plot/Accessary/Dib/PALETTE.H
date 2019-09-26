/*******************************************************************************
 *                                                                             *
 *  MODULE      : PALETTE.H                                                    *
 *                                                                             *
 *  DESCRIPTION : Routines for dealing with palettes                           *
 *                                                                             *
 ******************************************************************************/

#define PALVERSION      0x300
#define MAXPALETTE      256       /* max. # supported palette entries */

HPALETTE    CreatePaletteFromRGBQUAD(LPRGBQUAD, WORD);
WORD        CreateRGBQUADFromPalette(LPRGBQUAD, HPALETTE);
int         PalEntriesOnDevice(HDC);
HPALETTE    GetSystemPalette();
HPALETTE    CreateSpectrumPalette();  
HPALETTE    CopyPalette(HPALETTE);
HPALETTE    CopyPaletteEx(HPALETTE, BYTE);
HPALETTE    CreateIdentityPalette(LPRGBQUAD, int);
void        ClearSystemPalette(void);
