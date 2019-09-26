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

#include "SliderImpl.h"

using namespace NsCChart;

CSliderImpl::CSliderImpl()
{

}

CSliderImpl::~CSliderImpl()
{

}

void	CSliderImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CSliderImpl::OnDraw(HDC hDC, RECT destRect)
{
	RECT plotRect = GetPlotRect(hDC, destRect);
	RECT barRect = GetBarRect(hDC, plotRect);
	
	DrawBorder(hDC, plotRect);
	DrawBar(hDC, barRect);
	DrawTitle(hDC, barRect, plotRect);
	DrawAxis(hDC, barRect);
}

RECT	CSliderImpl::GetBarRect(HDC hDC, RECT plotRect)
{
	RECT barRect = CInstruBaseImpl::GetBarRect(hDC, plotRect);
	SIZE paddingSize = GetPaddingSize(hDC);
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		if(paddingSize.cx > m_nSliderHalfLen)
			barRect.left += (paddingSize.cx - m_nSliderHalfLen);
		if(paddingSize.cy > m_nSliderHalfLen)
			barRect.right -= (paddingSize.cy - m_nSliderHalfLen);
		break;
	case kInstruShapeVertical:
		if(paddingSize.cy > m_nSliderHalfLen)
			barRect.top += (paddingSize.cy - m_nSliderHalfLen);
		if(paddingSize.cx > m_nSliderHalfLen)
			barRect.bottom -= (paddingSize.cx - m_nSliderHalfLen);
		break;
	}
	return barRect;
}

RECT	CSliderImpl::GetAxisRect(HDC hDC, RECT barRect)
{
	RECT axisRect = CInstruBaseImpl::GetAxisRect(hDC, barRect);
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		axisRect.left += m_nSliderHalfLen;
		axisRect.right -= m_nSliderHalfLen;
		break;
	case kInstruShapeVertical:
		axisRect.top += m_nSliderHalfLen;
		axisRect.bottom -= m_nSliderHalfLen;
		break;
	}
	return axisRect;
}

void	CSliderImpl::DrawBar(HDC hDC, RECT barRect)
{
	CInstruBaseImpl::DrawBar(hDC, barRect);
	if(m_bShowBarCenter)DrawBarCenter(hDC, barRect);
	DrawSlider(hDC, barRect);
}

void	CSliderImpl::DrawBarCenter(HDC hDC, RECT barRect)
{
	RECT axisRect = GetAxisRect(hDC, barRect);
	RECT centerRect = axisRect;
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		centerRect.top = long((barRect.bottom + barRect.top)/2.0 - m_nBarCenterWidth/2.0);
		centerRect.bottom = centerRect.top + m_nBarCenterWidth;
		break;
	case kInstruShapeVertical:
		centerRect.left = long((barRect.left + barRect.right)/2.0 - m_nBarCenterWidth/2.0);
		centerRect.right = centerRect.left + m_nBarCenterWidth;
		break;
	}
	DrawEdge(hDC, &centerRect, EDGE_SUNKEN, BF_RECT|BF_MIDDLE);
}

void	CSliderImpl::DrawSlider(HDC hDC, RECT barRect)
{
	RECT axisRect = GetAxisRect(hDC, barRect);
	RECT sliderRect;

	HBRUSH hBrush, hOldBrush;
	hBrush = CreateSolidBrush(m_crSliderColor);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	int pos;
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		sliderRect.top = barRect.top;
		sliderRect.bottom = barRect.bottom;
		if(m_bBarBorder3D)
			InflateRect(&sliderRect, 0, -2);
		
		pos = int(axisRect.left + Width(axisRect)*(m_fCurrValue - m_pfRange[0])/(m_pfRange[1] - m_pfRange[0]));	
		
		sliderRect.right = pos;
		sliderRect.left = sliderRect.right - m_nSliderHalfLen;
		//FillRect(hDC, &sliderRect, hBrush);
		DrawEdge(hDC, &sliderRect, EDGE_RAISED, BF_RECT|BF_MIDDLE);
		
		sliderRect.left = pos;
		sliderRect.right = sliderRect.left + m_nSliderHalfLen;
		DrawEdge(hDC, &sliderRect, EDGE_RAISED, BF_RECT|BF_MIDDLE);
		break;
	case kInstruShapeVertical:
		sliderRect.left = barRect.left;
		sliderRect.right = barRect.right;
		if(m_bBarBorder3D)
			InflateRect(&sliderRect, -2, 0);

		pos = int(axisRect.bottom - Height(axisRect)*(m_fCurrValue - m_pfRange[0])/(m_pfRange[1] - m_pfRange[0]));
		
		sliderRect.bottom = pos;
		sliderRect.top = sliderRect.bottom - m_nSliderHalfLen;
		DrawEdge(hDC, &sliderRect, EDGE_RAISED, BF_RECT|BF_MIDDLE);

		sliderRect.top = pos;
		sliderRect.bottom = sliderRect.top + m_nSliderHalfLen;
		DrawEdge(hDC, &sliderRect, EDGE_RAISED, BF_RECT|BF_MIDDLE);
		break;
	}

	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
}

void	CSliderImpl::DrawCurrentFilling(HDC hDC, RECT barRect)
{
	if(m_fCurrValue < m_pfRange[0] || m_fCurrValue > m_pfRange[1] || !m_bShowBar || !m_bFillCurr)return;
	
	RECT fillRect = barRect;
	if(m_bBarBorder3D)
		InflateRect(&fillRect, -2, -2);
	RECT axisRect = GetAxisRect(hDC, barRect);
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		fillRect.right = long(axisRect.left + Width(axisRect)*(m_fCurrValue - m_pfRange[0])/(m_pfRange[1] - m_pfRange[0]));	
		break;
	case kInstruShapeVertical:
		fillRect.top = long(axisRect.bottom - Height(axisRect)*(m_fCurrValue - m_pfRange[0])/(m_pfRange[1] - m_pfRange[0]));
		break;
	case kInstruShapeCircle:
		return;
	}
	
	HBRUSH hBrush = CreateSolidBrush(m_crCurrColor);
	FillRect(hDC, &fillRect, hBrush);
	DeleteObject(hBrush);
}
