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

#include "ShareXHandler.h"

Declare_Namespace_CChart

class CShareXPlotImplI;

void	InitPageParaSpecial(CShareXPlotImplI *plot, vPagePara &vPP)
{
	
}

void	CreateOscilloPopup(CShareXPlotImplI *plot, HMENU menu)
{
}

int		InterpretOscilloPopupItem( CShareXPlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}
void	CreateExtraFunctionPopup(CShareXPlotImplI *plot, HMENU subMenu)
{
	
}
int		InterpretExtraFunctionPopupItem( CShareXPlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}
void	CreateTooltipPopup(CShareXPlotImplI *plot, HMENU subMenu)
{
	
}

int		InterpretTooltipPopupItem( CShareXPlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	CreateLegendPopup(CShareXPlotImplI *plot, HMENU menu)
{
	
}
int		InterpretLegendPopupItem( CShareXPlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	CreateDataPopup(CShareXPlotImplI *plot, HMENU menu)
{

}
int		InterpretDataPopupItem( CShareXPlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	CreateStatusPopup(CShareXPlotImplI *plot, HMENU menu)
{

}
int		InterpretStatusPopupItem( CShareXPlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	InitPageParaLegend(CShareXPlotImplI *pPlot, vPagePara &vPP)
{
	
}

void	InitPageParaAutoSetting(CShareXPlotImplI *pPlot, vPagePara &vPP)
{
	
}
void	SetupDataForAutoSettings(HWND hWnd, CShareXPlotImplI *plot)
{

}
void	SaveDataForAutoSettings(HWND hWnd, CShareXPlotImplI *plot)
{

}

//void	InitPageParaLegend(CShareXPlotImplI *pPlot, vPagePara &vPP)
//{
//	
//}
void	SetupDataForLegend(HWND hWnd, CShareXPlotImplI *plot, LOGFONT	&lfLegendFont)
{
	
}
void	SaveDataForLegend(HWND hWnd, CShareXPlotImplI *plot, LOGFONT	lfLegendFont)
{
	
}

Declare_Namespace_End