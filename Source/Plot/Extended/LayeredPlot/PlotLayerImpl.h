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

#include "PlotLayerBasicImpl.h"
#include "PlotLayer.h"
#include "PlotLayerHandler.h"

Declare_Namespace_CChart

class CLayeredPlotImpl;

class CPlotLayerImpl :	public CPlotLayer,
						public CPlotLayerBasicImpl
{
public:
	CPlotLayerImpl(CLayeredPlotImpl *pParent);
	virtual ~CPlotLayerImpl();
	
	virtual	void		SetXRange( double low, double high);
};

class CPlotLayerImplI :	public CPlotLayerImpl,
						public CPlotLayerHandler<CPlotLayerImplI>,
						public CXYReactStatus<CPlotLayerImplI>
{
public:
	CPlotLayerImplI(CLayeredPlotImpl *pParent):CPlotLayerImpl(pParent){};
	
public:
	tstring				GetDescriptionByMousePos(HDC hDC, POINT point);
};

void	InitPageParaSpecial(CPlotLayerImplI *plot, vPagePara &vPP);
void	CreateOscilloPopup(CPlotLayerImplI *plot, HMENU menu);
int		InterpretOscilloPopupItem( CPlotLayerImplI *plot, HWND hWnd, int result );
void	CreateExtraFunctionPopup(CPlotLayerImplI *plot, HMENU subMenu);
int		InterpretExtraFunctionPopupItem( CPlotLayerImplI *plot, HWND hWnd, int result );
void	CreateTooltipPopup(CPlotLayerImplI *plot, HMENU subMenu);
int		InterpretTooltipPopupItem( CPlotLayerImplI *plot, HWND hWnd, int result );

void	CreateLegendPopup(CPlotLayerImplI *plot, HMENU menu);
int		InterpretLegendPopupItem( CPlotLayerImplI *plot, HWND hWnd, int result );

void	InitPageParaLegend(CPlotLayerImplI *pPlot, vPagePara &vPP);
void	SetupDataForLegend(HWND hWnd, CPlotLayerImplI *plot, LOGFONT	&lfLegendFont);
void	SaveDataForLegend(HWND hWnd, CPlotLayerImplI *plot, LOGFONT	lfLegendFont);


Declare_Namespace_End
