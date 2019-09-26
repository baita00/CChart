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
#include "PlotDataFill.h"
#include "PlotData.h"

Declare_Namespace_CChart

template<class PlotDataT>
class CPlotDataFillImpl : public CPlotDataFill
{
public:
	CPlotDataFillImpl();
	virtual	~CPlotDataFillImpl();

protected:
	bool	AppendFillPoints( HDC hDC, RECT plotRect, double *xRange, double *yRange );
public:
	void	DrawFillArea( HDC hDC, RECT plotRect, double *xRange, double *yRange );
};

template<class PlotDataT>
CPlotDataFillImpl<PlotDataT>::CPlotDataFillImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
CPlotDataFillImpl<PlotDataT>::~CPlotDataFillImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
bool	CPlotDataFillImpl<PlotDataT>::AppendFillPoints( HDC hDC, RECT plotRect, double *xRange, double *yRange )
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);

	if(pT->GetDataCount()<=0)return false;
	if(!pT->IsVisible())return false;
	if(!m_bDataFill)return false;
	
	double xScale, yScale;
	POINT *myPoints;
	int nPts4Fill;
	xScale = pT->xScale;
	yScale = pT->yScale;
	myPoints = pT->pPointsGdi;
	nPts4Fill = pT->nPts4Fill;

	int bordersize = ((CPlotImpl*)pT->GetPlot())->IsLightDataRegion()?((CPlotImpl*)pT->GetPlot())->GetLightLineSize():((CPlotImpl*)pT->GetPlot())->GetBorderSize();
	
	int			nDatas;
	double		myX, myY;
	bool		bXLog, bYLog;
	
	POINT		point;
	
	nDatas = pT->GetDataCount();
	
	bXLog = ((CPlotImpl*)pT->GetPlot())->IsXLogarithm();
	bYLog = ((CPlotImpl*)pT->GetPlot())->IsYLogarithm();
	
	int plottype = pT->GetPlotType();
	
	switch(m_nDataFillMode)
	{
	case kDataFillClosed:
		break;
	case kDataFillFromBottomAxis:
	case kDataFillFromTopAxis:
		myX = pT->GetData()[nDatas-1].val[0];
		if(bXLog)myX = log10(myX);
		if(!((CPlotImpl*)pT->GetPlot())->IsXOpposite())
			point.x = (long)(plotRect.left + xScale * (myX-xRange[0]) + 0.5);
		else
			point.x = (long)(plotRect.right - xScale * (myX-xRange[0]) + 0.5);

		if(m_nDataFillMode == kDataFillFromBottomAxis)
			point.y = plotRect.bottom-bordersize/2.0;
		else
			point.y = plotRect.top+bordersize/2.0;
		myPoints[nPts4Fill - 2] = point;
		
		myX = pT->GetData()[0].val[0];
		if(bXLog)myX = log10(myX);
		if(!((CPlotImpl*)pT->GetPlot())->IsXOpposite())
			point.x = (long)(plotRect.left + xScale * (myX-xRange[0]) + 0.5);
		else
			point.x = (long)(plotRect.right - xScale * (myX-xRange[0]) + 0.5);
		myPoints[nPts4Fill - 1] = point;
		break;
	case kDataFillFromLeftAxis:
	case kDataFillFromRightAxis:
		myY = pT->GetData()[nDatas-1].val[1];
		if(bYLog)myY = log10(myY);
		if(!((CPlotImpl*)pT->GetPlot())->IsYOpposite())
			point.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]) + 0.5);
		else
			point.y = (long)(plotRect.top + yScale * (myY-yRange[0]) + 0.5);

		if(m_nDataFillMode == kDataFillFromLeftAxis)
			point.x = plotRect.left+bordersize/2.0;
		else
			point.x = plotRect.right-bordersize/2.0;
		myPoints[nPts4Fill - 2] = point;
		
		myY = pT->GetData()[0].val[1];
		if(bYLog)myY = log10(myY);
		if(!((CPlotImpl*)pT->GetPlot())->IsYOpposite())
			point.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]) + 0.5);
		else
			point.y = (long)(plotRect.top + yScale * (myY-yRange[0]) + 0.5);
		myPoints[nPts4Fill - 1] = point;
		break;
	default:
		return false;
	}
	return true;
}

//void	MyDrawFill(HDC hDC, COLORREF color, int alpha);

template<class PlotDataT>
void	CPlotDataFillImpl<PlotDataT>::DrawFillArea( HDC hDC, RECT plotRect, double *xRange, double *yRange )
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);

	if(!pT->IsVisible())return;
	if(!m_bDataFill)return;

	double xScale, yScale;
	POINT *myPoints;
	int nPts4Fill;
	xScale = pT->xScale;
	yScale = pT->yScale;
	myPoints = pT->pPointsGdi;
	nPts4Fill = pT->nPts4Fill;
	
	HBRUSH		hBrush, hOldBrush;
	
	if( pT->GetDataCount()<=0 ) return;
	
	int plottype = pT->GetPlotType();
	
	switch(m_nDataFillBrushType)
	{
	case kBrushSolid:
		hBrush = CreateSolidBrush( m_crDataFillColor );
		break;
	case kBrushHatch:
		hBrush = CreateHatchBrush(m_nDataFillHatchStyle,m_crDataFillColor);
		break;
	case kBrushPattern:
		if(m_hDataFillPattern)
			hBrush = CreatePatternBrush(m_hDataFillPattern);
		else
			hBrush = CreateSolidBrush( m_crDataFillColor );
		break;
	default:
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	}
	hOldBrush = (HBRUSH)SelectObject( hDC,hBrush );
	
	BeginPath(hDC);
	switch(plottype)
	{
	case kXYPlotConnect:
	case kXYPlotStepHV:
	case kXYPlotStepVH:
	case kXYPlotStepHVH:
	case kXYPlotStepVHV:
		Polyline(hDC, myPoints, nPts4Fill);
		break;
	case kXYPlotBezier:
		PolyBezier(hDC, myPoints, nPts4Fill );
		break;
	}			
	EndPath( hDC );
	
	if(m_nDataFillAlpha == 255)
	{
		// GDI+ not needed
		StrokeAndFillPath(hDC);
	}
	else
	{
		// use GDI+ to get alpha blend effect
		MyDrawFill(hDC, m_crDataFillColor, m_nDataFillAlpha);
/*		HRGN hRgn = PathToRegion(hDC);
		Region rgn(hRgn);
		
		byte a, r, g, b;
		a = m_nDataFillAlpha;
		r = GetRValue(m_crDataFillColor);
		g = GetGValue(m_crDataFillColor);
		b = GetBValue(m_crDataFillColor);
		Color fillColor(a, r, g, b);
		SolidBrush brush(fillColor);
		
		Graphics graph(hDC);
		graph.FillRegion(&brush, &rgn);
		
		DeleteObject(hRgn);*/
	}
	
	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);
}

Declare_Namespace_End
