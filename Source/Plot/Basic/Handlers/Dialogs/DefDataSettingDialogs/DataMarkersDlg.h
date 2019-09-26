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
void SetupDataForMarkers(HWND hWnd, PlotImplT *plot, int whichDataSet)
{
	tostringstream ostr;
	tstring str;
	
	CPlotData<DataPoint2D> *data = plot->GetAbsPlotData(whichDataSet);
	int		num;
	
	// Deal with the markers
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHOW, BM_SETCHECK, plot->IsMarkerShow(plot->GetDataID(whichDataSet)), 0);

	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_RESETCONTENT, 0, 0);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("No markers"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Circle"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Upright square"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Oblique square"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Left triangle"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Right triangle"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Up triangle"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Down triangle"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("X"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Cross"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Color Bar"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Dot"));
		break;
	case kLangChinese:
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("无标记"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("圆圈"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("竖直方块"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("斜方块"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("左向三角形"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("右向三角形"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("上三角形"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("下三角形"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("斜十字"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("正十字"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("颜色棒"));
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("点"));
		break;
	}
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_SETCURSEL, (WPARAM)plot->GetMarkerType(plot->GetDataID(whichDataSet)), 0);
	ostr<<plot->GetMarkerSize(plot->GetDataID(whichDataSet));
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SIZE, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_FILL, BM_SETCHECK, plot->GetMarkerFillState(plot->GetDataID(whichDataSet)), 0);
	num = plot->GetMarkerFrequency(plot->GetDataID(whichDataSet));
	if(num == 0)
	{
		CheckRadioButton(hWnd,IDC_DS_MARKER_FREQ_EVERY,IDC_DS_MARKER_FREQ_NONE,IDC_DS_MARKER_FREQ_NONE);
		ostr.str(_TEXT(""));
		ostr<<10;
		str=ostr.str();
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_NUMBER, WM_SETTEXT, 0, (LPARAM)str.c_str());
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_TOTAL_DATA, WM_SETTEXT, 0, (LPARAM)str.c_str());
	}
	else if(num == 1)
	{
		CheckRadioButton(hWnd,IDC_DS_MARKER_FREQ_EVERY,IDC_DS_MARKER_FREQ_NONE,IDC_DS_MARKER_FREQ_EVERY);
		ostr.str(_TEXT(""));
		ostr<<10;
		str=ostr.str();
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_NUMBER, WM_SETTEXT, 0, (LPARAM)str.c_str());
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_TOTAL_DATA, WM_SETTEXT, 0, (LPARAM)str.c_str());
	}
	else if(num > 0)
	{
		CheckRadioButton(hWnd,IDC_DS_MARKER_FREQ_EVERY,IDC_DS_MARKER_FREQ_NONE,IDC_DS_MARKER_FREQ_NTH);
		ostr.str(_TEXT(""));
		ostr<<num;
		str=ostr.str();
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_NUMBER, WM_SETTEXT, 0, (LPARAM)str.c_str());
		ostr.str(_TEXT(""));
		ostr<<10;
		str=ostr.str();
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_TOTAL_DATA, WM_SETTEXT, 0, (LPARAM)str.c_str());
	}
	else if(num <0)
	{
		CheckRadioButton(hWnd,IDC_DS_MARKER_FREQ_EVERY,IDC_DS_MARKER_FREQ_NONE,IDC_DS_MARKER_FREQ_TOTAL);
		ostr.str(_TEXT(""));
		ostr<<10;
		str=ostr.str();
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_NUMBER, WM_SETTEXT, 0, (LPARAM)str.c_str());
		ostr.str(_TEXT(""));
		ostr<<abs(num);
		str=ostr.str();
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_TOTAL_DATA, WM_SETTEXT, 0, (LPARAM)str.c_str());
	}
}

template<class PlotImplT>
void SaveDataForMarkers(HWND hWnd, PlotImplT *plot, int whichDataSet)
{
	TCHAR	strtmp[nMaxString];
	
	CPlotData<DataPoint2D> *data = plot->GetAbsPlotData(whichDataSet);
#ifndef _WIN64
	int			check,num;
#else
	LRESULT		check,num;
#endif
	
	// Deal with the markers
	check = SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHOW, BM_GETCHECK, 0, 0);
	plot->SetMarkerShow(plot->GetDataID(whichDataSet), check==BST_CHECKED);

	num = SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_GETCURSEL, 0, 0);
	plot->SetMarkerType(plot->GetDataID(whichDataSet), (int)num);
	GetWindowText(GetDlgItem(hWnd, IDC_DS_MARKER_SIZE), strtmp, nMaxString);
	plot->SetMarkerSize(plot->GetDataID(whichDataSet), _ttoi(strtmp));
	check = SendDlgItemMessage(hWnd, IDC_DS_MARKER_FILL, BM_GETCHECK, 0, 0);
	plot->SetMarkerFillState(plot->GetDataID(whichDataSet), check==BST_CHECKED);
	
	if( IsDlgButtonChecked(hWnd, IDC_DS_MARKER_FREQ_EVERY) == BST_CHECKED)
	{
		plot->SetMarkerFrequency(plot->GetDataID(whichDataSet), 1);
	}
	else if( IsDlgButtonChecked(hWnd, IDC_DS_MARKER_FREQ_NTH) == BST_CHECKED)
	{
		GetWindowText(GetDlgItem(hWnd, IDC_DS_MARKER_FREQ_NUMBER), strtmp, nMaxString);
		plot->SetMarkerFrequency(plot->GetDataID(whichDataSet), _ttoi(strtmp));
	}
	else if( IsDlgButtonChecked(hWnd, IDC_DS_MARKER_FREQ_TOTAL) == BST_CHECKED)
	{
		GetWindowText(GetDlgItem(hWnd, IDC_DS_MARKER_FREQ_TOTAL_DATA), strtmp, nMaxString);
		plot->SetMarkerFrequency(plot->GetDataID(whichDataSet), -abs(_ttoi(strtmp)) );
	}
	else if( IsDlgButtonChecked(hWnd, IDC_DS_MARKER_FREQ_NONE) == BST_CHECKED)
	{
		plot->SetMarkerFrequency(plot->GetDataID(whichDataSet), 0);
	}
}

template<class PlotImplT>
class ClassForDataMarkersDlg
{
public:
#ifndef _WIN64
	static BOOL CALLBACK DataMarkersSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static LRESULT CALLBACK DataMarkersSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
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
			SetupDataForMarkers(hWnd, plot, whichDataSet);
			return TRUE;
		case WM_COMMAND:
			
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForMarkers(hWnd, plot, whichDataSet);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}

};

Declare_Namespace_End
