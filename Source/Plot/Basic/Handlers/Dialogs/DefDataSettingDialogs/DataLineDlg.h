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
void SetupDataForLine(HWND hWnd, PlotImplT *plot, int whichDataSet)
{
	tostringstream ostr;
	tstring str;
	
	CPlotData<DataPoint2D> *data = plot->GetAbsPlotData(whichDataSet);
	
	// Deal with the line
	SendDlgItemMessage(hWnd, IDC_DS_COLOR_LINE, WM_SETCOLOR, 0, plot->GetDataColor(plot->GetDataID(whichDataSet)));
	SendDlgItemMessage(hWnd, IDC_DS_COLOR_AUTO, BM_SETCHECK, plot->IsAutoColor(plot->GetDataID(whichDataSet)), 0);
	SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE_AUTO, BM_SETCHECK, plot->IsAutoLineStyle(plot->GetDataID(whichDataSet)), 0);
	ostr<<plot->GetDataLineSize(plot->GetDataID(whichDataSet));
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DS_LINE_SIZE, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_RESETCONTENT, 0, 0);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Solid _______________________"));
		SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Dashed _ _ _ _ _ _ _ _ _ _ _ _ _ _"));
		SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Dotted . . . . . . . . . . . . . . . . . . . . . ."));
		SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Dash-Dot _ . _ . _ . _ . _ . _ . _ . _ ."));
		SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Dash-Dot-Dot _ . . _ . . _ . . _ . . _ ."));
		break;
	case kLangChinese:
		SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("实线 _______________________"));
		SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("虚划线 _ _ _ _ _ _ _ _ _ _ _ _ _ _"));
		SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("点线 . . . . . . . . . . . . . . . . . . . . . ."));
		SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("划点线 _ . _ . _ . _ . _ . _ . _ . _ ."));
		SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("划点点线 _ . . _ . . _ . . _ . . _ ."));
		break;
	}
	SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_SETCURSEL, (WPARAM)plot->GetDataLineStyle(plot->GetDataID(whichDataSet)), 0);
}

template<class PlotImplT>
void SaveDataForLine(HWND hWnd, PlotImplT *plot, int whichDataSet)
{
	TCHAR	strtmp[nMaxString];
	
	CPlotData<DataPoint2D> *data = plot->GetAbsPlotData(whichDataSet);
#ifndef _WIN64
	int			check,num;
#else
	LRESULT		check,num;
#endif
	COLORREF	color;
	
	// Deal with the line
	SendDlgItemMessage(hWnd, IDC_DS_COLOR_LINE, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetDataColor(plot->GetDataID(whichDataSet), color);
	check = SendDlgItemMessage(hWnd, IDC_DS_COLOR_AUTO, BM_GETCHECK, 0, 0);
	plot->SetAutoColor(plot->GetDataID(whichDataSet), check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE_AUTO, BM_GETCHECK, 0, 0);
	plot->SetAutoLineStyle(plot->GetDataID(whichDataSet), check==BST_CHECKED);
	GetWindowText(GetDlgItem(hWnd, IDC_DS_LINE_SIZE), strtmp, nMaxString);
	plot->SetDataLineSize(plot->GetDataID(whichDataSet), _ttoi(strtmp));
	num = SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_GETCURSEL, 0, 0);
	plot->SetDataLineStyle(plot->GetDataID(whichDataSet), (int)num);
}

template<class PlotImplT>
class	ClassForDataLingDlg
{
public:
#ifndef _WIN64
	static	BOOL CALLBACK DataLineSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT CALLBACK DataLineSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static DataSettingParam<PlotImplT> *para;
		static PlotImplT	*plot;
		static int whichDataSet;
		
		switch(message)
		{
		case WM_INITDIALOG:
			para = (DataSettingParam<PlotImplT> *)lparam;
			plot = para->plot;
			whichDataSet = para->whichDataSet;
			
			// Deal with the title
			SetupDataForLine(hWnd, plot, whichDataSet);
			return TRUE;
		case WM_COMMAND:
			
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForLine(hWnd, plot, whichDataSet);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End