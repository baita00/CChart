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
void SetupDataForColors(HWND hWnd, PlotImplT *plot, int location)
{
	CAxis *axis = plot->GetAxisByLocation(location);
	// Deal with the colors
	SendDlgItemMessage(hWnd, IDC_AS_COLOR_SAMECHECK, BM_SETCHECK, axis->IsSameColor(), 0);
	SendDlgItemMessage(hWnd, IDC_AS_COLOR_AXIS, WM_SETCOLOR, 0, (LPARAM)axis->GetAxisColor());
	
	tostringstream ostr;
	tstring str;
	ostr<<plot->GetAxisByLocation(location)->GetLineSize();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_AS_AXIS_SIZE, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_AS_AXIS_STYLE, CB_RESETCONTENT, 0, 0);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		SendMessage(GetDlgItem(hWnd,IDC_AS_AXIS_STYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Solid _______________________"));
		SendMessage(GetDlgItem(hWnd,IDC_AS_AXIS_STYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Dashed _ _ _ _ _ _ _ _ _ _ _ _ _ _"));
		SendMessage(GetDlgItem(hWnd,IDC_AS_AXIS_STYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Dotted . . . . . . . . . . . . . . . . . . . . . ."));
		SendMessage(GetDlgItem(hWnd,IDC_AS_AXIS_STYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Dash-Dot _ . _ . _ . _ . _ . _ . _ . _ ."));
		SendMessage(GetDlgItem(hWnd,IDC_AS_AXIS_STYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Dash-Dot-Dot _ . . _ . . _ . . _ . . _ ."));
		break;
	case kLangChinese:
		SendMessage(GetDlgItem(hWnd,IDC_AS_AXIS_STYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("实线 _______________________"));
		SendMessage(GetDlgItem(hWnd,IDC_AS_AXIS_STYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("虚划线 _ _ _ _ _ _ _ _ _ _ _ _ _ _"));
		SendMessage(GetDlgItem(hWnd,IDC_AS_AXIS_STYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("虚点线 . . . . . . . . . . . . . . . . . . . . . ."));
		SendMessage(GetDlgItem(hWnd,IDC_AS_AXIS_STYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("划点线 _ . _ . _ . _ . _ . _ . _ . _ ."));
		SendMessage(GetDlgItem(hWnd,IDC_AS_AXIS_STYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("划点点线 _ . . _ . . _ . . _ . . _ ."));
		break;
	}
	SendMessage(GetDlgItem(hWnd,IDC_AS_AXIS_STYLE), CB_SETCURSEL, (WPARAM)plot->GetAxisByLocation(location)->GetLineStyle(), 0);
}

template<class PlotImplT>
void SaveDataForColors(HWND hWnd, PlotImplT *plot, int location)
{
	// local varibles
	CAxis	*axis = plot->GetAxisByLocation(location);
	
	TCHAR strtmp[nMaxString];
	
	COLORREF color;
#ifndef _WIN64
	int num, check;
#else
	LRESULT num, check;
#endif
	
	
	// Deal with the colors
	check = SendDlgItemMessage(hWnd, IDC_AS_COLOR_SAMECHECK, BM_GETCHECK, 0, 0);
	axis->SetSameColor(check==BST_CHECKED);
	SendDlgItemMessage(hWnd, IDC_AS_COLOR_AXIS, WM_GETCOLOR, 0, (LPARAM)&color); 
	axis->SetAxisColor(color);
	
	GetWindowText(GetDlgItem(hWnd, IDC_AS_AXIS_SIZE), strtmp, nMaxString);
	plot->GetAxisByLocation(location)->SetLineSize(_ttoi(strtmp));
	num = SendDlgItemMessage(hWnd, IDC_AS_AXIS_STYLE, CB_GETCURSEL, 0, 0);
	plot->GetAxisByLocation(location)->SetLineStyle((int)num);
}

template<class PlotImplT>
class	ClassForAxisColorsDlg
{
public:
#ifndef _WIN64
	static	BOOL CALLBACK AxisColorsDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT CALLBACK AxisColorsDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
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
			SetupDataForColors(hWnd, plot, location);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForColors(hWnd, plot, location);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End
