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

#include "OscilloScope.h"

Declare_Namespace_CChart

template<class PlotImplT>
class COscilloScopeImpl : public COscilloScope
{
public:
	COscilloScopeImpl();
	virtual ~COscilloScopeImpl();

public:
	void				DrawCursor( HDC hDC, RECT plotRect );
	void				DrawSelZone( HDC hDC, RECT plotRect );
	
	void				DrawTrigArrow( HDC hDC, RECT plotRect );
	void				DrawDataSetZeroArrow( HDC hDC, RECT plotRect );
	void				DrawSingleDataSetZeroArrow( HDC hDC, RECT plotRect, int which );
		
	void				InitializeSelZone();
	void				InitializeCursor();
	
	bool				HitZoneEdge(HDC hDC, POINT point, int &index);
	
	void				SteppingPlotData(int dataID, bool bX, bool bInc);
	void				SteppingCursor(bool bX, bool bInc);
	void				SteppingZoneEdge(int nIndex, bool bInc);

public:
	void				DrawOscilloScope(HDC hDC);
};

template<class PlotImplT>
COscilloScopeImpl<PlotImplT>::COscilloScopeImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
COscilloScopeImpl<PlotImplT>::~COscilloScopeImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
void	COscilloScopeImpl<PlotImplT>::DrawCursor( HDC hDC, RECT plotRect )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(!m_bShowHCursor && !m_bShowVCursor)return;

	double xRange[2],yRange[2];
	pT->GetLastPlotRange(xRange,yRange);

	if(m_fCursorX<xRange[0] || m_fCursorX>xRange[1] || m_fCursorY<yRange[0] || m_fCursorY>yRange[1])return;
	
	HPEN		hPen, hOldPen;
	hPen=CreatePen( m_nCursorStyle, m_nCursorSize, m_crCursorColor );
	hOldPen = (HPEN)SelectObject( hDC, hPen );

	double data[2];
	POINT point;
	data[0]=m_fCursorX;
	data[1]=m_fCursorY;

	pT->DataToLP(data,&point);
	
	// Bound to a curve
	if(m_bHoldCursorToCurve && m_nIndexToHoldCursor>=0 
		&& m_nIndexToHoldCursor < pT->GetPlotDataCount() )
	{
		double dt[2];
		if(pT->GetNeareastDataPoint(pT->GetDataID(m_nIndexToHoldCursor), hDC, point, dt)>=0)
		{
			data[1]=dt[1];
			pT->DataToLP(data,&point);
		}
	}	

	if(m_bShowHCursor)
	{
		MoveToEx(hDC,plotRect.left,point.y,NULL);
		LineTo(hDC,plotRect.right,point.y);
	}
	if(m_bShowVCursor)
	{
		MoveToEx(hDC, point.x,plotRect.bottom,NULL);
		LineTo(hDC, point.x,plotRect.top);
	}
	if(m_bShowHCursor || m_bShowVCursor)
	{
		Rectangle(hDC, point.x - 5, point.y - 5, point.x + 5, point.y + 5);
	}

	SelectObject( hDC, hOldPen );
	DeleteObject(hPen);
}

template<class PlotImplT>
void	COscilloScopeImpl<PlotImplT>::DrawSelZone( HDC hDC, RECT plotRect )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	bool xDraw=true,yDraw=true;
	if( !m_bEnableXZone )xDraw=false;
//	if(m_fXZoneLower>m_fXZoneUpper)xDraw=false;
	if( !m_bEnableYZone )yDraw=false;
//	if(m_fYZoneLower>m_fYZoneUpper)yDraw=false;

	if(m_fXZoneUpper<m_fXZoneLower)swap(m_fXZoneLower, m_fXZoneUpper);
	if(m_fYZoneUpper<m_fYZoneLower)swap(m_fYZoneLower, m_fYZoneUpper);

	double xRange[2],yRange[2];
	pT->GetLastPlotRange(xRange, yRange);
	if(m_fXZoneLower>xRange[1] || m_fXZoneUpper<xRange[0])xDraw=false;
	if(m_fXZoneLower<xRange[0] && m_fXZoneUpper>xRange[1])xDraw=false;
	if(m_fYZoneLower>yRange[1] || m_fYZoneUpper<yRange[0])yDraw=false;
	if(m_fYZoneLower<yRange[0] && m_fYZoneUpper>yRange[1])yDraw=false;

	if(!xDraw && !yDraw)return;

	HPEN		hPen1, hOldPen;
	hPen1=CreatePen( PS_SOLID, m_nZoneEdgeWidth, pT->GetLightColor() );
	
	HBRUSH		hBrush, hOldBrush;
	hBrush=CreateSolidBrush( m_crZoneColor );
	hOldBrush = (HBRUSH)SelectObject( hDC, hBrush );

	int xlow,xupp,ylow,yupp;
	double data[2];
	POINT point;
	if(xDraw)
	{
		data[1]=0.0;
		data[0]=m_fXZoneLower;	
		pT->DataToLP(data,&point);
		xlow=point.x;
		data[0]=m_fXZoneUpper;
		pT->DataToLP(data,&point);
		xupp=point.x;

		if(m_fXZoneLower>xRange[0] && m_fXZoneUpper<xRange[1])
		{
			RECT lightRect = plotRect;
			lightRect.left = xlow;
			lightRect.right = xupp;
			//Rectangle(hDC,lightRect.left,lightRect.top,lightRect.right,lightRect.bottom);
			Graphics graph(hDC);
			Color fillcolor(m_nZoneAlpha, GetRValue(m_crZoneColor), GetGValue(m_crZoneColor), GetBValue(m_crZoneColor));
			SolidBrush brush(fillcolor);
			graph.FillRectangle(&brush, lightRect.left, lightRect.top, Width(lightRect), Height(lightRect));
		}
	}
	if(yDraw)
	{
		data[0]=0.0;
		data[1]=m_fYZoneLower;
		pT->DataToLP(data,&point);
		ylow=point.y;
		data[1]=m_fYZoneUpper;
		pT->DataToLP(data,&point);
		yupp=point.y;

		if(m_fYZoneLower>yRange[0] && m_fYZoneUpper<yRange[1])
		{
			RECT lightRect = plotRect;
			lightRect.top = yupp;
			lightRect.bottom = ylow;
			//Rectangle(hDC,lightRect.left,lightRect.top,lightRect.right,lightRect.bottom);
			Graphics graph(hDC);
			Color fillcolor(m_nZoneAlpha, GetRValue(m_crZoneColor), GetGValue(m_crZoneColor), GetBValue(m_crZoneColor));
			SolidBrush brush(fillcolor);
			graph.FillRectangle(&brush, lightRect.left, lightRect.top, Width(lightRect), Height(lightRect));
		}
	}

	hOldPen = (HPEN)SelectObject( hDC, hPen1 );
	if(xDraw)
	{
		if(m_fXZoneLower>xRange[0] && m_fXZoneLower<xRange[1])
		{
			MoveToEx(hDC,xlow,plotRect.top,NULL);
			LineTo(hDC,xlow,plotRect.bottom);
		}
		if(m_fXZoneUpper>xRange[0] && m_fXZoneUpper<xRange[1])
		{
			MoveToEx(hDC,xupp,plotRect.top,NULL);
			LineTo(hDC,xupp,plotRect.bottom);
		}
	}
	if(yDraw)
	{
		if(m_fYZoneLower>yRange[0] && m_fYZoneLower<yRange[1])
		{			
			MoveToEx(hDC,plotRect.left,ylow,NULL);
			LineTo(hDC,plotRect.right,ylow);
		}
		if(m_fYZoneUpper>yRange[0] && m_fYZoneUpper<yRange[1])
		{
			MoveToEx(hDC,plotRect.left,yupp,NULL);
			LineTo(hDC,plotRect.right,yupp);
		}
	}

	SelectObject( hDC,hOldBrush );
	DeleteObject(hBrush);
	SelectObject( hDC,hOldPen );
	DeleteObject( hPen1 );
}

template<class PlotImplT>
void	COscilloScopeImpl<PlotImplT>::DrawTrigArrow( HDC hDC, RECT plotRect )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(!m_bShowTrigArrow)return;

	double xRange[2], yRange[2];
	pT->GetLastPlotRange(xRange, yRange);
	if(m_fTrigLevel >= yRange[1] || m_fTrigLevel <= yRange[0])
		return;

	double data[2];
	data[0] = xRange[1];
	data[1] = m_fTrigLevel;

	POINT	point;
	pT->DataToLP(data, &point);

	// Draw the Arrow
	HBRUSH hBrush = CreateSolidBrush(m_crTrigArrowColor);
	HRGN hRgn;
	POINT pPt[7];

	point.x -= 23;
	pPt[0].x = point.x;
	pPt[0].y = point.y;
	pPt[1].x = point.x + 15;
	pPt[1].y = point.y + 5;
	pPt[2].x = point.x + 15;
	pPt[2].y = point.y + 1;
	pPt[3].x = point.x + 23;
	pPt[3].y = point.y + 1;
	pPt[4].x = point.x + 23;
	pPt[4].y = point.y - 1;
	pPt[5].x = point.x + 15;
	pPt[5].y = point.y - 1;
	pPt[6].x = point.x + 15;
	pPt[6].y = point.y - 5;

	hRgn = CreatePolygonRgn(pPt, 7, ALTERNATE);
	FillRgn(hDC, hRgn, hBrush);
	DeleteObject(hBrush);
	DeleteObject(hRgn);
}

template<class PlotImplT>
void	COscilloScopeImpl<PlotImplT>::DrawDataSetZeroArrow( HDC hDC, RECT plotRect )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(!m_bShowZeroArrow)return;
	for(int i=0; i<pT->GetPlotDataCount(); i++)
	{
		DrawSingleDataSetZeroArrow(hDC, plotRect, i);
	}
}

template<class PlotImplT>
void	COscilloScopeImpl<PlotImplT>::DrawSingleDataSetZeroArrow( HDC hDC, RECT plotRect, int which )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	double xRange[2], yRange[2];
	pT->GetLastPlotRange(xRange, yRange);
	
	double data[2];
	POINT point;

	POINT center;
	int	radius = 6;
	POINT pPt[7], pt;
	
	HPEN hPen, hOldPen;
	hPen = CreatePen(PS_SOLID, 1, pT->GetAbsPlotData(which)->GetPlotColor());
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	HBRUSH hBrush;
	hBrush = CreateSolidBrush(pT->GetAbsPlotData(which)->GetPlotColor());

	RECT rect;
	tostringstream ostr;
	tstring str;
	ostr.str(_TEXT(""));
	ostr<<which+1;
	str = ostr.str();

	double offsetx, offsety;
	offsetx = pT->GetAbsPlotData(which)->fOffsetX;
	offsety = pT->GetAbsPlotData(which)->fOffsetY;
	//Draw Left arrow
	if(yRange[0] < offsety && yRange[1] > offsety)
	{
		data[0] = xRange[0];
		data[1] = offsety;
		pT->DataToLP(data, &point);

		center.x = point.x + radius;
		center.y = point.y;

		Ellipse(hDC, center.x - radius, center.y - radius, center.x + radius, center.y + radius);

		rect.left = center.x - radius;
		rect.right = center.x + radius;
		rect.top = center.y - radius;
		rect.bottom = center.y + radius;
		DrawTextInRect(hDC, rect, str, &pT->GetTitleFont(), TRANSPARENT, pT->GetAbsPlotData(which)->GetDataColor());

		pt.x = point.x + 2*radius + 15;
		pt.y = point.y;

		pPt[0].x = pt.x;
		pPt[0].y = pt.y;
		pPt[1].x = pt.x - 12;
		pPt[1].y = pt.y - 5;
		pPt[2].x = pt.x - 12;
		pPt[2].y = pt.y - 1;
		pPt[3].x = pt.x - 15;
		pPt[3].y = pt.y - 1;
		pPt[4].x = pt.x - 15;
		pPt[4].y = pt.y + 1;
		pPt[5].x = pt.x - 12;
		pPt[5].y = pt.y + 1;
		pPt[6].x = pt.x - 12;
		pPt[6].y = pt.y + 5;

		HRGN hRgn = CreatePolygonRgn(pPt, 7, ALTERNATE);
		FillRgn(hDC, hRgn, hBrush);
		DeleteObject(hRgn);
	}

	//Draw bottom arrow
	if(xRange[0]< offsetx && xRange[1] > offsetx)
	{
		data[0] = offsetx;
		data[1] = yRange[0];
		pT->DataToLP(data, &point);

		center.x = point.x;
		center.y = point.y - radius;

		Ellipse(hDC, center.x - radius, center.y - radius, center.x + radius, center.y + radius);

		rect.left = center.x - radius;
		rect.right = center.x + radius;
		rect.top = center.y - radius;
		rect.bottom = center.y + radius;
		DrawTextInRect(hDC, rect, str, &pT->GetTitleFont(), TRANSPARENT, pT->GetAbsPlotData(which)->GetDataColor());

		pt.x = point.x;
		pt.y = point.y - 2*radius - 15;
		
		pPt[0].x = pt.x;
		pPt[0].y = pt.y;
		pPt[1].x = pt.x + 5;
		pPt[1].y = pt.y + 12;
		pPt[2].x = pt.x + 1;
		pPt[2].y = pt.y + 12;
		pPt[3].x = pt.x + 1;
		pPt[3].y = pt.y + 15;
		pPt[4].x = pt.x - 1;
		pPt[4].y = pt.y + 15;
		pPt[5].x = pt.x - 1;
		pPt[5].y = pt.y + 12;
		pPt[6].x = pt.x - 5;
		pPt[6].y = pt.y + 12;

		HRGN hRgn = CreatePolygonRgn(pPt, 7, ALTERNATE);
		FillRgn(hDC, hRgn, hBrush);
		DeleteObject(hRgn);
	}
	
	DeleteObject(hBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

template<class PlotImplT>
void	COscilloScopeImpl<PlotImplT>::InitializeSelZone()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	double xRange[2],yRange[2];
	pT->GetLastPlotRange(xRange, yRange);
	if(GetXZoneLower()<=xRange[0] || GetXZoneLower()>=xRange[1])
	{
		SetXZoneLower(xRange[0]*3.0/4.0+xRange[1]/4.0);
	}
	if(GetXZoneUpper()<=xRange[0] || GetXZoneUpper()>=xRange[1])
	{
		SetXZoneUpper(xRange[0]/2.0+xRange[1]/2.0);
	}
	if(GetYZoneLower()<=yRange[0] || GetYZoneLower()>=yRange[1])
	{
		SetYZoneLower(yRange[0]*3.0/4.0+yRange[1]/4.0);
	}
	if(GetYZoneUpper()<=yRange[0] || GetYZoneUpper()>=yRange[1])
	{
		SetYZoneUpper(yRange[0]/2.0+yRange[1]/2.0);
	}
}

template<class PlotImplT>
void	COscilloScopeImpl<PlotImplT>::InitializeCursor()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	double xRange[2],yRange[2];
	pT->GetLastPlotRange(xRange, yRange);
	if(GetCursorX()<=xRange[0] || GetCursorX()>=xRange[1])
	{
		SetCursorX((xRange[0]+xRange[1])/2.0);
	}
	if(GetCursorY()<=yRange[0] || GetCursorY()>=yRange[1])
	{
		SetCursorY((yRange[0]+yRange[1])/2.0);
	}
}

template<class PlotImplT>
bool	COscilloScopeImpl<PlotImplT>::HitZoneEdge(HDC hDC, POINT point, int &index)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	// index: 0-xl, 1-xh, 2-yl, 3-yh
	RECT plotRect = pT->GetLastPlotRect();
	if(!PtInRect(&plotRect, point))return false;

	long xL, xH, yL, yH;
	double data[2];
	POINT pt;

	data[0] = m_fXZoneLower;
	data[1] = m_fYZoneLower;
	pT->DataToLP(data, &pt);
	xL = pt.x;
	yL = pt.y;

	data[0] = m_fXZoneUpper;
	data[1] = m_fYZoneUpper;
	pT->DataToLP(data, &pt);
	xH = pt.x;
	yH = pt.y;

	if( abs(point.x - xL) <= 2 )
	{
		index = 0;
		return true;
	}
	else if( abs(point.x - xH) <= 2 )
	{
		index = 1;
		return true;
	}
	else if( abs(point.y - yL) <= 2 )
	{
		index = 2;
		return true;
	}
	else if( abs(point.y - yH) <= 2 )
	{
		index = 3;
		return true;
	}

	return false;
}

template<class PlotImplT>
void	COscilloScopeImpl<PlotImplT>::SteppingPlotData(int dataID, bool bX, bool bInc)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(dataID<0)return;
	int nIndex = pT->GetIndex(dataID);
	if(nIndex<0)return;

	
	double xRange[2], yRange[2];
	pT->GetLastPlotRange(xRange, yRange);
	double divider;
	if(m_bSteppingHighPrecision)
		divider = m_nSteppingPrecisionHigh;
	else
		divider = m_nSteppingPrecisionLow;
	
	double thesign;
	if(bInc)thesign = 1;
	else thesign = -1;
	
	double dlt;
	if(bX)
	{
		dlt = (xRange[1]-xRange[0])/divider;
		//SetDataSetOffset(dataID, GetDataSetOffsetX(dataID)+thesign*dlt, GetDataSetOffsetY(dataID));
		pT->GetPlotData(dataID)->fOffsetX += thesign*dlt;
	}
	else
	{
		dlt = (yRange[1]-yRange[0])/divider;
		//SetDataSetOffset(dataID, GetDataSetOffsetX(dataID), GetDataSetOffsetY(dataID)+thesign*dlt);
		pT->GetPlotData(dataID)->fOffsetY += thesign*dlt;
	}
}

template<class PlotImplT>
void	COscilloScopeImpl<PlotImplT>::SteppingCursor(bool bX, bool bInc)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	double xRange[2], yRange[2];
	pT->GetLastPlotRange(xRange, yRange);
	double divider;
	if(m_bSteppingHighPrecision)
		divider = m_nSteppingPrecisionHigh;
	else
		divider = m_nSteppingPrecisionLow;
	
	double thesign;
	if(bInc)thesign = 1;
	else thesign = -1;

	if(bX)
	{
		m_fCursorX += thesign*(xRange[1]-xRange[0])/divider;
	}
	else
	{
		m_fCursorY += thesign*(yRange[1]-yRange[0])/divider;
	}
}

template<class PlotImplT>
void	COscilloScopeImpl<PlotImplT>::SteppingZoneEdge(int nIndex, bool bInc)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(nIndex<0 || nIndex>=4)return;

	double xRange[2], yRange[2];
	pT->GetLastPlotRange(xRange, yRange);
	double divider;
	if(m_bSteppingHighPrecision)
		divider = m_nSteppingPrecisionHigh;
	else
		divider = m_nSteppingPrecisionLow;
	
	double thesign;
	if(bInc)thesign = 1;
	else thesign = -1;

	switch(nIndex)
	{
	case 0:
		m_fXZoneLower += thesign*(xRange[1]-xRange[0])/divider;
		break;
	case 1:
		m_fXZoneUpper += thesign*(xRange[1]-xRange[0])/divider;
		break;
	case 2:
		m_fYZoneLower += thesign*(yRange[1]-yRange[0])/divider;
		break;
	case 3:
		m_fYZoneUpper += thesign*(yRange[1]-yRange[0])/divider;
		break;
	}
}

template<class PlotImplT>
void	COscilloScopeImpl<PlotImplT>::DrawOscilloScope(HDC hDC)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	RECT plotRect = pT->GetLastPlotRect();
	if(!pT->IsOscilloScopeMode() || !IsStartOscilloScope())return;

	// now draw oscilloscopes
	// Draw Selelected zone
	InitializeSelZone();
	DrawSelZone( hDC, plotRect );
	// Draw the cross
	InitializeCursor();
	DrawCursor(hDC, plotRect);
	// Draw trig arrow
	DrawTrigArrow(hDC, plotRect);
	// Draw the dataset zero arrow
	DrawDataSetZeroArrow(hDC, plotRect);
}

Declare_Namespace_End

