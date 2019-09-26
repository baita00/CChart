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

#include "RulerImpl.h"

using namespace NsCChart;

CRulerImpl::CRulerImpl()
{
	SetRangeOnly(0, m_nRulerLength);
	SetTickCount(m_nRulerLength-1);
	SetMinorTickCount(10);
	SetShowTitle(false);
	SetBarWidth(0);
	SetTickSize(3);
	SetSubTickSize(2);
	SetTickLength(36);
	SetSubTickLength(18);
	SetMinorTickLength(12);
	SetBorder3D(true);
	SetBorder3dEdge(EDGE_RAISED);
	SetShowSubMajorTicks(true);

	GetLabelFont().lfHeight -= 2;
}

CRulerImpl::~CRulerImpl()
{

}

void	CRulerImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CRulerImpl::OnDraw(HDC hDC, RECT destRect)
{
	bool bShowLabel = m_bShowTickLabel;;
	if(m_bDodgeLabel)
	{
		m_bShowTickLabel = false;;
	}
	if(m_bLowerSide)
	{
		m_nAxisPosition = kInstruPositionWideSmall;
	}
	else
	{
		m_nAxisPosition = kInstruPositionWideLarge;
	}
	SetRangeOnly(0, m_nRulerLength);
	SetTickCount(m_nRulerLength-1);
	RECT plotRect = GetPlotRect(hDC, destRect);
	RECT barRect = GetBarRect(hDC, plotRect);
	RECT axisRect = GetAxisRect(hDC, barRect);
	
	DrawBorder(hDC, plotRect);
	DrawAxis(hDC, barRect);

	if(m_bDodgeLabel)
	{
		m_bShowTickLabel = bShowLabel;
		DrawDodgedLabels(hDC, axisRect, barRect);
	}
	
}

void	CRulerImpl::DrawDodgedLabels(HDC hDC, RECT axisRect, RECT barRect)
{
	if(!m_bShowTickLabel)return;
	
	int margin = m_nBarMargin;
	if(margin > m_nBarWidth/3) margin = m_nBarWidth/3;

	//RECT axisRect = GetAxisRect(hDC, barRect);
	
	HFONT hFont, hOldFont;
	hFont = CreateFontIndirect(&m_lfLabelFont);
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	
	int i;
	double step;
	tstring label;
	RECT textRect;
	SIZE labelSize;

	SetTextColor(hDC, m_crTickLabelColor);
	SetBkMode(hDC, TRANSPARENT);
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		step = Width(axisRect)/(m_nTickCount+1.0);
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
			for(i=0; i<=m_nTickCount+1; i++)
			{
				label = GetMajorTickLabel(i);
				MyGetTextExtentPoint(hDC, &m_lfLabelFont, label, DT_CENTER, &labelSize);
				textRect.top = axisRect.bottom - GetMaxTickLength();
				textRect.bottom = textRect.top + labelSize.cy;
				if(!m_bOpposite)
				{
					textRect.left = long(axisRect.left + i*step) + 3;
					if(i==m_nTickCount+1)
					{
						textRect.left  -= (labelSize.cx+6);
					}
				}
				else
				{
					textRect.left = long(axisRect.right - i*step) - 3 -  labelSize.cx;
					if(i==m_nTickCount+1)
					{
						textRect.left  += (labelSize.cx+6);
					}
				}
				textRect.right = textRect.left + labelSize.cx;
				
				DrawText(hDC, label.c_str(), (int)label.length(), &textRect, DT_CENTER);
			}
			break;
		case kInstruPositionWideLarge:
			for(i=0; i<=m_nTickCount+1; i++)
			{
				label = GetMajorTickLabel(i);
				MyGetTextExtentPoint(hDC, &m_lfLabelFont, label, DT_CENTER, &labelSize);
				textRect.bottom = axisRect.top + GetMaxTickLength();
				textRect.top = textRect.bottom - labelSize.cy;
				if(!m_bOpposite)
				{
					textRect.left = long(axisRect.left + i*step) + 3;
					if(i==m_nTickCount+1)
					{
						textRect.left  -= (labelSize.cx+6);
					}
				}
				else
				{
					textRect.left = long(axisRect.right - i*step) - 3 -  labelSize.cx;
					if(i==m_nTickCount+1)
					{
						textRect.left  += (labelSize.cx+6);
					}
				}
				textRect.right = textRect.left + labelSize.cx;
				
				DrawText(hDC, label.c_str(), (int)label.length(), &textRect, DT_CENTER);
			}
			break;
		}
		break;
	case kInstruShapeVertical:
		step = Height(axisRect)/(m_nTickCount+1.0);
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
			for(i=0; i<=m_nTickCount+1; i++)
			{
				label = GetMajorTickLabel(i);
				MyGetTextExtentPoint(hDC, &m_lfLabelFont, label, DT_RIGHT, &labelSize);
				textRect.left = axisRect.right - GetMaxTickLength();
				textRect.right = textRect.left + labelSize.cx;
				if(!m_bOpposite)
				{
					textRect.bottom = long(axisRect.bottom - i*step) - 3;
					if(i==m_nTickCount+1)
					{
						textRect.bottom  += (labelSize.cy+6);
					}
				}
				else
				{
					textRect.bottom = long(axisRect.top + i*step) + 3 + labelSize.cy;
					if(i==m_nTickCount+1)
					{
						textRect.bottom  -= (labelSize.cy+6);
					}
				}
				textRect.top = textRect.bottom - labelSize.cy;
				
				DrawText(hDC, label.c_str(), (int)label.length(), &textRect, DT_RIGHT);
			}
			break;
		case kInstruPositionWideLarge:
			for(i=0; i<=m_nTickCount+1; i++)
			{
				label = GetMajorTickLabel(i);
				MyGetTextExtentPoint(hDC, &m_lfLabelFont, label, DT_LEFT, &labelSize);
				textRect.right = axisRect.left + GetMaxTickLength();
				textRect.left = textRect.right - labelSize.cx;
				if(!m_bOpposite)
				{
					textRect.bottom = long(axisRect.bottom - i*step) - 3;
					if(i==m_nTickCount+1)
					{
						textRect.bottom  += (labelSize.cy+6);
					}
				}
				else
				{
					textRect.bottom = long(axisRect.top + i*step) + 3 + labelSize.cy;
					if(i==m_nTickCount+1)
					{
						textRect.bottom  -= (labelSize.cy+6);
					}
				}
				textRect.top = textRect.bottom - labelSize.cy;
				
				DrawText(hDC, label.c_str(), (int)label.length(), &textRect, DT_LEFT);
				
				DrawText(hDC, label.c_str(), (int)label.length(), &textRect, DT_CENTER);
			}
			break;
		}
		break;
	}
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}

