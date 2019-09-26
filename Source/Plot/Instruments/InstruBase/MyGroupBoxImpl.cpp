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

#include "MyGroupBoxImpl.h"
#include "../../Basic/PlotUtility.h"

using namespace NsCChart;

CMyGroupBoxImpl::CMyGroupBoxImpl()
{
}

CMyGroupBoxImpl::~CMyGroupBoxImpl()
{

}

void	CMyGroupBoxImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CMyGroupBoxImpl::OnDraw(HDC hDC, RECT destRect)
{
	RECT plotRect = GetPlotRect(hDC, destRect);
	RECT titleRect = GetTitleRect(hDC, plotRect);

	if(m_bBorder3D)
	{
		if(m_bSunken)
			DrawEdge(hDC, &plotRect, EDGE_SUNKEN, BF_RECT);
		else
			DrawEdge(hDC, &plotRect, EDGE_BUMP, BF_RECT);
	}

	if(m_bShowTitle)
	{
		HFONT hFont, hOldFont;
		hFont = CreateFontIndirect(&m_lfTitleFont);
		hOldFont = (HFONT)SelectObject(hDC, hFont);
		COLORREF oldTextColor = SetTextColor(hDC, m_crTitleColor);
		int	oldBkMode = SetBkMode(hDC, TRANSPARENT);
		
		DrawText(hDC, m_strTitle.c_str(), (int)m_strTitle.length(), &titleRect, DT_LEFT);
		
		SetBkMode(hDC, oldBkMode);
		SetTextColor(hDC, oldTextColor);
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);
	}

	if(!m_bBorder3D)
	{
		HPEN hPen, hOldPen;
		hPen = CreatePen(m_nBorderStyle, m_nBorderSize, m_crBorderColor);
		hOldPen = (HPEN)SelectObject(hDC, hPen);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		int rop2 = SetROP2(hDC, R2_NOTXORPEN);
		
		Rectangle(hDC, plotRect.left, plotRect.top, plotRect.right, plotRect.bottom);

		if(m_bTitleInsect && m_strTitle.length() > 0 && m_bShowTitle)
		{
			MoveToEx(hDC, titleRect.left, plotRect.top, NULL);
			LineTo(hDC, titleRect.right, plotRect.top);
		}

		SetROP2(hDC, rop2);		
		SelectObject(hDC, hOldBrush);
		SelectObject(hDC, hOldPen);
		DeleteObject(hPen);
	}	
}

RECT	CMyGroupBoxImpl::GetPlotRect(HDC hDC, RECT destRect)
{
	RECT plotRect = destRect;
	if(!m_bShowTitle)return plotRect;
	if(m_strTitle.length()<=0)return plotRect;
	
	SIZE titleSize;
	MyGetTextExtentPoint(hDC, &m_lfTitleFont, m_strTitle, DT_LEFT, &titleSize);
	
	if(m_bTitleInsect)plotRect.top += titleSize.cy/2;
	else plotRect.top += titleSize.cy;
	
	return plotRect;
}

RECT	CMyGroupBoxImpl::GetTitleRect(HDC hDC, RECT plotRect)
{
	RECT titleRect = plotRect;

	SIZE titleSize;
	MyGetTextExtentPoint(hDC, &m_lfTitleFont, m_strTitle, DT_LEFT, &titleSize);

	titleRect.left = plotRect.left + m_nTitleDisplaceX;
	titleRect.right = titleRect.left + titleSize.cx;
	
	if(m_bTitleInsect)titleRect.top = plotRect.top - titleSize.cy/2;
	else titleRect.top = plotRect.top - titleSize.cy;

	titleRect.bottom = titleRect.top + titleSize.cy;

	return titleRect;
}
