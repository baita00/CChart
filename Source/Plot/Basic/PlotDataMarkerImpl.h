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

#include "PlotDataMarker.h"

Declare_Namespace_CChart

template<class PlotDataT>
class CPlotDataMarkerImpl : public CPlotDataMarker
{
public:
	CPlotDataMarkerImpl();
	virtual ~CPlotDataMarkerImpl();
public:
	void	DrawMarkers(HDC hDC, RECT plotRect, double *xRange, double *yRange);
	void	DrawSolelyMarker( HDC hDC, RECT plotRect, double *xRange, double *yRange, int whichPoint );
	void	DrawSolelyMarker( HDC hDC, POINT point, bool bForce ){ DrawSingleMarker( hDC, point, 0 , bForce);}
	void	DrawSingleMarker( HDC hDC, POINT point, int whichPoint, bool bForce );
	void	DrawPointSelectionMode(HDC hDC, RECT plotRect, double *xRange, double *yRange);
};

template<class PlotDataT>
CPlotDataMarkerImpl<PlotDataT>::CPlotDataMarkerImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
CPlotDataMarkerImpl<PlotDataT>::~CPlotDataMarkerImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
void	CPlotDataMarkerImpl<PlotDataT>::DrawMarkers(HDC hDC, RECT plotRect, double *xRange, double *yRange)
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
			point.x = (long)(plotRect.left + xScale * (myX-xRange[0]) + 0.5);
		else
			point.x = (long)(plotRect.right - xScale * (myX-xRange[0]) + 0.5);
		if(!((CPlotImpl*)pT->GetPlot())->IsYOpposite())
			point.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]) + 0.5);
		else
			point.y = (long)(plotRect.top + yScale * (myY-yRange[0]) + 0.5);
		DrawSingleMarker( hDC, point, j, false);
	}

	if(m_bMarkFirstPoint)
	{
		int nDataCount = pT->GetDataCount();
		if(nDataCount>0)
		{
			DrawSolelyMarker(hDC, plotRect, xRange, yRange, 0);
		}
	}
	if(m_bMarkLastPoint)
	{
		int nDataCount = pT->GetDataCount();
		if(nDataCount>0)
		{
			DrawSolelyMarker(hDC, plotRect, xRange, yRange, nDataCount-1);
		}
	}
}

template<class PlotDataT>
void	CPlotDataMarkerImpl<PlotDataT>::DrawPointSelectionMode(HDC hDC, RECT plotRect, double *xRange, double *yRange)
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
	
	if(pT->GetDataSet()->IsPointSelectionMode())
	{
		if(pT->GetIdxPtSelected()>=0)
		{
			int makerSize = m_nMarkerSize;
			m_nMarkerSize += 2;
			DrawSolelyMarker(hDC, plotRect, xRange, yRange, pT->GetIdxPtSelected());
			m_nMarkerSize = makerSize;
		}
		if(pT->GetIdxPtLighted()>=0)
		{
			int makerSize = m_nMarkerSize;
			m_nMarkerSize += 1;
			DrawSolelyMarker(hDC, plotRect, xRange, yRange, pT->GetIdxPtLighted());
			m_nMarkerSize = makerSize;
		}
	}
}

template<class PlotDataT>
void	CPlotDataMarkerImpl<PlotDataT>::DrawSolelyMarker( HDC hDC, RECT plotRect, double *xRange, double *yRange, int whichPoint )
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
	if(!pT->IsVisible())return;
	if(whichPoint<0 || whichPoint>=pT->GetDataCount())return;

	double		myX, myY;
	bool		bXLog, bYLog;
	bXLog = ((CPlotImpl*)pT->GetPlot())->IsXLogarithm();
	bYLog = ((CPlotImpl*)pT->GetPlot())->IsYLogarithm();
	
	POINT point;
	
	//xScale = ((double)Width(plotRect))/(xRange[1] - xRange[0]);
	//yScale = ((double)Height(plotRect))/(yRange[1] - yRange[0]);
	double		xScale = pT->xScale;
	double		yScale = pT->yScale;
	
	myX = pT->GetData()[whichPoint].val[0];
	if(bXLog)myX = log10(myX);
	myY =pT->GetData()[whichPoint].val[1];
	if(bYLog)myY = log10(myY);
	
	if(!((CPlotImpl*)pT->GetPlot())->IsXOpposite())
		point.x = (long)(plotRect.left + xScale * (myX-xRange[0]) + 0.5);
	else
		point.x = (long)(plotRect.right - xScale * (myX-xRange[0]) + 0.5);
	if(!((CPlotImpl*)pT->GetPlot())->IsYOpposite())
		point.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]) + 0.5);
	else
		point.y = (long)(plotRect.top + yScale * (myY-yRange[0]) + 0.5);
	DrawSingleMarker( hDC, point, whichPoint, true);
}

extern void	MyDrawFill(HDC hDC, COLORREF color, int alpha);

// Draws a marker at the current point, if a marker should be drawn there
template<class PlotDataT>
void	CPlotDataMarkerImpl<PlotDataT>::DrawSingleMarker( HDC hDC, POINT point, int whichPoint, bool bForce )
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);

	HBRUSH		hBrush, hOldBrush;
	RECT		rect;
	HPEN		hPen, hOldPen;
	POINT		startPoint, startLocation;

	if( m_nMarkerType == kXYMarkerNone ) return;

	if(bForce) goto NeedDrawMarker;

	// Check to see if we want to draw this marker
	if( m_nMarkerFrequency > 0 )
	{
		// positive means every nth data point
		if( (whichPoint - m_nMarkerStartIndex) % m_nMarkerFrequency != 0 ) return;
	}
	else if( m_nMarkerFrequency < 0 )
	{
		// negative means n number of times
		int			n_elements = pT->GetDataCount();
		int			n_markers = abs(m_nMarkerFrequency);
		if(n_markers==1)
		{
			if(whichPoint!=m_nMarkerStartIndex)
				return;
		}
		else
		{
			// it is not easy to find an expression, so i use a loop
			for(int i=0;i<n_markers;i++)
			{
				if(whichPoint==m_nMarkerStartIndex+i*(n_elements-1)/(n_markers-1))
					goto NeedDrawMarker;
			}
			return;
		}
	}
	else
	{
		return; // == 0 means no marker
	}

NeedDrawMarker:

	if(m_hMarkerImage)
	{
		SIZE bitmapSize = GetBitmapSize(m_hMarkerImage);
		RECT bitmapRect;
		bitmapRect.left = point.x - bitmapSize.cx/2;
		bitmapRect.right = bitmapRect.left + bitmapSize.cx;
		bitmapRect.top = point.y - bitmapSize.cy/2;
		bitmapRect.bottom = bitmapRect.top + bitmapSize.cy;

		HRGN hImgRgn = 0;
		if(m_bMarkerImageRound)
		{
			hImgRgn = CreateEllipticRgn(bitmapRect.left, bitmapRect.top, bitmapRect.right, bitmapRect.bottom);
			SelectClipRgn(hDC, hImgRgn);
		}

		HDC hMemDC = CreateCompatibleDC(hDC);
		SelectObject(hMemDC, m_hMarkerImage);
		BitBlt(hDC, bitmapRect.left, bitmapRect.top, bitmapSize.cx, bitmapSize.cy, hMemDC, 0, 0, SRCCOPY);
		DeleteDC(hMemDC);

		if(m_bMarkerImageRound)
		{
			SelectClipRgn(hDC, 0);
			DeleteObject(hImgRgn);
		}
		
		return;
	}

	GetCurrentPositionEx(hDC,&startLocation);
	
/*	if(m_vpXYDataInfo[whichDataSet]->bColorPtByPt && m_vpXYDataInfo[whichDataSet]->vPointColors.size() == GetAbsoluteData(whichDataSet).size())
	{
		hPen=CreatePen( PS_SOLID, 1, m_vpXYDataInfo[whichDataSet]->vPointColors[whichPoint] );
	}
	else
	{
		hPen=CreatePen( PS_SOLID, 1, m_vpXYDataInfo[whichDataSet]->crPlotColor );
	}*/
	hPen=CreatePen( PS_SOLID, 1, pT->GetPlotColor() );
	
	hOldPen = (HPEN)SelectObject( hDC,hPen );

	SetRect( &rect, point.x - m_nMarkerSize, point.y -m_nMarkerSize,	point.x + m_nMarkerSize, point.y + m_nMarkerSize );

	COLORREF markerfillcolor;
/*	if(m_vpXYDataInfo[whichDataSet]->bColorPtByPt && m_vpXYDataInfo[whichDataSet]->vPointColors.size() == GetAbsoluteData(whichDataSet).size())
	{
		markerfillcolor = m_vpXYDataInfo[whichDataSet]->vPointColors[whichPoint];
	}
	else*/ if(m_bMarkerFillOtherColor)
	{
		markerfillcolor = m_crMarkerFillColor;
	}
	else
	{
		markerfillcolor = pT->GetPlotColor();
	}
	//hBrush=CreateSolidBrush( m_vpXYDataInfo[whichDataSet]->crPlotColor );
	hBrush=CreateSolidBrush( markerfillcolor );
	hOldBrush = (HBRUSH)SelectObject( hDC,hBrush );

	BeginPath(hDC);

	double s2=sqrt(2.0);
	double offset;
	switch( m_nMarkerType )
	{
	case kXYMarkerNone:
		return; break;
	case kXYMarkerCircle:
		// the size is the radius
		Arc( hDC, rect.left,rect.top,rect.right,rect.bottom, point.x, point.y, point.x, point.y );
		break;
	case kXYMarkerSquareUpright:
		//MyFrameRect( pDC, rect );
		MoveToEx(hDC,rect.left,rect.top,NULL);
		LineTo(hDC,rect.right,rect.top);
		LineTo(hDC,rect.right,rect.bottom);
		LineTo(hDC,rect.left,rect.bottom);
		LineTo(hDC,rect.left,rect.top);
		break;
	case kXYMarkerSquareOblique:
		offset=ceil(m_nMarkerSize*s2);
		startPoint.x = point.x - (long)offset;
		startPoint.y = point.y;
		MoveToEx( hDC, startPoint.x, startPoint.y, NULL );
		startPoint.x += (long)offset;
		startPoint.y += (long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		startPoint.x += (long)offset;
		startPoint.y -= (long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		startPoint.x -= (long)offset;
		startPoint.y -= (long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		startPoint.x -= (long)offset;
		startPoint.y += (long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		MoveToEx( hDC, point.x, point.y, NULL );
		break;
	case kXYMarkerTriangleLeft:
		// Need to update this to actually center correctly using
		// the triangle's geometry
		offset=m_nMarkerSize;
		startPoint.x = point.x;// - (long)offset;
		startPoint.y = point.y ;
		MoveToEx( hDC, startPoint.x, startPoint.y, NULL );
		//startPoint.Offset( 2 *offset,  offset );
		startPoint.x += 2*(long)offset;
		startPoint.y -= (long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset( 0, -2*offset );
		startPoint.y += 2*(long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset( -2 *offset,  offset );
		startPoint.x -= 2*(long)offset;
		startPoint.y -= (long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		MoveToEx( hDC, point.x, point.y, NULL );
		break;
	case kXYMarkerTriangleRight:
		// Need to update this to actually center correctly using
		// the triangle's geometry
		offset=m_nMarkerSize;
		startPoint.x = point.x;// + (long)offset;
		startPoint.y = point.y ;
		MoveToEx( hDC, startPoint.x, startPoint.y, NULL );
		//startPoint.Offset( -2 *offset,  offset );
		startPoint.x -= 2*(long)offset;
		startPoint.y -= (long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset( 0, -2*offset );
		startPoint.y += 2*(long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset( 2 *offset,  offset );
		startPoint.x += 2*(long)offset;
		startPoint.y -= (long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		MoveToEx( hDC, point.x, point.y, NULL );
		break;
	case kXYMarkerTriangleUp:
		// Need to update this to actually center correctly using
		// the triangle's geometry
		offset=m_nMarkerSize;
		startPoint.x = point.x;
		startPoint.y = point.y;// - (long)offset;
		MoveToEx( hDC, startPoint.x, startPoint.y, NULL );
		//startPoint.Offset( offset, -2 * offset );
		startPoint.x += (long)offset;
		startPoint.y += 2*(long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset( -2*offset, 0 );
		startPoint.x -= 2*(long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset( offset, 2 * offset );
		startPoint.x += (long)offset;
		startPoint.y -= 2*(long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		MoveToEx( hDC, point.x, point.y, NULL );
		break;
	case kXYMarkerTriangleDown:
		// Need to update this to actually center correctly using
		// the triangle's geometry
		offset=m_nMarkerSize;
		startPoint.x = point.x;
		startPoint.y = point.y;// + (long)offset;
		MoveToEx( hDC, startPoint.x, startPoint.y, NULL );
		//startPoint.Offset( offset, 2 * offset );
		startPoint.x += (long)offset;
		startPoint.y -= 2*(long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset( -2*offset, 0 );
		startPoint.x -= 2*(long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset( offset, -2 * offset );
		startPoint.x += (long)offset;
		startPoint.y += 2*(long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		MoveToEx( hDC, point.x, point.y, NULL );
		break;
	case kXYMarkerX:
		offset=m_nMarkerSize;
		startPoint.x = point.x - (long)offset;
		startPoint.y = point.y + (long)offset;
		MoveToEx( hDC, startPoint.x, startPoint.y, NULL );
		//startPoint.Offset( 2*offset, 2*offset );
		startPoint.x += 2*(long)offset;
		startPoint.y -= 2*(long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset( -2*offset, 0 );
		startPoint.x -= 2*(long)offset;
		MoveToEx( hDC, startPoint.x, startPoint.y, NULL );
		//startPoint.Offset( 2*offset, -2*offset );
		startPoint.x += 2*(long)offset;
		startPoint.y += 2*(long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		// Move back to beginning
		MoveToEx( hDC, point.x, point.y, NULL );
		break;
	case kXYMarkerCross:
		offset=m_nMarkerSize;
		startPoint.x = point.x - (long)offset;
		startPoint.y = point.y;
		MoveToEx( hDC, startPoint.x, startPoint.y, NULL );
		//startPoint.Offset( 2*offset, 2*offset );
		startPoint.x += 2*(long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset( -2*offset, 0 );
		startPoint.x = point.x;
		startPoint.y = point.y - (long)offset;
		MoveToEx( hDC, startPoint.x, startPoint.y, NULL );
		//startPoint.Offset( 2*offset, -2*offset );
		startPoint.y += 2*(long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		// Move back to beginning
		MoveToEx( hDC, point.x, point.y, NULL );
		break;
	case kXYMarkerBar:
		int dx, dy, de;
		if(m_bMarkerBarVertical)
		{
			dx = m_nMarkerBarHWidth*m_nMarkerSize;
			dy = m_nMarkerBarHHeight*m_nMarkerSize;
			de = m_nMarkerBarExtLen*m_nMarkerSize;
			MoveToEx(hDC, point.x - dx, point.y - dy - de, NULL);
			LineTo(hDC, point.x + dx, point.y - dy - de);
			MoveToEx(hDC, point.x, point.y - dy - de, NULL);
			LineTo(hDC, point.x, point.y -dy);
			MoveToEx(hDC, point.x - dx, point.y + dy + de, NULL);
			LineTo(hDC, point.x + dx, point.y + dy + de);
			MoveToEx(hDC, point.x, point.y + dy + de, NULL);
			LineTo(hDC, point.x, point.y + dy);
			
			MoveToEx(hDC, point.x - dx, point.y - dy, NULL);
			LineTo(hDC, point.x + dx, point.y - dy);
			LineTo(hDC, point.x + dx, point.y + dy);
			LineTo(hDC, point.x - dx, point.y + dy);
			LineTo(hDC, point.x - dx, point.y - dy);
		}
		else
		{
			dx = m_nMarkerBarHHeight*m_nMarkerSize;
			dy = m_nMarkerBarHWidth*m_nMarkerSize;
			de = m_nMarkerBarExtLen*m_nMarkerSize;
			MoveToEx(hDC, point.x - dx - de, point.y - dy, NULL);
			LineTo(hDC, point.x - dx - de , point.y + dy);
			MoveToEx(hDC, point.x - dx - de, point.y, NULL);
			LineTo(hDC, point.x - dx, point.y);
			MoveToEx(hDC, point.x + dx + de, point.y - dy, NULL);
			LineTo(hDC, point.x + dx + de, point.y + dy);
			MoveToEx(hDC, point.x + dx + de, point.y, NULL);
			LineTo(hDC, point.x + dx, point.y);
			
			MoveToEx(hDC, point.x - dx, point.y - dy, NULL);
			LineTo(hDC, point.x + dx, point.y - dy);
			LineTo(hDC, point.x + dx, point.y + dy);
			LineTo(hDC, point.x - dx, point.y + dy);
			LineTo(hDC, point.x - dx, point.y - dy);
		}
		break;
	case kXYMarkerDot:
		offset=m_nMarkerSize/5;
		startPoint.x = point.x - (long)offset;
		startPoint.y = point.y;
		MoveToEx( hDC, startPoint.x, startPoint.y, NULL );
		//startPoint.Offset(offset,offset);
		startPoint.x += (long)offset;
		startPoint.y -= (long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset(offset,-offset);
		startPoint.x += (long)offset;
		startPoint.y += (long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset(-offset,-offset);
		startPoint.x -= (long)offset;
		startPoint.y += (long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		//startPoint.Offset(-offset,+offset);
		startPoint.x -= (long)offset;
		startPoint.y -= (long)offset;
		LineTo( hDC, startPoint.x, startPoint.y );
		// Move back to beginning
		MoveToEx( hDC, point.x, point.y, NULL );
		break;
	}

	EndPath(hDC);

	if( m_bMarkerFill )
	{
		if(m_nMarkerFillAlpha == 255)
		{
			// GDI+ not needed
			StrokeAndFillPath(hDC);
		}
		else
		{
			MyDrawFill(hDC, pT->GetPlotColor(), m_nMarkerFillAlpha);
			// use GDI+ to get alpha blend effect
/*			HRGN hRgn = PathToRegion(hDC);
			Region rgn(hRgn);

			byte a, r, g, b;
			a = m_nMarkerFillAlpha;
			r = GetRValue(pT->GetPlotColor());
			g = GetGValue(pT->GetPlotColor());
			b = GetBValue(pT->GetPlotColor());
			Color fillColor(a, r, g, b);
			SolidBrush brush(fillColor);

			Graphics graph(hDC);
			graph.FillRegion(&brush, &rgn);

			DeleteObject(hRgn);*/
		}
	}
	else
	{
		StrokePath(hDC);
	}

	MoveToEx( hDC, startLocation.x, startLocation.y, NULL );
	SelectObject( hDC,hOldPen );
	DeleteObject(hPen);
	SelectObject( hDC,hOldBrush );
	DeleteObject(hBrush);
}

Declare_Namespace_End