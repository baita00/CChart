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

extern double tlScale;

template<class PlotImplT>
void SetupDataForMajorTicks(HWND hWnd, PlotImplT *plot, int location)
{
	CAxisImpl *axis = plot->GetAxisByLocation(location);
	
	bool bX = axis->IsXAxis();
	
	tostringstream ostr;
	tstring str;
	
	// Deal with the major tick marks
	ostr<<(bX?plot->GetXTickCount():plot->GetYTickCount())+1;
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_AS_NUMBER_MAJOR_TICKS, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_AS_AUTO_MAJOR_TICKS, BM_SETCHECK, bX?plot->IsAutoMajorTickX():plot->IsAutoMajorTickY(), 0);
	ostr.str(_TEXT(""));
	ostr<<axis->GetTickLength()/tlScale;
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_AS_MAJOR_TICK_LENGTH, WM_SETTEXT, 0, (LPARAM)str.c_str());
	ostr.str(_TEXT(""));
	ostr<<axis->GetTickSize();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_AS_MAJOR_TICK_SIZE, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_AS_COLOR_MAJORTICK, WM_SETCOLOR, 0, (LPARAM)axis->GetTickColor());
	SendDlgItemMessage(hWnd, IDC_AS_MAJORTICK_POSITION, CB_RESETCONTENT, 0, 0);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		SendDlgItemMessage(hWnd, IDC_AS_MAJORTICK_POSITION, CB_ADDSTRING, 0, (LPARAM)_TEXT("None"));
		SendDlgItemMessage(hWnd, IDC_AS_MAJORTICK_POSITION, CB_ADDSTRING, 0, (LPARAM)_TEXT("Inner"));
		SendDlgItemMessage(hWnd, IDC_AS_MAJORTICK_POSITION, CB_ADDSTRING, 0, (LPARAM)_TEXT("Outer"));
		SendDlgItemMessage(hWnd, IDC_AS_MAJORTICK_POSITION, CB_ADDSTRING, 0, (LPARAM)_TEXT("Both"));
		break;
	case kLangChinese:
		SendDlgItemMessage(hWnd, IDC_AS_MAJORTICK_POSITION, CB_ADDSTRING, 0, (LPARAM)_TEXT("无"));
		SendDlgItemMessage(hWnd, IDC_AS_MAJORTICK_POSITION, CB_ADDSTRING, 0, (LPARAM)_TEXT("内部"));
		SendDlgItemMessage(hWnd, IDC_AS_MAJORTICK_POSITION, CB_ADDSTRING, 0, (LPARAM)_TEXT("外部"));
		SendDlgItemMessage(hWnd, IDC_AS_MAJORTICK_POSITION, CB_ADDSTRING, 0, (LPARAM)_TEXT("双边"));
		break;
	}
	SendDlgItemMessage(hWnd, IDC_AS_MAJORTICK_POSITION, CB_SETCURSEL, (WPARAM)axis->GetTickPosition(), 0);
}

template<class PlotImplT>
void SaveDataForMajorTicks(HWND hWnd, PlotImplT *plot, int location)
{
	// local varibles
	TCHAR	strtmp[nMaxString];
	CAxisImpl	*axis = plot->GetAxisByLocation(location);
	
	bool bX = axis->IsXAxis();
	
	COLORREF color;
#ifndef _WIN64
	int num, check;
#else
	LRESULT num, check;
#endif
	
	// Deal with the major tick marks
	GetWindowText(GetDlgItem(hWnd, IDC_AS_NUMBER_MAJOR_TICKS), strtmp, nMaxString);
	if(bX)plot->SetXTickCount(abs(_ttoi(strtmp))-1);
	else plot->SetYTickCount(abs(_ttoi(strtmp))-1);
	
	check = SendDlgItemMessage(hWnd, IDC_AS_AUTO_MAJOR_TICKS, BM_GETCHECK, 0, 0);
	if(bX)plot->SetAutoMajorTickX(check==BST_CHECKED);
	else plot->SetAutoMajorTickY(check==BST_CHECKED);
	
	if(check==BST_CHECKED)
	{
		if(bX)plot->SetXRangeSet(false);
		else plot->SetYRangeSet(false);
	}
	GetWindowText(GetDlgItem(hWnd, IDC_AS_MAJOR_TICK_LENGTH), strtmp, nMaxString);
	axis->SetTickLength((int)(fabs(_tcstod(strtmp, 0))*tlScale));
	GetWindowText(GetDlgItem(hWnd, IDC_AS_MAJOR_TICK_SIZE), strtmp, nMaxString);
	axis->SetTickSize((int)fabs(_tcstod(strtmp, 0)));
	SendDlgItemMessage(hWnd, IDC_AS_COLOR_MAJORTICK, WM_GETCOLOR, 0, (LPARAM)&color); 
	axis->SetTickColor(color);
	num = SendDlgItemMessage(hWnd, IDC_AS_MAJORTICK_POSITION, CB_GETCURSEL, 0, 0);
	axis->SetTickPosition((int)num);
}


template<class PlotImplT>
class ClassForAxisMajorTicksDlg
{
public:
#ifndef _WIN64
	static BOOL CALLBACK AxisMajorTicksDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT CALLBACK AxisMajorTicksDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
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
			SetupDataForMajorTicks(hWnd, plot, location);
			return TRUE;
		case WM_COMMAND:
			if(HIWORD (wparam) == EN_CHANGE)
			{
				switch(LOWORD(wparam))
				{
				case IDC_AS_NUMBER_MAJOR_TICKS:
					SendDlgItemMessage(hWnd, IDC_AS_AUTO_MAJOR_TICKS, BM_SETCHECK, FALSE, 0);
					return TRUE;
				}
			}
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForMajorTicks(hWnd, plot, location);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End