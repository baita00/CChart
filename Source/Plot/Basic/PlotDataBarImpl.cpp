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
#include "PlotDataBarImpl.h"

Declare_Namespace_CChart

void	MyDrawErrorBar(HDC hDC, POINT *pPoints, POINT *pPointsBase, int nDataBarAlpha, COLORREF linecolor, int linesize, int linestyle, int nDatas)
{
	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	
	Color cr(nDataBarAlpha, GetRValue(linecolor), GetGValue(linecolor), GetBValue(linecolor));
	Pen pen(cr, (Gdiplus::REAL)linesize);
	pen.SetDashStyle((enum Gdiplus::DashStyle)linestyle);
	
	Point pt1, pt2;
	int j;
	for(j=0; j<nDatas; j++)
	{
		pt1.X = pPoints[j].x;
		pt1.Y = pPoints[j].y;
		pt2.X = pPointsBase[j].x;
		pt2.Y = pPointsBase[j].y;
		graph.DrawLine(&pen, pt1, pt2);
	}
}

Declare_Namespace_End