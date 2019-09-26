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
#include "../Dialogs/DefDataSettingDialogs/MultiDataOptionDlg.h"

#include "MyMenuID.h"

#include "../DataProcessing.h"

Declare_Namespace_CChart

// Add a popup for the line size of a data set
template<class PlotImplT>
void	AddDataLineSizePopup(PlotImplT *plot, HMENU addMenu, tstring titleString, int dataID, int startVal )
{
	HMENU		subMenu = CreatePopupMenu();
	UINT		flag;
		
	// Line size
	for( int i = 0; i < nLineSizes; i++ )
	{
		flag = GetPopupCheckedFlag( plot->GetDataLineSize( dataID ) == i+1 );
		tostringstream ostr;
		tstring str;
		ostr<<i+1;
		str=ostr.str();
		AppendMenu(subMenu, MF_STRING | flag, startVal+i, str.c_str() );
	}
	AppendMenu(addMenu, MF_POPUP, (UINT) subMenu, titleString.c_str() );

	DestroyMenu(subMenu);
}

// Add a popup for the line size of a data set
template<class PlotImplT>
void	AddDataLineStylePopup( PlotImplT *plot, HMENU addMenu, tstring titleString, int dataID, int startVal )
{
	HMENU		subMenu;
	UINT		flag;
	
	subMenu = CreatePopupMenu();
	
	// We'll just hard code these for now
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 0 );
		AppendMenu(subMenu, MF_STRING | flag, startVal,   _TEXT("Solid ____________") );
		flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 1 );
		AppendMenu(subMenu, MF_STRING | flag, startVal+1, _TEXT("Dashed _ _ _ _ _ _") );
		flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 2 );
		AppendMenu(subMenu, MF_STRING | flag, startVal+2, _TEXT("Dotted . . . . . .") );
		flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 3 );
		AppendMenu(subMenu, MF_STRING | flag, startVal+3, _TEXT("Dash-Dot _ . _ . _") );
		flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 4 );
		AppendMenu(subMenu, MF_STRING | flag, startVal+4, _TEXT("Dash-Dot-Dot _ . . _ . .") );
		break;
	case kLangChinese:
		flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 0 );
		AppendMenu(subMenu, MF_STRING | flag, startVal,   _TEXT("实线 ____________") );
		flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 1 );
		AppendMenu(subMenu, MF_STRING | flag, startVal+1, _TEXT("虚划线 _ _ _ _ _ _") );
		flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 2 );
		AppendMenu(subMenu, MF_STRING | flag, startVal+2, _TEXT("虚点线 . . . . . .") );
		flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 3 );
		AppendMenu(subMenu, MF_STRING | flag, startVal+3, _TEXT("划点线 _ . _ . _") );
		flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 4 );
		AppendMenu(subMenu, MF_STRING | flag, startVal+4, _TEXT("划点点线 _ . . _ . .") );
		break;
	}
	
	AppendMenu(addMenu, MF_POPUP, (UINT) subMenu, titleString.c_str() );

	DestroyMenu(subMenu);
}

// Add a popup menu for the different marker types and frequencies
template<class PlotImplT>
void	AddMarkerPopup( PlotImplT *plot, HMENU addMenu, tstring titleString, int dataID, int startVal )
{
	HMENU			subMenu;
	HMENU			shapeMenu,sizeMenu,freqMenu;
	int				i, markerShape, freq;
	UINT			flag;

	tostringstream ostr;
	tstring str;
	
	subMenu = CreatePopupMenu();
	// Show markers
	flag = GetPopupCheckedFlag( plot->IsMarkerShow(dataID));
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(subMenu, MF_STRING | flag, startVal, _TEXT("&Show Markers"));
		break;
	case kLangChinese:
		AppendMenu(subMenu, MF_STRING | flag, startVal, _TEXT("显示标记(&S)"));
		break;
	}

	// Marker shape
	shapeMenu = CreatePopupMenu();
	markerShape = plot->GetMarkerType( dataID );

	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerNone );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerNone, _TEXT("&No marker") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerCircle );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerCircle, _TEXT("&Circle marker") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerSquareUpright );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerSquareUpright, _TEXT("&Upright square marker") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerSquareOblique );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerSquareOblique, _TEXT("&Oblique square marker") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerTriangleLeft );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerTriangleLeft, _TEXT("&Left triangle marker") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerTriangleRight );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerTriangleRight, _TEXT("&Right triangle marker") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerTriangleUp );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerTriangleUp, _TEXT("&Up triangle marker") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerTriangleDown );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerTriangleDown, _TEXT("&Down triangle marker") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerX );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerX, _TEXT("&X marker") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerCross );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerCross, _TEXT("&Cross marker") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerBar );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerBar, _TEXT("&Bar marker") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerDot );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerDot, _TEXT("&Dot marker") );
		
		AppendMenu(subMenu, MF_POPUP, (UINT) shapeMenu, _TEXT("&Marker shape") );
		break;
	case kLangChinese:
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerNone );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerNone, _TEXT("无标记(&N)") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerCircle );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerCircle, _TEXT("圆圈(&C)") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerSquareUpright );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerSquareUpright, _TEXT("竖直方块(&U)") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerSquareOblique );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerSquareOblique, _TEXT("斜方块(&O)") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerTriangleLeft );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerTriangleLeft, _TEXT("左向三角形(&L)") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerTriangleRight );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerTriangleRight, _TEXT("右向三角形(&R)") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerTriangleUp );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerTriangleUp, _TEXT("上三角形(&U)") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerTriangleDown );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerTriangleDown, _TEXT("下三角形(&D)") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerX );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerX, _TEXT("斜十字(&X)") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerCross );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerCross, _TEXT("正十字(&C)") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerBar );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerBar, _TEXT("颜色棒(&B)") );
		flag = GetPopupCheckedFlag( markerShape == kXYMarkerDot );
		AppendMenu(shapeMenu, MF_STRING | flag, startVal + kMenuMarkerType + kXYMarkerDot, _TEXT("点(&D)") );
		
		AppendMenu(subMenu, MF_POPUP, (UINT) shapeMenu, _TEXT("标记形状(&M)") );
		break;
	}
	DestroyMenu(shapeMenu);
	
	// Marker Fill
	flag = GetPopupCheckedFlag( plot->GetMarkerFillState( dataID ) );
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(subMenu, MF_STRING | flag, startVal+kMenuMarkerFill, _TEXT("M&arker fill") );
		break;
	case kLangChinese:
		AppendMenu(subMenu, MF_STRING | flag, startVal+kMenuMarkerFill, _TEXT("填充标记(&A)") );
		break;
	}
	
	// Marker size
	sizeMenu = CreatePopupMenu();
	for( i = 0; i < nMarkerSizes; i++ )
	{
		flag = GetPopupCheckedFlag( plot->GetMarkerSize( dataID ) == (i+1) );
		ostr.str(_TEXT(""));
		ostr<<i+1;
		str=ostr.str();
		AppendMenu(sizeMenu, MF_STRING | flag, startVal+i+kMenuMarkerSize, str.c_str() );
	}
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(subMenu, MF_POPUP, (UINT) sizeMenu, _TEXT("Ma&rker size") );
		break;
	case kLangChinese:
		AppendMenu(subMenu, MF_POPUP, (UINT) sizeMenu, _TEXT("标记大小(&R)") );
		break;
	}
	DestroyMenu(sizeMenu);
	
	// Marker frequency
	freqMenu= CreatePopupMenu();
	freq = abs(plot->GetMarkerFrequency( dataID ));
	if( freq == 1 || freq ==0 ) freq = 10;
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		flag = GetPopupCheckedFlag( plot->GetMarkerFrequency( dataID ) == 1 );
		AppendMenu(freqMenu, MF_STRING | flag, startVal+kMenuMarkerFrequency, _TEXT("&Mark every point") );
		flag = GetPopupCheckedFlag( (plot->GetMarkerFrequency( dataID )) > 1 );
		ostr.str(_TEXT(""));
		ostr<<_TEXT("M&ark every ")<<freq<<_TEXT(" points");
		str=ostr.str();
		AppendMenu(freqMenu, MF_STRING | flag, startVal+kMenuMarkerFrequency+1, str.c_str() );
		flag = GetPopupCheckedFlag( (plot->GetMarkerFrequency( dataID )) < 0 );
		ostr.str(_TEXT(""));
		ostr<<_TEXT("Ma&rk ")<<freq<<_TEXT(" points");
		str=ostr.str();
		AppendMenu(freqMenu, MF_STRING | flag, startVal+kMenuMarkerFrequency+2, str.c_str() );
		flag = GetPopupCheckedFlag( (plot->GetMarkerFrequency( dataID )) == 0 );
		str=_TEXT("Mar&k none");
		AppendMenu(freqMenu, MF_STRING | flag, startVal+kMenuMarkerFrequency+3, str.c_str() );
		AppendMenu(subMenu, MF_POPUP, (UINT) freqMenu, _TEXT("Marker &frequency") );
		break;
	case kLangChinese:
		flag = GetPopupCheckedFlag( plot->GetMarkerFrequency( dataID ) == 1 );
		AppendMenu(freqMenu, MF_STRING | flag, startVal+kMenuMarkerFrequency, _TEXT("标记所有点(&M)") );
		flag = GetPopupCheckedFlag( (plot->GetMarkerFrequency( dataID )) > 1 );
		ostr.str(_TEXT(""));
		ostr<<_TEXT("每隔")<<freq<<_TEXT("个点(&A)");
		str=ostr.str();
		AppendMenu(freqMenu, MF_STRING | flag, startVal+kMenuMarkerFrequency+1, str.c_str() );
		flag = GetPopupCheckedFlag( (plot->GetMarkerFrequency( dataID )) < 0 );
		ostr.str(_TEXT(""));
		ostr<<_TEXT("总共")<<freq<<_TEXT("个点(&R)");
		str=ostr.str();
		AppendMenu(freqMenu, MF_STRING | flag, startVal+kMenuMarkerFrequency+2, str.c_str() );
		flag = GetPopupCheckedFlag( (plot->GetMarkerFrequency( dataID )) == 0 );
		str=_TEXT("无标记点(&K)");
		AppendMenu(freqMenu, MF_STRING | flag, startVal+kMenuMarkerFrequency+3, str.c_str() );
		AppendMenu(subMenu, MF_POPUP, (UINT) freqMenu, _TEXT("标记频率(&F)") );
		break;
	}
	
	DestroyMenu(freqMenu);

	AppendMenu(addMenu, MF_POPUP, (UINT)subMenu, titleString.c_str());
	DestroyMenu(subMenu);
}

// Add the data Processing popup menu
template<class PlotImplT>
void	AddDataProcessPopup( PlotImplT *plot, HMENU addMenu, tstring titleString, int dataID, int startVal )
{
	HMENU	subMenu, tMenu;
	
	subMenu = CreatePopupMenu();
	
	tMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcTrans, _TEXT("&Offset..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcTrans+1, _TEXT("&Scale..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcTrans+2, _TEXT("&Affine..."));
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("&Transform"));
		break;
	case kLangChinese:
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcTrans, _TEXT("偏移(&O)..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcTrans+1, _TEXT("缩放(&S)..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcTrans+2, _TEXT("变换(&A)..."));
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("线性变换(&T)"));
		break;
	}
	DestroyMenu(tMenu);
	tMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcCalculus, _TEXT("&Differential..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcCalculus+1, _TEXT("&Intergrate..."));
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("&Calculus"));
		break;
	case kLangChinese:
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcCalculus, _TEXT("微分(&D)..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcCalculus+1, _TEXT("积分(&I)..."));
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("微积分(&C)"));
		break;
	}
	DestroyMenu(tMenu);
	tMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcSmoothing, _TEXT("&Median filter..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcSmoothing+1, _TEXT("&Average filter..."));
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("&Smoothing"));
		break;
	case kLangChinese:
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcSmoothing, _TEXT("中值滤波(&M)..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcSmoothing+1, _TEXT("均值滤波(&A)..."));
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("数据平滑(&S)"));
		break;
	}
	DestroyMenu(tMenu);
	tMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting, _TEXT("&Polynomial..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+1, _TEXT("&Exponential..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+2, _TEXT("&Logarithmic..."));
		AppendMenu(tMenu, MF_SEPARATOR, 0, (LPCTSTR)0);
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+3, _TEXT("&Single gaussian..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+4, _TEXT("Slope &backgnd gaussian..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+5, _TEXT("&Parabolic backgnd gaussian..."));
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("&Fitting"));
		AppendMenu(tMenu, MF_SEPARATOR, 0, (LPCTSTR)0);
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+6, _TEXT("&Multiple gaussian..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+7, _TEXT("M&ultiple slope gaussian..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+8, _TEXT("Mu&ltiple parabolic gaussian..."));
		break;
	case kLangChinese:
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting, _TEXT("多项式拟合(&P)..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+1, _TEXT("指数函数拟合(&E)..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+2, _TEXT("对数函数拟合(&L)..."));
		AppendMenu(tMenu, MF_SEPARATOR, 0, (LPCTSTR)0);
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+3, _TEXT("高斯拟合(&S)..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+4, _TEXT("斜本底高斯拟合(&B)..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+5, _TEXT("抛物本底高斯拟合(&P)..."));
		AppendMenu(subMenu, MF_POPUP | MF_STRING, (UINT)tMenu, _TEXT("数据拟合(&F)"));
		AppendMenu(tMenu, MF_SEPARATOR, 0, (LPCTSTR)0);
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+6, _TEXT("多峰高斯拟合(&M)..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+7, _TEXT("斜本底多峰高斯拟合(&U)..."));
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataProcFitting+8, _TEXT("抛物本底多峰高斯拟合(&L)..."));
		break;
	}
	DestroyMenu(tMenu);
	
	AppendMenu(addMenu, MF_STRING | MF_POPUP, (UINT)subMenu, titleString.c_str());
	
	DestroyMenu(subMenu);
}

// add the 
template<class PlotImplT>
void	AddDataManagementPopup( PlotImplT *plot, HMENU addMenu, int dataID, int startVal )
{
	HMENU tMenu;

	int whichDataSet = plot->GetIndex(dataID);

	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement, _TEXT("&Load data...") );
		AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement+1, _TEXT("S&ave data...") );
		AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement+2, _TEXT("S&how data value...") );
		
		AppendMenu(addMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		
		AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement+3, _TEXT("Swap &XY") );
		
		tMenu = CreatePopupMenu();
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataManagement+4, _TEXT("Sort X &Inc") );
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataManagement+5, _TEXT("Sort X &Dec") );
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataManagement+6, _TEXT("Sort Y I&nc") );
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataManagement+7, _TEXT("Sort Y D&ec") );
		AppendMenu(addMenu, MF_POPUP, (UINT) tMenu, _TEXT("S&ort data") );
		DestroyMenu(tMenu);
		
		AppendMenu(addMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement+8, _TEXT("&Erase data") );
		if(plot->IsVisible(plot->GetDataID(whichDataSet)))
			AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement+9, _TEXT("&Hide  data") );
		else
			AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement+9, _TEXT("&Show  data") );
		break;
	case kLangChinese:
		AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement, _TEXT("载入数据(&L)...") );
		AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement+1, _TEXT("保存数据(&A)...") );
		AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement+2, _TEXT("显示数值(&A)...") );
		AppendMenu(addMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement+3, _TEXT("交换&XY") );

		tMenu = CreatePopupMenu();
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataManagement+4, _TEXT("X增(&I)") );
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataManagement+5, _TEXT("X减(&D)") );
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataManagement+6, _TEXT("Y增(&N)") );
		AppendMenu(tMenu, MF_STRING, startVal+kMenuDataManagement+7, _TEXT("Y减(&E)") );
		AppendMenu(addMenu, MF_POPUP, (UINT) tMenu, _TEXT("数据排序(&O)") );
		DestroyMenu(tMenu);

		AppendMenu(addMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement+8, _TEXT("清除数据(&E)") );
		if(plot->IsVisible(plot->GetDataID(whichDataSet)))
			AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement+9, _TEXT("隐藏数据(&H)") );
		else
			AppendMenu(addMenu, MF_STRING, startVal+kMenuDataManagement+9, _TEXT("显示数据(&S)") );
		break;
	}
}

template<class PlotImplT>
void	AddDataPopup( PlotImplT *plot, int whichDataSet, HMENU menu, int startVal )
{
	tstring			str,name;
	HMENU			dataMenu;
	HMENU			subMenu;
	int				dataID;
	
	if( whichDataSet < 0 || whichDataSet >= plot->GetPlotDataCount() ) return;
	
	dataID = plot->GetDataID(whichDataSet);
	
	dataMenu = CreatePopupMenu();
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AddPlotTypePopup( dataMenu, _TEXT("&Plot type"), plot->GetPlotType( dataID ), startVal+kMenuChartType );
		break;
	case kLangChinese:
		AddPlotTypePopup( dataMenu, _TEXT("曲线类型(&P)"), plot->GetPlotType( dataID ), startVal+kMenuChartType );
		break;
	}
	
//	AppendMenu(dataMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );

	subMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AddDataLineSizePopup( plot, subMenu, _TEXT("&Line size"), dataID, startVal + kMenuLineSize );
		AddDataLineStylePopup( plot, subMenu, _TEXT("L&ine style"), dataID, startVal + kMenuLineStyle );
		AppendMenu(dataMenu, MF_POPUP | MF_STRING, (UINT)subMenu, _TEXT("&Line proterty"));
		break;
	case kLangChinese:
		AddDataLineSizePopup( plot, subMenu, _TEXT("线粗(&L)"), dataID, startVal + kMenuLineSize );
		AddDataLineStylePopup( plot, subMenu, _TEXT("线型(&I)"), dataID, startVal + kMenuLineStyle );
		AppendMenu(dataMenu, MF_POPUP | MF_STRING, (UINT)subMenu, _TEXT("曲线属性(&L)"));
		break;
	}
	DestroyMenu(subMenu);
//	AppendMenu(dataMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AddMarkerPopup( plot, dataMenu, _TEXT("Mar&kers"), dataID, startVal + kMenuMarker );
		break;
	case kLangChinese:
		AddMarkerPopup( plot, dataMenu, _TEXT("数据标记(&K)"), dataID, startVal + kMenuMarker );
		break;
	}
	
	//data management menu
	subMenu = CreatePopupMenu();
	AddDataManagementPopup(plot, subMenu,dataID,startVal);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(dataMenu, MF_POPUP, (UINT) subMenu, _TEXT("&Management") );
		break;
	case kLangChinese:
		AppendMenu(dataMenu, MF_POPUP, (UINT) subMenu, _TEXT("管理(&M)") );
		break;
	}
	DestroyMenu(subMenu);

	if(CGlobal::m_bShowDataProcessingMenu)
	{
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			AddDataProcessPopup(plot, dataMenu, _TEXT("&Processing"), dataID, startVal+kMenuDataProcessing);
			break;
		case kLangChinese:
			AddDataProcessPopup(plot, dataMenu, _TEXT("数据处理(&P)"), dataID, startVal+kMenuDataProcessing);
			break;
		}
	}
	// Add it to main menu
	AppendMenu(dataMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(dataMenu, MF_STRING, startVal+kMenuDataRange-1, _TEXT("&Data Settings...") );
		break;
	case kLangChinese:
		AppendMenu(dataMenu, MF_STRING, startVal+kMenuDataRange-1, _TEXT("数据设置(&D)...") );
		break;
	}	
	
	str = plot->GetDataTitle( dataID );
	if( str == _TEXT("") )str = CGlobal::GetStrUntitle();
	tostringstream ostr;
	ostr<<str.c_str()<<_TEXT("\t("<<whichDataSet+1)<<_TEXT("/")<<plot->GetPlotDataCount()<<_TEXT(")");
	name = ostr.str();
	if( name.length() > 24 )
	{
		name = str.substr(0, 24);
		name = str + _TEXT("...");
	}
	
	AppendMenu(menu, MF_POPUP, (UINT) dataMenu, name.c_str() );
	DestroyMenu(dataMenu);
}

template<class PlotImplT>
void	AddDelPointPopup( PlotImplT *plot, HMENU menu, int startVal )
{
	tstring			name;
	HMENU			subMenu;

	subMenu = CreatePopupMenu();

	bool lightGrayed = plot->GetLightedPointIdx()<0;
	bool selectGrayed = plot->GetSelectedPointIdx()<0;
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(subMenu, MF_STRING | lightGrayed?MF_DISABLED:MF_ENABLED, startVal, _TEXT("&Lighted") );
		AppendMenu(subMenu, MF_STRING | selectGrayed?MF_DISABLED:MF_ENABLED, startVal+1, _TEXT("&Selected") );
		name = _TEXT("&Delete Data Point");
		break;
	case kLangChinese:
		AppendMenu(subMenu, MF_STRING | lightGrayed?MF_DISABLED:MF_ENABLED, startVal, _TEXT("高亮点(&L)") );
		AppendMenu(subMenu, MF_STRING | selectGrayed?MF_DISABLED:MF_ENABLED, startVal+1, _TEXT("选择点(&L)") );
		name = _TEXT("删除数据点(&D)");
		break;
	}

	AppendMenu(menu, MF_POPUP, (UINT) subMenu, name.c_str() );
}

template<class PlotImplT>
int		InterpretDataProcessing( PlotImplT *plot, HWND hWnd, int whichDataSet, int result )
{
	if( result - kMenuDataProcessing - kMenuDataProcTrans >=0 && result - kMenuDataProcessing - kMenuDataProcTrans < nMenuDataProcTrans)
	{
		if( result - kMenuDataProcessing - kMenuDataProcTrans == 0)
		{
			if(ShowOffsetDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
		else if( result - kMenuDataProcessing - kMenuDataProcTrans == 1)
		{
			if(ShowScaleDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
		else if( result - kMenuDataProcessing - kMenuDataProcTrans == 2)
		{
			if(ShowAffineDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
	}
	else if( result - kMenuDataProcessing - kMenuDataProcCalculus >=0 && result - kMenuDataProcessing - kMenuDataProcCalculus < nMenuDataProcCalculus)
	{
		if( result - kMenuDataProcessing - kMenuDataProcCalculus == 0)
		{
			if(ShowDiffentialDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
		if( result - kMenuDataProcessing - kMenuDataProcCalculus == 1)
		{
			if(ShowIntegratialDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
	}
	else if( result - kMenuDataProcessing - kMenuDataProcSmoothing >=0 && result - kMenuDataProcessing - kMenuDataProcSmoothing < nMenuDataProcSmoothing)
	{
		if( result - kMenuDataProcessing - kMenuDataProcSmoothing == 0)
		{
			if(ShowMedianFilterDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
		if( result - kMenuDataProcessing - kMenuDataProcSmoothing == 1)
		{
			if(ShowAverageFilterDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
	}
	else
	{
		if( result - kMenuDataProcessing - kMenuDataProcFitting == 0)
		{
			if(ShowFittingPolynomialDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
		else if( result - kMenuDataProcessing - kMenuDataProcFitting == 1)
		{
			if(ShowFittingExponentialDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
		else if( result - kMenuDataProcessing - kMenuDataProcFitting == 2)
		{
			if(ShowFittingLogarithmicDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
		else if( result - kMenuDataProcessing - kMenuDataProcFitting == 3)
		{
			if(ShowFittingGaussianDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
		else if( result - kMenuDataProcessing - kMenuDataProcFitting == 4)
		{
			if(ShowFittingSlopeGaussianDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
		else if( result - kMenuDataProcessing - kMenuDataProcFitting == 5)
		{
			if(ShowFittingParabolicGaussianDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
		else if( result - kMenuDataProcessing - kMenuDataProcFitting == 6)
		{
			if(ShowFittingMultipleGaussianDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
		else if( result - kMenuDataProcessing - kMenuDataProcFitting == 7)
		{
			if(ShowFittingMultipleSlopeGaussianDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
		else if( result - kMenuDataProcessing - kMenuDataProcFitting == 8)
		{
			if(ShowFittingMultipleParabolicGaussianDataDlg(plot, hWnd, whichDataSet))
			{
				return kPopupUpdate;
			}
		}
	}
	return kPopupNoAction;
}

template<class PlotImplT>
int		InterpretDataSetPopupItem( PlotImplT *plot, HWND hWnd, int whichDataSet, int result )
{
	tstring		filename, fileext, filepath;
	int			dataID = plot->GetDataID(whichDataSet);

	// result is chart type
	if( result >= kMenuChartType && result < kMenuLineSize )
	{
		// These correlate perfectly
		plot->SetPlotType( dataID, result - kMenuChartType );

		return kPopupUpdate;
	}

	// result is line size
	else if( result - kMenuLineSize >= 0 && result < kMenuLineSize + nLineSizes )
	{
		plot->SetDataLineSize( dataID, result - kMenuLineSize + 1 );

		return kPopupUpdate;
	}

	// result is line style
	else if( result - kMenuLineStyle >= 0 && result < kMenuLineStyle + 5 )
	{
		plot->SetDataLineStyle( dataID, result - kMenuLineStyle );

		return kPopupUpdate;
	}

	// result is show marker
	else if( result - kMenuMarker == 0 )
	{
		plot->SetMarkerShow(dataID, !plot->IsMarkerShow(dataID));
		return kPopupUpdate;
	}

	// result is marker type
	else if( result - kMenuMarker >= kMenuMarkerType && result - kMenuMarker < kMenuMarkerType+kMarkerTypeCount )
	{
		plot->SetMarkerType( dataID, result - (kMenuMarker + kMenuMarkerType) );
		return kPopupUpdate;
	}

	// marker fill item
	else if( result - kMenuMarker == kMenuMarkerFill )
	//else if( result - kMenuMarkerFill == 0 )
	{
		plot->SetMarkerFillState( dataID, !plot->GetMarkerFillState(dataID) );
		return kPopupUpdate;
	}

	// marker size popup
	else if( result - kMenuMarker >= kMenuMarkerSize && result - kMenuMarker < kMenuMarkerSize+nMarkerSizes )
	{
		plot->SetMarkerSize( dataID, (result - (kMenuMarkerFill + kMenuMarker) ) );
		return kPopupUpdate;
	}

	// marker frequency
	else if( result - kMenuMarker >= kMenuMarkerFrequency && result - kMenuMarker < kMenuMarkerFrequency + 4 )
	{
		int			tempResult = result - kMenuMarkerFrequency - kMenuMarker;
		int			freq = abs(plot->GetMarkerFrequency(dataID));
		
		if( freq == 1 || freq==0 ) freq = 10;
		
		switch( tempResult )
		{
		case 0: plot->SetMarkerFrequency( dataID, 1 ); break;
		case 1: plot->SetMarkerFrequency( dataID, freq ); break;
		case 2: plot->SetMarkerFrequency( dataID, -freq ); break;
		case 3: plot->SetMarkerFrequency( dataID, 0 ); break;
		default: break;
		}
		
		return kPopupUpdate;
	}

	else if(result - kMenuDataManagement >=0 && result - kMenuDataManagement < nMenuDataManagement)
	{
		// Load data
		if( result == kMenuDataManagement)
		{
			if( !ShowFileDialog(hWnd, true, filename, fileext, filepath) )
				return kPopupNoAction;
			if(plot->GetPlotData(plot->GetDataID(whichDataSet))->ReadFromFile(filename.c_str()))
			{
				plot->SetDataModified(plot->GetDataID(whichDataSet));
				//plot->GetXYDataInfo()[whichDataSet].bDataRangesSet = false;
				//plot->GetDataRanges();
				MessageBox(hWnd, _TEXT("Load data OK"), _TEXT("Infomation"), MB_OK);
				return kPopupUpdate;
			}
			else
			{
				return kPopupNoAction;
			}
		}

		// Save data
		else if( result == kMenuDataManagement+1)
		{
			if( !ShowFileDialog(hWnd, false, filename, fileext, filepath) )
				return kPopupNoAction;
			if(plot->GetPlotData(plot->GetDataID(whichDataSet))->WriteToFile(filename.c_str()))
			{
				MessageBox(hWnd, _TEXT("Save data OK"), _TEXT("Infomation"), MB_OK);
			}

			return kPopupNoAction;
		}
		else if(result == kMenuDataManagement+2)
		{
			if(whichDataSet<0 || whichDataSet>=plot->GetPlotDataCount())return kPopupNoAction;
			if(plot->GetAbsData(whichDataSet).size()<=0)return kPopupNoAction;

			plot->ShowDataInGrid(whichDataSet, hWnd);
			return kPopupNoAction;
		}
		else if( result == kMenuDataManagement+3)
		{
			if(whichDataSet<0 || whichDataSet>=plot->GetPlotDataCount())return kPopupNoAction;
			if(plot->GetAbsData(whichDataSet).size()<=0)return kPopupNoAction;
			plot->SwapXY(plot->GetDataID(whichDataSet));

			return kPopupUpdate;
		}
		else if( result - kMenuDataManagement >=4 && result - kMenuDataManagement <= 7)
		{
			if(whichDataSet<0 || whichDataSet>=plot->GetPlotDataCount())return kPopupNoAction;
			if(plot->GetAbsData(whichDataSet).size()<=0)return kPopupNoAction;
			plot->SortData(dataID, result-kMenuDataManagement-4);
			
			return kPopupUpdate;
		}
		// delete this curve
		else if( result == kMenuDataManagement+8)
		{
			plot->DeleteData(plot->GetDataID(whichDataSet));
			return kPopupUpdate;
		}
		// hide this curve
		else if( result == kMenuDataManagement+9)
		{
			plot->SetVisible(plot->GetDataID(whichDataSet), !plot->IsVisible(plot->GetDataID(whichDataSet)));
			return kPopupUpdate;		
		}
	}

	else if( result - kMenuDataProcessing >=0 && result - kMenuDataProcessing < nMenuDataProcessing)
	{
		if(CGlobal::m_bShowDataProcessingMenu)
		{
			return InterpretDataProcessing(plot, hWnd, whichDataSet, result);
		}
	}
	
	return kPopupNoAction;
}


template<class PlotImplT>
void	CreateDataPopup(PlotImplT *plot, HMENU menu)
{
	HMENU subMenu;
	int i;
	subMenu = CreatePopupMenu();
	
	for( i = 0; i < min( plot->GetPlotDataCount(),nMaxDataSetsMenus ); i++ )
	{
		AddDataPopup(plot, i, subMenu, (kMenuDataRange*i) + kMenuDataMinimum );
	}
	
	tostringstream ostr;
	tstring str;
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		ostr<<_TEXT("&Data Sets(Total ")<<plot->GetPlotDataCount()<<_TEXT(")");
		break;
	case kLangChinese:
		ostr<<_TEXT("曲线数据(共")<<plot->GetPlotDataCount()<<_TEXT("个)");
		break;
	}	
	str=ostr.str();
	AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, str.c_str() );
	
	AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	
	if( (plot->GetLightedID()>=0 && plot->GetLightedPointIdx()>=0) || (plot->GetSelectedID()>=0 && plot->GetSelectedPointIdx()>=0) )
	{
		AddDelPointPopup(plot, subMenu, kenuDataDelLightPoint);
	}
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(subMenu, MF_STRING, kMenuDataGlobalLoad, _TEXT("&Load data..."));
		AppendMenu(subMenu, MF_STRING, kMenuDataGlobalSave, _TEXT("&Save data..."));
		AppendMenu(subMenu, MF_STRING, kMenuDataGlobalShow, _TEXT("S&how data value..."));
		break;
	case kLangChinese:
		AppendMenu(subMenu, MF_STRING, kMenuDataGlobalLoad, _TEXT("载入数据(&L)..."));
		AppendMenu(subMenu, MF_STRING, kMenuDataGlobalSave, _TEXT("保存数据(&S)..."));
		AppendMenu(subMenu, MF_STRING, kMenuDataGlobalShow, _TEXT("显示数值(&H)..."));
		break;
	}
	
	DestroyMenu(subMenu);
}

template<class PlotImplT>
int		InterpretDataPopupItem( PlotImplT *plot, HWND hWnd, int result )
{
	tstring		filename, fileext, filepath;

	if( result >= kMenuDataMinimum && result < kMenuDataMinimum + min( plot->GetPlotDataCount(), nMaxDataSetsMenus )*kMenuDataRange )
	{
		int			whichDataSet;
		
		whichDataSet = (result-kMenuDataMinimum) / kMenuDataRange;
		
		if( whichDataSet < 0 || whichDataSet > plot->GetPlotDataCount()-1 ) return kPopupError;
		
		if( (result-kMenuDataMinimum) % kMenuDataRange == kMenuDataRange-1 ) // have to do this one manually
		{
			HDC hDC = GetDC(hWnd);
			int ret;
			if( DefDataSettings(plot, hDC, whichDataSet) ) ret = kPopupUpdate; else ret = kPopupNoAction;
			ReleaseDC(hWnd, hDC);
			return ret;
		}
		
		return InterpretDataSetPopupItem( plot, hWnd, whichDataSet, (result-kMenuDataMinimum) % kMenuDataRange );
	}
	else if(result <= kMenuDataSetsEnd)
	{
		if( result == kenuDataDelLightPoint || result == kenuDataDelLightPoint+1)
		{
			if(plot->IsPointSelectionMode())
			{
				if( result == kenuDataDelLightPoint )
				{
					int id = plot->GetLightedID();
					int idx = plot->GetLightedPointIdx();
					if(idx>=0)
					{
						plot->UnselectDataPoints();
						plot->UnlightDataPoints();
						plot->RemovePoint(id, idx);
						return kPopupUpdate;
					}
					return kPopupNoAction;
				} 
				else if( result == kenuDataDelLightPoint+1 )
				{
					int id = plot->GetSelectedID();
					int idx = plot->GetSelectedPointIdx();
					if(idx>=0)
					{
						plot->UnselectDataPoints();
						plot->UnlightDataPoints();
						plot->RemovePoint(id, idx);
						return kPopupUpdate;
					}
					return kPopupNoAction;
				}
			}
		}
		else if( result == kMenuDataGlobalLoad )
		{
			if( !MultiDataOption(plot, hWnd) )return kPopupNoAction;
			if( !ShowFileDialog(hWnd, true, filename, fileext, filepath) )	return kPopupNoAction;
			if( plot->ReadFromFile(filename) )
			{
				MessageBox(hWnd, _TEXT("Load data OK"), _TEXT("Infomation"), MB_OK);
				return kPopupUpdate;
			}
			return kPopupNoAction;
		}
		
		else if( result == kMenuDataGlobalSave )
		{
			if(plot->GetPlotDataCount()>1)
			{
				if( !MultiDataOption(plot, hWnd) )return kPopupNoAction;
			}
			if(plot->GetPlotDataCount()<=0)return kPopupNoAction;
			if( !ShowFileDialog(hWnd, false, filename, fileext, filepath) ) return kPopupNoAction;
			if( plot->WriteToFile(filename) )
			{
				MessageBox(hWnd, _TEXT("Save data OK"), _TEXT("Infomation"), MB_OK);
			}
			return kPopupNoAction;
		}
		else if( result == kMenuDataGlobalShow )
		{
			if(plot->GetPlotDataCount()<=0)return kPopupNoAction;
			plot->ShowDataInGrid(hWnd);
			return kPopupNoAction;
		}
	}
	return kPopupNoAction;
}

template<class PlotImplT>
bool	IsDataItem(PlotImplT *plot, int result)
{
	return ( result >= kMenuDataMinimum && result <= kMenuDataSetsEnd && !plot->IsNoDataPlot());
}

Declare_Namespace_End

