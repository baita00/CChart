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

#include "PlotLayerHandler.h"


Declare_Namespace_CChart

class CPlotLayerImplI;

void	InitPageParaSpecial(CPlotLayerImplI *plot, vPagePara &vPP)
{
	
}

void	CreateOscilloPopup(CPlotLayerImplI *plot, HMENU menu)
{
}

int		InterpretOscilloPopupItem( CPlotLayerImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}
void	CreateExtraFunctionPopup(CPlotLayerImplI *plot, HMENU subMenu)
{
	
}
int		InterpretExtraFunctionPopupItem( CPlotLayerImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}
void	CreateTooltipPopup(CPlotLayerImplI *plot, HMENU subMenu)
{
	
}

int		InterpretTooltipPopupItem( CPlotLayerImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	CreateLegendPopup(CPlotLayerImplI *plot, HMENU menu)
{
	
}
int		InterpretLegendPopupItem( CPlotLayerImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	InitPageParaLegend(CPlotLayerImplI *pPlot, vPagePara &vPP)
{
	
}
void	SetupDataForLegend(HWND hWnd, CPlotLayerImplI *plot, LOGFONT	&lfLegendFont)
{
	
}
void	SaveDataForLegend(HWND hWnd, CPlotLayerImplI *plot, LOGFONT	lfLegendFont)
{
	
}

Declare_Namespace_End