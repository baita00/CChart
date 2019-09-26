#define MAKE556WORD(r,g,b) ((((WORD)(r) >> 3) << 11) | (((WORD)(g) >> 3) << 6)  | ((WORD)(b) >> 2))    
#define MAKE565WORD(r,g,b) ((((WORD)(r) >> 3) << 11) | (((WORD)(g) >> 2) << 5)  | ((WORD)(b) >> 3))    
#define MAKE555WORD(r,g,b) ((((WORD)(r) >> 3) << 10) | (((WORD)(g) >> 3) << 5)  | ((WORD)(b) >> 3)) 

#define MAKE565RGB(w,r,g,b) (r = (w & 0xF800) >> 8, g = (w & 0x07E0) >> 3, b = (w & 0x001F) << 3) 
#define MAKE555RGB(w,r,g,b) (r = (w & 0x7C00) >> 7, g = (w & 0x03E0) >> 2, b = (w & 0x001F) << 3) 

/* Macro to swap two values */
#define SWAP(x, y)   ((x)^=(y)^=(x)^=(y))

/* Macro to determine the number of bytes in a DWORD aligned DIB scanline */
#define BYTESPERLINE(Width, BPP) ((WORD)((((DWORD)(Width) * (DWORD)(BPP) + 31) >> 5)) << 2)

BOOL DIBSecBitBlt(HDC, int, int, int, int, HDC, int, int, DWORD);
BOOL DIBSecStretchBlt(HDC, int, int, int, int, HDC,  int, int, int, int, DWORD);
