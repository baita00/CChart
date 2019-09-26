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
//#include <prsht.h>
#include "../../../CChartNameSpace.h"

Declare_Namespace_CChart

template<class PlotImplT>
void	SetupDataForCopyright(HWND hWnd, PlotImplT *plot)
{
	tstring str;
	str = CGlobal::GetStrCopyright();
	SendDlgItemMessage(hWnd, IDC_PS_COPYRIGHT, WM_SETTEXT, 0, (LPARAM)str.c_str());
	str = CGlobal::GetStrMyVersion();
	SendDlgItemMessage(hWnd, IDC_PS_VERSION, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	str = CGlobal::GetStrAuthorInfo();
	SendDlgItemMessage(hWnd, IDC_PS_AUTHOR, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	str = CGlobal::GetStrLicense();
	SendDlgItemMessage(hWnd, IDC_PS_LICENSE, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	str = CGlobal::GetStrStatement();
	SendDlgItemMessage(hWnd, IDC_PS_STATEMENT, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	str = CGlobal::GetStrContact();
	SendDlgItemMessage(hWnd, IDC_PS_CONTACT, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
}

template<class PlotImplT>
void	SaveDataForCopyright(HWND hWnd, PlotImplT *plot)
{
	return;
}

template<class PlotImplT>
class	ClassForPlotCopyrightDlg
{
public:
#ifndef _WIN64
	static	BOOL	CALLBACK PlotCopyrightDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT	CALLBACK PlotCopyrightDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static PlotImplT	*plot;
		
		// local varibles
		
		LPNMHDR pnmh;
		
		switch(message)
		{
		case WM_INITDIALOG:
			plot = (PlotImplT *)((LPPROPSHEETPAGE)lparam)->lParam;
			SetupDataForCopyright(hWnd, plot);
			
			return TRUE;
			
		case WM_NOTIFY:
			pnmh = (LPNMHDR) lparam;
			switch(pnmh->code)
			{
			case PSN_APPLY:
				// Apply Changes
				if(IsWindowVisible(hWnd))
					SaveDataForCopyright(hWnd, plot);
			}
			break;
		}
		return FALSE;
	}
};

Declare_Namespace_End


