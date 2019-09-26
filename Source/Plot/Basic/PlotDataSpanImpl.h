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
#include "PlotDataSpan.h"
#include <float.h>

Declare_Namespace_CChart

template<class PlotDataT>
class	CPlotDataSpanImpl	: public CPlotDataSpan
{
public:
	CPlotDataSpanImpl();
	virtual	~CPlotDataSpanImpl();

public:
	void	DrawDataSpan( HDC hDC, RECT plotRect, double *xRange, double *yRange);
};

template<class PlotDataT>
CPlotDataSpanImpl<PlotDataT>::CPlotDataSpanImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
CPlotDataSpanImpl<PlotDataT>::~CPlotDataSpanImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
void	CPlotDataSpanImpl<PlotDataT>::DrawDataSpan( HDC hDC, RECT plotRect, double *xRange, double *yRange)
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);

	if(pT->GetPlotType()!=kXYPlotConnect)return;

	if(m_crSpan[0]<=0.0 || m_crSpan[1]<=0.0)return;

	POINT points[6];
	double scale;
	scale = m_bSpanHorizontal?Width(plotRect)/MyRange(xRange):Height(plotRect)/MyRange(yRange);
	int i;
	for(i=0; i<pT->nPts-1; i++)
	{
		if(!m_bSpanHorizontal)if( fabs( double(pT->pPointsGdi[i].x - pT->pPointsGdi[i+1].x) )<DBL_EPSILON )continue;
		else if( fabs( double(pT->pPointsGdi[i].y - pT->pPointsGdi[i+1].y) )<DBL_EPSILON )continue;

		if(!m_bSpanHorizontal)
		{
			points[0].x = pT->pPointsGdi[i].x;
			points[0].y = pT->pPointsGdi[i].y + m_fSpan[0]*scale;
			
			points[1] = pT->pPointsGdi[i];
			
			points[2].x = pT->pPointsGdi[i].x;
			points[2].y = pT->pPointsGdi[i].y - m_fSpan[1]*scale;
			
			points[3].x = pT->pPointsGdi[i+1].x;
			points[3].y = pT->pPointsGdi[i+1].y - m_fSpan[1]*scale;
			
			points[4] = pT->pPointsGdi[i+1];
			
			points[5].x = pT->pPointsGdi[i+1].x;
			points[5].y = pT->pPointsGdi[i+1].y + m_fSpan[0]*scale;
		}
		else
		{
			points[0].x = pT->pPointsGdi[i].x - m_fSpan[0]*scale;
			points[0].y = pT->pPointsGdi[i].y;
			
			points[1] = pT->pPointsGdi[i];
			
			points[2].x = pT->pPointsGdi[i].x + m_fSpan[1]*scale;
			points[2].y = pT->pPointsGdi[i].y;
			
			points[3].x = pT->pPointsGdi[i+1].x + m_fSpan[1]*scale;
			points[3].y = pT->pPointsGdi[i+1].y;
			
			points[4] = pT->pPointsGdi[i+1];
			
			points[5].x = pT->pPointsGdi[i+1].x - m_fSpan[0]*scale;
			points[5].y = pT->pPointsGdi[i+1].y;
		}

		GradientSides(hDC, points, pT->GetPlotColor(), m_crSpan[0], m_crSpan[1]);
		if(m_bSpanBkgnd)
		{
			POINT pts[4];
			if(!m_bSpanHorizontal)
			{
				//lower
				pts[0].x = points[0].x;
				pts[0].y = plotRect.bottom-1;
				pts[1].x = points[0].x;
				pts[1].y = min(points[0].y, plotRect.bottom-1);
				pts[2].x = points[5].x;
				pts[2].y = min(points[5].y, plotRect.bottom-1);
				pts[3].x = points[5].x;
				pts[3].y = plotRect.bottom-1;
				if(memcmp(&pts[0], &pts[1],2*sizeof(POINT))!=0 || memcmp(&pts[2], &pts[3],2*sizeof(POINT))!=0)
					GradientSides(hDC, pts[0], pts[1], pts[2], pts[3], MixColor(m_crSpan[0], ((CPlotImpl*)pT->GetPlot())->GetBkgndColor(), 1), ((CPlotImpl*)pT->GetPlot())->GetBkgndColor(), m_crSpan[0]);
				//upper
				pts[0].x = points[2].x;
				pts[0].y = plotRect.top+1;
				pts[1].x = points[2].x;
				pts[1].y = max(points[2].y, plotRect.top+1);
				pts[2].x = points[3].x;
				pts[2].y = max(points[3].y, plotRect.top+1);
				pts[3].x = points[3].x;
				pts[3].y = plotRect.top+1;
				if(memcmp(&pts[0], &pts[1],2*sizeof(POINT))!=0 || memcmp(&pts[2], &pts[3],2*sizeof(POINT))!=0)
					GradientSides(hDC, pts[0], pts[1], pts[2], pts[3], MixColor(m_crSpan[1], ((CPlotImpl*)pT->GetPlot())->GetBkgndColor(), 1), ((CPlotImpl*)pT->GetPlot())->GetBkgndColor(), m_crSpan[1]);	
			}
			else
			{
				//lower
				pts[0].x = plotRect.left+1;
				pts[0].y = points[0].y;
				pts[1].x = max(points[0].x, plotRect.left+1);
				pts[1].y = points[0].y;
				pts[2].x = max(points[5].x, plotRect.left+1);
				pts[2].y = points[5].y;
				pts[3].x = plotRect.left+1;
				pts[3].y = points[5].y;
				if(memcmp(&pts[0], &pts[1],2*sizeof(POINT))!=0 || memcmp(&pts[2], &pts[3],2*sizeof(POINT))!=0)
					GradientSides(hDC, pts[0], pts[1], pts[2], pts[3], MixColor(m_crSpan[0], ((CPlotImpl*)pT->GetPlot())->GetBkgndColor(), 1), ((CPlotImpl*)pT->GetPlot())->GetBkgndColor(), m_crSpan[0]);
				//upper
				pts[0].x = plotRect.right-1;
				pts[0].y = points[2].y;
				pts[1].x = min(points[2].x, plotRect.right-1);
				pts[1].y = points[2].y;
				pts[2].x = min(points[3].x, plotRect.right-1);
				pts[2].y = points[3].y;
				pts[3].x = plotRect.right-1;
				pts[3].y = points[3].y;
				if(memcmp(&pts[0], &pts[1],2*sizeof(POINT))!=0 || memcmp(&pts[2], &pts[3],2*sizeof(POINT))!=0)
					GradientSides(hDC, pts[0], pts[1], pts[2], pts[3], MixColor(m_crSpan[1], ((CPlotImpl*)pT->GetPlot())->GetBkgndColor(), 1), ((CPlotImpl*)pT->GetPlot())->GetBkgndColor(), m_crSpan[1]);	
			}
			
		}
	}
}

Declare_Namespace_End