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
void	SetupDataForCursors(HWND hWnd, PlotImplT *plot)
{
	tostringstream ostr;
	tstring str;
	
	SendDlgItemMessage(hWnd, IDC_PS_CURSOR_YUSE, BM_SETCHECK, plot->IsShowVCursor(), 0);
	ostr.str(_TEXT(""));
	ostr<<plot->GetCursorX();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_CURSER_XPOS, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	SendDlgItemMessage(hWnd, IDC_PS_CURSOR_XUSE, BM_SETCHECK, plot->IsShowHCursor(), 0);
	ostr.str(_TEXT(""));
	ostr<<plot->GetCursorY();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_CURSER_YPOS, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	
	SendDlgItemMessage(hWnd, IDC_PS_CURSOR_BOND, BM_SETCHECK, plot->IsHoldCursorToCurve(), 0);
	for(int i=0; i<plot->GetPlotDataCount(); i++)
	{
		ostr.str(_TEXT(""));
		ostr<<i<<_TEXT("---")<<plot->GetAbsDataTitle(i);
		str=ostr.str();
		SendMessage(GetDlgItem(hWnd,IDC_PS_CURSOR_CURVEBOND), CB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
	SendMessage(GetDlgItem(hWnd,IDC_PS_CURSOR_CURVEBOND), CB_SETCURSEL, (WPARAM)plot->GetIndexToHoldCursor(), 0);
	
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_CURSOR, WM_SETCOLOR, 0, (LPARAM)plot->GetCursorColor());
}

template<class PlotImplT>
void	SaveDataForCursors(HWND hWnd, PlotImplT *plot)
{
	TCHAR strtmp[nMaxString];
#ifndef _WIN64
	int			check, index;
#else
	LRESULT		check, index;
#endif
	COLORREF color;
	double	fVal;
	
	check = SendDlgItemMessage(hWnd, IDC_PS_CURSOR_YUSE, BM_GETCHECK, 0, 0);
	plot->SetShowVCursor(check==BST_CHECKED);
	
	GetWindowText(GetDlgItem(hWnd, IDC_PS_CURSER_XPOS), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	plot->SetCursorX(fVal);
	
	check = SendDlgItemMessage(hWnd, IDC_PS_CURSOR_XUSE, BM_GETCHECK, 0, 0);
	plot->SetShowHCursor(check==BST_CHECKED);
	
	GetWindowText(GetDlgItem(hWnd, IDC_PS_CURSER_YPOS), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	plot->SetCursorY(fVal);
	
	check = SendDlgItemMessage(hWnd, IDC_PS_CURSOR_BOND, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetHoldCursorToCurve(true);
		index = SendDlgItemMessage(hWnd, IDC_PS_CURSOR_CURVEBOND, CB_GETCURSEL, 0, 0);
		plot->SetIndexToHoldCursor((int)index);
	}
	else
	{
		plot->SetHoldCursorToCurve(false);
	}
	
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_CURSOR, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetCursorColor(color);
}

template<class PlotImplT>
class ClassForPlotCursorsDlg
{
public:
#ifndef _WIN64
	static BOOL	CALLBACK PlotCursorsDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static LRESULT	CALLBACK PlotCursorsDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static PlotImplT	*plot;	
		
		LPNMHDR pnmh;
		
		switch(message)
		{
		case WM_INITDIALOG:
			plot = (PlotImplT *)((LPPROPSHEETPAGE)lparam)->lParam;
			// Deal with the title
			SetupDataForCursors(hWnd, (CXYPlotImplI *)plot);
			return TRUE;		
		case WM_NOTIFY:
			pnmh = (LPNMHDR) lparam;
			switch(pnmh->code)
			{
			case PSN_APPLY:
				// Apply Changes
				if(IsWindowVisible(hWnd))
					SaveDataForCursors(hWnd, (CXYPlotImplI *)plot);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End
