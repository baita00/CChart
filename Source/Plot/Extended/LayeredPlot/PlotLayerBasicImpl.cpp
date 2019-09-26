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
#include "PlotLayerBasicImpl.h"

using namespace NsCChart;

CPlotLayerBasicImpl::CPlotLayerBasicImpl()
{
	DeleteAllAxes();
//	AddAxis(kLocationLeft);

	SetEraseBkgnd(false);
	//m_bRangeZoomMode = true;
	SetXAutoRange(false);
	SetYAutoRange(false);
}

CPlotLayerBasicImpl::~CPlotLayerBasicImpl()
{
	
}

SIZE	CPlotLayerBasicImpl::GetAxisSize( HDC hDC, int location )
{
	SIZE size={0,0};
	CAxisImpl		*axis=GetAxisByLocation(location);
	if(axis==NULL)return size;
	
	SIZE		axisDims, paddingDims;
	axisDims = axis->GetMinDisplaySize( hDC );
	paddingDims = axis->GetPaddingSize( hDC );
	size=axisDims;
	int			gap;
	gap = axis->GetAxisGap();
	
	switch(location)
	{
	case kLocationBottom:		
	case kLocationTop:
		size.cx+=(paddingDims.cx+paddingDims.cy);
		size.cy=axisDims.cy + gap;
		break;
	case kLocationLeft:
	case kLocationRight:
		size.cy+=(paddingDims.cx+paddingDims.cy);
		size.cx=axisDims.cx + gap;
		break;
	default: break;
	}
	
	return size;
}

void	CPlotLayerBasicImpl::FinalDraw( HDC hDC, RECT destRect )
{
	if(GetPlotDataCount()<=0)
	{
//		EraseBkgnd( hDC, destRect );
		return;
	}

	// Set line colors,line styles, line sizes
	SetPlotColors();
	SetPlotLineStyles();
	SetPlotLineSizes();
	
	double	xRange[] = {fMaxVal, -fMaxVal}, yRange[]={fMaxVal,-fMaxVal};
	GetPlotRange( xRange, yRange );
	SetLastPlotRange(xRange, yRange);
	
	DrawDataSet( hDC, xRange, yRange );
	
	SetLastClientRect(destRect);
//	m_LastPlotRect = destRect;
}

void	CPlotLayerBasicImpl::DataToLP( double *data, LPPOINT point )
{
	RECT		plotRect, tempRect;
	double		xRange[2], yRange[2];
	double		scaleX, scaleY;
	
	if( point == NULL ) return;
	
	tempRect = GetLastClientRect() ;
	//NormalizeRect(tempRect);
	if( IsRectEmpty(&tempRect) ) return;
	
	//plotRect = GetPlotRect( pDC, m_rctLastClientRect  );
	//plotRect = m_LastPlotRect;
	plotRect = GetLastPlotRect();
	
	GetRange( xRange, yRange );
	
	scaleX = (xRange[1] - xRange[0] ) / abs(plotRect.right - plotRect.left);
	scaleY = (yRange[1] - yRange[0] ) / abs(plotRect.bottom - plotRect.top);
	
	if( scaleX == 0. || scaleY == 0. ) return;
	
	point->x = (long)((data[0]-xRange[0]) / scaleX) + plotRect.left;
	point->y = -(long)((data[1]-yRange[0]) / scaleY) + plotRect.bottom;
}

void	CPlotLayerBasicImpl::LPToData( LPPOINT point, double *data )
{
	RECT		plotRect, tempRect;
	double		xRange[2], yRange[2];
	double		scaleX, scaleY;
	
	if( point == NULL ) return;
	
	tempRect = GetLastClientRect() ;
	//tempRect.NormalizeRect();
	if( IsRectEmpty( &tempRect) ) return;
	
	//plotRect = GetPlotRect( pDC, m_rctLastClientRect  );
	//plotRect = m_LastPlotRect;
	plotRect = GetLastPlotRect();
	
	GetRange( xRange, yRange );
	
	scaleX = (xRange[1] - xRange[0] ) / abs(plotRect.right - plotRect.left);
	scaleY = (yRange[1] - yRange[0] ) / abs(plotRect.bottom - plotRect.top);
	
	if( scaleX == 0. || scaleY == 0. ) return;
	
	data[0] = (point->x-plotRect.left) * scaleX + xRange[0];
	data[1] = -(point->y-plotRect.bottom) * scaleY + yRange[0];
}
