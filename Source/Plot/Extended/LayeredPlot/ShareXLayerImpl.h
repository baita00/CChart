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

//#include "../../Basic/XYPlotBasicImpl.h"
#include "PlotLayerBasicImpl.h"
#include "ShareXLayer.h"
#include "ShareXLayerHandler.h"

Declare_Namespace_CChart

class CShareXPlotImpl;


class CShareXLayerImpl :	public CShareXLayer,
							public CPlotLayerBasicImpl
{
public:
	CShareXLayerImpl(CShareXPlotImpl *pParent);
	virtual ~CShareXLayerImpl();
	
	virtual	void		SetXRange( double low, double high);

	bool		IsToCalcXRange();
};

class CShareXLayerImplI :	public CShareXLayerImpl,
							public CShareXLayerHandler<CShareXLayerImplI>,
							public CXYReactStatus<CShareXLayerImplI>
{
public:
	CShareXLayerImplI(CShareXPlotImpl *pParent):CShareXLayerImpl(pParent){};
	virtual	~CShareXLayerImplI(){};

public:
	tstring				GetDescriptionByMousePos(HDC hDC, POINT point);
};

void	InitPageParaSpecial(CShareXLayerImplI *plot, vPagePara &vPP);
void	CreateOscilloPopup(CShareXLayerImplI *plot, HMENU menu);
int		InterpretOscilloPopupItem( CShareXLayerImplI *plot, HWND hWnd, int result );
void	CreateExtraFunctionPopup(CShareXLayerImplI *plot, HMENU subMenu);
int		InterpretExtraFunctionPopupItem( CShareXLayerImplI *plot, HWND hWnd, int result );
void	CreateTooltipPopup(CShareXLayerImplI *plot, HMENU subMenu);
int		InterpretTooltipPopupItem( CShareXLayerImplI *plot, HWND hWnd, int result );

void	CreateLegendPopup(CShareXLayerImplI *plot, HMENU menu);
int		InterpretLegendPopupItem( CShareXLayerImplI *plot, HWND hWnd, int result );

void	InitPageParaLegend(CShareXLayerImplI *pPlot, vPagePara &vPP);
void	SetupDataForLegend(HWND hWnd, CShareXLayerImplI *plot, LOGFONT	&lfLegendFont);
void	SaveDataForLegend(HWND hWnd, CShareXLayerImplI *plot, LOGFONT	lfLegendFont);


Declare_Namespace_End
