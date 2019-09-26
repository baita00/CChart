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
#include "../../MyString.h"

#include "../../Global.h"
#include "MyMenuID.h"

Declare_Namespace_CChart

template<class PlotImplT>
void	AddTitlePopup(PlotImplT *plot, HMENU addMenu, tstring titleString, int startVal)
{
	HMENU			subMenu;
	
	subMenu = CreatePopupMenu();
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		if( plot->IsTitleRealyShow() )
			AppendMenu(subMenu, MF_STRING, startVal, _TEXT("&Hide It") );
		else
			AppendMenu(subMenu, MF_STRING, startVal, _TEXT("&Show It") );
		AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		AddFontSizePopup( subMenu, _TEXT("S&ize"), plot->GetTitleFont().lfHeight, startVal+1 );
		AppendMenu(subMenu, MF_STRING, startVal+1+nFontSizes, _TEXT("&Color...") );
		AppendMenu(subMenu, MF_STRING, startVal+1+nFontSizes+1, _TEXT("&Font...") );
		AddTitlePositionPopup(subMenu, _TEXT("&Position"), plot->GetTitlePosition()+1, kMenuTitlePosition);
		break;
	case kLangChinese:
		if( plot->IsTitleRealyShow() )
			AppendMenu(subMenu, MF_STRING, startVal, _TEXT("隐藏(&H)") );
		else
			AppendMenu(subMenu, MF_STRING, startVal, _TEXT("显示(&S)") );
		AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		AddFontSizePopup( subMenu, _TEXT("尺寸(&S)"), plot->GetTitleFont().lfHeight, startVal+1 );
		AppendMenu(subMenu, MF_STRING, startVal+1+nFontSizes, _TEXT("颜色(&C)...") );
		AppendMenu(subMenu, MF_STRING, startVal+1+nFontSizes+1, _TEXT("字体(&F)...") );
		AddTitlePositionPopup(subMenu, _TEXT("位置(&P)"), plot->GetTitlePosition()+1, kMenuTitlePosition);
		break;
	}
	
	AppendMenu(addMenu, MF_POPUP | MF_STRING, (UINT) subMenu, titleString.c_str() );
	
	DestroyMenu(subMenu);
}

template<class PlotImplT>
void	CreateTitlePopup(PlotImplT *plot, HMENU menu)
{
	HMENU subMenu;
	if( plot->GetTitle() != _TEXT("") )
	{
		subMenu = CreatePopupMenu();
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			AddTitlePopup(plot, subMenu, _TEXT("&Main Title"), kMenuTitle);
			break;
		case kLangChinese:
			AddTitlePopup(plot, subMenu, _TEXT("主标题(&M)"), kMenuTitle);
			break;
		}

		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, _TEXT("&Titles") );
			break;
		case kLangChinese:
			AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, _TEXT("标题选项(&T)") );
			break;
		}
		DestroyMenu(subMenu);
	}
}

template<class PlotImplT>
int		InterpretTitlePopupItem( PlotImplT *plot, HWND hWnd, int result )
{
	COLORREF color;
	LOGFONT logfont;

	if( result == kMenuTitle )
	{
		plot->SetTitleShow( !plot->IsTitleShow() );
		return kPopupUpdate;
	}
	if( result >= kMenuTitle + 1 && result < kMenuTitle + 1 + nFontSizes )
	{
		plot->GetTitleFont().lfHeight = fontSizes[result-kMenuTitle-1];
		return kPopupUpdate;
	}
	else if( result == kMenuTitle + 1 + nFontSizes )
	{
		color = plot->GetTitleColor();
		if(ShowColorDialog(hWnd, color))
		{
			plot->SetTitleColor(color);
			return kPopupUpdate;
		}
	}
	else if( result == kMenuTitle + 1 + nFontSizes +1 )
	{
		logfont = plot->GetTitleFont();
		if(ShowFontDialog(hWnd, logfont, color))
		{
			plot->SetTitleFont(logfont);
			return kPopupUpdate;
		}
	}
	else if( result >= kMenuTitlePosition && result < kMenuTitlePosition + nMenuTitlePosition)
	{
		plot->SetTitleAlignment(result-kMenuTitlePosition-1);
		return kPopupUpdate;
	}
	return kPopupNoAction;
}

template<class PlotImplT>
bool	IsTitleItem(PlotImplT *plot, int result)
{
	return (result >= kMenuTitle && result < kMenuTitle + nMenuTitle);
}

Declare_Namespace_End