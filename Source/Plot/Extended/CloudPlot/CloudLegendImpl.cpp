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
#include "../../Basic/Global.h"

#include "CloudLegendImpl.h"
//#include "CloudPlotImpl.h"
#include "../../Basic/PlotUtility.h"
#include "../../Basic/MyString.h"

#include "../../Resources/PlotResources.h"

using namespace NsCChart;
/*
CCloudLegendImpl::CCloudLegendImpl(CCloudPlotImpl *pPlotImpl) : CLegendImpl(pPlotImpl)
{
	m_nLineLength = 30;
	m_nDragMarginSize = 5;

	m_nLegends = 6;

	m_crBkgndColor = RGB(128, 192, 128);

	if(CGlobal::m_bLoadDefPatterns)
	{
		HBITMAP hb = (HBITMAP)LoadImage(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDB_LEGEND_PATTERN), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		if(hb)
		{
			SetBkgndPattern(hb);
			DeleteObject(hb);
		}
	}
}

CCloudLegendImpl::~CCloudLegendImpl()
{

}

RECT	CCloudLegendImpl::GetInitialPlotRect(HDC hDC, RECT chartRect)
{
	CCloudPlotImpl *pPlot = (CCloudPlotImpl *)m_pPlot;
	
	RECT legendRect = {0,0,0,0};
	if(!pPlot)return legendRect;
	if(!pPlot->IsUseLegend())return legendRect;

	// init
	legendRect.bottom = (7*chartRect.top+chartRect.bottom)/8;
	legendRect.top = chartRect.top;
	legendRect.left = (chartRect.left+2*chartRect.right)/3;
	legendRect.right = chartRect.right;

	int nLegend = m_nLegends;
	
//	HFONT		hOldFont, hFont;
	bool		keepGoing = true;
	SIZE		legendSize,tmSize;
	LOGFONT		logFont;
	int			i;
	tstring		strName;
	
	//adjust the font size
	logFont = m_lfFont;
	m_nRealLineLength = m_nLineLength;

	while( keepGoing == true )
	{
		legendSize.cx = legendSize.cy = 0;
		for(i = 0;i<nLegend;i++)
		{
			strName = pPlot->GetAbsoluteDataTitle(i);
			if(strName.empty())strName = CGlobal::GetStrUntitle();
			//GetTextExtentPoint(hDC,strName.c_str(),strName.length(),&tmSize);
			MyGetTextExtentPoint(hDC,logFont,strName,DT_LEFT,&tmSize);
			if(tmSize.cx>legendSize.cx)legendSize.cx = tmSize.cx;
			legendSize.cy += tmSize.cy;
		}
		
		if( legendSize.cx+m_nRealLineLength+2*m_nMarginSizeXOuter+m_nDragMarginSize > abs(legendRect.right-legendRect.left)
			|| legendSize.cy+2*m_nMarginSizeYOuter> abs(legendRect.bottom-legendRect.top) )
		{
			if( legendSize.cx+m_nRealLineLength+2*m_nMarginSizeXOuter+m_nDragMarginSize > abs(legendRect.right-legendRect.left) )
			{
				if( legendSize.cx+m_nRealLineLength+2*m_nMarginSizeXOuter+m_nDragMarginSize > abs(chartRect.right-chartRect.left) )
				{
					if(logFont.lfHeight >=6 )
						logFont.lfHeight--;
					else
						m_nRealLineLength--;
				}
				else
				{
					keepGoing = false;
				}
			}
			if( legendSize.cy+2*m_nMarginSizeYOuter> abs(legendRect.bottom-legendRect.top) )
			{
				if( legendSize.cy+2*m_nMarginSizeYOuter> abs(chartRect.bottom-chartRect.top) )
				{
					if(logFont.lfHeight >=6 )
						logFont.lfHeight--;
					else
						m_nRealLineLength--;
				}
				else
				{
					keepGoing = false;
				}
			}
			
		}
		else
		{
			keepGoing = false;
		}
		
		if( logFont.lfHeight <= 4 || m_nRealLineLength <= 10 ) keepGoing = false;
	}
	m_nRealFontHeight = logFont.lfHeight;

	// actual size
	legendRect.right = legendRect.left+legendSize.cx+m_nRealLineLength+2*m_nMarginSizeXOuter+m_nDragMarginSize;
	legendRect.top = legendRect.bottom - legendSize.cy - 2*m_nMarginSizeYOuter;

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
		legendRect.right = chartRect.right;
		legendRect.left = legendRect.right - legendSize.cx - m_nRealLineLength - 2*m_nMarginSizeXOuter - m_nDragMarginSize;
	}
	if(legendRect.top < chartRect.top)
	{
		legendRect.top = chartRect.top;
		legendRect.bottom = legendRect.top + legendSize.cy + 2*m_nMarginSizeYOuter;
	}
	return legendRect;
}

int		CCloudLegendImpl::LegendRegionIdentify(HDC hDC,POINT point)
{
	CCloudPlotImpl *pPlot = (CCloudPlotImpl *)m_pPlot;
	
	RECT plotRect = pPlot->GetLastPlotRect( );
	RECT legendRect = GetLegendRect(hDC, plotRect, pPlot->GetLastClientRect());
	
	if(!PtInRect(&legendRect,point) )
		return -1;
	else
		return 0;
}

tstring	CCloudLegendImpl::GetLegendTitle(int index)
{
	CCloudPlotImpl *pPlot = (CCloudPlotImpl *)m_pPlot;
	if(index<0 || index>=m_nLegends)return _TEXT("");
	
	double v1,v2,value;;
	v1 = pPlot->GetMinValue();
	v2 = pPlot->GetMaxValue();

	if( pPlot->IsExponential() && v1>0 )
	{
		v1 = log10(v1);
		v2 = log10(v2);
		value = v1 + index*(v2 - v1)/(m_nLegends - 1);
		value = pow(10.0, value);
	}
	else
	{
		value = v1 + index*(v2 - v1)/(m_nLegends - 1);
	}
	value = Round(value,2);

	tostringstream ostr;
	ostr<<value;
	return ostr.str();
}

void	CCloudLegendImpl::OnDraw( HDC hDC, RECT plotRect)
{
	if(!m_pPlot)return;
	CCloudPlotImpl *pPlot = (CCloudPlotImpl *)m_pPlot;
	if(pPlot->IsEmpty())return;
	
	RECT legendRect = GetLegendRect(hDC, plotRect, pPlot->GetLastClientRect());
	if(!m_bCanShow)
	{
		m_bCanShow = true;
		return;
	}
	EraseBkgnd(hDC, legendRect);
	
	int nLegend = m_nLegends;
	int i;
	
	HFONT	hOldFont, hFont;
	LOGFONT logFont;
	SIZE	tmSize;
	tstring	strName;
	
	SetBkMode( hDC,TRANSPARENT );
	logFont = m_lfFont;
	logFont.lfHeight = m_nRealFontHeight;
	hFont = CreateFontIndirect(&logFont);
	hOldFont = (HFONT)SelectObject(hDC,hFont);

	//Draw the color rect
	RECT	colorRect = legendRect;
	//GetTextExtentPoint(hDC, CGlobal::GetStrUntitle(), _tcslen(_TEXT("Un")),&tmSize);
	MyGetTextExtentPoint(hDC,logFont, CGlobal::GetStrUntitle(), DT_LEFT,&tmSize);
	colorRect.left = legendRect.left+m_nMarginSizeXOuter+m_nDragMarginSize;
	colorRect.top = legendRect.top + m_nMarginSizeYOuter + tmSize.cy/2;
	colorRect.right	= colorRect.left + m_nRealLineLength;
	colorRect.bottom = legendRect.bottom - m_nMarginSizeYOuter - tmSize.cy/2;

	TRIVERTEX        vert[2];
	GRADIENT_RECT    gRect;
	UINT r,g,b;
	r=GetRValue(pPlot->GetColor2());
	g=GetGValue(pPlot->GetColor2());
	b=GetBValue(pPlot->GetColor2());

	vert [0] .x      = colorRect.left;
	vert [0] .y      = colorRect.top;
	vert [0] .Red    = r*256;
	vert [0] .Green  = g*256;
	vert [0] .Blue   = b*256;
	vert [0] .Alpha  = 0x0000;
	
	r=GetRValue(pPlot->GetColor1());
	g=GetGValue(pPlot->GetColor1());
	b=GetBValue(pPlot->GetColor1());

	vert [1] .x      = colorRect.right;
	vert [1] .y      = colorRect.bottom;
	vert [1] .Red    = r*256;
	vert [1] .Green  = g*256;
	vert [1] .Blue   = b*256;
	vert [1] .Alpha  = 0x0000;

	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;

	GradientFill(hDC, vert, 4, &gRect, 1, GRADIENT_FILL_RECT_V);

	// Draw title
	SetTextColor( hDC,RGB(0,0,0) );
	for(i = 0;i<nLegend;i++)
	{
		strName = GetLegendTitle(nLegend -1 - i);
		if(strName.empty())strName = CGlobal::GetStrUntitle();
		//GetTextExtentPoint(hDC,strName.c_str(),strName.length(),&tmSize);
		MyGetTextExtentPoint(hDC,logFont,strName, DT_LEFT,&tmSize);

		//Draw Title
		TextOut(hDC,legendRect.left+m_nMarginSizeXOuter+m_nDragMarginSize+m_nRealLineLength,legendRect.top+i*tmSize.cy+m_nMarginSizeYOuter,strName.c_str(),(int)strName.length());
		
	}
	SelectObject( hDC,hOldFont );
	//SelectObject( hDC,hOldPen );
	DeleteObject(hFont);
	//DeleteObject(hBorderPen);
}
*/
