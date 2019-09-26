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
#include "TrafficLightImpl.h"

using namespace NsCChart;

CTrafficLightImpl::CTrafficLightImpl()
{
}

CTrafficLightImpl::~CTrafficLightImpl()
{

}

void	CTrafficLightImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CTrafficLightImpl::OnDraw(HDC hDC, RECT destRect)
{
	RECT plotRect = GetPlotRect(hDC, destRect);
	RECT barRect = GetBarRect(hDC, plotRect);

	DrawLight(hDC, barRect);
}

RECT	CTrafficLightImpl::GetPlotRect(HDC hDC, RECT destRect)
{
	RECT plotRect = destRect;
	InflateRect(&plotRect, -m_nMarginHorizontal, -m_nMarginVertical);
	plotRect.right = plotRect.left + 2 * m_nRadius;
	plotRect.bottom = plotRect.top + 2 * m_nRadius;
	InflateRect(&plotRect, m_nMarginHorizontal, m_nMarginVertical);
	return plotRect;
}

RECT	CTrafficLightImpl::GetBarRect(HDC hDC, RECT plotRect)
{
	RECT barRect = plotRect;
	InflateRect(&barRect, - m_nMarginHorizontal, - m_nMarginVertical);
	return barRect;
}

void	CTrafficLightImpl::DrawLight(HDC hDC, RECT barRect)
{
	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	
	Pen borderPen(Color(GetRValue(m_crBorderColor), GetGValue(m_crBorderColor), GetBValue(m_crBorderColor)), (Gdiplus::REAL)m_nBorderSize);
	borderPen.SetAlignment(PenAlignmentInset);
	double ctx, cty, radius, thick;
	ctx = GetCenterX(barRect);
	cty = GetCenterY(barRect);
	radius = Width(barRect)/2.0 - m_nBorderSize;
	thick = radius/2.0;
	graph.DrawEllipse(&borderPen, float(ctx - radius), float(cty - radius), float(2.0 * radius), float(2.0 * radius));
	SolidBrush bkBrush(Color(GetRValue(m_crBarColor), GetGValue(m_crBarColor), GetBValue(m_crBarColor)));
	graph.FillEllipse(&bkBrush, float(ctx - radius), float(cty - radius), float(2.0 * radius), float(2.0 * radius));

	switch(m_nStatus)
	{
	case kTrafficOff:
		{
			SolidBrush brush(Color(GetRValue(m_crOffColor), GetGValue(m_crOffColor), GetBValue(m_crOffColor)));
			graph.FillEllipse(&brush, float(ctx - radius), float(cty - radius), float(2.0 * radius), float(2.0 * radius));
		}
		break;
	case kTrafficRed:
		{
			SolidBrush brush(Color(255, 0, 0));
			graph.FillEllipse(&brush, float(ctx - radius), float(cty - radius), float(2.0 * radius), float(2.0 * radius));
		}
		break;
	case kTrafficGreen:
		{
			SolidBrush brush(Color(0, 255, 0));
			graph.FillEllipse(&brush, float(ctx - radius), float(cty - radius), float(2.0 * radius), float(2.0 * radius));
		}
		break;
	case kTrafficYellow:
		{
			SolidBrush brush(Color(255, 255, 0));
			graph.FillEllipse(&brush, float(ctx - radius), float(cty - radius), float(2.0 * radius), float(2.0 * radius));
		}
		break;
	case kTrafficLeftRed:
		{
			Pen pen(Color(255, 0, 0), (Gdiplus::REAL)thick);
			pen.SetEndCap(LineCapArrowAnchor);
			graph.DrawLine(&pen, float(ctx + radius), float(cty), float(ctx - radius), float(cty));
		}
		break;
	case kTrafficLeftGreen:
		{
			Pen pen(Color(0, 255, 0), (Gdiplus::REAL)thick);
			pen.SetEndCap(LineCapArrowAnchor);
			graph.DrawLine(&pen, float(ctx + radius), float(cty), float(ctx - radius), float(cty));
		}
		break;
	case kTrafficRightRed:
		{
			Pen pen(Color(255, 0, 0), (Gdiplus::REAL)thick);
			pen.SetEndCap(LineCapArrowAnchor);
			graph.DrawLine(&pen, float(ctx - radius), float(cty), float(ctx + radius), float(cty));
		}
		break;
	case kTrafficRightGreen:
		{
			Pen pen(Color(0, 255, 0), (Gdiplus::REAL)thick);
			pen.SetEndCap(LineCapArrowAnchor);
			graph.DrawLine(&pen, float(ctx - radius), float(cty), float(ctx + radius), float(cty));
		}
		break;
	case kTrafficTurnLeftRed:
		{
//			double s2 = sqrt(2.0)/2.0;
			Pen pen(Color(255, 0, 0), (Gdiplus::REAL)(thick/3.0*2.0));
			pen.SetEndCap(LineCapArrowAnchor);
			GraphicsPath path;
			path.AddLine(float(ctx + radius/4.0), float(cty + radius/4.0*3.0), float(ctx + radius/4.0), float(cty - radius/4.0));
			path.AddLine(float(ctx + radius/4.0), float(cty - radius/4.0), float(ctx - radius/4.0*3.0), float(cty - radius/4.0));
			graph.DrawPath(&pen, &path);

//			pen.SetWidth(thick/3.0);
//			pen.SetEndCap(LineCapNoAnchor);
//			graph.DrawLine(&pen, float(ctx - radius*s2), float(cty - radius*s2), float(ctx + radius*s2), float(cty + radius*s2));
		}
		break;
	case kTrafficTurnLeftGreen:
		{
			double s2 = sqrt(2.0)/2.0;
			Pen pen(Color(0, 255, 0), (Gdiplus::REAL)(thick/3.0*2.0));
			pen.SetEndCap(LineCapArrowAnchor);
			GraphicsPath path;
			path.AddLine(float(ctx + radius/4.0), float(cty + radius/4.0*3.0), float(ctx + radius/4.0), float(cty - radius/4.0));
			path.AddLine(float(ctx + radius/4.0), float(cty - radius/4.0), float(ctx - radius/4.0*3.0), float(cty - radius/4.0));
			graph.DrawPath(&pen, &path);
		}
		break;
	case kTrafficTurnRightRed:
		{
//			double s2 = sqrt(2.0)/2.0;
			Pen pen(Color(255, 0, 0), (Gdiplus::REAL)(thick/3.0*2.0));
			pen.SetEndCap(LineCapArrowAnchor);
			GraphicsPath path;
			path.AddLine(float(ctx - radius/4.0), float(cty + radius/4.0*3.0), float(ctx - radius/4.0), float(cty - radius/4.0));
			path.AddLine(float(ctx - radius/4.0), float(cty - radius/4.0), float(ctx + radius/4.0*3.0), float(cty - radius/4.0));
			graph.DrawPath(&pen, &path);
			
//			pen.SetWidth(thick/3.0);
//			pen.SetEndCap(LineCapNoAnchor);
//			graph.DrawLine(&pen, float(ctx - radius*s2), float(cty - radius*s2), float(ctx + radius*s2), float(cty + radius*s2));
		}
		break;
	case kTrafficTurnRightGreen:
		{
			double s2 = sqrt(2.0)/2.0;
			Pen pen(Color(0, 255, 0), (Gdiplus::REAL)(thick/3.0*2.0));
			pen.SetEndCap(LineCapArrowAnchor);
			GraphicsPath path;
			path.AddLine(float(ctx - radius/4.0), float(cty + radius/4.0*3.0), float(ctx - radius/4.0), float(cty - radius/4.0));
			path.AddLine(float(ctx - radius/4.0), float(cty - radius/4.0), float(ctx + radius/4.0*3.0), float(cty - radius/4.0));
			graph.DrawPath(&pen, &path);
		}
		break;
	case kTrafficNoLeft:
		{
			Pen pen(Color(224, 224, 224), (Gdiplus::REAL)(thick/3.0*2.0));
			pen.SetEndCap(LineCapArrowAnchor);
			GraphicsPath path;
			path.AddLine(float(ctx + radius/4.0), float(cty + radius/4.0*3.0), float(ctx + radius/4.0), float(cty - radius/4.0));
			path.AddLine(float(ctx + radius/4.0), float(cty - radius/4.0), float(ctx - radius/4.0*3.0), float(cty - radius/4.0));
			graph.DrawPath(&pen, &path);
			
			pen.SetEndCap(LineCapNoAnchor);
			pen.SetColor(Color(192, 255, 0, 0));
			pen.SetWidth(float(thick/2.0));
			double s2 = sqrt(2.0)/2.0;
			graph.DrawLine(&pen, float(ctx - radius*s2), float(cty - radius*s2), float(ctx + radius*s2), float(cty + radius*s2));
			
		}
		break;
	case kTrafficNoRight:
		{
			Pen pen(Color(224, 224, 224), (Gdiplus::REAL)(thick/3.0*2.0));
			pen.SetEndCap(LineCapArrowAnchor);
			GraphicsPath path;
			path.AddLine(float(ctx - radius/4.0), float(cty + radius/4.0*3.0), float(ctx - radius/4.0), float(cty - radius/4.0));
			path.AddLine(float(ctx - radius/4.0), float(cty - radius/4.0), float(ctx + radius/4.0*3.0), float(cty - radius/4.0));
			graph.DrawPath(&pen, &path);
			
			pen.SetEndCap(LineCapNoAnchor);
			pen.SetColor(Color(192, 255, 0, 0));
			pen.SetWidth(float(thick/2.0));
			double s2 = sqrt(2.0)/2.0;
			graph.DrawLine(&pen, float(ctx - radius*s2), float(cty - radius*s2), float(ctx + radius*s2), float(cty + radius*s2));
			
		}
		break;
	case kTrafficForwardRed:
		{
			Pen pen(Color(255, 0, 0), (Gdiplus::REAL)thick);
			pen.SetEndCap(LineCapArrowAnchor);
			graph.DrawLine(&pen, float(ctx), float(cty + radius), float(ctx), float(cty - radius));
		}
		break;
	case kTrafficForwardGreen:
		{
			Pen pen(Color(0, 255, 0), (Gdiplus::REAL)thick);
			pen.SetEndCap(LineCapArrowAnchor);
			graph.DrawLine(&pen, float(ctx), float(cty + radius), float(ctx), float(cty - radius));
		}
		break;
	case kTrafficBackwardRed:
		{
			Pen pen(Color(255, 0, 0), (Gdiplus::REAL)(thick/3.0));
//			double s2 = sqrt(2.0)/2.0;
//			graph.DrawLine(&pen, float(ctx - radius*s2), float(cty - radius*s2), float(ctx + radius*s2), float(cty + radius*s2));
			pen.SetEndCap(LineCapArrowAnchor);
			pen.SetWidth(float(thick/2.0));
			GraphicsPath path;
			path.AddLine(float(ctx + radius/3.0), float(cty + radius/3.0*2.0), float(ctx + radius/3.0), float(cty - radius/3.0));
			path.AddArc(float(ctx - radius/3.0), float(cty - radius/3.0*2.0), float(radius/3.0*2.0), float(radius/3.0*2.0), 0, -180);
			path.AddLine(float(ctx - radius/3.0), float(cty - radius/3.0), float(ctx - radius/3.0), float(cty + radius/3.0*2.0));
			graph.DrawPath(&pen, &path);
		}
		break;
	case kTrafficBackwardGreen:
		{
			Pen pen(Color(0, 255, 0), (Gdiplus::REAL)(thick/2.0));
			pen.SetEndCap(LineCapArrowAnchor);
			GraphicsPath path;
			path.AddLine(float(ctx + radius/3.0), float(cty + radius/3.0*2.0), float(ctx + radius/3.0), float(cty - radius/3.0));
			path.AddArc(float(ctx - radius/3.0), float(cty - radius/3.0*2.0), float(radius/3.0*2.0), float(radius/3.0*2.0), 0, -180);
			path.AddLine(float(ctx - radius/3.0), float(cty - radius/3.0), float(ctx - radius/3.0), float(cty + radius/3.0*2.0));
			graph.DrawPath(&pen, &path);
		}
		break;
	case kTrafficNoReturn:
		{
			Pen pen(Color(224, 224, 224), (Gdiplus::REAL)(thick/2.0));
			pen.SetEndCap(LineCapArrowAnchor);
			GraphicsPath path;
			path.AddLine(float(ctx + radius/3.0), float(cty + radius/3.0*2.0), float(ctx + radius/3.0), float(cty - radius/3.0));
			path.AddArc(float(ctx - radius/3.0), float(cty - radius/3.0*2.0), float(radius/3.0*2.0), float(radius/3.0*2.0), 0, -180);
			path.AddLine(float(ctx - radius/3.0), float(cty - radius/3.0), float(ctx - radius/3.0), float(cty + radius/3.0*2.0));
			graph.DrawPath(&pen, &path);

			pen.SetEndCap(LineCapNoAnchor);
			pen.SetColor(Color(192, 255, 0, 0));
			pen.SetWidth(float(thick/2.0));
			double s2 = sqrt(2.0)/2.0;
			graph.DrawLine(&pen, float(ctx - radius*s2), float(cty - radius*s2), float(ctx + radius*s2), float(cty + radius*s2));
			
		}
		break;
	case kTrafficStop:
		{
			Pen pen(Color(255, 0, 0), (Gdiplus::REAL)thick);
			graph.DrawLine(&pen, float(ctx - radius), float(cty), float(ctx + radius), float(cty));
		}
		break;
	case kTrafficError:
		{
			double s2 = sqrt(2.0)/2.0;
			Pen pen(Color(255, 0, 0), (Gdiplus::REAL)(thick/2.0));
			graph.DrawLine(&pen, float(ctx - radius*s2), float(cty - radius*s2), float(ctx + radius*s2), float(cty + radius*s2));
			graph.DrawLine(&pen, float(ctx - radius*s2), float(cty + radius*s2), float(ctx + radius*s2), float(cty - radius*s2));
		}
		break;
	}
	
}