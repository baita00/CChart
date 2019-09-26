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
#include "GradienterImpl.h"
using namespace NsCChart;

CGradienterImpl::CGradienterImpl()
{
	m_nInstruShape = kInstruShapeCircle;

	m_nTickLength = 15;
	m_nMinorTickLength = 10;

	m_bHideIdleArc = false;

	m_fAngleStart = myPi;
	m_fAngleRange = myPi;

	m_nTickCount = 5;
	m_nMinorTickCount = 2;
	m_bShowTickLabel = false;
	m_bPointerCircle3D = false;

	m_nPointerWithdraw = 0;
	m_nPointerShape = kPointerOuterTriangle;

	m_pfRange[0] = 0;
	m_pfRange[1] = myPi;

	m_nPointerSwordLenEnd = 30;
	m_nPointerHalfWidthEnd = 10;

	m_bShowStick = false;
	m_bFillCurr = false;
	m_bShowBar = false;
}

CGradienterImpl::~CGradienterImpl()
{

}

void	CGradienterImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CGradienterImpl::OnDraw(HDC hDC, RECT destRect)
{
	RECT plotRect = GetPlotRect(hDC, destRect);
	RECT barRect = GetBarRect(hDC, plotRect);

	DrawBorder(hDC, plotRect);
	DrawTitle(hDC, barRect, plotRect);
	
	double angle = m_fAngleStart;
	double val = m_fCurrValue;
	if(!m_bClockWise)
		m_fAngleStart += m_fCurrValue;
	else
		m_fAngleStart -= m_fCurrValue;
	DrawBar(hDC, barRect);
	DrawAxis(hDC, barRect);
	if(!m_bClockWise)
		m_fCurrValue = angle + myPi/2.0;
	else
		m_fCurrValue = angle - myPi/2.0;
	DrawPointer(hDC, barRect);
	m_fCurrValue = val;
	m_fAngleStart = angle;
}

void	CGradienterImpl::DrawBar(HDC hDC, RECT barRect)
{
	CInstruBaseImpl::DrawBar(hDC, barRect);
	RECT pointerRect = GetPointerRect(hDC, barRect);
	
	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	
	SolidBrush brush1(Color(GetRValue(m_crAirColor), GetGValue(m_crAirColor), GetBValue(m_crAirColor)));
	SolidBrush brush2(Color(GetRValue(m_crWaterColor), GetGValue(m_crWaterColor), GetBValue(m_crWaterColor)));
	graph.FillPie(&brush1, pointerRect.left, pointerRect.top, Width(pointerRect), Height(pointerRect), 180, 180);
	graph.FillPie(&brush2, pointerRect.left, pointerRect.top, Width(pointerRect), Height(pointerRect), 0, 180);

	double r = Width(pointerRect)/2.0;
	double tht = m_fAngleStart;
	double ctx, cty;
	ctx = GetCenterX(pointerRect);
	cty = GetCenterY(pointerRect);
	
	Pen pen(Color(GetRValue(m_crSurfaceColor), GetGValue(m_crSurfaceColor), GetBValue(m_crSurfaceColor)), (Gdiplus::REAL)m_nSurfaceThick);
	graph.DrawLine(&pen, float(ctx + r * cos(tht)), float(cty - r * sin(tht)), float(ctx - r * cos(tht)), float(cty + r * sin(tht)));

}

void	CGradienterImpl::DrawPointer(HDC hDC, RECT barRect)
{
	CInstruBaseImpl::DrawPointer(hDC, barRect);

	Graphics graph(hDC);
	double ctx, cty, radius, tht;
	RECT axisRect = GetAxisRect(hDC, barRect);
	GetPointerParams(hDC, axisRect, tht, radius, ctx, cty);
	SolidBrush brush(Color(128, 244, 0, 0));
	graph.FillPie(&brush, float(ctx - radius), float(cty - radius), float(2.0 * radius), float(2.0 * radius), Gdiplus::REAL(-m_fAngleStart/myPi*180.0), Gdiplus::REAL(m_bClockWise?180.0:-180.0));
}
