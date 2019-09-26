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
#include "LegendImpl.h"

Declare_Namespace_CChart

void	MyDrawLegendLine(HDC hDC, RECT rt, COLORREF lineCr, int lineSize, int lineStyle, int nMarginSizeXInner, int nRealLineLength)
{
	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	
	Color cr( GetRValue(lineCr), GetGValue(lineCr), GetBValue(lineCr));
	Pen pen(cr, (Gdiplus::REAL)lineSize);
	pen.SetDashStyle((enum Gdiplus::DashStyle)lineStyle);
	
	Point pt1, pt2;
	pt1.X = rt.left+nMarginSizeXInner;
	pt1.Y = (rt.bottom + rt.top)/2;
	pt2.X = rt.left+nMarginSizeXInner+nRealLineLength;
	pt2.Y = (rt.bottom + rt.top)/2;
	
	graph.DrawLine(&pen, pt1, pt2);
}

Declare_Namespace_End