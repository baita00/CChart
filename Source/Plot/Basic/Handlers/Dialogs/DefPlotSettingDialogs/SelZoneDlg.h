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
void	SetupDataForSelZone(HWND hWnd, PlotImplT *plot)
{
	plot->InitializeSelZone();
	
	tostringstream ostr;
	tstring str;
	
	SendDlgItemMessage(hWnd, IDC_PS_ZONE_XSELECT, BM_SETCHECK, plot->IsEnableXZone(), 0);
	ostr.str(_TEXT(""));
	ostr<<plot->GetXZoneLower();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_XZONE_START, WM_SETTEXT, 0, (LPARAM)str.c_str());
	ostr.str(_TEXT(""));
	ostr<<plot->GetXZoneUpper();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_XZONE_END, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	SendDlgItemMessage(hWnd, IDC_PS_ZONE_YSELECT, BM_SETCHECK, plot->IsEnableYZone(), 0);
	ostr.str(_TEXT(""));
	ostr<<plot->GetYZoneLower();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_YZONE_START, WM_SETTEXT, 0, (LPARAM)str.c_str());
	ostr.str(_TEXT(""));
	ostr<<plot->GetYZoneUpper();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_YZONE_END, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_SELZONE, WM_SETCOLOR, 0, (LPARAM)plot->GetZoneColor());
}

template<class PlotImplT>
void	SaveDataForSelZone(HWND hWnd, PlotImplT *plot)
{
	TCHAR strtmp[nMaxString];
#ifndef _WIN64
	int			check;
#else
	LRESULT		check;
#endif
	COLORREF color;
	double	fVal;
	
	check = SendDlgItemMessage(hWnd, IDC_PS_ZONE_XSELECT, BM_GETCHECK, 0, 0);
	plot->SetEnableXZone(check==BST_CHECKED);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_XZONE_START), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	plot->SetXZoneLower(fVal);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_XZONE_END), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	plot->SetXZoneUpper(fVal);
	
	check = SendDlgItemMessage(hWnd, IDC_PS_ZONE_YSELECT, BM_GETCHECK, 0, 0);
	plot->SetEnableYZone(check==BST_CHECKED);
	
	GetWindowText(GetDlgItem(hWnd, IDC_PS_YZONE_START), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	plot->SetYZoneLower(fVal);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_YZONE_END), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	plot->SetYZoneUpper(fVal);
	
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_SELZONE, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetZoneColor(color);
}

template<class PlotImplT>
class ClassForPlotSelZoneDlg
{
public:
#ifndef _WIN64
	static	BOOL	CALLBACK PlotSelZoneDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT	CALLBACK PlotSelZoneDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static PlotImplT	*plot;	
		
		LPNMHDR pnmh;
		
		switch(message)
		{
		case WM_INITDIALOG:
			plot = (PlotImplT *)((LPPROPSHEETPAGE)lparam)->lParam;
			// Deal with the title
			SetupDataForSelZone(hWnd, (CXYPlotImplI *)plot);
			return TRUE;		
		case WM_NOTIFY:
			pnmh = (LPNMHDR) lparam;
			switch(pnmh->code)
			{
			case PSN_APPLY:
				// Apply Changes
				if(IsWindowVisible(hWnd))
					SaveDataForSelZone(hWnd, (CXYPlotImplI *)plot);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End
