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

#include "UsedPopup.h"

#include "TitlePopup.h"
#include "AxisPopup.h"
#include "BkgndPopup.h"
#include "LegendPopup.h"
#include "DataPopup.h"
#include "OscilloPopup.h"
#include "StatusPopup.h"
#include "ImagePopup.h"

Declare_Namespace_CChart

template<class PlotImplT>
HMENU	CreateContextMenu(PlotImplT *plot)
{
	HMENU			menu;

	menu = CreatePopupMenu();

	// Check for a title, and put in title menu if needed
	CreateTitlePopup(plot, menu);
	CreateAxisPopup(plot, menu);
	CreateBkgndPopup(plot, menu);
	CreateLegendPopup(plot, menu);
	CreateDataPopup(plot, menu);
	CreateOscilloPopup(plot, menu);
	CreateStatusPopup(plot, menu);

	// For the Additional menus
	if(plot->m_pFcnAddPluginMenu && plot->m_pFcnInterpretPluginMenu)
	{
		plot->m_pFcnAddPluginMenu(plot, menu, plot->m_pParaAddPlugInMenu);
	}
	
	// Image
	if(CGlobal::m_bImagePopupMenu)CreateImagePopup(plot, menu);

	// language
	tstring thetitle;
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		thetitle = _TEXT("&Langurage");
		break;
	case kLangChinese:
		thetitle = _TEXT("语言选项(&L)");
		break;
	}	
	AddLanguragePopup(menu, thetitle, CGlobal::m_nLangurage, kMenuLang);

	// Settings...
	AppendMenu(menu, MF_SEPARATOR, 0, (LPCTSTR)0 );

	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(menu, MF_STRING, kMenuPlotSettings, _TEXT("&Plot Settings...") );
		break;
	case kLangChinese:
		AppendMenu(menu, MF_STRING, kMenuPlotSettings, _TEXT("绘图设置(&P)...") );
		break;
	}

	return menu;
}

// Take the result of the popup menu selection, figure out
// what the user wanted, and take the appropriate action
template<class PlotImplT>
int		InterpretPopupItem( PlotImplT *plot, HWND hWnd, int result )
{
//	int			whichAxis, returnVal;
	tstring		filename, fileext, filepath;

	COLORREF	color=0;
//	LOGFONT		logfont;

	//if( result >= kMinimumPopupValue ) return result;

	// Grids
	if( result <= 0 ) return kPopupError;

	// Title
	else if(IsTitleItem(plot, result))
	{
		return InterpretTitlePopupItem(plot, hWnd, result);
	}

	// Axes
	else if( IsAxisItem(plot, result) )
	{
		return InterpretAxisPopupItem(plot, hWnd, result);
	}

	// Background
	else if( IsBkgndItem(plot, result) )//gradient back ground
	{
		return InterpretBkgndPopupItem(plot, hWnd, result);
	}

	// Legend
	else if( IsLegendItem(plot, result) )//use legend
	{
		return InterpretLegendPopupItem(plot, hWnd, result);
	}

	// Dataset
	else if( IsDataItem(plot, result))
	{
		return InterpretDataPopupItem(plot, hWnd, result);
	}

	// The image
	else if(IsImageItem(plot, result))
	{
		return InterpretImagePopupItem(plot, hWnd, result);
	}
	
	// for the oscilloscope
	else if(IsOscilloItem(plot, result))
	{
		return InterpretOscilloPopupItem(plot, hWnd, result);
	}
	

	// for the status menu
	else if(IsStatusItem(plot, result))
	{
		return InterpretStatusPopupItem(plot, hWnd, result);
	}

	// for the langurage menu
	else if(result >= kMenuLang && result < kMenuLang + nMenuLang)
	{
		CGlobal::m_nLangurage = result - kMenuLang;
		return kPopupUpdate;
	}

	// The last menu, ^_^
	else if( result == kMenuPlotSettings )
	{
		HDC hDC = GetDC(hWnd);
		int ret;
		if( DefPlotSettings(plot, hDC) ) ret = kPopupUpdate; else ret = kPopupNoAction;
		ReleaseDC(hWnd, hDC);
		return ret;
	}

	// The additional context menus
	else
	{
		if(plot->m_pFcnInterpretPluginMenu && plot->m_pFcnAddPluginMenu)
		{
			//plot->m_pParaPlugInMenu = plot;
			int ret = plot->m_pFcnInterpretPluginMenu(plot, hWnd, result, plot->m_pParaInterpretPlugInMenu);
			if(ret) return ret;
		}
	}

	return kPopupError;
}

template<class PlotImplT>
int DefContextMenu( PlotImplT *plot, HMENU hMenu, HDC hDC, POINT point )
{
	HMENU			menu;
	int				result;
	
	HWND	hWnd = WindowFromDC(hDC);
	
	if(!IsWindow(hWnd))return kPopupError;
	//	if( plot->IsEmpty() )return kPopupError;
	if(!plot->m_bContextMenu)return kPopupNoAction;
	
	if( IsMenu(hMenu) )
	{
		result = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, 0, hWnd, NULL);
		SendMessage( hWnd, WM_COMMAND, (WPARAM)result, 0);
		return kPopupNoAction;
	}
	
	menu = CreateContextMenu(plot);
	result = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, 0, hWnd, NULL);
	DestroyMenu(menu);
	
	return InterpretPopupItem( plot, hWnd, result );
}

Declare_Namespace_End