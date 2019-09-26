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

#include "ContourPlot.h"
#include "../ContourLinePlot/ContourLinePlotImpl.h"
#include "../CloudPlot/CloudPlotImpl.h"

Declare_Namespace_CChart

class CContourPlotImpl :	public CContourPlot,
							public CContourLinePlotImpl,
							public CCloudPlot,
							public CCloudLegendImpl<CContourPlotImpl>
{
public:
	CContourPlotImpl();
	virtual	~CContourPlotImpl();
public:
	virtual	void		FinalDraw( HDC hDC, RECT destRect );

protected:
	//void	DrawCloud(HDC hDC, RECT plotRect);
	void	SetAxisLegend(int location, bool set);
	void	SetAllAxesLegend();
public:
	inline	void		SetXRange( double low, double high ) {SetContourXRange(low, high); CPlotImpl::SetXRange(low, high);}
	inline	void		SetYRange( double low, double high ) {SetContourYRange(low, high); CPlotImpl::SetYRange(low, high);}

public:
	bool	IsLegendShow(){return CCloudLegendImpl<CContourPlotImpl>::IsLegendShow();}
};

class CContourPlotImplI :	public CContourPlotImpl,
							public CCloudHandler<CContourPlotImplI>,
							public CCloudReactStatus<CContourPlotImplI>
{
public:
	CContourPlotImplI();
	virtual	~CContourPlotImplI();
public:
//	bool		ShowDlgBySelection(HDC hDC);

};

void	InitPageParaSpecial(CContourPlotImplI *plot, vPagePara &vPP);
void	CreateOscilloPopup(CContourPlotImplI *plot, HMENU menu);
int		InterpretOscilloPopupItem( CContourPlotImplI *plot, HWND hWnd, int result );
void	CreateExtraFunctionPopup(CContourPlotImplI *plot, HMENU subMenu);
void	CreateTooltipPopup(CContourPlotImplI *plot, HMENU subMenu);
int		InterpretExtraFunctionPopupItem( CContourPlotImplI *plot, HWND hWnd, int result );
int		InterpretTooltipPopupItem( CContourPlotImplI *plot, HWND hWnd, int result );

void	CreateDataPopup(CContourPlotImplI *plot, HMENU menu);
int		InterpretDataPopupItem( CContourPlotImplI *plot, HWND hWnd, int result );
void	CreateStatusPopup(CContourPlotImplI *plot, HMENU menu);
int		InterpretStatusPopupItem( CContourPlotImplI *plot, HWND hWnd, int result );

void	InitPageParaAutoSetting(CContourPlotImplI *plot, vPagePara &vPP);

Declare_Namespace_End