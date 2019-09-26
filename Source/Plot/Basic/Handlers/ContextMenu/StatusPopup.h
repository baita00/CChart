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
#include "../../Global.h"
#include "MyMenuID.h"

Declare_Namespace_CChart

template<class PlotImplT>
void	CreateExtraFunctionPopup(PlotImplT *plot, HMENU subMenu)
{
	HMENU tMenu, ttMenu;
	int				flag;
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		tMenu = CreatePopupMenu();
		flag = GetPopupCheckedFlag( plot->IsOscilloScopeMode() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusExtraFunciton, _TEXT("Show Oscillo &menus"));
		flag = GetPopupCheckedFlag( CGlobal::m_bShowDataProcessingMenu );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusExtraFunciton+1, _TEXT("Enable data &process"));
		flag = GetPopupCheckedFlag( plot->IsRangeZoomMode() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusExtraFunciton+2, _TEXT("Rang&e zoom"));
		
		ttMenu = CreatePopupMenu();
		flag = GetPopupCheckedFlag( plot->IsRangeDrag() );
		AppendMenu(ttMenu, MF_STRING | flag, kMenuStatusEFRangeDrag, _TEXT("&Start"));
		AddRangeDragPopup(ttMenu, _TEXT("Range drag &type"), plot->GetRangeDragType(), kMenuStatusEFRangeDrag+1);
		AppendMenu(tMenu, MF_POPUP | MF_STRING, (UINT)ttMenu, _TEXT("&Range drag"));
		DestroyMenu(ttMenu);

		flag = GetPopupCheckedFlag( plot->IsPointSelectionMode() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusPointSelection, _TEXT("Point &section mode"));
		
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("&Extra functions"));
		DestroyMenu(tMenu);
		break;
	case kLangChinese:
		tMenu = CreatePopupMenu();
		flag = GetPopupCheckedFlag( plot->IsOscilloScopeMode() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusExtraFunciton, _TEXT("示波器模式菜单(&M)"));
		flag = GetPopupCheckedFlag( CGlobal::m_bShowDataProcessingMenu );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusExtraFunciton+1, _TEXT("允许数据处理(&P)"));
		flag = GetPopupCheckedFlag( plot->IsRangeZoomMode() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusExtraFunciton+2, _TEXT("坐标缩放(&E)"));
		
		ttMenu = CreatePopupMenu();
		flag = GetPopupCheckedFlag( plot->IsRangeDrag() );
		AppendMenu(ttMenu, MF_STRING | flag, kMenuStatusEFRangeDrag, _TEXT("开始(&S)"));
		AddRangeDragPopup(ttMenu, _TEXT("坐标拖动模式(&T)"), plot->GetRangeDragType(), kMenuStatusEFRangeDrag+1);
		AppendMenu(tMenu, MF_POPUP | MF_STRING, (UINT)ttMenu, _TEXT("坐标拖动(&R)"));
		DestroyMenu(ttMenu);

		flag = GetPopupCheckedFlag( plot->IsPointSelectionMode() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusPointSelection, _TEXT("点选模式(&s)"));
		
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("特别功能(&E)"));
		DestroyMenu(tMenu);
		break;
	}
}

template<class PlotImplT>
void	CreateTooltipPopup(PlotImplT *plot, HMENU subMenu)
{
	HMENU tMenu;
	int flag;
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		tMenu = CreatePopupMenu();
		flag = GetPopupCheckedFlag( CGlobal::m_bEnableToolTip );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusTooltip, _TEXT("&Enable tool tip"));
		AddTooltipTypePopup(tMenu, _T("&Tool tip type"), plot->GetPointDesType(), kMenuStatusTooltip+1);
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("&Tool tip"));
		DestroyMenu(tMenu);
		break;
	case kLangChinese:
		tMenu = CreatePopupMenu();
		flag = GetPopupCheckedFlag( CGlobal::m_bEnableToolTip );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusTooltip, _TEXT("&启用工具提示"));
		AddTooltipTypePopup(tMenu, _T("&工具提示类型"), plot->GetPointDesType(), kMenuStatusTooltip+1);
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("工具提示信息(&T)"));
		DestroyMenu(tMenu);
		break;
	}
}

template<class PlotImplT>
void	CreateStatusPopup(PlotImplT *plot, HMENU menu)
{
	HMENU subMenu, tMenu;
	int				flag;

	subMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		if(!plot->IsNoDataPlot())
		{
			flag = GetPopupCheckedFlag( plot->IsAutoDataColorGeneral() );
			AppendMenu(subMenu, MF_STRING | flag, kMenuStatus, _TEXT("Automatic line &color"));
			flag = GetPopupCheckedFlag( plot->IsAutoLineStyleGeneral() );
			AppendMenu(subMenu, MF_STRING | flag, kMenuStatus+1, _TEXT("Automatic line &style"));
			AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		
			CreateExtraFunctionPopup(plot, subMenu);
		}

		CreateTooltipPopup(plot, subMenu);

		tMenu = CreatePopupMenu();
		flag = GetPopupCheckedFlag( plot->IsFloatXTicks() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusFloatTicks, _TEXT("&X axis"));
		flag = GetPopupCheckedFlag( plot->IsFloatYTicks() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusFloatTicks+1, _TEXT("&Y axis"));
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("&Float Ticks mode"));
		DestroyMenu(tMenu);

		tMenu = CreatePopupMenu();
		flag = GetPopupCheckedFlag( plot->IsEqualXYAxis() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusEqual, _TEXT("&Square rect"));
		flag = GetPopupCheckedFlag( plot->IsEqualXYRange() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusEqual+1, _TEXT("&Equal range"));
		flag = GetPopupCheckedFlag( plot->IsEqualXYScale() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusEqual+2, _TEXT("E&qual scale"));
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("&Inter axes"));
		DestroyMenu(tMenu);
		AddApperanceTypePopup(subMenu, _T("&Apperance mode"), plot->GetApperanceMode(), kMenuStatusApperance);
		AppendMenu(menu, MF_POPUP | MF_STRING, (UINT)subMenu, _TEXT("&Plot Status"));
		break;
	case kLangChinese:
		if(!plot->IsNoDataPlot())
		{
			flag = GetPopupCheckedFlag( plot->IsAutoDataColorGeneral() );
			AppendMenu(subMenu, MF_STRING | flag, kMenuStatus, _TEXT("自动选择线色(&C)"));
			flag = GetPopupCheckedFlag( plot->IsAutoLineStyleGeneral() );
			AppendMenu(subMenu, MF_STRING | flag, kMenuStatus+1, _TEXT("自动选择线型(&S)"));
			AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );

			CreateExtraFunctionPopup(plot, subMenu);

		}

		CreateTooltipPopup(plot, subMenu);			

		tMenu = CreatePopupMenu();
		flag = GetPopupCheckedFlag( plot->IsFloatXTicks() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusFloatTicks, _TEXT("&X轴"));
		flag = GetPopupCheckedFlag( plot->IsFloatYTicks() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusFloatTicks+1, _TEXT("&Y轴"));
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("浮动刻度模式(&F)"));
		DestroyMenu(tMenu);

		tMenu = CreatePopupMenu();
		flag = GetPopupCheckedFlag( plot->IsEqualXYAxis() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusEqual, _TEXT("绘图区域正方(&)"));
		flag = GetPopupCheckedFlag( plot->IsEqualXYRange() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusEqual+1, _TEXT("等数据范围(&E)"));
		flag = GetPopupCheckedFlag( plot->IsEqualXYScale() );
		AppendMenu(tMenu, MF_STRING | flag, kMenuStatusEqual+2, _TEXT("等比例尺(&Q)"));
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("坐标轴关系(&)"));
		DestroyMenu(tMenu);

		AddApperanceTypePopup(subMenu, _T("外观模式(&A)"), plot->GetApperanceMode(), kMenuStatusApperance);
		AppendMenu(menu, MF_POPUP | MF_STRING, (UINT)subMenu, _TEXT("绘图状态(&P)"));
		break;
	}
	DestroyMenu(subMenu);
}

template<class PlotImplT>
int		InterpretExtraFunctionPopupItem( PlotImplT *plot, HWND hWnd, int result )
{
	if(result == kMenuStatusExtraFunciton)
	{
		plot->SetRangeSet(false, false);
		plot->SetOscilloScopeMode(!plot->IsOscilloScopeMode());
		plot->SetStartOscilloScope(plot->IsOscilloScopeMode());
		return kPopupUpdate;
	}
	else if(result == kMenuStatusExtraFunciton+1)
	{
		plot->SetRangeSet(false, false);
		CGlobal::m_bShowDataProcessingMenu = !CGlobal::m_bShowDataProcessingMenu;
		return kPopupUpdate;
	}
	else if(result == kMenuStatusExtraFunciton+2)
	{
		plot->SetRangeSet(false, false);
		plot->SetRangeZoomMode(!plot->IsRangeZoomMode());
		return kPopupUpdate;
	}
	else if(result == kMenuStatusEFRangeDrag)
	{
		if(!plot->IsRangeDrag())
		{
			plot->SetFloatXTicksOld(plot->IsFloatXTicks());
			plot->SetFloatXTicks(true);
			
			plot->SetFloatYTicksOld(plot->IsFloatYTicks());
			plot->SetFloatYTicks(true);
		}
		else
		{
			plot->SetFloatXTicks(plot->IsFloatXTicksOld());
			plot->SetFloatYTicks(plot->IsFloatYTicksOld());
		}
		plot->SetRangeSet(false, false);
		plot->SetRangeDrag(!plot->IsRangeDrag());
		return kPopupUpdate;
	}
	else if(result >= kMenuStatusEFRangeDrag+1 && result<kMenuStatusEFRangeDrag+1+kRangeDragCount)
	{
		plot->SetRangeDragType(result - kMenuStatusEFRangeDrag - 1);
		return kPopupUpdate;
	}
	else if(result == kMenuStatusPointSelection)
	{
		plot->SetPointSelectionMode(!plot->IsPointSelectionMode());
		return kPopupUpdate;
	}
	return kPopupNoAction;
}

template<class PlotImplT>
int		InterpretTooltipPopupItem( PlotImplT *plot, HWND hWnd, int result )
{
	if(result == kMenuStatusTooltip)
	{
		CGlobal::m_bEnableToolTip = !CGlobal::m_bEnableToolTip;
		return kPopupNoAction;
	}
	else if(result >= kMenuStatusTooltip+1 && result < kMenuStatusTooltip + 1 + kDesCount)
	{
		plot->SetPointDesType(result - kMenuStatusTooltip - 1);
	}
	return kPopupNoAction;
}

template<class PlotImplT>
int		InterpretStatusPopupItem( PlotImplT *plot, HWND hWnd, int result )
{
	if(result == kMenuStatus)
	{
		plot->SetAutoDataColorGeneral(!plot->IsAutoDataColorGeneral());
		return kPopupUpdate;
	}
	else if(result == kMenuStatus + 1)
	{
		plot->SetAutoLineStyleGeneral(!plot->IsAutoLineStyleGeneral());
		return kPopupUpdate;
	}
	else if(result>=kMenuStatusExtraFunciton && result < kMenuStatusExtraFunciton + nMenuStatusExtraFunction)
	{
		return InterpretExtraFunctionPopupItem(plot, hWnd, result);
	}
	else if(result >= kMenuStatusTooltip && result < kMenuStatusTooltip + nMenuStatusTooltip)
	{
		InterpretTooltipPopupItem(plot, hWnd, result);
	}
	else if(result == kMenuStatusFloatTicks)
	{
		plot->SetFloatXTicks(!plot->IsFloatXTicks());
		plot->SetFloatXTicksOld(plot->IsFloatXTicks());
		return kPopupUpdate;
	}
	else if(result == kMenuStatusFloatTicks+1)
	{
		plot->SetFloatYTicks(!plot->IsFloatYTicks());
		plot->SetFloatYTicksOld(plot->IsFloatYTicks());
		return kPopupUpdate;
	}
	else if(result - kMenuStatusEqual >=0 && result - kMenuStatusEqual<nMenuStatusEqual)
	{
		if(result == kMenuStatusEqual)
		{
			plot->SetEqualXYAxis(!plot->IsEqualXYAxis());
			return kPopupUpdate;
		}
		else if(result == kMenuStatusEqual+1)
		{
			plot->SetEqualXYRange(!plot->IsEqualXYRange());
			return kPopupUpdate;
		}
		else if(result == kMenuStatusEqual+1)
		{
			plot->SetEqualXYScale(!plot->IsEqualXYScale());
			return kPopupUpdate;
		}
	}
	else if(result - kMenuStatusApperance >= 0 && result - kMenuStatusApperance < kApperanceModeCount)
	{
		plot->SetApperanceMode(result-kMenuStatusApperance);
		return kPopupUpdate;
	}
	return kPopupNoAction;
}

template<class PlotImplT>
bool	IsStatusItem(PlotImplT *plot, int result)
{
	return (result >= kMenuStatus && result < kMenuStatus + nMenuStatus);
}

Declare_Namespace_End
