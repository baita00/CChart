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

#include "AnalogClockImpl.h"

using namespace NsCChart;

CAnalogClockImpl::CAnalogClockImpl()
{
	m_nInstruShape = kInstruShapeCircle;

	m_nTickLength = 10;
	m_nMinorTickLength = 5;

	m_bBorder3D = true;

	SetRange(0, 12);
	m_nTickCount = 11;
	m_nMinorTickCount = 5;

	m_fAngleStart = myPi/2.0;
	m_nLabelEdgeType = kShowLabelEdgeEnd;
	m_nEdgeGap = 10;

	m_bShowPointerTail = false;
	m_bShowBar = false;
	m_bPointerCircle3D = false;
	m_bShowAxisLine = false;

	m_bTickBar = true;
	m_bMinorTickBar = true;
	m_nTickBarHalfWidthBase = 3;
	m_nTickBarHalfWidthEnd = 3;
	m_nMinorTickBarHalfWidthBase = 2;
	m_nMinorTickBarHalfWidthEnd = 2;
	m_bFillWholeCircle = true;
	m_crWholeCircleColor = RGB(192, 192, 96);

	m_bFillSubTitleRect = true;
}

CAnalogClockImpl::~CAnalogClockImpl()
{

}

void	CAnalogClockImpl::SetShowTime(bool show)
{
	m_bShowSubTitle = show;
}

void	CAnalogClockImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CAnalogClockImpl::OnDraw(HDC hDC, RECT destRect)
{
	RECT plotRect = GetPlotRect(hDC, destRect);
	RECT barRect = GetBarRect(hDC, plotRect);
	
	DrawBorder(hDC, plotRect);
	DrawTitle(hDC, barRect, plotRect);
	DrawBar(hDC, barRect);
	DrawWholeCircleFilling(hDC, barRect);
	DrawAxis(hDC, barRect);

	if(m_bShowSubTitle)
	{
		RECT axisRect = GetAxisRect(hDC, barRect);
		SIZE offset;
		offset.cx = 0;
		offset.cy = -Height(axisRect)/3;
		SetSubTitleOffset(offset);
		tostringstream ostr;
		tstring title;
		ostr.width(2);
		ostr.fill(TCHAR('0'));
		ostr << right << m_nHour;
		title = ostr.str();

		title += _TEXT(":");
		
		ostr.str(_TEXT(""));
		ostr << right << m_nMinute; 
		title += ostr.str();

		title += _TEXT(" ");

		ostr.str(_TEXT(""));
		ostr << right << m_nSecond; 
		title += ostr.str();

		SetSubTitle(title);
		DrawSubTitle(hDC, barRect);
	}
	

	DrawPointer(hDC, barRect);
	
}

void	CAnalogClockImpl::DrawPointer(HDC hDC, RECT barRect)
{
	int hour, minute, second;
	hour = m_nHour%12;
	minute = m_nMinute%60;
	second = m_nSecond%60;
	
	bool bFill = m_bFillPointerCircle;
	bool b3D = m_bPointerCircle3D;
	
	m_bPointerCircle3D = false;
	m_bFillPointerCircle = false;
	
	m_fCurrValue = hour + minute/60.0 + second/3600.0;
	m_nPointerShape = m_nHourShape;
	m_nPointerWithdraw = m_nHourWithdraw;
	m_nPointerHalfWidthBase = m_nHourBaseHW;
	m_nPointerHalfWidthEnd = m_nHourEndHW;
	m_crPointerColor1 = m_crHourColor;
	CInstruBaseImpl::DrawPointer(hDC, barRect);
	
	m_fCurrValue = (minute + second/60.0)* 12.0 / 60.0;
	m_nPointerShape = m_nMinuteShape;
	m_nPointerWithdraw = m_nMinuteWithdraw;
	m_nPointerHalfWidthBase = m_nMinuteBaseHW;
	m_nPointerHalfWidthEnd = m_nMinuteEndHW;
	m_crPointerColor1 = m_crMinuteColor;
	CInstruBaseImpl::DrawPointer(hDC, barRect);
	
	m_bPointerCircle3D = b3D;
	m_bFillPointerCircle = bFill;

	m_fCurrValue = second * 12.0 / 60.0;
	m_nPointerShape = m_nSecondShape;
	m_nPointerWithdraw = m_nSecondWithdraw;
	m_nPointerHalfWidthBase = m_nSecondBaseHW;
	m_nPointerHalfWidthEnd = m_nSecondEndHW;
	m_crPointerColor1 = m_crSecondColor;
	CInstruBaseImpl::DrawPointer(hDC, barRect);
}