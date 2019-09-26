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

Declare_Namespace_CChart

template<class PlotImplT>
void SetupDataForTitle(HWND hWnd, PlotImplT *plot, int location, LOGFONT &lfTitleFont)
{
	CAxis *axis = plot->GetAxisByLocation(location);
	
	// Deal with the title
	SendDlgItemMessage(hWnd, IDC_AS_PLOT_TITLE, WM_SETTEXT, 0, (LPARAM)axis->GetTitle().c_str());
	SendDlgItemMessage(hWnd, IDC_AS_COLOR_TITLE_FONT, WM_SETCOLOR, 0, (LPARAM)axis->GetTitleColor());
	lfTitleFont = axis->GetTitleFont();
	SendDlgItemMessage(hWnd, IDC_AS_TITLE_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &axis->GetTitleFont() ).c_str());
	SendDlgItemMessage(hWnd, IDC_AS_TITLE_SHOW, BM_SETCHECK, axis->IsShowTitle(), 0);
	SendDlgItemMessage(hWnd, IDC_AS_TITLE_YVERTICLE, BM_SETCHECK, axis->IsYTitleVerticle(), 0);
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		if(axis->IsXAxis())
		{
			SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Left"));
			SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Middle"));
			SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Right"));
		}
		else
		{
			SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Top"));
			SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Middle"));
			SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("Bottom"));
		}
		break;
	case kLangChinese:
		if(axis->IsXAxis())
		{
			SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("左"));
			SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("中"));
			SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("右"));
		}
		else
		{
			SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("上"));
			SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("中"));
			SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_ADDSTRING, 0, (LPARAM)_TEXT("下"));
		}
		break;
	}
	int pos = axis->GetTitlePosition(), d=axis->IsXAxis()?1:-1;
	SendMessage(GetDlgItem(hWnd,IDC_AS_TITLE_POSITION), CB_SETCURSEL, (WPARAM)(pos==0?1:(pos<0?1-d:1+d)), 0);
}

template<class PlotImplT>
void SaveDataForTitle(HWND hWnd, PlotImplT *plot, int location, LOGFONT lfTitleFont)
{
	// local varibles
	TCHAR	strtmp[nMaxString];
	CAxis	*axis = plot->GetAxisByLocation(location);
	COLORREF color;
#ifndef _WIN64
	int num, check;
#else
	LRESULT num, check;
#endif
	
	// Deal with the title
	GetWindowText(GetDlgItem(hWnd, IDC_AS_PLOT_TITLE), strtmp, nMaxString);
	axis->SetTitle(strtmp);
	SendDlgItemMessage(hWnd, IDC_AS_COLOR_TITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
	axis->SetTitleColor(color);
	axis->SetTitleFont(lfTitleFont);
	
	check = SendDlgItemMessage(hWnd, IDC_AS_TITLE_SHOW, BM_GETCHECK, 0, 0);
	axis->SetShowTitle(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_AS_TITLE_YVERTICLE, BM_GETCHECK, 0, 0);
	axis->SetYTitleVerticle(check==BST_CHECKED);
	
	num = SendDlgItemMessage(hWnd, IDC_AS_TITLE_POSITION, CB_GETCURSEL, 0, 0);
	axis->SetTitlePosition(axis->IsXAxis()?(int)num-1:1-(int)num);
}

template<class PlotImplT>
class	ClassForAxisTitleDlg
{
public:
#ifndef _WIN64
	static	BOOL CALLBACK AxisTitleDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT CALLBACK AxisTitleDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static AxisSettingParam<PlotImplT> *para;
		static PlotImplT	*plot;
		static int location;
		static LOGFONT	lfTitleFont;
		
		// local varibles
		HDC		hdc;
		LOGFONT	*pLogFont;
		COLORREF color;
		
		int oldHeight;
		
		switch(message)
		{
		case WM_INITDIALOG:
			para = (AxisSettingParam<PlotImplT> *)lparam;
			plot = para->plot;
			location = para->location;
			
			// Deal with the title
			SetupDataForTitle(hWnd, plot, location, lfTitleFont);
			return TRUE;
		case WM_COMMAND:
			
			// font buttons are clicked
			if(HIWORD (wparam) == BN_CLICKED)
			{
				if(LOWORD(wparam)==IDC_AS_CHOOSE_TITLE_FONT)
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
						
						SendDlgItemMessage(hWnd, IDC_AS_TITLE_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &lfTitleFont ).c_str());
						
					}
					
					ReleaseDC(hWnd, hdc);
				}
			}
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForTitle(hWnd, plot, location, lfTitleFont);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End