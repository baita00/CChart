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
#include "../../MyString.h"
#include "../../CChartNameSpace.h"

Declare_Namespace_CChart

class CAxis;
// Adds a popup menu for font sizes.
void	AddFontSizePopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddLineSizePopup(HMENU addMenu, tstring titleString, long compare, int startVal);
void	AddBrushTypePopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddHatchTypePopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddLanguragePopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddTitlePositionPopup(HMENU addMenu, tstring titleString, long compare, int startVal);
void	AddNumberOfTicksPopup( HMENU addMenu, tstring titleString, long compare, int startVal, BOOL bAuto );
void	AddAxisTickPositionPopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddAxisTitlePositionPopup(CAxis *axis, HMENU addMenu, tstring titleString, long compare, int startVal);
void	AddLegendPosPopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddLegendColumnPopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddPlotTypePopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddTooltipTypePopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddApperanceTypePopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddRangeDragPopup(HMENU addMenu, tstring titleString, long compare, int startVal);

Declare_Namespace_End
