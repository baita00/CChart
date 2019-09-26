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
void	SetupDataForRange(HWND hWnd, PlotImplT *plot)
{
	//if(plot->IsNoDataPlot())return;

	double xRange[2],yRange[2];
	tostringstream ostr;
	tstring str;
	plot->GetLastPlotRange(xRange, yRange);

	if(plot->IsXTime())
	{
		MoveRange(xRange, -(double)plot->GetXStartTime());
	}
	if(plot->IsYTime())
	{
		MoveRange(yRange, -(double)plot->GetYStartTime());
	}
	
	ostr.str(_TEXT(""));
	if(!plot->IsXLogarithm())
		ostr<<xRange[1];
	else
		ostr<<pow(10.0, xRange[1]);
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_XRANGE_HIGH, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	
	ostr.str(_TEXT(""));
	if(!plot->IsXLogarithm())
		ostr<<xRange[0];
	else
		ostr<<pow(10.0, xRange[0]);
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_XRANGE_LOW, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	
	ostr.str(_TEXT(""));
	if(!plot->IsYLogarithm())
		ostr<<yRange[1];
	else
		ostr<<pow(10.0, yRange[1]);
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_YRANGE_HIGH, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	
	ostr.str(_TEXT(""));
	if(!plot->IsYLogarithm())
		ostr<<yRange[0];
	else
		ostr<<pow(10.0, yRange[0]);
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_YRANGE_LOW, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	SendDlgItemMessage(hWnd, IDC_PS_AUTO_XRANGE, BM_SETCHECK, plot->IsXAutoRange(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_AUTO_YRANGE, BM_SETCHECK, plot->IsYAutoRange(), 0);

	SendDlgItemMessage(hWnd, IDC_PS_CHECK_XRANGEEXACT, BM_SETCHECK, plot->IsExactXRange(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_CHECK_YRANGEEXACT, BM_SETCHECK, plot->IsExactYRange(), 0);

	SendDlgItemMessage(hWnd, IDC_PS_CHECK_XRANGEOPTEXACT, BM_SETCHECK, plot->IsOptimalExactXRange(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_CHECK_YRANGEOPTEXACT, BM_SETCHECK, plot->IsOptimalExactYRange(), 0);

	SendDlgItemMessage(hWnd, IDC_PS_CHECK_XRANGEOPTZEROREPAIR, BM_SETCHECK, plot->IsOptimalXZeroRepair(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_CHECK_YRANGEOPTZEROREPAIR, BM_SETCHECK, plot->IsOptimalYZeroRepair(), 0);

	SendDlgItemMessage(hWnd, IDC_PS_CHECK_XRANGEOPTSYMREPAIR, BM_SETCHECK, plot->IsOptimalXSymetryRepair(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_CHECK_YRANGEOPTSYMREPAIR, BM_SETCHECK, plot->IsOptimalYSymetryRepair(), 0);

	SendDlgItemMessage(hWnd, IDC_PS_CHECK_XLRANGEOPTEXTEND, BM_SETCHECK, plot->IsOptimalXlExtend(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_CHECK_YLRANGEOPTEXTEND, BM_SETCHECK, plot->IsOptimalYlExtend(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_CHECK_XURANGEOPTEXTEND, BM_SETCHECK, plot->IsOptimalXuExtend(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_CHECK_YURANGEOPTEXTEND, BM_SETCHECK, plot->IsOptimalYuExtend(), 0);
}

template<class PlotImplT>
void	SaveDataForRange(HWND hWnd, PlotImplT *plot)
{
	//if(plot->IsNoDataPlot())return;

	double xRange[2], yRange[2];
	TCHAR strtmp[nMaxString];
#ifndef _WIN64
	int			check;
#else
	LRESULT		check;
#endif
	
	GetWindowText(GetDlgItem(hWnd, IDC_PS_XRANGE_LOW), strtmp, nMaxString);
	xRange[0]=_tcstod(strtmp, 0);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_XRANGE_HIGH), strtmp, nMaxString);
	xRange[1]=_tcstod(strtmp, 0);
	if(plot->IsXTime())MoveRange(xRange, (double)plot->GetXStartTime());
	
	check = SendDlgItemMessage(hWnd, IDC_PS_AUTO_XRANGE, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetXAutoRange(true);
		
	}
	else if(check==BST_UNCHECKED)
	{
		if(!plot->IsXLogarithm())
			plot->SetXRange(xRange[0], xRange[1]);
		else
			plot->SetXRange(log10(xRange[0]), log10(xRange[1]));

		plot->SetXAutoRange(false);
//		plot->GetXMainAxis()->SetRangeSet(false);
	}
	
	GetWindowText(GetDlgItem(hWnd, IDC_PS_YRANGE_LOW), strtmp, nMaxString);
	yRange[0]=_tcstod(strtmp, 0);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_YRANGE_HIGH), strtmp, nMaxString);
	yRange[1]=_tcstod(strtmp, 0);
	if(plot->IsYTime())MoveRange(yRange, (double)plot->GetYStartTime());

	check = SendDlgItemMessage(hWnd, IDC_PS_AUTO_YRANGE, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetYAutoRange(true);
		
	}
	else if(check==BST_UNCHECKED)
	{
		if(!plot->IsYLogarithm())
			plot->SetYRange(yRange[0], yRange[1]);
		else
			plot->SetYRange(log10(yRange[0]), log10(yRange[1]));

		plot->SetYAutoRange(false);
//		plot->GetYMainAxis()->SetRangeSet(false);
	}

	check = SendDlgItemMessage(hWnd, IDC_PS_CHECK_XRANGEEXACT, BM_GETCHECK, 0, 0);
	plot->SetExactXRange(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_CHECK_YRANGEEXACT, BM_GETCHECK, 0, 0);
	plot->SetExactYRange(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_CHECK_XRANGEOPTEXACT, BM_GETCHECK, 0, 0);
	plot->SetOptimalExactXRange(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_CHECK_YRANGEOPTEXACT, BM_GETCHECK, 0, 0);
	plot->SetOptimalExactYRange(check==BST_CHECKED);

	check = SendDlgItemMessage(hWnd, IDC_PS_CHECK_XRANGEOPTZEROREPAIR, BM_GETCHECK, 0, 0);
	plot->SetOptimalXZeroRepair(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_CHECK_YRANGEOPTZEROREPAIR, BM_GETCHECK, 0, 0);
	plot->SetOptimalYZeroRepair(check==BST_CHECKED);

	check = SendDlgItemMessage(hWnd, IDC_PS_CHECK_XRANGEOPTSYMREPAIR, BM_GETCHECK, 0, 0);
	plot->SetOptimalXSymetryRepair(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_CHECK_YRANGEOPTSYMREPAIR, BM_GETCHECK, 0, 0);
	plot->SetOptimalYSymetryRepair(check==BST_CHECKED);

	check = SendDlgItemMessage(hWnd, IDC_PS_CHECK_XLRANGEOPTEXTEND, BM_GETCHECK, 0, 0);
	plot->SetOptimalXlExtend(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_CHECK_YLRANGEOPTEXTEND, BM_GETCHECK, 0, 0);
	plot->SetOptimalYlExtend(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_CHECK_XURANGEOPTEXTEND, BM_GETCHECK, 0, 0);
	plot->SetOptimalXuExtend(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_PS_CHECK_YURANGEOPTEXTEND, BM_GETCHECK, 0, 0);
	plot->SetOptimalYuExtend(check==BST_CHECKED);
}

template<class PlotImplT>
class ClassForPlotRangesDlg
{
public:
	
#ifndef _WIN64
	static	BOOL	CALLBACK PlotRangesDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static	LRESULT	CALLBACK PlotRangesDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static PlotImplT	*plot;
		
		// local varibles
		
		LPNMHDR pnmh;
		
		switch(message)
		{
		case WM_INITDIALOG:
			plot = (PlotImplT *)((LPPROPSHEETPAGE)lparam)->lParam;
			
			SetupDataForRange(hWnd, plot); 
			
			return TRUE;
		case WM_COMMAND:
			
			if(HIWORD (wparam) == EN_CHANGE)
			{
				switch(LOWORD(wparam))
				{
				case IDC_PS_XRANGE_HIGH:
				case IDC_PS_XRANGE_LOW:
					SendDlgItemMessage(hWnd, IDC_PS_AUTO_XRANGE, BM_SETCHECK, FALSE, 0);
					return TRUE;
				case IDC_PS_YRANGE_HIGH:
				case IDC_PS_YRANGE_LOW:
					SendDlgItemMessage(hWnd, IDC_PS_AUTO_YRANGE, BM_SETCHECK, FALSE, 0);
					return TRUE;
				}
			}
			break;
			
		case WM_NOTIFY:
			pnmh = (LPNMHDR) lparam;
			switch(pnmh->code)
			{
			case PSN_APPLY:
				// Apply Changes
				if(IsWindowVisible(hWnd))
					SaveDataForRange(hWnd, plot);
				return TRUE;
			}
			break;
		}
		return FALSE;
	}
};

Declare_Namespace_End
