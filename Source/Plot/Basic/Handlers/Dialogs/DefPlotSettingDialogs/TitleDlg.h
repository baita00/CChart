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

#pragma once

#include "../../../../Resources/PlotResources.h"
#include "../ColorControl.h"
#include "../../../CChartNameSpace.h"

Declare_Namespace_CChart

template<class PlotImplT>
void	SetupDataForTitle(HWND hWnd, PlotImplT *plot, LOGFONT	&lfTitleFont)
{
	SendDlgItemMessage(hWnd, IDC_PS_PLOT_TITLESHOW, BM_SETCHECK, plot->IsTitleShow(), 0);
	tstring title = plot->GetTitle();
	//replace_all_distinct(title, "\r\n", "\\r\\n");
	SendDlgItemMessage(hWnd, IDC_PS_PLOT_TITLE, WM_SETTEXT, 0, (LPARAM)title.c_str()); 
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_TITLE_FONT, WM_SETCOLOR, 0, (LPARAM)plot->GetTitleColor()); 
	lfTitleFont = plot->GetTitleFont();
	SendDlgItemMessage(hWnd, IDC_PS_TITLE_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &plot->GetTitleFont() ).c_str()); 
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		SendMessage(GetDlgItem(hWnd,IDC_PS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Left"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Middle"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Right"));
		break;
	case kLangChinese:
		SendMessage(GetDlgItem(hWnd,IDC_PS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("左"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("中"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("右"));
		break;
	}
	int pos = plot->GetTitleAlignment();
	SendMessage(GetDlgItem(hWnd,IDC_PS_TITLE_POSITION), CB_SETCURSEL, (WPARAM)(pos==0?1:(pos<0?0:2)), 0);
	
}

template<class PlotImplT>
void	SaveDataForTitle(HWND hWnd, PlotImplT *plot, LOGFONT	lfTitleFont)
{
	TCHAR strtmp[nMaxString];
#ifndef _WIN64
	int			num, check;
#else
	LRESULT		num, check;
#endif
	COLORREF color;
	
	check = SendDlgItemMessage(hWnd, IDC_PS_PLOT_TITLESHOW, BM_GETCHECK, 0, 0);
	plot->SetTitleShow(check==BST_CHECKED);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_PLOT_TITLE), strtmp, nMaxString);
	plot->SetTitle(strtmp);
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_TITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetTitleColor(color);
	plot->SetTitleFont(lfTitleFont);
	
	num = SendDlgItemMessage(hWnd, IDC_PS_TITLE_POSITION, CB_GETCURSEL, 0, 0);
	plot->SetTitleAlignment((int)num-1);
	
}

// templated function pointer should be wrapped in a class
template<class PlotImplT>
class	ClassForPlotTitleDlg
{
public:
#ifndef _WIN64
	static BOOL	CALLBACK PlotTitleDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static LRESULT	CALLBACK PlotTitleDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static PlotImplT	*plot;
		
		static LOGFONT	lfTitleFont;
		
		// local varibles
		int		oldHeight;
		
		HDC		hdc;
		LOGFONT	*pLogFont;
		COLORREF color=0;
		
		LPNMHDR pnmh;
		
		switch(message)
		{
		case WM_INITDIALOG:
			plot = (PlotImplT *)((LPPROPSHEETPAGE)lparam)->lParam;
			// Deal with the title
			SetupDataForTitle(hWnd, plot, lfTitleFont);
			return TRUE;
		case WM_COMMAND:
			
			// font buttons are clicked
			if(HIWORD (wparam) == BN_CLICKED)
			{
				if(LOWORD(wparam)==IDC_PS_CHOOSE_TITLE_FONT)
				{
					hdc = GetDC(hWnd);
					
					pLogFont = &lfTitleFont;
					SendDlgItemMessage(hWnd, IDC_PS_COLOR_TITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
					
					oldHeight = pLogFont->lfHeight;
					pLogFont->lfHeight = -MulDiv(oldHeight, GetDeviceCaps(hdc, LOGPIXELSY), 72);
					
					// Font Dialog				
					if( ShowFontDialog(hWnd, *pLogFont, color))
					{
						pLogFont->lfHeight = -MulDiv(pLogFont->lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY));
						
						SendDlgItemMessage(hWnd, IDC_PS_TITLE_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &lfTitleFont ).c_str());
						
					}
					
					ReleaseDC(hWnd, hdc);
				}
			}
			break;
			
		case WM_NOTIFY:
			pnmh = (LPNMHDR) lparam;
			switch(pnmh->code)
			{
			case PSN_APPLY:
				// Apply Changes
				if(IsWindowVisible(hWnd))
					SaveDataForTitle(hWnd, plot, lfTitleFont);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End

