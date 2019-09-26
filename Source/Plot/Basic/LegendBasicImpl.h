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

#include "Legend.h"

Declare_Namespace_CChart

template<class PlotImplT>
class CLegendBasicImpl :  public CLegend
{
public:
	CLegendBasicImpl();
	virtual ~CLegendBasicImpl();
protected:
	bool				m_bCanShow;
	int					m_nRealLineLength;
	int					m_nRealFontHeight;

	void				EraseLegendBkgnd(HDC hDC, RECT legendRect);
	virtual RECT		GetInitialPlotRect(HDC hDC, RECT chartRect);

public:
	virtual	RECT		GetLegendRect(HDC hDC, RECT chartRect, RECT clientRect);
	virtual	SIZE		GetLegendSize(HDC hDC, RECT chartRect);
	virtual	int			LegendRegionIdentify(HDC hDC,POINT point)=0;

	RECT				AdjustByLegend( HDC hDC, RECT plotRect);

protected:
	virtual	SIZE		GetLegendSizeEach(HDC hDC, RECT chartRect);
	RECT				GetLegendRectEach(HDC hDC, RECT chartRect, RECT clientRect, int nIndex);

protected:
	void				SetLegends();

};

template<class PlotImplT>
CLegendBasicImpl<PlotImplT>::CLegendBasicImpl()
{
	m_bCanShow = true;

	m_nRealLineLength = m_nLineLength;
	m_nRealFontHeight = m_lfFont.lfHeight;
}

template<class PlotImplT>
CLegendBasicImpl<PlotImplT>::~CLegendBasicImpl()
{
	
}

void	MyDrawLegendRoundBorder(HDC hDC, RECT legendRect, COLORREF bordercolor, int lineSize, int lineStyle, int width, int height);

template<class PlotImplT>
void	CLegendBasicImpl<PlotImplT>::EraseLegendBkgnd(HDC hDC, RECT legendRect)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(pT->IsEmpty())return;
	
	COLORREF bordercolor;
	if(m_bLighted)
	{
		bordercolor = pT->GetLightColor();
	}
	else
	{
		bordercolor = m_crBorderColor;
	}
	HPEN		hBorderPen, hOldPen;
	if(m_bBorderShow)
	{
		hBorderPen = CreatePen( m_nBorderLineStyle, m_nBorderLineSize, bordercolor );
	}
	else
	{
		hBorderPen = (HPEN)GetStockObject(NULL_PEN);
	}
	hOldPen = (HPEN)SelectObject( hDC,hBorderPen );
	
	HBRUSH		hBrush, hOldBrush;
	if(m_bTransparent)
	{
		hBrush = (HBRUSH) GetStockObject(NULL_BRUSH);
	}
	else
	{
		switch(m_nBkgndBrushType)
		{
		case kBrushSolid:
			hBrush = CreateSolidBrush(m_crBkgndColor);
			break;
		case kBrushHatch:
			hBrush = CreateHatchBrush( m_nBkgndHatchStyle, m_crBkgndHatchColor);
			break;
		case kBrushPattern:
			hBrush = CreatePatternBrush( m_hBkgndPattern );
			break;
		default:
			hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			break;
		}
	}
	hOldBrush = (HBRUSH)SelectObject(hDC,hBrush);

	if(m_bShadow && m_bBorderShow && !m_bTransparent)
	{
		HBRUSH hShadowBrush = CreateSolidBrush(m_crShadow);
		RECT shadowRect = legendRect;
		OffsetRect(&shadowRect, m_sizeShadowOffset.cx, m_sizeShadowOffset.cy);
		FillRect(hDC, &shadowRect, hShadowBrush);
		DeleteObject(hShadowBrush);
	}
	
	if(!m_bRound)
		Rectangle(hDC,legendRect.left,legendRect.top,legendRect.right,legendRect.bottom);
	else
	{
		float width, height;
		width = (float)m_nRoundRadius;
		height = width * Height(legendRect)/(float)Width(legendRect)*m_nMaxVerticalNumOfLegend*m_nHorizontalNumOfLegend/(float)m_nLegends;
		
		if(m_bBorderShow)
		{
			MyDrawLegendRoundBorder(hDC, legendRect, bordercolor, m_nBorderLineSize, m_nBorderLineStyle, width, height);
		}
		
		//Fill only
		HPEN NullPen;
		NullPen = (HPEN)GetStockObject(NULL_PEN);
		SelectObject(hDC, NullPen);
		RoundRect(hDC,legendRect.left+m_nBorderLineSize,legendRect.top+m_nBorderLineSize,legendRect.right,legendRect.bottom, (int)width, (int)height);
		SelectObject(hDC, hBorderPen);
		DeleteObject(NullPen);
	}
	
	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hBorderPen);
}

template<class PlotImplT>
RECT		CLegendBasicImpl<PlotImplT>::GetInitialPlotRect(HDC hDC, RECT chartRect)
{
	RECT legendRect = {0,0,0,0};
	
	int dragmarginsize = 0;
	if(m_bShowDragMargin)
		dragmarginsize = m_nDragMarginSize;
	
	// init position
	legendRect.bottom = (7*chartRect.top+chartRect.bottom)/8;
	legendRect.top = chartRect.top;
	legendRect.left = (chartRect.left+2*chartRect.right)/3;
	legendRect.right = chartRect.right;
	
	SIZE legendSize = GetLegendSize(hDC, chartRect);
	
	legendRect.right = legendRect.left + legendSize.cx;
	legendRect.bottom = legendRect.top + legendSize.cy;

	OffsetRect(&legendRect, 0, 5);
	
	// 
	if(m_nPosition != kLegendArbitrary )return legendRect;
	
	m_bCanShow = true;
	// if it is too large
	if( abs(legendRect.right-legendRect.left)>abs(chartRect.right-chartRect.left) || abs(legendRect.bottom-legendRect.top)>abs(chartRect.bottom-chartRect.top) )
	{
		m_bCanShow = false;
		RECT nullRect = {0,0,0,0};
		return nullRect;
	}
	
	if(!m_bConfined)return legendRect;
	
	// if it is out of range
	if(legendRect.right > chartRect.right)//over the right border
	{
		legendRect.left -= (legendRect.right - chartRect.right);
		legendRect.right = chartRect.right;
	}
	if(legendRect.bottom > chartRect.bottom)
	{
		legendRect.top -= (legendRect.bottom - chartRect.bottom);
		legendRect.bottom = chartRect.bottom;
	}
	return legendRect;
}

template<class PlotImplT>
RECT		CLegendBasicImpl<PlotImplT>::GetLegendRect(HDC hDC, RECT chartRect, RECT clientRect)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	RECT legendRect = {0,0,0,0};

	SIZE legendSize, axisSize;
//	CAxisImpl *axis;

	legendSize = GetLegendSize(hDC, chartRect);
	switch(m_nPosition)
	{
	case kLegendBottom:
		
		legendRect.left = (chartRect.left+chartRect.right)/2 - legendSize.cx/2;
		legendRect.right = legendRect.left + legendSize.cx;
		legendRect.top = chartRect.bottom + m_nGap;
/*		axis = pT->GetAxisByLocation(kLocationBottom);
		if(axis)
		{
			axisSize = axis->GetSize(hDC);
			legendRect.top += axisSize.cy;
		}*/
		axisSize = pT->GetAxisSize(hDC, kLocationBottom);
		legendRect.top += axisSize.cy;
		legendRect.bottom = legendRect.top + legendSize.cy;
		break;
	case kLegendBottomLeft:
		legendRect.left = chartRect.left;
		legendRect.right = legendRect.left + legendSize.cx;
		legendRect.top = chartRect.bottom + m_nGap;
/*		axis = pT->GetAxisByLocation(kLocationBottom);
		if(axis)
		{
			axisSize = axis->GetSize(hDC);
			legendRect.top += axisSize.cy;
		}*/
		axisSize = pT->GetAxisSize(hDC, kLocationBottom);
		legendRect.top += axisSize.cy;
		legendRect.bottom = legendRect.top + legendSize.cy;
		break;
	case kLegendBottomRight:
		legendRect.right = chartRect.right;
		legendRect.left = legendRect.right - legendSize.cx;
		legendRect.top = chartRect.bottom + m_nGap;
/*		axis = pT->GetAxisByLocation(kLocationBottom);
		if(axis)
		{
			axisSize = axis->GetSize(hDC);
			legendRect.top += axisSize.cy;
		}*/
		axisSize = pT->GetAxisSize(hDC, kLocationBottom);
		legendRect.top += axisSize.cy;
		legendRect.bottom = legendRect.top + legendSize.cy;
		break;
	case kLegendTop:
		legendRect.left = (chartRect.left+chartRect.right)/2 - legendSize.cx/2;
		legendRect.right = legendRect.left + legendSize.cx;
		legendRect.bottom = chartRect.top - m_nGap;
/*		axis = pT->GetAxisByLocation(kLocationTop);
		if(axis)
		{
			axisSize = axis->GetSize(hDC);
			legendRect.bottom -= axisSize.cy;
		}*/
		axisSize = pT->GetAxisSize(hDC, kLocationTop);
		legendRect.bottom -= axisSize.cy;
		legendRect.top = legendRect.bottom - legendSize.cy;
		break;
	case kLegendTopLeft:
		legendRect.left = chartRect.left;
		legendRect.right = legendRect.left + legendSize.cx;
		legendRect.bottom = chartRect.top - m_nGap;
/*		axis = pT->GetAxisByLocation(kLocationTop);
		if(axis)
		{
			axisSize = axis->GetSize(hDC);
			legendRect.bottom -= axisSize.cy;
		}*/
		axisSize = pT->GetAxisSize(hDC, kLocationTop);
		legendRect.bottom -= axisSize.cy;
		legendRect.top = legendRect.bottom - legendSize.cy;
		break;
	case kLegendTopRight:
		legendRect.right = chartRect.right;
		legendRect.left = legendRect.right - legendSize.cx;
		legendRect.bottom = chartRect.top - m_nGap;
/*		axis = pT->GetAxisByLocation(kLocationTop);
		if(axis)
		{
			axisSize = axis->GetSize(hDC);
			legendRect.bottom -= axisSize.cy;
		}*/
		axisSize = pT->GetAxisSize(hDC, kLocationTop);
		legendRect.bottom -= axisSize.cy;
		legendRect.top = legendRect.bottom - legendSize.cy;
		break;
	case kLegendLeft:
		legendRect.top = (chartRect.top + chartRect.bottom)/2 - legendSize.cy;
		legendRect.bottom = legendRect.top + legendSize.cy;
		legendRect.right = chartRect.left - m_nGap;
/*		axis = pT->GetAxisByLocation(kLocationLeft);
		if(axis)
		{
			axisSize = axis->GetSize(hDC);
			legendRect.right -= axisSize.cx;
		}*/
		axisSize = pT->GetAxisSize(hDC, kLocationLeft);
		legendRect.right -= axisSize.cx;
		legendRect.left = legendRect.right - legendSize.cx;
		break;
	case kLegendLeftTop:
		legendRect.top = chartRect.top;
		legendRect.bottom = legendRect.top + legendSize.cy;
		legendRect.right = chartRect.left - m_nGap;
/*		axis = pT->GetAxisByLocation(kLocationLeft);
		if(axis)
		{
			axisSize = axis->GetSize(hDC);
			legendRect.right -= axisSize.cx;
		}*/
		axisSize = pT->GetAxisSize(hDC, kLocationLeft);
		legendRect.right -= axisSize.cx;
		legendRect.left = legendRect.right - legendSize.cx;
		break;
	case kLegendLeftBottom:
		legendRect.bottom = chartRect.bottom;
		legendRect.top = legendRect.bottom - legendSize.cy;
		legendRect.right = chartRect.left - m_nGap;
/*		axis = pT->GetAxisByLocation(kLocationLeft);
		if(axis)
		{
			axisSize = axis->GetSize(hDC);
			legendRect.right -= axisSize.cx;
		}*/
		axisSize = pT->GetAxisSize(hDC, kLocationLeft);
		legendRect.right -= axisSize.cx;
		legendRect.left = legendRect.right - legendSize.cx;
		break;
	case kLegendRight:
		legendRect.top = (chartRect.top + chartRect.bottom)/2 - legendSize.cy;
		legendRect.bottom = legendRect.top + legendSize.cy;
		legendRect.left = chartRect.right + m_nGap;
/*		axis = pT->GetAxisByLocation(kLocationRight);
		if(axis)
		{
			axisSize = axis->GetSize(hDC);
			legendRect.left += axisSize.cx;
		}*/
		axisSize = pT->GetAxisSize(hDC, kLocationRight);
		legendRect.left += axisSize.cx;
		legendRect.right = legendRect.left + legendSize.cx;
		break;
	case kLegendRightTop:
		legendRect.top = chartRect.top;
		legendRect.bottom = legendRect.top + legendSize.cy;
		legendRect.left = chartRect.right + m_nGap;
/*		axis = pT->GetAxisByLocation(kLocationRight);
		if(axis)
		{
			axisSize = axis->GetSize(hDC);
			legendRect.left += axisSize.cx;
		}*/
		axisSize = pT->GetAxisSize(hDC, kLocationRight);
		legendRect.left += axisSize.cx;
		legendRect.right = legendRect.left + legendSize.cx;
		break;
	case kLegendRightBottom:
		legendRect.bottom = chartRect.bottom;
		legendRect.top = legendRect.bottom - legendSize.cy;
		legendRect.left = chartRect.right + m_nGap;
/*		axis = pT->GetAxisByLocation(kLocationRight);
		if(axis)
		{
			axisSize = axis->GetSize(hDC);
			legendRect.left += axisSize.cx;
		}*/
		axisSize = pT->GetAxisSize(hDC, kLocationRight);
		legendRect.left += axisSize.cx;
		legendRect.right = legendRect.left + legendSize.cx;
		break;
	case kLegendInnerLeftTop:
		legendRect.top = chartRect.top;
		legendRect.bottom = legendRect.top + legendSize.cy;
		legendRect.left = chartRect.left;
		legendRect.right = legendRect.left + legendSize.cx;
		break;
	case kLegendInnerLeftBottom:
		legendRect.bottom = chartRect.bottom;
		legendRect.top = legendRect.bottom - legendSize.cy;
		legendRect.left = chartRect.left;
		legendRect.right = legendRect.left + legendSize.cx;
		break;
	case kLegendInnerLeft:
		legendRect.top = (chartRect.top + chartRect.bottom - legendSize.cy)/2;
		legendRect.bottom = (chartRect.top + chartRect.bottom + legendSize.cy)/2;
		legendRect.left = chartRect.left;
		legendRect.right = legendRect.left + legendSize.cx;
		break;
	case kLegendInnerRightTop:
		legendRect.top = chartRect.top;
		legendRect.bottom = legendRect.top + legendSize.cy;
		legendRect.right = chartRect.right;
		legendRect.left = legendRect.right - legendSize.cx;
		break;
	case kLegendInnerRightBottom:
		legendRect.bottom = chartRect.bottom;
		legendRect.top = legendRect.bottom - legendSize.cy;
		legendRect.right = chartRect.right;
		legendRect.left = legendRect.right - legendSize.cx;
		break;
	case kLegendInnerRight:
		legendRect.top = (chartRect.top + chartRect.bottom - legendSize.cy)/2;
		legendRect.bottom = (chartRect.top + chartRect.bottom + legendSize.cy)/2;
		legendRect.right = chartRect.right;
		legendRect.left = legendRect.right - legendSize.cx;
		break;
	case kLegendInnerTop:
		legendRect.top = chartRect.top;
		legendRect.bottom = legendRect.top + legendSize.cy;
		legendRect.left = (chartRect.left + chartRect.right - legendSize.cx)/2;
		legendRect.right = (chartRect.left + chartRect.right + legendSize.cx)/2;
		break;
	case kLegendInnerBottom:
		legendRect.bottom = chartRect.bottom;
		legendRect.top = legendRect.bottom - legendSize.cy;
		legendRect.left = (chartRect.left + chartRect.right - legendSize.cx)/2;
		legendRect.right = (chartRect.left + chartRect.right + legendSize.cx)/2;
		break;
	case kLegendArbitrary:
	default:
		legendRect = GetInitialPlotRect(hDC, chartRect);
		if(!m_bCanShow)
		{
			return legendRect;
		}

		// add the offset
		OffsetRect(&legendRect,m_sizeOffset.cx,m_sizeOffset.cy);
		if(!m_bConfined)return legendRect;
		
		// restrict in the rectangle
		int width = legendRect.right-legendRect.left;
		int height = legendRect.bottom - legendRect.top;
		if(legendRect.right > chartRect.right)
		{
			legendRect.right = chartRect.right;
			legendRect.left = legendRect.right - width;
		}
		if(legendRect.left < chartRect.left)
		{
			legendRect.left = chartRect.left;
			legendRect.right = legendRect.left + width;
		}
		if(legendRect.top < chartRect.top)
		{
			legendRect.top = chartRect.top;
			legendRect.bottom = legendRect.top + height;
		}
		if(legendRect.bottom > chartRect.bottom)
		{
			legendRect.bottom = chartRect.bottom;
			legendRect.top = legendRect.bottom - height;
		}
	}
	
	return legendRect;
}

template<class PlotImplT>
SIZE		CLegendBasicImpl<PlotImplT>::GetLegendSize(HDC hDC, RECT chartRect)
{	
	RECT legendRect = {0, 0, 0, 0};
	SIZE legendSize = {0, 0};
	
	int dragmarginsize = 0;
	if(m_bShowDragMargin)
		dragmarginsize = m_nDragMarginSize;
	
	SetLegends();
	
	int nLegend = min(m_nLegends, m_nMaxVerticalNumOfLegend*m_nHorizontalNumOfLegend);
	int nH, nV;
	nV = (nLegend - 1) / m_nHorizontalNumOfLegend + 1;
	if(nV == 1)
		nH = nLegend;
	else
		nH = m_nHorizontalNumOfLegend;
	
	bool		keepGoing = true;
	SIZE		eachSize;
	LOGFONT		logFont;
	
	//adjust the font size
	logFont = m_lfFont;
	m_nRealLineLength = m_nLineLength;
	m_nRealFontHeight = m_lfFont.lfHeight;
	
	while( keepGoing == true )
	{
		eachSize = GetLegendSizeEach(hDC, chartRect);
		legendSize.cx = dragmarginsize + 2*m_nMarginSizeXOuter + eachSize.cx * nH;
		legendSize.cy = 2*m_nMarginSizeYOuter + eachSize.cy * nV;
		
		if(legendSize.cx > abs(chartRect.right - chartRect.left)
			|| legendSize.cy > abs(chartRect.bottom - chartRect.top) )
		{
			if(m_nRealLineLength > 30)
			{
				m_nRealLineLength--;
			}
			else if(m_nRealFontHeight > 8)
			{
				m_nRealFontHeight--;
			}
			else
			{
				keepGoing = false;
			}
		}
		else
		{
			keepGoing = false;
		}
	}
	return legendSize;
}

template<class PlotImplT>
RECT		CLegendBasicImpl<PlotImplT>::AdjustByLegend( HDC hDC, RECT plotRect )
{
	SIZE legendSize = GetLegendSize(hDC, plotRect);
	int gap = GetLegendGap();
	switch(GetLegendPosition())
	{
	case kLegendTop:
	case kLegendTopLeft:
	case kLegendTopRight:
		plotRect.top += (legendSize.cy+gap);
		break;
	case kLegendBottom:
	case kLegendBottomLeft:
	case kLegendBottomRight:
		plotRect.bottom -= (legendSize.cy+gap);
		break;
	case kLegendLeft:
	case kLegendLeftTop:
	case kLegendLeftBottom:
		plotRect.left += (legendSize.cx+gap);
		break;
	case kLegendRight:
	case kLegendRightTop:
	case kLegendRightBottom:
		plotRect.right -= (legendSize.cx+gap);
		break;
	}
	
	return plotRect;
}

template<class PlotImplT>
SIZE	CLegendBasicImpl<PlotImplT>::GetLegendSizeEach(HDC hDC, RECT chartRect)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
	LOGFONT	logFont;
	logFont = m_lfFont;
	logFont.lfHeight = m_nRealFontHeight;
	
	GetLegends();
	int nLegend = min(m_nLegends, m_nMaxVerticalNumOfLegend*m_nHorizontalNumOfLegend);
	
	tstring strName;
	SIZE	legendSize = {0, 0}, tmSize;
	legendSize.cx = legendSize.cy = 0;
	for(int i = 0;i<nLegend;i++)
	{
		strName = pT->GetAbsDataTitle(i);//GetAbsoluteDataTitle(i);
		strName += pT->GetAbsDataAppendix(i);
		if(strName.empty())strName = CGlobal::GetStrUntitle();
		MyGetTextExtentPoint(hDC,&logFont,strName,DT_LEFT,&tmSize);
		if(tmSize.cx>legendSize.cx)legendSize.cx = tmSize.cx;
		if(tmSize.cy>legendSize.cy)legendSize.cy = tmSize.cy;
	}
	legendSize.cx += (m_nRealLineLength + 2*m_nMarginSizeXInner);
	legendSize.cy += 2*m_nMarginSizeYInner;
	
	return legendSize;
}

template<class PlotImplT>
RECT	CLegendBasicImpl<PlotImplT>::GetLegendRectEach(HDC hDC, RECT chartRect, RECT clientRect, int nIndex)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	RECT legendRect = GetLegendRect(hDC, chartRect, clientRect);
	RECT singleRect = {0, 0, 0, 0};
	
	GetLegends();
	int nLegend = min(m_nLegends, m_nMaxVerticalNumOfLegend*m_nHorizontalNumOfLegend);
	if(nLegend <= 0)return singleRect;
	if(nIndex < 0 || nIndex >= nLegend)return singleRect;
	
	int nH, nV;
	nV = (nLegend - 1) / m_nHorizontalNumOfLegend + 1;
	if(nV == 1)
		nH = nLegend;
	else
		nH = m_nHorizontalNumOfLegend;
	
	int ix, iy;
	ix = nIndex%nH;
	iy = nIndex/nH;
	
	int dragmarginsize = 0;
	if(m_bShowDragMargin)
		dragmarginsize = m_nDragMarginSize;
	
	SIZE eachSize = GetLegendSizeEach(hDC, chartRect);
	int cx, cy;
	cx = eachSize.cx;
	cy = eachSize.cy;
	
	singleRect.left = legendRect.left + m_nMarginSizeXOuter + dragmarginsize + ix * cx;
	singleRect.right = singleRect.left + cx;;
	singleRect.top = legendRect.top + m_nMarginSizeYOuter + iy * cy;
	singleRect.bottom = singleRect.top + cy;
	
	return singleRect;
}

template<class PlotImplT>
void	CLegendBasicImpl<PlotImplT>::SetLegends()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	int num = pT->GetPlotDataCount();
	if(num >= 1 && num <= 10)
		m_nLegends = pT->GetPlotDataCount();
}


Declare_Namespace_End