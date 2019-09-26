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
void SetupDataForFill(HWND hWnd, PlotImplT *plot, int whichDataSet)
{
	CPlotData<DataPoint2D> *data;
	
	data = plot->GetAbsPlotData(whichDataSet);
	
	// Deal with the fill enclosed curve
	SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILL, BM_SETCHECK, plot->IsDataFill(plot->GetDataID(whichDataSet)), 0);
	SendDlgItemMessage(hWnd, IDC_DS_COLOR_ENCLOSEDCURVE, WM_SETCOLOR, 0, plot->GetDataFillColor(plot->GetDataID(whichDataSet)));
	
	SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_RESETCONTENT, 0, 0);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("Closed"));
		SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("From Bottom"));
		SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("From Top"));
		SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("From Left"));
		SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("From Right"));
		break;
	case kLangChinese:
		SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("封闭"));
		SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("底轴开始"));
		SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("顶轴开始"));
		SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("左轴开始"));
		SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("右轴开始"));
		break;
	}
	SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_SETCURSEL, (WPARAM)plot->GetDataFillMode(plot->GetDataID(whichDataSet)), 0);
	
	SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLALPHA, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM) MAKELONG(0, 255));
	SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLALPHA, TBM_SETPOS, (WPARAM)TRUE,(LPARAM)plot->GetDataFillAlpha(plot->GetDataID(whichDataSet)));
	
}

template<class PlotImplT>
void SaveDataForFill(HWND hWnd, PlotImplT *plot, int whichDataSet)
{
	CPlotData<DataPoint2D> *data = plot->GetAbsPlotData(whichDataSet);
	COLORREF	color;
#ifndef _WIN64
	int			num, check;
#else
	LRESULT		num, check;
#endif
	
	// Deal with the fill enclosed curve
	check = SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILL, BM_GETCHECK, 0, 0);
	plot->SetDataFill(plot->GetDataID(whichDataSet), check==BST_CHECKED);
	SendDlgItemMessage(hWnd, IDC_DS_COLOR_ENCLOSEDCURVE, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetDataFillColor(plot->GetDataID(whichDataSet), color);
	
	num = SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLMETHOD, CB_GETCURSEL, 0, 0);
	plot->SetDataFillMode(plot->GetDataID(whichDataSet), (int)num);
	
	num = (unsigned char)SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILLALPHA,TBM_GETPOS,0,0);
	plot->SetDataFillAlpha(plot->GetDataID(whichDataSet), (int)num);
}


template<class PlotImplT>
class ClassForDataFillDlg
{
public:
#ifndef _WIN64
	static	BOOL CALLBACK DataFillSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT CALLBACK DataFillSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
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
			SetupDataForFill(hWnd, plot, whichDataSet);
			return TRUE;
		case WM_COMMAND:
			
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForFill(hWnd, plot, whichDataSet);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End