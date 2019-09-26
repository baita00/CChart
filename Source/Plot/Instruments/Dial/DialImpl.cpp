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

#include "DialImpl.h"

using namespace NsCChart;

CDialImpl::CDialImpl()
{
	m_nInstruShape = kInstruShapeCircle;

	m_nTickLength = 15;
	m_nMinorTickLength = 10;

	m_bBorder3D = true;
}

CDialImpl::~CDialImpl()
{

}

void	CDialImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CDialImpl::OnDraw(HDC hDC, RECT destRect)
{
	RECT plotRect = GetPlotRect(hDC, destRect);
	RECT barRect = GetBarRect(hDC, plotRect);

	DrawBorder(hDC, plotRect);
	DrawTitle(hDC, barRect, plotRect);
	DrawWholeCircleFilling(hDC, barRect);
	DrawBar(hDC, barRect);
	DrawAxis(hDC, barRect);
	DrawSubTitle(hDC, barRect);
	DrawPointer(hDC, barRect);
}

RECT	CDialImpl::GetSubTitleRect(HDC hDC, RECT barRect)
{
	RECT subtitleRect = barRect;
	RECT nullRect = {0, 0, 0, 0};

	tstring subtitle;
	subtitle = GetRealSubTitle();
	
	if(!m_bShowSubTitle || subtitle.length() <= 0)return nullRect;

	SIZE subtitleSize;
	MyGetTextExtentPoint(hDC, &m_lfSubTitleFont, subtitle, DT_LEFT, &subtitleSize);

	double ctx, cty;
	ctx = GetCenterX(barRect);
	cty = GetCenterY(barRect);

	subtitleRect.top = long(cty);
	subtitleRect.bottom = long(cty + subtitleSize.cy);
	subtitleRect.left = long(ctx - subtitleSize.cx/2.0);
	subtitleRect.right = subtitleRect.left + subtitleSize.cx;

	OffsetRect(&subtitleRect, m_sizeSubTitleOffset.cx, m_sizeSubTitleOffset.cy);
	return subtitleRect;
}

void	CDialImpl::DrawSubTitle(HDC hDC, RECT barRect)
{
	tstring subtitle;
	subtitle = GetRealSubTitle();
	if(!m_bShowSubTitle || subtitle.length()<=0)return;

	RECT subtitleRect = GetSubTitleRect(hDC, barRect);

	if(m_bFillSubTitleRect)
	{
		EraseRect(hDC, subtitleRect, m_crSubTitleRectColor);
	}

	HFONT hFont, hOldFont;
	hFont = CreateFontIndirect(&m_lfSubTitleFont);
	hOldFont = (HFONT)SelectObject(hDC, hFont);

	int oldMode = SetBkMode(hDC, TRANSPARENT);
	COLORREF oldColor = SetTextColor(hDC, m_crSubTitleColor);

	DrawText(hDC,subtitle.c_str(), (int)subtitle.length(), &subtitleRect, DT_LEFT);

	SetTextColor(hDC, oldColor);
	SetBkMode(hDC, oldMode);
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}


tstring		CDialImpl::GetRealSubTitle()
{
	if(!m_bSubTitleAsValue)return m_strSubTitle;
	
	tstring realtitle;
	
	tostringstream ostr;
	ostr<<GetCurrValue();
	realtitle = ostr.str();
	if(m_bShowUnit)
		realtitle += (_TEXT(" ") + m_strUnit);
	return realtitle;
	
}