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

#include "../../Basic/MyGdiPlus.h"
#include "ContourPlotImpl.h"
#include "../../Basic/MyMemDC.h"

using namespace NsCChart;

CContourPlotImpl::CContourPlotImpl()
{

}

CContourPlotImpl::~CContourPlotImpl()
{
	
}

void	CContourPlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	//	int nOldMapmode = SetMapMode(hDC, MM_TEXT);
	
	// Create Bitmap
	if(!m_hMemImageDC)
	{
		CreateMemDC(WindowFromDC(hDC));
	}
	if(m_bContourChanged)
	{
		CreateImage();
	}
	if(!IsContourCreated() || m_bContourChanged)
	{
		CreateContour();
		
		if(GetFieldFcn()==ContourHeight)
		{
			vector<double> vX, vY;
			for(int i=0; i<(int)m_vContourPoints.size(); i++)
			{
				vX.push_back(m_vContourPoints[i].c[0]);
				vY.push_back(m_vContourPoints[i].c[1]);
			}
			int dataID = AddCurve(vX, vY);
			SetPlotType(dataID, kXYPlotScatter);
			SetMarkerShow(dataID, true);
			SetDataTitle(dataID, _TEXT("data points"));
		}
	}
	m_bContourChanged = false;
	
	SetAllAxesLegend();

	double xRange[2], yRange[2];
	GetPlotRange( xRange, yRange );	
	
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
		
	RECT plotRect  = GetPlotRect(hDC, destRect);
	m_rctClient = destRect;
	m_rctPlot = plotRect;
	
//	EnterCriticalSection(&m_csPlot);

	DrawBkgnd(hdc);
	
	// Draw the map
	//UpdateImage(m_rctPlot);
	DrawCloud(hdc, plotRect, m_bXOpposite, m_bYOpposite);
	
	// Draw basics
	SetEraseReservePlotRect(true);
	CPlotImpl::FinalDraw(hdc, destRect);

	DrawDataSet(hdc, xRange, yRange);
	
	CCloudLegendImpl<CContourPlotImpl>::DrawLegend(hdc);
	
//	LeaveCriticalSection(&m_csPlot);
	
	//	SetMapMode(hDC, nOldMapmode);
}

void	CContourPlotImpl::SetAxisLegend(int location, bool set)
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

void	CContourPlotImpl::SetAllAxesLegend()
{
	if(m_bLeftAxisAsLegend)SetAxisLegend(kLocationLeft, true);
	if(m_bRightAxisAsLegend)SetAxisLegend(kLocationRight, true);
	if(m_bTopAxisAsLegend)SetAxisLegend(kLocationTop, true);
	if(m_bBottomAxisAsLegend)SetAxisLegend(kLocationBottom, true);
}


CContourPlotImplI::CContourPlotImplI()
{

}

CContourPlotImplI::~CContourPlotImplI()
{

}
/*
bool	CContourPlotImplI::ShowDlgBySelection(HDC hDC)
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
