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
#include "InterConnectImpl.h"

Declare_Namespace_CChart

void	MyDrawInterConnect(HDC hDC, int alpha, COLORREF lineColor, int lineSize, int lineStyle, POINT *pPoints1, POINT *pPoints2, int nDatas)
{
	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	
	Color cr(alpha, GetRValue(lineColor), GetGValue(lineColor), GetBValue(lineColor));
	Pen pen(cr, (Gdiplus::REAL)lineSize);
	pen.SetDashStyle((enum Gdiplus::DashStyle)lineStyle);
	
	Point pt1, pt2;
	
	int i;
	for(i=0; i<nDatas; i++)
	{
		pt1.X = pPoints1[i].x;
		pt1.Y = pPoints1[i].y;
		pt2.X = pPoints2[i].x;
		pt2.Y = pPoints2[i].y;
		graph.DrawLine(&pen, pt1, pt2);
	}
}

Declare_Namespace_End