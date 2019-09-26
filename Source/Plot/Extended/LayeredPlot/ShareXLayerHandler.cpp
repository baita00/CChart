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

#include "ShareXLayerHandler.h"


Declare_Namespace_CChart

class CShareXLayerImplI;

void	InitPageParaSpecial(CShareXLayerImplI *plot, vPagePara &vPP)
{
	
}

void	CreateOscilloPopup(CShareXLayerImplI *plot, HMENU menu)
{
}

int		InterpretOscilloPopupItem( CShareXLayerImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}
void	CreateExtraFunctionPopup(CShareXLayerImplI *plot, HMENU subMenu)
{
	
}
int		InterpretExtraFunctionPopupItem( CShareXLayerImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}
void	CreateTooltipPopup(CShareXLayerImplI *plot, HMENU subMenu)
{
	
}

int		InterpretTooltipPopupItem( CShareXLayerImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	CreateLegendPopup(CShareXLayerImplI *plot, HMENU menu)
{
	
}
int		InterpretLegendPopupItem( CShareXLayerImplI *plot, HWND hWnd, int result )
{
	return kPopupNoAction;
}

void	InitPageParaLegend(CShareXLayerImplI *pPlot, vPagePara &vPP)
{
	
}
void	SetupDataForLegend(HWND hWnd, CShareXLayerImplI *plot, LOGFONT	&lfLegendFont)
{
	
}
void	SaveDataForLegend(HWND hWnd, CShareXLayerImplI *plot, LOGFONT	lfLegendFont)
{
	
}

Declare_Namespace_End