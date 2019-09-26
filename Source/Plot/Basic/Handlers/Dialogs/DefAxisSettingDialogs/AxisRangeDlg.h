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
void SetupDataForRange(HWND hWnd, PlotImplT *plot, int location)
{
	CAxisImpl *axis = plot->GetAxisByLocation(location);
	
	tostringstream ostr;
	tstring str;

	double range[2];
	bool bX = axis->IsXAxis();

	if(bX)plot->GetXRange(range);
	else plot->GetYRange(range);

	if(axis->IsTime())
	{
		if(bX)MoveRange(range, -(double)plot->GetXStartTime());
		else MoveRange(range, -(double)plot->GetYStartTime());
	}
	// Deal with the range
	ostr.str(_TEXT(""));
	//	if(!axis->IsLogarithm())
	//		ostr<<axis->GetLowerRange();
	//	else
	//		ostr<<pow(10.0, axis->GetLowerRange());
	if(!(bX?plot->IsXLogarithm():plot->IsYLogarithm()))
	{
		ostr<<range[0];
	}
	else
	{
		ostr<<pow(10.0, range[0]);
	}
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_AS_XRANGE_LOW, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	ostr.str(_TEXT(""));
	//	if(!axis->IsLogarithm())
	//		ostr<<axis->GetUpperRange();
	//	else
	//		ostr<<pow(10.0, axis->GetUpperRange());
	if(!(bX?plot->IsXLogarithm():plot->IsYLogarithm()))
	{
		ostr<<range[1];
	}
	else
	{
		ostr<<pow(10.0, range[1]);
	}
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_AS_XRANGE_HIGH, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	bool btx = plot->IsXAutoRange();
	bool bty = plot->IsYAutoRange();
	SendDlgItemMessage(hWnd, IDC_AS_AUTO_XRANGE, BM_SETCHECK, bX?plot->IsXAutoRange():plot->IsYAutoRange(), 0);
}

template<class PlotImplT>
void SaveDataForRange(HWND hWnd, PlotImplT *plot, int location)
{
	// local varibles
	TCHAR	strtmp[nMaxString];
	CAxisImpl	*axis = plot->GetAxisByLocation(location);
	
	bool	bX = axis->IsXAxis();
	
	double	range[2];
	
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif
	
	// Deal with the range
	GetWindowText(GetDlgItem(hWnd, IDC_AS_XRANGE_LOW), strtmp, nMaxString);
	//xRange[0]=atof(strtmp);
	range[0] = _tcstod(strtmp, 0);
	GetWindowText(GetDlgItem(hWnd, IDC_AS_XRANGE_HIGH), strtmp, nMaxString);
	range[1] = _tcstod(strtmp, 0);
	check = SendDlgItemMessage(hWnd, IDC_AS_AUTO_XRANGE, BM_GETCHECK, 0, 0);

	if(axis->IsTime())
	{
		MoveRange(range, bX?(double)plot->GetXStartTime():(double)plot->GetYStartTime());
	}
	
	if(bX?plot->IsXLogarithm():plot->IsYLogarithm())
	{
		range[0] = log10(range[0]);
		range[1] = log10(range[1]);
	}
	if(bX)
	{
		if(!(check == BST_CHECKED))plot->SetXRange(range[0], range[1]);
		//plot->SetLastXRange(range);
		plot->SetXAutoRange(check == BST_CHECKED);
	}
	else
	{
		if(!(check == BST_CHECKED))plot->SetYRange(range[0], range[1]);
		//plot->SetLastYRange(range);
		plot->SetYAutoRange(check == BST_CHECKED);
	}
	
	//if(bX)plot->SetXAutoRange(check == BST_CHECKED);
	//else plot->SetYAutoRange(check == BST_CHECKED);
	
	bool btx = plot->IsXAutoRange();
	bool bty = plot->IsYAutoRange();
}

template<class PlotImplT>
class ClassForAxisRangeDlg
{
public:
#ifndef _WIN64
	static	BOOL CALLBACK AxisRangeDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT CALLBACK AxisRangeDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static AxisSettingParam<PlotImplT> *para;
		static PlotImplT	*plot;
		static int location;
		
		
		switch(message)
		{
		case WM_INITDIALOG:
			para = (AxisSettingParam<PlotImplT> *)lparam;
			plot = para->plot;
			location = para->location;
			
			// Deal with the title
			SetupDataForRange(hWnd, plot, location);
			return TRUE;
		case WM_COMMAND:
			
			// font buttons are clicked
			if(HIWORD (wparam) == EN_CHANGE)
			{
				switch(LOWORD(wparam))
				{
				case IDC_AS_XRANGE_LOW:
				case IDC_AS_XRANGE_HIGH:
					//bAutoXRange = false;
					SendDlgItemMessage(hWnd, IDC_AS_AUTO_XRANGE, BM_SETCHECK, FALSE, 0);
					return TRUE;
				}
			}
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForRange(hWnd, plot, location);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End
