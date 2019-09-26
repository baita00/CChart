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

// Adds the menus that pertain to the axis, including auto ranging,
// title size, label size, tick marks, etc. This is only called
// by the popup menu constructors for the chart types.
template<class PlotImplT>
void	AddAxisPopup( PlotImplT *plot, HMENU menu, int which, int startVal )
{
	UINT			flag;
	tstring			str;
	HMENU			axisMenu, subMenu;
	
	axisMenu = CreatePopupMenu();

	CAxisImpl *axis = plot->CPlotImpl::GetAxis(which);
	bool bX = axis->IsXAxis();
	
	flag = GetPopupCheckedFlag( bX?plot->IsXAutoRange():plot->IsYAutoRange() );
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(axisMenu, MF_STRING | flag, startVal, _TEXT("&Auto range") );
		break;
	case kLangChinese:
		AppendMenu(axisMenu, MF_STRING | flag, startVal, _TEXT("自动确定范围(&A)") );
		break;
	}

	AppendMenu(axisMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	// Font sizes
	subMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsShowTitle() );
		AppendMenu(subMenu, MF_STRING | flag, startVal+kMenuAxisTitleShow, _TEXT("&Show title") );
		AddFontSizePopup( subMenu, _TEXT("&Title size"), plot->CPlotImpl::GetAxis(which)->GetTitleFont().lfHeight, startVal+kMenuAxisTitleSize );
		AddAxisTitlePositionPopup(plot->CPlotImpl::GetAxis(which), subMenu, _TEXT("&Position"), plot->CPlotImpl::GetAxis(which)->GetTitlePosition()+1, startVal+kMenuAxisTitlePosition);

		if(!plot->CPlotImpl::GetAxis(which)->IsXAxis())
		{
			flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsYTitleVerticle() );
			AppendMenu(subMenu, MF_STRING | flag, startVal+kMenuAxisTitleYVerticle, _TEXT("&Verticle") );
		}
		
		AppendMenu(axisMenu, MF_POPUP, (UINT)subMenu, _TEXT("&Title"));
		break;
	case kLangChinese:
		flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsShowTitle() );
		AppendMenu(subMenu, MF_STRING | flag, startVal+kMenuAxisTitleShow, _TEXT("显示标题(&S)") );
		AddFontSizePopup( subMenu, _TEXT("标题大小(&T)"), plot->CPlotImpl::GetAxis(which)->GetTitleFont().lfHeight, startVal+kMenuAxisTitleSize );
		AddAxisTitlePositionPopup(plot->CPlotImpl::GetAxis(which), subMenu, _TEXT("位置(&P)"), plot->CPlotImpl::GetAxis(which)->GetTitlePosition()+1, startVal+kMenuAxisTitlePosition);
		
		if(!plot->CPlotImpl::GetAxis(which)->IsXAxis())
		{
			flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsYTitleVerticle() );
			AppendMenu(subMenu, MF_STRING | flag, startVal+kMenuAxisTitleYVerticle, _TEXT("标题竖排(&V)") );
		}
		
		AppendMenu(axisMenu, MF_POPUP, (UINT)subMenu, _TEXT("标题(&T)"));
		break;
	}
	DestroyMenu(subMenu);

	subMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsTickLabelShow() );
		AppendMenu(subMenu, MF_STRING | flag, startVal+kMenuAxisLabelShow, _TEXT("&Show labels") );
		if(plot->CPlotImpl::GetAxis(which)->IsXAxis())
		{
			flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsTwoStoreyX() );
			AppendMenu(subMenu, MF_STRING | flag, startVal+kMenuAxisLabelShow+1, _TEXT("S&plit level") );
		}
		
		AddFontSizePopup( subMenu, _TEXT("&Label size"), plot->CPlotImpl::GetAxis(which)->GetLabelFont().lfHeight, startVal+kMenuAxisLabelSize );
		AppendMenu(axisMenu, MF_POPUP, (UINT)subMenu, _TEXT("&Labels"));
		break;
	case kLangChinese:
		flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsTickLabelShow() );
		AppendMenu(subMenu, MF_STRING | flag, startVal+kMenuAxisLabelShow, _TEXT("显示刻度值(&S)") );
		if(plot->CPlotImpl::GetAxis(which)->IsXAxis())
		{
			flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsTwoStoreyX() );
			AppendMenu(subMenu, MF_STRING | flag, startVal+kMenuAxisLabelShow+1, _TEXT("错层排列(&P)") );
		}
		
		AddFontSizePopup( subMenu, _TEXT("刻度值大小(&L)"), plot->CPlotImpl::GetAxis(which)->GetLabelFont().lfHeight, startVal+kMenuAxisLabelSize );
		AppendMenu(axisMenu, MF_POPUP, (UINT)subMenu, _TEXT("刻度值(&L)"));
		break;
	}
	DestroyMenu(subMenu);
	
	AppendMenu(axisMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	
	// Number of major and minor ticks
	//AddNumberOfTicksPopup( &axisMenu, "Major ticks", m_nTickCount+1, startVal + kMenuAxisMajorTickCount );
	//AddNumberOfTicksPopup( &axisMenu, "Minor ticks", m_nMinorTickCount, startVal + kMenuAxisMinorTickCount );
	subMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AddNumberOfTicksPopup( subMenu, _TEXT("&Number"), (bX?plot->GetXTickCount():plot->GetYTickCount())+1, startVal + kMenuAxisMajorTickCount, bX?plot->IsAutoMajorTickX():plot->IsAutoMajorTickY() );
		AddAxisTickPositionPopup( subMenu, _TEXT("&Position"), plot->CPlotImpl::GetAxis(which)->GetTickPosition(), startVal + kMenuAxisMajorTickPos);
		AppendMenu(axisMenu, MF_POPUP, (UINT)subMenu, _TEXT("&Major ticks"));
		break;
	case kLangChinese:
		AddNumberOfTicksPopup( subMenu, _TEXT("数量(&N)"), (bX?plot->GetXTickCount():plot->GetYTickCount())+1, startVal + kMenuAxisMajorTickCount, bX?plot->IsAutoMajorTickX():plot->IsAutoMajorTickY() );
		AddAxisTickPositionPopup( subMenu, _TEXT("位置(&P)"), plot->CPlotImpl::GetAxis(which)->GetTickPosition(), startVal + kMenuAxisMajorTickPos);
		AppendMenu(axisMenu, MF_POPUP, (UINT)subMenu, _TEXT("主刻度线(&M)"));
		break;
	}
	DestroyMenu(subMenu);

	subMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AddNumberOfTicksPopup( subMenu, _TEXT("&Number"), bX?plot->GetXMinorTickCount():plot->GetYMinorTickCount(), startVal + kMenuAxisMinorTickCount, bX?plot->IsAutoMinorTickX():plot->IsAutoMinorTickY() );
		AddAxisTickPositionPopup( subMenu, _TEXT("&Position"), plot->CPlotImpl::GetAxis(which)->GetMinorTickPosition(), startVal + kMenuAxisMinorTickPos);
		AppendMenu(axisMenu, MF_POPUP, (UINT)subMenu, _TEXT("M&inor ticks"));
		break;
	case kLangChinese:
		AddNumberOfTicksPopup( subMenu, _TEXT("数量(&N)"), bX?plot->GetXMinorTickCount():plot->GetYMinorTickCount(), startVal + kMenuAxisMinorTickCount, bX?plot->IsAutoMinorTickX():plot->IsAutoMinorTickY() );
		AddAxisTickPositionPopup( subMenu, _TEXT("位置(&P)"), plot->CPlotImpl::GetAxis(which)->GetMinorTickPosition(), startVal + kMenuAxisMinorTickPos);
		AppendMenu(axisMenu, MF_POPUP, (UINT)subMenu, _TEXT("副刻度线(&M)"));
		break;
	}
	DestroyMenu(subMenu);
	
	// Add it to main menu
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		switch( plot->CPlotImpl::GetAxis(which)->GetLocation() )
		{
		case kLocationLeft: str = _TEXT("&Left Axis"); break;
		case kLocationRight: str = _TEXT("&Right Axis"); break;
		case kLocationTop: str = _TEXT("&Top Axis"); break;
		case kLocationBottom: str = _TEXT("&Bottom Axis"); break;
			
		case kLocationCenterVL: str = _TEXT("&Center VL"); break;
		case kLocationCenterVR: str = _TEXT("&Center VR"); break;
		case kLocationCenterHB: str = _TEXT("&Center HB"); break;
		case kLocationCenterHT: str = _TEXT("&Center HT"); break;
		default: str = _TEXT("Error"); break;
		}
		break;
	case kLangChinese:
		switch( plot->CPlotImpl::GetAxis(which)->GetLocation() )
		{
		case kLocationLeft: str = _TEXT("左轴(&L)"); break;
		case kLocationRight: str = _TEXT("右轴(&R)"); break;
		case kLocationTop: str = _TEXT("上轴(&T)"); break;
		case kLocationBottom: str = _TEXT("下轴(&B)"); break;
			
		case kLocationCenterVL: str = _TEXT("中轴竖直偏左"); break;
		case kLocationCenterVR: str = _TEXT("中轴竖直偏右"); break;
		case kLocationCenterHB: str = _TEXT("中轴水平偏下"); break;
		case kLocationCenterHT: str = _TEXT("中轴水平偏上"); break;
		default: str = _TEXT("Error"); break;
		}
		break;
	}
	
	AppendMenu(axisMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );

	// Add axis arrow menu
	subMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsArrowStart() );
		AppendMenu(subMenu, MF_STRING | flag, startVal + kMenuAxisArrows, _TEXT("&Start Arrow") );
		flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsArrowEnd() );
		AppendMenu(subMenu, MF_STRING | flag, startVal + kMenuAxisArrows + 1, _TEXT("&End Arrow") );
		AppendMenu(axisMenu, MF_POPUP, (UINT)subMenu, _TEXT("&Arrows"));
		break;
	case kLangChinese:
		flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsArrowStart() );
		AppendMenu(subMenu, MF_STRING | flag, startVal + kMenuAxisArrows, _TEXT("起始箭头(&S)") );
		flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsArrowEnd() );
		AppendMenu(subMenu, MF_STRING | flag, startVal + kMenuAxisArrows + 1, _TEXT("结束箭头(&E)") );
		AppendMenu(axisMenu, MF_POPUP, (UINT)subMenu, _TEXT("箭头(&A)"));
		break;
	}
	DestroyMenu(subMenu);

	// Add independent
	flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsIndependent() );
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(axisMenu, MF_STRING | flag, startVal + kMenuAxisIndependent, _TEXT("&Independent") );
		break;
	case kLangChinese:
		AppendMenu(axisMenu, MF_STRING | flag, startVal + kMenuAxisIndependent, _TEXT("独立(&I)") );
		break;
	}

	// Add Color bar
	subMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsShowColorBar() );
		AppendMenu(subMenu, MF_STRING | flag, startVal + kMenuAxisColorBar, _TEXT("&Show ColorBar") );
		AppendMenu(subMenu, MF_STRING, startVal + kMenuAxisColorBar+1, _TEXT("&Color1...") );
		AppendMenu(subMenu, MF_STRING, startVal + kMenuAxisColorBar+2, _TEXT("C&olor2...") );
		
		AppendMenu(axisMenu, MF_POPUP, (UINT)subMenu, _TEXT("&Color Bar"));
		break;
	case kLangChinese:
		flag = GetPopupCheckedFlag( plot->CPlotImpl::GetAxis(which)->IsShowColorBar() );
		AppendMenu(subMenu, MF_STRING | flag, startVal + kMenuAxisColorBar, _TEXT("显示颜色棒(&S)") );
		AppendMenu(subMenu, MF_STRING, startVal + kMenuAxisColorBar+1, _TEXT("颜色1(&C)...") );
		AppendMenu(subMenu, MF_STRING, startVal + kMenuAxisColorBar+2, _TEXT("颜色2(&O)...") );
		
		AppendMenu(axisMenu, MF_POPUP, (UINT)subMenu, _TEXT("颜色棒(&C)"));
		break;
	}
	DestroyMenu(subMenu);

	AppendMenu(axisMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	
	// Visible
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		if( plot->CPlotImpl::GetAxis(which)->IsVisible() )
			AppendMenu(axisMenu, MF_STRING, startVal+kMenuAxisRange-2, _TEXT("&Hide Axis") );
		else
			AppendMenu(axisMenu, MF_STRING, startVal+kMenuAxisRange-2, _TEXT("&Show Axis") );
		break;
	case kLangChinese:
		if( plot->CPlotImpl::GetAxis(which)->IsVisible() )
			AppendMenu(axisMenu, MF_STRING, startVal+kMenuAxisRange-2, _TEXT("隐藏坐标轴(&H)") );
		else
			AppendMenu(axisMenu, MF_STRING, startVal+kMenuAxisRange-2, _TEXT("显示坐标轴(&S)") );
		break;
	}
	// A settings item is always useful
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(axisMenu, MF_STRING, startVal+kMenuAxisRange-1, _TEXT("&Axis Settings...") );
		break;
	case kLangChinese:
		AppendMenu(axisMenu, MF_STRING, startVal+kMenuAxisRange-1, _TEXT("坐标轴设置(&A)...") );
		break;
	}
	
	AppendMenu(menu, MF_POPUP, (UINT) axisMenu, str.c_str() );
	DestroyMenu(axisMenu);
}

template<class PlotImplT>
void	AddAxisTypePopup( PlotImplT *plot, bool bCheckExist, HMENU addMenu, tstring titleString, long compare, int startVal )
{
	HMENU subMenu;
	subMenu = CreatePopupMenu();

	UINT uFlag;

	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationLeft)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal, _TEXT("&Left"));
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationBottom)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+1, _TEXT("&Bottom"));
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationRight)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+2, _TEXT("&Right"));
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationTop)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+3, _TEXT("&Top"));
		
		AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationCenterVL)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+4, _TEXT("Center&VL"));
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationCenterVR)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+5, _TEXT("Center&VR"));
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationCenterHB)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+6, _TEXT("Center&HB"));
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationCenterHT)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+7, _TEXT("Center&HT"));
		break;
	case kLangChinese:
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationLeft)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal, _TEXT("左轴(&L)"));
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationBottom)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+1, _TEXT("下轴(&B)"));
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationRight)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+2, _TEXT("右轴(&R)"));
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationTop)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+3, _TEXT("上轴(&T)"));
		
		AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationCenterVL)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+4, _TEXT("中轴垂直偏左"));
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationCenterVR)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+5, _TEXT("中轴垂直偏右"));
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationCenterHB)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+6, _TEXT("中轴水平偏下"));
		uFlag = MF_STRING;
		if(bCheckExist && plot->CPlotImpl::GetAxisByLocation(kLocationCenterHT)==0)uFlag |= MF_GRAYED;
		AppendMenu(subMenu, uFlag, startVal+7, _TEXT("中轴水平偏上"));
		break;
	}
	AppendMenu(addMenu, MF_POPUP | MF_STRING, (UINT) subMenu, titleString.c_str());
	DestroyMenu(subMenu);
}

template<class PlotImplT>
int		InterpretSingleAxisPopupItem( PlotImplT *plot, HWND hWnd, int which, int whichResult )
{
	CAxisImpl *axis = plot->CPlotImpl::GetAxis(which);
	bool bX = axis->IsXAxis();

	if( whichResult < 0 ) return kPopupError;

	COLORREF color;

	// Did user say "auto range"?
	//if( whichResult == 0 ) { m_bRangeSet = !m_bRangeSet; return kPopupUpdate; }
	if( whichResult == 0 )
	{
		if(plot->CPlotImpl::GetAxis(which)->IsXAxis())
		{
			plot->SetXAutoRange(!plot->IsXAutoRange());// it will set the axis
		}
		else
		{
			plot->SetYAutoRange(!plot->IsYAutoRange());// it will set the axis
		}
		//plot->CPlotImpl::GetAxis(which)->SetAutoRange(!plot->CPlotImpl::GetAxis(which)->IsAutoRange());
		return kPopupUpdate;
	}

	// font sizes
	else if( whichResult < kMenuAxisLabelSize + nFontSizes )
	{
		if( whichResult == kMenuAxisTitleShow)
		{
			plot->CPlotImpl::GetAxis(which)->SetShowTitle(!plot->CPlotImpl::GetAxis(which)->IsShowTitle());
		}
		else if( whichResult < kMenuAxisTitleSize + nFontSizes ) // title font
		{
			//m_lfTitleFont.lfHeight = fontSizes[whichResult - kMenuAxisTitleSize];
			plot->CPlotImpl::GetAxis(which)->GetTitleFont().lfHeight = fontSizes[whichResult - kMenuAxisTitleSize];
		}
		else if( whichResult < kMenuAxisTitlePosition + 3)
		{
			plot->CPlotImpl::GetAxis(which)->SetTitlePosition(whichResult-kMenuAxisTitlePosition-1);
		}
		else if( whichResult == kMenuAxisTitleYVerticle )
		{
			plot->CPlotImpl::GetAxis(which)->SetYTitleVerticle(!plot->CPlotImpl::GetAxis(which)->IsYTitleVerticle());
		}
		else if(whichResult == kMenuAxisLabelShow)
		{
			plot->CPlotImpl::GetAxis(which)->SetTickLabelShow(!plot->CPlotImpl::GetAxis(which)->IsTickLabelShow());
		}
		else if(whichResult == kMenuAxisLabelShow+1)
		{
			plot->CPlotImpl::GetAxis(which)->SetTwoStroeyX(!plot->CPlotImpl::GetAxis(which)->IsTwoStoreyX());
		}
		else // label font
		{
			plot->CPlotImpl::GetAxis(which)->GetLabelFont().lfHeight = fontSizes[whichResult - kMenuAxisLabelSize];
		}

		return kPopupUpdate;
	}

	// Number of ticks
	else if( whichResult >= kMenuAxisMajorTickCount && whichResult < kMenuAxisMajorTickCount + nTickSizes + 1 ) // major ticks
	{
		//if( whichResult - kMenuAxisMajorTickCount == 0 ) m_nTickCount = -abs(m_nTickCount);
		if( whichResult - kMenuAxisMajorTickCount == 0 )
		{
			//m_bAutoMajorTick	 = true;
			//m_nTickCount = abs(m_nTickCount);
			if(bX)plot->SetAutoMajorTickX(true);
			else plot->SetAutoMajorTickY(true);
			//plot->CPlotImpl::GetAxis(which)->SetTickCount(-abs(plot->CPlotImpl::GetAxis(which)->GetTickCount()));
			if(bX)plot->SetXRangeSet(false);
			else plot->SetYRangeSet(false);
		}
		else
		{
			//m_bAutoMajorTick  = FALSE;
			//m_nTickCount = tickSizes[whichResult-kMenuAxisMajorTickCount-1] - 1;
			if(bX)plot->SetAutoMajorTickX(false);
			else plot->SetAutoMajorTickY(false);

			if(bX)plot->SetXTickCount(tickSizes[whichResult-kMenuAxisMajorTickCount-1] - 1);
			else plot->SetYTickCount(tickSizes[whichResult-kMenuAxisMajorTickCount-1] - 1);
		}
		return kPopupUpdate;
	}
	else if( whichResult >= kMenuAxisMajorTickPos && whichResult < kMenuAxisMajorTickPos + kTickPositionCount)
	{
		plot->CPlotImpl::GetAxis(which)->SetTickPosition(whichResult - kMenuAxisMajorTickPos);
		return kPopupUpdate;
	}
	else if( whichResult >= kMenuAxisMinorTickCount && whichResult < kMenuAxisMinorTickCount + nTickSizes + 1 ) // minor ticks
	{
		//if( (whichResult-kMenuAxisMinorTickCount) == 0 ) m_nMinorTickCount = -abs(m_nTickCount);
		if( (whichResult-kMenuAxisMinorTickCount) == 0 )
		{
			//m_bAutoMinorTick  = true;
			//m_nMinorTickCount = abs(m_nTickCount);
			if(bX)plot->SetAutoMinorTickX(true);
			else plot->SetAutoMinorTickY(true);
			//plot->CPlotImpl::GetAxis(which)->SetMinorTickCount(abs(plot->CPlotImpl::GetAxis(which)->GetMinorTickCount()));
			if(bX)plot->SetXRangeSet(false);
			else plot->SetYRangeSet(false);
		}
		else
		{
			//m_bAutoMinorTick  = false;
			//m_nMinorTickCount = tickSizes[(whichResult-kMenuAxisMinorTickCount-1)];
			if(bX)plot->SetAutoMinorTickX(false);
			else plot->SetAutoMinorTickY(false);

			if(bX)plot->SetXMinorTickCount(tickSizes[(whichResult-kMenuAxisMinorTickCount-1)]);
			else plot->SetYMinorTickCount(tickSizes[(whichResult-kMenuAxisMinorTickCount-1)]);
		}
		return kPopupUpdate;
	}
	else if( whichResult >= kMenuAxisMinorTickPos && whichResult < kMenuAxisMinorTickPos + kTickPositionCount)
	{
		plot->CPlotImpl::GetAxis(which)->SetMinorTickPosition(whichResult - kMenuAxisMinorTickPos);
		return kPopupUpdate;
	}
	else if( whichResult >= kMenuAxisArrows && whichResult <= kMenuAxisArrows+1)
	{
		if( whichResult == kMenuAxisArrows )
		{
			plot->CPlotImpl::GetAxis(which)->SetArrowStart(!plot->CPlotImpl::GetAxis(which)->IsArrowStart());
		}
		else
		{
			plot->CPlotImpl::GetAxis(which)->SetArrowEnd(!plot->CPlotImpl::GetAxis(which)->IsArrowEnd());
		}
		return kPopupUpdate;
	}
	else if(whichResult == kMenuAxisIndependent)
	{
		plot->CPlotImpl::GetAxis(which)->SetIndependent(!plot->CPlotImpl::GetAxis(which)->IsIndependent());
		return kPopupUpdate;
	}
	else if(whichResult >= kMenuAxisColorBar && whichResult < kMenuAxisColorBar + nMenuAxisColorBar)
	{
		if(whichResult == kMenuAxisColorBar)
		{
			plot->CPlotImpl::GetAxis(which)->SetShowColorBar(!plot->CPlotImpl::GetAxis(which)->IsShowColorBar());
			return kPopupUpdate;
		}
		else if(whichResult >= kMenuAxisColorBar + 1 && whichResult <= kMenuAxisColorBar + 2)
		{
			if(whichResult == kMenuAxisColorBar + 1)
			{
				color = plot->CPlotImpl::GetAxis(which)->GetBarColor1();
				if(ShowColorDialog(hWnd, color))
				{
					plot->CPlotImpl::GetAxis(which)->SetBarColor1(color);
					return kPopupUpdate;
				}
			}
			else if(whichResult == kMenuAxisColorBar + 2)
			{
				color = plot->CPlotImpl::GetAxis(which)->GetBarColor2();
				if(ShowColorDialog(hWnd, color))
				{
					plot->CPlotImpl::GetAxis(which)->SetBarColor2(color);
					return kPopupUpdate;
				}
			}
		}
	}

	// Visible
	else if( whichResult == kMenuAxisRange - 2 )
	{
		plot->CPlotImpl::GetAxis(which)->SetVisible(!plot->CPlotImpl::GetAxis(which)->IsVisible());
		return kPopupUpdate;
	}
	// Settings (unfortunately, have to do this one in parent)

	return kPopupError;
}

template<class PlotImplT>
void	CreateAxisPopup(PlotImplT *plot, HMENU menu)
{
	HMENU subMenu;
	int i;

	subMenu = CreatePopupMenu();
	for( i = 0; i < plot->CPlotImpl::GetAxesCount(); i++ )
	{
		AddAxisPopup(plot, subMenu, i, (kMenuAxisRange*i)+kMenuAxisMinimum );
	}
	if(plot->CPlotImpl::GetAxesCount() > 0) AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AddAxisTypePopup(plot, false, subMenu, _TEXT("&Add Axis"), 0, (kMenuAxisRange*plot->CPlotImpl::GetAxesCount())+kMenuAxisMinimum );
		AddAxisTypePopup(plot, true, subMenu, _TEXT("&Del Axis"), 0, (kMenuAxisRange*plot->CPlotImpl::GetAxesCount())+kMenuAxisMinimum+kLocation2DCount );
		break;
	case kLangChinese:
		AddAxisTypePopup(plot, false, subMenu, _TEXT("增加坐标轴(&A)"), 0, (kMenuAxisRange*plot->CPlotImpl::GetAxesCount())+kMenuAxisMinimum );
		AddAxisTypePopup(plot, true, subMenu, _TEXT("删除坐标轴(&D)"), 0, (kMenuAxisRange*plot->CPlotImpl::GetAxesCount())+kMenuAxisMinimum+kLocation2DCount );
		break;
	}
	
	if( plot->CPlotImpl::GetAxesCount() > 1)
	{
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, _TEXT("&Axes") );
			break;
		case kLangChinese:
			AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, _TEXT("坐标轴(&A)") );
			break;
		}
		
	}
	else
	{
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, _TEXT("&Axis") );
			break;
		case kLangChinese:
			AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, _TEXT("坐标轴(&A)") );
			break;
		}
	}
	DestroyMenu(subMenu);
}

template<class PlotImplT>
int		InterpretAxisPopupItem( PlotImplT *plot, HWND hWnd, int result )
{
	int whichAxis;
	int returnVal;

	if( result >= kMenuAxisMinimum && result < kMenuAxisMinimum + (plot->CPlotImpl::GetAxesCount()*kMenuAxisRange) )
	{
		// These must be axis sets
		whichAxis = (result-kMenuAxisMinimum) / kMenuAxisRange;
		
		if( whichAxis < 0 || whichAxis > plot->CPlotImpl::GetAxesCount() ) return kPopupError;
		
		if( result % kMenuAxisRange == kMenuAxisRange-1 ) // have to do this one manually
		{
			HDC hDC = GetDC(hWnd);
			int ret;
			if( DefAxisSettings(plot, hDC, plot->CPlotImpl::GetAxis(whichAxis)->GetLocation()) ) ret = kPopupUpdate; else ret = kPopupNoAction;
			ReleaseDC(hWnd, hDC);
			return ret;
		}
		
		//if( (returnVal = plot->CPlotImpl::GetAxis(whichAxis)->InterpretAxisPopupItem( (result-kMenuAxisMinimum) % kMenuAxisRange ) ) == kPopupError )
		if( (returnVal = InterpretSingleAxisPopupItem(plot, hWnd, whichAxis, (result-kMenuAxisMinimum) % kMenuAxisRange ) ) == kPopupError )
			return result;
		else
			return returnVal;
	}
	else
	{
		int startVal = kMenuAxisMinimum + (plot->CPlotImpl::GetAxesCount()*kMenuAxisRange) ;
		if(result - startVal < kLocation2DCount)
		{
			plot->CPlotImpl::AddAxis(result - startVal);
			return kPopupUpdate;
		}
		else
		{
			plot->DeleteAxisByLocation(result - startVal - kLocation2DCount);
			return kPopupUpdate;
		}
	}
	return kPopupNoAction;
}

template<class PlotImplT>
bool	IsAxisItem(PlotImplT *plot, int result)
{
	return ( result >= kMenuAxisMinimum && result < kMenuAxisMinimum + (plot->CPlotImpl::GetAxesCount()*kMenuAxisRange)+kMenuAxisAddtional );
}

Declare_Namespace_End
