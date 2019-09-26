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
void SetupDataForArrows(HWND hWnd, PlotImplT *plot, int location)
{
	CAxisImpl *axis = plot->GetAxisByLocation(location);
	
	tostringstream ostr;
	tstring str;
	
	SendDlgItemMessage(hWnd, IDC_AS_ARROWS_START, BM_SETCHECK, axis->IsArrowStart(), 0);
	SendDlgItemMessage(hWnd, IDC_AS_ARROWS_END, BM_SETCHECK, axis->IsArrowEnd(), 0);
	
	ostr<<axis->GetArrowSize();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_AS_ARROWS_ARRORSIZE, WM_SETTEXT, 0, (LPARAM)str.c_str());
	ostr.str(_TEXT(""));
	ostr<<axis->GetArrowRatio();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_AS_ARROWS_ARROWRATIO, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	SendDlgItemMessage(hWnd, IDC_AS_ARROWS_FLATBUTTOM, BM_SETCHECK, axis->IsArrowFlatBottom(), 0);
}

template<class PlotImplT>
void SaveDataForArrows(HWND hWnd, PlotImplT *plot, int location)
{
	// local varibles
	TCHAR	strtmp[nMaxString];
	CAxisImpl	*axis = plot->GetAxisByLocation(location);
	
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif
	
	check = SendDlgItemMessage(hWnd, IDC_AS_ARROWS_START, BM_GETCHECK, 0, 0);
	axis->SetArrowStart(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_AS_ARROWS_END, BM_GETCHECK, 0, 0);
	axis->SetArrowEnd(check==BST_CHECKED);
	
	GetWindowText(GetDlgItem(hWnd, IDC_AS_ARROWS_ARRORSIZE), strtmp, nMaxString);
	axis->SetArrowSize(_ttoi(strtmp));
	GetWindowText(GetDlgItem(hWnd, IDC_AS_ARROWS_ARROWRATIO), strtmp, nMaxString);
	axis->SetArrowRatio(_ttoi(strtmp));
	
	check = SendDlgItemMessage(hWnd, IDC_AS_ARROWS_FLATBUTTOM, BM_GETCHECK, 0, 0);
	axis->SetArrowFlatBottom(check==BST_CHECKED);
}

template<class PlotImplT>
class ClassForAxisArrowsDlg
{
public:
#ifndef _WIN64
	static BOOL CALLBACK AxisArrowsDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
		static LRESULT CALLBACK AxisArrowsDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
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
			SetupDataForArrows(hWnd, plot, location);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForArrows(hWnd, plot, location);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End

