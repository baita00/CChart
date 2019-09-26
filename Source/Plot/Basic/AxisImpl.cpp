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

#include "AxisImpl.h"
#include "Global.h"
#include "PlotImpl.h"
#include "Ranges.h"
#include "../Resources/PlotResources.h"
#include <time.h>
#include "XYPlotImpl.h"

using namespace NsCChart;

CAxisImpl::CAxisImpl(CPlotImpl *plot)
{
	m_pPlot = plot;

	m_nRealTitleHeight = m_lfTitleFont.lfHeight;
	m_nRealTickLabelHeight = m_lfLabelFont.lfHeight;
	
	if(CGlobal::m_bLoadDefPatterns)
	{
		HBITMAP hb = (HBITMAP)LoadImage(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDB_BACKGROUND_IMAGE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		if(hb)
		{
			SetPattern(hb);
			DeleteObject(hb);
		}
	}

	m_bIndependent = false;
	m_pPlotDep = plot;
	bool bD = CGlobal::m_bAddDefAxes;
	CGlobal::m_bAddDefAxes = false;
	m_pPlotIndep = new CXYPlotImpl;//如果在CPlotImpl类的构造函数中new axis, 会引起无限循环
	CGlobal::m_bAddDefAxes = bD;
}


CAxisImpl::~CAxisImpl()
{
	if(m_pPlotIndep)
	{
		delete m_pPlotIndep;
		m_pPlot = 0;
	}
}

bool	CAxisImpl::IsIndependent()
{
	return m_bIndependent;
}

void	CAxisImpl::SetIndependent(bool indep)
{
	m_bIndependent = indep;

	if(indep) m_pPlot = m_pPlotIndep;
	else m_pPlot = m_pPlotDep;
}


RECT	CAxisImpl::GetPlotRect( HDC hDC )//, RECT clientRect, RECT chartRect )
{
	RECT chartRect = GetPlot()->GetLastPlotRect();
	RECT plotRect;
	SIZE axisDims, paddingDims;
	plotRect = chartRect;
	axisDims = GetMinDisplaySize( hDC );

	bool bX = IsXAxis() ;
	bool bFloat = (bX?(m_pPlot)->IsFloatXTicks():(m_pPlot)->IsFloatYTicks());
	
	if(!bFloat)
		paddingDims = GetPaddingSize( hDC, plotRect );
	else
		paddingDims = GetPaddingSizeForFloatTicks(hDC, plotRect);
	
	switch( GetLocation() )
	{
	case kLocationBottom:
		plotRect.top=chartRect.bottom + m_nAxisGap;
		plotRect.bottom=plotRect.top+axisDims.cy;

		if(!m_pPlot->IsXOpposite())
		{
			if(!m_bDespiteLow)plotRect.left-=paddingDims.cx;
			if(!m_bDespiteHigh)plotRect.right+=paddingDims.cy;
		}
		else
		{
			if(!m_bDespiteHigh)plotRect.left-=paddingDims.cy;
			if(!m_bDespiteLow)plotRect.right+=paddingDims.cx;
		}
		break;
	case kLocationTop:
		plotRect.bottom=chartRect.top - m_nAxisGap;
		plotRect.top=plotRect.bottom-axisDims.cy;

		if(!m_pPlot->IsXOpposite())
		{
			if(!m_bDespiteLow)plotRect.left-=paddingDims.cx;
			if(!m_bDespiteHigh)plotRect.right+=paddingDims.cy;
		}
		else
		{
			if(!m_bDespiteHigh)plotRect.left-=paddingDims.cy;
			if(!m_bDespiteLow)plotRect.right+=paddingDims.cx;
		}
		break;
	case kLocationLeft:
		plotRect.right=chartRect.left - m_nAxisGap;
		plotRect.left=plotRect.right-axisDims.cx;

		if(!m_pPlot->IsYOpposite())
		{
			if(!m_bDespiteLow)plotRect.bottom+=paddingDims.cx;
			if(!m_bDespiteHigh)plotRect.top-=paddingDims.cy;
		}
		else
		{
			if(!m_bDespiteHigh)plotRect.bottom+=paddingDims.cy;
			if(!m_bDespiteLow)plotRect.top-=paddingDims.cx;
		}
		break;
	case kLocationRight:
		plotRect.left=chartRect.right + m_nAxisGap;
		plotRect.right=plotRect.left+axisDims.cx;

		if(!m_pPlot->IsYOpposite())
		{
			if(!m_bDespiteLow)plotRect.bottom+=paddingDims.cx;
			if(!m_bDespiteHigh)plotRect.top-=paddingDims.cy;
		}
		else
		{
			if(!m_bDespiteHigh)plotRect.bottom+=paddingDims.cy;
			if(!m_bDespiteLow)plotRect.top-=paddingDims.cx;
		}
		break;
	case kLocationCenterVL:
		plotRect.right = (chartRect.left + chartRect.right)/2;
		plotRect.left = plotRect.right - axisDims.cx;

		if(!m_pPlot->IsYOpposite())
		{
			if(!m_bDespiteLow)plotRect.bottom+=paddingDims.cx;
			if(!m_bDespiteHigh)plotRect.top-=paddingDims.cy;
		}
		else
		{
			if(!m_bDespiteHigh)plotRect.bottom+=paddingDims.cy;
			if(!m_bDespiteLow)plotRect.top-=paddingDims.cx;
		}
		break;
	case kLocationCenterVR:
		plotRect.left = (chartRect.left + chartRect.right)/2;
		plotRect.right = plotRect.left + axisDims.cx;
		
		if(!m_pPlot->IsYOpposite())
		{
			if(!m_bDespiteLow)plotRect.bottom+=paddingDims.cx;
			if(!m_bDespiteHigh)plotRect.top-=paddingDims.cy;
		}
		else
		{
			if(!m_bDespiteHigh)plotRect.bottom+=paddingDims.cy;
			if(!m_bDespiteLow)plotRect.top-=paddingDims.cx;
		}
		break;
	case kLocationCenterHB:
		plotRect.top = (chartRect.top + chartRect.bottom)/2;
		plotRect.bottom = plotRect.top + axisDims.cy;

		if(!m_pPlot->IsXOpposite())
		{
			if(!m_bDespiteLow)plotRect.left-=paddingDims.cx;
			if(!m_bDespiteHigh)plotRect.right+=paddingDims.cy;
		}
		else
		{
			if(!m_bDespiteHigh)plotRect.left-=paddingDims.cy;
			if(!m_bDespiteLow)plotRect.right+=paddingDims.cx;
		}
		break;
	case kLocationCenterHT:
		plotRect.bottom = (chartRect.top + chartRect.bottom)/2;
		plotRect.top = plotRect.bottom + axisDims.cy;
		
		if(!m_pPlot->IsXOpposite())
		{
			if(!m_bDespiteLow)plotRect.left-=paddingDims.cx;
			if(!m_bDespiteHigh)plotRect.right+=paddingDims.cy;
		}
		else
		{
			if(!m_bDespiteHigh)plotRect.left-=paddingDims.cy;
			if(!m_bDespiteLow)plotRect.right+=paddingDims.cx;
		}
		break;
	default:
		break;
	}
	return plotRect;
}


RECT	CAxisImpl::GetPlotRectWithoutBar( HDC hDC )//, RECT clientRect, RECT chartRect )
{
	RECT axisRect = GetPlotRect(hDC);//, clientRect, chartRect);
	if(m_bColorBar)
	{
		switch(GetLocation())
		{
		case kLocationBottom:
			axisRect.top += m_nColorBarWidth;
			break;
		case kLocationTop:
			axisRect.bottom -= m_nColorBarWidth;
			break;
		case kLocationLeft:
			axisRect.right -= m_nColorBarWidth;
			break;
		case kLocationRight:
			axisRect.left += m_nColorBarWidth;
			break;
		}
	}
	return axisRect;
}


RECT	CAxisImpl::GetBarRect( HDC hDC )//, RECT clientRect, RECT chartRect )
{
	RECT chartRect = GetPlot()->GetLastPlotRect();
	RECT barRect = chartRect;
	
	int margino = m_nColorBarMarginOuter;
	if(margino > m_nColorBarWidth/3) margino = m_nColorBarWidth/3;
	int margini = m_nColorBarMarginInner;
	if(margini > m_nColorBarWidth/3) margini = m_nColorBarWidth/3;

	switch(GetLocation())
	{
	case kLocationBottom:
		barRect.top = chartRect.bottom;
		barRect.bottom = barRect.top + m_nColorBarWidth;
		barRect.top += margino;
		barRect.bottom -= margini;
		break;
	case kLocationCenterHB:
		barRect.top = (chartRect.top + chartRect.bottom)/2;
		barRect.bottom = barRect.top + m_nColorBarWidth;
		barRect.top += margino;
		barRect.bottom -= margini;
		break;
	case kLocationTop:
		barRect.bottom = chartRect.top;
		barRect.top = barRect.bottom - m_nColorBarWidth;
		barRect.top += margini;
		barRect.bottom -= margino;
		break;
	case kLocationCenterHT:
		barRect.bottom = (chartRect.top + chartRect.bottom)/2;
		barRect.top = barRect.bottom - m_nColorBarWidth;
		barRect.top += margini;
		barRect.bottom -= margino;
		break;
	case kLocationLeft:
		barRect.right = chartRect.left;
		barRect.left = barRect.right - m_nColorBarWidth;
		barRect.left += margini;
		barRect.right -= margino;
		break;
	case kLocationCenterVL:
		barRect.right = (chartRect.left + chartRect.right)/2;
		barRect.left = barRect.right - m_nColorBarWidth;
		barRect.left += margini;
		barRect.right -= margino;
		break;
	case kLocationRight:
		barRect.left = chartRect.right;
		barRect.right = barRect.left + m_nColorBarWidth;
		barRect.left += margino;
		barRect.right -= margini;
		break;
	case kLocationCenterVR:
		barRect.left = (chartRect.left + chartRect.right)/2;
		barRect.right = barRect.left + m_nColorBarWidth;
		barRect.left += margino;
		barRect.right -= margini;
		break;
	}
	return barRect;
}


int	CAxisImpl::GetMaxTickLength()
{
	int mtlen=0;
	if(m_bShowMajorTicks)
	{
		// Addint the major tick
		switch(m_nTickPosition)
		{
		case kTickPositionInner:
			if(m_nTickLength>mtlen)mtlen=m_nTickLength;
			break;
		case kTickPositionBoth:
			if(m_nTickLength/2>mtlen)mtlen=m_nTickLength/2;
			break;
		default:
			break;
		}
	}
	
	if(m_bShowMinorTicks)
	{
		// Addin the minor tick
		switch(m_nMinorTickPosition)
		{
		case kTickPositionInner:
			if(m_nMinorTickLength>mtlen)mtlen=m_nMinorTickLength;
			break;
		case kTickPositionBoth:
			if(m_nMinorTickLength/2>mtlen)mtlen=m_nMinorTickLength/2;
			break;
		default:
			break;
		}
	}
	return mtlen;
}


SIZE CAxisImpl::GetRealTickLabelDimension(HDC hDC)//, RECT chartRect)
{
	RECT chartRect = GetPlot()->GetLastPlotRect();

	bool			keepGoing = true;
//	HFONT			hFont,hOldFont;
	LOGFONT			logFont;

	int				nMaxLabelWidth;
	int				i;
	tstring			tickLabel;
	int				tickHeight=0;
	SIZE			labelExtent={0,0};

	logFont = m_lfLabelFont;

	bool bX = IsXAxis();
	int	nTicks = (bX?m_pPlot->GetXTickCount():m_pPlot->GetYTickCount());//GetTickCount();
	// Set up font and make sure that the font size will fit
	// the width or height of our chart
	while( keepGoing == true )
	{
		nMaxLabelWidth=0;
		for(i=0; i<nTicks+2; i++)
		{
			tickLabel=GetMajorTickLabel(i);
			//GetTextExtentPoint(hDC, tickLabel.c_str(),tickLabel.length(),&labelExtent );
			MyGetTextExtentPoint(hDC, &logFont, tickLabel, 0, &labelExtent );
			if(labelExtent.cx>nMaxLabelWidth)nMaxLabelWidth=labelExtent.cx;
		}
		if(IsXAxis())
		{
			if( nMaxLabelWidth > (m_bTwoStoreyX?2:1)*abs(chartRect.right - chartRect.left) / (nTicks<0?(nTicks+2):(nTicks+1)) && logFont.lfHeight > 4 )
				logFont.lfHeight--;
			else
				keepGoing = false;
		}
		else
		{
			if( labelExtent.cy > abs(chartRect.bottom - chartRect.top) / (nTicks<0?(nTicks+2):(nTicks+1)) && logFont.lfHeight > 4  )
				logFont.lfHeight--;
			else
				keepGoing = false;
		}
	}

	m_nRealTickLabelHeight = logFont.lfHeight;
	labelExtent.cx = nMaxLabelWidth;
	return labelExtent;
}


int	CAxisImpl::GetRealTitleHeight(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	RECT chartRect = GetPlot()->GetLastPlotRect();

	bool keepGoing;
	LOGFONT logFont;
	SIZE tmSize;

	keepGoing = true;
	logFont=m_lfTitleFont;
	if( m_nTitlePosition == 0 )
	{
		while( keepGoing == true )
		{
			MyGetTextExtentPoint(hDC,&logFont,m_strTitle,0,&tmSize);

			if(IsXAxis())
			{
				if( tmSize.cx > abs(chartRect.right - chartRect.left) )
					logFont.lfHeight--;
				else
					keepGoing = false;
			}
			else
			{
				if( tmSize.cx > abs(chartRect.bottom - chartRect.top) )
					logFont.lfHeight--;
				else
					keepGoing = false;
			}

			if( logFont.lfHeight <= 4 ) keepGoing = false;
		}
	}
	m_nRealTitleHeight = logFont.lfHeight;
	return logFont.lfHeight;
}


RECT CAxisImpl::GetTitleRect(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	RECT chartRect = GetPlot()->GetLastPlotRect();
	RECT textRect= GetPlot()->GetLastClientRect();;

	LOGFONT			logFont;
	SIZE			tmSize;

	long			width,height;
	SIZE			paddingSize;//=GetPaddingSize(hDC);
	tstring			tickLabel;
	SIZE			MaxLabelExtent={0,0};

	bool bX = IsXAxis() ;
	bool bFloat = (bX?m_pPlot->IsFloatXTicks():m_pPlot->IsFloatYTicks());
	if(!bFloat)
		paddingSize = GetPaddingSize( hDC, chartRect );
	else
		paddingSize = GetPaddingSizeForFloatTicks(hDC, chartRect);

	//calc the size of tick labels
	if(m_bTickLabelShow)
	{
		MaxLabelExtent = GetRealTickLabelDimension(hDC);//, chartRect);
		if(IsXAxis() && IsTwoStoreyX())MaxLabelExtent.cy *= 2;
	}

	logFont=m_lfTitleFont;
	logFont.lfHeight = GetRealTitleHeight(hDC);//, clientRect, chartRect);

	if(!IsXAxis() && m_bYTitleVerticle)
	{
		HFONT hFont, hOldFont;
		logFont.lfEscapement = 900;
		logFont.lfOrientation = 900;
		hFont = CreateFontIndirect(&logFont);
		hOldFont = (HFONT)SelectObject(hDC, hFont);
		GetTextExtentPoint32(hDC, m_strTitle.c_str(), (int)m_strTitle.length(), &tmSize);
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);
	}
	else
	{
		MyGetTextExtentPoint(hDC,&logFont,m_strTitle, 0,&tmSize);
	}

	RECT plotRect = GetPlotRect(hDC);//, clientRect, chartRect);
	// Find the rectangle that encloses our axis title
	switch( GetLocation() )
	{
	case kLocationLeft:
	case kLocationCenterVL:
		textRect = chartRect;
		if(GetLocation()==kLocationLeft)
			textRect.right = chartRect.left - m_nTitleGap - MaxLabelExtent.cx - GetMaxTickLength() - m_nLabelGap - m_nAxisGap;
		else 
			textRect.right = (chartRect.left + chartRect.right)/2  - m_nTitleGap - MaxLabelExtent.cx - GetMaxTickLength() - m_nLabelGap;
		textRect.left = textRect.right - (m_bYTitleVerticle?tmSize.cy:tmSize.cx);

		//ugly repair
		if(textRect.left < plotRect.left)OffsetRect(&textRect, plotRect.left-textRect.left, 0);

		height = chartRect.bottom - chartRect.top;
		
		// Adjust position for font orientation and size
		if(m_nTitlePosition == 0)
		{
			textRect.top = chartRect.top + height/2 - (m_bYTitleVerticle?tmSize.cx:tmSize.cy)/2;
			textRect.bottom = textRect.top + (m_bYTitleVerticle?tmSize.cx:tmSize.cy);
		}
		else if(m_nTitlePosition<0)
		{
			textRect.bottom = chartRect.bottom;
			textRect.top = textRect.bottom - (m_bYTitleVerticle?tmSize.cx:tmSize.cy);
		}
		else
		{
			textRect.top = chartRect.top;
			textRect.bottom = textRect.top + (m_bYTitleVerticle?tmSize.cx:tmSize.cy);
		}
		if(IsShowColorBar())
		{
			OffsetRect(&textRect, -m_nColorBarWidth, 0);
		}
		break;
	case kLocationRight:
	case kLocationCenterVR:
		textRect = chartRect;
		if(GetLocation()==kLocationRight)
			textRect.left = chartRect.right + m_nTitleGap + MaxLabelExtent.cx + GetMaxTickLength() + m_nLabelGap + m_nAxisGap;
		else
			textRect.left = (chartRect.left + chartRect.right)/2  + m_nTitleGap + MaxLabelExtent.cx + GetMaxTickLength() + m_nLabelGap;
		textRect.right = textRect.left + (m_bYTitleVerticle?tmSize.cy:tmSize.cx);

		// ugly repair
		if(textRect.right > plotRect.right)OffsetRect(&textRect, plotRect.right-textRect.right, 0);

		height = chartRect.bottom-chartRect.top;

		if(m_nTitlePosition == 0)
		{
			textRect.top = chartRect.top + height/2 - (m_bYTitleVerticle?tmSize.cx:tmSize.cy)/2;
			textRect.bottom = textRect.top + (m_bYTitleVerticle?tmSize.cx:tmSize.cy);
		}
		else if(m_nTitlePosition<0)
		{
			textRect.bottom = chartRect.bottom;
			textRect.top = textRect.bottom - (m_bYTitleVerticle?tmSize.cx:tmSize.cy);
		}
		else
		{
			textRect.top = chartRect.top;
			textRect.bottom = textRect.top + (m_bYTitleVerticle?tmSize.cx:tmSize.cy);
		}
		if(IsShowColorBar())
		{
			OffsetRect(&textRect, m_nColorBarWidth, 0);
		}
		break;
	case kLocationTop:
	case kLocationCenterHT:
		textRect = chartRect;
		if(GetLocation()==kLocationTop)
			textRect.bottom = chartRect.top - m_nTitleGap - MaxLabelExtent.cy - GetMaxTickLength() -m_nLabelGap - m_nAxisGap;
		else
			textRect.bottom = (chartRect.top + chartRect.bottom)/2 - m_nTitleGap - MaxLabelExtent.cy - GetMaxTickLength() - m_nLabelGap;
		textRect.top = textRect.bottom - tmSize.cy;

		width = chartRect.right - chartRect.left; 

		if(m_nTitlePosition == 0)
		{
			textRect.left = chartRect.left + width/2 - tmSize.cx/2;
			textRect.right = textRect.left + tmSize.cx;
		}
		else if(m_nTitlePosition < 0)
		{
			textRect.left = chartRect.left;
			textRect.right = textRect.left + tmSize.cx;
		}
		else
		{
			textRect.right = chartRect.right;
			textRect.left = textRect.right - tmSize.cx;
		}
		

		logFont.lfEscapement = 0;
		logFont.lfOrientation = 0;

		if(IsShowColorBar())
		{
			OffsetRect(&textRect, 0, -m_nColorBarWidth);
		}

		break;
	case kLocationBottom:
	case kLocationCenterHB:
		textRect = chartRect;
		if(GetLocation()==kLocationBottom)
			textRect.top = chartRect.bottom + m_nTitleGap + MaxLabelExtent.cy + GetMaxTickLength() + m_nLabelGap + m_nAxisGap;
		else
			textRect.top = (chartRect.top + chartRect.bottom)/2 + m_nTitleGap + MaxLabelExtent.cy + GetMaxTickLength() + m_nLabelGap;
		textRect.bottom = textRect.top + tmSize.cy;

		width = chartRect.right - chartRect.left; 
		if(m_nTitlePosition == 0)
		{
			textRect.left = chartRect.left + width/2 - tmSize.cx/2;
			textRect.right = textRect.left + tmSize.cx;
		}
		else if(m_nTitlePosition < 0)
		{
			textRect.left = chartRect.left;
			textRect.right = textRect.left + tmSize.cx;
		}
		else
		{
			textRect.right = chartRect.right;
			textRect.left = textRect.right - tmSize.cx;
		}

		logFont.lfEscapement = 0;
		logFont.lfOrientation = 0;

		if(IsShowColorBar())
		{
			OffsetRect(&textRect, 0, m_nColorBarWidth);
		}

		break;
	}

	if(m_bSciNotation && GetPowerMag() != 0 && m_bTickLabelShow)
	{
		if(!IsXAxis())
		{
			SIZE sizeSn = GetSizeSN(hDC);
			if(m_nLocation == kLocationLeft || m_nLocation == kLocationCenterVL)
			{
				OffsetRect(&textRect, -sizeSn.cx, 0);
			}
			else
			{
				OffsetRect(&textRect, sizeSn.cx, 0);
			}
		}
	}
	return textRect;
}


RECT CAxisImpl::GetPseudoChartRect( )//RECT chartRect )
{
	RECT chartRect = GetPlot()->GetLastPlotRect();

	if(!m_bColorBar)
		return chartRect;

	RECT psRect = chartRect;
	switch(GetLocation())
	{
	case kLocationBottom:
		psRect.bottom += m_nColorBarWidth;
		break;
	case kLocationTop:
		psRect.top -= m_nColorBarWidth;
		break;
	case kLocationLeft:
		psRect.left -= m_nColorBarWidth;
		break;
	case kLocationRight:
		psRect.right += m_nColorBarWidth;
		break;
	default:
		break;
	}
	return psRect;
}


void CAxisImpl::OnDraw(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	if(!m_bVisible) return;

	if(m_bIndependent)
	{
		m_pPlotIndep->SetLastClientRect(m_pPlotDep->GetLastClientRect());
		m_pPlotIndep->SetLastPlotRect(m_pPlotDep->GetLastPlotRect());
	}
	
	DrawFrame(hDC);//, clientRect, chartRect);
	DrawColorBar(hDC);//, clientRect, chartRect);
	DrawTitle(hDC);//, clientRect, chartRect );
	DrawAxisLine(hDC);//,clientRect,chartRect);
	DrawAxisArrows(hDC);//, clientRect, chartRect);
	DrawMajorTicks(hDC);//,clientRect,chartRect);
	DrawMinorTicks(hDC);//,clientRect,chartRect);
	DrawTickLabels(hDC);//,clientRect,chartRect);
	DrawSNPower(hDC);//, clientRect, chartRect);
}


void	CAxisImpl::DrawFrame(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;

	if(m_bSelected)
	{
		if(m_pPlot->GetLightLineSize()<=0)return;
		hPen = CreatePen(m_pPlot->GetLigntLineStyle(), m_pPlot->GetLightLineSize(), m_pPlot->GetSelectColor());
	}
	else if(m_bLighted)
	{
		if(m_pPlot->GetLightLineSize()<=0)return;
		hPen = CreatePen(m_pPlot->GetLigntLineStyle(), m_pPlot->GetLightLineSize(), m_pPlot->GetLightColor());
	}
	else if(m_bBorderShow)
	{
		hPen = CreatePen(PS_SOLID, m_nBorderSize, m_crAxisColor);
	}
	else
	{
		hPen = (HPEN)GetStockObject(NULL_PEN);
	}

	switch(m_nBrushType)
	{
	case kBrushSolid:
		hBrush = CreateSolidBrush(m_crBgColor);
		break;
	case kBrushHatch:
		hBrush = CreateHatchBrush(m_nHatchStyle, m_crHatchColor);
		break;
	case kBrushPattern:
		hBrush = CreatePatternBrush(m_hPattern);
		break;
	default:
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	}
	
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	RECT rect = GetPlotRectWithoutBar( hDC );//, clientRect, chartRect );
	Rectangle(hDC,rect.left,rect.top,rect.right,rect.bottom);

	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);
}


void CAxisImpl::DrawTitle(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	if( m_strTitle == _TEXT("") || !m_bShowTitle )return;

	//unsigned int	textMode;
	RECT	textRect;
	HFONT	hFont,hOldFont;
	LOGFONT logFont;
	COLORREF titlecolor;

	textRect = GetTitleRect(hDC);//, clientRect,chartRect);// also set height of title font and ticklabel font
	// Adding offset
	OffsetRect(&textRect, m_sizeTitleOffset.cx, m_sizeTitleOffset.cy);
	
	logFont=m_lfTitleFont;
	logFont.lfHeight = m_nRealTitleHeight;

	if(m_bSameColor)
		titlecolor = m_crAxisColor;
	else
		titlecolor = m_crTitleColor;
	
	SetBkMode( hDC,TRANSPARENT );
	SetTextColor( hDC,titlecolor );
	//Rectangle(hDC,textRect.left,textRect.top,textRect.right,textRect.bottom);

	// Find the rectangle that encloses our axis title
	int x0,y0;
	
	x0 = textRect.left;
	y0 = textRect.top;

	if(!IsXAxis() && m_bYTitleVerticle)
	{
		switch( GetLocation() )
		{
		case kLocationLeft:
		case kLocationCenterVL:
			logFont.lfEscapement = 900;
			logFont.lfOrientation = 900;
			x0 = textRect.left;
			y0 = textRect.bottom;
			break;
		case kLocationRight:
		case kLocationCenterVR:
			logFont.lfEscapement = 2700;
			logFont.lfOrientation = 2700;
			x0 = textRect.right;
			y0 = textRect.top;
			break;
		default:
			break;
		}
	}
	
	
	hFont = CreateFontIndirect(&logFont);
	hOldFont = (HFONT)SelectObject(hDC,hFont);
	
	//if(GetLocation() == kLocationBottom || GetLocation() == kLocationTop
	//	|| GetLocation() == kLocationCenterHB || GetLocation() == kLocationCenterHT)
	if(IsXAxis() || !m_bYTitleVerticle)
	{
		RECT rect;
		rect.left = rect.right = x0;
		rect.bottom = rect.top = y0;
		DrawText(hDC, m_strTitle.c_str(), (int)m_strTitle.length(), &rect, DT_CALCRECT);
		DrawText(hDC, m_strTitle.c_str(), (int)m_strTitle.length(), &rect, DT_CENTER);
	}
	else
	{
		TextOut(hDC, x0, y0, m_strTitle.c_str(), (int)m_strTitle.length());
	}

	SelectObject(hDC, hOldFont );
	DeleteObject(hFont);

	if(m_bShowTitleRect)
	{
		HPEN hPen, hOldPen;
		hPen = CreatePen(m_nTitleRectLineStyle, m_nTitleRectLineSize, m_crTitleColor);
		hOldPen = (HPEN)SelectObject(hDC, hPen);
		HBRUSH hBrush, hOldBrush;
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

		//FrameRect(hDC, &textRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		Rectangle(hDC, textRect.left, textRect.top, textRect.right, textRect.bottom);
		
		SelectObject(hDC, hOldBrush);
		SelectObject(hDC, hOldPen);
		DeleteObject(hPen);
	}
}


void CAxisImpl::DrawAxisLine(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	if(!m_bShowAxisLine)return;
	
	RECT chartRect = GetPlot()->GetLastPlotRect();
	//POINT tmpPoint;
	bool bX = IsXAxis() ;
	double pfRange[2];
	if(bX) m_pPlot->GetXRange(pfRange);
	else m_pPlot->GetYRange(pfRange);
	if( fabs( pfRange[1] - pfRange[0] ) <= fMinVal )return;

	if(m_bColorBar)
		chartRect = GetPseudoChartRect( );//chartRect);

	HPEN		hPen,hOldPen;

	hPen = CreatePen( m_nLineStyle, m_nLineSize, m_crAxisColor );
	hOldPen = (HPEN)SelectObject(hDC,hPen);
	//firstly, draw the axis line
	switch( GetLocation() )
	{
	case kLocationLeft:
		MoveToEx(hDC,chartRect.left - m_nAxisGap, chartRect.bottom,NULL);
		LineTo(hDC,chartRect.left - m_nAxisGap, chartRect.top);
		break;
	case kLocationRight:
		MoveToEx(hDC,chartRect.right + m_nAxisGap, chartRect.bottom,NULL);
		LineTo(hDC,chartRect.right + m_nAxisGap, chartRect.top);
		break;
	case kLocationBottom:
		MoveToEx(hDC,chartRect.left, chartRect.bottom + m_nAxisGap,NULL);
		LineTo(hDC,chartRect.right, chartRect.bottom + m_nAxisGap);
		break;
	case kLocationTop:
		MoveToEx(hDC,chartRect.left, chartRect.top - m_nAxisGap, NULL);
		LineTo(hDC,chartRect.right, chartRect.top - m_nAxisGap );
		break;
	case kLocationCenterVL:
	case kLocationCenterVR:
		MoveToEx(hDC,(chartRect.left+chartRect.right)/2,chartRect.bottom,NULL);
		LineTo(hDC,(chartRect.left+chartRect.right)/2,chartRect.top);
	case kLocationCenterHB:
	case kLocationCenterHT:
		MoveToEx(hDC,chartRect.left,(chartRect.top+chartRect.bottom)/2,NULL);
		LineTo(hDC,chartRect.right,(chartRect.top+chartRect.bottom)/2);
		break;
	}
	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);
}


void CAxisImpl::DrawAxisArrows(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	RECT chartRect = GetPlot()->GetLastPlotRect();

	bool bX = IsXAxis() ;
	double pfRange[2];
	if(bX) m_pPlot->GetXRange(pfRange);
	else m_pPlot->GetYRange(pfRange);
	if( fabs( pfRange[1] - pfRange[0] ) <= fMinVal )return;

	if(m_bColorBar)
		chartRect = GetPseudoChartRect( );//chartRect);
	
	HPEN		hPen,hOldPen;
	HBRUSH		hBrush, hOldBrush;
	
	hPen = CreatePen( m_nLineStyle, m_nLineSize, m_crAxisColor );
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hBrush = CreateSolidBrush( m_crAxisColor );
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	POINT point;
	if(m_bArrowStart)
	{
		switch( GetLocation() )
		{
		case kLocationLeft:
			if(!m_bArrowReverse)
			{
				point.x = chartRect.left - m_nAxisGap;
				point.y = chartRect.bottom - m_nArrowRatio*m_nArrowSize;
			}
			else
			{
				point.x = chartRect.left - m_nAxisGap;
				point.y = chartRect.bottom;
			}
			goto yStart;
		case kLocationRight:
			if(!m_bArrowReverse)
			{
				point.x = chartRect.right + m_nAxisGap;
				point.y = chartRect.bottom - m_nArrowRatio*m_nArrowSize;
			}
			else
			{
				point.x = chartRect.right + m_nAxisGap;
				point.y = chartRect.bottom;
			}
			goto yStart;
		case kLocationCenterVL:
		case kLocationCenterVR:
			if(!m_bArrowReverse)
			{
				point.x = (chartRect.left + chartRect.right)/2;
				point.y = chartRect.bottom - m_nArrowRatio*m_nArrowSize;
			}
			else
			{
				point.x = (chartRect.left + chartRect.right)/2;
				point.y = chartRect.bottom;
			}
yStart:
			DrawArrow(hDC, point, false, hPen, hBrush, false);
			break;
		case kLocationBottom:
			if(!m_bArrowReverse)
			{
				point.x = chartRect.left + m_nArrowRatio*m_nArrowSize;
				point.y = chartRect.bottom + m_nAxisGap;
			}
			else
			{
				point.x = chartRect.left;
				point.y = chartRect.bottom + m_nAxisGap;
			}
			goto xStart;
		case kLocationTop:
			if(!m_bArrowReverse)
			{
				point.x = chartRect.left + m_nArrowRatio*m_nArrowSize;
				point.y = chartRect.top - m_nAxisGap;
			}
			else
			{
				point.x = chartRect.left;
				point.y = chartRect.top - m_nAxisGap;
			}
			goto xStart;
		case kLocationCenterHB:
		case kLocationCenterHT:
			if(!m_bArrowReverse)
			{
				point.x = chartRect.left + m_nArrowRatio*m_nArrowSize;
				point.y = (chartRect.bottom + chartRect.top)/2;
			}
			else
			{
				point.x = chartRect.left;
				point.y = (chartRect.bottom + chartRect.top)/2;
			}
xStart:
			DrawArrow(hDC, point, true, hPen, hBrush, false);
			break;
		default:
			break;
		}
	}

	if(m_bArrowEnd)
	{
		switch( GetLocation() )
		{
		case kLocationLeft:
			if(!m_bArrowReverse)
			{
				point.x = chartRect.left - m_nAxisGap;
				point.y = chartRect.top;
			}
			else
			{
				point.x = chartRect.left - m_nAxisGap;
				point.y = chartRect.top + m_nArrowRatio*m_nArrowSize;
			}
			goto yEnd;
		case kLocationRight:
			if(!m_bArrowReverse)
			{
				point.x = chartRect.right + m_nAxisGap;
				point.y = chartRect.top;
			}
			else
			{
				point.x = chartRect.right + m_nAxisGap;
				point.y = chartRect.top + m_nArrowRatio*m_nArrowSize;
			}
			goto yEnd;
		case kLocationCenterVL:
		case kLocationCenterVR:
			if(!m_bArrowReverse)
			{
				point.x = (chartRect.left + chartRect.right)/2;
				point.y = chartRect.top;
			}
			else
			{
				point.x = (chartRect.left + chartRect.right)/2;
				point.y = chartRect.top + m_nArrowRatio*m_nArrowSize;
			}
			
yEnd:
			DrawArrow(hDC, point, false, hPen, hBrush, true);
			break;
		case kLocationBottom:
			if(!m_bArrowReverse)
			{
				point.x = chartRect.right;
				point.y = chartRect.bottom + m_nAxisGap;
			}
			else
			{
				point.x = chartRect.right - m_nArrowRatio*m_nArrowSize;
				point.y = chartRect.bottom + m_nAxisGap;
			}
			goto xEnd;
		case kLocationTop:
			if(!m_bArrowReverse)
			{
				point.x = chartRect.right;
				point.y = chartRect.top - m_nAxisGap;
			}
			else
			{
				point.x = chartRect.right - m_nArrowRatio*m_nArrowSize;
				point.y = chartRect.top - m_nAxisGap;
			}
			goto xEnd;
		case kLocationCenterHB:
		case kLocationCenterHT:
			if(!m_bArrowReverse)
			{
				point.x = chartRect.right;
				point.y = (chartRect.bottom + chartRect.top)/2;
			}
			else
			{
				point.x = chartRect.right - m_nArrowRatio*m_nArrowSize;
				point.y = (chartRect.bottom + chartRect.top)/2;
			}
xEnd:
			DrawArrow(hDC, point, true, hPen, hBrush, true);
			break;
		default:
			break;
		}
	}

	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);
}


void CAxisImpl::DrawMajorTicks(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	RECT chartRect = GetPlot()->GetLastPlotRect();

	if(!m_bShowMajorTicks)return;

	bool bX = IsXAxis() ;

	if(m_bColorBar)
		chartRect = GetPseudoChartRect( );//chartRect);

	double position, range;

	HPEN		hPen,hOldPen;
	double		scale, interval, tickScale;
	int			i;
	long		startPoint;
	int			tickMarkLength;
	POINT		tickPoint;

	COLORREF	tickcolor;
	if(m_bSameColor)
	{
		tickcolor = m_crAxisColor;
	}
	else
	{
		tickcolor = m_crTickColor;
	}

	hPen = CreatePen( PS_SOLID, m_nTickSize, tickcolor );
	hOldPen = (HPEN)SelectObject(hDC,hPen);

	double pfRange[2];
	if(bX) m_pPlot->GetXRange(pfRange);
	else m_pPlot->GetYRange(pfRange);
	bool bFloat = (bX?m_pPlot->IsFloatXTicks():m_pPlot->IsFloatYTicks());
	double fTickMax = (bX?m_pPlot->GetXTickMax():m_pPlot->GetYTickMax());
	double fTickMin = (bX?m_pPlot->GetXTickMin():m_pPlot->GetYTickMin());

	if(!bFloat)
	{
		range = pfRange[1] - pfRange[0];
	}
	else
	{
		range = fTickMax - fTickMin;
	}
	
	tickMarkLength = m_nTickLength;

	int nTicks, nMinorTicks;
	if(bX)
	{
		nTicks = m_pPlot->GetXTickCount();
		nMinorTicks = m_pPlot->GetXMinorTickCount();
	}
	else
	{
		nTicks = m_pPlot->GetYTickCount();
		nMinorTicks = m_pPlot->GetYMinorTickCount();
	}
	
	interval = range/(nTicks+1); //GetMajorTickPosValue(1) - GetMajorTickPosValue(0);
	tickScale = interval /nMinorTicks;

	//then draw ticks and marks
	for( i = 0; i <= nTicks+1; i++ )
	{
		position = m_pPlot->GetMajorTickPosValue( bX?0:1, i );

		if(IsXAxis())
		{
			//startPoint = GetLocation() == kLocationBottom?chartRect.bottom:chartRect.top;
			if(GetLocation()==kLocationBottom)
				startPoint = chartRect.bottom + m_nAxisGap;
			else if(GetLocation()==kLocationTop)
				startPoint = chartRect.top - m_nAxisGap;
			else
				startPoint = (chartRect.bottom + chartRect.top)/2;
			scale = (double)abs(chartRect.right - chartRect.left) / (pfRange[1] - pfRange[0]);
			// Major Tick marks
			switch(m_nTickPosition)
			{
			case kTickPositionNone:
				goto NoMajorTick;
			case kTickPositionOuter:
				switch( GetLocation() )
				{
				case kLocationBottom:
				case kLocationCenterHB:
					tickPoint.y = startPoint - tickMarkLength;
					break;
				default:
					tickPoint.y = startPoint;
				}
				break;
			case kTickPositionInner:
				switch( GetLocation() )
				{
				case kLocationBottom:
				case kLocationCenterHB:
					tickPoint.y = startPoint;
					break;
				default:
					tickPoint.y = startPoint - tickMarkLength;
				}
				break;
			default:
				tickPoint.y = startPoint - tickMarkLength/2;
			}
			if(!m_pPlot->IsXOpposite())
				tickPoint.x = chartRect.left + (long)((position-pfRange[0]) * scale);
			else
				tickPoint.x = chartRect.right - (long)((position-pfRange[0]) * scale);
			MoveToEx(hDC, tickPoint.x, tickPoint.y, NULL );
			tickPoint.y += tickMarkLength;
			LineTo(hDC, tickPoint.x, tickPoint.y );
		}
		else
		{
			// Major Tick marks
			//startPoint = (GetLocation()==kLocationLeft?chartRect.left:chartRect.right);
			if(GetLocation()==kLocationLeft)
				startPoint = chartRect.left - m_nAxisGap;
			else if(GetLocation()==kLocationRight)
				startPoint = chartRect.right + m_nAxisGap;
			else
				startPoint = (chartRect.left + chartRect.right)/2;
			
			scale = fabs(double(chartRect.bottom - chartRect.top)) / (pfRange[1] - pfRange[0]);
			if(!m_pPlot->IsYOpposite())
				tickPoint.y = chartRect.bottom - (long)((position-pfRange[0]) * scale);
			else
				tickPoint.y = chartRect.top + (long)((position-pfRange[0]) * scale);
			switch( m_nTickPosition )
			{
			case kTickPositionNone:
				goto NoMajorTick;
			case kTickPositionOuter:
				switch( GetLocation() )
				{
				case kLocationLeft:
				case kLocationCenterVL:
					tickPoint.x = startPoint;
					break;
				default:
					tickPoint.x = startPoint - tickMarkLength;
				}
				break;
			case kTickPositionInner:
				switch( GetLocation() )
				{
				case kLocationLeft:
				case kLocationCenterVL:
					tickPoint.x = startPoint - tickMarkLength;
					break;
				default:
					tickPoint.x = startPoint;
				}
				break;
			default:
				tickPoint.x = startPoint - tickMarkLength/2;
			}
			MoveToEx(hDC, tickPoint.x, tickPoint.y, NULL );
			tickPoint.x += tickMarkLength;
			LineTo( hDC, tickPoint.x, tickPoint.y );
		}

	}
	
NoMajorTick:		
	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);
}


void CAxisImpl::DrawMinorTicks(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	RECT chartRect = GetPlot()->GetLastPlotRect();

	if(!m_bShowMinorTicks)return;

	if(m_bColorBar)
		chartRect = GetPseudoChartRect( );//chartRect);

	double position, range;
	
	HPEN		hPen,hOldPen;
	double		scale, interval,tickScale;
	int			i,j;
	long		startPoint;
	int			minorTickMarkLength;
	POINT		tickPoint;
	double		 *minorPos;
	
	COLORREF	minortickcolor;
	if(m_bSameColor)
	{
		minortickcolor = m_crAxisColor;
	}
	else
	{
		minortickcolor = m_crTickColor;
	}
	
	hPen = CreatePen( PS_SOLID, m_nMinorTickSize, minortickcolor );
	hOldPen = (HPEN)SelectObject(hDC,hPen);

	bool bX = IsXAxis() ;
	bool bFloat = (bX?m_pPlot->IsFloatXTicks():m_pPlot->IsFloatYTicks());

	double pfRange[2];
	if(bX) m_pPlot->GetXRange(pfRange);
	else m_pPlot->GetYRange(pfRange);
	double fTickMax = (bX?m_pPlot->GetXTickMax():m_pPlot->GetYTickMax());
	double fTickMin = (bX?m_pPlot->GetXTickMin():m_pPlot->GetYTickMin());
	int nTicks, nMinorTicks;
	if(bX)
	{
		nTicks = m_pPlot->GetXTickCount();
		nMinorTicks = m_pPlot->GetXMinorTickCount();
	}
	else
	{
		nTicks = m_pPlot->GetYTickCount();
		nMinorTicks = m_pPlot->GetYMinorTickCount();
	}
	
	if(!bFloat)
	{
		range = pfRange[1] - pfRange[0];
	}
	else
	{
		range =fTickMax - fTickMin;
	}
	minorTickMarkLength = m_nMinorTickLength;

	interval = range/(nTicks+1);;
	tickScale = interval / nMinorTicks;

	int startIdx,endIdx;
	if(!bFloat)
	{
		startIdx = 0;
		endIdx = nTicks + 1;
	}
	else
	{
		if(fabs(fTickMin-pfRange[0])<fMinVal)
		{
			startIdx = 0;
		}
		else
		{
			startIdx = -1;
		}
		if(fabs(fTickMax-pfRange[1])<fMinVal)
		{
			endIdx = nTicks + 1;
		}
		else
		{
			endIdx = nTicks + 2;
		}
	}
	
	minorPos = new double[nMinorTicks+1];
	//then draw ticks and marks
	for( i = startIdx; i <= endIdx; i++ )
	{
		position = m_pPlot->GetMajorTickPosValue( bX?0:1, i );

		if(IsXAxis())
		{
			if(GetLocation()==kLocationBottom)
				startPoint = chartRect.bottom + m_nAxisGap;
			else if(GetLocation()==kLocationTop)
				startPoint = chartRect.top - m_nAxisGap;
			else
				startPoint = (chartRect.bottom + chartRect.top)/2;
			//startPoint = GetLocation() == kLocationBottom?chartRect.bottom:chartRect.top;
			scale = (double)abs(chartRect.right - chartRect.left) / (pfRange[1] - pfRange[0]);
			
			// Minor tick marks
			//minorPos = new double[nMinorTicks+1];
			m_pPlot->GetMinorTickPosValue(bX?0:1, i, minorPos);
			
			for( j = 1; j < nMinorTicks; j++ )
			{
				if(minorPos[j] < pfRange[0] || minorPos[j] > pfRange[1])continue;
				//tickPoint.x = chartRect.left + (long)((position-m_Range[0]+j*tickScale) * scale);
				if(!m_pPlot->IsXOpposite())
					tickPoint.x = chartRect.left + (long)((minorPos[j] - pfRange[0]) * scale);
				else
					tickPoint.x = chartRect.right - (long)((minorPos[j] - pfRange[0]) * scale);
				switch( m_nMinorTickPosition )
				{
				case kTickPositionNone:
					goto NoMinorTick;
				case kTickPositionOuter:
					switch( GetLocation() )
					{
					case kLocationBottom:
					case kLocationCenterHB:
						tickPoint.y = startPoint - minorTickMarkLength;
						break;
					default:
						tickPoint.y = startPoint;
					}
					break;
				case kTickPositionInner:
					switch( GetLocation() )
					{
					case kLocationBottom:
					case kLocationCenterHB:
						tickPoint.y = startPoint;
						break;
					default:
						tickPoint.y = startPoint - minorTickMarkLength;
					}
					break;
				default:
					tickPoint.y = startPoint - minorTickMarkLength/2;
				}
				MoveToEx(hDC, tickPoint.x, tickPoint.y, NULL );
				tickPoint.y += minorTickMarkLength;
				LineTo(hDC, tickPoint.x, tickPoint.y );
			}
			//delete []minorPos;
		}
		else
		{
			// Major Tick marks
			//startPoint = (GetLocation()==kLocationLeft?chartRect.left:chartRect.right);
			if(GetLocation()==kLocationLeft)
				startPoint = chartRect.left - m_nAxisGap;
			else if(GetLocation()==kLocationRight)
				startPoint = chartRect.right + m_nAxisGap;
			else
				startPoint = (chartRect.left + chartRect.right)/2;
			
			scale = fabs(double(chartRect.bottom - chartRect.top)) / (pfRange[1] - pfRange[0]);
			// Minor tick marks
			//minorPos = new double[nMinorTicks+1];
			m_pPlot->GetMinorTickPosValue(bX?0:1, i, minorPos);
			
			for( j = 1; j < nMinorTicks; j++ )
			{
				if(minorPos[j] < pfRange[0] || minorPos[j] > pfRange[1])continue;
				//tickPoint.y = chartRect.bottom - (long)((position-m_Range[0]+j*tickScale) * scale);
				if(!m_pPlot->IsYOpposite())
					tickPoint.y = chartRect.bottom - (long)((minorPos[j] - pfRange[0]) * scale);
				else
					tickPoint.y = chartRect.top + (long)((minorPos[j] - pfRange[0]) * scale);
				switch( m_nMinorTickPosition )
				{
				case kTickPositionNone:
					goto NoMinorTick;
				case kTickPositionOuter:
					switch( GetLocation() )
					{
					case kLocationLeft:
					case kLocationCenterVL:
						tickPoint.x = startPoint;
						break;
					default:
						tickPoint.x = startPoint - minorTickMarkLength;
					}
					break;
				case kTickPositionInner:
					switch( GetLocation() )
					{
					case kLocationLeft:
					case kLocationCenterVL:
						tickPoint.x = startPoint - minorTickMarkLength;
						break;
					default:
						tickPoint.x = startPoint;
					}
					break;
				default:
					tickPoint.x = startPoint - minorTickMarkLength/2;
				}
				MoveToEx(hDC, tickPoint.x, tickPoint.y, NULL );
				tickPoint.x += minorTickMarkLength;
				LineTo(hDC, tickPoint.x, tickPoint.y );
			}
			//delete []minorPos;
		}
	}
	delete []minorPos;
NoMinorTick:
	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);
}


void CAxisImpl::DrawTickLabels(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	RECT chartRect = GetPlot()->GetLastPlotRect();

	if(!m_bTickLabelShow)return;

	if(m_bColorBar)
	{
		chartRect = GetPseudoChartRect( );//chartRect);
	}

	double position, range;
	
	HFONT		hFont,hOldFont;
	LOGFONT		logFont;
	RECT			textRect;
	SIZE			tmSize={0,0};
	unsigned int	textMode;
	int				maxTickLength;
	double		scale, interval,tickScale;
	int			i,sign;
	long		startPoint;
	POINT		tickPoint;
	int			tickMarkLength;
	tstring		tickLabel;
	SIZE		labelExtent={0,0};
	
	COLORREF	ticklabelcolor;
	if(m_bSameColor)
	{
		ticklabelcolor = m_crAxisColor;
	}
	else
	{
		ticklabelcolor = m_crTickLabelColor;
	}

	// the following functin is called in GetTitleRect
	// when no title, the following function do not execute, we cannot get a good ticklabelheight
	if(m_strTitle==_TEXT(""))
		GetRealTickLabelDimension(hDC);//, chartRect);
	
	// now ready
	logFont = m_lfLabelFont;
	logFont.lfHeight = m_nRealTickLabelHeight;

	hFont = CreateFontIndirect( &logFont );
	hOldFont = (HFONT)SelectObject(hDC,hFont);

	bool bX = IsXAxis() ;
	bool bFloat = (bX?m_pPlot->IsFloatXTicks():m_pPlot->IsFloatYTicks());
	
	double pfRange[2];
	if(bX) m_pPlot->GetXRange(pfRange);
	else m_pPlot->GetYRange(pfRange);
	double fTickMax = (bX?m_pPlot->GetXTickMax():m_pPlot->GetYTickMax());
	double fTickMin = (bX?m_pPlot->GetXTickMin():m_pPlot->GetYTickMin());
	int nTicks, nMinorTicks;
	if(bX)
	{
		nTicks = m_pPlot->GetXTickCount();
		nMinorTicks = m_pPlot->GetXMinorTickCount();
	}
	else
	{
		nTicks = m_pPlot->GetYTickCount();
		nMinorTicks = m_pPlot->GetYMinorTickCount();
	}
	
	range = pfRange[1] - pfRange[0];
	tickMarkLength = GetMaxTickLength();//m_nTickLength;
	
	// Setup of ranges, pens
	int nOldBkMode = SetBkMode(hDC, TRANSPARENT );
	COLORREF crOldColor = SetTextColor(hDC, ticklabelcolor );

	if(IsXAxis())
	{
		textMode = DT_NOCLIP | DT_CENTER ;
		if(GetLocation() == kLocationBottom || GetLocation() == kLocationCenterHB)
			textMode |= DT_TOP;
		else
			textMode |= DT_BOTTOM;
	}
	else
	{
		textMode = DT_NOCLIP | DT_VCENTER;
		//if(GetLocation() == kLocationRight || GetLocation() == kLocationCenterVR)
		//	textMode |= DT_LEFT;
		//else
		//	textMode |= DT_RIGHT;
		textMode |= DT_CENTER;
		// Don't calculate tickHeight here because it is calculated later
	}
	
	// For moving our labels and titles, we need to be away
	// from the longest tick mark
	maxTickLength = GetMaxTickLength();

	// Do we add or subtract distance to move labels?
	if( GetLocation() == kLocationBottom || GetLocation() == kLocationRight
		|| m_nLocation == kLocationCenterHB || m_nLocation == kLocationCenterVR)
		sign = -1; else sign = 1;
	
	if(!bX)
	{
		interval = m_pPlot->GetMajorTickPosValue(bX?0:1, 1) - m_pPlot->GetMajorTickPosValue(bX?0:1, 0);
		tickScale = interval / nMinorTicks;
	}
	else
	{
		interval = (fTickMax - fTickMin)/(nTicks-1);
		tickScale = interval / nMinorTicks;
	}
	
	//then draw ticks and marks
	for( i = 0; i <= nTicks+1; i++ )
	{
		if(m_bTickLabelSEOnly)
		{
			if( (i != 0) && (i != (bX?m_pPlot->GetXTickCount():m_pPlot->GetYTickCount())+1))continue;
		}
		
		position = m_pPlot->GetMajorTickPosValue( bX?0:1, i );

		// Hide this label
		if(CheckLabelHiden(position))continue;

		if(IsXAxis())
		{
			//startPoint = (GetLocation() == kLocationBottom || GetLocation() == kLocationCenterHB)?chartRect.bottom:chartRect.top;
			if(GetLocation() == kLocationBottom)
				startPoint = chartRect.bottom + m_nAxisGap;
			else if(GetLocation() == kLocationTop)
				startPoint = chartRect.top - m_nAxisGap;
			else
				startPoint = (chartRect.bottom + chartRect.top)/2;
			
			scale = (double)abs(chartRect.right - chartRect.left) / range;
			
			// Tick labels
			if(!m_pPlot->IsXOpposite())
				tickPoint.x = chartRect.left + (long)((position-pfRange[0]) * scale);
			else
				tickPoint.x = chartRect.right - (long)((position-pfRange[0]) * scale);
			//tickPoint.y = startPoint - sign*tickMarkLength/2;
			tickPoint.y = startPoint - sign*(tickMarkLength + m_nLabelGap);
			
			tickLabel = GetMajorTickLabel( i );
			//
			MyGetTextExtentPoint(hDC, &logFont, tickLabel, 0, &labelExtent);
			textRect.left = tickPoint.x - labelExtent.cx/2;
			textRect.right = tickPoint.x + labelExtent.cx/2;
			
			if(GetLocation() == kLocationBottom || GetLocation() == kLocationCenterHB)
			{
				textRect.top = tickPoint.y;
				textRect.bottom = textRect.top + labelExtent.cy;
				
				if(IsTwoStoreyX() && i%2==1)
					OffsetRect(&textRect, 0, labelExtent.cy);
			}
			else
			{
				textRect.bottom = tickPoint.y;
				textRect.top = textRect.bottom - labelExtent.cy;
				
				if(IsTwoStoreyX()  && i%2==1)
					OffsetRect(&textRect, 0, -labelExtent.cy);
			}
			OffsetRect(&textRect, m_sizeLabelOffset.cx, m_sizeLabelOffset.cy);// Adding offset
			
			//
			DrawText(hDC, tickLabel.c_str(), (int)tickLabel.length(), (LPRECT)&textRect, textMode );

		}
		else
		{
			// Major Tick marks
			//startPoint = ((GetLocation()==kLocationLeft || GetLocation()==kLocationCenterVL)?chartRect.left:chartRect.right);
			if(GetLocation() == kLocationLeft)
				startPoint = chartRect.left - m_nAxisGap;
			else if(GetLocation() == kLocationRight)
				startPoint = chartRect.right + m_nAxisGap;
			else
				startPoint = (chartRect.left + chartRect.right)/2;
			
			scale = fabs(double(chartRect.bottom - chartRect.top)) / range;
			
			// Tick labels
			if(!m_pPlot->IsYOpposite())
				tickPoint.y = chartRect.bottom - (long)((position-pfRange[0]) * scale);
			else
				tickPoint.y = chartRect.top + (long)((position-pfRange[0]) * scale);
			//tickPoint.x = startPoint - sign*tickMarkLength/2;
			tickPoint.x = startPoint - sign*(tickMarkLength + m_nLabelGap);
			tickLabel = GetMajorTickLabel( i );
			//TEST
			TCHAR *str = (TCHAR *)tickLabel.c_str();
			//
			MyGetTextExtentPoint(hDC, &logFont, tickLabel, 0, &labelExtent);
			textRect.top = tickPoint.y - labelExtent.cy/2;
			textRect.bottom = tickPoint.y + labelExtent.cy/2;
			
			if(GetLocation() == kLocationLeft || GetLocation() == kLocationCenterVL)
			{
				textRect.right = tickPoint.x;
				textRect.left = textRect.right - labelExtent.cx;
			}
			else
			{
				textRect.left = tickPoint.x;
				textRect.right = textRect.left + labelExtent.cx;
			}

			if(m_bSciNotation && GetPowerMag()!=0)
			{
				if(m_nLocation == kLocationLeft || m_nLocation == kLocationCenterVL)
				{
					SIZE sizeSN = GetSizeSN(hDC);
					OffsetRect(&textRect, -sizeSN.cx, 0);
				}
				
			}

			OffsetRect(&textRect, m_sizeLabelOffset.cx, m_sizeLabelOffset.cy);// Adding offset
			
			DrawText(hDC, tickLabel.c_str(), (int)tickLabel.length(), (LPRECT)&textRect, textMode );

		}
	}
	
	SetBkMode(hDC, nOldBkMode);
	SetTextColor(hDC, crOldColor);
	SelectObject( hDC, hOldFont);
	DeleteObject( hFont );
}


void	CAxisImpl::DrawColorBar(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	if(!m_bColorBar)return;

	RECT barRect;
	if(IsXAxis())
	{
		barRect = GetBarRect(hDC);//, clientRect, chartRect);
		GradientRect(hDC, barRect, kGradientL2R, m_crBarColor1, m_crBarColor2);
	}
	else
	{
		barRect = GetBarRect(hDC);//, clientRect, chartRect);
		GradientRect(hDC, barRect, kGradientT2B, m_crBarColor2, m_crBarColor1);
	}
}


void	CAxisImpl::DrawArrow(HDC hDC, POINT point, bool bHor, HPEN hPen, HBRUSH hBrush, bool bEnd)
{
	POINT	pPt[4];
	HRGN	hRgn;

	int		r = m_nArrowRatio;
	if(!bHor)
	{
		pPt[0].x = point.x;
		pPt[0].y = point.y;

		pPt[1].x = point.x - m_nArrowSize;
		if(!m_bArrowReverse)
			pPt[1].y = point.y + r*m_nArrowSize;
		else
			pPt[1].y = point.y - r*m_nArrowSize;
		
		pPt[2].x = point.x;
		if(m_bArrowFlatBottom)
		{
			if(!m_bArrowReverse)
				pPt[2].y = point.y + r*m_nArrowSize;
			else
				pPt[2].y = point.y - r*m_nArrowSize; 
		}
		else
		{
			if(!m_bArrowReverse)
				pPt[2].y = point.y + (r-1)*m_nArrowSize;
			else
				pPt[2].y = point.y - (r-1)*m_nArrowSize;
		}
		
		pPt[3].x = point.x + m_nArrowSize;
		if(!m_bArrowReverse)
			pPt[3].y = point.y + r*m_nArrowSize;
		else
			pPt[3].y = point.y - r*m_nArrowSize;
	}
	else
	{
		pPt[0].x = point.x;
		pPt[0].y = point.y;
		
		if(!m_bArrowReverse)
			pPt[1].x = point.x - r*m_nArrowSize;
		else
			pPt[1].x = point.x + r*m_nArrowSize;
		pPt[1].y = point.y - m_nArrowSize;
		
		if(m_bArrowFlatBottom)
		{
			if(!m_bArrowReverse)
				pPt[2].x = point.x - r*m_nArrowSize;
			else
				pPt[2].x = point.x + r*m_nArrowSize;
		}
		else
		{
			if(!m_bArrowReverse)
				pPt[2].x = point.x - (r-1)*m_nArrowSize;
			else
				pPt[2].x = point.x + (r-1)*m_nArrowSize;
		}
		pPt[2].y = point.y;
		
		if(!m_bArrowReverse)
			pPt[3].x = point.x - r*m_nArrowSize;
		else
			pPt[3].x = point.x + r*m_nArrowSize;
		pPt[3].y = point.y + m_nArrowSize;
	}
	hRgn = CreatePolygonRgn(pPt, 4, ALTERNATE);
	FillRgn(hDC, hRgn, hBrush);
	DeleteObject(hRgn);
}


void	CAxisImpl::DrawSNPower(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	if(!m_bSciNotation || GetPowerMag()==0 || !m_bTickLabelShow)return;

	RECT		snRect, saRect, spRect;
	
	SIZE sizeSA = GetSizeAntilog(hDC);
	SIZE sizeSP = GetSizePower(hDC);

	snRect = GetSNRect(hDC);//, clientRect, chartRect);
	
	saRect = snRect;
	saRect.right -= sizeSP.cx;

	spRect = snRect;
	spRect.left += sizeSA.cx;
	spRect.bottom = spRect.top + sizeSP.cy;

	LOGFONT			logFont = m_lfLabelFont;
	//logFont.lfItalic = 1;
	logFont.lfWeight = FW_BOLD;
	logFont.lfHeight = m_nRealTickLabelHeight;


	int nOldBkMode = SetBkMode(hDC, TRANSPARENT );
	COLORREF cr = m_crTickLabelColor;
	COLORREF crOldColor = SetTextColor(hDC, RGB(GetRValue(cr), 255-GetGValue(cr), 255-GetBValue(cr) ));
	
	int textMode;
	if(IsXAxis())
	{
		textMode = DT_NOCLIP | DT_CENTER ;
		if(GetLocation() == kLocationBottom || GetLocation() == kLocationCenterHB)
			textMode |= DT_TOP;
		else
			textMode |= DT_BOTTOM;
	}
	else
	{
		textMode = DT_NOCLIP | DT_VCENTER | DT_SINGLELINE;
		if(GetLocation() == kLocationRight || GetLocation() == kLocationCenterVR)
			textMode |= DT_LEFT;
		else
			textMode |= DT_RIGHT;
		// Don't calculate tickHeight here because it is calculated later
	}

	HFONT hFont, hOldFont;
	hFont = CreateFontIndirect( &logFont );
	hOldFont = (HFONT)SelectObject(hDC,hFont);

	tstring strSA = _T("x10");
	DrawText(hDC, strSA.c_str(), (int)strSA.length(), (LPRECT)&saRect, textMode );

	DeleteObject(hFont);
	logFont.lfHeight = long(logFont.lfHeight*m_fSnFontRatio);
	hFont = CreateFontIndirect( &logFont );
	SelectObject(hDC, hFont);

	tostringstream tostr;
	tstring strSP;
	tostr<<GetPowerMag();
	strSP = tostr.str();
	DrawText(hDC, strSP.c_str(), (int)strSP.length(), (LPRECT)&spRect, textMode );

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);

	
	SetBkMode(hDC, nOldBkMode );
	SetTextColor(hDC, crOldColor );

	//FrameRect(hDC, &saRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	//FrameRect(hDC, &spRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
}

/*
tstring	CAxisImpl::GetTimeString(time_t theTime)
{
	TCHAR strLabel[32];
	struct tm *myTm;
	
#if defined(_MSC_VER) && (_MSC_VER>=1300)
	myTm = new struct tm;
	localtime_s(myTm, &theTime);
#else
	myTm = localtime(&theTime);
#endif
	
	memset(strLabel, 0, sizeof(TCHAR)*32);
	_tcsftime(strLabel, 31, m_strTimeFormat.c_str(), myTm);

#if defined(_MSC_VER) && (_MSC_VER>=1300)
	delete myTm;
#endif
	return strLabel;
}*/

// Returns the rectangle, in the current coordinate space,
// that the axis will take up. Used to adjust the size
// of the main chart when an axis is near.

SIZE	CAxisImpl::GetMinDisplaySize( HDC hDC )
{
	SIZE		minSize={0,0};
	int			i;

	// We have to calculate the height (or width) of the tick marks,
	// the fonts (titles, if any) and the labels
	//if( m_strTitle != "" )
	if( m_bShowTitle && m_strTitle != _TEXT("") )//  && m_nTitlePosition==0)
	{
		MyGetTextExtentPoint(hDC, &m_lfTitleFont, m_strTitle, 0, &minSize);

		// for y and title not verticle
		if(!IsXAxis() && !m_bYTitleVerticle)
			swap(minSize.cx, minSize.cy);

		// add gap of title and labels
		minSize.cy += m_nTitleGap;
	}
	
	bool bX = IsXAxis();
	int	nTicks = (bX?m_pPlot->GetXTickCount():m_pPlot->GetYTickCount());

	if(m_bTickLabelShow)
	{
		RECT chartRect = GetPlot()->GetLastPlotRect();
		LOGFONT logFont = m_lfLabelFont;
		if(Width(chartRect)>0 && Height(chartRect)>0)
		{
			tstring tickLabel;
			SIZE labelExtent;
			bool keepGoing = true;
			while( keepGoing == true )
			{
				int nMaxLabelWidth=0;
				for(i=0; i<nTicks+2; i++)
				{
					tickLabel=GetMajorTickLabel(i);
					//GetTextExtentPoint(hDC, tickLabel.c_str(),tickLabel.length(),&labelExtent );
					MyGetTextExtentPoint(hDC, &logFont, tickLabel, 0, &labelExtent );
					if(labelExtent.cx>nMaxLabelWidth)nMaxLabelWidth=labelExtent.cx;
				}
				if(bX)
				{
					if( nMaxLabelWidth > (m_bTwoStoreyX?2:1)*abs(chartRect.right - chartRect.left) / (nTicks<0?(nTicks+2):(nTicks+1)) && logFont.lfHeight > 4 )
						logFont.lfHeight--;
					else
						keepGoing = false;
				}
				else
				{
					if( labelExtent.cy > abs(chartRect.bottom - chartRect.top) / (nTicks<0?(nTicks+2):(nTicks+1)) && logFont.lfHeight > 4  )
						logFont.lfHeight--;
					else
						keepGoing = false;
				}
			}
		}
		
		//the aboves is adding

		SIZE		temp={0,0}, temp2={0,0};
		tstring strLabel;
		for( i = 0; i <= nTicks+1; i++ )
		{
			strLabel = GetMajorTickLabel(i);
			//MyGetTextExtentPoint(hDC, &m_lfLabelFont, strLabel, 0,&temp);
			MyGetTextExtentPoint(hDC, &logFont, strLabel, 0,&temp);
			
			// special
			if(bX && IsTwoStoreyX())temp.cy*=2;
			
			if( temp.cx > temp2.cx ) temp2.cx = temp.cx;
			if( temp.cy > temp2.cy ) temp2.cy = temp.cy;
		}

		// Some approximate spacings
		if(!bX)
		{ minSize.cy += temp2.cx + m_nMarginVertical; minSize.cx += temp2.cy + m_nMarginVertical; }
		else
		{ minSize.cx += temp2.cx + m_nMarginHorizontal; minSize.cy += temp2.cy + m_nMarginHorizontal; }

		if(m_bSciNotation && GetPowerMag()!=0)
		{
			if(!bX)
			{
				SIZE sizeSN = GetSizeSN(hDC);
				minSize.cy += sizeSN.cx;
			}
		}
	}

	if(m_bColorBar)
	{
		minSize.cy += m_nColorBarWidth;
	}
	
	// Finally the tick marks
	//minSize.cy += max(m_nMinorTickLength, m_nTickLength)/2;//Infact, this line should be changed with the tick position
	minSize.cy += GetMaxTickLength();
	minSize.cy += m_nLabelGap;

	// If we're left or right, switch x and y
//	if( m_nLocation == kLocationLeft || m_nLocation == kLocationRight
//		|| m_nLocation == kLocationCenterVL || m_nLocation == kLocationCenterVR)
	if(!bX)
	{
		SIZE	temp={minSize.cy,minSize.cx};
		return temp;
	}
	else
	{
		return minSize;
	}
}

// These are always returned axis-specific
// so for bottom  = left, right,
// for right = bottom pad, top pad
// for top = left, right
// for left = bottom, top

// This function looks at the first and last tick mark of
// an axis to see how much it extends over the edge of the
// chart. It assumes that the first and last tick marks
// go on the end of the axis. This may change in the future.

SIZE	CAxisImpl::GetPaddingSize( HDC hDC, RECT chartRect )
{
	SIZE		temp1={0,0}, temp2={0,0};
	int			i;
//	TEXTMETRIC	tm;
	bool bX = IsXAxis();
	int	nTicks = (bX?m_pPlot->GetXTickCount():m_pPlot->GetYTickCount());

	if( nTicks > 0 )
	{
//		GetTextMetrics(hDC,&tm);
		tstring strLabel;
		// Get info for first and last tick marks
		for( i = 0; i <= nTicks+1; i+= nTicks+1 )
		{
			strLabel = GetMajorTickLabel(i);
			//GetTextExtentPoint(hDC,strLabel.c_str(),strLabel.length(),&temp1);
			MyGetTextExtentPoint(hDC, &m_lfLabelFont, strLabel, 0,&temp1);
			if(IsXAxis())
			{
				if( i == 0 ) temp2.cx = temp1.cx / 2; else temp2.cy = temp1.cx / 2;
			}
			else
			{
				if( i == 0 ) temp2.cx = temp1.cy / 2; else temp2.cy = temp1.cy / 2;
			}
		}
	}

	if(m_bSciNotation && GetPowerMag() != 0 && m_bTickLabelShow)
	{
		SIZE sizeSA = GetSizeAntilog(hDC);
		SIZE sizeSP = GetSizePower(hDC);
		if(IsXAxis())
		{
			temp2.cy += (sizeSA.cx + sizeSP.cx);
		}
//		else
//		{
//			temp2.cy += sizeSA.cy;
//		}
	}

	return temp2;
}

SIZE	CAxisImpl::GetPaddingSize( HDC hDC )
{
	return GetPaddingSize( hDC, GetPlot()->GetLastPlotRect());
}

SIZE	CAxisImpl::GetPaddingSizeForFloatTicks( HDC hDC, RECT chartRect )
{
	//RECT chartRect = GetPlot()->GetLastPlotRect();
	//first time
	if(Width(chartRect)==0 || Height(chartRect)==0)
		chartRect = GetPlot()->GetLastClientRect();

	bool bX = IsXAxis() ;
	bool bFloat = (bX?m_pPlot->IsFloatXTicks():m_pPlot->IsFloatYTicks());
	
	double pfRange[2];
	if(bX) m_pPlot->GetXRange(pfRange);
	else m_pPlot->GetYRange(pfRange);
	double fTickMax = (bX?m_pPlot->GetXTickMax():m_pPlot->GetYTickMax());
	double fTickMin = (bX?m_pPlot->GetXTickMin():m_pPlot->GetYTickMin());

	SIZE paddingSize = GetPaddingSize(hDC, chartRect);
	if(!bFloat)return paddingSize;

	double scale;
	double leftLow, leftHigh;

	if(bX)
	{
		scale = (double)abs(chartRect.right - chartRect.left) / (pfRange[1] - pfRange[0]);
	}
	else
	{
		scale = (double)abs(chartRect.bottom - chartRect.top) / (pfRange[1] - pfRange[0]);
	}

	leftLow = (fTickMin - pfRange[0])*scale;
	leftHigh = (pfRange[1] - fTickMax)*scale;
	
	if(paddingSize.cx < leftLow)
		paddingSize.cx = 0;
	else
		paddingSize.cx -= (long)leftLow;
	
	if(paddingSize.cy < leftHigh)
		paddingSize.cy = 0;
	else
		paddingSize.cy -= (long)leftHigh;

	return paddingSize;
}


SIZE	CAxisImpl::GetSize( HDC hDC )
{
	bool bX = IsXAxis() ;
	bool bFloat = (bX?m_pPlot->IsFloatXTicks():m_pPlot->IsFloatYTicks());
	
	double pfRange[2];
	if(bX) m_pPlot->GetXRange(pfRange);
	else m_pPlot->GetYRange(pfRange);
	double fTickMax = (bX?m_pPlot->GetXTickMax():m_pPlot->GetYTickMax());
	double fTickMin = (bX?m_pPlot->GetXTickMin():m_pPlot->GetYTickMin());

	SIZE theSize;
	SIZE axisDims, paddingDims;
	axisDims = GetMinDisplaySize( hDC );
	if(!bFloat)
		paddingDims = GetPaddingSize( hDC, m_pPlot->GetLastPlotRect());
	else
		paddingDims = GetPaddingSizeForFloatTicks(hDC, m_pPlot->GetLastPlotRect());

	if(bX)
	{
		theSize.cx = axisDims.cx + paddingDims.cx + paddingDims.cy;
		theSize.cy = axisDims.cy;
	}
	else
	{
		theSize.cx = axisDims.cx;
		theSize.cy = axisDims.cy + paddingDims.cx + paddingDims.cy;
	}

	return theSize;
}


bool		CAxisImpl::CheckLabelHiden(double val)
{
	for(int i=0; i<(int)m_vRanges4LabelHiden.size(); i++)
	{
		if((val - m_vRanges4LabelHiden[i].val[0])*(m_vRanges4LabelHiden[i].val[1] - val)>=0)
		{
			return true;
		}
	}
	return false;
}

void		CAxisImpl::ApplyMapAlternated()
{
	if(!m_bMapAlternateMode)return;
	if(!m_bReplaceLabel)return;
	double gap;
	bool bX = IsXAxis();
	if(bX)
	{
		if(!GetPlot()->IsFloatXTicks())return;
		if(!GetPlot()->IsXGapDetermined())return;
		gap = GetPlot()->GetXTickGap();
	}
	else
	{
		if(!GetPlot()->IsFloatYTicks())return;
		if(!GetPlot()->IsYGapDetermined())return;
		gap = GetPlot()->GetYTickGap();
	}
	if(gap<=0)return;
	
	m_vstrAlternateLabels.clear();

	int i, n;
	double tickVal;
	tstring tickLabel;
	int tickCnt;
	tickCnt = bX?m_pPlot->GetXTickCount():m_pPlot->GetYTickCount();
	for(i=0; i<=tickCnt+1; i++)
	{
		tickVal = m_pPlot->GetMajorTickPosValue(bX?0:1, i);
		n = (tickVal - m_fMapAlternateStartVal)/gap;
		tickLabel = GetAlternateMap(n);

		m_vstrAlternateLabels.push_back(tickLabel);
	}
}

// Gets the label string for the requested tick mark.
// Useful in future if want to add user-specified
// tick labels.

tstring		CAxisImpl::GetMajorTickLabel( int whichTick )
{
	bool bX = IsXAxis() ;
	bool bLog = (bX?m_pPlot->IsXLogarithm():m_pPlot->IsYLogarithm());

	tstring			tickLabel;
	
	if(!m_bReplaceLabel)
	{
		double			value = m_pPlot->GetMajorTickPosValue( bX?0:1, whichTick );
		if(m_bMinusTicks)value*=-1.0;
		if(m_bSciNotation)
		{
			if(!bLog)value /= pow(10.0, GetPowerMag());
			else value -= GetPowerMag();
		}

		if(!m_bTime)
		{
			TCHAR tstr[64];
			if(!bLog)
			{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
				_stprintf_s(tstr, 64, m_strLabelFormat.c_str(), value);
#else
				_stprintf(tstr, m_strLabelFormat.c_str(), value);
#endif
			}
			else
			{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
				_stprintf_s(tstr, 64, m_strLabelFormat.c_str(), pow(10.0, value));
#else
				_stprintf(tstr, m_strLabelFormat.c_str(), pow(10.0, value));
#endif
			}
			tickLabel = tstr;
		}
		else
		{
			//for safety
			if(value<0.0)value=0.0;
			//tickLabel = GetTimeString((time_t)value);
			MyTime2Str((time_t)value, tickLabel, m_strTimeFormat);
		}
	}
	else
	{
		if( whichTick < (int)m_vstrAlternateLabels.size() )
			tickLabel = m_vstrAlternateLabels[whichTick];
		else
			tickLabel = _TEXT("");
	}

	// add the label suffix
	if(tickLabel != _TEXT("") && m_strLabelPrefix != _TEXT(""))
		tickLabel = m_strLabelPrefix + tickLabel;
	if(tickLabel != _TEXT("") && m_strLabelSuffix != _TEXT(""))
		tickLabel += m_strLabelSuffix;

	return tickLabel;
}


SIZE		CAxisImpl::GetSizeAntilog(HDC hDC)
{
	LOGFONT			logFont = m_lfLabelFont;
	//logFont.lfItalic = 1;
	logFont.lfWeight = FW_BOLD;
	logFont.lfHeight = m_nRealTickLabelHeight;

	tstring			strAntilog;
	SIZE			sizeAntilog={0,0};

	strAntilog = _TEXT("x10");
	MyGetTextExtentPoint(hDC, &logFont, strAntilog, 0, &sizeAntilog );

	return sizeAntilog;
}


SIZE		CAxisImpl::GetSizePower(HDC hDC)
{
	LOGFONT			logFont = m_lfLabelFont;
	//logFont.lfItalic = 1;
	logFont.lfWeight = FW_BOLD;
	logFont.lfHeight = m_nRealTickLabelHeight;
	
	
	tostringstream	tostr;
	tstring			strPower;
	SIZE			sizePower={0,0};
	
	tostr<<GetPowerMag();
	strPower = tostr.str();
	logFont.lfHeight = long(logFont.lfHeight*m_fSnFontRatio);
	MyGetTextExtentPoint(hDC, &logFont, strPower, 0, &sizePower );
	
	return sizePower;
}


SIZE		CAxisImpl::GetSizeSN(HDC hDC)
{
	SIZE		snSize;
	
	SIZE sizeSA = GetSizeAntilog(hDC);
	SIZE sizeSP = GetSizePower(hDC);
	
	snSize.cx = sizeSA.cx + sizeSP.cx;
	snSize.cy = sizeSA.cy;

	return snSize;
}


RECT		CAxisImpl::GetSNRect(HDC hDC)//, RECT clientRect, RECT chartRect)
{
	RECT clientRect = GetPlot()->GetLastClientRect();
	RECT chartRect = GetPlot()->GetLastPlotRect();

	bool bX = IsXAxis();
	bool bFloat = (bX?m_pPlot->IsFloatXTicks():m_pPlot->IsFloatYTicks());
	int sign;
	if( GetLocation() == kLocationBottom || GetLocation() == kLocationRight
		|| m_nLocation == kLocationCenterHB || m_nLocation == kLocationCenterVR)
		sign = -1;
	else
		sign = 1;
	long		startPoint;
	POINT		tickPoint;

	int			tickMarkLength = GetMaxTickLength();

	SIZE		paddingSize;
	if(!bFloat)
		paddingSize = GetPaddingSize( hDC, chartRect );
	else
		paddingSize = GetPaddingSizeForFloatTicks(hDC, chartRect);

	RECT		snRect;
	SIZE		snSize;

	snSize = GetSizeSN(hDC);

	if(IsXAxis())
	{
		if(GetLocation() == kLocationBottom)
			startPoint = chartRect.bottom + m_nAxisGap;
		else if(GetLocation() == kLocationTop)
			startPoint = chartRect.top - m_nAxisGap;
		else
			startPoint = (chartRect.bottom + chartRect.top)/2;
		
		tickPoint.y = startPoint - sign*(tickMarkLength + m_nLabelGap);

		snRect.right = chartRect.right + paddingSize.cy;
		snRect.left = snRect.right - snSize.cx;

		if(GetLocation() == kLocationBottom || GetLocation() == kLocationCenterHB)
		{
			snRect.top = tickPoint.y;
			if(m_bColorBar)snRect.top += m_nColorBarWidth;
			snRect.bottom = snRect.top + snSize.cy;
			
		}
		else if(GetLocation() == kLocationTop || GetLocation() == kLocationCenterHT)
		{
			snRect.bottom = tickPoint.y;
			if(m_bColorBar)snRect.bottom -= m_nColorBarWidth;
			snRect.top = snRect.bottom - snSize.cy;
		}
	}
	else
	{
		if(GetLocation() == kLocationLeft)
			startPoint = chartRect.left - m_nAxisGap;
		else if(GetLocation() == kLocationRight)
			startPoint = chartRect.right + m_nAxisGap;
		else
			startPoint = (chartRect.left + chartRect.right)/2;
		
		//tickPoint.x = startPoint - sign*tickMarkLength/2;
		tickPoint.x = startPoint - sign*(tickMarkLength + m_nLabelGap);

		snRect.top = chartRect.top - paddingSize.cy;
		snRect.bottom = snRect.top + snSize.cy;
		
		if(GetLocation() == kLocationLeft || GetLocation() == kLocationCenterVL)
		{
			snRect.right = tickPoint.x;
			if(m_bColorBar)snRect.right -= m_nColorBarWidth;
			snRect.left = snRect.right - snSize.cx;
			if(snRect.left < clientRect.left)
				OffsetRect(&snRect, clientRect.left-snRect.left, 0);
		}
		else
		{
			snRect.left = tickPoint.x;
			SIZE tickLabelSize = GetRealTickLabelDimension(hDC);//, chartRect);
			snRect.left += tickLabelSize.cx;
			if(m_bColorBar)snRect.left += m_nColorBarWidth;

			snRect.right = snRect.left + snSize.cx;
			if(snRect.right > clientRect.right)
				OffsetRect(&snRect, clientRect.right-snRect.right, 0);
		}
	}
	return snRect;
}


int		CAxisImpl::GetPowerMag()
{
	bool bX = IsXAxis();
	double pfRange[2];
	if(bX) m_pPlot->GetXRange(pfRange);
	else m_pPlot->GetYRange(pfRange);
	return GetMagnitude((pfRange[1]-pfRange[0])/9.0);
}

