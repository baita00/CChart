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
#include <windows.h>
#include <tchar.h>
//class CXYPlotImplI;


//bool	MultiDataOption( CXYPlotImplI *plot, HWND hWnd);

#include "../../../../Resources/PlotResources.h"
#include "../../../CChartNameSpace.h"

Declare_Namespace_CChart

template<class PlotImplT>
void	SetupDataForMDOption(HWND hWnd, PlotImplT *plot)
{
	SendDlgItemMessage(hWnd, IDC_MULTIDATA_SEQUANCE, BM_SETCHECK, plot->IsSequence(), 0);
	SendDlgItemMessage(hWnd, IDC_MULTIDATA_SHAREX, BM_SETCHECK, plot->IsShareX(), 0);
	
	tstring str = plot->GetFileSeparator();
	trimspace(str);
	SendDlgItemMessage(hWnd, IDC_MULTIDATA_SEGSTR, WM_SETTEXT, 0, (LPARAM)str.c_str());
}

template<class PlotImplT>
void	SaveDataForMDOption(HWND hWnd, PlotImplT *plot)
{
#ifndef _WIN64
	int			check;
#else
	LRESULT		check;
#endif
	
	check = SendDlgItemMessage(hWnd, IDC_MULTIDATA_SEQUANCE, BM_GETCHECK, 0, 0);
	plot->SetSequence(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_MULTIDATA_SHAREX, BM_GETCHECK, 0, 0);
	plot->SetShareX(check==BST_CHECKED);
	
	TCHAR	str[128];
	SendDlgItemMessage(hWnd, IDC_MULTIDATA_SEGSTR, WM_GETTEXT, 128, LPARAM(str));
	tstring sep ;
	sep += _TEXT("\r\n");
	sep += str;
	sep += _TEXT("\r\n");
	plot->SetFileSeparator( sep );
}

template<class PlotImplT>
class ClassForMultiDataOptionDlg
{
public:
#ifndef _WIN64
	static	BOOL CALLBACK MDOptionDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT CALLBACK MDOptionDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static PlotImplT	*plot;
		
		// local varibles	
		switch(message)
		{
		case WM_INITDIALOG:
			plot = (PlotImplT *)lparam;
			SetupDataForMDOption(hWnd, plot);
			
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wparam))
			{
			case IDOK:
				SaveDataForMDOption(hWnd, plot);
				EndDialog(hWnd, 1);
				return TRUE;
			case IDCANCEL:
				EndDialog(hWnd, 0);
				return FALSE;
			}
			break;
		}
		return FALSE;
	}
};


template<class PlotImplT>
bool	MultiDataOption( PlotImplT *plot, HWND hWnd)
{
	UINT dlgID;
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		dlgID = IDD_MULTIDATA;
		break;
	case kLangChinese:
		dlgID = IDD_MULTIDATA_C;
		break;
	default:
		return false;
	}
	if(DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(dlgID), hWnd, (DLGPROC)ClassForMultiDataOptionDlg<PlotImplT>::MDOptionDlgFunc, (LPARAM)plot))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

Declare_Namespace_End
