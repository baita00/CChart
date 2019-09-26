 
/* Macro to restrict a given value to an upper or lower boundary value */
#define BOUND(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/* Macro to swap two values */
#define SWAP(x, y)   ((x)^=(y)^=(x)^=(y))

/* Macros to display/remove hourglass cursor for lengthy operations */
#define StartWait() hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT))
#define EndWait()   SetCursor(hcurSave)

#define NEW_DIB_FORMAT(lpbih) (lpbih->biSize != sizeof(BITMAPCOREHEADER))

/* Macro to align given value to the closest DWORD (unsigned long) */
#define ALIGNULONG(i)   ((((i)+3) >> 2) << 2)

/* Macro to determine to round off the given value to the closest byte */
#define WIDTHBYTES(i)   ((((i)+31) >> 5) << 2)

/* Macro to determine the number of bytes in a DWORD aligned DIB scanline */
#define BYTESPERLINE(Width, BPP) ((WORD)((((DWORD)(Width) * (DWORD)(BPP) + 31) >> 5)) << 2)

/* Raise a generic exception so that our hadler gets called */
#define RAISE_AN_EXCEPTION() RaiseException(0xE0000001, 0, 0, 0)

#define MAKE565WORD(r,g,b) ((((WORD)(r) >> 3) << 11) | (((WORD)(g) >> 2) << 5)  | ((WORD)(b) >> 3))    

#define MAXPALETTE      256       /* max. # supported palette entries */

/* Header signatutes for various resources */
#define BFT_ICON   0x4349   /* 'IC' */
#define BFT_BITMAP 0x4d42   /* 'BM' */
#define BFT_CURSOR 0x5450   /* 'PT' */

/* macro to determine if resource is a DIB */
#define ISDIB(bft) ((bft) == BFT_BITMAP)

/***************** FUNCTION DECLARATIONS *******************/

/***********************************************************/
/* Declarations of functions used in dib.c module          */
/***********************************************************/

HANDLE      OpenDIB (LPCSTR szFile);
BOOL        WriteDIB (LPCSTR, HANDLE);
BOOL        WriteDIBEx(LPCSTR, HANDLE, WORD, DWORD);
WORD        ColorTableSize (LPVOID);
WORD        DIBNumColors (LPVOID);
HPALETTE    CreateOptimizedPalette(LPBITMAPINFOHEADER, LPDWORD, LPVOID);
HPALETTE    CreateDIBPalette (HANDLE);
HPALETTE    CreateBIPalette (LPBITMAPINFOHEADER);
HANDLE      DIBFromBitmap (HBITMAP, DWORD, WORD, HPALETTE);
HBITMAP     BitmapFromDIB (HANDLE, HPALETTE);
BOOL        DIBBlt (HDC, INT, INT, INT, INT, HANDLE, INT, INT, LONG);
BOOL        StretchDIBBlt (HDC, INT, INT, INT, INT, HANDLE, INT, INT, INT, INT, LONG);
BOOL        DIBInfo (HANDLE hdib, LPBITMAPINFOHEADER);
HANDLE      ReadDIBBitmapInfo (INT);
BOOL        DrawBitmap (HDC, INT, INT, HBITMAP, DWORD);
VOID        ReadPackedFileHeader(HFILE, LPBITMAPFILEHEADER, LPDWORD);
VOID        WritePackedFileHeader(HFILE, LPBITMAPFILEHEADER);
HANDLE 		ChangeDIBFormat(HANDLE, WORD, DWORD);
BOOL        ChangeDIBPalette(HANDLE, HPALETTE);
HANDLE 		CopyDIB(HANDLE);
HBITMAP     CopyBitmap(HBITMAP);
HBITMAP     CropBitmap(HBITMAP, LPRECT);
BOOL 		CopyDIBData(HANDLE, HANDLE);
HANDLE      GetDIBFromClipboard(HWND);
BOOL 		GetDIBResolution(HANDLE, LPLONG, LPLONG);
DWORD       GetDIBCompression(HANDLE);
WORD        GetDIBBitCount(HANDLE);
HPALETTE    CreateDIBPaletteEx(HANDLE, BYTE);
DWORD 		GetDIBColorUsed(HANDLE);
BOOL 		GetDIBPointers(HANDLE, LPVOID*, LPVOID*, LPVOID*);
HANDLE 		CreateRGBDIB(DWORD, DWORD, WORD, DWORD);

/*****************************************************/
/* make new sizeof structs to cover dword alignment  */
/*****************************************************/

#define SIZEOF_BITMAPFILEHEADER_PACKED  (   \
    sizeof(WORD) +      /* bfType      */   \
    sizeof(DWORD) +     /* bfSize      */   \
    sizeof(WORD) +      /* bfReserved1 */   \
    sizeof(WORD) +      /* bfReserved2 */   \
    sizeof(DWORD))      /* bfOffBits   */
