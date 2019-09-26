/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2019 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/
////////////////////////////////////////////////////////////////////////////////
//                                  版权申明                                  //
//                         版权所有(C)2006-2019，杨国君                       //
//                                保留全部权利                                //
////////////////////////////////////////////////////////////////////////////////
/* ############################################################################################################################## */

#include "../../Resources/PlotResources.h"
#include "DefOperation.h"
#include "../XYPlotImpl.h"
#include "../../Accessary/Dib/dib.h"


#include <sstream>
#include <windows.h>
#include <commdlg.h>

Declare_Namespace_CChart

// Returns a string containing the name and font size of
// the passed font
tstring	GetFontDescription( LOGFONT *font )
{
	if( font == NULL ) return _TEXT("");

	tostringstream ostr;
	ostr<<font->lfHeight<<_TEXT(" point ")<<font->lfFaceName;
	return ostr.str();
}

UINT	GetPopupCheckedFlag( BOOL how )
{
	if( how ) return MF_CHECKED;
	else return MF_UNCHECKED;
}

bool	ShowFileDialog(HWND hWnd, bool open, tstring &filename, tstring &fileext, tstring &filepath, LPCTSTR filter, LPCTSTR defExt)
{
	OPENFILENAME ofn;       // common dialog box structure
	const int	nMaxFile = 1024;
	TCHAR szFile[nMaxFile]={0};       // buffer for file name

	RtlZeroMemory (&ofn , sizeof (ofn ));
	ofn . lStructSize = sizeof (ofn );
	ofn . hwndOwner = 0 ;
	ofn . lpstrFilter = filter;
	ofn . lpstrFile = szFile ;
	ofn . nMaxFile = nMaxFile ;
	ofn . lpstrDefExt = defExt; 
	ofn . Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST |OFN_PATHMUSTEXIST ;

	
	//OleInitialize(NULL);
	if(open)
	{
		if( GetOpenFileName(&ofn) == TRUE)
		{
			filename = ofn.lpstrFile;
			fileext = filename.substr(ofn.nFileExtension);
			filepath = filename.substr(0, ofn.nFileOffset);
			return true;
		}
	}
	else
	{
		if( GetSaveFileName(&ofn) == TRUE)
		{
			filename = ofn.lpstrFile;
			fileext = filename.substr(ofn.nFileExtension);
			return true;
		}
	}

	// The following codes is for debugging
	UINT errCode = CommDlgExtendedError();
	
	if(errCode != 0)
	{
		tostringstream ostr;
		ostr<<_TEXT("Open File Dialog failed, Error code is ")<<errCode<<_TEXT(" .");
		tstring str = ostr.str();
		MessageBox(hWnd, str.c_str(),_TEXT("Error"), MB_OK);
		if(errCode == CDERR_INITIALIZATION)
		{
			MessageBox(hWnd, _TEXT("Initialization failed!!"), _TEXT("Error"), MB_OK);
		}
	}

	return false;
}

bool	ShowFontDialog(HWND hWnd, LOGFONT &logfont, COLORREF &color)
{
	CHOOSEFONT cf;

	ZeroMemory(&cf, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof (CHOOSEFONT);
	cf.hwndOwner = hWnd;
	cf.hDC = (HDC)NULL; 
	cf.lpLogFont = &logfont;
	cf.iPointSize = 0; 
	cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	cf.rgbColors = color;
	cf.lCustData = 0L;
	cf.lpfnHook = (LPCFHOOKPROC)NULL; 
	cf.lpTemplateName = _TEXT(""); 
	cf.hInstance = (HINSTANCE) NULL; 
	cf.lpszStyle = _TEXT(""); 
	cf.nFontType = SCREEN_FONTTYPE; 
	cf.nSizeMin = 0; 
	cf.nSizeMax = 0; 
	
	if (ChooseFont(&cf)==TRUE)
	{
		color = cf.rgbColors;
		return true;
	}
	else
	{
		return false;
	}
}

bool	ShowColorDialog(HWND hWnd, COLORREF &cr)
{
	CHOOSECOLOR cc;
	COLORREF crCustColors[16];
	cc.lStructSize=sizeof(CHOOSECOLOR);
	cc.hwndOwner=hWnd;
	cc.hInstance=NULL;
	cc.rgbResult=cr;
	cc.lpCustColors =crCustColors;
	cc.Flags=CC_RGBINIT|CC_FULLOPEN|CC_ANYCOLOR;
	cc.lCustData=0;
	cc.lpfnHook=NULL;
	cc.lpTemplateName=NULL;
	BOOL ret=ChooseColor(&cc);
	if(ret)cr=cc.rgbResult;
	return ret==TRUE;
}

bool	ShowPrintDialog(HWND hWnd, bool stretch)
{
	DOCINFO docInfo;
	ZeroMemory(&docInfo, sizeof(docInfo));
	docInfo.cbSize = sizeof(docInfo);
	docInfo.lpszDocName = _TEXT("CChartPrint");

	PRINTDLG pd;
	ZeroMemory(&pd, sizeof(PRINTDLG));
	pd.lStructSize = sizeof(PRINTDLG);
	pd.hwndOwner   = hWnd;
	pd.hDevMode    = NULL;     // Don't forget to free or store hDevMode
	pd.hDevNames   = NULL;     // Don't forget to free or store hDevNames
	pd.Flags       = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC; 
	pd.nCopies     = 1;
	pd.nFromPage   = 0xFFFF; 
	pd.nToPage     = 0xFFFF; 
	pd.nMinPage    = 1; 
	pd.nMaxPage    = 0xFFFF; 
	
	HBITMAP hBitmap = HBitmapFromWnd(hWnd);
	if (PrintDlg(&pd)==TRUE) 
	{		
		int prHeight,prWidth;
		
		HDC printDC = pd.hDC;
		prWidth = GetDeviceCaps(printDC, HORZRES);
		prHeight = GetDeviceCaps(printDC, VERTRES);
		
		StartDoc(printDC, &docInfo);
		StartPage(printDC);
	
		BITMAP bm;
		GetObject(hBitmap, sizeof(BITMAP), &bm);
		
		HANDLE hDib;
		hDib = DIBFromBitmap(hBitmap, 0, bm.bmBitsPixel, 0);
		if(hDib)
		{
			LPBITMAPINFOHEADER        lpbi;			
			int nColors = (1 << (bm.bmPlanes * bm.bmBitsPixel) );
			if( nColors > 256 )	nColors = 0;
			
			int width, height;

			width = prWidth;
			height = prHeight;
			
			if(!stretch)
			{
				if(prWidth*bm.bmHeight > prHeight*bm.bmWidth)
				{
					width = height*bm.bmWidth/bm.bmHeight;
				}
				else
				{
					height = width*bm.bmHeight/bm.bmWidth;
				}
			}
			
			lpbi = (LPBITMAPINFOHEADER)hDib;
			StretchDIBits(printDC, 0, 0, width, height, 0, 0, bm.bmWidth, bm.bmHeight,(LPBYTE)lpbi + (sizeof(BITMAPINFOHEADER) +nColors *sizeof(RGBQUAD)),(LPBITMAPINFO)lpbi,DIB_RGB_COLORS,SRCCOPY);
			
			DeleteObject(hDib);
		}

		EndPage(printDC);
		EndDoc(printDC); 
	}
	DeleteObject(hBitmap);
	if(pd.hDevMode) 
		GlobalFree(pd.hDevMode);
	if(pd.hDevNames) 
		GlobalFree(pd.hDevNames);
	if(pd.hDC)
		DeleteDC(pd.hDC); 

	return true;
}

Declare_Namespace_End