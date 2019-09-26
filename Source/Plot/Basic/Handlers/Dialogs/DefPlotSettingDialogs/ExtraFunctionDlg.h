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
void	SetupDataForExtraFunctions(HWND hWnd, PlotImplT *plot)
{
	SendDlgItemMessage(hWnd, IDC_PS_ENABLEOSCILLO, BM_SETCHECK, plot->IsOscilloScopeMode(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_ENABLEPROCESS, BM_SETCHECK, CGlobal::m_bShowDataProcessingMenu, 0);
	SendDlgItemMessage(hWnd, IDC_PS_RANGEZOOMMODE, BM_SETCHECK, plot->IsRangeZoomMode(), 0);
	
	SendDlgItemMessage(hWnd, IDC_PS_RANGEDRAG, BM_SETCHECK, plot->IsRangeDrag(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_RANGEDRAGTYPE, CB_RESETCONTENT, 0, 0);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		SendMessage(GetDlgItem(hWnd,IDC_PS_RANGEDRAGTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Drag X"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_RANGEDRAGTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Drag Y"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_RANGEDRAGTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Drag Both"));
		break;
	case kLangChinese:
		SendMessage(GetDlgItem(hWnd,IDC_PS_RANGEDRAGTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("拖动X"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_RANGEDRAGTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("拖动Y"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_RANGEDRAGTYPE), CB_ADDSTRING, 0, (LPARAM)_TEXT("拖动全部"));
		break;
	}
	SendMessage(GetDlgItem(hWnd,IDC_PS_RANGEDRAGTYPE), CB_SETCURSEL, (WPARAM)plot->GetRangeDragType(), 0);

	SendDlgItemMessage(hWnd, IDC_CHECK_POINTSELECTIONMODE, BM_SETCHECK, plot->IsPointSelectionMode(), 0);

	tostringstream ostr;
	tstring str;
	
	ostr.str(_TEXT(""));
	ostr<<plot->GetIndex(plot->GetLightedID());
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_EDIT_LIGHTEDDATAINDEX, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	ostr.str(_TEXT(""));
	ostr<<plot->GetLightedPointIdx();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_EDIT_LIGHTEDPOINTINDEX, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	ostr.str(_TEXT(""));
	ostr<<plot->GetIndex(plot->GetSelectedID());
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_EDIT_SELECTEDDATAINDEX, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	ostr.str(_TEXT(""));
	ostr<<plot->GetSelectedPointIdx();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_EDIT_SELECTEDPOINTINDEX, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
}

template<class PlotImplT>
void	SaveDataForExtraFunctions(HWND hWnd, PlotImplT *plot)
{
#ifndef _WIN64
	int			check,num;
#else
	LRESULT		check,num;
#endif

	check = SendDlgItemMessage(hWnd, IDC_PS_ENABLEOSCILLO, BM_GETCHECK, 0, 0);
	plot->SetOscilloScopeMode(check==BST_CHECKED);
	if(check!=BST_CHECKED)plot->SetRangeSet(false, false);
	check = SendDlgItemMessage(hWnd, IDC_PS_ENABLEPROCESS, BM_GETCHECK, 0, 0);
	CGlobal::m_bShowDataProcessingMenu = (check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_RANGEZOOMMODE, BM_GETCHECK, 0, 0);
	plot->SetRangeZoomMode(check==BST_CHECKED);
	if(check!=BST_CHECKED)plot->SetRangeSet(false, false);

	check = SendDlgItemMessage(hWnd, IDC_PS_RANGEDRAG, BM_GETCHECK, 0, 0);
	plot->SetRangeDrag(check==BST_CHECKED);
	plot->SetRangeSet(false, false);
	if(plot->IsRangeDrag())
	{
		plot->SetFloatXTicks(true);
		
		plot->SetFloatYTicks(true);
	}

	num = SendDlgItemMessage(hWnd, IDC_PS_RANGEDRAGTYPE, CB_GETCURSEL, 0, 0);
	plot->SetRangeDragType(num);

	check = SendDlgItemMessage(hWnd, IDC_CHECK_POINTSELECTIONMODE, BM_GETCHECK, 0, 0);
	plot->SetPointSelectionMode(check==BST_CHECKED);
}

template<class PlotImplT>
class	ClassForPlotExtraFunctionDlg
{
public:
#ifndef _WIN64
	static	BOOL	CALLBACK PlotExtraFunctionDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT	CALLBACK PlotExtraFunctionDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static PlotImplT	*plot;
		
		// local varibles
		
		LPNMHDR pnmh;
		
		switch(message)
		{
		case WM_INITDIALOG:
			plot = (PlotImplT *)((LPPROPSHEETPAGE)lparam)->lParam;
			SetupDataForExtraFunctions(hWnd, plot);
			
			return TRUE;
		case WM_COMMAND:
			
			// font buttons are clicked
			if(HIWORD (wparam) == BN_CLICKED)
			{
				if(LOWORD(wparam)==IDC_BUTTON_DELETELIGHTEDDATA)
				{
					plot->DeleteData(plot->GetLightedID());
				}
				else if(LOWORD(wparam)==IDC_BUTTON_DELETELIGHTEDPOINT)
				{
					if(plot->GetLightedID()>=0)
					{
						plot->RemovePoint(plot->GetLightedID(), plot->GetLightedPointIdx());
						plot->UnlightDataPoints();
						plot->UnselectDataPoints();
					}
				}
				else if(LOWORD(wparam)==IDC_BUTTON_DELETESELECTEDDATA)
				{
					plot->DeleteData(plot->GetSelectedID());
				}
				else if(LOWORD(wparam)==IDC_BUTTON_DELETESELECTEDPOINT)
				{
					if(plot->GetSelectedID()>=0)
					{
						plot->RemovePoint(plot->GetSelectedID(), plot->GetSelectedPointIdx());
						plot->UnlightDataPoints();
						plot->UnselectDataPoints();
					}
				}
				return TRUE;
			}
			break;
			
		case WM_NOTIFY:
			pnmh = (LPNMHDR) lparam;
			switch(pnmh->code)
			{
			case PSN_APPLY:
				// Apply Changes
				if(IsWindowVisible(hWnd))
					SaveDataForExtraFunctions(hWnd, plot);
				return TRUE;
			}
			break;
		}
		return FALSE;
	}
};

Declare_Namespace_End