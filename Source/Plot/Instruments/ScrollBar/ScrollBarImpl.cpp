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

#include "ScrollBarImpl.h"

using namespace NsCChart;

CScrollBarImpl::CScrollBarImpl()
{
	SetShowBar(true);
	//SetFillBar(true);
	m_bFillCurr = false;
}

CScrollBarImpl::~CScrollBarImpl()
{

}

void	CScrollBarImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CScrollBarImpl::OnDraw(HDC hDC, RECT destRect)
{
	RECT plotRect = destRect;
	RECT barRect = GetBarRect(hDC, plotRect);
	
	DrawBorder(hDC, plotRect);
	DrawBar(hDC, barRect);
	DrawSlider(hDC, barRect);
}

RECT	CScrollBarImpl::GetBarRect(HDC hDC, RECT plotRect)
{
	return plotRect;
}

RECT	CScrollBarImpl::GetSliderRect(HDC hDC, RECT barRect)
{
	RECT rect = barRect;
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		rect.left = GetPosInBar(m_pfSliderRange[0], barRect, true);
		rect.right = GetPosInBar(m_pfSliderRange[1], barRect, true);
		break;
	case kInstruShapeVertical:
		rect.bottom = GetPosInBar(m_pfSliderRange[0], barRect, false);
		rect.top = GetPosInBar(m_pfSliderRange[1], barRect, false);
		break;
	}
	return rect;
}

RECT	CScrollBarImpl::GetLowerRect(HDC hDC, RECT barRect)
{
	RECT rect = barRect;
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		rect.right = GetPosInBar(m_pfSliderRange[0], barRect, true);
		break;
	case kInstruShapeVertical:
		rect.top = GetPosInBar(m_pfSliderRange[0], barRect, false);
		break;
	}
	return rect;
}

RECT	CScrollBarImpl::GetUpperRect(HDC hDC, RECT barRect)
{
	RECT rect = barRect;
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		rect.left = GetPosInBar(m_pfSliderRange[1], barRect, true);
		break;
	case kInstruShapeVertical:
		rect.bottom = GetPosInBar(m_pfSliderRange[1], barRect, false);
		break;
	}
	return rect;
}

RECT	CScrollBarImpl::GetLowerPRect(HDC hDC, RECT barRect)
{
	RECT rect = barRect;
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		rect.right = GetPosInBar(m_pfSliderRange[1], barRect, true);
		break;
	case kInstruShapeVertical:
		rect.top = GetPosInBar(m_pfSliderRange[1], barRect, false);
		break;
	}
	return rect;
}

RECT	CScrollBarImpl::GetUpperPRect(HDC hDC, RECT barRect)
{
	RECT rect = barRect;
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		rect.left = GetPosInBar(m_pfSliderRange[0], barRect, true);
		break;
	case kInstruShapeVertical:
		rect.bottom = GetPosInBar(m_pfSliderRange[0], barRect, false);
		break;
	}
	return rect;
}

void	CScrollBarImpl::DrawSlider(HDC hDC, RECT barRect)
{
	RECT sliderRect = barRect;
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		sliderRect.left = GetPosInBar(m_pfSliderRange[0], barRect, true);
		sliderRect.right = GetPosInBar(m_pfSliderRange[1], barRect, true);
		InflateRect(&sliderRect, 0, -2);
		break;
	case kInstruShapeVertical:
		sliderRect.bottom = GetPosInBar(m_pfSliderRange[0], barRect, false);
		sliderRect.top = GetPosInBar(m_pfSliderRange[1], barRect, false);
		InflateRect(&sliderRect, -2, 0);
		break;
	default:
		return;
	}
	DrawEdge(hDC, &sliderRect, EDGE_RAISED, BF_RECT|BF_MIDDLE);
}

long	CScrollBarImpl::GetPosInBar(double val, RECT barRect, bool bHoriz)
{
	if(bHoriz)
	{
		return barRect.left + long(Width(barRect)/MyRange(m_pfRange)*(val-m_pfRange[0]));
	}
	else
	{
		return barRect.bottom - long(Height(barRect)/MyRange(m_pfRange)*(val-m_pfRange[0]));
	}
}

double	CScrollBarImpl::GetValInBar(long pos, RECT barRect, bool bHoriz)
{
	if(bHoriz)
	{
		return m_pfRange[0] + MyRange(m_pfRange)/Width(barRect)*(pos-barRect.left);
	}
	else
	{
		return m_pfRange[0] - MyRange(m_pfRange)/Height(barRect)*(barRect.bottom-pos);
	}
}

double	CScrollBarImpl::GetRatio(RECT barRect)
{
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		return MyRange(m_pfRange)/Width(barRect);
	case kInstruShapeVertical:
		return MyRange(m_pfRange)/Height(barRect);
	default:
		return 1.0;
	}
}

bool	CScrollBarImpl::HitLower(POINT point, RECT barRect)
{
	if(!PtInRect(&barRect, point))return false;

	long cursor;

	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		cursor = GetPosInBar(m_pfSliderRange[0], barRect, true);
		if(abs(cursor-point.x)<=2)return true;
		break;
	case kInstruShapeVertical:
		cursor = GetPosInBar(m_pfSliderRange[0], barRect, false);
		if(abs(cursor-point.y)<=2)return true;
		break;
	default:
		return false;
	}
	return false;
}

bool	CScrollBarImpl::HitUpper(POINT point, RECT barRect)
{
	if(!PtInRect(&barRect, point))return false;

	long cursor;
	
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		cursor = GetPosInBar(m_pfSliderRange[1], barRect, true);
		if(abs(cursor-point.x)<=2)return true;
		break;
	case kInstruShapeVertical:
		cursor = GetPosInBar(m_pfSliderRange[1], barRect, false);
		if(abs(cursor-point.y)<=2)return true;
		break;
	default:
		return false;
	}
	return false;
}
