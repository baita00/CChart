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

#include "PlotDataBar.h"

Declare_Namespace_CChart

template<class PlotDataT>
class	CPlotDataBarImpl : public CPlotDataBar
{
public:
	CPlotDataBarImpl();
	virtual	~CPlotDataBarImpl();
public:
	void				DrawDataBars( HDC hDC, RECT plotRect, double *xRange, double *yRange );
};

template<class PlotDataT>
CPlotDataBarImpl<PlotDataT>::CPlotDataBarImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
CPlotDataBarImpl<PlotDataT>::~CPlotDataBarImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

void	MyDrawErrorBar(HDC hDC, POINT *pPoints, POINT *pPointsBase, int nDataBarAlpha, COLORREF linecolor, int linesize, int linestyle, int nDatas);

template<class PlotDataT>
void	CPlotDataBarImpl<PlotDataT>::DrawDataBars( HDC hDC, RECT plotRect, double *xRange, double *yRange)
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);

	if(pT->GetDataCount()<=0)return;
	if(!pT->IsVisible())return;
	if(!m_bDataBarShow)return;
	
	int			j, nDatas;
	double		myX, myY;
	bool		bXLog, bYLog;
		
	double xScale, yScale;
	xScale = pT->xScale;
	yScale = pT->yScale;
	
	nDatas = pT->GetDataCount();
	
	bXLog = ((CPlotImpl*)pT->GetPlot())->IsXLogarithm();
	bYLog = ((CPlotImpl*)pT->GetPlot())->IsYLogarithm();

	POINT *pPoints = new POINT[nDatas];
	POINT *pPointsBase = new POINT[nDatas];

	int offset = m_nDataBarOffset;

	for( j = 0; j < nDatas; j++)
	{
		myX = pT->GetData()[j].val[0];
		if(bXLog)myX = log10(myX);
		myY = pT->GetData()[j].val[1];
		if(bYLog)myY = log10(myY);

		if(!((CPlotImpl*)pT->GetPlot())->IsXOpposite())
			pPoints[j].x = (long)(plotRect.left + xScale * (myX-xRange[0]));
		else
			pPoints[j].x = (long)(plotRect.right - xScale * (myX-xRange[0]));
		if(!((CPlotImpl*)pT->GetPlot())->IsYOpposite())
			pPoints[j].y = (long)(plotRect.bottom - yScale * (myY-yRange[0]));
		else
			pPoints[j].y = (long)(plotRect.top + yScale * (myY-yRange[0]));

		switch(m_nDataBarBase)
		{
		case kDataBarBaseBottom:
			pPoints[j].x += offset;
			pPointsBase[j].x = pPoints[j].x;
			pPointsBase[j].y = plotRect.bottom;
			break;
		case kDataBarBaseTop:
			pPoints[j].x += offset;
			pPointsBase[j].x = pPoints[j].x;
			pPointsBase[j].y = plotRect.top;
			break;
		case kDataBarBaseLeft:
			pPoints[j].y -= offset;
			pPointsBase[j].x = plotRect.left;
			pPointsBase[j].y = pPoints[j].y;
			break;
		case kDataBarBaseRight:
			pPoints[j].y -= offset;
			pPointsBase[j].x = plotRect.right;
			pPointsBase[j].y = pPoints[j].y;
			break;
		}
	}

	int linestyle, linesize, linecolor;
	linestyle = m_nDataBarLineStyle;
	linesize = m_nDataBarLineSize;
	if(m_bDataBarSameColor)
	{
		linecolor = pT->GetPlotColor();
	}
	else
	{
		linecolor = m_crDataBarLineColor;
	}

	if( m_nDataBarAlpha == 255 || m_nDataBarLineSize == 1 || (m_nDataBarLineSize != 1 && m_nDataBarLineStyle == PS_SOLID))
	{
		HPEN hPen, hOldPen;
		hPen = CreatePen(linestyle, linesize, linecolor);
		hOldPen = (HPEN)SelectObject(hDC, hPen);

		for(j=0; j<nDatas; j++)
		{
			MoveToEx(hDC, pPoints[j].x, pPoints[j].y, NULL);
			LineTo(hDC, pPointsBase[j].x, pPointsBase[j].y);
		}

		SelectObject(hDC, hOldPen);
		DeleteObject(hPen);
	}
	else
	{
		MyDrawErrorBar(hDC, pPoints, pPointsBase, m_nDataBarAlpha, linecolor, linesize, linestyle, nDatas);
		/*
		Graphics graph(hDC);
		graph.SetSmoothingMode(SmoothingModeHighQuality);
		
		Color cr(m_nDataBarAlpha, GetRValue(linecolor), GetGValue(linecolor), GetBValue(linecolor));
		Pen pen(cr, (Gdiplus::REAL)linesize);
		pen.SetDashStyle((enum Gdiplus::DashStyle)linestyle);

		Point pt1, pt2;
		
		for(j=0; j<nDatas; j++)
		{
			pt1.X = pPoints[j].x;
			pt1.Y = pPoints[j].y;
			pt2.X = pPointsBase[j].x;
			pt2.Y = pPointsBase[j].y;
			graph.DrawLine(&pen, pt1, pt2);
		}*/
	}

	delete []pPoints;
	delete []pPointsBase;
	
}

Declare_Namespace_End