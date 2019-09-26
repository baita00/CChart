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
void SetupDataForColorBar(HWND hWnd, PlotImplT *plot, int location)
{
	CAxisImpl *axis = plot->GetAxisByLocation(location);
	
	tostringstream ostr;
	tstring str;
	
	SendDlgItemMessage(hWnd, IDC_AS_COLORBAR_SHOWCOLORBAR, BM_SETCHECK, axis->IsShowColorBar(), 0);
	
	ostr<<axis->GetColorBarWidth();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_AS_COLORBAR_WIDTH, WM_SETTEXT, 0, (LPARAM)str.c_str());
	ostr.str(_TEXT(""));
	ostr<<axis->GetColorBarMarginOuter();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_AS_COLORBAR_MARGINOUTER, WM_SETTEXT, 0, (LPARAM)str.c_str());
	ostr.str(_TEXT(""));
	ostr<<axis->GetColorBarMarginInner();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_AS_COLORBAR_MARGININNER, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	SendDlgItemMessage(hWnd, IDC_AS_COLORBAR_COLOR1, WM_SETCOLOR, 0, (LPARAM)axis->GetBarColor1());
	SendDlgItemMessage(hWnd, IDC_AS_COLORBAR_COLOR2, WM_SETCOLOR, 0, (LPARAM)axis->GetBarColor2());
}

template<class PlotImplT>
void SaveDataForColorBar(HWND hWnd, PlotImplT *plot, int location)
{
	// local varibles
	TCHAR	strtmp[nMaxString];
	CAxisImpl	*axis = plot->GetAxisByLocation(location);
	
	COLORREF color;
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif
	
	check = SendDlgItemMessage(hWnd, IDC_AS_COLORBAR_SHOWCOLORBAR, BM_GETCHECK, 0, 0);
	axis->SetShowColorBar(check==BST_CHECKED);
	
	GetWindowText(GetDlgItem(hWnd, IDC_AS_COLORBAR_WIDTH), strtmp, nMaxString);
	axis->SetColorBarWidth(_ttoi(strtmp));
	GetWindowText(GetDlgItem(hWnd, IDC_AS_COLORBAR_MARGINOUTER), strtmp, nMaxString);
	axis->SetColorBarMarginOuter(_ttoi(strtmp));
	GetWindowText(GetDlgItem(hWnd, IDC_AS_COLORBAR_MARGININNER), strtmp, nMaxString);
	axis->SetColorBarMarginInner(_ttoi(strtmp));
	
	SendDlgItemMessage(hWnd, IDC_AS_COLORBAR_COLOR1, WM_GETCOLOR, 0, (LPARAM)&color); 
	axis->SetBarColor1(color);
	SendDlgItemMessage(hWnd, IDC_AS_COLORBAR_COLOR2, WM_GETCOLOR, 0, (LPARAM)&color); 
	axis->SetBarColor2(color); 
}

template<class PlotImplT>
class ClassForAxisColorBarDlg
{
public:
#ifndef _WIN64
	static BOOL CALLBACK AxisColorBarDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
		static LRESULT CALLBACK AxisColorBarDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static AxisSettingParam<PlotImplT> *para;
		static PlotImplT	*plot;
		static int location;
		
		switch(message)
		{
		case WM_INITDIALOG:
			para = (AxisSettingParam<PlotImplT> *)lparam;
			plot = para->plot;
			location = para->location;
			
			// Deal with the title
			SetupDataForColorBar(hWnd, plot, location);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForColorBar(hWnd, plot, location);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End
