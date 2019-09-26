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

#include "ContourLineHandler.h"
#include "ContourLinePlotImpl.h"

Declare_Namespace_CChart

class CContourLinePlotImplI;

void	InitPageParaSpecial(CContourLinePlotImplI *plot, vPagePara &vPP)
{
	
}

void	CreateOscilloPopup(CContourLinePlotImplI *plot, HMENU menu)
{
}

int		InterpretOscilloPopupItem( CContourLinePlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}
void	CreateExtraFunctionPopup(CContourLinePlotImplI *plot, HMENU subMenu)
{
	
}
int		InterpretExtraFunctionPopupItem( CContourLinePlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}
void	CreateTooltipPopup(CContourLinePlotImplI *plot, HMENU subMenu)
{
	
}

int		InterpretTooltipPopupItem( CContourLinePlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	CreateLegendPopup(CContourLinePlotImplI *plot, HMENU menu)
{

}
int		InterpretLegendPopupItem( CContourLinePlotImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	InitPageParaLegend(CContourLinePlotImplI *pPlot, vPagePara &vPP)
{
	
}
void	SetupDataForLegend(HWND hWnd, CContourLinePlotImplI *plot, LOGFONT	&lfLegendFont)
{

}
void	SaveDataForLegend(HWND hWnd, CContourLinePlotImplI *plot, LOGFONT	lfLegendFont)
{

}

Declare_Namespace_End