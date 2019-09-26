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
void	SetupDataForBkgnd(HWND hWnd, PlotImplT *plot)
{
	tostringstream ostr;
	tstring str;

	SendDlgItemMessage(hWnd, IDC_PS_COLOR_BACKGROUND, WM_SETCOLOR, 0, (LPARAM)plot->GetBkgndColor()); 
	
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_BORDER, WM_SETCOLOR, 0, (LPARAM)plot->GetBorderColor());
	SendDlgItemMessage(hWnd, IDC_PS_BORDER_SHOW, BM_SETCHECK, plot->IsBorderShow(), 0);
	ostr<<plot->GetBorderSize();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_BORDER_SIZE, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_PS_BACKGROUND_GRADIENT, BM_SETCHECK, plot->IsGradientBkgnd(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_BACKGROUND_GRADIENTTYPE, CB_RESETCONTENT, 0, 0);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Left to Right"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Top to Bottom"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("LT to RB"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("LB to RT"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Center to Bound"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Radial"));
		break;
	case kLangChinese:
		SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("从左到右"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("从上到下"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("左上到右下"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("左下到右上"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("中心到边界"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("径向"));
		break;
	}
	SendMessage(GetDlgItem(hWnd,IDC_PS_BACKGROUND_GRADIENTTYPE), CB_SETCURSEL, (WPARAM)plot->GetGradientType(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_BKGND_COLOR1, WM_SETCOLOR, 0, (LPARAM)plot->GetGradientColor1()); 
	SendDlgItemMessage(hWnd, IDC_PS_BKGND_COLOR2, WM_SETCOLOR, 0, (LPARAM)plot->GetGradientColor2()); 

	SendDlgItemMessage(hWnd, IDC_PS_DATAAREA_ENABLE, BM_SETCHECK, plot->IsUseDataRegionBrush(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_DATAAREA, WM_SETCOLOR, 0, (LPARAM)plot->GetDataRegionColor()); 

	SendDlgItemMessage(hWnd, IDC_PS_DATAAREA_GRADIENT, BM_SETCHECK, plot->IsGradientDataRegion(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_DATAAREA_GRADIENTTYPE, CB_RESETCONTENT, 0, 0);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Left to Right"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Top to Bottom"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("LT to RB"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("LB to RT"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Center to Bound"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Radial"));
		break;
	case kLangChinese:
		SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("从左到右"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("从上到下"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("左上到右下"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("左下到右上"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("中心到边界"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("径向"));
		break;
	}
	SendMessage(GetDlgItem(hWnd,IDC_PS_DATAAREA_GRADIENTTYPE), CB_SETCURSEL, (WPARAM)plot->GetDataRegionGradientType(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_DATAAREA_COLOR1, WM_SETCOLOR, 0, (LPARAM)plot->GetDataRegionGradientColor1()); 
	SendDlgItemMessage(hWnd, IDC_PS_DATAAREA_COLOR2, WM_SETCOLOR, 0, (LPARAM)plot->GetDataRegionGradientColor2()); 
}

template<class PlotImplT>
void	SaveDataForBkgnd(HWND hWnd, PlotImplT *plot)
{
#ifndef _WIN64
	int			check,num;
#else
	LRESULT		check,num;
#endif
	COLORREF color;
	TCHAR strtmp[nMaxString];

	SendDlgItemMessage(hWnd, IDC_PS_COLOR_BACKGROUND, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetBkgndColor(color);

	SendDlgItemMessage(hWnd, IDC_PS_COLOR_BORDER, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetBorderColor(color);

	check = SendDlgItemMessage(hWnd, IDC_PS_BORDER_SHOW, BM_GETCHECK, 0, 0);
	plot->SetBorderShow(check==BST_CHECKED);
	
	GetWindowText(GetDlgItem(hWnd, IDC_PS_BORDER_SIZE), strtmp, nMaxString);
	num=(int)_tcstod(strtmp, 0);
	plot->SetBorderSize((int)num);

	check = SendDlgItemMessage(hWnd, IDC_PS_BACKGROUND_GRADIENT, BM_GETCHECK, 0, 0);
	plot->SetGradientBkgnd(check==BST_CHECKED);
	num = SendDlgItemMessage(hWnd, IDC_PS_BACKGROUND_GRADIENTTYPE, CB_GETCURSEL, 0, 0);
	plot->SetGradientType((int)num);
	SendDlgItemMessage(hWnd, IDC_PS_BKGND_COLOR1, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetGradientColor1(color);
	SendDlgItemMessage(hWnd, IDC_PS_BKGND_COLOR2, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetGradientColor2(color);

	check = SendDlgItemMessage(hWnd, IDC_PS_DATAAREA_ENABLE, BM_GETCHECK, 0, 0);
	plot->SetUseDataRegionBrush(check==BST_CHECKED);

	SendDlgItemMessage(hWnd, IDC_PS_COLOR_DATAAREA, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetDataRegionColor(color);

	check = SendDlgItemMessage(hWnd, IDC_PS_DATAAREA_GRADIENT, BM_GETCHECK, 0, 0);
	plot->SetGradientDataRegion(check==BST_CHECKED);
	num = SendDlgItemMessage(hWnd, IDC_PS_DATAAREA_GRADIENTTYPE, CB_GETCURSEL, 0, 0);
	plot->SetDataRegionGradientType((int)num);
	SendDlgItemMessage(hWnd, IDC_PS_DATAAREA_COLOR1, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetDataRegionGradientColor1(color);
	SendDlgItemMessage(hWnd, IDC_PS_DATAAREA_COLOR2, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetDataRegionGradientColor2(color);
	
}

template<class PlotImplT>
class	ClassForPlotBkgndDlg
{
public:
#ifndef _WIN64
	static BOOL	CALLBACK PlotBkgndDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static LRESULT	CALLBACK PlotBkgndDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static PlotImplT	*plot;
		
		// local varibles
		
		LPNMHDR pnmh;
		
		switch(message)
		{
		case WM_INITDIALOG:
			plot = (PlotImplT *)((LPPROPSHEETPAGE)lparam)->lParam;
			SetupDataForBkgnd(hWnd,plot);
			
			return TRUE;
			
		case WM_NOTIFY:
			pnmh = (LPNMHDR) lparam;
			switch(pnmh->code)
			{
			case PSN_APPLY:
				// Apply Changes
				if(IsWindowVisible(hWnd))
					SaveDataForBkgnd(hWnd, plot);
				return TRUE;
			}
			break;
		}
		return FALSE;
	}
};

Declare_Namespace_End