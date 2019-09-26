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
void	SetupDataForLegend(HWND hWnd, PlotImplT *plot, LOGFONT	&lfLegendFont)
{
	tostringstream ostr;
	tstring str;

	SendDlgItemMessage(hWnd, IDC_PS_PLOT_LEGEND, BM_SETCHECK, plot->IsLegendShow(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_LEGEND_TRANSPARENT, BM_SETCHECK, plot->IsLegendTransparent(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_LEGEND_ROUNDCORNER, BM_SETCHECK, plot->IsLegendRound(), 0);
	lfLegendFont = plot->GetLegendFont();
	SendDlgItemMessage(hWnd, IDC_PS_LEGEND_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &plot->GetLegendFont() ).c_str());

	SendDlgItemMessage(hWnd, IDC_PS_LEGEND_SHOWBORDER, BM_SETCHECK, plot->IsLegendBorderShow(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_LEGEND_BORDERCOLOR, WM_SETCOLOR, 0, (LPARAM)plot->GetLegendBorderColor());
	ostr.str(_TEXT(""));
	ostr<<plot->GetLegendBorderLineSize();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_LEGEND_BORDERSIZE, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	SendDlgItemMessage(hWnd, IDC_PS_LEGEND_POSITION, CB_RESETCONTENT, 0, 0);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Arbitrary"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Inner Left"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Inner Left Top"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Inner Left Bottom"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Inner Right"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Inner Right Top"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Inner Right Bottom"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Inner Top"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Inner Bottom"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Left"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Left Top"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Left Bottom"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Right"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Right Top"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Right Bottom"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Top"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Top Left"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Top Right"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Bottom"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Bottom Left"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Bottom Right"));
		break;
	case kLangChinese:
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("任意位置"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("内左"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("内左上"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("内左下"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("内右"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("内右上"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("内右下"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("内上"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("内下"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("左"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("左上"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("左下"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("右"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("右上"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("右下"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("上"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("上左"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("上右"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("下"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("下左"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("下右"));
		break;
	}
	SendMessage(GetDlgItem(hWnd,IDC_PS_LEGEND_POSITION), CB_SETCURSEL, (WPARAM)plot->GetLegendPosition(), 0);

	ostr.str(_TEXT(""));
	ostr<<plot->GetHorizontalNumOfLegend();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_LEGEND_COLUMNS, WM_SETTEXT, 0, (LPARAM)str.c_str());
	ostr.str(_TEXT(""));
	ostr<<plot->GetMaxVerticalNumOfLegend();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_LEGEND_MAXCOLS, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
}

template<class PlotImplT>
void	SaveDataForLegend(HWND hWnd, PlotImplT *plot, LOGFONT	lfLegendFont)
{
#ifndef _WIN64
	int			check,num;
#else
	LRESULT		check,num;
#endif
	COLORREF color;
	TCHAR strtmp[nMaxString];

	check = SendDlgItemMessage(hWnd, IDC_PS_PLOT_LEGEND, BM_GETCHECK, 0, 0);
	plot->SetLegendShow(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_LEGEND_TRANSPARENT, BM_GETCHECK, 0, 0);
	plot->SetLegendTransparent(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_LEGEND_ROUNDCORNER, BM_GETCHECK, 0, 0);
	plot->SetLegendRound(check==BST_CHECKED);

	plot->SetLegendFont(lfLegendFont);

	check = SendDlgItemMessage(hWnd, IDC_PS_LEGEND_SHOWBORDER, BM_GETCHECK, 0, 0);
	plot->SetLegendBorderShow(check==BST_CHECKED);
	SendDlgItemMessage(hWnd, IDC_PS_LEGEND_BORDERCOLOR, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetLegendBorderColor(color);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_LEGEND_BORDERSIZE), strtmp, nMaxString);
	num =(int)_tcstod(strtmp, 0);
	plot->SetLegendBorderLizeSize((int)num);

	num = SendDlgItemMessage(hWnd, IDC_PS_LEGEND_POSITION, CB_GETCURSEL, 0, 0);
	plot->SetLegendPosition((int)num);

	GetWindowText(GetDlgItem(hWnd, IDC_PS_LEGEND_COLUMNS), strtmp, nMaxString);
	num =(int)_tcstod(strtmp, 0);
	plot->SetHorizontalNumOfLegend((int)num);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_LEGEND_MAXCOLS), strtmp, nMaxString);
	num =(int)_tcstod(strtmp, 0);
	plot->SetMaxVerticalNumOfLegend((int)num);
}

template<class PlotImplT>
class ClassForPlotLegendDlg
{
public:
#ifndef _WIN64
	static BOOL	CALLBACK PlotLegendDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static LRESULT	CALLBACK PlotLegendDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static PlotImplT	*plot;
		
		static LOGFONT	lfLegendFont;
		
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
			SetupDataForLegend(hWnd, plot, lfLegendFont);
			
			return TRUE;
		case WM_COMMAND:
			
			// font buttons are clicked
			if(HIWORD (wparam) == BN_CLICKED)
			{
				if(LOWORD(wparam)==IDC_PS_CHOOSE_LEGEND_FONT)
				{
					hdc = GetDC(hWnd);
					
					pLogFont = &lfLegendFont;
					SendDlgItemMessage(hWnd, IDC_PS_COLOR_TITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
					
					oldHeight = pLogFont->lfHeight;
					pLogFont->lfHeight = -MulDiv(oldHeight, GetDeviceCaps(hdc, LOGPIXELSY), 72);
					
					// Font Dialog				
					if( ShowFontDialog(hWnd, *pLogFont, color))
					{
						pLogFont->lfHeight = -MulDiv(pLogFont->lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY));
						
						SendDlgItemMessage(hWnd, IDC_PS_LEGEND_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &lfLegendFont ).c_str());
						
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
					SaveDataForLegend(hWnd, plot, lfLegendFont);
				return TRUE;
			}
			break;
		}
		return FALSE;
	}
};

Declare_Namespace_End
