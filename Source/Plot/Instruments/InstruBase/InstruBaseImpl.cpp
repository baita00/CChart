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
#include "InstruBaseImpl.h"
#include <math.h>

using namespace NsCChart;

CInstruBaseImpl::CInstruBaseImpl()
{

}

CInstruBaseImpl::~CInstruBaseImpl()
{

}

SIZE	CInstruBaseImpl::GetTitleSize(HDC hDC)
{
	SIZE		titleSize={0,0};
	tstring		realtitle = GetRealTitle();
	if( m_bShowTitle && realtitle != _TEXT(""))
	{
		MyGetTextExtentPoint(hDC, &m_lfTitleFont, realtitle, DT_LEFT, &titleSize);
	}
	return titleSize;
}

SIZE	CInstruBaseImpl::GetMaxSingleTickLabelSize(HDC hDC)
{
	SIZE		temp={0,0}, temp2={0,0};
	int			i;
	
	tstring strLabel;
	for( i = 0; i <= GetTickCount()+1; i++ )
	{
		strLabel = GetMajorTickLabel(i);
		MyGetTextExtentPoint(hDC, &m_lfLabelFont, strLabel, DT_LEFT, &temp);
		if(temp.cx > temp2.cx) temp2.cx = temp.cx;
		if(temp.cy > temp2.cy) temp2.cy = temp.cy;
	}

	return temp2;
}

int		CInstruBaseImpl::GetMaxTickLength()
{
	return max(m_nTickLength, max(m_nSubTickLength, m_nMinorTickLength));
}

SIZE	CInstruBaseImpl::GetPaddingSize( HDC hDC )
{
	SIZE		temp1={0,0}, temp2={0,0};
	
	if( GetTickCount() > 0 )
	{
		int			i;
		
		tstring strLabel;
		// Get info for first and last tick marks
		for( i = 0; i <= GetTickCount()+1; i+= GetTickCount()+1 )
		{
			strLabel = GetMajorTickLabel(i);
			//GetTextExtentPoint(hDC,strLabel.c_str(),strLabel.length(),&temp1);
			MyGetTextExtentPoint(hDC, &m_lfLabelFont, strLabel, 0,&temp1);
			switch( m_nInstruShape )
			{
			case kInstruShapeHorizontal:
				if( i == 0 ) temp2.cx = temp1.cx / 2; else temp2.cy = temp1.cx / 2;
				break;
			case kInstruShapeVertical:
				if( i == 0 ) temp2.cx = temp1.cy / 2; else temp2.cy = temp1.cy / 2;
				break;
			}
		}
	}
	return temp2;
}

tstring		CInstruBaseImpl::GetMajorTickLabel( int whichTick )
{
	tstring			tickLabel;

	if(!m_bReplaceLabel)
	{
		double			value = GetMajorTickPosValue( whichTick );
		tostringstream ostr;
		if(!m_bLogarithm)
			ostr<<value;
		else
			ostr<<pow(10.0, value);
		tickLabel = ostr.str();
	}
	else
	{
		if( whichTick < (int)m_vstrAlternateLabels.size() )
			tickLabel = m_vstrAlternateLabels[whichTick];
		else
			tickLabel = _TEXT("");
	}

	return tickLabel;
}

double		CInstruBaseImpl::GetMajorTickPosValue( int whichTick )
{
	double		range,pos,epsl=1.0e-12;
	
	// Assume user has checked the tick values first
	if( whichTick < 0 || 
		( (GetTickCount() > 0 && whichTick > GetTickCount()+1) ) )
		return 0.0;
	
	range = m_pfRange[1] - m_pfRange[0];
	
	if( GetTickCount() > 0 )
	{
		// User has specified number of ticks
		pos = m_pfRange[0] + range * (double)whichTick/((double)GetTickCount()+1.0);
		if(range!=0.0)
		{
			if(fabs(pos/range)<epsl)pos=0.0;
		}
	}
	else
	{
		// For now, just use four regions
		pos = m_pfRange[0] + range * (double)whichTick/((double)5.0);
		if(range!=0.0)
		{
			if(fabs(pos/range)<epsl)pos=0.0;
		}
	}
	
	return pos;
}

bool	CInstruBaseImpl::GetMinorTickPosValue( int whichMajor, double *fPos )
{	
	// Assume user has checked the tick values first
	if( whichMajor < 0 ||
		( (GetTickCount() > 0 && whichMajor > GetTickCount()) ) )
		return false;
	if( GetMinorTickCount() <=0 )
		return false;
	
	double pos1, pos2;
	pos1 = GetMajorTickPosValue(whichMajor);
	pos2 = GetMajorTickPosValue(whichMajor+1);
	
	int i;
	if(!m_bLogarithm)
	{
		for(i=0; i<=GetMinorTickCount(); i++)
		{
			fPos[i] = pos1 + i * (pos2 - pos1)/GetMinorTickCount();
		}
	}
	else
	{
		for(i=0; i<=GetMinorTickCount(); i++)
		{
			fPos[i] = log10( pow(10, pos1) + i * ( pow(10, pos2)-pow(10, pos1) )/GetMinorTickCount() );
		}
	}
	
	return true;
}

RECT	CInstruBaseImpl::GetPlotRect(HDC hDC, RECT destRect)
{
	RECT plotRect = destRect;
	RECT nullRect = {0, 0, 0, 0};
	
	SIZE titleSize = GetTitleSize(hDC);
	SIZE labelSize = GetMaxSingleTickLabelSize(hDC);
	int	tickLength = GetMaxTickLength();
	
	int margin = m_nBarMargin;
	if(margin > m_nBarWidth/3) margin = m_nBarWidth/3;
	
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		plotRect.bottom = plotRect.top;
		plotRect.bottom += m_nMarginVertical;
		plotRect.bottom += m_nBarWidth;
		plotRect.bottom += margin;
		if(m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionWideLarge)
		{
			if(m_bShowTitle)
				plotRect.bottom += titleSize.cy;
		}
		if(m_bShowAxis)
		{
			if(m_bShowTickLabel)
				plotRect.bottom += labelSize.cy;
			plotRect.bottom += tickLength;
		}
		
		plotRect.bottom += m_nMarginVertical;
		break;
	case kInstruShapeVertical:
		plotRect.right = plotRect.left;
		plotRect.right += m_nMarginHorizontal;
		plotRect.right += m_nBarWidth;
		plotRect.right += margin;
		if(m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionWideLarge)
		{
			if(m_bShowTitle)
				plotRect.right += titleSize.cy;
		}
		if(m_bShowAxis)
		{
			if(m_bShowTickLabel)
				plotRect.right += labelSize.cx;
			plotRect.right += tickLength;
		}
		
		plotRect.right += m_nMarginVertical;
		break;
	case kInstruShapeCircle:
		RECT barRect;
		barRect = plotRect;
		
		if(m_bShowTitle)
		{
			if(m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionNarrowSmall)
			{
				barRect.top += titleSize.cy;
			}
			else
			{
				barRect.bottom -= titleSize.cy;
			}
		}
		
		InflateRect(&barRect, -m_nMarginHorizontal, -m_nMarginHorizontal);
		if(Width(barRect) > Height(barRect))barRect.right = barRect.left + Height(barRect);
		if(Width(barRect) < Height(barRect))barRect.bottom = barRect.top + Width(barRect);
		InflateRect(&barRect, m_nMarginHorizontal, m_nMarginHorizontal);
		
		plotRect = barRect;
		if(m_bShowTitle)
		{
			if(m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionNarrowSmall)
			{
				plotRect.top -= titleSize.cy;
			}
			else
			{
				plotRect.bottom += titleSize.cy;
			}
		}
		break;
	case kInstruShapeWheel:
		InflateRect(&plotRect, -m_nMarginHorizontal, -m_nMarginVertical);
		if(m_bWheelHorizontal)
		{
			plotRect.bottom = plotRect.top + m_nWheelThick;
		}
		else
		{
			plotRect.right = plotRect.left + m_nWheelThick;
		}
		break;
	}
	
	return plotRect;
}

RECT	CInstruBaseImpl::GetBarRect(HDC hDC, RECT plotRect)
{
	RECT barRect = plotRect;
	RECT nullRect = {0, 0, 0, 0};

	SIZE titleSize = GetTitleSize(hDC);
	SIZE labelSize = GetMaxSingleTickLabelSize(hDC);
	int	tickLength = GetMaxTickLength();
	SIZE paddingSize = GetPaddingSize(hDC);

	int margin = m_nBarMargin;
	if(margin > m_nBarWidth/3) margin = m_nBarWidth/3;

	InflateRect(&barRect, -m_nMarginHorizontal, -m_nMarginVertical);
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		switch(m_nTitlePosition)
		{
		case kInstruPositionWideSmall:
			if(m_bShowTitle)barRect.top += (titleSize.cy + margin);
			if(m_bShowAxis)
			{
				if(m_nAxisPosition == kInstruPositionWideSmall)
				{
					barRect.top += tickLength;
					if(m_bShowTickLabel)barRect.top += labelSize.cy;
				}
			}
			
			barRect.bottom = barRect.top + m_nBarWidth;

			if(m_bOpposite)swap(paddingSize.cx, paddingSize.cy);
			barRect.left += paddingSize.cx;
			barRect.right -= paddingSize.cy;
			break;
		case kInstruPositionWideLarge:
			if(m_bShowAxis)
			{
				if(m_nAxisPosition == kInstruPositionWideSmall)
				{
					barRect.top += tickLength;
					if(m_bShowTickLabel)barRect.top += labelSize.cy;
				}
			}

			barRect.bottom = barRect.top + m_nBarWidth;
			
			if(m_bOpposite)swap(paddingSize.cx, paddingSize.cy);
			barRect.left += paddingSize.cx;
			barRect.right -= paddingSize.cy;
			break;
		case kInstruPositionNarrowSmall:
			if(m_bShowAxis)
			{
				if(m_nAxisPosition == kInstruPositionWideSmall)
				{
					barRect.top += tickLength;
					if(m_bShowTickLabel)barRect.top += labelSize.cy;
				}
			}
			
			barRect.bottom = barRect.top + m_nBarWidth;
			
			if(m_bShowTitle)barRect.left += (titleSize.cx + margin);

			if(m_bOpposite)swap(paddingSize.cx, paddingSize.cy);
			barRect.left += paddingSize.cx;
			barRect.right -= paddingSize.cy;
			break;
		case kInstruPositionNarrowLarge:
			if(m_bShowAxis)
			{
				if(m_nAxisPosition == kInstruPositionWideSmall)
				{
					barRect.top += tickLength;
					if(m_bShowTickLabel)barRect.top += labelSize.cy;
				}
			}
			
			barRect.bottom = barRect.top + m_nBarWidth;
			
			if(m_bOpposite)swap(paddingSize.cx, paddingSize.cy);
			barRect.left += paddingSize.cx;
			if(m_bShowTitle)barRect.right -= (titleSize.cx + margin);
			barRect.right -= paddingSize.cy;
			break;
		default:
			return nullRect;
		}
		break;
	case kInstruShapeVertical:
		switch(m_nTitlePosition)
		{
		case kInstruPositionWideSmall:
			if(m_bShowTitle)barRect.left += (titleSize.cy + margin);
			if(m_bShowAxis)
			{
				if(m_nAxisPosition == kInstruPositionWideSmall)
				{
					barRect.left += tickLength;
					if(m_bShowTickLabel)barRect.left += labelSize.cx;
				}
			}
			
			barRect.right = barRect.left + m_nBarWidth;
			
			if(m_bOpposite)swap(paddingSize.cx, paddingSize.cy);
			barRect.top += paddingSize.cx;
			barRect.bottom -= paddingSize.cy;
			break;
		case kInstruPositionWideLarge:
			if(m_bShowAxis)
			{
				if(m_nAxisPosition == kInstruPositionWideSmall)
				{
					barRect.left += tickLength;
					if(m_bShowTickLabel)barRect.left += labelSize.cx;
				}
			}
			
			
			barRect.right = barRect.left + m_nBarWidth;
			
			if(m_bOpposite)swap(paddingSize.cx, paddingSize.cy);
			barRect.top += paddingSize.cx;
			barRect.bottom -= paddingSize.cy;
			break;
		case kInstruPositionNarrowSmall:
			if(m_bShowAxis)
			{
				if(m_nAxisPosition == kInstruPositionWideSmall)
				{
					barRect.left += tickLength;
					if(m_bShowTickLabel)barRect.left += labelSize.cx;
				}
			}
			
			barRect.right = barRect.left + m_nBarWidth;

			if(m_bOpposite)swap(paddingSize.cx, paddingSize.cy);
			if(m_bShowTitle)barRect.top += (titleSize.cy + margin);
			barRect.top += paddingSize.cx;
			barRect.bottom -= paddingSize.cy;
			break;
		case kInstruPositionNarrowLarge:
			if(m_bShowAxis)
			{
				if(m_nAxisPosition == kInstruPositionWideSmall)
				{
					barRect.left += tickLength;
					if(m_bShowTickLabel)barRect.left += labelSize.cx;
				}
			}
			
			barRect.right = barRect.left + m_nBarWidth;
			
			if(m_bOpposite)swap(paddingSize.cx, paddingSize.cy);
			barRect.top += paddingSize.cx;
			if(m_bShowTitle)barRect.bottom -= (titleSize.cy + margin);
			barRect.bottom -= paddingSize.cy;
			break;
		default:
			return nullRect;
		}
		break;
	case kInstruShapeCircle:
		if(m_bShowTitle)
		{
			if(m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionNarrowSmall)
			{
				barRect.top += titleSize.cy;
			}
			else
			{
				barRect.bottom -= titleSize.cy;
			}
		}
		break;
	case kInstruShapeWheel:
		InflateRect(&barRect, -2, -2);
		break;
	}

	return barRect;
}

RECT	CInstruBaseImpl::GetTitleRect(HDC hDC, RECT barRect, RECT plotRect)
{
	SIZE titleSize = GetTitleSize(hDC);
	SIZE labelSize = GetMaxSingleTickLabelSize(hDC);
	SIZE paddingSize = GetPaddingSize(hDC);
	int	tickLength = GetMaxTickLength();
	RECT titleRect = barRect;
	RECT nullRect = {0, 0, 0, 0};

	int margin = m_nBarMargin;
	if(margin > m_nBarWidth/3) margin = m_nBarWidth/3;
	
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		switch(m_nTitlePosition)
		{
		case kInstruPositionWideSmall:
			titleRect.bottom = barRect.top - margin;
			if(m_bShowAxis)
			{
				if(m_nAxisPosition == kInstruPositionWideSmall)
				{
					titleRect.bottom -= tickLength;
					if(m_bShowTickLabel)
					{
						titleRect.bottom -= labelSize.cy;
					}
				}
			}
			
			titleRect.top = titleRect.bottom - titleSize.cy;
			break;
		case kInstruPositionWideLarge:
			titleRect.top = barRect.bottom + margin;
			if(m_bShowAxis)
			{
				if(m_nAxisPosition == kInstruPositionWideLarge)
				{
					titleRect.top += tickLength;
					if(m_bShowTickLabel)
					{
						titleRect.top += labelSize.cy;
					}
				}
			}
			
			titleRect.bottom = titleRect.top + titleSize.cy;
			break;
		case kInstruPositionNarrowSmall:
			titleRect.right = barRect.left - margin - paddingSize.cx;
			titleRect.left = titleRect.right - titleSize.cx;
			titleRect.top = long( GetCenterY(plotRect) - titleSize.cy/2.0);
			titleRect.bottom = titleRect.top + titleSize.cy;
			break;
		case kInstruPositionNarrowLarge:
			titleRect.left = barRect.right + margin + paddingSize.cy;
			titleRect.right = titleRect.left + titleSize.cx;
			titleRect.top = long( GetCenterY(plotRect) - titleSize.cy/2.0);
			titleRect.bottom = titleRect.top + titleSize.cy;
			break;
		default:
			return nullRect;
		}
		break;
	case kInstruShapeVertical:
		switch(m_nTitlePosition)
		{
		case kInstruPositionWideSmall:
			titleRect.right = barRect.left - margin;
			if(m_bShowAxis)
			{
				if(m_nAxisPosition == kInstruPositionWideSmall)
				{
					titleRect.right -= tickLength;
					if(m_bShowTickLabel)
					{
						titleRect.right -= labelSize.cx;
					}
				}
			}
			
			titleRect.left = titleRect.right - titleSize.cy;
			titleRect.top = long( (barRect.top + barRect.bottom)/2.0 - titleSize.cx/2.0);
			titleRect.bottom = titleRect.top + titleSize.cx;
			break;
		case kInstruPositionWideLarge:
			titleRect.left = barRect.right + margin;
			if(m_bShowAxis)
			{
				if(m_nAxisPosition == kInstruPositionWideLarge)
				{
					titleRect.left += tickLength;
					if(m_bShowTickLabel)
					{
						titleRect.left += labelSize.cx;
					}
					
				}
			}
			
			titleRect.right = titleRect.left + titleSize.cy;
			titleRect.top = long( (barRect.top + barRect.bottom)/2.0 - titleSize.cx/2.0);
			titleRect.bottom = titleRect.top + titleSize.cx;
			break;
		case kInstruPositionNarrowSmall:
			titleRect.bottom = barRect.top - margin - paddingSize.cy;
			titleRect.top = titleRect.bottom - titleSize.cy;
			titleRect.left = long( GetCenterX(plotRect) - titleSize.cx/2.0);
			titleRect.right = titleRect.left + titleSize.cx;
			break;
		case kInstruPositionNarrowLarge:
			titleRect.top = barRect.bottom + margin + paddingSize.cx;
			titleRect.bottom = titleRect.top + titleSize.cy;
			titleRect.left = long( GetCenterX(plotRect) - titleSize.cx/2.0);
			titleRect.right = titleRect.left + titleSize.cx;
			break;
		default:
			return nullRect;
		}
		break;
	case kInstruShapeCircle:
		switch(m_nTitlePosition)
		{
		case kInstruPositionWideSmall:
		case kInstruPositionNarrowSmall:
			titleRect.bottom = barRect.top;
			titleRect.top = titleRect.bottom - titleSize.cy;
			break;
		case kInstruPositionWideLarge:
		case kInstruPositionNarrowLarge:
			titleRect.top = barRect.bottom;
			titleRect.bottom = titleRect.top + titleSize.cy;
			break;
		}
		break;
	default:
		return nullRect;
	}
	
	OffsetRect(&titleRect, m_sizeTitleOffset.cx, m_sizeTitleOffset.cy);
	
	return titleRect;
}

RECT	CInstruBaseImpl::GetAxisRect(HDC hDC, RECT barRect)
{
	SIZE labelSize = GetMaxSingleTickLabelSize(hDC);
	RECT axisRect = barRect;
	RECT nullRect = {0, 0, 0, 0};
	
	int margin = m_nBarMargin;
	if(margin > m_nBarWidth/3) margin = m_nBarWidth/3;
	
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
		case kInstruPositionNarrowSmall:
			axisRect.bottom = barRect.top - margin;
			axisRect.top = axisRect.bottom - GetMaxTickLength();
			if(m_bShowTickLabel)axisRect.top -= labelSize.cy;
			break;
		case kInstruPositionWideLarge:
		case kInstruPositionNarrowLarge:
			axisRect.top = barRect.bottom + margin;
			axisRect.bottom = axisRect.top + GetMaxTickLength();
			if(m_bShowTickLabel)axisRect.bottom += labelSize.cy;
			break;
		default:
			return nullRect;
		}
		break;
	case kInstruShapeVertical:
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
		case kInstruPositionNarrowSmall:
			axisRect.right = barRect.left - margin;
			axisRect.left = axisRect.right - GetMaxTickLength();
			if(m_bShowTickLabel)axisRect.left -= labelSize.cx;
			break;
		case kInstruPositionWideLarge:
		case kInstruPositionNarrowLarge:
			axisRect.left = barRect.right + margin;
			axisRect.right = axisRect.left + GetMaxTickLength();
			if(m_bShowTickLabel)axisRect.right += labelSize.cx;
			break;
		default:
			return nullRect;
		}
		break;
	case kInstruShapeCircle:
		axisRect = barRect;
		InflateRect(&axisRect, -m_nEdgeGap, -m_nEdgeGap);
		if(m_bShowBar)
			InflateRect(&axisRect, -m_nBarWidth, -m_nBarWidth);

		if(m_bShowTickLabel && m_nLabelPosition == kInstruPosOuter)
		{
			InflateRect(&axisRect, -max(labelSize.cx, labelSize.cy), -max(labelSize.cx, labelSize.cy));
		}

		double tickL;
		tickL = GetSideTickLengh(true);
		InflateRect(&axisRect, int(-tickL), int(-tickL));
		
		break;
	}

	return axisRect;
}

double	CInstruBaseImpl::GetSideTickLengh(bool outer)
{
	int tickL = 0, mtickL = 0;
	if(outer)
	{
		if(m_nTickPosition == kInstruPosOuter || m_nTickPosition == kInstruPosBoth
			|| m_nMinorTickPosition == kInstruPosOuter || m_nMinorTickPosition == kInstruPosBoth)
		{
			
			if(m_nTickPosition == kInstruPosOuter)tickL = m_nTickLength;
			else if(m_nTickPosition == kInstruPosBoth)tickL = m_nTickLength/2;
			if(m_nMinorTickPosition == kInstruPosOuter)mtickL = m_nMinorTickLength;
			else if(m_nMinorTickPosition == kInstruPosBoth)mtickL = m_nMinorTickLength/2;
		}
	}
	else
	{
		if(m_nTickPosition == kInstruPosInner || m_nTickPosition == kInstruPosBoth
			|| m_nMinorTickPosition == kInstruPosInner || m_nMinorTickPosition == kInstruPosBoth)
		{
			
			if(m_nTickPosition == kInstruPosInner)tickL = m_nTickLength;
			else if(m_nTickPosition == kInstruPosBoth)tickL = m_nTickLength/2;
			if(m_nMinorTickPosition == kInstruPosInner)mtickL = m_nMinorTickLength;
			else if(m_nMinorTickPosition == kInstruPosBoth)mtickL = m_nMinorTickLength/2;
		}
	}
	return max(tickL, mtickL);
}

void	CInstruBaseImpl::DrawBorder(HDC hDC, RECT plotRect)
{
	if(!m_bShowBorder)return;
	
	HPEN hPen, hOldPen;
	hPen = CreatePen(m_nBorderStyle, m_nBorderSize, m_crBorderColor);
	hOldPen = (HPEN)SelectObject(hDC, hPen);

	HBRUSH hBrush, hOldBrush;
	if(m_bEraseBkgnd)
		hBrush = CreateSolidBrush(m_crBkColor);
	else
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	if(m_bBorder3D)
	{
		if(m_nInstruShape != kInstruShapeWheel)
			DrawEdge(hDC, &plotRect, m_nBorder3DEdge, BF_RECT|BF_MIDDLE);
		else
		{
			DrawEdge(hDC, &plotRect, m_nBorder3DEdge, BF_LEFT|BF_RIGHT|BF_TOP|BF_BOTTOM);
			InflateRect(&plotRect, -2, -2);
			FillRect(hDC, &plotRect, hBrush);
		}
	}
	else
	{
		Rectangle(hDC, plotRect.left, plotRect.top, plotRect.right, plotRect.bottom);
	}

	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void	CInstruBaseImpl::DrawTitle(HDC hDC, RECT barRect, RECT plotRect)
{
	if(!m_bShowTitle)return;

	tstring realtitle = GetRealTitle();

	RECT titleRect = GetTitleRect(hDC, barRect, plotRect);

	LOGFONT logFont = m_lfTitleFont;

	if(m_nInstruShape == kInstruShapeVertical)
	{
		if(m_nTitlePosition == kInstruPositionWideSmall)
		{
			logFont.lfOrientation = 900;
			logFont.lfEscapement = 900;
		}
		else if(m_nTitlePosition == kInstruPositionWideLarge)
		{
			logFont.lfOrientation = 2700;
			logFont.lfEscapement = 2700;
		}
	}
	HFONT hFont, hOldFont;
	hFont = CreateFontIndirect(&logFont);
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextColor(hDC, m_crTitleColor);
	SetBkMode(hDC, TRANSPARENT);

	if(m_nInstruShape == kInstruShapeVertical && (m_nTitlePosition == kInstruPositionWideSmall || m_nTitlePosition == kInstruPositionWideLarge))
	{
		if(m_nTitlePosition == kInstruPositionWideSmall)
		{
			TextOut(hDC, titleRect.left, titleRect.bottom, realtitle.c_str(), (int)realtitle.length());
		}
		else if(m_nTitlePosition == kInstruPositionWideLarge)
		{
			TextOut(hDC, titleRect.right, titleRect.top, realtitle.c_str(), (int)realtitle.length());
		}
	}
	else
	{
		DrawText(hDC, realtitle.c_str(), (int)realtitle.length(), &titleRect, DT_CENTER);
	}
	
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}

void	CInstruBaseImpl::DrawBar(HDC hDC, RECT barRect)
{
	if(!m_bShowBar)return;
	
	HPEN hPen, hOldPen;
	hPen = CreatePen(PS_SOLID, m_nBarBorderSize, m_crBarBorderColor);
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	
	HBRUSH hBrush, hOldBrush;
	if(m_bFillBar)
		hBrush = CreateSolidBrush(m_crBarColor);
	else
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	RECT fillRect = barRect;
	
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
	case kInstruShapeVertical:
		if(m_bBarBorder3D)
		{
			DrawEdge(hDC, &barRect, EDGE_SUNKEN, BF_RECT|BF_MIDDLE);
			if(m_bFillBar)
			{
				InflateRect(&barRect, -2, -2);
				FillRect(hDC, &barRect, hBrush);
			}
		}
		else
		{
			Rectangle(hDC, barRect.left, barRect.top, barRect.right, barRect.bottom);
		}
		break;
	case kInstruShapeCircle:
		if(m_bShowBar)
		{
			Graphics graph(hDC);
			GraphicsPath path;

			graph.SetSmoothingMode(SmoothingModeHighQuality);

			SolidBrush brush(Color(GetRValue(m_crBarColor), GetGValue(m_crBarColor), GetBValue(m_crBarColor)));
			RECT barRect1 = barRect;
			InflateRect(&barRect1, -m_nBarWidth, -m_nBarWidth);

//			int sgn = 1;
//			if(!m_bClockWise)sgn=-1;
			int sgn = m_bClockWise?1:-1;

			path.AddArc(barRect.left, barRect.top, Width(barRect), Height(barRect), (Gdiplus::REAL)(-m_fAngleStart/myPi*180.0), (Gdiplus::REAL)(sgn*m_fAngleRange/myPi*180.0));
			path.AddArc(barRect1.left, barRect1.top, Width(barRect1), Height(barRect1), (Gdiplus::REAL)(-(m_fAngleStart - sgn*m_fAngleRange)/myPi*180.0), (Gdiplus::REAL)(-sgn*m_fAngleRange/myPi*180.0));

			graph.FillPath(&brush, &path);
			
		}
		if(m_bBarBorder3D)
		{
			DrawCircleEdge(hDC, GetCenterX(barRect), GetCenterY(barRect), Width(barRect)/2.0);
		}
		break;
	case kInstruShapeWheel:
		DrawWheelBar(hDC, barRect);
		break;
	}
	
	if(m_bFillCurr)DrawCurrentFilling(hDC, fillRect);

	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void	CInstruBaseImpl::DrawAxisLine(HDC hDC, RECT axisRect)
{
	if(!m_bShowAxisLine)return;
	
	int margin = m_nBarMargin;
	if(margin > m_nBarWidth/3) margin = m_nBarWidth/3;

//	RECT axisRect = GetAxisRect(hDC, barRect);
	float ctx, cty, radius;

	HPEN hPen, hOldPen;
	hPen = CreatePen(m_nLineStyle, m_nLineSize, m_crAxisColor);
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
			MoveToEx(hDC, axisRect.left, axisRect.bottom, NULL);
			LineTo(hDC, axisRect.right, axisRect.bottom);
			break;
		case kInstruPositionWideLarge:
			MoveToEx(hDC, axisRect.left, axisRect.top, NULL);
			LineTo(hDC, axisRect.right, axisRect.top);
			break;
		}
		break;
	case kInstruShapeVertical:
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
			MoveToEx(hDC, axisRect.right, axisRect.top, NULL);
			LineTo(hDC, axisRect.right, axisRect.bottom);
			break;
		case kInstruPositionWideLarge:
			MoveToEx(hDC, axisRect.left, axisRect.top, NULL);
			LineTo(hDC, axisRect.left, axisRect.bottom);
			break;
		}
		break;
	case kInstruShapeCircle:
		{
			Graphics graph(hDC);
			Pen pen(Color(GetRValue(m_crAxisColor), GetGValue(m_crAxisColor), GetBValue(m_crAxisColor)), (Gdiplus::REAL)m_nLineSize);
			graph.SetSmoothingMode(SmoothingModeHighQuality);
			ctx = (float)GetCenterX(axisRect);
			cty = (float)GetCenterY(axisRect);
			radius = (float)(Width(axisRect)/2.0);
			if(!m_bHideIdleArc)
			{
				graph.DrawEllipse(&pen, ctx - radius, cty - radius, 2.0f*radius, 2.0f*radius);
			}
			else
			{
				graph.DrawArc(&pen, ctx - radius, cty - radius, 2.0f*radius, 2.0f*radius,
					(Gdiplus::REAL)(-m_fAngleStart/myPi*180.0), (Gdiplus::REAL)((m_bClockWise?m_fAngleRange:-m_fAngleRange)/myPi*180.0));
			}
		}
		break;
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void	CInstruBaseImpl::DrawColorAxis(HDC hDC, RECT axisRect)
{
	if(!m_bAxisColorful || !m_bShowAxis || (!m_bShowMajorTicks && !m_bShowMinorTicks) )return;

	int i = 0, nowTicks = 0, preTicks = 0;
	COLORREF color;
	bool flag = true;
	double x1, y1, x2, y2;
	double step;

	Graphics graph(hDC);
	SolidBrush brush(Color(0, 0, 0));
	graph.SetSmoothingMode(SmoothingModeHighQuality);

	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
		case kInstruPositionWideLarge:
			step = Width(axisRect)/(m_nTickCount+1.0);
			while(flag)
			{
				if(i>=(int)m_vAxisSegColors.size())i-=(int)m_vAxisSegColors.size();

				preTicks = nowTicks;
				nowTicks += m_vAxisSegColors[i].nTicks;
				if(nowTicks >= m_nTickCount+1)
				{
					nowTicks = m_nTickCount + 1;
					flag = false;
				}
				color = m_vAxisSegColors[i].color;

				if(m_nAxisPosition == kInstruPositionWideSmall)
				{
					x1 = axisRect.left + preTicks*step;
					x2 = axisRect.left + nowTicks*step;
					y1 = axisRect.bottom - m_nTickLength;
					y2 = axisRect.bottom;
				}
				else
				{
					x1 = axisRect.left + preTicks*step;
					x2 = axisRect.left + nowTicks*step;
					y1 = axisRect.top;
					y2 = axisRect.top + m_nTickLength;
				}

				brush.SetColor(Color(GetRValue(color), GetGValue(color), GetBValue(color)));
				graph.FillRectangle(&brush, float(x1), float(y1), float(x2-x1), float(y2-y1));

				i++;
			}
			break;
		}
		break;
	case kInstruShapeVertical:
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
		case kInstruPositionWideLarge:
			step = Height(axisRect)/(m_nTickCount+1.0);
			while(flag)
			{
				if(i>=(int)m_vAxisSegColors.size())i-=(int)m_vAxisSegColors.size();
				
				preTicks = nowTicks;
				nowTicks += m_vAxisSegColors[i].nTicks;
				if(nowTicks >= m_nTickCount+1)
				{
					nowTicks = m_nTickCount + 1;
					flag = false;
				}
				color = m_vAxisSegColors[i].color;
				
				if(m_nAxisPosition == kInstruPositionWideSmall)
				{
					x1 = axisRect.right - m_nTickLength;
					x2 = axisRect.right;
					
					y1 = axisRect.bottom - nowTicks*step;
					y2 = axisRect.bottom - preTicks*step;
				}
				else
				{
					x1 = axisRect.left;
					x2 = axisRect.left + m_nTickLength;
					y1 = axisRect.bottom - nowTicks*step;
					y2 = axisRect.bottom - preTicks*step;
				}
				
				brush.SetColor(Color(GetRValue(color), GetGValue(color), GetBValue(color)));
				graph.FillRectangle(&brush, float(x1), float(y1), float(x2-x1), float(y2-y1));
				
				i++;
			}
			break;
		}
		break;
	case kInstruShapeCircle:
		{
			step = m_fAngleRange/(m_nTickCount+1.0);
			if(m_bClockWise)step *= -1;

			double radius, ctx, cty;
			radius = Width(axisRect)/2.0;
			ctx = (axisRect.left + axisRect.right)/2.0;
			cty = (axisRect.top + axisRect.bottom)/2.0;
			
			double r1, r2;
			if(m_nTickPosition == kInstruPosInner)
			{
				r1 = radius - m_nTickLength;
				r2 = radius;
			}
			else if(m_nTickPosition == kInstruPosOuter)
			{
				r1 = radius;
				r2 = radius + m_nTickLength;
			}
			else
			{
				r1 = radius - m_nTickLength/2.0;
				r2 = radius + m_nTickLength/2.0;
			}

			double tht1, tht2;

			while(flag)
			{
				if(i>=(int)m_vAxisSegColors.size())i-=(int)m_vAxisSegColors.size();
				
				preTicks = nowTicks;
				nowTicks += m_vAxisSegColors[i].nTicks;
				if(nowTicks >= m_nTickCount+1)
				{
					nowTicks = m_nTickCount + 1;
					flag = false;
				}
				color = m_vAxisSegColors[i].color;

				tht1 = m_fAngleStart + preTicks*step;
				tht2 = m_fAngleStart + nowTicks*step;

				double range = tht2 - tht1;

				GraphicsPath path;

				path.AddArc(float(ctx - r1), float(cty - r1), float(2.0 * r1), float(2.0 * r1), (Gdiplus::REAL)(-tht1/myDeg), (Gdiplus::REAL)(-range/myDeg));
				path.AddArc(float(ctx - r2), float(cty - r2), float(2.0 * r2), float(2.0 * r2), (Gdiplus::REAL)(-tht2/myDeg), (Gdiplus::REAL)(range/myDeg));
				
				brush.SetColor(Color(GetRValue(color), GetGValue(color), GetBValue(color)));

				path.CloseAllFigures();
				graph.FillPath(&brush, &path);
				
				i++;
			}
		}
		break;
	}
}

void	CInstruBaseImpl::DrawMajorTicks(HDC hDC, RECT axisRect)
{
	if(!m_bShowMajorTicks)return;

	//int margin = m_nBarMargin;
	//if(margin > m_nBarWidth/3) margin = m_nBarWidth/3;

	//RECT axisRect = GetAxisRect(hDC, barRect);
	
	HPEN hPen, hOldPen;
	hPen = CreatePen( PS_SOLID, m_nTickSize, m_crTickColor );
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	
	int i;
	double x, y;
	double step;
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
			step = Width(axisRect)/(m_nTickCount+1.0);
			for(i=0; i<=m_nTickCount+1; i++)
			{
				if(!m_bOpposite)
					x = axisRect.left + i*step;
				else
					x = axisRect.right - i*step;
				y = axisRect.bottom;
				MoveToEx(hDC, (int)x, (int)y, NULL);
				LineTo(hDC, (int)x, int(y-m_nTickLength));
			}
			break;
		case kInstruPositionWideLarge:
			step = Width(axisRect)/(m_nTickCount+1.0);
			for(i=0; i<=m_nTickCount+1; i++)
			{
				if(!m_bOpposite)
					x = axisRect.left + i*step;
				else
					x = axisRect.right - i*step;
				y = axisRect.top;
				MoveToEx(hDC, (int)x, (int)y, NULL);
				LineTo(hDC, (int)x, int(y+m_nTickLength));
			}
			break;
		}
		break;
	case kInstruShapeVertical:
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
			step = Height(axisRect)/(m_nTickCount+1.0);
			for(i=0; i<=m_nTickCount+1; i++)
			{
				if(!m_bOpposite)
					y = axisRect.bottom - i*step;
				else
					y = axisRect.top + i*step;
				x = axisRect.right;
				MoveToEx(hDC, (int)x, (int)y, NULL);
				LineTo(hDC, int(x-m_nTickLength), (int)y);
			}
			break;
		case kInstruPositionWideLarge:
			step = Height(axisRect)/(m_nTickCount+1.0);
			for(i=0; i<=m_nTickCount+1; i++)
			{
				if(!m_bOpposite)
					y = axisRect.bottom - i*step;
				else
					y = axisRect.top + i*step;
				x = axisRect.left;
				MoveToEx(hDC, (int)x, (int)y, NULL);
				LineTo(hDC, int(x+m_nTickLength), (int)y);
			}
			break;
		}
		break;
	case kInstruShapeCircle:
		{
			Graphics graph(hDC);
			Pen pen(Color(GetRValue(m_crTickColor),GetGValue(m_crTickColor),GetBValue(m_crTickColor)), (Gdiplus::REAL)m_nTickSize);
			graph.SetSmoothingMode(SmoothingModeHighQuality);

			step = m_fAngleRange/(m_nTickCount+1.0);
			if(m_bClockWise)step *= -1;

			double radius, ctx, cty;
			radius = Width(axisRect)/2.0;
			ctx = (axisRect.left + axisRect.right)/2.0;
			cty = (axisRect.top + axisRect.bottom)/2.0;
			
			double r1, r2;
			if(m_nTickPosition == kInstruPosInner)
			{
				r1 = radius - m_nTickLength;
				r2 = radius;
			}
			else if(m_nTickPosition == kInstruPosOuter)
			{
				r1 = radius;
				r2 = radius + m_nTickLength;
			}
			else
			{
				r1 = radius - m_nTickLength/2.0;
				r2 = radius + m_nTickLength/2.0;
			}

			double x1,y1;
			double tht;
			
			for(i=0; i<=m_nTickCount+1; i++)
			{
				tht = m_fAngleStart + i*step;

				if(!m_bTickBar)
				{
					x = ctx + r1 * cos(tht);
					y = cty - r1 * sin(tht);
					x1 = ctx + r2 * cos(tht);
					y1 = cty - r2 * sin(tht);
					
					graph.DrawLine(&pen, (float)x, (float)y, (float)x1, (float)y1);
				}
				else
				{
					POINT pPt[4];
					pPt[0].x = long(ctx + r1 * cos(tht) + m_nTickBarHalfWidthEnd * cos(tht - myPi/2.0));
					pPt[0].y = long(cty - r1 * sin(tht) - m_nTickBarHalfWidthEnd * sin(tht - myPi/2.0));
					pPt[1].x = long(ctx + r1 * cos(tht) + m_nTickBarHalfWidthEnd * cos(tht + myPi/2.0));
					pPt[1].y = long(cty - r1 * sin(tht) - m_nTickBarHalfWidthEnd * sin(tht + myPi/2.0));
					pPt[2].x = long(ctx + r2 * cos(tht) + m_nTickBarHalfWidthBase * cos(tht + myPi/2.0));
					pPt[2].y = long(cty - r2 * sin(tht) - m_nTickBarHalfWidthBase * sin(tht + myPi/2.0));
					pPt[3].x = long(ctx + r2 * cos(tht) + m_nTickBarHalfWidthBase * cos(tht - myPi/2.0));
					pPt[3].y = long(cty - r2 * sin(tht) - m_nTickBarHalfWidthBase * sin(tht - myPi/2.0));
					FillPolygon(hDC, m_crTickColor, pPt, 4);
				}
			}
		}
		break;
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void	CInstruBaseImpl::DrawSubMajorTicks(HDC hDC, RECT axisRect)
{
	if(!m_bShowMajorTicks)return;
	if(!m_bShowSubMajorTicks)return;

	//int margin = m_nBarMargin;
	//if(margin > m_nBarWidth/3) margin = m_nBarWidth/3;

	//RECT axisRect = GetAxisRect(hDC, barRect);
	
	HPEN hPen, hOldPen;
	hPen = CreatePen( PS_SOLID, m_nSubTickSize, m_crSubTickColor );
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	
	int i;
	double x, y;
	double step;
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
			step = Width(axisRect)/(m_nTickCount+1.0);
			for(i=0; i<=m_nTickCount; i++)
			{
				if(!m_bOpposite)
					x = axisRect.left + (i+0.5)*step;
				else
					x = axisRect.right - (i+0.5)*step;
				y = axisRect.bottom;
				MoveToEx(hDC, (int)x, (int)y, NULL);
				LineTo(hDC, (int)x, int(y-m_nSubTickLength));
			}
			break;
		case kInstruPositionWideLarge:
			step = Width(axisRect)/(m_nTickCount+1.0);
			for(i=0; i<=m_nTickCount; i++)
			{
				if(!m_bOpposite)
					x = axisRect.left + (i+0.5)*step;
				else
					x = axisRect.right - (i+0.5)*step;
				y = axisRect.top;
				MoveToEx(hDC, (int)x, (int)y, NULL);
				LineTo(hDC, (int)x, int(y+m_nSubTickLength));
			}
			break;
		}
		break;
	case kInstruShapeVertical:
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
			step = Height(axisRect)/(m_nTickCount+1.0);
			for(i=0; i<=m_nTickCount; i++)
			{
				if(!m_bOpposite)
					y = axisRect.bottom - (i+0.5)*step;
				else
					y = axisRect.top + (i+0.5)*step;
				x = axisRect.right;
				MoveToEx(hDC, (int)x, (int)y, NULL);
				LineTo(hDC, int(x-m_nSubTickLength), (int)y);
			}
			break;
		case kInstruPositionWideLarge:
			step = Height(axisRect)/(m_nTickCount+1.0);
			for(i=0; i<=m_nTickCount; i++)
			{
				if(!m_bOpposite)
					y = axisRect.bottom - (i+0.5)*step;
				else
					y = axisRect.top + (i+0.5)*step;
				x = axisRect.left;
				MoveToEx(hDC, (int)x, (int)y, NULL);
				LineTo(hDC, int(x+m_nSubTickLength), (int)y);
			}
			break;
		}
		break;
	case kInstruShapeCircle:
		{
			Graphics graph(hDC);
			Pen pen(Color(GetRValue(m_crTickColor),GetGValue(m_crTickColor),GetBValue(m_crTickColor)), (Gdiplus::REAL)m_nTickSize);
			graph.SetSmoothingMode(SmoothingModeHighQuality);

			step = m_fAngleRange/(m_nTickCount+1.0);
			if(m_bClockWise)step *= -1;

			double radius, ctx, cty;
			radius = Width(axisRect)/2.0;
			ctx = (axisRect.left + axisRect.right)/2.0;
			cty = (axisRect.top + axisRect.bottom)/2.0;
			
			double r1, r2;
			if(m_nTickPosition == kInstruPosInner)
			{
				r1 = radius - m_nTickLength;
				r2 = radius;
			}
			else if(m_nTickPosition == kInstruPosOuter)
			{
				r1 = radius;
				r2 = radius + m_nTickLength;
			}
			else
			{
				r1 = radius - m_nTickLength/2.0;
				r2 = radius + m_nTickLength/2.0;
			}

			double x1,y1;
			double tht;
			
			for(i=0; i<=m_nTickCount; i++)
			{
				tht = m_fAngleStart + (i+0.5)*step;

				if(!m_bTickBar)
				{
					x = ctx + r1 * cos(tht);
					y = cty - r1 * sin(tht);
					x1 = ctx + r2 * cos(tht);
					y1 = cty - r2 * sin(tht);
					
					graph.DrawLine(&pen, (float)x, (float)y, (float)x1, (float)y1);
				}
				else
				{
					POINT pPt[4];
					pPt[0].x = long(ctx + r1 * cos(tht) + m_nTickBarHalfWidthEnd * cos(tht - myPi/2.0));
					pPt[0].y = long(cty - r1 * sin(tht) - m_nTickBarHalfWidthEnd * sin(tht - myPi/2.0));
					pPt[1].x = long(ctx + r1 * cos(tht) + m_nTickBarHalfWidthEnd * cos(tht + myPi/2.0));
					pPt[1].y = long(cty - r1 * sin(tht) - m_nTickBarHalfWidthEnd * sin(tht + myPi/2.0));
					pPt[2].x = long(ctx + r2 * cos(tht) + m_nTickBarHalfWidthBase * cos(tht + myPi/2.0));
					pPt[2].y = long(cty - r2 * sin(tht) - m_nTickBarHalfWidthBase * sin(tht + myPi/2.0));
					pPt[3].x = long(ctx + r2 * cos(tht) + m_nTickBarHalfWidthBase * cos(tht - myPi/2.0));
					pPt[3].y = long(cty - r2 * sin(tht) - m_nTickBarHalfWidthBase * sin(tht - myPi/2.0));
					FillPolygon(hDC, m_crTickColor, pPt, 4);
				}
			}
		}
		break;
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void	CInstruBaseImpl::DrawMinorTicks(HDC hDC, RECT axisRect)
{
	if(!m_bShowMinorTicks)return;
	
	//int margin = m_nBarMargin;
	//if(margin > m_nBarWidth/3) margin = m_nBarWidth/3;

	//RECT axisRect = GetAxisRect(hDC, barRect);

	double *minorPos = new double[GetMinorTickCount()+1];
	double scale;
	
	HPEN hPen, hOldPen;
	hPen = CreatePen( PS_SOLID, m_nMinorTickSize, m_crMinorTickColor );
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	
	int i, j;
	double x, y;
	double step;
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		scale = (double)abs(axisRect.right - axisRect.left) / (m_pfRange[1] - m_pfRange[0]);
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
			step = Width(axisRect)/(m_nTickCount+1.0);
			for(i=0; i<m_nTickCount+1; i++)
			{
				GetMinorTickPosValue(i, minorPos);
				for(j=1; j<m_nMinorTickCount; j++)
				{
					if(!m_bLogarithm)
					{
						if(!m_bOpposite)
							x = axisRect.left + i*step + j*step/m_nMinorTickCount;
						else
							x = axisRect.right - i*step - j*step/m_nMinorTickCount;
					}
					else
					{
						//chartRect.left + (long)((minorPos[j] - m_pfRange[0]) * scale);
						if(!m_bOpposite)
							x = axisRect.left + (minorPos[j] - m_pfRange[0]) * scale;
						else
							x = axisRect.right - (minorPos[j] - m_pfRange[0]) * scale;
					}
					y = axisRect.bottom;
					MoveToEx(hDC, (int)x, (int)y, NULL);
					LineTo(hDC, (int)x, int(y-m_nMinorTickLength));
				}
				
			}
			break;
		case kInstruPositionWideLarge:
			step = Width(axisRect)/(m_nTickCount+1.0);
			for(i=0; i<m_nTickCount+1; i++)
			{
				GetMinorTickPosValue(i, minorPos);
				for(j=1; j<m_nMinorTickCount; j++)
				{
					if(!m_bLogarithm)
					{
						if(!m_bOpposite)
							x = axisRect.left + i*step + j*step/m_nMinorTickCount;
						else
							x = axisRect.right - i*step - j*step/m_nMinorTickCount;
					}
					else
					{
						if(!m_bOpposite)
							x = axisRect.left + (minorPos[j] - m_pfRange[0]) * scale;
						else
							x = axisRect.right - (minorPos[j] - m_pfRange[0]) * scale;
					}
					
					y = axisRect.top;
					MoveToEx(hDC, (int)x, (int)y, NULL);
					LineTo(hDC, (int)x, int(y+m_nMinorTickLength));
				}
			}
			break;
		}
		break;
	case kInstruShapeVertical:
		scale = (double)abs(axisRect.bottom - axisRect.top) / (m_pfRange[1] - m_pfRange[0]);
		switch(m_nAxisPosition)
		{
		case kInstruPositionWideSmall:
			step = Height(axisRect)/(m_nTickCount+1.0);
			for(i=0; i<m_nTickCount+1; i++)
			{
				GetMinorTickPosValue(i, minorPos);
				for(j=1; j<m_nMinorTickCount; j++)
				{
					if(!m_bLogarithm)
					{
						if(!m_bOpposite)
							y = axisRect.bottom - i*step - j*step/m_nMinorTickCount;
						else
							y = axisRect.top + i*step + j*step/m_nMinorTickCount;
					}
					else
					{
						if(!m_bOpposite)
							y = axisRect.bottom - (minorPos[j] - m_pfRange[0]) * scale;
						else
							y = axisRect.top + (minorPos[j] - m_pfRange[0]) * scale;
					}
					x = axisRect.right;
					MoveToEx(hDC, (int)x, (int)y, NULL);
					LineTo(hDC, int(x-m_nMinorTickLength), (int)y);
				}
				
			}
			break;
		case kInstruPositionWideLarge:
			step = Height(axisRect)/(m_nTickCount+1.0);
			for(i=0; i<m_nTickCount+1; i++)
			{
				GetMinorTickPosValue(i, minorPos);
				for(j=1; j<m_nMinorTickCount; j++)
				{
					if(!m_bLogarithm)
					{
						if(!m_bOpposite)
							y = axisRect.bottom - i*step - j*step/m_nMinorTickCount;
						else
							y = axisRect.top + i*step + j*step/m_nMinorTickCount;
					}
					else
					{
						if(!m_bOpposite)
							y = axisRect.bottom - (minorPos[j] - m_pfRange[0]) * scale;
						else
							y = axisRect.top + (minorPos[j] - m_pfRange[0]) * scale;
					}
					x = axisRect.left;
					MoveToEx(hDC, (int)x, (int)y, NULL);
					LineTo(hDC, int(x+m_nMinorTickLength), (int)y);
				}
			}
			break;
		}
		break;
	case kInstruShapeCircle:
		{
			Graphics graph(hDC);
			Pen pen(Color(GetRValue(m_crTickColor),GetGValue(m_crTickColor),GetBValue(m_crTickColor)), (Gdiplus::REAL)m_nTickSize);
			graph.SetSmoothingMode(SmoothingModeHighQuality);

			step = m_fAngleRange/(m_nTickCount+1.0);
			if(m_bClockWise)step *= -1;

			double radius, ctx, cty;
			radius = Width(axisRect)/2.0;
			ctx = (axisRect.left + axisRect.right)/2.0;
			cty = (axisRect.top + axisRect.bottom)/2.0;

			double r1, r2;
			if(m_nMinorTickPosition == kInstruPosInner)
			{
				r1 = radius - m_nMinorTickLength;
				r2 = radius;
			}
			else if(m_nMinorTickPosition == kInstruPosOuter)
			{
				r1 = radius;
				r2 = radius + m_nMinorTickLength;
			}
			else
			{
				r1 = radius - m_nMinorTickLength/2.0;
				r2 = radius + m_nMinorTickLength/2.0;
			}

			double x1,y1;
			double tht;
			for(i=0; i<m_nTickCount+1; i++)
			{
				for(j=1; j<m_nMinorTickCount; j++)
				{
					if(!m_bLogarithm)
					{
						tht = m_fAngleStart + i*step + j*step/m_nMinorTickCount;
					}
					else
					{
						
					}
					
					if(!m_bMinorTickBar)
					{
						x = ctx + r1 * cos(tht);
						y = cty - r1 * sin(tht);
						x1 = ctx + r2 * cos(tht);
						y1 = cty - r2 * sin(tht);
						
						graph.DrawLine(&pen, (float)x, (float)y, (float)x1, (float)y1);
					}
					else
					{
						POINT pPt[4];
						pPt[0].x = long(ctx + r1 * cos(tht) + m_nMinorTickBarHalfWidthEnd * cos(tht - myPi/2.0));
						pPt[0].y = long(cty - r1 * sin(tht) - m_nMinorTickBarHalfWidthEnd * sin(tht - myPi/2.0));
						pPt[1].x = long(ctx + r1 * cos(tht) + m_nMinorTickBarHalfWidthEnd * cos(tht + myPi/2.0));
						pPt[1].y = long(cty - r1 * sin(tht) - m_nMinorTickBarHalfWidthEnd * sin(tht + myPi/2.0));
						pPt[2].x = long(ctx + r2 * cos(tht) + m_nMinorTickBarHalfWidthBase * cos(tht + myPi/2.0));
						pPt[2].y = long(cty - r2 * sin(tht) - m_nMinorTickBarHalfWidthBase * sin(tht + myPi/2.0));
						pPt[3].x = long(ctx + r2 * cos(tht) + m_nMinorTickBarHalfWidthBase * cos(tht - myPi/2.0));
						pPt[3].y = long(cty - r2 * sin(tht) - m_nMinorTickBarHalfWidthBase * sin(tht - myPi/2.0));
						FillPolygon(hDC, m_crTickColor, pPt, 4);
					}
				}
			}
		}
		
		break;
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);

	delete []minorPos;
}

void	CInstruBaseImpl::DrawTickLabels(HDC hDC, RECT axisRect, RECT barRect)
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
				textRect.bottom = axisRect.bottom - GetMaxTickLength();
				textRect.top = textRect.bottom - labelSize.cy;
				if(!m_bOpposite)
					textRect.left = long(axisRect.left + i*step - labelSize.cx/2.0);
				else
					textRect.left = long(axisRect.right - i*step - labelSize.cx/2.0);
				textRect.right = textRect.left + labelSize.cx;
				
				DrawText(hDC, label.c_str(), (int)label.length(), &textRect, DT_CENTER);
			}
			break;
		case kInstruPositionWideLarge:
			for(i=0; i<=m_nTickCount+1; i++)
			{
				label = GetMajorTickLabel(i);
				MyGetTextExtentPoint(hDC, &m_lfLabelFont, label, DT_CENTER, &labelSize);
				textRect.top = axisRect.top + GetMaxTickLength();
				textRect.bottom = textRect.top + labelSize.cy;
				if(!m_bOpposite)
					textRect.left = long(axisRect.left + i*step - labelSize.cx/2.0);
				else
					textRect.left = long(axisRect.right - i*step - labelSize.cx/2.0);
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
				textRect.right = axisRect.right - GetMaxTickLength();
				textRect.left = textRect.right - labelSize.cx;
				if(!m_bOpposite)
					textRect.bottom = long(axisRect.bottom - i*step + labelSize.cy/2.0);
				else
					textRect.bottom = long(axisRect.top + i*step + labelSize.cy/2.0);
				textRect.top = textRect.bottom - labelSize.cy;
				
				DrawText(hDC, label.c_str(), (int)label.length(), &textRect, DT_RIGHT);
			}
			break;
		case kInstruPositionWideLarge:
			for(i=0; i<=m_nTickCount+1; i++)
			{
				label = GetMajorTickLabel(i);
				MyGetTextExtentPoint(hDC, &m_lfLabelFont, label, DT_LEFT, &labelSize);
				textRect.left = axisRect.left + GetMaxTickLength();
				textRect.right = textRect.left + labelSize.cx;
				if(!m_bOpposite)
					textRect.bottom = long(axisRect.bottom - i*step + labelSize.cy/2.0);
				else
					textRect.bottom = long(axisRect.top + i*step + labelSize.cy/2.0);
				textRect.top = textRect.bottom - labelSize.cy;
				
				DrawText(hDC, label.c_str(), (int)label.length(), &textRect, DT_LEFT);
				
				DrawText(hDC, label.c_str(), (int)label.length(), &textRect, DT_CENTER);
			}
			break;
		}
		break;
	case kInstruShapeCircle:
		double tickL;
		//tickL = GetSideTickLengh(true);
		//InflateRect(&axisRect, (int)tickL, (int)tickL);
		
		double ctx, cty;
		ctx = (axisRect.left + axisRect.right)/2.0;
		cty = (axisRect.top + axisRect.bottom)/2.0;
		double tht;

		RECT rectForInner;
		rectForInner = axisRect;

		tickL = GetSideTickLengh(false);
		InflateRect(&rectForInner, int(-tickL), int(-tickL));

		step = m_fAngleRange/(m_nTickCount+1.0);
		if(m_bClockWise)step *= -1;

		int count1, count2;
		if(m_nLabelEdgeType == kShowLabelEdgeStart)
		{
			count1 = 0;
			count2 = m_nTickCount;
		}
		else if(m_nLabelEdgeType == kShowLabelEdgeEnd)
		{
			count1 = 1;
			count2 = m_nTickCount + 1;
		}
		else
		{
			count1 = 0;
			count2 = m_nTickCount + 1;
		}
		for(i=count1; i<=count2; i++)
		{
			label = GetMajorTickLabel(i);
			if(label.length() <= 0)continue;

			tht = m_fAngleStart + i*step;
			
			if(m_nLabelPosition == kInstruPosOuter)
			{
				textRect = GetTextRectAroundEllipse(hDC, axisRect, &barRect, tht, 1.0, 1.0, label, m_lfLabelFont);
			}
			else
			{
				textRect = GetTextRectAroundEllipse(hDC, rectForInner, &barRect, tht, 1.0, 1.0, label, m_lfLabelFont, 4, false);
			}
			
			DrawText(hDC, label.c_str(), (int)label.length(), &textRect, DT_LEFT);
		}
		break;
	}
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}

void	CInstruBaseImpl::DrawAxis(HDC hDC, RECT barRect)
{
	if(!m_bShowAxis)return;

	RECT axisRect = GetAxisRect(hDC, barRect);
	DrawAxisLine(hDC, axisRect);
	DrawColorAxis(hDC, axisRect);
	DrawMajorTicks(hDC, axisRect);
	DrawSubMajorTicks(hDC, axisRect);
	DrawMinorTicks(hDC, axisRect);
	DrawTickLabels(hDC, axisRect, barRect);
}

void	CInstruBaseImpl::DrawCurrentFilling(HDC hDC, RECT barRect)
{
	if(m_fCurrValue < m_pfRange[0] || m_fCurrValue > m_pfRange[1])return;

	RECT fillRect = barRect;
	if(m_bBarBorder3D)
		InflateRect(&fillRect, -2, -2);
	switch(m_nInstruShape)
	{
	case kInstruShapeHorizontal:
		if(!m_bOpposite)
			fillRect.right = long(fillRect.left + Width(barRect)*(m_fCurrValue - m_pfRange[0])/(m_pfRange[1] - m_pfRange[0]));
		else
			fillRect.left = long(fillRect.right - Width(barRect)*(m_fCurrValue - m_pfRange[0])/(m_pfRange[1] - m_pfRange[0]));
		break;
	case kInstruShapeVertical:
		if(!m_bOpposite)
			fillRect.top = long(fillRect.bottom - Height(barRect)*(m_fCurrValue - m_pfRange[0])/(m_pfRange[1] - m_pfRange[0]));
		else
			fillRect.bottom = long(fillRect.top + Height(barRect)*(m_fCurrValue - m_pfRange[0])/(m_pfRange[1] - m_pfRange[0]));
		break;
	case kInstruShapeCircle:
		if(m_bShowBar &&m_bFillCurr)
		{
			Graphics graph(hDC);
			GraphicsPath path;

			graph.SetSmoothingMode(SmoothingModeHighQuality);
			
			SolidBrush brush(Color(GetRValue(m_crCurrColor), GetGValue(m_crCurrColor), GetBValue(m_crCurrColor)));
			RECT barRect1 = barRect;
			InflateRect(&barRect1, -m_nBarWidth, -m_nBarWidth);
			
			int sgn = 1;
			if(!m_bClockWise)sgn=-1;

			double curAngle = (m_fCurrValue - m_pfRange[0])*m_fAngleRange/(m_pfRange[1] - m_pfRange[0]);
			
			path.AddArc(barRect.left, barRect.top, Width(barRect), Height(barRect), Gdiplus::REAL(-m_fAngleStart/myPi*180.0), Gdiplus::REAL(sgn*curAngle/myPi*180.0));
			path.AddArc(barRect1.left, barRect1.top, Width(barRect1), Height(barRect1), Gdiplus::REAL(-(m_fAngleStart - sgn*curAngle)/myPi*180.0), Gdiplus::REAL(-sgn*curAngle/myPi*180.0));
			
			graph.FillPath(&brush, &path);
			
		}
		return;
	}

	HBRUSH hBrush = CreateSolidBrush(m_crCurrColor);
	FillRect(hDC, &fillRect, hBrush);
	DeleteObject(hBrush);
}

void	CInstruBaseImpl::DrawWholeCircleFilling(HDC hDC, RECT barRect)
{
	if(!m_bFillWholeCircle)return;
	if(m_nInstruShape != kInstruShapeCircle)return;

	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	SolidBrush brush(Color(GetRValue(m_crWholeCircleColor), GetGValue(m_crWholeCircleColor), GetBValue(m_crWholeCircleColor)));
	graph.FillEllipse(&brush, barRect.left, barRect.top, Width(barRect), Height(barRect));
}

void	CInstruBaseImpl::FillPolygon(HDC hDC, COLORREF color, POINT *pPt, int nPt)
{
	if(nPt<=2)return;
	
	Graphics graph(hDC);
	GraphicsPath path;
	
	Point *pPoints = new Point[nPt];
	for(int i=0; i<nPt; i++)
	{
		pPoints[i].X = pPt[i].x;
		pPoints[i].Y = pPt[i].y;
	}
	
	path.AddPolygon(pPoints, nPt);
	
	SolidBrush brush(Color(GetRValue(color), GetGValue(color), GetBValue(color)));
	
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	graph.FillPath(&brush, &path);
	delete []pPoints;
}

void	CInstruBaseImpl::DrawStaticPointers(HDC hDC, RECT barRect)
{
	if(m_nInstruShape != kInstruShapeCircle)return;
	if(!m_bShowStaticPointer)return;
	if(m_nStaticPointerIndex <= 0 || m_nStaticPointerIndex >= 5)return;

	RECT pointerRect = GetPointerRect(hDC, barRect);

	COLORREF cr1, cr2;
	cr1 = m_crStaticPointerColor1;
	cr2 = m_crStaticPointerColor2;
	int	withdraw;
	withdraw = m_nStaticPointerAdditionalWithdraw;

	double radius, tht, ctx, cty;

	radius = Width(pointerRect)/2.0;
	ctx = (pointerRect.left + pointerRect.right)/2.0;
	cty = (pointerRect.top + pointerRect.bottom)/2.0;

	POINT pt[3];

	int i;
	double t1,len;

	double sgn = 1;
	if(m_bClockWise)sgn = -1;

	switch(m_nStaticPointerIndex)
	{
	case 1:
		for(i = 0; i <= 1; i++)
		{
			tht = m_fAngleStart + i * myPi * sgn;
			pt[0].x = long(ctx + radius * cos(tht));
			pt[0].y = long(cty - radius * sin(tht));
			pt[1].x = long(ctx + m_nStaticPointerBaseHalfWidth * cos(tht - myPi/2.0));
			pt[1].y = long(cty - m_nStaticPointerBaseHalfWidth * sin(tht - myPi/2.0));
			pt[2].x = long(ctx);
			pt[2].y = long(cty);
			FillPolygon(hDC, cr1, pt, 3);
			pt[1].x = long(ctx + m_nStaticPointerBaseHalfWidth * cos(tht + myPi/2.0));
			pt[1].y = long(cty - m_nStaticPointerBaseHalfWidth * sin(tht + myPi/2.0));
			FillPolygon(hDC, cr2, pt, 3);
		}
		break;
	case 2:
		t1 = atan(m_nStaticPointerBaseHalfWidth/radius);
		len = radius/sin(0.75*myPi - t1)*sin(t1);

		for(i = 0; i <= 3; i++)
		{
			tht = m_fAngleStart + i * myPi/2.0 * sgn;
			pt[0].x = long(ctx + radius * cos(tht));
			pt[0].y = long(cty - radius * sin(tht));
			pt[1].x = long(ctx + len * cos(tht - myPi/4.0));
			pt[1].y = long(cty - len * sin(tht - myPi/4.0));
			pt[2].x = long(ctx);
			pt[2].y = long(cty);
			FillPolygon(hDC, cr1, pt, 3);
			pt[1].x = long(ctx + len * cos(tht + myPi/4.0));
			pt[1].y = long(cty - len * sin(tht + myPi/4.0));
			FillPolygon(hDC, cr2, pt, 3);
		}
		break;
	case 3:
		t1 = atan(m_nStaticPointerBaseHalfWidth/radius);
		len = radius/sin(0.875*myPi - t1)*sin(t1);
		
		for(i = 0; i <= 7; i++)
		{
			tht = m_fAngleStart + i * myPi/4.0 * sgn;
			pt[0].x = long(ctx + radius * cos(tht));
			pt[0].y = long(cty - radius * sin(tht));
			pt[1].x = long(ctx + len * cos(tht - myPi/8.0));
			pt[1].y = long(cty - len * sin(tht - myPi/8.0));
			pt[2].x = long(ctx);
			pt[2].y = long(cty);
			FillPolygon(hDC, cr1, pt, 3);
			pt[1].x = long(ctx + len * cos(tht + myPi/8.0));
			pt[1].y = long(cty - len * sin(tht + myPi/8.0));
			FillPolygon(hDC, cr2, pt, 3);
		}
		break;
	case 4:
		t1 = atan(m_nStaticPointerBaseHalfWidth/radius);
		len = radius/sin(0.875*myPi - t1)*sin(t1);
		
		radius -= withdraw;
		for(i = 0; i <= 7; i++)
		{
			tht = m_fAngleStart + i * myPi/4.0 * sgn + myPi/8.0;
			pt[0].x = long(ctx + radius * cos(tht));
			pt[0].y = long(cty - radius * sin(tht));
			pt[1].x = long(ctx + len * cos(tht - myPi/8.0));
			pt[1].y = long(cty - len * sin(tht - myPi/8.0));
			pt[2].x = long(ctx);
			pt[2].y = long(cty);
			FillPolygon(hDC, cr1, pt, 3);
			pt[1].x = long(ctx + len * cos(tht + myPi/8.0));
			pt[1].y = long(cty - len * sin(tht + myPi/8.0));
			FillPolygon(hDC, cr2, pt, 3);
		}
		radius += withdraw;
		for(i = 0; i <= 7; i++)
		{
			tht = m_fAngleStart + i * myPi/4.0 * sgn;
			pt[0].x = long(ctx + radius * cos(tht));
			pt[0].y = long(cty - radius * sin(tht));
			pt[1].x = long(ctx + len * cos(tht - myPi/8.0));
			pt[1].y = long(cty - len * sin(tht - myPi/8.0));
			pt[2].x = long(ctx);
			pt[2].y = long(cty);
			FillPolygon(hDC, cr1, pt, 3);
			pt[1].x = long(ctx + len * cos(tht + myPi/8.0));
			pt[1].y = long(cty - len * sin(tht + myPi/8.0));
			FillPolygon(hDC, cr2, pt, 3);
		}
		break;
	}
}

void	CInstruBaseImpl::DrawStick(HDC hDC, RECT barRect)
{
	if(m_nInstruShape != kInstruShapeCircle)return;
	
	double ctx, cty;
	ctx = GetCenterX(barRect);
	cty = GetCenterY(barRect);
	
	Graphics graph(hDC);
	SolidBrush brush(Color(GetRValue(m_crStickColor), GetGValue(m_crStickColor), GetBValue(m_crStickColor)));
	
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	graph.FillEllipse(&brush, float(ctx - m_nStickRadius), float(cty - m_nStickRadius), float(2 * m_nStickRadius), float(2 * m_nStickRadius) );
}

void	CInstruBaseImpl::DrawPointer(HDC hDC, RECT barRect)
{
	if(m_nInstruShape != kInstruShapeCircle)return;
	if(m_pfRange[1] == m_pfRange[0])return;
	if(!m_bShowPointer)return;

	double tht, radius, ctx, cty;
	GetPointerParams(hDC, barRect, tht, radius, ctx, cty);
	
	if(m_bPointerCircle3D)
	{
		DrawCircleEdge(hDC, ctx, cty, radius, false);
	}
	
	if(m_bFillPointerCircle)
	{
		Graphics graph(hDC);
		SolidBrush brush(Color(GetRValue(m_crFillPointerCircleColor), GetGValue(m_crFillPointerCircleColor), GetBValue(m_crFillPointerCircleColor)));
		graph.FillEllipse(&brush, float(ctx - radius), float(cty - radius), float(2.0 * radius), float(2.0 * radius));
	}

	switch(m_nPointerShape)
	{
	case kPointerLine:
		DrawPointerLineShape(hDC, tht, radius, ctx, cty);
		break;
	case kPointerSingleBar:
	case kPointerDualBar:
		DrawPointerBarShape(hDC, tht, radius, ctx, cty);
		break;
	case kPointerSingleSword:
	case kPointerDualSword:
		DrawPointerSwordShape(hDC, tht, radius, ctx, cty);
		break;
	case kPointerArrow:
	case kPointerReverseArrow:
		DrawPointerArrowShape(hDC, tht, radius, ctx, cty);
		break;
	case kPointerOuterTriangle:
		DrawPointerTriangleShape(hDC, tht, radius, ctx, cty);
		break;
	case kPointerFillArc:
		DrawPointerFillArcShape(hDC, tht, radius, ctx, cty);
		break;
	}
	
	if(m_bShowStick)DrawStick(hDC, barRect);
}

void	CInstruBaseImpl::DrawPointerLineShape(HDC hDC, double tht, double radius, double ctx, double cty)
{
	if(m_nInstruShape != kInstruShapeCircle)return;

	HPEN hPen, hOldPen;
	hPen = CreatePen(PS_SOLID, m_nPointerLineSize, m_crPointerColor1);
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	
	MoveToEx(hDC, (int)ctx, (int)cty, NULL);
	LineTo(hDC, int(ctx + radius * cos(tht)), int(cty - radius * sin(tht)));
	
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void	CInstruBaseImpl::DrawPointerBarShape(HDC hDC, double tht, double radius, double ctx, double cty)
{
	if(m_nInstruShape != kInstruShapeCircle)return;

	POINT pPt[4];
	
	pPt[0].x = long(ctx + m_nPointerHalfWidthBase * cos(tht - myPi/2.0));
	pPt[0].y = long(cty - m_nPointerHalfWidthBase * sin(tht - myPi/2.0));
	pPt[1].x = long(ctx + m_nPointerHalfWidthBase * cos(tht + myPi/2.0));
	pPt[1].y = long(cty - m_nPointerHalfWidthBase * sin(tht + myPi/2.0));
	
	pPt[2].x = long(ctx + radius * cos(tht) + m_nPointerHalfWidthEnd * cos(tht + myPi/2.0));
	pPt[2].y = long(cty - radius * sin(tht) - m_nPointerHalfWidthEnd * sin(tht + myPi/2.0));
	pPt[3].x = long(ctx + radius * cos(tht) + m_nPointerHalfWidthEnd * cos(tht - myPi/2.0));
	pPt[3].y = long(cty - radius * sin(tht) - m_nPointerHalfWidthEnd * sin(tht - myPi/2.0));
	FillPolygon(hDC, m_crPointerColor1, pPt, 4);
	
	if(m_nPointerShape == kPointerDualBar)
	{
		pPt[2].x = long(ctx + radius * cos(tht + myPi) + m_nPointerHalfWidthEnd * cos(tht + myPi/2.0));
		pPt[2].y = long(cty - radius * sin(tht + myPi) - m_nPointerHalfWidthEnd * sin(tht + myPi/2.0));
		pPt[3].x = long(ctx + radius * cos(tht + myPi) + m_nPointerHalfWidthEnd * cos(tht - myPi/2.0));
		pPt[3].y = long(cty - radius * sin(tht + myPi) - m_nPointerHalfWidthEnd * sin(tht - myPi/2.0));
		FillPolygon(hDC, m_crPointerColor2, pPt, 4);
	}
	else
	{
		if(m_bShowPointerTail)
			DrawPointerTail(hDC, tht, radius, ctx, cty);
	}
}

void	CInstruBaseImpl::DrawPointerSwordShape(HDC hDC, double tht, double radius, double ctx, double cty)
{
	if(m_nInstruShape != kInstruShapeCircle)return;

	POINT pPt[5];
	
	pPt[0].x = long(ctx + m_nPointerHalfWidthBase * cos(tht - myPi/2.0));
	pPt[0].y = long(cty - m_nPointerHalfWidthBase * sin(tht - myPi/2.0));
	pPt[1].x = long(ctx + m_nPointerHalfWidthBase * cos(tht + myPi/2.0));
	pPt[1].y = long(cty - m_nPointerHalfWidthBase * sin(tht + myPi/2.0));
	
	pPt[2].x = long(ctx + (radius - m_nPointerSwordLenEnd) * cos(tht) + m_nPointerHalfWidthEnd * cos(tht + myPi/2.0));
	pPt[2].y = long(cty - (radius - m_nPointerSwordLenEnd) * sin(tht) - m_nPointerHalfWidthEnd * sin(tht + myPi/2.0));
	pPt[3].x = long(ctx + radius * cos(tht));
	pPt[3].y = long(cty - radius * sin(tht));
	pPt[4].x = long(ctx + (radius - m_nPointerSwordLenEnd) * cos(tht) + m_nPointerHalfWidthEnd * cos(tht - myPi/2.0));
	pPt[4].y = long(cty - (radius - m_nPointerSwordLenEnd) * sin(tht) - m_nPointerHalfWidthEnd * sin(tht - myPi/2.0));
	FillPolygon(hDC, m_crPointerColor1, pPt, 5);
	
	if(m_nPointerShape == kPointerDualSword)
	{
		pPt[2].x = long(ctx + (radius - m_nPointerSwordLenEnd) * cos(tht + myPi) + m_nPointerHalfWidthEnd * cos(tht + myPi/2.0));
		pPt[2].y = long(cty - (radius - m_nPointerSwordLenEnd) * sin(tht + myPi) - m_nPointerHalfWidthEnd * sin(tht + myPi/2.0));
		pPt[3].x = long(ctx + radius * cos(tht + myPi));
		pPt[3].y = long(cty - radius * sin(tht + myPi));
		pPt[4].x = long(ctx + (radius - m_nPointerSwordLenEnd) * cos(tht + myPi) + m_nPointerHalfWidthEnd * cos(tht - myPi/2.0));
		pPt[4].y = long(cty - (radius - m_nPointerSwordLenEnd) * sin(tht + myPi) - m_nPointerHalfWidthEnd * sin(tht - myPi/2.0));
		FillPolygon(hDC, m_crPointerColor2, pPt, 5);
	}
	else
	{
		if(m_bShowPointerTail)
			DrawPointerTail(hDC, tht, radius, ctx, cty);
	}
	
	if(m_bPointerShowSwordHandle)
	{
		Graphics graph(hDC);
		graph.SetSmoothingMode(SmoothingModeHighQuality);
		
		int thick = m_nPointerHalfWidthBase + m_nPointerSwordHandleRadiusInc;
		
		SolidBrush brush1(Color(GetRValue(m_crPointerColor1), GetGValue(m_crPointerColor1), GetBValue(m_crPointerColor1)));
		
		if(m_nPointerShape == kPointerSingleSword)
		{
			graph.FillEllipse(&brush1, float(ctx - thick), float(cty - thick), 2.0f * thick, 2.0f * thick );
		}
		else
		{
			SolidBrush brush2(Color(GetRValue(m_crPointerColor2), GetGValue(m_crPointerColor2), GetBValue(m_crPointerColor2)));
			
			graph.FillPie(&brush1, float(ctx - thick), float(cty - thick), 2.0f * thick, 2.0f * thick, Gdiplus::REAL(tht/myPi*180.0) , 180.0f);
			graph.FillPie(&brush2, float(ctx - thick), float(cty - thick), 2.0f * thick, 2.0f * thick, Gdiplus::REAL(tht/myPi*180.0 + 180.0), 180.0f);
		}
	}
}

void	CInstruBaseImpl::DrawPointerArrowShape(HDC hDC, double tht, double radius, double ctx, double cty)
{
	COLORREF cr1, cr2;
	cr1 = m_crPointerColor1;
	cr2 = m_crPointerColor2;
	
	POINT pPt[3];
	
	if(m_nPointerShape == kPointerArrow)
	{
		pPt[0].x = long(ctx + m_nPointerArrowDisplaceBase * cos(tht));
		pPt[0].y = long(cty - m_nPointerArrowDisplaceBase * sin(tht));
		pPt[1].x = long(ctx + radius * cos(tht));
		pPt[1].y = long(cty - radius * sin(tht));
		
		pPt[2].x = long(ctx + m_nPointerHalfWidthBase * cos(tht - myPi/2.0));
		pPt[2].y = long(cty - m_nPointerHalfWidthBase * sin(tht - myPi/2.0));
		FillPolygon(hDC, cr1, pPt, 3);
		pPt[2].x = long(ctx + m_nPointerHalfWidthBase * cos(tht + myPi/2.0));
		pPt[2].y = long(cty - m_nPointerHalfWidthBase * sin(tht + myPi/2.0));
		FillPolygon(hDC, cr2, pPt, 3);
	}
	else
	{
		pPt[0].x = long(ctx + m_nPointerArrowDistFromCenter * cos(tht));
		pPt[0].y = long(cty - m_nPointerArrowDistFromCenter * sin(tht));
		pPt[1].x = long(ctx + (radius - m_nPointerArrowDisplaceBase) * cos(tht));
		pPt[1].y = long(cty - (radius - m_nPointerArrowDisplaceBase) * sin(tht));
		
		pPt[2].x = long(ctx + radius * cos(tht) + m_nPointerHalfWidthBase * cos(tht - myPi/2.0));
		pPt[2].y = long(cty - radius * sin(tht) - m_nPointerHalfWidthBase * sin(tht - myPi/2.0));
		FillPolygon(hDC, cr1, pPt, 3);
		pPt[2].x = long(ctx + radius * cos(tht) + m_nPointerHalfWidthBase * cos(tht + myPi/2.0));
		pPt[2].y = long(cty + radius * cos(tht) - m_nPointerHalfWidthBase * sin(tht + myPi/2.0));
		FillPolygon(hDC, cr2, pPt, 3);
	}
}

void	CInstruBaseImpl::DrawPointerTriangleShape(HDC hDC, double tht, double radius, double ctx, double cty)
{
	Graphics graph(hDC);

	PointF pPt[3];
	pPt[0].X = float(ctx + radius * cos(tht));
	pPt[0].Y = float(cty - radius * sin(tht));
	pPt[1].X = float(ctx + (radius - m_nPointerSwordLenEnd) * cos(tht) + m_nPointerHalfWidthEnd * cos(tht - myPi/2.0));
	pPt[1].Y = float(cty - (radius - m_nPointerSwordLenEnd) * sin(tht) - m_nPointerHalfWidthEnd * sin(tht - myPi/2.0));
	pPt[2].X = float(ctx + (radius - m_nPointerSwordLenEnd) * cos(tht) + m_nPointerHalfWidthEnd * cos(tht + myPi/2.0));
	pPt[2].Y = float(cty - (radius - m_nPointerSwordLenEnd) * sin(tht) - m_nPointerHalfWidthEnd * sin(tht + myPi/2.0));
	
	SolidBrush brush(Color(255, 0, 0));
	graph.FillPolygon(&brush, pPt, 3, FillModeAlternate);
}

void	CInstruBaseImpl::DrawPointerFillArcShape(HDC hDC, double tht, double radius, double ctx, double cty)
{
	Graphics graph(hDC);
	SolidBrush brush(Color(GetRValue(m_crPointerColor1), GetGValue(m_crPointerColor1), GetBValue(m_crPointerColor1)));
	graph.FillPie(&brush, float(ctx - radius), float(cty - radius), float(2.0*radius), float(2.0*radius), float(-m_fAngleStart/myPi*180.0), -float((tht-m_fAngleStart)/myPi*180.0));
}

void	CInstruBaseImpl::DrawPointerTail(HDC hDC, double tht, double radius, double ctx, double cty)
{
	if(m_nInstruShape != kInstruShapeCircle)return;
	
	POINT pPt[4];
	
	pPt[0].x = long(ctx + m_nPointerHalfWidthBase * cos(tht - myPi/2.0));
	pPt[0].y = long(cty - m_nPointerHalfWidthBase * sin(tht - myPi/2.0));
	pPt[1].x = long(ctx + m_nPointerHalfWidthBase * cos(tht + myPi/2.0));
	pPt[1].y = long(cty - m_nPointerHalfWidthBase * sin(tht + myPi/2.0));
	
	pPt[2].x = long(ctx - m_nPointerLenTail * cos(tht) + m_nPointerHalfWidthEnd * cos(tht + myPi/2.0));
	pPt[2].y = long(cty + m_nPointerLenTail * sin(tht) - m_nPointerHalfWidthEnd * sin(tht + myPi/2.0));
	pPt[3].x = long(ctx - m_nPointerLenTail * cos(tht) + m_nPointerHalfWidthEnd * cos(tht - myPi/2.0));
	pPt[3].y = long(cty + m_nPointerLenTail * sin(tht) - m_nPointerHalfWidthEnd * sin(tht - myPi/2.0));
	FillPolygon(hDC, m_crPointerColor1, pPt, 4);
}

RECT	CInstruBaseImpl::GetPointerRect(HDC hDC, RECT barRect)
{
	RECT axisRect = GetAxisRect(hDC, barRect);
	double tickL = GetSideTickLengh(false);
	InflateRect(&axisRect, int(-tickL), int(-tickL));
	InflateRect(&axisRect, -m_nPointerWithdraw, -m_nPointerWithdraw);

	return axisRect;
}

void	CInstruBaseImpl::GetPointerParams(HDC hDC, RECT barRect, double &tht, double &radius, double &ctx, double &cty)
{
	RECT pointerRect = GetPointerRect(hDC, barRect);
	
	double val = m_fCurrValue;
	//if(val < 0 || val > 4)val = val - 4*floor(val/4.0);
	if(val < m_pfRange[0] || val < m_pfRange[1]) val = val - (m_pfRange[1] - m_pfRange[0])*floor(val/(m_pfRange[1] - m_pfRange[0]));
	
	double sgn = 1.0;
	if(m_bClockWise)sgn = -1.0;
	
	//double tht = m_fAngleStart + val * myPi/2.0 * sgn;
	tht = m_fAngleStart + (val - m_pfRange[0]) * m_fAngleRange/(m_pfRange[1] - m_pfRange[0]) * sgn;
	
	radius = Width(pointerRect)/2.0;
	
	ctx = (pointerRect.left + pointerRect.right)/2.0;
	cty = (pointerRect.bottom + pointerRect.top)/2.0;
}

void	CInstruBaseImpl::DrawWheelBar(HDC hDC, RECT barRect)
{
	DrawEdge(hDC, &barRect, EDGE_BUMP, BF_RECT|BF_MIDDLE);
	
	double radius;
	radius = Width(barRect)/2.0/sin(m_fWheelWindowAngle/2.0);

	m_fAngleStart = -m_fAngleRange/2.0;
	double curAngle;
	curAngle = m_fAngleStart + m_fAngleRange * (m_fCurrValue - m_pfRange[0])/(m_pfRange[1] - m_pfRange[0]);
	double dAngle;
	dAngle = m_fAngleRange/m_nWheelEdges;

	double hW;
	double ct;
	if(m_bWheelHorizontal)
	{
		hW = Width(barRect)/2.0;
		ct = GetCenterX(barRect);
	}
	else
	{
		hW = Height(barRect)/2.0;
		ct = GetCenterY(barRect);
	}
		
	RECT edgeRect = barRect;

	InflateRect(&edgeRect, -2, -2);

	double x, y;
	double angle;
	double da;
	double a1, a2;
	for(int i=-m_nWheelEdges/2; i<=m_nWheelEdges/2; i++)
	{
		angle = i * dAngle;
		da = curAngle - dAngle*floor(curAngle/dAngle);
		angle += da;
		angle += dAngle/2.0;
		if(!ValInInterval(angle, -m_fWheelWindowAngle/2.0, m_fWheelWindowAngle/2.0))continue;

		x = radius * cos(angle);
		y = radius * sin(angle);
		if(x < 0.0)continue;

		a1 = angle - dAngle / 2.0 * m_fWheelEdgeRatio;
		a2 = angle + dAngle / 2.0 * m_fWheelEdgeRatio;
		if(a1 < -m_fWheelWindowAngle/2.0)a1 = -m_fWheelWindowAngle/2.0;
		if(a2 > m_fWheelWindowAngle/2.0)a2 = m_fWheelWindowAngle/2.0;
		
		if(m_bWheelHorizontal)
		{
			edgeRect.left = long(ct + radius * sin(a1));
			edgeRect.right = long(ct + radius * sin(a2));
			if(edgeRect.right <= barRect.left || edgeRect.left > barRect.right)continue;
			if(edgeRect.left <= barRect.left)edgeRect.left = barRect.left;
			if(edgeRect.right >= barRect.right)edgeRect.right = barRect.right;
			InflateRect(&edgeRect, -1, 0);
			DrawEdge(hDC, &edgeRect, EDGE_SUNKEN, BF_LEFT|BF_RIGHT|BF_TOP|BF_BOTTOM|BF_MIDDLE);
		}
		else
		{
			edgeRect.bottom = long(ct - radius * sin(a1));
			edgeRect.top = long(ct - radius * sin(a2));
			if(edgeRect.top <= barRect.bottom || edgeRect.bottom > barRect.top)continue;
			if(edgeRect.top <= barRect.top)edgeRect.top = barRect.top;
			if(edgeRect.bottom >= barRect.bottom)edgeRect.bottom = barRect.bottom;
			InflateRect(&edgeRect, -1, 0);
			DrawEdge(hDC, &edgeRect, EDGE_SUNKEN, BF_LEFT|BF_RIGHT|BF_TOP|BF_BOTTOM|BF_MIDDLE);
		}
	}

	if(ValInInterval(curAngle, -m_fWheelWindowAngle/2.0, m_fWheelWindowAngle/2.0))
	{
		double x1, x2;
		x1 = radius * sin(curAngle - m_fCurrSizeAngle/2.0);
		x2 = radius * sin(curAngle + m_fCurrSizeAngle/2.0);
		Graphics graph(hDC);
		SolidBrush brush(Color(GetRValue(m_crCurrColor), GetGValue(m_crCurrColor), GetBValue(m_crCurrColor)));
		if(m_bWheelHorizontal)
		{
			graph.FillRectangle(&brush, float(ct + x1), float(barRect.top + 2), float(x2 - x1), float(Height(barRect) - 4));
		}
		else
		{
			graph.FillRectangle(&brush, float(barRect.left + 2), float(ct + x1), float(Width(barRect) - 4), float(x2 - x1));
		}
	}
}