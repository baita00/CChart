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

#include "LegendBasicImpl.h"

Declare_Namespace_CChart

template<class PlotImplT>
class CLegendImpl : public CLegendBasicImpl<PlotImplT>
{
public:
	CLegendImpl();
	virtual ~CLegendImpl();
	
public:
	
	virtual	int			LegendRegionIdentify(HDC hDC,POINT point);
	
	virtual	void		DrawLegend( HDC hDC );
};

template<class PlotImplT>
CLegendImpl<PlotImplT>::CLegendImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
CLegendImpl<PlotImplT>::~CLegendImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}


template<class PlotImplT>
int			CLegendImpl<PlotImplT>::LegendRegionIdentify(HDC hDC,POINT point)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	RECT plotRect,theRect;

	int dragmarginsize = 0;
	if(m_bShowDragMargin)
		dragmarginsize = m_nDragMarginSize;
	
	//data region
	//plotRect = pT->GetPlotRect(hDC,pT->GetLastClientRect());
	plotRect = pT->GetLastPlotRect();

	// in legend rect
	theRect = GetLegendRect(hDC, plotRect, pT->GetLastClientRect());
	if(!PtInRect(&theRect,point) )
		return -1;
	theRect.left += m_nMarginSizeXOuter + dragmarginsize;
	theRect.right -= m_nMarginSizeXOuter;
	theRect.top += m_nMarginSizeYOuter;
	theRect.bottom -= m_nMarginSizeYOuter;

	if(PtInRect(&theRect,point))
	{
		SIZE eachSize = GetLegendSizeEach(hDC, plotRect);

		SetLegends();

		int nLegend = min(m_nLegends, m_nMaxVerticalNumOfLegend*m_nHorizontalNumOfLegend);
		int nH, nV;
		nV = (nLegend - 1) / m_nHorizontalNumOfLegend + 1;
		if(nV == 1)
			nH = nLegend;
		else
		nH = m_nHorizontalNumOfLegend;

		int ix, iy;
		ix = (point.x - theRect.left)/eachSize.cx;
		iy = (point.y - theRect.top)/eachSize.cy;
		if(ix<0)ix = 0;
		else if(ix >= nH)ix = nH - 1;
		if(iy<0)iy = 0;
		else if(iy >= nV)iy = nV - 1;

		if(iy * nH + ix >= nLegend)
			return pT->GetPlotDataCount();
		else
			return iy *nH + ix;
	}
	else
	{
		return pT->GetPlotDataCount();
	}
}

void	MyDrawLegendLine(HDC hDC, RECT rt, COLORREF lineCr, int lineSize, int lineStyle, int nMarginSizeXInner, int nRealLineLength);

template<class PlotImplT>
void		CLegendImpl<PlotImplT>::DrawLegend( HDC hDC )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(pT->IsEmpty())return;
	if(!m_bLegendShow)return;

	RECT plotRect = pT->GetLastPlotRect();

	RECT legendRect = GetLegendRect(hDC,plotRect, pT->GetLastClientRect());
	if(!m_bCanShow)
	{
		m_bCanShow = true;
		return;
	}
	EraseLegendBkgnd(hDC, legendRect);

	int dragmarginsize = 0;
	if(m_bShowDragMargin)
		dragmarginsize = m_nDragMarginSize;

	SetLegends();

	int nLegend = min(m_nLegends, m_nMaxVerticalNumOfLegend*m_nHorizontalNumOfLegend);
	int i;

	HFONT	hOldFont, hFont;
	LOGFONT logFont;
	HPEN hLinePen, hOldPen;
	COLORREF crText;
	SIZE	tmSize;
	tstring	strName, strAppendix;
	RECT eachRect, textRect;

	CPlotDataImpl<DataPoint2D> *pd;

	int nOldBkMode = GetBkMode(hDC);
	SetBkMode( hDC,TRANSPARENT );

	logFont = m_lfFont;
	logFont.lfHeight = m_nRealFontHeight;
	hFont = CreateFontIndirect(&logFont);
	hOldFont = (HFONT)SelectObject(hDC,hFont);
	for(i = 0;i<nLegend;i++)
	{
		pd = pT->GetAbsPlotData(i);
		if(!pd->IsVisible())continue;
		
		hLinePen = CreatePen(pd->GetPlotLineStyle(),pd->GetPlotLineSize(),pd->GetPlotColor());
		hOldPen = (HPEN)SelectObject( hDC,hLinePen );

		crText = pd->GetPlotColor();
		SetTextColor( hDC,crText );
		strName = pT->GetAbsDataTitle(i);//pd->GetTitle();
		strAppendix = pT->GetAbsDataAppendix(i);//pd->GetAppendix();
		if(strName.empty())strName = CGlobal::GetStrUntitle();
		//GetTextExtentPoint(hDC,strName.c_str(),strName.length(),&tmSize);
		MyGetTextExtentPoint(hDC,&logFont,strName,DT_LEFT,&tmSize);

		eachRect = GetLegendRectEach(hDC, plotRect, pT->GetLastClientRect(), i);

		//Draw Marker
		if(m_bMarkerShow)
		{
			//POINT pt={legendRect.left+m_nMarginSize+dragmarginsize+m_nRealLineLength/2, legendRect.top+long((i+0.5)*tmSize.cy)+m_nMarginSize};
			POINT pt = {eachRect.left + m_nMarginSizeXInner + m_nRealLineLength/2, (eachRect.bottom + eachRect.top)/2};
			pT->GetAbsPlotData(i)->DrawSolelyMarker(hDC, pt, true);
		}
		
		
		//Draw Line
		if(!m_bColorRect)
		{
			if( pd->GetPlotType() != kXYPlotScatter && m_bLineShow)
			{
				//MoveToEx(hDC,legendRect.left+m_nMarginSize+dragmarginsize,legendRect.top+long((i+0.5)*tmSize.cy)+m_nMarginSize,NULL);
				//LineTo(hDC,legendRect.left+m_nMarginSize+dragmarginsize+m_nRealLineLength,legendRect.top+long((i+0.5)*tmSize.cy)+m_nMarginSize);
				if( pd->GetDataLineSize() == 1 )
				{
					MoveToEx(hDC, eachRect.left+m_nMarginSizeXInner, (eachRect.bottom + eachRect.top)/2, NULL);
					LineTo(hDC, eachRect.left+m_nMarginSizeXInner+m_nRealLineLength, (eachRect.bottom + eachRect.top)/2);
				}
				else
				{
					MyDrawLegendLine(hDC, eachRect, pd->GetPlotColor(), pd->GetPlotLineSize(), pd->GetPlotLineStyle(), m_nMarginSizeXInner, m_nRealLineLength);
				}
			}
		}
		else
		{
			HBRUSH hDataBrush = CreateSolidBrush(pT->GetAbsPlotData(i)->GetPlotColor());
			HBRUSH hOldBrush = (HBRUSH)SelectObject( hDC,hDataBrush );
			//RECT colorRect={legendRect.left+m_nMarginSize+dragmarginsize, legendRect.top+i*tmSize.cy+m_nMarginSize, legendRect.left+m_nMarginSize+dragmarginsize+m_nRealLineLength, legendRect.top+(i+1)*tmSize.cy+m_nMarginSize};
			RECT colorRect={eachRect.left+m_nMarginSizeXInner, eachRect.top, eachRect.left+m_nMarginSizeXInner+m_nRealLineLength, eachRect.bottom};
			Rectangle(hDC, colorRect.left, colorRect.top, colorRect.right, colorRect.bottom);
			SelectObject(hDC, hOldBrush);
			DeleteObject(hDataBrush);
		}

		//Draw Title and appendix, title aligned left, appendix aligned right
		//TextOut(hDC,legendRect.left+m_nMarginSize+dragmarginsize+m_nRealLineLength,legendRect.top+i*tmSize.cy+m_nMarginSize,strName.c_str(),strName.length());
		textRect.left = eachRect.left + m_nMarginSizeXInner + m_nRealLineLength;
		textRect.right = eachRect.right - m_nMarginSizeXInner;
		textRect.top = eachRect.top;
		textRect.bottom = eachRect.bottom;
		DrawText(hDC, strName.c_str(), (int)strName.length(), &textRect, DT_TOP | DT_LEFT);
		DrawText(hDC, strAppendix.c_str(), (int)strAppendix.length(), &textRect, DT_TOP | DT_RIGHT);
		
		SelectObject(hDC, hOldPen);
		DeleteObject(hLinePen);

	}

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);

	SetBkMode(hDC, nOldBkMode);
}


Declare_Namespace_End
