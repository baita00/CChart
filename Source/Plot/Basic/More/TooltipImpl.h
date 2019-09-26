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

#pragma once

#include "../CChartNameSpace.h"
#include "Tooltip.h"

Declare_Namespace_CChart

template<class PlotImplT>
class	CTooltipImpl : public CTooltip
{
public:
	CTooltipImpl();
	virtual	~CTooltipImpl();

public:
	void	DrawTooltip(HDC hDC, RECT destRect);
	
};

template<class PlotImplT>
CTooltipImpl<PlotImplT>::CTooltipImpl()
{
	
}

template<class PlotImplT>
CTooltipImpl<PlotImplT>::~CTooltipImpl()
{
	
}

template<class PlotImplT>
void	CTooltipImpl<PlotImplT>::DrawTooltip(HDC hDC, RECT destRect)
{
	if(CGlobal::m_bEnableToolTip && m_bShowTooltip && m_strTooltip.length()>0)
	{
		if(PtInRect(&destRect, m_pointTooltip))
		{
			RECT tooltipRect = GetTextRect(hDC, m_pointTooltip, m_strTooltip, &m_lfTooltipFont, true, &destRect, false);
			if(m_bTooltipRectShow)
			{
				HPEN hPen, hOldPen;
				HBRUSH hBrush, hOldBrush;
				hPen = CreatePen(m_nTooltipRectLineStyle, m_nTooltipRectLineSize, m_crTooltipRect);
				hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				hOldPen = (HPEN)SelectObject(hDC, hPen);
				hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				
				Rectangle(hDC, tooltipRect.left, tooltipRect.top,tooltipRect.right, tooltipRect.bottom);
				
				SelectObject(hDC, hOldPen);
				SelectObject(hDC, hOldBrush);
				DeleteObject(hPen);
			}
			DrawTextInRect(hDC, tooltipRect, m_strTooltip, &m_lfTooltipFont, TRANSPARENT, m_crTooltip, DT_LEFT|DT_BOTTOM);
		}
	}
}


Declare_Namespace_End