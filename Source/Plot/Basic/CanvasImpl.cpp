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
#include "CanvasImpl.h"

Declare_Namespace_CChart

void	DrawRoundRect(HDC hDC, RECT destRect, COLORREF edgeColor, int edgeSize, int edgeStyle, int conorRadius)
{
	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	
	Color cr(GetRValue(edgeColor), GetRValue(edgeColor), GetRValue(edgeColor));
	Pen pen(cr, (float)edgeSize);
	pen.SetDashStyle((enum Gdiplus::DashStyle)edgeStyle);
	GraphicsPath path;
	float width, height;
	width = (float)conorRadius;
	height = width * Height(destRect) / (float)Width(destRect);
	path.AddArc(float(destRect.left), float(destRect.top), float(2 * width), float(2 * height), 180.0, 90.0);
	path.AddArc(float(destRect.right - 2 * width), float(destRect.top), float(2 * width), float(2 * height), 270.0, 90.0);
	path.AddArc(float(destRect.right - 2 * width), float(destRect.bottom - 2 * height), float(2 * width), float(2 * height), 0.0, 90.0);
	path.AddArc(float(destRect.left), float(destRect.bottom - 2 * height), float(2 * width), float(2 * height), 90.0, 90.0);
	path.CloseAllFigures();
	graph.DrawPath(&pen, &path);
}

Declare_Namespace_End