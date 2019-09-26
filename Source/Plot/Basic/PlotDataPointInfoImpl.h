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

#include "PlotDataPointInfo.h"
#include <windows.h>

Declare_Namespace_CChart

template<class PlotDataT>
class	CPlotDataPointInfoImpl	: public CPlotDataPointInfo
{
public:
	CPlotDataPointInfoImpl();
	virtual	~CPlotDataPointInfoImpl();

public:
	void	DrawDataPointInfo( HDC hDC, RECT plotRect, double *xRange, double *yRange);
	void	DrawSingleDataPointInfo( HDC hDC, RECT plotRect, POINT point, int whichPoint );
};

template<class PlotDataT>
CPlotDataPointInfoImpl<PlotDataT>::CPlotDataPointInfoImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
CPlotDataPointInfoImpl<PlotDataT>::~CPlotDataPointInfoImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
void	CPlotDataPointInfoImpl<PlotDataT>::DrawDataPointInfo( HDC hDC, RECT plotRect, double *xRange, double *yRange)
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);

	int j;
	
	double		myX, myY;
	bool		bXLog, bYLog;
	bXLog = ((CPlotImpl*)pT->GetPlot())->IsXLogarithm();
	bYLog = ((CPlotImpl*)pT->GetPlot())->IsYLogarithm();
	
	POINT point;
	
	//xScale = ((double)Width(plotRect))/(xRange[1] - xRange[0]);
	//yScale = ((double)Height(plotRect))/(yRange[1] - yRange[0]);
	double		xScale = pT->xScale;
	double		yScale = pT->yScale;
	
	for( j = 0; j < pT->GetDataCount(); j++ )
	{
		myX = pT->GetData()[j].val[0];
		if(bXLog)myX = log10(myX);
		myY = pT->GetData()[j].val[1];
		if(bYLog)myY = log10(myY);
		
		if(!((CPlotImpl*)pT->GetPlot())->IsXOpposite())
			point.x = (long)(plotRect.left + xScale * (myX-xRange[0]));
		else
			point.x = (long)(plotRect.right - xScale * (myX-xRange[0]));
		if(!((CPlotImpl*)pT->GetPlot())->IsYOpposite())
			point.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]));
		else
			point.y = (long)(plotRect.top + yScale * (myY-yRange[0]));
		DrawSingleDataPointInfo( hDC, plotRect, point, j);
	}
}

template<class PlotDataT>
void	CPlotDataPointInfoImpl<PlotDataT>::DrawSingleDataPointInfo( HDC hDC, RECT plotRect, POINT point, int whichPoint )
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);

	if( !m_bDataPointInfoShow ) return;
	
	// Check to see if we want to draw this marker
	if( m_nDataPointInfoFrequency > 0 )
	{
		// positive means every nth data point
		if( (whichPoint - m_nDataPointInfoStartIndex) % m_nDataPointInfoFrequency != 0 ) return;
	}
	else if( m_nDataPointInfoFrequency < 0 )
	{
		// negative means n number of times
		int			n_elements = pT->GetDataCount();
		int			n_dpinfo = abs(m_nDataPointInfoFrequency);
		if(n_dpinfo==1)
		{
			if(whichPoint!=m_nDataPointInfoStartIndex)
				return;
		}
		else
		{
			// it is not easy to find an expression, so i use a loop
			for(int i=0;i<n_dpinfo;i++)
			{
				if(whichPoint==m_nDataPointInfoStartIndex+i*(n_elements-1)/(n_dpinfo-1))
					goto NeedDrawDataPointInfo;
			}
			return;
		}
	}
	else
	{
		return; // == 0 means no marker
	}
	
NeedDrawDataPointInfo:
	
	HFONT		hOldFont, hFont;
	LOGFONT		logFont;
	SIZE		tmSize;
	POINT		ptInfo;
	
	tstring		tstr;
	tostringstream	tostr;
	
	switch(m_nDataPointInfoType)
	{
	case kShowDataPointInfoX:
		tostr<<pT->GetData()[whichPoint].val[0];
		tstr = tostr.str();
		break;
	case kShowDataPointInfoY:
		tostr<<pT->GetData()[whichPoint].val[1];
		tstr = tostr.str();
		break;
	case kShowDataPointInfoXY:
		tostr<<pT->GetData()[whichPoint].val[0]<<_TEXT("/")<<pT->GetData()[whichPoint].val[1];
		tstr = tostr.str();
		break;
	default:
		return;
	}
	
	logFont=((CPlotImpl*)pT->GetPlot())->GetTitleFont();
	logFont.lfHeight=pT->GetInfoFontsize();
	hFont = CreateFontIndirect(&logFont);
	hOldFont = (HFONT)SelectObject( hDC, hFont );
	
	MyGetTextExtentPoint(hDC, &logFont, tstr, DT_LEFT, &tmSize);
	
	ptInfo.x=point.x - tmSize.cx/2;
	ptInfo.y = point.y - tmSize.cy;
	if(pT->IsMarkerShow() && pT->GetMarkerFrequency()!=0)ptInfo.y -= pT->GetMarkerSize();

	if(ptInfo.x<plotRect.left)ptInfo.x = plotRect.left;
	if(ptInfo.x+tmSize.cx>plotRect.right)ptInfo.x=plotRect.right-tmSize.cx;
	if(ptInfo.y<plotRect.top)ptInfo.y=plotRect.top;
	if(ptInfo.y+tmSize.cy>plotRect.bottom)ptInfo.y=plotRect.bottom-tmSize.cy;
	
	SetBkMode(hDC,TRANSPARENT);
	SetTextColor(hDC,pT->GetPlotColor());
	TextOut(hDC,ptInfo.x,ptInfo.y,tstr.c_str(),(int)tstr.length());
	
	SelectObject(hDC, hOldFont );
	DeleteObject(hFont);
}

Declare_Namespace_End