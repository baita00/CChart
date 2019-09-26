/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2011 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/

/* ############################################################################################################################## */

//#include "StdAfx.h"
#include "LayeredPlotImpl.h"
#include "PlotLayerImpl.h"
#include "../../Basic/PlotUtility.h"
#include "../../Basic/MyMemDC.h"

using namespace NsCChart;

///////////////////////////////////////////////////////////////////////////////////////////////////
//

CLayeredPlotImpl::CLayeredPlotImpl()
{
	m_nTopLayer = -1;
}

CLayeredPlotImpl::~CLayeredPlotImpl()
{
	ClearLayers();
}

int		CLayeredPlotImpl::AddLayer()
{
	CPlotLayerImplI *tmp = new CPlotLayerImplI(this);
	int num = GetLayerCount();
	tmp->SetAutoColorStartIndex(12*num%CPlot::m_nColorsInTable);
	m_vpLayers.push_back(tmp);	
	return (int)m_vpLayers.size();
}

int		CLayeredPlotImpl::SetLayerCount( int num )
{
	ClearLayers();
	if(num > 0)
	{
		m_vpLayers.resize(num);
		for(int i=0; i<(int)m_vpLayers.size(); i++)
		{
			m_vpLayers[i] = new CPlotLayerImplI(this);
		}
	}
	return (int)m_vpLayers.size();
}

void	CLayeredPlotImpl::DrawBasic( HDC hDC, RECT destRect )
{
	int i;
	
	// Get number of left axis and right axis
	CAxisImpl *axis=NULL;
	
	// get plot rect
	RECT plotRect=GetPlotRect(hDC,destRect);
	SetLastClientRect(destRect);
	SetLastPlotRect(plotRect);
	
	//;
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
	// then left and right axis;
	SIZE axisSize;
	RECT newRect=plotRect;
	RECT drawRect;
	
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		//		if(!m_vpLayers[i]->NeedDrawing())continue;
		
		axis=m_vpLayers[i]->GetAxisByLocation(kLocationLeft);
		{
			if(axis)
			{
				if(axis->IsVisible())
				{
					drawRect.bottom = plotRect.bottom;
					drawRect.top = plotRect.top;
					drawRect.left = newRect.left;
					drawRect.right = newRect.right;

					m_vpLayers[i]->SetLastClientRect(destRect);
					m_vpLayers[i]->SetLastPlotRect(drawRect);
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
					drawRect.bottom = plotRect.bottom;
					drawRect.top = plotRect.top;
					drawRect.left = newRect.left;
					drawRect.right = newRect.right;

					m_vpLayers[i]->SetLastClientRect(destRect);
					m_vpLayers[i]->SetLastPlotRect(drawRect);
					axis->OnDraw(hDC);
					
					axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationRight );
					newRect.right+=axisSize.cx;
				}
			}
		}

		axis=m_vpLayers[i]->GetAxisByLocation(kLocationBottom);
		{
			if(axis)
			{
				if(axis->IsVisible())
				{
					drawRect.left = plotRect.left;
					drawRect.right = plotRect.right;
					drawRect.bottom = newRect.bottom;
					drawRect.top = newRect.top;

					m_vpLayers[i]->SetLastClientRect(destRect);
					m_vpLayers[i]->SetLastPlotRect(drawRect);
					axis->OnDraw(hDC);
					
					axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationBottom );
					newRect.bottom+=axisSize.cy;
				}
			}
		}

		axis=m_vpLayers[i]->GetAxisByLocation(kLocationTop);
		{
			if(axis)
			{
				if(axis->IsVisible())
				{
					drawRect.left = plotRect.left;
					drawRect.right = plotRect.right;
					drawRect.bottom = newRect.bottom;
					drawRect.top = newRect.top;
					
					m_vpLayers[i]->SetLastClientRect(destRect);
					m_vpLayers[i]->SetLastPlotRect(drawRect);
					axis->OnDraw(hDC);
					
					axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationTop );
					newRect.top-=axisSize.cy;
				}
			}
		}

		m_vpLayers[i]->SetLastPlotRect(plotRect);
	}
}

SIZE	CLayeredPlotImpl::GetAxisSize( HDC hDC, int location )
{
	SIZE axisSize={0,0};
	SIZE dispSize,paddingSize,maxPaddingSize;
	int i;
	CAxisImpl *axis;
	switch(location)
	{
	case kLocationBottom:
	case kLocationTop:
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
				axisSize.cy += dispSize.cy;
				if( dispSize.cx > axisSize.cx ) axisSize.cx = dispSize.cx;
				if( paddingSize.cx > maxPaddingSize.cx ) maxPaddingSize.cx = paddingSize.cx;
				if( paddingSize.cy > maxPaddingSize.cy ) maxPaddingSize.cy = paddingSize.cy;
				
				axisSize.cy += axis->GetAxisGap();
			}
		}
		axisSize.cx+=(maxPaddingSize.cx+maxPaddingSize.cy);
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

SIZE	CLayeredPlotImpl::GetAxisPaddingSize( HDC hDC, int location )
{
	SIZE paddingSize,maxPaddingSize={0,0};
	int i;
	CAxisImpl *axis;
	switch(location)
	{
	case kLocationBottom:
	case kLocationTop:
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

void	CLayeredPlotImpl::GetPlotRange( double xRange[2], double yRange[2] )
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
		
	GetSubPlotRanges();
	GetDataRange1D(0, xRange);
	GetDataRange1D(1, yRange);
}

RECT	CLayeredPlotImpl::GetPlotRect( HDC hDC, RECT clientRect)
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
	sizeRect.cx = sizeRect.cy = 0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		axis = m_vpLayers[i]->GetAxisByLocation(kLocationTop);
		if(axis)
		{
			if(axis->IsVisible())
			{
				sizeTmp=m_vpLayers[i]->GetAxisSize( hDC, kLocationTop );
				sizeRect.cy+=sizeTmp.cy;
			}
		}
		
	}
	plotRect.top+=sizeRect.cy;
	
	// then bottom axis
	sizeRect.cx = sizeRect.cy = 0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		axis = m_vpLayers[i]->GetAxisByLocation(kLocationBottom);
		if(axis)
		{
			if(axis->IsVisible())
			{
				sizeTmp=m_vpLayers[i]->GetAxisSize( hDC, kLocationBottom );
				sizeRect.cy+=sizeTmp.cy;
			}
		}
		
	}
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
	SIZE paddingSize1, paddingSize2;
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
		paddingSize1 = GetAxisPaddingSize( hDC, kLocationBottom);
		paddingSize2 = GetAxisPaddingSize( hDC, kLocationTop);
		plotRect.right -= max(paddingSize1.cy, paddingSize2.cy);
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
		paddingSize1 = GetAxisPaddingSize( hDC, kLocationBottom);
		paddingSize2 = GetAxisPaddingSize( hDC, kLocationTop);
		plotRect.left += max(paddingSize1.cx, paddingSize2.cx);
	}
	// the top padding size
	exist = false;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		if( m_vpLayers[i]->GetAxisByLocation(kLocationTop) )
		{
			exist = true;
			break;
		}
	}
	if(!exist)
	{
		paddingSize1 = GetAxisPaddingSize( hDC, kLocationLeft);
		paddingSize2 = GetAxisPaddingSize( hDC, kLocationRight);
		plotRect.top += max(paddingSize1.cy, paddingSize2.cy);
	}
	// the bottom padding size
	exist = false;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		if( m_vpLayers[i]->GetAxisByLocation(kLocationBottom) )
		{
			exist = true;
			break;
		}
	}
	if(!exist)
	{
		paddingSize1 = GetAxisPaddingSize( hDC, kLocationLeft);
		paddingSize2 = GetAxisPaddingSize( hDC, kLocationRight);
		plotRect.bottom -= max(paddingSize1.cx, paddingSize2.cx);
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

RECT	CLayeredPlotImpl::GetLayerAxisRect( int idxOfLayer, int location, HDC hDC, RECT plotRect )
{
	CAxisImpl *axis;
	SIZE axisSize,paddingSize;
	RECT axisRect={0,0,0,0};
	RECT newRect=plotRect;
	
	if(!m_vpLayers[idxOfLayer]->NeedDrawing())return axisRect;
	//if(location==kLocationBottom || location==kLocationTop)return axisRect;
	
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
		axis=m_vpLayers[i]->GetAxisByLocation(kLocationTop);
		{
			if(axis)
			{
				axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationTop );
				newRect.top-=axisSize.cy;
			}
		}
		axis=m_vpLayers[i]->GetAxisByLocation(kLocationBottom);
		{
			if(axis)
			{
				axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationBottom );
				newRect.bottom+=axisSize.cy;
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
			switch(location)
			{
			case kLocationLeft:
			case kLocationRight:
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
				break;
			case kLocationTop:
			case kLocationBottom:
				if(location==kLocationTop)
				{
					axisRect.bottom=newRect.top;
					axisRect.top=axisRect.bottom-axisSize.cy;
				}
				else
				{
					axisRect.top=newRect.bottom;
					axisRect.bottom=axisRect.top+axisSize.cy;
				}
				axisRect.right+=paddingSize.cy;
				axisRect.left-=paddingSize.cx;
				break;
			}
			
		}
	}
	return axisRect;
}

int		CLayeredPlotImpl::GetIdxOfLayerByPointInAxisRegion(HDC hDC, RECT plotRect, POINT point)
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
	else if( region==kXYRegionTop )
	{
		for(i=0;i<GetLayerCount();i++)
		{
			axisRect=GetLayerAxisRect(i,kLocationTop,hDC,plotRect);
			if(IsRectEmpty(&axisRect))continue;
			if( PtInRect(&axisRect, point) )
			{
				return i;
			}
		}
	}
	else if( region==kXYRegionBottom )
	{
		for(i=0;i<GetLayerCount();i++)
		{
			axisRect=GetLayerAxisRect(i,kLocationBottom,hDC,plotRect);
			if(IsRectEmpty(&axisRect))continue;
			if( PtInRect(&axisRect, point) )
			{
				return i;
			}
		}
	}
	return -1;
}

int		CLayeredPlotImpl::RegionIdentify(HDC hDC,POINT point)
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

	//top and bottom axis region	
	axisSize = GetAxisSize(hDC,kLocationTop);
	padddingSize = GetAxisPaddingSize(hDC, kLocationTop);
	axisRect.bottom = theRect.top;
	axisRect.top = axisRect.bottom - axisSize.cy;
	axisRect.left = theRect.left - padddingSize.cx;
	axisRect.right = theRect.right + padddingSize.cy;
	if( PtInRect(&axisRect, point) )
		return kXYRegionTop;
	
	axisSize = GetAxisSize(hDC,kLocationBottom);
	padddingSize = GetAxisPaddingSize(hDC, kLocationBottom);
	axisRect.top = theRect.bottom;
	axisRect.bottom = axisRect.top + axisSize.cy;
	axisRect.left = theRect.left - padddingSize.cx;
	axisRect.right = theRect.right + padddingSize.cy;
	if( PtInRect(&axisRect, point) )
		return kXYRegionBottom;
	
	return kXYRegionOut;
}

CLayeredPlotImplI::CLayeredPlotImplI()
{

}

CLayeredPlotImplI::~CLayeredPlotImplI()
{
	
}

tstring	CLayeredPlotImplI::GetDescriptionByMousePos(HDC hDC, POINT point)
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
			ostr<<_T("µÚ")<<GetTop()+1<<_T("²ã:\r\n")<<str.c_str();
			break;
		}
		str = ostr.str();
		return str;
	}
	return str;
}
