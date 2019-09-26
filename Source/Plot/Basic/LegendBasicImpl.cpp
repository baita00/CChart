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

#include "MyGdiPlus.h"
#include "LegendBasicImpl.h"

Declare_Namespace_CChart

void	MyDrawLegendRoundBorder(HDC hDC, RECT legendRect, COLORREF bordercolor, int lineSize, int lineStyle, int width, int height)
{
	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	
	Color cr(GetRValue(bordercolor), GetGValue(bordercolor), GetBValue(bordercolor));
	Pen pen(cr, (Gdiplus::REAL)lineSize);
	pen.SetDashStyle((enum Gdiplus::DashStyle)lineStyle);
	GraphicsPath path;
	
	
	path.AddArc(float(legendRect.left), float(legendRect.top), float(2 * width), float(2 * height), 180, 90);
	path.AddArc(float(legendRect.right - 2 * width), float(legendRect.top), float(2 * width), float(2 * height), 270, 90);
	path.AddArc(float(legendRect.right - 2 * width), float(legendRect.bottom - 2 * height), float(2 * width), float(2 * height), 0, 90);
	path.AddArc(float(legendRect.left), float(legendRect.bottom - 2 * height), float(2 * width), float(2 * height), 90, 90);
	path.CloseAllFigures();
	graph.DrawPath(&pen, &path);
}

Declare_Namespace_End