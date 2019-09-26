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
#include "PlotDataFillImpl.h"

Declare_Namespace_CChart

void	MyDrawFill(HDC hDC, COLORREF color, int alpha)
{
	HRGN hRgn = PathToRegion(hDC);
	Region rgn(hRgn);
	
	byte a, r, g, b;
	a = alpha;
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);
	Color fillColor(a, r, g, b);
	SolidBrush brush(fillColor);
	
	Graphics graph(hDC);
	graph.FillRegion(&brush, &rgn);
	
	DeleteObject(hRgn);
}

Declare_Namespace_End
