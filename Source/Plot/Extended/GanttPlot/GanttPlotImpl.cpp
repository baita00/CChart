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

#include "GanttPlotImpl.h"
#include <time.h>

using namespace NsCChart;

CGanttPlotImpl::CGanttPlotImpl()
{
	CAxisImpl *axis;
	axis = AddAxis(kLocationBottom);
	if(axis)
	{
		axis->SetTime(true);
	}
	axis = AddAxis(kLocationLeft);
	if(axis)
	{
		axis->SetVisible(false);
	}

	SetLegendShow(true);
	SetLegendColorRect(true);
	SetMajorHGrids(false);
	SetMajorVGrids(true);

	SetAtomX(86400.0);
}

CGanttPlotImpl::~CGanttPlotImpl()
{

}

int		CGanttPlotImpl::AddGantt(tstring t1, tstring t2, tstring fmt)
{
	tstring ptstr[2];
	ptstr[0] = t1;
	ptstr[1] = t2;
	double pY[2];
	pY[0] = GetPlotDataCount();
	pY[1] = GetPlotDataCount();

	int dataID = AddCurveByStrTime(ptstr, fmt, pY, 2);
	return dataID;
}

int		CGanttPlotImpl::AddGantt(tstring t1, tstring t2, tstring fmt, tstring title)
{
	int dataID = AddGantt(t1, t2, fmt);
	if(dataID >= 0)
	{
		SetDataTitle(dataID, title);
	}
	return dataID;
}

void	CGanttPlotImpl::DrawSinglePlotData( int which, HDC hDC, double *xRange, double *yRange )
{
	if(which < 0 || which >= GetPlotDataCount())return;

	RECT plotRect = GetLastPlotRect();
	RECT dataRect = GetDataRect(which, plotRect, xRange, yRange);

	int dataID = GetDataID(which);

	// Draw gantt bar
	HPEN hPen, hOldPen;
	hPen = CreatePen(GetPlotLineStyle(dataID), GetPlotLineSize(dataID), GetPlotColor(dataID));
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	HBRUSH hBrush, hOldBrush;
	hBrush = CreateSolidBrush(GetPlotColor(dataID));
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	Rectangle(hDC, dataRect.left, dataRect.top, dataRect.right, dataRect.bottom);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);

	// Draw gantt titles
	if(m_bDrawGanttTitle)
	{
		HFONT hFont, hOldFont;
		hFont = CreateFontIndirect(&m_lfGanttTitleFont);
		hOldFont = (HFONT)SelectObject(hDC, hFont);

		tstring titleL, titleR;
		
		TCHAR strLabel[32];
		struct tm *myTm;
		time_t theTime;
		
		theTime = (time_t)GetAbsData(which)[0].val[0];
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		myTm = new struct tm;
		localtime_s(myTm, &theTime);
#else
		myTm = localtime(&theTime);
#endif
		
		memset(strLabel, 0, sizeof(char)*32);
		_tcsftime(strLabel, 31, m_strGanttTitleFormat.c_str(), myTm);
		titleL = strLabel;
		
		theTime = (time_t)GetAbsData(which)[1].val[0];
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		localtime_s(myTm, &theTime);
#else
		myTm = localtime(&theTime);
#endif
		memset(strLabel, 0, sizeof(char)*32);
		_tcsftime(strLabel, 31, m_strGanttTitleFormat.c_str(), myTm);
		titleR = strLabel;

		RECT lRect, rRect;
		GetGanttTitleRect(hDC, plotRect, titleL, titleR, dataRect, lRect, rRect);

		COLORREF color = GetPlotColor(dataID);
		DrawTextInRect(hDC, lRect, titleL, &m_lfGanttTitleFont, TRANSPARENT, color);
		DrawTextInRect(hDC, rRect, titleR, &m_lfGanttTitleFont, TRANSPARENT, color);
		
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		delete myTm;
#endif
	}	
}

RECT		CGanttPlotImpl::GetDataRect( int which, RECT plotRect, double *xRange, double *yRange )
{
	RECT dataRect =  {0, 0, 0, 0};
	if(which < 0 || which >= GetPlotDataCount())return dataRect;
	
	double xScale, yScale;
	xScale = (plotRect.right - plotRect.left)/(xRange[1] - xRange[0]);
	yScale = (plotRect.bottom - plotRect.top)/(yRange[1] - yRange[0]);
	
	int nData = GetPlotDataCount();
	double dy = (plotRect.bottom - plotRect.top)/(nData + 1.0);
	double width = dy*m_fRatioOccupy;

	int	pos;
	if(m_bReverseOrder)
		pos = nData - which;
	else
		pos = which + 1;
	
	int x1, x2, y1, y2;
	x1 = int(plotRect.left + (GetAbsData(which)[0].val[0] - xRange[0])*xScale);
	x2 = int(plotRect.left + (GetAbsData(which)[1].val[0] - xRange[0])*xScale);
	y1 = int(plotRect.bottom - pos * dy - width/2.0);
	y2 = int(y1 + width);
	
	dataRect.left = x1;
	dataRect.right = x2;
	dataRect.top = y1;
	dataRect.bottom = y2;
	return dataRect;
}

void	CGanttPlotImpl::GetGanttTitleRect(HDC hDC, RECT plotRect, tstring titleL, tstring titleR, RECT dataRect, RECT &leftRect, RECT &rightRect )
{
	SIZE	tmSizeL, tmSizeR;

	MyGetTextExtentPoint(hDC, &m_lfGanttTitleFont, titleL, DT_LEFT, &tmSizeL);
	MyGetTextExtentPoint(hDC, &m_lfGanttTitleFont, titleR, DT_LEFT, &tmSizeR);

	if(m_bGanttTitleUpper)
	{
		leftRect.left = dataRect.left;
		leftRect.right = leftRect.left + tmSizeL.cx;
		leftRect.bottom = dataRect.top;
		leftRect.top = leftRect.bottom - tmSizeL.cy;

		rightRect.right = dataRect.right;
		rightRect.left = rightRect.right - tmSizeR.cx;
		rightRect.bottom = dataRect.top;
		rightRect.top = rightRect.bottom - tmSizeR.cy;

		if(leftRect.top < plotRect.top)
		{
			leftRect.top += (plotRect.top - leftRect.top);
			leftRect.bottom += (plotRect.top - leftRect.top);
		}
	}
	else
	{
		leftRect.left = dataRect.left;
		leftRect.right = leftRect.left + tmSizeL.cx;
		leftRect.top = dataRect.bottom;
		leftRect.bottom = leftRect.top + tmSizeL.cy;
		
		rightRect.right = dataRect.right;
		rightRect.left = rightRect.right - tmSizeR.cx;
		rightRect.top = dataRect.bottom;
		rightRect.bottom = rightRect.top + tmSizeR.cy;
		
		if(leftRect.bottom > plotRect.bottom)
		{
			leftRect.bottom -= (leftRect.bottom - plotRect.bottom);
			leftRect.top -= (leftRect.bottom - plotRect.bottom);
		}
	}
}

void	CGanttPlotImpl::GetPlotRange( double *xRange, double *yRange )
{
	GetDataRanges(xRange, yRange);
	SetXRange(xRange[0], xRange[1]);

	int nData = GetPlotDataCount();
	SetYRange( 0, nData+1 );
	yRange[0] = 0;
	yRange[1] = nData+1;

//	memcpy(m_pfXPlotRange, xRange, 2*sizeof(double));
//	memcpy(m_pfYPlotRange, yRange, 2*sizeof(double));

	SetXRange(xRange);
	SetYRange(yRange);

	//memcpy(m_pfLastXPlotRange, xRange, 2*sizeof(double));
	//memcpy(m_pfLastYPlotRange, yRange, 2*sizeof(double));
	SetLastPlotRange(xRange, yRange);
}

int		CGanttPlotImpl::GetNeareastPlotData( HDC hDC, POINT point )
{
	if(GetPlotDataCount()<=0)return -1;
	
	int			i, nIdx;
	
	double		xRange[] = {fMaxVal, -fMaxVal}, yRange[]={fMaxVal,-fMaxVal};
	GetLastPlotRange( xRange, yRange );

	RECT plotRect = GetLastPlotRect( );
	
	nIdx = -1;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		if( PtInDataArea( i, hDC, point, plotRect, xRange, yRange) )
		{
			nIdx = i;
			break;
		}
	}
	if(nIdx==-1)
		return -1;
	else
		return GetDataID(nIdx);
}

bool	CGanttPlotImpl::PtInDataArea( int which, HDC hDC, POINT point, RECT plotRect, double *xRange, double *yRange )
{
	if(which<0 || which>=GetPlotDataCount())return false;
	if(!GetAbsPlotData(which)->IsVisible())return false;
	
	CPlotData<DataPoint2D>	*dataSet = GetAbsPlotData( which );
	if( dataSet->GetDataCount()<=0 ) return false;
	
	RECT rt;
	for(int i=0; i<(int)GetAbsData(which).size(); i++)
	{
		rt = GetDataRect(which, plotRect, xRange, yRange);
		if( PtInRect(&rt, point) )return true;
	}
	
	return false;
}

