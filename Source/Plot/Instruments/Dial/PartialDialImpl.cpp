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
#include "PartialDialImpl.h"

using namespace NsCChart;

CPartialDialImpl::CPartialDialImpl()
{
	m_nInstruShape = kInstruShapeCircle;

	m_nTickLength = 15;
	m_nMinorTickLength = 10;

	m_nPointerWithdraw = 8;

	m_nLabelEdgeType = kShowLabelEdgeBoth;

	m_fAngleRange = myPi/3.0;
	m_bShowBar = false;
	m_bHideIdleArc = true;

	m_bPointerCircle3D = false;
	m_nPointerHalfWidthEnd = 2;
	m_nPointerHalfWidthBase = 4;
	m_bShowPointerTail = false;
}

CPartialDialImpl::~CPartialDialImpl()
{

}

void	CPartialDialImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CPartialDialImpl::OnDraw(HDC hDC, RECT destRect)
{
	//CDialImpl::OnDraw(hDC, destRect);
	RECT plotRect = GetPlotRect(hDC, destRect);
	RECT barRect = GetPartialBarRect(hDC, plotRect);
	RECT axisRect = GetPartialAxisRect(hDC, barRect);
	RECT psedoAxisRect = GetPsedoAxisRect(hDC, axisRect);
	RECT psedoBarRect = GetPsedoBarRectFromAxis(hDC, psedoAxisRect);

//	Rectangle(hDC, plotRect.left+2, plotRect.top+2, plotRect.right-2, plotRect.bottom-2);
	
	DrawBorder(hDC, plotRect);
	DrawTitle(hDC, barRect, plotRect);
	DrawBar(hDC, psedoBarRect);
	DrawAxis(hDC, psedoBarRect);
	DrawSubTitle(hDC, barRect);

	HRGN hRgn = CreateRectRgn(barRect.left, barRect.top, barRect.right, barRect.bottom);
	SelectClipRgn(hDC, hRgn);
	CDialImpl::DrawPointer(hDC, psedoBarRect);
	SelectClipRgn(hDC, NULL);
	DeleteObject(hRgn);

	DrawGlass(hDC, barRect, psedoBarRect);
}

void	CPartialDialImpl::DrawGlass(HDC hDC, RECT barRect, RECT psedoBarRect)
{
	if(!m_bShowGlass)return;

	Graphics graph(hDC);
	SolidBrush brush(Color(m_nGlassAlpha, GetRValue(m_crGlassColor), GetGValue(m_crGlassColor), GetBValue(m_crGlassColor)));

	GraphicsPath path;

	float ctx, cty, radius,tht, sgn;
	float x1, y1, x2, y2;

	ctx = (float)GetCenterX(psedoBarRect);
	cty = (float)GetCenterY(psedoBarRect);
	radius = (float)Width(psedoBarRect)/2.0f;

	tht = (float)m_fAngleStart;
	sgn = m_bClockWise?-1.0f:1.0f;

	double delta;
	double tickL = GetSideTickLengh(true);
	SIZE labelSize = GetMaxSingleTickLabelSize(hDC);
	delta = m_nEdgeGap + m_bShowTickLabel?max(labelSize.cx, labelSize.cy):0 + m_bShowBar?m_nBarWidth:0 + GetSideTickLengh(true);
	
	switch(m_nPartialDialType)
	{
	case kPartialDialTop:
	case kPartialDialBottom:
	case kPartialDialLeft:
	case kPartialDialRight:
		path.AddArc(ctx - radius, cty - radius, 2 * radius, 2 * radius, float(-tht/myPi*180.0), float(-m_fAngleRange/myPi*180.0*sgn));

		x1 = float(ctx + radius * cos(tht + m_fAngleRange*sgn));
		y1 = float(cty - radius * sin(tht + m_fAngleRange*sgn));
		x2 = float(ctx + radius * cos(tht));
		y2 = float(cty - radius * sin(tht));

		if(m_nPartialDialType == kPartialDialTop)
		{
			path.AddLine(x1, y1, x1, float(barRect.bottom));
			path.AddLine(x1, float(barRect.bottom), x2, float(barRect.bottom));
			path.AddLine(x2, float(barRect.bottom), x2, y2);
		}
		else if(m_nPartialDialType == kPartialDialBottom)
		{
			path.AddLine(x1, y1, x1, float(barRect.top));
			path.AddLine(x1, float(barRect.top), x2, float(barRect.top));
			path.AddLine(x2, float(barRect.top), x2, y2);
		}
		else if(m_nPartialDialType == kPartialDialLeft)
		{
			path.AddLine(x1, y1, float(barRect.right),y1 );
			path.AddLine(float(barRect.right),y1, float(barRect.right),y2);
			path.AddLine(float(barRect.right),y2, x2, y2);
		}
		else if(m_nPartialDialType == kPartialDialRight)
		{
			path.AddLine(x1, y1, float(barRect.left),y1 );
			path.AddLine(float(barRect.left),y1, float(barRect.left),y2);
			path.AddLine(float(barRect.left),y2, x2, y2);
		}
		break;
	case kPartialDialRightTop:
	case kPartialDialRightBottom:
	case kPartialDialLeftTop:
	case kPartialDialLeftBottom:
		radius += max(labelSize.cx, labelSize.cy)/2.0f;
		if(m_nPartialDialType == kPartialDialRightTop)
		{
			ctx -= max(labelSize.cx, labelSize.cy)/2.0f;
			cty += max(labelSize.cx, labelSize.cy)/2.0f;
		}
		else if(m_nPartialDialType == kPartialDialRightBottom)
		{
			ctx -= max(labelSize.cx, labelSize.cy)/2.0f;
			cty -= max(labelSize.cx, labelSize.cy)/2.0f;
		}
		else if(m_nPartialDialType == kPartialDialLeftTop)
		{
			ctx += max(labelSize.cx, labelSize.cy)/2.0f;
			cty += max(labelSize.cx, labelSize.cy)/2.0f;
		}
		else if(m_nPartialDialType == kPartialDialLeftBottom)
		{
			ctx += max(labelSize.cx, labelSize.cy)/2.0f;
			cty -= max(labelSize.cx, labelSize.cy)/2.0f;
		}
		path.AddArc(ctx - radius, cty - radius, 2 * radius, 2 * radius, float(-tht/myPi*180.0), float(-m_fAngleRange/myPi*180.0*sgn));
		
		x1 = float(ctx + radius * cos(tht + m_fAngleRange*sgn));
		y1 = float(cty - radius * sin(tht + m_fAngleRange*sgn));
		x2 = float(ctx + radius * cos(tht));
		y2 = float(cty - radius * sin(tht));

		bool flag;
		if(m_nPartialDialType == kPartialDialRightTop || m_nPartialDialType == kPartialDialLeftBottom)
			flag = m_bClockWise;
		else
			flag = !m_bClockWise;

		if(flag)
		{
			ctx = x2;
			cty = y1;
		}
		else
		{
			ctx = x1;
			cty = y2;
		}
		path.AddLine(x2, y2, ctx, cty);
		path.AddLine(ctx, cty, x1, y1);
		break;
	}

	graph.SetSmoothingMode(SmoothingModeHighQuality);
	graph.FillPath(&brush, &path);
}

RECT	CPartialDialImpl::GetPlotRect(HDC hDC, RECT destRect)
{
	RECT plotRect = destRect;
	RECT nullRect = {0, 0, 0, 0};
	SIZE labelSize = GetMaxSingleTickLabelSize(hDC);

	if(m_nInstruShape != kInstruShapeCircle)return nullRect;
	if(m_fAngleRange > myPi || m_fAngleRange < 0.0)return nullRect;

	double sgn = m_bClockWise?1:-1;

	SIZE titleSize = GetTitleSize(hDC);
	double tickL;

	RECT basicRect;
	long left, top;

	double radius;
	switch(m_nPartialDialType)
	{
	case kPartialDialTop:
	case kPartialDialBottom:
		if(m_nPartialDialType == kPartialDialTop)
			m_fAngleStart = myPi/2.0 + m_fAngleRange/2.0*sgn;
		else
			m_fAngleStart = myPi/2.0*3.0 + m_fAngleRange/2.0*sgn;

		if(m_bShowTitle)
		{
			if(m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionNarrowSmall)
			{
				plotRect.top += titleSize.cy;
			}
		}
		InflateRect(&plotRect, -m_nMarginHorizontal, -m_nMarginVertical);
		left = plotRect.left;
		top = plotRect.top;

		//begin
		InflateRect(&plotRect, -m_nEdgeGap, -m_nEdgeGap);

		if(m_bShowTickLabel)
		{
			InflateRect(&plotRect, -max(labelSize.cx, labelSize.cy), -max(labelSize.cx, labelSize.cy));
		}
		if(m_bShowBar)
		{
			InflateRect(&plotRect, -m_nBarWidth, -m_nBarWidth);
		}

		tickL = GetSideTickLengh(true);
		InflateRect(&plotRect, -(int)tickL, -(int)tickL);
		
		radius = Width(plotRect)/2.0/sin(m_fAngleRange/2.0);
		tickL = GetSideTickLengh(false);
		plotRect.bottom = plotRect.top + long(radius*(1.0 - cos(m_fAngleRange/2.0)));
		plotRect.bottom += (long)tickL;
		plotRect.bottom += m_nPointerWithdraw;
		plotRect.bottom += m_nPointerGap;

		basicRect = plotRect;
		
		tickL = GetSideTickLengh(true);
		InflateRect(&plotRect, (int)tickL, (int)tickL);
		
		if(m_bShowBar)
		{
			InflateRect(&plotRect, m_nBarWidth, m_nBarWidth);
		}

		if(m_bShowTickLabel)
		{
			InflateRect(&plotRect, max(labelSize.cx, labelSize.cy), max(labelSize.cx, labelSize.cy));
		}

		InflateRect(&plotRect, m_nEdgeGap, m_nEdgeGap);

		if(m_nPartialDialType == kPartialDialTop)
		{
			plotRect.bottom = basicRect.bottom;
		}
		else
		{
			plotRect.top = basicRect.top;
		}
		if(plotRect.left != left)
			OffsetRect(&plotRect, left - plotRect.left, 0);
		if(plotRect.top != top)
			OffsetRect(&plotRect, 0, top - plotRect.top);
		//end
		
		InflateRect(&plotRect, m_nMarginHorizontal, m_nMarginVertical);
		if(m_bShowTitle)
		{
			if(m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionNarrowSmall)
			{
				plotRect.top -= titleSize.cy;
			}
			else if(m_nTitlePosition == kInstruPositionWideLarge || m_nTitlePosition == kInstruPositionNarrowLarge)
			{
				plotRect.bottom += titleSize.cy;
			}
		}
		break;

	case kPartialDialLeft:
	case kPartialDialRight:
		if(m_nPartialDialType == kPartialDialLeft)
			m_fAngleStart = myPi + m_fAngleRange/2.0*sgn;
		else
			m_fAngleStart = m_fAngleRange/2.0*sgn;
		
		if(m_bShowTitle)
		{
			if(m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionNarrowSmall)
			{
				plotRect.top += titleSize.cy;
			}
		}
		InflateRect(&plotRect, -m_nMarginHorizontal, -m_nMarginVertical);
		left = plotRect.left;
		top = plotRect.top;

		//begin
		InflateRect(&plotRect, -m_nEdgeGap, -m_nEdgeGap);
\
		if(m_bShowTickLabel)
		{
			InflateRect(&plotRect, -max(labelSize.cx, labelSize.cy), -max(labelSize.cx, labelSize.cy));
		}
		if(m_bShowBar)
		{
			InflateRect(&plotRect, -m_nBarWidth, -m_nBarWidth);
		}

		tickL = GetSideTickLengh(true);
		InflateRect(&plotRect, -(int)tickL, -(int)tickL);
		
		radius = Height(plotRect)/2.0/sin(m_fAngleRange/2.0);
		tickL = GetSideTickLengh(false);
		plotRect.right = plotRect.left + long(radius*(1.0 - cos(m_fAngleRange/2.0)));
		plotRect.right += (long)tickL;
		plotRect.right += m_nPointerWithdraw;
		plotRect.right += m_nPointerGap;
		
		basicRect = plotRect;
		
		tickL = GetSideTickLengh(true);
		InflateRect(&plotRect, (int)tickL, (int)tickL);

		if(m_bShowBar)
		{
			InflateRect(&plotRect, m_nBarWidth, m_nBarWidth);
		}
		if(m_bShowTickLabel)
		{
			InflateRect(&plotRect, max(labelSize.cx, labelSize.cy), max(labelSize.cx, labelSize.cy));
		}
		InflateRect(&plotRect, m_nEdgeGap, m_nEdgeGap);

		if(m_nPartialDialType == kPartialDialLeft)
		{
			plotRect.right = basicRect.right;
		}
		else
		{
			plotRect.left = basicRect.left;
		}
		if(plotRect.left != left)
			OffsetRect(&plotRect, left - plotRect.left, 0);
		if(plotRect.top != top)
			OffsetRect(&plotRect, 0, top - plotRect.top);
		//end

		InflateRect(&plotRect, m_nMarginHorizontal, m_nMarginVertical);
		if(m_bShowTitle)
		{
			if(m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionNarrowSmall)
			{
				plotRect.top -= titleSize.cy;
			}
			else if(m_nTitlePosition == kInstruPositionWideLarge || m_nTitlePosition == kInstruPositionNarrowLarge)
			{
				plotRect.bottom += titleSize.cy;
			}
		}
		break;
	case kPartialDialRightTop:
	case kPartialDialLeftTop:
	case kPartialDialRightBottom:
	case kPartialDialLeftBottom:
		m_fAngleRange = myPi/2.0;

		if(m_nPartialDialType == kPartialDialRightTop)
			m_fAngleStart = myPi/4.0 + m_fAngleRange/2.0*sgn;
		else if(m_nPartialDialType == kPartialDialLeftTop)
			m_fAngleStart = myPi/4.0*3.0 + m_fAngleRange/2.0*sgn;
		else if(m_nPartialDialType == kPartialDialLeftBottom)
			m_fAngleStart = myPi/4.0*5.0 + m_fAngleRange/2.0*sgn;
		else
			m_fAngleStart = myPi/4.0*7.0 + m_fAngleRange/2.0*sgn;

		if(m_bShowTitle)
		{
			if(m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionNarrowSmall)
			{
				plotRect.top += titleSize.cy;
			}
		}
		InflateRect(&plotRect, -m_nMarginHorizontal, -m_nMarginVertical);
		left = plotRect.left;
		top = plotRect.top;

		//begin
		InflateRect(&plotRect, -m_nEdgeGap, -m_nEdgeGap);

		if(m_bShowTickLabel)
		{
			InflateRect(&plotRect, -max(labelSize.cx, labelSize.cy), -max(labelSize.cx, labelSize.cy));
		}

		if(m_bShowBar)
		{
			InflateRect(&plotRect, -m_nBarWidth, -m_nBarWidth);
		}

		tickL = GetSideTickLengh(true);
		InflateRect(&plotRect, -(int)tickL, -(int)tickL);
		
		radius = min(Width(plotRect), Height(plotRect));
		tickL = GetSideTickLengh(false);

		plotRect.right = long(plotRect.left + radius + tickL + m_nPointerWithdraw + m_nPointerGap);
		plotRect.bottom = long(plotRect.top + radius + tickL + m_nPointerWithdraw + m_nPointerGap);

		basicRect = plotRect;
		
		tickL = GetSideTickLengh(true);
		InflateRect(&plotRect, (int)tickL, (int)tickL);

		if(m_bShowBar)
		{
			InflateRect(&plotRect, m_nBarWidth, m_nBarWidth);
		}
		if(m_bShowTickLabel)
		{
			InflateRect(&plotRect, max(labelSize.cx, labelSize.cy), max(labelSize.cx, labelSize.cy));
		}
		InflateRect(&plotRect, m_nEdgeGap, m_nEdgeGap);

		if(m_nPartialDialType == kPartialDialRightTop)
		{
			plotRect.left = basicRect.left;
			plotRect.bottom = basicRect.bottom;
		}
		else if(m_nPartialDialType == kPartialDialLeftTop)
		{
			plotRect.right = basicRect.right;
			plotRect.bottom = basicRect.bottom;
		}
		else if(m_nPartialDialType == kPartialDialLeftBottom)
		{
			plotRect.right = basicRect.right;
			plotRect.top = basicRect.top;
		}
		else
		{
			plotRect.left = basicRect.left;
			plotRect.top = basicRect.top;
		}

		if(plotRect.left != left)
			OffsetRect(&plotRect, left - plotRect.left, 0);
		if(plotRect.top != top)
			OffsetRect(&plotRect, 0, top - plotRect.top);

		//end
		InflateRect(&plotRect, m_nMarginHorizontal, m_nMarginVertical);
		
		if(m_bShowTitle)
		{
			if(m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionNarrowSmall)
			{
				plotRect.top -= titleSize.cy;
			}
			else if(m_nTitlePosition == kInstruPositionWideLarge || m_nTitlePosition == kInstruPositionNarrowLarge)
			{
				plotRect.bottom += titleSize.cy;
			}
		}
		break;
	default:
		return nullRect;
	}

	return plotRect;
}

RECT	CPartialDialImpl::GetPartialBarRect(HDC hDC, RECT plotRect)
{
	RECT barRect = plotRect;
	RECT nullRect = {0, 0, 0, 0};
	
	if(m_nInstruShape != kInstruShapeCircle)return nullRect;
	if(m_fAngleRange > myPi || m_fAngleRange < 0.0)return nullRect;
	
	SIZE titleSize = GetTitleSize(hDC);

	InflateRect(&barRect, -m_nMarginHorizontal, -m_nMarginVertical);
	if(m_bShowTitle)
	{
		if(m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionNarrowSmall)
		{
			barRect.top += titleSize.cy;
		}
		else
		{
			barRect.bottom -= titleSize.cy;
		}
	}

	return barRect;
}

RECT	CPartialDialImpl::GetPartialAxisRect(HDC hDC, RECT barRect)
{
	RECT axisRect = barRect;
	RECT nullRect = {0, 0, 0, 0};

	SIZE labelSize = GetMaxSingleTickLabelSize(hDC);
	
	if(m_nInstruShape != kInstruShapeCircle)return nullRect;
	if(m_fAngleRange > myPi || m_fAngleRange < 0.0)return nullRect;
	
	double tickL;

	InflateRect(&axisRect, -m_nMarginHorizontal, -m_nMarginVertical);
	
	switch(m_nPartialDialType)
	{
	case kPartialDialTop:
	case kPartialDialBottom:
	case kPartialDialLeft:
	case kPartialDialRight:
		
		InflateRect(&axisRect, -m_nEdgeGap, -m_nEdgeGap);
		
		if(m_bShowTickLabel)
		{
			InflateRect(&axisRect, -max(labelSize.cx, labelSize.cy), -max(labelSize.cx, labelSize.cy));
		}
		
		if(m_bShowBar)
		{
			InflateRect(&axisRect, -m_nBarWidth, -m_nBarWidth);
		}
		
		tickL = GetSideTickLengh(true);
		InflateRect(&axisRect, -(int)tickL, -(int)tickL);

		if(m_nPartialDialType == kPartialDialTop)
		{
			axisRect.bottom = barRect.bottom;
		}
		else if(m_nPartialDialType == kPartialDialBottom)
		{
			axisRect.top = barRect.top;
		}
		else if(m_nPartialDialType == kPartialDialLeft)
		{
			axisRect.right = barRect.right;
		}
		else
		{
			axisRect.left = barRect.left;
		}
		break;
	case kPartialDialRightTop:
	case kPartialDialRightBottom:
	case kPartialDialLeftTop:
	case kPartialDialLeftBottom:
		if(Width(axisRect)>Height(axisRect))
			axisRect.right = axisRect.left + Height(axisRect);
		else
			axisRect.bottom = axisRect.top + Width(axisRect);

		double delta;
		tickL = GetSideTickLengh(true);
		delta = m_nEdgeGap + m_bShowTickLabel?max(labelSize.cx, labelSize.cy):0 + m_bShowBar?m_nBarWidth:0 + GetSideTickLengh(true);

		if(m_nPartialDialType == kPartialDialRightTop)
		{
			axisRect.left += max(labelSize.cx, labelSize.cy)/2;
			axisRect.bottom -= max(labelSize.cx, labelSize.cy)/2;
			axisRect.right -= (long)delta;
			axisRect.top += (long)delta;
		}
		else if(m_nPartialDialType == kPartialDialRightBottom)
		{
			axisRect.left += max(labelSize.cx, labelSize.cy)/2;
			axisRect.top += max(labelSize.cx, labelSize.cy)/2;
			axisRect.right -= (long)delta;
			axisRect.bottom -= (long)delta;
		}
		else if(m_nPartialDialType == kPartialDialLeftTop)
		{
			axisRect.right -= max(labelSize.cx, labelSize.cy)/2;
			axisRect.bottom -= max(labelSize.cx, labelSize.cy)/2;
			axisRect.left += (long)delta;
			axisRect.top += (long)delta;
		}
		else if(m_nPartialDialType == kPartialDialLeftBottom)
		{
			axisRect.right -= max(labelSize.cx, labelSize.cy)/2;
			axisRect.top += max(labelSize.cx, labelSize.cy)/2;
			axisRect.left += (long)delta;
			axisRect.bottom -= (long)delta ;
		}

		break;
	default:
		return nullRect;
	}
	
	return axisRect;
}

RECT	CPartialDialImpl::GetPsedoAxisRect(HDC hDC, RECT axisRect)
{
	RECT psedoAxisRect = axisRect;

	double ctx, cty, radius;

	switch(m_nPartialDialType)
	{
	case kPartialDialTop:
		ctx = GetCenterX(axisRect);
		radius = Width(axisRect)/2.0/sin(m_fAngleRange/2.0);
		cty = axisRect.top + radius;
		break;
	case kPartialDialBottom:
		ctx = GetCenterX(axisRect);
		radius = Width(axisRect)/2.0/sin(m_fAngleRange/2.0);
		cty = axisRect.bottom - radius;
		break;
	case kPartialDialLeft:
		cty = GetCenterY(axisRect);
		radius = Height(axisRect)/2.0/sin(m_fAngleRange/2.0);
		ctx = axisRect.left + radius;
		break;
	case kPartialDialRight:
		cty = GetCenterY(axisRect);
		radius = Height(axisRect)/2.0/sin(m_fAngleRange/2.0);
		ctx = axisRect.right - radius;
		break;
	case kPartialDialRightTop:
		ctx = axisRect.left;
		cty = axisRect.bottom;
		radius = min(Width(axisRect), Height(axisRect));
		break;
	case kPartialDialRightBottom:
		ctx = axisRect.left;
		cty = axisRect.top;
		radius = min(Width(axisRect), Height(axisRect));
		break;
	case kPartialDialLeftTop:
		ctx = axisRect.right;
		cty = axisRect.bottom;
		radius = min(Width(axisRect), Height(axisRect));
		break;
	case kPartialDialLeftBottom:
		ctx = axisRect.right;
		cty = axisRect.top;
		radius = min(Width(axisRect), Height(axisRect));
		break;
	}
	
	psedoAxisRect.left = long(ctx - radius);
	psedoAxisRect.right = long(ctx + radius);
	psedoAxisRect.top = long(cty - radius);
	psedoAxisRect.bottom = long(cty + radius);

	return psedoAxisRect;
}

RECT	CPartialDialImpl::GetPsedoBarRectFromAxis(HDC hDC, RECT psedoAxisRect)
{
	//RECT barRect = GetPsedoAxisRect(hDC, axisRect);
	RECT barRect = psedoAxisRect;

	SIZE labelSize = GetMaxSingleTickLabelSize(hDC);

	double tickL;
	
	tickL = GetSideTickLengh(true);
	InflateRect(&barRect, (int)tickL, (int)tickL);
	
	if(m_bShowBar)
	{
		InflateRect(&barRect, m_nBarWidth, m_nBarWidth);
	}
	
	if(m_bShowTickLabel)
	{
		InflateRect(&barRect, max(labelSize.cx, labelSize.cy), max(labelSize.cx, labelSize.cy));
	}
	
	InflateRect(&barRect, m_nEdgeGap, m_nEdgeGap);

	return barRect;
}

RECT	CPartialDialImpl::GetSubTitleRect(HDC hDC, RECT barRect)
{
	RECT subtitleRect = barRect;
	tstring subtitle = GetRealSubTitle();

	RECT axisRect = GetPartialAxisRect(hDC, barRect);
	RECT psedoAxisRect = GetPsedoAxisRect(hDC, axisRect);
	RECT psedoBarRect = GetPsedoBarRectFromAxis(hDC, psedoAxisRect);

	if(!m_bSubtitleOuter)
	{
		double tickL = GetSideTickLengh(false);
		psedoBarRect = psedoAxisRect;
		InflateRect(&psedoBarRect, -(int)tickL, -(int)tickL);
		InflateRect(&psedoBarRect, int(-Width(psedoAxisRect)/6.0), int(-Height(psedoAxisRect)/6.0));
	}
	
	switch(m_nPartialDialType)
	{
	case kPartialDialLeft:
	case kPartialDialRight:
	case kPartialDialTop:
	case kPartialDialBottom:
		return CDialImpl::GetSubTitleRect(hDC, barRect);
	case kPartialDialRightTop:
		subtitleRect = GetTextRectAroundEllipse( hDC, psedoBarRect, NULL, myPi/4.0, 1.0, 1.0, subtitle, m_lfSubTitleFont, 4, m_bSubtitleOuter );
		break;
	case kPartialDialRightBottom:
		subtitleRect = GetTextRectAroundEllipse( hDC, psedoBarRect, NULL, myPi/4.0*7.0, 1.0, 1.0, subtitle, m_lfSubTitleFont, 4, m_bSubtitleOuter );
		break;
	case kPartialDialLeftTop:
		subtitleRect = GetTextRectAroundEllipse( hDC, psedoBarRect, NULL, myPi/4.0*3.0, 1.0, 1.0, subtitle, m_lfSubTitleFont, 4, m_bSubtitleOuter );
		break;
	case kPartialDialLeftBottom:
		subtitleRect = GetTextRectAroundEllipse( hDC, psedoBarRect, NULL, myPi/4.0*5.0, 1.0, 1.0, subtitle, m_lfSubTitleFont, 4, m_bSubtitleOuter );
		break;
	}

	OffsetRect(&subtitleRect, m_sizeSubTitleOffset.cx, m_sizeSubTitleOffset.cy);

	return subtitleRect;
}