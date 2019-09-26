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
#include "ShareXPlotImpl.h"
#include "ShareXLayerImpl.h"
#include "../../Basic/PlotUtility.h"
#include "../../Basic/MyMemDC.h"

using namespace NsCChart;

///////////////////////////////////////////////////////////////////////////////////////////////////
//

CShareXPlotImpl::CShareXPlotImpl()
{
	CAxisImpl *axis;
	axis = CPlotImpl::AddAxis(kLocationBottom);

	m_nTopLayer = -1;
}

CShareXPlotImpl::~CShareXPlotImpl()
{
	ClearLayers();
}

int		CShareXPlotImpl::AddLayer()
{
	CShareXLayerImplI *tmp = new CShareXLayerImplI(this);
	int num = GetLayerCount();
	tmp->SetAutoColorStartIndex(12*num%CPlot::m_nColorsInTable);
	m_vpLayers.push_back(tmp);	
	return (int)m_vpLayers.size();
}

int		CShareXPlotImpl::SetLayerCount( int num )
{
	ClearLayers();
	if(num > 0)
	{
		m_vpLayers.resize(num);
		for(int i=0; i<(int)m_vpLayers.size(); i++)
		{
			m_vpLayers[i] = new CShareXLayerImplI(this);
		}
	}
	return (int)m_vpLayers.size();
}

void	CShareXPlotImpl::GetPlotRange( double xRange[2], double yRange[2] )
{
	xRange[0]=-fMaxVal;
	xRange[1]=fMaxVal;
	yRange[0]=-fMaxVal;
	yRange[1]=fMaxVal;

	if(!NeedDrawing())
	{
		xRange[0]=yRange[0]=0.0;
		xRange[1]=yRange[1]=50.0;
		return;
	}
	
	double xrng[2],yrng[2];

	GetSubPlotRanges();
	if(m_bOriginRange)
	{
		GetDataRange1D(0, xRange);
		GetDataRange1D(1, yRange);
	}
	else
	{
		GetPlotRange1D(0, xRange);
		GetPlotRange1D(1, yRange);
	}

	int nTicks,nMinorTicks;
	if(IsRangeUpdateNeeded())
	{
		if(m_bShareExact)
		{
			GetOptimalRangeExact(xRange, xrng, &nTicks, &nMinorTicks);
		}
		else
		{
			GetOptimalRange(xRange, xrng, &nTicks, &nMinorTicks, true, true, false, false, false, false);
		}
		memcpy(xRange, xrng, 2*sizeof(double));
	}
	else
	{
//		GetXRange(xRange);
//		nTicks = GetXTickCount();
//		nMinorTicks = GetXMinorTickCount();
		GetPlotRange1D(0, xRange);
		GetPlotRange1D(1, yRange);
		return;
	}

	int i;
	// Moved to lower
/*	for(i=0; i<(int)m_vpLayers.size(); i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		m_vpLayers[i]->SetXRange(xRange[0],xRange[1]);
		m_vpLayers[i]->SetLastXPlotRange(xRange);
	}*/
	

	if(!IsFloatXTicks())
	{
		// Moved from upper
		for(i=0; i<(int)m_vpLayers.size(); i++)
		{
			if(!m_vpLayers[i]->NeedDrawing())continue;
			m_vpLayers[i]->SetXRange(xRange[0],xRange[1]);
			m_vpLayers[i]->SetLastXPlotRange(xRange);
	
		}

		SetXRange( xRange[0],xRange[1] );
		SetLastXPlotRange(xRange);

		SetXTickCount(nTicks);
		SetXMinorTickCount(nMinorTicks);
	}
	else
	{
		int nTicks;
		double tickMin, tickMax;
		RegulateTicks(xRange[0], xRange[1], nTicks, nMinorTicks, tickMin, tickMax);
		SetXRange( xRange[0],xRange[1] );
		SetLastXPlotRange(xRange);
		SetXTickCount(nTicks-2);
		SetXMinorTickCount(nMinorTicks);
		SetXTickMin(tickMin);
		SetXTickMax(tickMax);

	}

	int nGridLayer = m_nGridBindLayer;
	if(nGridLayer<0)nGridLayer=0;
	if(nGridLayer>=(int)m_vpLayers.size())nGridLayer=(int)m_vpLayers.size()-1;
	m_vpLayers[nGridLayer]->GetLastPlotRange( xrng, yrng );
	SetYRange(yrng[0], yrng[1]);
	SetLastYPlotRange(yrng);
	SetYTickCount(m_vpLayers[nGridLayer]->GetYTickCount());
	SetYMinorTickCount(m_vpLayers[nGridLayer]->GetYMinorTickCount());
	memcpy(yRange, yrng, 2*sizeof(double));

	//memcpy(m_pfXPlotRange, xRange, 2*sizeof(double));
	//memcpy(m_pfYPlotRange, yRange, 2*sizeof(double));
	SetXRange(xRange);
	SetYRange(yRange);
	SetLastPlotRange(xRange, yRange);

	if(IsExactXRange())
	{
		GetDataRange1D(0, xRange);
		SetXRange(xRange);
		SetLastXPlotRange(xRange);
		for(i=0; i<(int)m_vpLayers.size(); i++)
		{
			if(!m_vpLayers[i]->NeedDrawing())continue;
			m_vpLayers[i]->SetXRange(xRange[0],xRange[1]);
			m_vpLayers[i]->SetLastXPlotRange(xRange);
		}
	}

	SetFirstDraw(false);
	SetNewDataComming(false);
}

SIZE	CShareXPlotImpl::GetAxisSize( HDC hDC, int location )
{
	SIZE axisSize={0,0};
	SIZE dispSize,paddingSize,maxPaddingSize;
	int i;
	CAxisImpl *axis;
	switch(location)
	{
	case kLocationBottom:
	case kLocationTop:
		axis=GetAxisByLocation(location);
		if(axis)
		{
			dispSize = axis->GetMinDisplaySize( hDC );
			paddingSize = axis->GetPaddingSize( hDC );
			axisSize = dispSize;
			axisSize.cx += (paddingSize.cx+paddingSize.cy);
			axisSize.cy += axis->GetAxisGap();
		}
		break;
	case kLocationLeft:
	case kLocationRight:
		for(i=0;i<GetLayerCount();i++)
		{
			if(!m_vpLayers[i]->NeedDrawing())continue;
			SIZE tmp={0,0};
			maxPaddingSize=tmp;
			axis=m_vpLayers[i]->GetAxisByLocation(location);
			if(axis)
			{
				dispSize = axis->GetMinDisplaySize( hDC );
				paddingSize = axis->GetPaddingSize( hDC );
				axisSize.cx += dispSize.cx;
				if( dispSize.cy > axisSize.cy ) axisSize.cy = dispSize.cy;
				if( paddingSize.cx > maxPaddingSize.cx ) maxPaddingSize.cx = paddingSize.cx;
				if( paddingSize.cy > maxPaddingSize.cy ) maxPaddingSize.cy = paddingSize.cy;

				axisSize.cx += axis->GetAxisGap();
			}
		}
		axisSize.cy+=(maxPaddingSize.cx+maxPaddingSize.cy);
		break;
	default:
		break;
	}
	return axisSize;
}

SIZE	CShareXPlotImpl::GetAxisPaddingSize( HDC hDC, int location )
{
	SIZE paddingSize,maxPaddingSize={0,0};
	int i;
	CAxisImpl *axis;
	switch(location)
	{
	case kLocationBottom:
	case kLocationTop:
		axis=GetAxisByLocation(location);
		if(axis)
		{
			maxPaddingSize = axis->GetPaddingSize( hDC );
		}
		break;
	case kLocationLeft:
	case kLocationRight:
		for(i=0;i<GetLayerCount();i++)
		{
			if(!m_vpLayers[i]->NeedDrawing())continue;
			axis=m_vpLayers[i]->GetAxisByLocation(location);
			if(axis)
			{
				paddingSize = axis->GetPaddingSize( hDC );

				if( paddingSize.cx > maxPaddingSize.cx ) maxPaddingSize.cx = paddingSize.cx;
				if( paddingSize.cy > maxPaddingSize.cy ) maxPaddingSize.cy = paddingSize.cy;
			}
		}
		break;
	default:
		break;
	}
	return maxPaddingSize;
}

void	CShareXPlotImpl::GetPlotRange1D( int whichDim, double *range )
{
	if(whichDim != 0)
	{
		CLayeredPlotBasicImpl<CShareXLayerImplI>::GetPlotRange1D(whichDim, range);
		return;
	}
	
	double rng[2];
	range[0] = fMaxVal;
	range[1] = -fMaxVal;
		
	int i;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;

		if(IsToCalcXRange() && !m_vpLayers[i]->IsToCalcXRange())continue;
		
		m_vpLayers[i]->GetPlotRange1D( whichDim, rng );
		
		if(IsXLogarithm())
		{
			range[0] = min( range[0], log10(rng[0]) );
			range[1] = max( range[1], log10(rng[1]) );
		}
		else
		{
			range[0] = min( range[0], rng[0] );
			range[1] = max( range[1], rng[1] );
		}
	}
}

bool	CShareXPlotImpl::IsToCalcXRange()
{
	int i;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		if(m_vpLayers[i]->IsToCalcXRange())return true;
	}
	return false;
}
/*
*/
RECT	CShareXPlotImpl::GetPlotRect( HDC hDC, RECT clientRect)
{
	//////////////////////////////////////////////////////////////
	// Get Plot Rect
	int i;

	CAxisImpl *axis;
	
	RECT plotRect=clientRect;
	SIZE sizeRect={0,0},sizeTmp={0,0};
	
	// First title
	plotRect = AdjustByTitle(hDC, plotRect);
	
	// then top axis
	sizeRect = GetAxisSize( hDC, kLocationTop);
	plotRect.top+=sizeRect.cy;
	
	// then bottom axis
	sizeRect = GetAxisSize( hDC, kLocationBottom);
	plotRect.bottom-=sizeRect.cy;
	
	// then left axis
	sizeRect.cx = sizeRect.cy = 0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		axis = m_vpLayers[i]->GetAxisByLocation(kLocationLeft);
		if(axis)
		{
			if(axis->IsVisible())
			{
				sizeTmp=m_vpLayers[i]->GetAxisSize( hDC, kLocationLeft );
				sizeRect.cx+=sizeTmp.cx;
			}
		}
		
	}
	plotRect.left+=sizeRect.cx;
	
	// then right axis
	sizeRect.cx = sizeRect.cy = 0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		axis = m_vpLayers[i]->GetAxisByLocation(kLocationRight);
		
		if(axis)
		{
			if(axis->IsVisible())
			{
				sizeTmp=m_vpLayers[i]->GetAxisSize( hDC, kLocationRight );
				sizeRect.cx+=sizeTmp.cx;
			}
		}
	}
	plotRect.right-=sizeRect.cx;

	// the right padding size
	SIZE paddingSize;
	bool exist;
	exist = false;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		if( m_vpLayers[i]->GetAxisByLocation(kLocationRight) )
		{
			exist = true;
			break;
		}
	}
	if(!exist)
	{
		paddingSize = GetAxisPaddingSize( hDC, kLocationBottom);
		plotRect.right -= paddingSize.cy;
	}
	// the left padding size
	exist = false;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		if( m_vpLayers[i]->GetAxisByLocation(kLocationLeft) )
		{
			exist = true;
			break;
		}
	}
	if(!exist)
	{
		paddingSize = GetAxisPaddingSize( hDC, kLocationBottom);
		plotRect.left += paddingSize.cx;
	}
	// the top padding size
	if(!GetAxisByLocation(kLocationTop))
	{
		if(!m_bTitleShow || GetTitle().length()==0 || GetTitlePosition() != kPositionT)
		{
			SIZE ps1, ps2;
			ps1 = GetAxisPaddingSize( hDC, kLocationLeft );
			ps2 = GetAxisPaddingSize( hDC, kLocationRight );
			plotRect.top += max(ps1.cy, ps2.cy);
		}
		
	}
	// the bottom padding size
	if(!GetAxisByLocation(kLocationBottom))
	{
		if(!m_bTitleShow || GetTitle().length()==0 || GetTitlePosition() != kPositionB)
		{
			SIZE ps1, ps2;
			ps1 = GetAxisPaddingSize( hDC, kLocationLeft );
			ps2 = GetAxisPaddingSize( hDC, kLocationRight );
			plotRect.bottom -= max(ps1.cx, ps2.cx);
		}
		
	}
	
	// header and footnote
	plotRect = AdjustPlotSizeForHeadernote(hDC, plotRect);
	plotRect = AdjustPlotSizeForFootnote(hDC, plotRect);
	// legend
	plotRect = AddtionalPlotRectAdjust(hDC, plotRect);
	// Add spacing
	//InflateRect( &plotRect, -10, -10 );
	plotRect.left += m_nMarginLeft;
	plotRect.right -= m_nMarginRight;
	plotRect.top += m_nMarginTop;
	plotRect.bottom -= m_nMarginBottom;
	
	return plotRect;
}

void	CShareXPlotImpl::DrawBasic( HDC hDC, RECT destRect )
{
	int i;
	
	// Get number of left axis and right axis
	CAxisImpl *axis=NULL;

	// get plot rect
	RECT plotRect=GetPlotRect(hDC,destRect);
	SetLastClientRect(destRect);
	SetLastPlotRect(plotRect);

	// Erase the background using parent function
	//DrawBkgnd( hDC );

	// First Draw Frame
	//DrawFrame(hDC);
	DrawCanvas(hDC);

	// Draw the grids using the parent function
	DrawGrids( hDC );

	// The draw title
	//RECT titleRect = GetTitleRect( hDC, destRect, plotRect);
	if(GetTop()<0 || m_bShowParentTitle)
	{
		DrawTitle(hDC);
	}
	else if(GetTop()<GetLayerCount())
	{
		tstring title = GetTitle();
		SetTitle(GetLayer(GetTop())->GetTitle());
		DrawTitle(hDC);
		SetTitle(title);
	}

	// then bottom and top axis;
	axis=GetAxisByLocation(kLocationBottom);
	if(axis)
	{
		axis->OnDraw(hDC);
	}
	axis=GetAxisByLocation(kLocationTop);
	if(axis)
	{
		axis->OnDraw(hDC);
	}
	// then left and right axis;
	SIZE axisSize;
	RECT newRect=plotRect;
	
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
//		if(!m_vpLayers[i]->NeedDrawing())continue;

		axis=m_vpLayers[i]->GetAxisByLocation(kLocationLeft);
		{
			if(axis)
			{
				if(axis->IsVisible())
				{
					m_vpLayers[i]->SetLastClientRect(destRect);
					m_vpLayers[i]->SetLastPlotRect(newRect);
					axis->OnDraw(hDC);
					
					axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationLeft );
					newRect.left-=axisSize.cx;
				}
				
			}
		}
		
		axis=m_vpLayers[i]->GetAxisByLocation(kLocationRight);
		{
			if(axis)
			{
				if(axis->IsVisible())
				{
					m_vpLayers[i]->SetLastClientRect(destRect);
					m_vpLayers[i]->SetLastPlotRect(newRect);
					axis->OnDraw(hDC);
					
					axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationRight );
					newRect.right+=axisSize.cx;
				}
				
			}
		}
		m_vpLayers[i]->SetLastPlotRect(plotRect);
	}
}

int		CShareXPlotImpl::RegionIdentify(HDC hDC,POINT point)
{
	//CAxis *pAxis=0;
	RECT theRect;
	
	//data region
	theRect=GetLastPlotRect( );
	if( PtInRect(&theRect, point) )
		return kXYRegionData;
	//title region
	theRect=GetTitleRect(hDC);
	if( PtInRect(&theRect, point) )
		return kXYRegionTitle;
	//top and bottom axis region
	CAxisImpl *axis;
	for(int i=0;i<CPlotImpl::GetAxesCount();i++)
	{
		axis=CPlotImpl::GetAxis(i);
		theRect=axis->GetPlotRect(hDC);
		if( PtInRect(&theRect, point) )
		{
			if(axis->GetLocation()==kLocationBottom)return kXYRegionBottom;
			if(axis->GetLocation()==kLocationTop)return kXYRegionTop;
		}
	}
	
	//left and right axis region
	theRect=GetLastPlotRect( );
	SIZE axisSize,padddingSize;
	RECT axisRect;
	
	axisSize = GetAxisSize(hDC,kLocationLeft);
	padddingSize = GetAxisPaddingSize(hDC, kLocationLeft);
	axisRect.right = theRect.left;
	axisRect.left = axisRect.right - axisSize.cx;
	axisRect.top = theRect.top - padddingSize.cx;
	axisRect.bottom = theRect.bottom + padddingSize.cy;
	if( PtInRect(&axisRect, point) )
		return kXYRegionLeft;
	
	axisSize = GetAxisSize(hDC,kLocationRight);
	padddingSize = GetAxisPaddingSize(hDC, kLocationRight);
	axisRect.left = theRect.right;
	axisRect.right = axisRect.left + axisSize.cx;
	axisRect.top = theRect.top - padddingSize.cx;
	axisRect.bottom = theRect.bottom + padddingSize.cy;
	if( PtInRect(&axisRect, point) )
		return kXYRegionRight;
	
	return kXYRegionOut;
}

RECT	CShareXPlotImpl::GetLayerAxisRect( int idxOfLayer, int location, HDC hDC, RECT plotRect )
{
	CAxisImpl *axis;
	SIZE axisSize,paddingSize;
	RECT axisRect={0,0,0,0};
	RECT newRect=plotRect;
	
	if(!m_vpLayers[idxOfLayer]->NeedDrawing())return axisRect;
	if(location==kLocationBottom || location==kLocationTop)return axisRect;
	
	for(int i=0;i<idxOfLayer;i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		
		axis=m_vpLayers[i]->GetAxisByLocation(kLocationLeft);
		{
			if(axis)
			{
				axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationLeft );
				newRect.left-=axisSize.cx;
			}
		}
		axis=m_vpLayers[i]->GetAxisByLocation(kLocationRight);
		{
			if(axis)
			{
				axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationRight );
				newRect.right+=axisSize.cx;
			}
		}
	}
	
	axis=m_vpLayers[idxOfLayer]->GetAxisByLocation(location);
	{
		if(axis)
		{
			axisSize = axis->GetMinDisplaySize( hDC );
			paddingSize = axis->GetPaddingSize( hDC );
			
			axisRect=newRect;
			if(location==kLocationLeft)
			{
				axisRect.right=newRect.left;
				axisRect.left=axisRect.right-axisSize.cx;
			}
			else
			{
				axisRect.left=newRect.right;
				axisRect.right=axisRect.left+axisSize.cx;
			}
			axisRect.bottom+=paddingSize.cy;
			axisRect.top-=paddingSize.cx;
		}
	}
	return axisRect;
}

int		CShareXPlotImpl::GetIdxOfLayerByPointInAxisRegion(HDC hDC, RECT plotRect, POINT point)
{
	int i,region=RegionIdentify(hDC,point);
	RECT axisRect;
	if( region==kXYRegionLeft )
	{
		for(i=0;i<GetLayerCount();i++)
		{
			axisRect=GetLayerAxisRect(i,kLocationLeft,hDC,plotRect);
			if(IsRectEmpty(&axisRect))continue;
			if( PtInRect(&axisRect, point) )
			{
				return i;
			}
		}
	}
	else if( region==kXYRegionRight )
	{
		for(i=0;i<GetLayerCount();i++)
		{
			axisRect=GetLayerAxisRect(i,kLocationRight,hDC,plotRect);
			if(IsRectEmpty(&axisRect))continue;
			if( PtInRect(&axisRect, point) )
			{
				return i;
			}
		}
	}
	return -1;
}

void	CShareXPlotImpl::SetShareXRange(double x1, double x2)
{
	double range[2]={x1, x2};
	int i;
	for(i=0; i<(int)m_vpLayers.size(); i++)
	{
		m_vpLayers[i]->SetXRange(x1, x2);
		m_vpLayers[i]->SetLastXPlotRange(range);
	}
	
	SetXRange( x1, x2 );
	SetLastXPlotRange(range);
}

CShareXPlotImplI::CShareXPlotImplI()
{

}

CShareXPlotImplI::~CShareXPlotImplI()
{
	
}

tstring	CShareXPlotImplI::GetDescriptionByMousePos(HDC hDC, POINT point)
{
	tostringstream ostr;
	tstring str=_TEXT("");
	if(GetTop()>=0 && GetTop()<GetSubPlotCount())
	{
		str = GetLayer(GetTop())->GetDescriptionByMousePos(hDC, point);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			ostr<<_T("Layer ")<<GetTop()+1<<_T(":\r\n")<<str.c_str();
			break;
		case kLangChinese:
			ostr<<_T("第")<<GetTop()+1<<_T("层:\r\n")<<str.c_str();
			break;
		}
		str = ostr.str();
		return str;
	}
	return str;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//

CLRPlotImpl::CLRPlotImpl()
{
	m_bExactXRange = false;

	ResizePlots(2);
	SetLRAxis();
}

CLRPlotImpl::~CLRPlotImpl()
{

}

void	CLRPlotImpl::GetPlotRange( double xRange[2], double yRange[2] )
{
	CShareXPlotImpl::GetPlotRange(xRange, yRange);

	if(!m_bExactXRange)return;
	
	double xrng1[2], yrng1[2];
	double xrng2[2], yrng2[2];
	
	GetLayer(0)->GetDataRanges(xrng1, yrng1);
	GetLayer(1)->GetDataRanges(xrng2, yrng2);
	
	xRange[0] = min(xrng1[0], xrng2[0]);
	xRange[1] = min(xrng1[1], xrng2[1]);
	
	SetBottomRange(xRange[0], xRange[1]);
}

int		CLRPlotImpl::AddLeftCurve( double *pX, double *pY, int nLen, tstring datatitle )
{
	int dataID = CShareXPlotImpl::AddCurve(pX, pY,nLen, 0);
	GetLayer(0)->SetDataTitle(dataID, datatitle);
	return dataID;
}

int		CLRPlotImpl::AddRightCurve( double *pX, double *pY, int nLen, tstring datatitle )
{
	int dataID = CShareXPlotImpl::AddCurve(pX, pY,nLen, 1);
	GetLayer(1)->SetDataTitle(dataID, datatitle);
	return dataID;
}

int		CLRPlotImpl::AddLeftCurve( double *pX, double *pY, int nLen )
{
	int dataID = CShareXPlotImpl::AddCurve(pX, pY,nLen, 0);
	return dataID;
}

int		CLRPlotImpl::AddRightCurve( double *pX, double *pY, int nLen )
{
	int dataID = CShareXPlotImpl::AddCurve(pX, pY,nLen, 1);
	return dataID;
}

int		CLRPlotImpl::AddLeftPoint( double x, double y, int nWhich )
{
	return AddPoint(x, y, nWhich, 0);
}

int		CLRPlotImpl::AddLeftPoint( double x, double y)
{
	return AddLeftPoint(x, y, 0);
}

int		CLRPlotImpl::AddRightPoint( double x, double y, int nWhich )
{
	return AddPoint(x, y, nWhich, 1);
}

int		CLRPlotImpl::AddRightPoint( double x, double y)
{
	return AddRightPoint(x, y, 0);
}

void	CLRPlotImpl::SetBottomTitle(tstring title)
{
	CAxisImpl *axis;
	axis = CPlotImpl::AddAxis(kLocationBottom);
	axis->SetTitle(title);
}

void	CLRPlotImpl::SetLeftTitle(tstring title)
{
	CAxisImpl *axis;
	axis = GetLayer(0)->AddAxis(kLocationLeft);
	axis->SetTitle(title);
}

void	CLRPlotImpl::SetRightTitle(tstring title)
{
	CAxisImpl *axis;
	axis = GetLayer(1)->AddAxis(kLocationRight);
	axis->SetTitle(title);
}

void	CLRPlotImpl::SetLeftRange(double low, double high)
{
	CAxisImpl *axis = GetLayer(0)->AddAxis(kLocationLeft);
	CPlotImpl::SetYRange(low, high);
}

void	CLRPlotImpl::SetRightRange(double low, double high)
{
	CAxisImpl *axis = GetLayer(1)->AddAxis(kLocationRight);
	CPlotImpl::SetYRange(low, high);
}

void	CLRPlotImpl::SetBottomRange(double low, double high)
{	
	GetLayer(0)->SetXRange(low, high);
	GetLayer(1)->SetXRange(low, high);
	
	SetXRange(low, high);
}
