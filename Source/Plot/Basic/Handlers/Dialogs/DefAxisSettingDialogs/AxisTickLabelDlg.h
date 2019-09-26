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
void SetupDataForTickLabel(HWND hWnd, PlotImplT *plot, int location, LOGFONT &lfLabelFont)
{
	CAxis *axis = plot->GetAxisByLocation(location);
	
	// Deal with the tick label
	SendDlgItemMessage(hWnd, IDC_AS_COLOR_LABEL_FONT, WM_SETCOLOR, 0, (LPARAM)axis->GetTickLabelColor());
	lfLabelFont = axis->GetLabelFont();
	SendDlgItemMessage(hWnd, IDC_AS_LABEL_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &axis->GetLabelFont() ).c_str());
	SendDlgItemMessage(hWnd, IDC_AS_LABEL_SHOW, BM_SETCHECK, axis->IsTickLabelShow(), 0);
	SendDlgItemMessage(hWnd, IDC_AS_LABEL_XTWOSTOREY, BM_SETCHECK, axis->IsTwoStoreyX(), 0);
}

template<class PlotImplT>
void SaveDataForTickLabel(HWND hWnd, PlotImplT *plot, int location, LOGFONT lfLabelFont)
{
	// local varibles
	CAxis	*axis = plot->GetAxisByLocation(location);
	COLORREF color;
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif
	
	// Deal with the tick label
	SendDlgItemMessage(hWnd, IDC_AS_COLOR_LABEL_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
	axis->SetTickLabelColor(color);
	axis->SetLabelFont(lfLabelFont);
	
	check = SendDlgItemMessage(hWnd, IDC_AS_LABEL_SHOW, BM_GETCHECK, 0, 0);
	axis->SetTickLabelShow(check==BST_CHECKED);
	check = SendDlgItemMessage(hWnd, IDC_AS_LABEL_XTWOSTOREY, BM_GETCHECK, 0, 0);
	axis->SetTwoStroeyX(check==BST_CHECKED);
}

template<class PlotImplT>
class ClassForAxisTickLabelDlg
{
public:
#ifndef _WIN64
	static BOOL CALLBACK AxisTickLabelDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
	static LRESULT CALLBACK AxisTickLabelDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
	{
		static AxisSettingParam<PlotImplT> *para;
		static PlotImplT	*plot;
		static int location;
		static LOGFONT	lfLabelFont;
		
		// local varibles
		HDC		hdc;
		LOGFONT	*pLogFont;
		COLORREF color;
		
		int oldHeight;
		
		switch(message)
		{
		case WM_INITDIALOG:
			para = (AxisSettingParam<PlotImplT> *)lparam;
			plot = para->plot;
			location = para->location;
			
			// Deal with the title
			SetupDataForTickLabel(hWnd, plot, location, lfLabelFont);
			return TRUE;
		case WM_COMMAND:
			
			// font buttons are clicked
			if(HIWORD (wparam) == BN_CLICKED)
			{
				if(LOWORD(wparam)==IDC_AS_CHOOSE_LABEL_FONT)
				{
					hdc = GetDC(hWnd);
					
					pLogFont = &lfLabelFont;
					SendDlgItemMessage(hWnd, IDC_PS_COLOR_TITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
					
					oldHeight = pLogFont->lfHeight;
					pLogFont->lfHeight = -MulDiv(oldHeight, GetDeviceCaps(hdc, LOGPIXELSY), 72);
					
					// Font Dialog				
					if( ShowFontDialog(hWnd, *pLogFont, color))
					{
						pLogFont->lfHeight = -MulDiv(pLogFont->lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY));
						
						SendDlgItemMessage(hWnd, IDC_AS_LABEL_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &lfLabelFont ).c_str());
						
					}
					
					ReleaseDC(hWnd, hdc);
				}
			}
			switch(LOWORD(wparam))
			{
			case IDOK:
				if(IsWindowVisible(hWnd))
					SaveDataForTickLabel(hWnd, plot, location, lfLabelFont);
				return TRUE;
			}
			break;
		}
		
		return FALSE;
	}
};

Declare_Namespace_End
