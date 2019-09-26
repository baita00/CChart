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

#include "LayeredHandler.h"

Declare_Namespace_CChart

class CLayeredPlotImplI;

void	InitPageParaSpecial(CLayeredPlotImplI *plot, vPagePara &vPP)
{
	
}

void	CreateOscilloPopup(CLayeredPlotImplI *plot, HMENU menu)
{
}

int		InterpretOscilloPopupItem( CLayeredPlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}
void	CreateExtraFunctionPopup(CLayeredPlotImplI *plot, HMENU subMenu)
{
	
}
int		InterpretExtraFunctionPopupItem( CLayeredPlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}
void	CreateTooltipPopup(CLayeredPlotImplI *plot, HMENU subMenu)
{
	
}

int		InterpretTooltipPopupItem( CLayeredPlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	CreateLegendPopup(CLayeredPlotImplI *plot, HMENU menu)
{
	
}
int		InterpretLegendPopupItem( CLayeredPlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	CreateDataPopup(CLayeredPlotImplI *plot, HMENU menu)
{

}
int		InterpretDataPopupItem( CLayeredPlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	CreateStatusPopup(CLayeredPlotImplI *plot, HMENU menu)
{

}
int		InterpretStatusPopupItem( CLayeredPlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	InitPageParaLegend(CLayeredPlotImplI *pPlot, vPagePara &vPP)
{
	
}

void	InitPageParaAutoSetting(CLayeredPlotImplI *pPlot, vPagePara &vPP)
{
	
}
void	SetupDataForAutoSettings(HWND hWnd, CLayeredPlotImplI *plot)
{

}
void	SaveDataForAutoSettings(HWND hWnd, CLayeredPlotImplI *plot)
{

}

//void	InitPageParaLegend(CLayeredPlotImplI *pPlot, vPagePara &vPP)
//{
//	
//}
void	SetupDataForLegend(HWND hWnd, CLayeredPlotImplI *plot, LOGFONT	&lfLegendFont)
{
	
}
void	SaveDataForLegend(HWND hWnd, CLayeredPlotImplI *plot, LOGFONT	lfLegendFont)
{
	
}

Declare_Namespace_End