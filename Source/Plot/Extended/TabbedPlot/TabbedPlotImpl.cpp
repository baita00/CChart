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
#include "TabbedPlotImpl.h"

using namespace NsCChart;

CTabbedPlotImpl::CTabbedPlotImpl()
{

}

CTabbedPlotImpl::~CTabbedPlotImpl()
{

}

void	CTabbedPlotImpl::DrawTabBar(HDC hDC, RECT destRect)
{	
	int i;

	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush, hCurBursh;
	hPen = CreatePen(PS_SOLID, 1, m_crTabEdgeColor);
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hBrush = CreateSolidBrush(m_crTabColor);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	hCurBursh = CreateSolidBrush(m_crCurTabColor);
	for(i=0; i<(int)m_vstrTabTitle.size(); i++)
	{
		if(i==m_nCurTab)
		{
			SelectObject(hDC, hCurBursh);
		}
		RECT rt = GetTabRect(i, hDC, destRect);
		Rectangle(hDC, rt.left, rt.top, rt.right, rt.bottom);
		if(i==m_nCurTab)
		{
			SelectObject(hDC, hBrush);
		}
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	DeleteObject(hCurBursh);
	
	for(i=0; i<(int)m_vstrTabTitle.size(); i++)
	{
		DrawTextInRect(hDC, GetTabRect(i, hDC, destRect), m_vstrTabTitle[i], &m_lfTabFont, TRANSPARENT, m_crTabTitleColor);
	}
	
}

void	CTabbedPlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	SetLastClientRect(destRect);
	SetLastPlotRect(destRect);
	
	DrawTabBar(hDC, destRect);
	SIZE barSize = GetTabBarSize(hDC);
	destRect.top += (barSize.cy + m_nTabMarginV);

	if(m_nCurTab<0 || m_nCurTab>=(int)m_vpPlots.size())return;

	m_vpPlots[m_nCurTab]->FinalDraw(hDC, destRect);
}

SIZE	CTabbedPlotImpl::GetTabBarSize( HDC hDC )
{
	SIZE barSize = {0, 0};

	SIZE titleSize;

	int i;
	for(i=0; i<(int)m_vstrTabTitle.size(); i++)
	{
		MyGetTextExtentPoint(hDC, &m_lfTabFont, m_vstrTabTitle[i].length()>0?m_vstrTabTitle[i]:_TEXT("Noname"), TRANSPARENT, &titleSize);

		barSize.cy = max(barSize.cy, titleSize.cy);
		barSize.cx += titleSize.cx + 2*m_nTabTitleMargin;
	}

	return barSize;
}

RECT	CTabbedPlotImpl::GetTabRect(int nIndex, HDC hDC, RECT destRect)
{
	RECT tabRect = {0, 0, 0, 0};

	if(nIndex<0 || nIndex>=(int)m_vstrTabTitle.size())return tabRect;

	SIZE titleSize;

	tabRect = destRect;
	tabRect.left += m_nTabMarginH;
	tabRect.top += m_nTabMarginV;
	tabRect.bottom = tabRect.top;

	int i;
	for(i=0; i<=nIndex; i++)
	{
		MyGetTextExtentPoint(hDC, &m_lfTabFont, m_vstrTabTitle[i].length()>0?m_vstrTabTitle[i]:_TEXT("Noname"), TRANSPARENT, &titleSize);
		if(i>0)tabRect.left = tabRect.right;
		tabRect.bottom = max(tabRect.bottom, tabRect.top + titleSize.cy);
		tabRect.right = tabRect.left + titleSize.cx + 2*m_nTabTitleMargin;
	}

	return tabRect;
}