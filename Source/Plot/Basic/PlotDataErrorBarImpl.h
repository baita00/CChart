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
#include "PlotDataErrorBar.h"
#include <windows.h>
#include <vector>
using namespace std;

Declare_Namespace_CChart

template<class PlotDataT>
class	CPlotDataErrorBarImpl : public CPlotDataErrorBar
{
public:
	CPlotDataErrorBarImpl();
	virtual	~CPlotDataErrorBarImpl();

public:
	static	void	MyAdjustDataErrorBar(int whichDim, vector<double> &vValue, double *range);

public:
	void	DrawErrorBars(HDC hDC, RECT plotRect, double *xRange, double *yRange);
};

template<class PlotDataT>
CPlotDataErrorBarImpl<PlotDataT>::CPlotDataErrorBarImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
	AdjustDataRange *pFunc = &MyAdjustDataErrorBar;
	//pT->GetFcnAdjustDataRange().push_back(pFunc);
}

template<class PlotDataT>
CPlotDataErrorBarImpl<PlotDataT>::~CPlotDataErrorBarImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
void	CPlotDataErrorBarImpl<PlotDataT>::MyAdjustDataErrorBar(int whichDim, vector<double> &vValue, double *range)
{
	if(whichDim != 1)return;//y data need adjust
	
	int			j;
	double		minV, maxV;
	double		lb = 0.0, ub = 0.0;
	
	minV = range[0];
	maxV = range[1];
	
	for( j = 0; j < (int)vValue.size(); j++)
	{
		if(m_bErrorBarMode)
		{
			lb = (m_vfLBound.size() == vValue.size())?m_vfLBound[j]:0.0;
			ub = (m_vfUBound.size() == vValue.size())?m_vfUBound[j]:0.0;
			if( vValue[j] - lb < minV ) minV = vValue[j] - lb;
			if( vValue[j] + ub > maxV ) maxV = vValue[j] + ub;
		}
	}
	range[0] = minV;
	range[1] = maxV;
}

template<class PlotDataT>
void	CPlotDataErrorBarImpl<PlotDataT>::DrawErrorBars( HDC hDC, RECT plotRect, double *xRange, double *yRange)
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);

	if(!pT->IsVisible())return;
	if(!m_bErrorBarMode)return;
	
	double xScale, yScale;
	xScale = pT->xScale;
	yScale = pT->yScale;
	
	int			j, nDatas;
	double		myX, myY;
	bool		bXLog, bYLog;
	
	nDatas = pT->GetDataCount();
	
	bXLog = ((CPlotImpl*)pT->GetPlot())->IsXLogarithm();
	bYLog = ((CPlotImpl*)pT->GetPlot())->IsYLogarithm();
	
	HPEN hBarPen, hOldBarPen;
	hBarPen = CreatePen(m_nErrorBarLineStyle, m_nErrorBarLineSize, m_bErrorBarSameColor?pT->GetPlotColor():m_crErrorBarLineColor);
	hOldBarPen = (HPEN)SelectObject(hDC, hBarPen);
	
	POINT pt1, pt2;
	double lb, ub;
	int	hl = m_nErrorBarHeadHalfLen;
	for( j = 0; j < nDatas; j++ )
	{
		if((int)m_vfLBound.size()<=j || (int)m_vfUBound.size()<=j)
			continue;
		
		lb = m_vfLBound[j];
		ub = m_vfUBound[j];
		
		myX = pT->GetData()[j].val[0];
		if(bXLog)myX = log10(myX);
		myY = pT->GetData()[j].val[1] - lb;
		if(bYLog)myY = log10(myY);
		if(!((CPlotImpl*)pT->GetPlot())->IsXOpposite())
			pt1.x = (long)(plotRect.left + xScale * (myX-xRange[0]));
		else
			pt1.x = (long)(plotRect.right - xScale * (myX-xRange[0]));
		if(!((CPlotImpl*)pT->GetPlot())->IsYOpposite())
			pt1.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]));
		else
			pt1.y = (long)(plotRect.top + yScale * (myY-yRange[0]));

		myY = pT->GetData()[j].val[1] + ub;
		if(bYLog)myY = log10(myY);
		if(!((CPlotImpl*)pT->GetPlot())->IsXOpposite())
			pt2.x = (long)(plotRect.left + xScale * (myX-xRange[0]));
		else
			pt2.x = (long)(plotRect.right - xScale * (myX-xRange[0]));
		if(!((CPlotImpl*)pT->GetPlot())->IsYOpposite())
			pt2.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]));
		else
			pt2.y = (long)(plotRect.top + yScale * (myY-yRange[0]));
		MoveToEx(hDC, pt1.x, pt1.y, NULL);
		LineTo(hDC, pt2.x, pt2.y);
		
		MoveToEx(hDC, pt1.x - hl, pt1.y, NULL);
		LineTo(hDC, pt1.x + hl, pt1.y);
		
		MoveToEx(hDC, pt2.x - hl, pt2.y, NULL);
		LineTo(hDC, pt2.x + hl, pt2.y);
	}
	SelectObject(hDC, hOldBarPen);
	DeleteObject(hBarPen);
}

Declare_Namespace_End
