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
#include "SevenSegmentCodeImpl.h"
#include <math.h>
#include "../../Basic/PlotUtility.h"

using namespace NsCChart;

CSevenSegmentCodeImpl::CSevenSegmentCodeImpl()
{
	m_nMarginHorizontal = 2;
	m_nMarginVertical = 2;

	m_fCurrValue = -1;
}

CSevenSegmentCodeImpl::~CSevenSegmentCodeImpl()
{

}

void	CSevenSegmentCodeImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CSevenSegmentCodeImpl::OnDraw(HDC hDC, RECT destRect)
{
	RECT plotRect = GetPlotRect(hDC, destRect);
	RECT barRect = GetBarRect(hDC, plotRect);

	DrawBorder(hDC, plotRect);

	Graphics graph(hDC);
	SolidBrush brush(Color(GetRValue(m_crCodeColor), GetGValue(m_crCodeColor), GetBValue(m_crCodeColor)));
	float ctx, cty;
	
	int val = (int)m_fCurrValue;
	if(val >= 0 && val <= 9)
	{
		for(int i=0; i<7; i++)
		{
			DrawSingleSeg(hDC, barRect, i, Seven_Segment_Code[val][i]);
		}
	}
	else if(val == 10)
	{
		ctx = (float)GetCenterX(barRect);
		cty = float(barRect.bottom - 2*m_nPointCodeSize);
		graph.FillEllipse(&brush, ctx - m_nPointCodeSize, cty - m_nPointCodeSize, 2.0f * m_nPointCodeSize, 2.0f * m_nPointCodeSize);
	}
	else if(val == 11)
	{
		ctx = (float)GetCenterX(barRect);
		cty = float(GetCenterY(barRect) - 2*m_nPointCodeSize);
		graph.FillEllipse(&brush, ctx - m_nPointCodeSize, cty - m_nPointCodeSize, 2.0f * m_nPointCodeSize, 2.0f * m_nPointCodeSize);
		cty = float(GetCenterY(barRect) + 2*m_nPointCodeSize);
		graph.FillEllipse(&brush, ctx - m_nPointCodeSize, cty - m_nPointCodeSize, 2.0f * m_nPointCodeSize, 2.0f * m_nPointCodeSize);
	}
	else if(val == 12)
	{

	}
}

SIZE	CSevenSegmentCodeImpl::GetSize(int val)
{
	RECT destRect = {0, 0, 100, 100};
	return GetSize(val, destRect);
}

SIZE	CSevenSegmentCodeImpl::GetSize(int val, RECT destRect)
{
	SIZE sscSize;
	double oldVal = m_fCurrValue;
	m_fCurrValue = val;
	
	RECT codeRect = GetPlotRect(NULL, destRect);
	sscSize.cx = Width(codeRect) + 2*m_nMarginHorizontal;
	sscSize.cy = Height(codeRect) + 2*m_nMarginVertical;
	
	m_fCurrValue = oldVal;
	
	return sscSize;
}

RECT	CSevenSegmentCodeImpl::GetPlotRect(HDC hDC, RECT destRect)
{
	RECT plotRect = destRect;

	InflateRect(&plotRect, -m_nMarginHorizontal, -m_nMarginVertical);

	double dx, dy;
	if(m_bSunkenVCenter)
		dx = m_nVBarThick/2.0;
	else
		dx = 0;
	if(m_bSunkenVCenter)
		dy = m_nHBarThick/2.0;
	else
		dy = 0; 
	
	double xlen, ylen;
	xlen = m_nHBarLen + dx * 2  + m_nInnerGap * 2;
	plotRect.right = plotRect.left + m_nMarginHorizontal * 2;
	plotRect.right += long(xlen);
	
	ylen = m_nVBarLen + dy * 2 + m_nInnerGap * 5;
	plotRect.bottom = plotRect.top + m_nMarginVertical * 2;
	plotRect.bottom += long(ylen);

	if(m_bSlope)
		plotRect.right += long(ylen * sin(m_fAngleSlope));

	if(m_fCurrValue == 10 || m_fCurrValue == 11 || m_fCurrValue == 12)
	{
		plotRect.right = plotRect.left + m_nPointCodeSize + m_nInnerGap*2 + m_nMarginHorizontal * 2;
		return plotRect;
	}

	return plotRect;
}

RECT	CSevenSegmentCodeImpl::GetBarRect(HDC hDC, RECT plotRect)
{
	RECT barRect = plotRect;
	InflateRect(&barRect, -m_nMarginHorizontal, -m_nMarginVertical);
	return barRect;
}

RECT	CSevenSegmentCodeImpl::GetSingleSegRect(HDC hDC, RECT barRect, int nIndex)
{
	RECT	segRect = barRect;
	RECT	nullRect = {0, 0, 0, 0};

	double dx, dy;
	if(m_bSunkenVCenter)
		dx = m_nVBarThick/2.0;
	else
		dx = 0;
	if(m_bSunkenVCenter)
		dy = m_nHBarThick/2.0;
	else
		dy = 0;

	double height = Height(barRect);
	switch(nIndex)
	{
	case 0:
		segRect.top += 2*m_nInnerGap;
		segRect.bottom = long(GetCenterY(barRect) - m_nInnerGap/2.0);
		if(m_bSlope)
			segRect.left += long(height/2.0 * sin(m_fAngleSlope));
		segRect.right = segRect.left + m_nVBarThick;
		if(m_bSlope)
			segRect.right += long(height/2.0 * sin(m_fAngleSlope));
		break;
	case 1:
		segRect.top = long(GetCenterY(barRect) + m_nInnerGap/2.0);
		segRect.bottom -= 2*m_nInnerGap;
		segRect.right = segRect.left + m_nVBarThick;
		if(m_bSlope)
			segRect.right += long(height/2.0 * sin(m_fAngleSlope));
		break;
	case 2:
		segRect.bottom = segRect.top + m_nHBarThick;
		segRect.left += m_nInnerGap;
		segRect.left += long(dx);
		if(m_bSlope)
			segRect.left += long(height * sin(m_fAngleSlope));
		segRect.right = segRect.left + m_nHBarLen;
		break;
	case 3:
		segRect.top += long(m_nInnerGap*2.0);
		segRect.bottom = long(GetCenterY(barRect) - m_nInnerGap/2.0);
		segRect.left = segRect.right - m_nHBarThick;
		if(m_bSlope)
			segRect.left -= long(height/2.0 * sin(m_fAngleSlope));
		break;
	case 4:
		segRect.top = long(GetCenterY(barRect) + m_nInnerGap/2.0);
		segRect.bottom -= 2*m_nInnerGap;
		if(m_bSlope)
			segRect.right -= long(height/2.0 * sin(m_fAngleSlope));
		segRect.left = segRect.right - m_nVBarThick;
		if(m_bSlope)
			segRect.left -= long(height/2.0 * sin(m_fAngleSlope));
		break;
	case 5:
		segRect.top = long(GetCenterY(barRect) - m_nHBarThick/2.0);
		segRect.bottom = segRect.top + m_nHBarThick;
		segRect.left += m_nInnerGap;
		segRect.left += long(m_nVBarThick - dx);
		if(m_bSlope)
			segRect.left += long(height/2.0 * sin(m_fAngleSlope));
		segRect.right -= m_nInnerGap;
		segRect.right -= long(m_nVBarThick - dx);
		if(m_bSlope)
			segRect.right -= long(height/2.0 * sin(m_fAngleSlope));
		break;
	case 6:
		segRect.top = segRect.bottom - m_nHBarThick;
		segRect.left += m_nInnerGap;
		segRect.left += long(dx);
		//if(m_bSlope)
		//	segRect.left += height * sin(m_fAngleSlope);
		segRect.right = segRect.left + m_nHBarLen;
		break;
	default:
		return nullRect;
	}
	return segRect;
}

void	CSevenSegmentCodeImpl::DrawSingleSeg(HDC hDC, RECT barRect, int nIndex, bool bLight)
{
	RECT segRect = GetSingleSegRect(hDC, barRect, nIndex);

	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	Color brushColor,edgeColor;
	int val = (int)m_fCurrValue;
	if( bLight )
		brushColor = Color::MakeARGB(255, GetRValue(m_crCodeColor), GetGValue(m_crCodeColor), GetBValue(m_crCodeColor));
	else
		brushColor = Color::MakeARGB(255, GetRValue(m_crOffColor), GetGValue(m_crOffColor), GetBValue(m_crOffColor));
	edgeColor = Color::MakeARGB(255, GetRValue(m_crSegEdgeColor), GetGValue(m_crSegEdgeColor), GetBValue(m_crSegEdgeColor));
	SolidBrush brush(brushColor);
	Pen pen(edgeColor, 1);
	
	float dx, dy;
	if(!m_bSlope)
	{
		dx = 0;
		dy = 0;
	}
	else
	{
		dx = float(m_nHBarThick*sin(m_fAngleSlope));
		dy = float(m_nVBarThick*sin(m_fAngleSlope));
	}
	PointF pPt[6];

	if(m_bSunkenVCenter)
	{
		switch(nIndex)
		{
		case 0:
		case 3:
			pPt[0].X = float(segRect.right - m_nVBarThick/2.0);
			pPt[0].Y = float(segRect.top);
			pPt[1].X = float(segRect.right - dx/2.0);
			pPt[1].Y = float(segRect.top + m_nHBarThick/2.0);
			pPt[2].X = float(segRect.left + m_nVBarThick + dx/2.0);
			pPt[2].Y = float(segRect.bottom - m_nHBarThick/2.0);
			pPt[3].X = float(segRect.left + m_nVBarThick/2.0);
			pPt[3].Y = float(segRect.bottom);
			pPt[4].X = float(segRect.left+ dx/2.0);
			pPt[4].Y = float(segRect.bottom - m_nHBarThick/2.0) ;
			pPt[5].X = float(segRect.right - m_nVBarThick - dx/2.0);
			pPt[5].Y = float(segRect.top + m_nHBarThick/2.0);
			break;
		case 1:
		case 4:
			pPt[0].X = float(segRect.left + m_nVBarThick/2.0);
			pPt[0].Y = float(segRect.bottom);
			pPt[1].X = float(segRect.left + dx/2.0);
			pPt[1].Y = float(segRect.bottom - m_nHBarThick/2.0);
			pPt[2].X = float(segRect.right - m_nVBarThick - dx/2.0);
			pPt[2].Y = float(segRect.top + m_nHBarThick/2.0);
			pPt[3].X = float(segRect.right - m_nVBarThick/2.0);
			pPt[3].Y = float(segRect.top);
			pPt[4].X = float(segRect.right - dx/2.0);
			pPt[4].Y = float(segRect.top + m_nHBarThick/2.0) ;
			pPt[5].X = float(segRect.left + m_nVBarThick + dx/2.0);
			pPt[5].Y = float(segRect.bottom - m_nHBarThick/2.0);
			break;
		case 2:
		case 5:
		case 6:
			pPt[0].X = float(segRect.left);
			pPt[0].Y = float(segRect.top + m_nHBarThick/2.0);
			pPt[1].X = float(segRect.left + m_nVBarThick/2.0);
			pPt[1].Y = float(segRect.top);
			pPt[2].X = float(segRect.right - m_nVBarThick/2.0);
			pPt[2].Y = float(segRect.top);
			pPt[3].X = float(segRect.right);
			pPt[3].Y = float(segRect.top + m_nHBarThick/2.0);
			pPt[4].X = float(segRect.right - m_nVBarThick/2.0);
			pPt[4].Y = float(segRect.bottom) ;
			pPt[5].X = float(segRect.left + m_nVBarThick/2.0);
			pPt[5].Y = float(segRect.bottom);
			break;
		default:
			return;
		}
		graph.FillPolygon(&brush, pPt, 6, FillModeAlternate);
		graph.DrawPolygon(&pen, pPt, 6);
	}
	else
	{
		switch(nIndex)
		{
		case 0:
			pPt[0].X = float(segRect.right - m_nVBarThick);
			pPt[0].Y = float(segRect.top);
			pPt[1].X = float(segRect.right - dx);
			pPt[1].Y = float(segRect.top + m_nHBarThick);
			pPt[2].X = float(segRect.left + m_nVBarThick);
			pPt[2].Y = float(segRect.bottom);
			pPt[3].X = float(segRect.left);
			pPt[3].Y = float(segRect.bottom);
			break;
		case 1:
			pPt[0].X = float(segRect.left);
			pPt[0].Y = float(segRect.bottom);
			pPt[1].X = float(segRect.left + m_nVBarThick + dx);
			pPt[1].Y = float(segRect.bottom - m_nHBarThick);
			pPt[2].X = float(segRect.right);
			pPt[2].Y = float(segRect.top);
			pPt[3].X = float(segRect.right - m_nVBarThick);
			pPt[3].Y = float(segRect.top);
			break;
		case 2:
			pPt[0].X = float(segRect.left);
			pPt[0].Y = float(segRect.top);
			pPt[1].X = float(segRect.right);
			pPt[1].Y = float(segRect.top);
			pPt[2].X = float(segRect.right - m_nVBarThick);
			pPt[2].Y = float(segRect.top + m_nHBarThick);
			pPt[3].X = float(segRect.left + m_nVBarThick);
			pPt[3].Y = float(segRect.top + m_nVBarThick);
			break;
		case 3:
			pPt[0].X = float(segRect.right);
			pPt[0].Y = float(segRect.top);
			pPt[1].X = float(segRect.right - m_nVBarThick - dx);
			pPt[1].Y = float(segRect.top + m_nHBarThick);
			pPt[2].X = float(segRect.left);
			pPt[2].Y = float(segRect.bottom);
			pPt[3].X = float(segRect.left + m_nVBarThick);
			pPt[3].Y = float(segRect.bottom);
			break;
		case 4:
			pPt[0].X = float(segRect.right);
			pPt[0].Y = float(segRect.top);
			pPt[1].X = float(segRect.right - m_nVBarThick);
			pPt[1].Y = float(segRect.top);
			pPt[2].X = float(segRect.left + dx);
			pPt[2].Y = float(segRect.bottom - m_nHBarThick);
			pPt[3].X = float(segRect.left + m_nVBarThick);
			pPt[3].Y = float(segRect.bottom);
			break;
		case 5:
			pPt[0].X = float(segRect.left + dx/2.0);
			pPt[0].Y = float(segRect.top);
			pPt[1].X = float(segRect.left - dx/2.0);
			pPt[1].Y = float(segRect.bottom);
			pPt[2].X = float(segRect.right - dx/2.0);
			pPt[2].Y = float(segRect.bottom);
			pPt[3].X = float(segRect.right + dx/2.0);
			pPt[3].Y = float(segRect.top);
			break;
		case 6:
			pPt[0].X = float(segRect.left);
			pPt[0].Y = float(segRect.bottom);
			pPt[1].X = float(segRect.right);
			pPt[1].Y = float(segRect.bottom);
			pPt[2].X = float(segRect.right - m_nVBarThick);
			pPt[2].Y = float(segRect.top);
			pPt[3].X = float(segRect.left + m_nVBarThick);
			pPt[3].Y = float(segRect.top);
			break;
		default:
			return;
		}
		graph.FillPolygon(&brush, pPt, 4, FillModeAlternate);
		graph.DrawPolygon(&pen, pPt, 4);
	}
}