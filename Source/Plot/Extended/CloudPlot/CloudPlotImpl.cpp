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

//#include "StdAfx.h"
#include "CloudPlotImpl.h"
#include "CloudLegendImpl.h"
#include "../../Basic/PlotUtility.h"
#include "../../Basic/MyMemDC.h"

using namespace NsCChart;

CCloudPlotImpl::CCloudPlotImpl()
{
	//m_bDrawLegendInChild = false;
}

CCloudPlotImpl::~CCloudPlotImpl()
{

}

RECT	CCloudPlotImpl::AddtionalPlotRectAdjust(HDC hDC, RECT plotRect)
{
	return CCloudLegendImpl<CCloudPlotImpl>::AdjustByLegend(hDC, plotRect);
}

void	CCloudPlotImpl::GetDataRange1D( int whichDim, double *range )
{
	double xRange[2], yRange[2];
	GetContourRange(xRange, yRange);
	
	if(whichDim == 0)memcpy(range, xRange, 2*sizeof(double));
	else memcpy(range, yRange, 2*sizeof(double));
}

int		CCloudPlotImpl::RegionIdentifyEx(HDC hDC, POINT point, bool &legendRegion, int &centerAxisRegion)
{
	int region = RegionIdentify(hDC, point);
	
	int which = LegendRegionIdentify(hDC,point);
	if(which>=0)
	{
		legendRegion = true;
	}
	else
	{
		legendRegion = false;
	}
	
	if(region == kXYRegionData )
	{
		RECT theRect;
		CAxis *axis;
		for(int i=0; i<GetAxesCount(); i++)
		{
			axis=GetAxis(i);
			theRect=((CAxisImpl*)axis)->GetPlotRect(hDC);//, GetLastClientRect(), GetLastPlotRect( ));
			//if( (theRect.right-point.x)*(point.x-theRect.left)>=0 && (theRect.top-point.y)*(point.y-theRect.bottom)>=0 )
			if( PtInRect(&theRect,point) )
			{
				if(axis->GetLocation()==kLocationCenterHB)centerAxisRegion = kLocationCenterHB;
				else if(axis->GetLocation()==kLocationCenterHT)centerAxisRegion = kLocationCenterHT;
				else if(axis->GetLocation()==kLocationCenterVL)centerAxisRegion = kLocationCenterVL;
				else if(axis->GetLocation()==kLocationCenterVR)centerAxisRegion = kLocationCenterVR;
				else centerAxisRegion = -1;
			}
			else
			{
				centerAxisRegion = -1;
			}
		}
	}
	else
	{
		centerAxisRegion = -1;
	}
	
	return region;
}

void	CCloudPlotImpl::SetAxisLegend(int location, bool set)
{
	CAxisImpl *axis = GetAxisByLocation(location);
	if(!axis)return;
	if(set)
	{
		axis->SetIndependent(true);
		axis->SetShowColorBar(true);
		axis->SetBarColor1(GetColor1());
		axis->SetBarColor2(GetColor2());
		axis->GetPlotIndep()->SetYRange(GetMinValue(), GetMaxValue());
		axis->SetColorBarMarginInner(0);
		axis->SetAxisGap(axis->GetLineSize());
	}
	else
	{
		axis->SetIndependent(false);
	}
}

void	CCloudPlotImpl::SetAllAxesLegend()
{
	if(m_bLeftAxisAsLegend)SetAxisLegend(kLocationLeft, true);
	if(m_bRightAxisAsLegend)SetAxisLegend(kLocationRight, true);
	if(m_bTopAxisAsLegend)SetAxisLegend(kLocationTop, true);
	if(m_bBottomAxisAsLegend)SetAxisLegend(kLocationBottom, true);
}

void	CCloudPlotImpl::DrawBasic( HDC hDC, RECT destRect )
{
	CPlotImpl::FinalDraw(hDC, destRect);
}

void	CCloudPlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
//	int nOldMapmode = SetMapMode(hDC, MM_TEXT);

	// Get ranges for plotting
	double xRange[2], yRange[2];
	//GetPlotRange( xRange, yRange );	

	// Create Bitmap
	if(!m_hMemImageDC)
	{
		CreateMemDC(WindowFromDC(hDC));
	}
	if(m_bContourChanged)
	{
		GetPlotRange( xRange, yRange );
		CreateImage();
	}
	SetAllAxesLegend();

	HDC hdc;
	CMyMemDC memdc;
	if(!m_bDoubleBuffer)
	{
		hdc = hDC;
	}
	else
	{
		memdc->CreateMemDC(hDC, NULL, &destRect);
		hdc = memdc;
	}

//	EnterCriticalSection(&m_csPlot);

	RECT plotRect  = GetPlotRect(hDC, destRect);
	m_rctClient = destRect;
	m_rctPlot = plotRect;

	DrawBkgnd(hdc);
	
	// Draw the map
	//UpdateImage(m_rctPlot);
	DrawCloud(hdc, plotRect, m_bXOpposite, m_bYOpposite);
	
	// Draw basics
	SetEraseReservePlotRect(true);
	DrawBasic(hdc, destRect);

	DrawLegend(hdc);

//	LeaveCriticalSection(&m_csPlot);
	

//	SetMapMode(hDC, nOldMapmode);
}

tstring	CCloudPlotImpl::GetAbsDataTitle( int which )
{
	return GetLegendTitle(which);
}

tstring	CCloudPlotImpl::GetAbsDataAppendix( int which )
{
	return _TEXT("");
}

int	CCloudPlotImpl::GetPlotDataCount( void )
{
	return GetLegends();
}

void	CCloudPlotImpl::GetPlotRange( double *xRange, double *yRange )
{
	GetContourRange(xRange, yRange);
	//memcpy(m_pfXPlotRange, xRange, 2*sizeof(double));
	//memcpy(m_pfYPlotRange, yRange, 2*sizeof(double));
	SetXRange(xRange[0], xRange[1]);
	SetYRange(yRange[0], yRange[1]);

	//memcpy(m_pfLastXPlotRange, xRange, 2*sizeof(double));
	//memcpy(m_pfLastYPlotRange, yRange, 2*sizeof(double));
	SetLastPlotRange(xRange, yRange);

	SetExactXRange(true);
	SetExactYRange(true);

	SetXRangeSet(true);
	SetYRangeSet(true);

	double xdr[2], ydr[2];
	GetDataRanges(xdr, ydr);
	GetClsRange(0)->CalcRange(xRange, xdr, IsNewDataComming());
	GetClsRange(1)->CalcRange(yRange, ydr, IsNewDataComming());
}

CCloudPlotImplI::CCloudPlotImplI()
{

}

CCloudPlotImplI::~CCloudPlotImplI()
{

}
/*
bool	CCloudPlotImplI::ShowDlgBySelection(HDC hDC)
{
	bool needUpdate = false;
	if(IsSelectTitle())
	{
		if(DefPlotSettings(this, hDC))
		{
			needUpdate = true;
		}
	}
	else if(IsAxisSelected())
	{
		int index, location;
		index = GetIndexOfSelectedAxis();
		location = GetAxis(index)->GetLocation();
		if(DefAxisSettings(this,hDC,location))
		{
			needUpdate = true;
		}
	}
	else
	{
		if(DefPlotSettings(this,hDC))
		{
			needUpdate = true;
		}
	}
	
	return needUpdate;
}
*/