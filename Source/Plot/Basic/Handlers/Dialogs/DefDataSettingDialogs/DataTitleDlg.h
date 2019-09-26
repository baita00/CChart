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
#include <windows.h>
#include "../../../CChartNameSpace.h"

Declare_Namespace_CChart

template<class PlotImplT>
void SetupDataForTitle(HWND hWnd, PlotImplT *plot, int whichDataSet)
{	
	CPlotData<DataPoint2D> *data = plot->GetAbsPlotData(whichDataSet);
	
	// Deal with the title
	SendDlgItemMessage(hWnd, IDC_DS_DATA_TITLE, WM_SETTEXT, 0, (LPARAM)data->GetTitle().c_str());
}

template<class PlotImplT>
void SaveDataForTitle(HWND hWnd, PlotImplT *plot, int whichDataSet)
{
	TCHAR	strtmp[nMaxString];
	
	CPlotData<DataPoint2D> *data = plot->GetAbsPlotData(whichDataSet);
	// Deal with the title
	GetWindowText(GetDlgItem(hWnd, IDC_DS_DATA_TITLE), strtmp, nMaxString);
	data->SetTitle(strtmp);
}

template<class PlotImplT>
class ClassForDataTitleDlg
{
public:
#ifndef _WIN64
	static	BOOL CALLBACK DataTitleSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT CALLBACK DataTitleSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
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
			SetupDataForTitle(hWnd, plot, whichDataSet);
			return TRUE;
		case WM_COMMAND:
			
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForTitle(hWnd, plot, whichDataSet);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End