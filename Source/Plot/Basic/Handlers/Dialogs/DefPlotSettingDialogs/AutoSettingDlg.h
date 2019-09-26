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
void	SetupDataForAutoSettings(HWND hWnd, PlotImplT *plot)
{
	SendDlgItemMessage(hWnd, IDC_PS_DATA_COLOR_AUTO, BM_SETCHECK, plot->IsAutoDataColorGeneral(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_DATA_LINESTYLE_AUTO, BM_SETCHECK, plot->IsAutoLineStyleGeneral(), 0);
	
	SendDlgItemMessage(hWnd, IDC_PS_FLOATXTICKS, BM_SETCHECK, plot->IsFloatXTicks(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_FLOATYTICKS, BM_SETCHECK, plot->IsFloatYTicks(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_EQUALXYRANGE, BM_SETCHECK, plot->IsEqualXYRange(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_EQUALXYSCALE, BM_SETCHECK, plot->IsEqualXYScale(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_EQUALXYAXIS, BM_SETCHECK, plot->IsEqualXYAxis(), 0);
	
	SendDlgItemMessage(hWnd, IDC_PS_LANGURAGE, CB_RESETCONTENT, 0, 0);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		SendMessage(GetDlgItem(hWnd,IDC_PS_LANGURAGE), CB_ADDSTRING, 0, (LPARAM)_TEXT("English"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LANGURAGE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Chinese"));
		break;
	case kLangChinese:
		SendMessage(GetDlgItem(hWnd,IDC_PS_LANGURAGE), CB_ADDSTRING, 0, (LPARAM)_TEXT("英文"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_LANGURAGE), CB_ADDSTRING, 0, (LPARAM)_TEXT("中文"));
		break;
	}
	SendMessage(GetDlgItem(hWnd,IDC_PS_LANGURAGE), CB_SETCURSEL, (WPARAM)CGlobal::m_nLangurage, 0);
	
	SendDlgItemMessage(hWnd, IDC_PS_APPERANCEMODE, CB_RESETCONTENT, 0, 0);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		SendMessage(GetDlgItem(hWnd,IDC_PS_APPERANCEMODE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Extra small"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_APPERANCEMODE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Small"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_APPERANCEMODE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Nomal"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_APPERANCEMODE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Large"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_APPERANCEMODE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Extra large"));
		break;
	case kLangChinese:
		SendMessage(GetDlgItem(hWnd,IDC_PS_APPERANCEMODE), CB_ADDSTRING, 0, (LPARAM)_TEXT("超小"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_APPERANCEMODE), CB_ADDSTRING, 0, (LPARAM)_TEXT("小"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_APPERANCEMODE), CB_ADDSTRING, 0, (LPARAM)_TEXT("正常"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_APPERANCEMODE), CB_ADDSTRING, 0, (LPARAM)_TEXT("大"));
		SendMessage(GetDlgItem(hWnd,IDC_PS_APPERANCEMODE), CB_ADDSTRING, 0, (LPARAM)_TEXT("超大"));
		break;
	}
	SendMessage(GetDlgItem(hWnd,IDC_PS_APPERANCEMODE), CB_SETCURSEL, (WPARAM)plot->GetApperanceMode(), 0);
}

template<class PlotImplT>
void	SaveDataForAutoSettings(HWND hWnd, PlotImplT *plot)
{
#ifndef _WIN64
	int			check,num;
#else
	LRESULT		check,num;
#endif

	check = SendDlgItemMessage(hWnd, IDC_PS_FLOATXTICKS, BM_GETCHECK, 0, 0);
	plot->SetFloatXTicks(check==BST_CHECKED);
	//plot->m_bFloatXTicksOld = (check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_FLOATYTICKS, BM_GETCHECK, 0, 0);
	plot->SetFloatYTicks(check==BST_CHECKED);
	//plot->m_bFloatYTicksOld = (check==BST_CHECKED);

	check = SendDlgItemMessage(hWnd, IDC_PS_DATA_COLOR_AUTO, BM_GETCHECK, 0, 0);
	plot->SetAutoDataColorGeneral(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_DATA_LINESTYLE_AUTO, BM_GETCHECK, 0, 0);
	plot->SetAutoLineStyleGeneral(check==BST_CHECKED);

	check = SendDlgItemMessage(hWnd, IDC_PS_EQUALXYRANGE, BM_GETCHECK, 0, 0);
	plot->SetEqualXYRange(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_EQUALXYSCALE, BM_GETCHECK, 0, 0);
	plot->SetEqualXYScale(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_EQUALXYAXIS, BM_GETCHECK, 0, 0);
	plot->SetEqualXYAxis(check==BST_CHECKED);

	num = SendDlgItemMessage(hWnd, IDC_PS_LANGURAGE, CB_GETCURSEL, 0, 0);
	CGlobal::m_nLangurage = (int)num;

	num = SendDlgItemMessage(hWnd, IDC_PS_APPERANCEMODE, CB_GETCURSEL, 0, 0);
	plot->SetApperanceMode((int)num);
}

template<class PlotImplT>
class	ClassForPlotAutoSettingDlg
{
public:
#ifndef _WIN64
	static	BOOL	CALLBACK PlotAutoSettingDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT	CALLBACK PlotAutoSettingDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static PlotImplT	*plot;
		
		// local varibles
		
		LPNMHDR pnmh;
		
		switch(message)
		{
		case WM_INITDIALOG:
			plot = (PlotImplT *)((LPPROPSHEETPAGE)lparam)->lParam;
			SetupDataForAutoSettings(hWnd, plot);
			
			return TRUE;
			
		case WM_NOTIFY:
			pnmh = (LPNMHDR) lparam;
			switch(pnmh->code)
			{
			case PSN_APPLY:
				// Apply Changes
				if(IsWindowVisible(hWnd))
					SaveDataForAutoSettings(hWnd, plot);
				return TRUE;
			}
			break;
		}
		return FALSE;
	}
};

Declare_Namespace_End