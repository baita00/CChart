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

#include "PlotDataHisto.h"
#include <windows.h>

Declare_Namespace_CChart

template<class PlotDataT>
class	CPlotDataHistoImpl : public	CPlotDataHisto
{
public:
	CPlotDataHistoImpl();
	virtual	~CPlotDataHistoImpl();
public:
	void	DrawHisto( HDC hDC, RECT plotRect, double *xRange, double *yRange );
};

template<class PlotDataT>
CPlotDataHistoImpl<PlotDataT>::CPlotDataHistoImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
	//pT->SetPlotType(kXYPlotStepHV);
	//pT->SetDataFill(true);
}

template<class PlotDataT>
CPlotDataHistoImpl<PlotDataT>::~CPlotDataHistoImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
void		CPlotDataHistoImpl<PlotDataT>::DrawHisto( HDC hDC, RECT plotRect, double *xRange, double *yRange )
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);

	if(pT->GetDataCount()<=0)return;
	if(!pT->IsVisible())return;
		
	int			nDatas;
	POINT		point;
	
	POINT *myPoints;
	myPoints = pT->pPointsGdi;
	nDatas = pT->GetDataCount();

	int plottype = pT->GetPlotType();
	int j;

	int linestyle, linesize, linecolor;
	linestyle = pT->GetPlotLineStyle();
	linesize = pT->GetPlotLineSize();
	linecolor = pT->GetPlotColor();
	HPEN hPen, hOldPen;
	hPen = CreatePen(linestyle, linesize, linecolor);
	hOldPen = (HPEN)SelectObject(hDC, hPen);

	if(m_bHisto)
	{
		switch(plottype)
		{
		case kXYPlotStepHV:
		case kXYPlotStepVH:
			for( j = 0; j < nDatas; j++ )
			{			
				point = myPoints[2*j];

				if(j>0)
				{
					switch(m_nHistoMode)
					{
					case kHistoFromBottom:
						if(myPoints[2*(j-1)].y<point.y)point.y = myPoints[2*(j-1)].y;
						break;
					case kHistoFromTop:
						if(myPoints[2*(j-1)].y>point.y)point.y = myPoints[2*(j-1)].y;
						break;
					case kHistoFromLeft:
						if(myPoints[2*(j-1)].x>point.x)point.x = myPoints[2*(j-1)].x;
						break;
					case kHistoFromRight:
						if(myPoints[2*(j-1)].x<point.x)point.x = myPoints[2*(j-1)].x;
						break;
					}
				}
				//Histo mode
				switch(m_nHistoMode)
				{
				case kHistoFromBottom:
					MoveToEx(hDC, point.x, plotRect.bottom, NULL);
					LineTo(hDC, point.x, point.y);
					break;
				case kHistoFromTop:
					MoveToEx(hDC, point.x, plotRect.top, NULL);
					LineTo(hDC, point.x, point.y);
					break;
				case kHistoFromLeft:
					MoveToEx(hDC, plotRect.left, point.y, NULL);
					LineTo(hDC, point.x, point.y);
					break;
				case kHistoFromRight:
					MoveToEx(hDC, plotRect.right, point.y, NULL);
					LineTo(hDC, point.x, point.y);
					break;
				}
			}
			break;
		case kXYPlotStepHVH:
		case kXYPlotStepVHV:
			for( j = 0; j < nDatas-1; j++ )
			{
				point = myPoints[3*j];

				if(j>0)
				{
					switch(m_nHistoMode)
					{
					case kHistoFromBottom:
						if(myPoints[3*(j-1)].y<point.y)point.y = myPoints[3*(j-1)].y;
						break;
					case kHistoFromTop:
						if(myPoints[3*(j-1)].y>point.y)point.y = myPoints[3*(j-1)].y;
						break;
					case kHistoFromLeft:
						if(myPoints[3*(j-1)].x>point.x)point.x = myPoints[3*(j-1)].x;
						break;
					case kHistoFromRight:
						if(myPoints[3*(j-1)].x<point.x)point.x = myPoints[3*(j-1)].x;
						break;
					}
				}

				switch(m_nHistoMode)
				{
				case kHistoFromBottom:
					MoveToEx(hDC, point.x, plotRect.bottom, NULL);
					LineTo(hDC, point.x, point.y);
					break;
				case kHistoFromTop:
					MoveToEx(hDC, point.x, plotRect.top, NULL);
					LineTo(hDC, point.x, point.y);
					break;
				case kHistoFromLeft:
					MoveToEx(hDC, plotRect.left, point.y, NULL);
					LineTo(hDC, point.x, point.y);
					break;
				case kHistoFromRight:
					MoveToEx(hDC, plotRect.right, point.y, NULL);
					LineTo(hDC, point.x, point.y);
					break;
				}
			}
			break;

		}
	}
	hPen = (HPEN)SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

Declare_Namespace_End

