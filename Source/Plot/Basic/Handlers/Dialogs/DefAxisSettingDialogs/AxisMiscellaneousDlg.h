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
void SetupDataForMiscellaneous(HWND hWnd, PlotImplT *plot, int location)
{
	
	CAxisImpl *axis = plot->GetAxisByLocation(location);
	bool bX = axis->IsXAxis();
	
	SendDlgItemMessage(hWnd, IDC_AS_MISCELLANEOUS_INDEPENDENT, BM_SETCHECK, axis->IsIndependent(), 0);
	SendDlgItemMessage(hWnd, IDC_AS_MISCELLANEOUS_LOGARITHM, BM_SETCHECK, bX?plot->IsXLogarithm():plot->IsYLogarithm(), 0);
	SendDlgItemMessage(hWnd, IDC_AS_MISCELLANEOUS_SCINOTATION, BM_SETCHECK, axis->IsSciNotation(), 0);
	
	tostringstream ostr;
	tstring str;
	
	// Deal with the range
	ostr.str(_TEXT(""));
	ostr<<axis->GetAxisGap();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_AS_MISCELLANEOUS_AXISGAP, WM_SETTEXT, 0, (LPARAM)str.c_str());
}

template<class PlotImplT>
void SaveDataForMiscellaneous(HWND hWnd, PlotImplT *plot, int location)
{
	// local varibles
	TCHAR	strtmp[nMaxString];
	CAxisImpl	*axis = plot->GetAxisByLocation(location);
	bool bX = axis->IsXAxis();
	
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif
	int num;
	
	check = SendDlgItemMessage(hWnd, IDC_AS_MISCELLANEOUS_INDEPENDENT, BM_GETCHECK, 0, 0);
	axis->SetIndependent(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_AS_MISCELLANEOUS_LOGARITHM, BM_GETCHECK, 0, 0);
	//axis->SetLogarithm(check==BST_CHECKED);
	if(!plot->CheckLogSafety(bX) && check==BST_CHECKED)
	{
		if(CGlobal::m_nLangurage == kLangEnglish)
			MessageBox(NULL, _T("Minus or zero value!!!"), _T("Warning"), MB_OK);
		else
			MessageBox(NULL, _T("有负数或者0!!!"), _T("警告"), MB_OK);
	}
	else
	{
		if(bX)plot->SetXLogarithm(check==BST_CHECKED);
		else plot->SetYLogarithm(check==BST_CHECKED);
	}
	
	check = SendDlgItemMessage(hWnd, IDC_AS_MISCELLANEOUS_SCINOTATION, BM_GETCHECK, 0, 0);
	axis->SetSciNotation(check==BST_CHECKED);
	
	GetWindowText(GetDlgItem(hWnd, IDC_AS_MISCELLANEOUS_AXISGAP), strtmp, nMaxString);
	num = _ttoi(strtmp);
	axis->SetAxisGap(num);
}

template<class PlotImplT>
class ClassForAxisMiscellaneousDlg
{
public:
#ifndef _WIN64
	static	BOOL CALLBACK AxisMiscellaneousDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT CALLBACK AxisMiscellaneousDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
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
			SetupDataForMiscellaneous(hWnd, plot, location);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForMiscellaneous(hWnd, plot, location);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End