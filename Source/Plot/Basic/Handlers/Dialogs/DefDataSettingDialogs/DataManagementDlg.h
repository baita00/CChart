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
#include "../../../CChartNameSpace.h"
#include <windows.h>

Declare_Namespace_CChart

template<class PlotImplT>
void SetupDataForManagement(HWND hWnd, PlotImplT *plot, int whichDataSet)
{
	CPlotData<DataPoint2D> *data = plot->GetAbsPlotData(whichDataSet);
	
	tostringstream ostr;
	tstring str;
	ostr<<data->GetDataCount();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DS_NUMOFPOINTS, WM_SETTEXT, 0, (LPARAM)str.c_str());
}

template<class PlotImplT>
void SaveDataForManagement(HWND hWnd, PlotImplT *plot, int whichDataSet)
{
	return;
}

template<class PlotImplT>
class ClassForDataManagementDlg
{
public:
#ifndef _WIN64
	static	BOOL CALLBACK DataManagementSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
		static	LRESULT CALLBACK DataManagementSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static DataSettingParam<PlotImplT> *para;
		static PlotImplT	*plot;
		static int whichDataSet;
		
		tstring filename, fileext, filepath;
		
		switch(message)
		{
		case WM_INITDIALOG:
			para = (DataSettingParam<PlotImplT> *)lparam;
			plot = para->plot;
			whichDataSet = para->whichDataSet;
			
			// Deal with the title
			SetupDataForManagement(hWnd, plot, whichDataSet);
			return TRUE;
		case WM_COMMAND:
			
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForManagement(hWnd, plot, whichDataSet);
				return TRUE;
			case IDC_DATA_LOAD:
				if( !ShowFileDialog(hWnd, true, filename, fileext, filepath) )	return FALSE;
				if( plot->ReadFromFile(whichDataSet, filename) )
				{
					MessageBox(hWnd, _TEXT("Load data OK"), _TEXT("Infomation"), MB_OK);
					return FALSE;
				}
				return TRUE;
			case IDC_DATA_SAVE:
				if( !ShowFileDialog(hWnd, false, filename, fileext, filepath) ) return FALSE;
				if( plot->WriteToFile(whichDataSet, filename) )
				{
					MessageBox(hWnd, _TEXT("Save data OK"), _TEXT("Infomation"), MB_OK);
					return FALSE;
				}
				return TRUE;
			case IDC_DATA_SHOW:
				plot->ShowDataInGrid(whichDataSet, hWnd);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End
